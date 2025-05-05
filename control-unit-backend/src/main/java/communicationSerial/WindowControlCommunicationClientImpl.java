package communicationSerial;

import io.vertx.core.AbstractVerticle;
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
    }

    

    private class ReadAgent implements Runnable {

        private final CommChannel channel;

        public ReadAgent(CommChannel channel) { 
            this.channel = channel;
        }

        @Override
        public void run() {
            if (serial.isMsgAvailable()) {
                try {
                    String cmd = serial.receiveMsg();
                }
                catch (InterruptedException e)
                {
                    System.out.println("Cannot read message from serial line");
                }
                /*
                 * Parsing del messaggio, bisogna decidere se farlo interno al thread o farlo fuori con
                 * un metodo privato e tenere compatto il codice dell'agent (a livello di performance non cambia niente).
                 * il metodo che fa parsing poi sceglierà il verticle a cui mandare i dati coerentemente.
                 */
            }
        }

    }
}