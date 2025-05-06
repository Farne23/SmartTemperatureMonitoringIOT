package communicationSerial;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Handler;
import communicationSerial.api.*;

public class WindowControlCommunicationClientImpl extends AbstractVerticle implements WindowControlCommunicationClient {
    //Message lines to receive from.
	private static final String UPDATE_TEMPERATURE_LINE = "update.temperature.line";
	private static final String UPDATE_CONTROL_MODE = "update.control.mode.line";
	private static final String UPDATE_OPENING_LEVEL = "update.opening.level";
    // Message lines to send data to.
    private static final String WINDOW_LEVEL_CHANGE = "window.level.change";
    private static final String CONTROL_MODE_SWITCH = "control.mode.switch";
    // Port  and baudrate for serial communication.
    private static final String PORT = "COM3";
    private static final int BAUD_RATE = 9600;
    private static final long DELAY = 150;

    private CommChannel serial;

    public void start() {
        // Creation of the serial line.
        try {
            this.serial = new SerialCommChannel(PORT, BAUD_RATE);
        }
        catch (Exception e)
        {
            System.out.println("Serial line not found");
        }
        System.out.println("Connected succesfully to serial line");

        //Handler for messages sent by the dashboard to change opening
        // level.
        vertx.eventBus().consumer(UPDATE_OPENING_LEVEL, message -> {
            System.out.println("Messaggio ricevuto: " + message.body() + "%");
            /*
             * chiamata di un metodo privato che scrive sul seriale di cambiare livello
             *  di apertura del servo
             */
        });

        //Handler for messages sent by the dashboard to switch mode
        vertx.eventBus().consumer(UPDATE_CONTROL_MODE, message -> {
            System.out.println("Messaggio ricevuto: switch to" + message.body());
            /*
             * chiamata di un metodo privato che scrive sul seriale di cambiare modalità
             */
        });

        // Handler for messages sent by the ESP to update temperature
        vertx.eventBus().consumer(UPDATE_TEMPERATURE_LINE, message -> {
            System.out.println("Messaggio ricevuto: " + message.body() + "°C");
            /*
             * chiamata di un metodo privato che scrive sul seriale di aggiornare la temperatura
             */
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
            if (!msg.equals("")) {
                parseMessage(msg);
            }
        });
    }

    private void parseMessage(String msg) {
        
    }

}