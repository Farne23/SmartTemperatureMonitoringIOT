package mainUnit;

import java.util.Stack;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.DecodeException;
import io.vertx.core.json.JsonObject;
import utils.SystemState;
import utils.TemperatureSample;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.Date;
import java.util.HashMap;

/*
 * Class implementing the main functions of the Window Controller Subsystem,
 */
public class ControlUnit extends AbstractVerticle implements TempSensorDataReceiver {
	
	private static final int MAX_HISTORY_SAMPLES = 100;
	private static final double TEMPERATURE_THRESHOLD_NORMAL = 20;
	private static final double TEMPERATURE_THRESHOLD_HOT = 22;
	private static final long TOO_HOT_MAX_SECONDS = 7;
	private static final int PERIOD_NORMAL = 3000;
	private static final int PERIOD_HOT = 2000;
	private static final int PERIOD_TOO_HOT = 1000;
	private static String FREQUENCY_LINE_ADDRESS = "frequency.line";
	private static String TEMPERATURES_LINE_ADDRESS = "temperatures.line";
	private static String OPENLEVEL_LINE_ADDRESS = "openlevel.line";
	
	private LocalDateTime tooHotStartTime;
	
	private SystemState systemState = SystemState.NORMAL;
	private int openPercentage;
	
	private HashMap<LocalDateTime, Double> dailyMax = new HashMap<>();
    private HashMap<LocalDateTime, Double> dailyMin = new HashMap<>();
    private HashMap<LocalDateTime, Double> dailySum = new HashMap<>();
    private HashMap<LocalDateTime, Integer> dailyCount = new HashMap<>();
    private HashMap<LocalDateTime, Double> dailyAverage = new HashMap<>();
	
	//Stack keeping track of the last N temperatures sampled
	private Stack<TemperatureSample> temperatures;
	
	public ControlUnit () {}

	public void start() {
		/* Control unit is going to listen from the temperatures.line address.
		 * It' going to receive json formatted temperature samples.
		 * Once they will be read, it will try to add them to the temperatures stack
		 * */
		 vertx.eventBus().consumer(TEMPERATURES_LINE_ADDRESS, message -> {
	            System.out.println("Messaggio ricevuto: " + message.body());
				try {
					JsonObject json = new JsonObject(message.body().toString());
					LocalDateTime timestamp = LocalDateTime.parse(json.getString("timestamp"));
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
	
	private void communicateTemperature(LocalDateTime date, double temperature) {
		TemperatureSample sample = new TemperatureSample(date,temperature);
		updateStats(sample);
		if(historyFull()){
			temperatures.pop();
		}
		temperatures.push(sample);	
	}
	
	private boolean historyFull() {
		return temperatures.size()>=MAX_HISTORY_SAMPLES;
	}
	
	private void changeFrequency(int frequency) {
		
		/*vertx.eventBus().send("temperatures.line", 
			    new JsonObject()
			        .put("timestamp", LocalDateTime.now().toString())
			        .put("temperature", 22.5)
			);*/
		vertx.eventBus().send(FREQUENCY_LINE_ADDRESS, frequency);		
	}
	
	private void changeWindowOpenLevel(int openPercentage) {
		
		/*vertx.eventBus().send("temperatures.line", 
			    new JsonObject()
			        .put("timestamp", LocalDateTime.now().toString())
			        .put("temperature", 22.5)
			);*/
		vertx.eventBus().send(OPENLEVEL_LINE_ADDRESS, openPercentage);		
	}
	
	private void closeWindow() {
		changeWindowOpenLevel(0);
	}
	
	private void openWindow() {
		changeWindowOpenLevel(100);
	}
	
	/*
	 * Updates the system's daily statistics 
	 */
	private void updateStats(TemperatureSample sample) {
        // Update Max Temperature
		dailyMax.put(sample.getDateTime(), Math.max(dailyMax.getOrDefault(sample.getDateTime(), Double.MIN_VALUE), sample.getTemperature()));
        dailyMin.put(sample.getDateTime(), Math.min(dailyMin.getOrDefault(sample.getDateTime(), Double.MAX_VALUE), sample.getTemperature()));
        dailySum.put(sample.getDateTime(), dailySum.getOrDefault(sample.getDateTime(), 0.0) + sample.getTemperature());
        dailyCount.put(sample.getDateTime(), dailyCount.getOrDefault(sample.getDateTime(), 0) + 1);
        dailyAverage.put(sample.getDateTime(), dailySum.get(sample.getDateTime()) / dailyCount.get(sample.getDateTime()));
        checkStatus(sample);
	}
	
	/*
	 * Function handling the management of the system every time a ne temperature sample
	 * is comumnicated
	 */
	private void checkStatus(TemperatureSample sample) {
		if(systemState != SystemState.ALARM) {	
			if(sample.getTemperature() <= TEMPERATURE_THRESHOLD_NORMAL && systemState != SystemState.NORMAL) {
				systemState = SystemState.NORMAL;
				closeWindow();
				changeFrequency(PERIOD_NORMAL);
			} else if(sample.getTemperature() <= TEMPERATURE_THRESHOLD_HOT && sample.getTemperature() > TEMPERATURE_THRESHOLD_NORMAL ) {
				if(systemState != SystemState.HOT) {
					systemState = SystemState.HOT;
					changeFrequency(PERIOD_HOT);
				}
				changeWindowOpenLevel(getOpenLevel(sample.getTemperature()));
			} else if (sample.getTemperature() > TEMPERATURE_THRESHOLD_HOT) {
				openWindow();
				if(systemState!=SystemState.TOO_HOT) {
					systemState = SystemState.TOO_HOT;
					tooHotStartTime = sample.getDateTime();
					changeFrequency(PERIOD_TOO_HOT);
				}else if (tooHotStartTime.plusSeconds(TOO_HOT_MAX_SECONDS).isBefore(sample.getDateTime())) {
					systemState = SystemState.ALARM;
				}
			}	
		}	
	}
	
	/*
	 * Returns the open level percentage of the window, proportionally to the temperature slot
	 * for the HOT state. 
	 * 
	 * @param temperature
	 */
	private int getOpenLevel(double temperature) {
		return (int)Math.round((temperature-TEMPERATURE_THRESHOLD_NORMAL)*100/(TEMPERATURE_THRESHOLD_HOT-TEMPERATURE_THRESHOLD_NORMAL));
	}
}
