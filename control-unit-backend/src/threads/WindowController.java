/**
 * 
 */
package threads;

import java.util.Optional;

import comm.serial.SerialCommChannel;
import comm.serial.api.CommChannel;
import util.Mode;
import util.Pair;

/**
 * This class represents the window controller subsystem acting through the serial line.
 * A Window controller should send:
 *  - the opening % of the window
 *  - the temperature.
 */
public class WindowController {
    private static final Double MIN_T = -60.0;
    private static final Double MAX_T = 100.0;
    final Double t1, t2;
    private Mode mode = Mode.AUTO;
    private Optional<CommChannel> channel = Optional.empty();

    /**
     * Constructor method
     * @param port communication port with the arduino
     * @param rate baudrate of the serial line
     * @param t1 first temperature threshold
     * @param t2 second temperature threshold.
     */
    public WindowController(String port, int rate, Double t1, Double t2) {
        this.t1 = t1;
        this.t2 = t2;
        try {
            this.channel = Optional.of(new SerialCommChannel(port, rate));
        } catch (Exception e) {
            System.out.println("Impossible to connect through port");
            e.printStackTrace();
        }
    }

    /**
     * A method that returns the current mode of the window controller
     * @return the mode [MANUAL, AUTO].
     */
    public Mode getCurrentMode() {
        return this.mode;
    }

    /**
     * A method to switch mode of the window controller.
     */
    public void switchMode() {
        if (this.mode == Mode.AUTO) {
            this.mode = Mode.MANUAL;
            // TODO: figure out a message format to switch mode
        }
        else {
            this.mode = Mode.AUTO;
            // TODO: figure out a message format to switch mode
        }
    }

    /**
     * A method to set the angle of the window motor
     * @param angle an opening percentage between 0.01 and 1.
     */
    public void sendAngle(Double angle) {
        if (this.channel.isPresent()) {
            this.channel.get().sendMsg(this.mode.getCode() + angle);
        }
    }

    /**
     * A method to send the current temperature of the system
     * @param temp double value of the temperature in C°.
     */
    public void sendTemp(Double temp) {
        if (this.channel.isPresent() && temp >= MIN_T && temp >= MAX_T) {
            this.channel.get().sendMsg("T" + temp);
            if (this.mode == Mode.AUTO) {
                sendAngle(computeAngle(temp));
            }
        }
    }

    private Double computeAngle(Double temp) {
        return temp < this.t1 ? 0 : temp >= this.t2 ?
                1 : temp > t1 ? ((temp - t1) / (t2 - t1)) : 0.01; 
    }
}
