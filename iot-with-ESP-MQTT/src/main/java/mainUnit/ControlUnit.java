package mainUnit;

import java.util.Stack;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.DecodeException;
import io.vertx.core.json.JsonObject;
import utils.TemperatureSample;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/*
 * Class implementing the main functions of the Window Controller Subsystem,
 */
public class ControlUnit extends AbstractVerticle implements TempSensorDataReceiver {
	
	private static final int MAX_HISTORY_SAMPLES = 100;
	
	//Stack keeping track of the last N temperatures sampled
	private Stack<TemperatureSample> temperatures;
	
	public ControlUnit () {}

	public void start() {
		/* Control unit is going to listen from the temperatures.line address.
		 * It' going to receive json formatted temperature samples.
		 * Once they will be read, it will try to add them to the temperatures stack
		 * */
		 vertx.eventBus().consumer("temperatures.line", message -> {
	            System.out.println("Messaggio ricevuto: " + message.body());
				try {
					JsonObject json = new JsonObject(message.body().toString());
					SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
					Date timestamp = sdf.parse(json.getString("timestamp"));
					double temperature = json.getDouble("temperature");
					communicateTemperature(timestamp,temperature);	
					JsonObject response = new JsonObject().put("status", "received");
					message.reply(response);
				} catch (ParseException | DecodeException e) {
					System.out.println("JSON non riconoscibile");
					e.printStackTrace();
				}		
	        });
	}
	
	private void communicateTemperature(Date date, double temperature) {
		if(historyFull()){
			temperatures.pop();
		}
		temperatures.push(new TemperatureSample(date,temperature));	
	}
	
	private boolean historyFull() {
		return temperatures.size()>=MAX_HISTORY_SAMPLES;
	}
}
