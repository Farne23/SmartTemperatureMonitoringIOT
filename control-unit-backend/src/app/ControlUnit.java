/**
 * 
 */
package app;
import communication.MQTTAgent;

/**
 * This class is the entry point of the program
 */
public class ControlUnit {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Ciao");
		MQTTAgent testAgent = new MQTTAgent();
		testAgent.start();
	}

}
