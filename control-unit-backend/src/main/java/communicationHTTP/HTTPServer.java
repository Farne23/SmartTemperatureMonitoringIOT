package communicationHTTP;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;
public class HTTPServer extends AbstractVerticle {

    private int port = 8080;
    private static final int MAX_SIZE = 10;
    //private LinkedList<DataPoint> values;

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
            default:
                sendError(400, response.setStatusMessage("Unknown message type: " + messageType));
                return;
        }

        response.setStatusCode(200).end();
        log("Control from client received, type: " + messageType);
    	
    }
    
    
    private void handleGetData(RoutingContext routingContext) {
        JsonArray arr = new JsonArray();
        JsonObject data = new JsonObject();
            data.put("time", "a");
            data.put("value", "b");
            data.put("place", "c");
            arr.add(data);
        routingContext.response()
            .putHeader("content-type", "application/json")
            .end(arr.encodePrettily());
    }

    private void sendError(int statusCode, HttpServerResponse response) {
        response.setStatusCode(statusCode).end();
    }

    private void log(String msg) {
        System.out.println("[DATA SERVICE] "+msg);
    }
}
