package communicationHTTP;


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

    private int port = 8080;
    
    /* Infos that are going to be periodically
     * updated by the control unit and shared to the dashboard
     * client accessing it via HTTP Get requests.*/
    private double maxTeperature;
    private double minTemperature;
    private double avgTemperature;
    private ControlMode controlMode;
    private SystemState systemState;
    private int openingLevel;
    private HashSet<TemperatureSample> temperatures;

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
    	router.post("/api/data").handler(this::handleDashaboardControls);
    	router.get("/api/data").handler(this::handleGetData);
  
        vertx
            .createHttpServer()
            .requestHandler(router)
            .listen(port);

        log("Service ready on port: " + port);
    }
    
    /* Handler of post messages received from the dashboard, acting as a client.
     * It checks the control sent trough the type parameter of the json message received
     * and alerts the control unit through messages sent via vertex message lines*/
    private void handleDashaboardControls(RoutingContext routingContext) {
    	HttpServerResponse response = routingContext.response();
        JsonObject body = routingContext.getBodyAsJson();

        if (body == null || !body.containsKey("type")) {
            sendError(400, response.setStatusMessage("Missing or invalid JSON 'type' field"));
            return;
        }

        String messageType = body.getString("type");
        switch (messageType) {
            case "stopAlarm":
                vertx.eventBus().publish("dashboard.alarm.stop", body);
                break;
            case "switchControlMode":
                vertx.eventBus().publish("dashboard.controlmode.switch", body);
                break;
            case "updateWindowLevel":
                if (!body.containsKey("level") || !(body.getValue("level") instanceof Integer)) {
                    sendError(400, response.setStatusMessage("Missing or invalid 'level' field"));
                    return;
                }
                vertx.eventBus().publish("dashboard.window.level", body);
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
        data.put("maxTemperature", maxTeperature);
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
