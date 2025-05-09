package communicationHTTP;


import java.time.LocalDateTime;
import java.util.HashSet;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;
import utils.ControlMode;
import utils.SystemState;
import utils.TemperatureSample;
public class HTTPServer extends AbstractVerticle {
	
	//Message lines for DASHBOARD updates	
	private static final String DASH_UPDATE_TEMPERATURE_STATS = "dash.update.temperature.stats";
	private static final String DASH_UPDATE_CONTROL_MODE = "update.control.mode.line";
	private static final String DASH_UPDATE_SYSTEM_STATE = "dash.update.system.state";
	private static final String DASH_UPDATE_OPENING_LEVEL = "update.opening.level.line";
	private static String CHANGE_WINDOW_LEVEL_LINE_ADDRESS = "window.level.change";
	private static String SWITCH_MODE_LINE_ADDRESS = "controlmode.switch";
	private static String STOP_ALARM_LINE_ADDRESS = "dashboard.alarm.stop";

    private int port = 8080;
    
    /* Infos that are going to be periodically
     * updated by the control unit and shared to the dashboard
     * client accessing it via HTTP Get requests.*/
    private double maxTemperature;
    private double minTemperature;
    private double avgTemperature;
    private ControlMode controlMode = ControlMode.AUTOMATIC;
    private SystemState systemState = SystemState.NORMAL;
    private int openingLevel;
    private HashSet<TemperatureSample> temperatures = new HashSet<TemperatureSample>();

    @Override
    public void start() {        
    	Router router = Router.router(vertx);

    	/*Cors handler needed to overcome problems with the free grok
    	 * verson use*/
    	router.route().handler(
    	    CorsHandler.create("*")
    	        .allowedMethod(io.vertx.core.http.HttpMethod.GET)
    	        .allowedMethod(io.vertx.core.http.HttpMethod.POST)
    	        .allowedMethod(io.vertx.core.http.HttpMethod.OPTIONS) 
    	        .allowedHeader("Content-Type")
    	        .allowedHeader("ngrok-skip-browser-warning")
    	);

    	//Enables parsing of the HTTP request bodies.
    	router.route().handler(BodyHandler.create());

    	//Handlers for get and post methods.
    	router.post("/api/commands").handler(this::handleDashboardControls);
    	router.get("/api/data").handler(this::handleGetData);
  
        vertx
            .createHttpServer()
            .requestHandler(router)
            .listen(port);

        log("Service ready on port: " + port);
        
        //Consumer for the stats sent by the control unit
        vertx.eventBus().consumer(DASH_UPDATE_TEMPERATURE_STATS, message -> {
            JsonObject body = (JsonObject) message.body();
            maxTemperature = body.getDouble("dailyMax", 0.0);
            minTemperature = body.getDouble("dailyMin", 0.0);
            avgTemperature = body.getDouble("dailyAverage", 0.0);

            JsonArray tempsArray = body.getJsonArray("temperatures");
            temperatures.clear();
            if (tempsArray != null) {
                for (int i = 0; i < tempsArray.size(); i++) {
                    JsonObject tempObj = tempsArray.getJsonObject(i);
                    LocalDateTime time = LocalDateTime.parse(tempObj.getString("time"));
                    double value = tempObj.getDouble("value");
                    temperatures.add(new TemperatureSample(time, value));
                }
            }
            log("Updated stats and temperatures.");
        });

        vertx.eventBus().consumer(DASH_UPDATE_CONTROL_MODE, message -> {
            JsonObject body = (JsonObject) message.body();
            String modeStr = body.getString("controlMode");
            if (modeStr != null) {
                controlMode = ControlMode.valueOf(modeStr);
                System.out.println("Updated control mode: " + controlMode);
            }
        });

        vertx.eventBus().consumer(DASH_UPDATE_SYSTEM_STATE, message -> {
            JsonObject body = (JsonObject) message.body();
            String stateStr = body.getString("systemState");
            if (stateStr != null) {
                systemState = SystemState.valueOf(stateStr);
                System.out.println("Updated system state: " + systemState);
            }
        });

        vertx.eventBus().consumer(DASH_UPDATE_OPENING_LEVEL, message -> {
            JsonObject body = (JsonObject) message.body();
            openingLevel = body.getInteger("openingLevel", 0);
            System.out.println("Updated opening level: " + openingLevel);
        });
    }
    
    /* Handler of post messages received from the dashboard, acting as a client.
     * It checks the control sent trough the type parameter of the json message received
     * and alerts the control unit through messages sent via vertex message lines*/
    private void handleDashboardControls(RoutingContext routingContext) {
    	log("AAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    	HttpServerResponse response = routingContext.response();
        JsonObject body = routingContext.getBodyAsJson();

        if (body == null || !body.containsKey("type")) {
            sendError(400, response.setStatusMessage("Missing or invalid JSON 'type' field"));
            return;
        }

        String messageType = body.getString("type");
        switch (messageType) {
            case "stopAlarm":
                vertx.eventBus().publish(STOP_ALARM_LINE_ADDRESS, body);
                break;
            case "switchControlMode":
                vertx.eventBus().publish(SWITCH_MODE_LINE_ADDRESS, body);
                break;
            case "updateWindowLevel":
                if (!body.containsKey("level") || !(body.getValue("level") instanceof Integer)) {
                    sendError(400, response.setStatusMessage("Missing or invalid 'level' field"));
                    return;
                }
                vertx.eventBus().publish(CHANGE_WINDOW_LEVEL_LINE_ADDRESS, body);
                break;
            default:
                sendError(400, response.setStatusMessage("Unknown message type: " + messageType));
                return;
        }

        response.setStatusCode(200).end();
        log("Control from client received, type: " + messageType);
    	
    }
    
    
    private void handleGetData(RoutingContext routingContext) {
        JsonObject data = new JsonObject();
        data.put("maxTemperature", maxTemperature);
        data.put("minTemperature", minTemperature);
        data.put("avgTemperature", avgTemperature);
        data.put("controlMode", controlMode != null ? controlMode.toString() : null);
        data.put("systemState", systemState != null ? systemState.toString() : null);
        data.put("openingLevel", openingLevel);

        JsonArray temperaturesArray = new JsonArray();
        if (temperatures != null) {
            for (TemperatureSample sample : temperatures) {
                JsonObject sampleJson = new JsonObject()
                    .put("time", sample.getDateTime().toString()) 
                    .put("value", sample.getTemperature());
                temperaturesArray.add(sampleJson);
            }
        }
        data.put("temperatures", temperaturesArray);
        
        System.out.println(data);

        routingContext.response()
            .putHeader("content-type", "application/json")
            .end(data.encodePrettily());
    }

    private void sendError(int statusCode, HttpServerResponse response) {
        response.setStatusCode(statusCode).end();
    }

    private void log(String msg) {
        System.out.println("[DATA SERVICE] "+msg);
    }
}
