package mainUnit;

import java.util.Stack;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.DecodeException;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import utils.ControlMode;
import utils.SystemState;
import utils.TemperatureSample;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.Map;

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
	private static String DASH_WINDOW_LEVEL_LINE_ADDRESS = "dashboard.window.level";
	private static String DASH_SWITCH_MODE_LINE_ADDRESS = "dashboard.controlmode.switch";
	private static String DASH_STOP_ALLARM_LINE_ADDRESS = "dashboard.alarm.stop";
	
	//Message lines for DASHBOARD updates	
	private static final String DASH_UPDATE_TEMPERATURE_STATS = "dash.update.temperature.stats";
	private static final String DASH_UPDATE_CONTROL_MODE = "dash.update.control.mode";
	private static final String DASH_UPDATE_SYSTEM_STATE = "dash.update.system.state";
	private static final String DASH_UPDATE_OPENING_LEVEL = "dash.update.opening.level";
	
	private LocalDateTime tooHotStartTime;
	
	private SystemState systemState = SystemState.NORMAL;
	private ControlMode controlMode = ControlMode.AUTOMATIC;
	
	private int openingLevel;
	
	private HashMap<LocalDateTime, Double> dailyMax = new HashMap<>();
    private HashMap<LocalDateTime, Double> dailyMin = new HashMap<>();
    private HashMap<LocalDateTime, Double> dailySum = new HashMap<>();
    private HashMap<LocalDateTime, Integer> dailyCount = new HashMap<>();
    private HashMap<LocalDateTime, Double> dailyAverage = new HashMap<>();
	
	//Stack keeping track of the last N temperatures sampled
	private Stack<TemperatureSample> temperatures;
	
	public ControlUnit () {
		this.openingLevel = 0;
	}

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
		 
		 /*Consumer for messages received from the server allerting that the user is trying to change the open level
		  * of the window thorugh the dashboard, the level is changed if the mode has already been switched to manual*/
		 vertx.eventBus().consumer(DASH_WINDOW_LEVEL_LINE_ADDRESS, message -> {
			    JsonObject body = (JsonObject) message.body();
			    int level = body.getInteger("level");
			    if(controlMode == ControlMode.MANUAL) {
			    	changeWindowOpenLevel(level);
			    }
			});
		 
		 /*Consumer for messages received from the server allerting that the user is trying to
		  * switch control mode*/
		 vertx.eventBus().consumer(DASH_SWITCH_MODE_LINE_ADDRESS, message -> {
			 this.controlMode = this.controlMode.switchMode();
			 this.sendControlModeDash();
			 });
		 
		 /*Consumer for messages received from the server allerting that the user has 
		  * solved the problem and wants to go back to the normal system state.*/
		 vertx.eventBus().consumer(DASH_STOP_ALLARM_LINE_ADDRESS, message -> {
			 if(this.systemState == SystemState.ALARM) {
				 this.systemState = SystemState.NORMAL;
				 this.sendSystemStateDash();
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
		this.openingLevel = openPercentage;
		vertx.eventBus().send(OPENLEVEL_LINE_ADDRESS, openPercentage);	
		this.sendOpeningLevelDash();
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
        this.sendStatsDash();
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
		this.sendSystemStateDash();
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
	
	// Funzione per ottenere il valore per il giorno corrente da una HashMap
	private <T> T getTodayValue(HashMap<LocalDateTime, T> map) {
	    LocalDate today = LocalDate.now();
	    for (Map.Entry<LocalDateTime, T> entry : map.entrySet()) {
	        if (entry.getKey().toLocalDate().equals(today)) {
	            return entry.getValue();
	        }
	    }
	    return null;  // oppure valore di default
	}

	//Function communicating to the server the update values for stats and last n recorded temperature samples
	private void sendStatsDash() {
	    Double todaysMax = getTodayValue(dailyMax);
	    Double todaysMin = getTodayValue(dailyMin);
	    Double todaysAverage = getTodayValue(dailyAverage);
	    JsonArray temperaturesArray = new JsonArray();
	    if (temperatures != null) {
	        for (TemperatureSample sample : temperatures) {
	            JsonObject sampleJson = new JsonObject()
	                .put("time", sample.getDateTime().toString())
	                .put("value", sample.getTemperature());
	            temperaturesArray.add(sampleJson);
	        }
	    }
	    vertx.eventBus().publish(DASH_UPDATE_TEMPERATURE_STATS, new JsonObject()
	    		.put("dailyMax", todaysMax)
	    		.put("dailyAverage", todaysAverage)
	    		.put("dailyMin", todaysMin)
	    		.put("temperatures", temperaturesArray));
	}

	private void sendControlModeDash() {
		vertx.eventBus().publish(DASH_UPDATE_CONTROL_MODE, new JsonObject()
	        .put("controlMode", controlMode != null ? controlMode.toString() : null));
	}

	private void sendSystemStateDash() {
	    vertx.eventBus().publish(DASH_UPDATE_SYSTEM_STATE, new JsonObject()
	        .put("systemState", systemState != null ? systemState.toString() : null));
	}
	private void sendOpeningLevelDash() {
		vertx.eventBus().publish(DASH_UPDATE_OPENING_LEVEL, new JsonObject()
	        .put("openingLevel", openingLevel));
	}
}
