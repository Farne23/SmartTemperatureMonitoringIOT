package communicationSerial;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;

import java.util.Map;

import communicationSerial.api.*;

public class WindowControlCommunicationClient extends AbstractVerticle{
    //Message lines to receive from.
	private static final String UPDATE_TEMPERATURE_LINE = "update.temperature.line";
	private static final String UPDATE_CONTROL_MODE = "update.control.mode.line";
	private static final String UPDATE_OPENING_LEVEL = "update.opening.level.line";
    // Message lines to send data to.
    private static final String WINDOW_LEVEL_CHANGE = "window.level.change";
    private static final String CONTROL_MODE_SWITCH = "controlmode.switch";
    // Port  and baudrate for serial communication.
    private static final String PORT = "COM3";
    private static final int BAUD_RATE = 9600;
    private static final long DELAY = 150;
    private static final String SWITCH_MSG = "S";
    private static final int TO_PERC = 100;

    private CommChannel serial;

    public void start() {
        // Creation of the serial line.
        try {
            this.serial = new SerialCommChannel(PORT, BAUD_RATE);
        }
        catch (Exception e)
        {
            log("Serial line not found");
        }
        log("Connected succesfully to serial line");

        //Handler for messages sent by the dashboard to change opening
        // level.
        vertx.eventBus().consumer(UPDATE_OPENING_LEVEL, message -> {
            log("Messaggio ricevuto: " + message.body() + "%");
            JsonObject body = (JsonObject) message.body();
            serial.sendMsg("P" + Integer.toString(body.getInteger("openingLevel")));
            log("Sending: P" + body.getInteger("openingLevel"));
        });

        //Handler for messages sent by the dashboard to switch mode
        vertx.eventBus().consumer(UPDATE_CONTROL_MODE, message -> {
            log("Messaggio ricevuto: switch to" + message.body());
            JsonObject body = (JsonObject) message.body();
            serial.sendMsg("M" + body.getString("controlMode"));
            log("Sending: M" + body.getString("controlMode"));
        });

        // Handler for messages sent by the ESP to update temperature
        vertx.eventBus().consumer(UPDATE_TEMPERATURE_LINE, message -> {
            log("Messaggio ricevuto: " + message.body() + "°C");
            JsonObject body = (JsonObject) message.body();
            serial.sendMsg("T" + String.format("%.2f", body.getDouble("temperature")));
            log("Sending: T" + String.format("%.2f", body.getDouble("temperature")));
        });
        // Sets a periodic handler to read from the serial line,
        // process the message once every DELAY ms.
        vertx.setPeriodic(DELAY, id -> {
            String msg = "";
            if (this.serial.isMsgAvailable()) {
                try {
                    msg = this.serial.receiveMsg();
                } catch (Exception e) {}
            }
            // if msg is not empty
            if (!msg.isEmpty()) {
                parseMessage(msg);
            }
        });
    }

    private void parseMessage(String msg) {
        if (msg.equals(SWITCH_MSG)) {
            sendSwitch();
        }
        else {
            // convert the level form [1, 0] to [100, 0]
            sendOpenLv(Double.parseDouble(msg) * TO_PERC);
        }
    }

    private void sendOpenLv(double lv) {
        // payload with the integer "level" field
        vertx.eventBus().publish(WINDOW_LEVEL_CHANGE, new JsonObject(Map.of("level", (int)lv)));
    }

    private void sendSwitch() {
        // No payload needed here.
        vertx.eventBus().publish(CONTROL_MODE_SWITCH, new JsonObject());
    }
    
    private void log(String msg) {
    	System.out.println("[SERIAL MODULE]: " + msg);
    }
}