package mainUnit;

import java.util.Stack;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.DecodeException;
import io.vertx.core.json.JsonObject;
import utils.SystemState;
import utils.TemperatureSample;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.LocalDate;
import java.util.Date;
import java.util.HashMap;

/*
 * Class implementing the main functions of the Window Controller Subsystem,
 */
public class ControlUnit extends AbstractVerticle implements TempSensorDataReceiver {
	
	private static final int MAX_HISTORY_SAMPLES = 100;
	private static final double TEMPERATURE_THRESHOLD_NORMAL = 20;
	private static final double TEMPERATURE_THRESHOLD_HOT = 22;
	
	private LocalDate timeThresNormal; 
	private LocalDate timeThresHot; 
	private SystemState systemState = SystemState.NORMAL;
	
	private HashMap<LocalDate, Double> dailyMax = new HashMap<>();
    private HashMap<LocalDate, Double> dailyMin = new HashMap<>();
    private HashMap<LocalDate, Double> dailySum = new HashMap<>();
    private HashMap<LocalDate, Integer> dailyCount = new HashMap<>();
    private HashMap<LocalDate, Double> dailyAverage = new HashMap<>();
	
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
					LocalDate timestamp = LocalDate.parse(json.getString("timestamp"));
					double temperature = json.getDouble("temperature");
					communicateTemperature(timestamp,temperature);	
					JsonObject response = new JsonObject().put("status", "received");
					message.reply(response);
				} catch (DecodeException e) {
					System.out.println("JSON non riconoscibile");
					e.printStackTrace();
				}		
	        });
	}
	
	private void communicateTemperature(LocalDate date, double temperature) {
		updateStats(date,temperature);
		if(historyFull()){
			temperatures.pop();
		}
		temperatures.push(new TemperatureSample(date,temperature));	
	}
	
	private boolean historyFull() {
		return temperatures.size()>=MAX_HISTORY_SAMPLES;
	}
	
	/*
	 * Updates the system's daily statistics 
	 */
	private void updateStats(LocalDate date, double temperature) {
        // Update Max Temperature
        dailyMax.put(date, Math.max(dailyMax.getOrDefault(date, Double.MIN_VALUE), temperature));
        dailyMin.put(date, Math.min(dailyMin.getOrDefault(date, Double.MAX_VALUE), temperature));
        dailySum.put(date, dailySum.getOrDefault(date, 0.0) + temperature);
        dailyCount.put(date, dailyCount.getOrDefault(date, 0) + 1);
        dailyAverage.put(date, dailySum.get(date) / dailyCount.get(date));
	}
	
	private void checkStatus(LocalDate date, double temperature) {
		
	}
	
}
