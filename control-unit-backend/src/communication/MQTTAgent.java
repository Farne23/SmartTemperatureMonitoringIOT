package communication;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;
import io.vertx.mqtt.MqttClientOptions;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/*
 * MQTT Agent
 */
public class MQTTAgent extends AbstractVerticle {

    private static final String BROKER_ADDRESS = "broker.hivemq.com"; 
    private static final int BROKER_PORT = 1883;
    private static final String TOPIC_NAME = "test/hivemq";
    private static final String USERNAME = "your_username";
    private static final String PASSWORD = "your_password";

    private MqttClient client;

    @Override
    public void start() {
        MqttClientOptions options = (MqttClientOptions) new MqttClientOptions()
        		.setAutoKeepAlive(true)
        		.setCleanSession(true)
        		.setUsername(USERNAME)
        		.setPassword(PASSWORD)
        		.setReconnectAttempts(5)
        		.setReconnectInterval(2000);

        client = MqttClient.create(vertx, options);

        client.connect(BROKER_PORT, BROKER_ADDRESS, result -> {
            if (result.succeeded()) {
                log("✅ Connesso al broker HiveMQ con autenticazione!");

                client.publishHandler(message -> {
                    log("📩 Nuovo messaggio ricevuto:");
                    log("Topic: " + message.topicName());
                    log("Contenuto: " + message.payload().toString());
                    log("QoS: " + message.qosLevel());
                }).subscribe(TOPIC_NAME, MqttQoS.AT_LEAST_ONCE.value(), subRes -> {
                    if (subRes.succeeded()) {
                        log("✅ Sottoscritto al topic: " + TOPIC_NAME);
                    } else {
                        log("❌ Errore nella sottoscrizione: " + subRes.cause().getMessage());
                    }
                });

                log("📤 Pubblicazione di un messaggio...");
                client.publish(TOPIC_NAME, Buffer.buffer("Hello from Vert.x with auth!"), MqttQoS.AT_LEAST_ONCE, false, false);

            } else {
                log("❌ Connessione fallita: " + result.cause().getMessage());
            }
        });
    }

    @Override
    public void stop() {
        if (client != null && client.isConnected()) {
            log("🛑 Disconnessione dal broker MQTT...");
            client.disconnect(ar -> {
                if (ar.succeeded()) {
                    log("✅ Disconnesso con successo.");
                } else {
                    log("❌ Errore durante la disconnessione: " + ar.cause().getMessage());
                }
            });
        }
    }

    private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new MQTTAgent());
    }
}