package controllers.server;

import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import controllers.serial.CommChannel;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpServerRequest;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import model.Mode;
import model.Model;
import model.Pair;
import model.State;

/*
 * Data Service as a vertx event-loop 
 */
public class HTTPServerController extends AbstractVerticle {

	private int port;
	private Model model;
	private DBMSController dbmsController;
	private CommChannel channel;
	
	private static final float D2_IN_M = 0.4f;
	private static final float D1_IN_M = 1.0f;
	private static final float DELTAD_IN_M = 0.04f;
	private static final float RIVER_HEIGHT_IN_M = 5f;
	
	public HTTPServerController(int port, Model model, DBMSController dbmsController, CommChannel channel) {
		this.port = port;
		this.model = model;
		this.dbmsController = dbmsController;
		this.channel = channel;
	}

	@Override
	public void start() {		
		Router router = Router.router(vertx);
		router.route().handler(BodyHandler.create());
		router.post("/api/data").handler(this::handleAddNewData);
		router.get("/api/data").handler(this::handleGetData);
		router.get().handler(this::handleGet);
		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);

		log("ready");
	}
	
	private void handleGet(RoutingContext routingContext) {
		HttpServerRequest request = routingContext.request();
		HttpServerResponse response = request.response();
		response.putHeader("content-type", "text/html");
		
		// Write to the response and end it
		String file = System.getProperty("user.dir") + "/src/main/java/view/" + request.path();
		response.sendFile(file);
	}
	
	private void handleAddNewData(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		// log("new msg "+routingContext.getBodyAsString());
		JsonObject res = routingContext.getBodyAsJson();
		if (res == null) {
			sendError(400, response);
		} else {
			/* We read the json */
			State state = State.valueOf(res.getString("State"));
			long timestamp = res.getLong("Timestamp");
			float distance = res.getFloat("Distance");
			
			/* Dam open percentage */
			int damOpening = this.getDamOpeningFromDistance(distance);
			/* We obtain the Water Level from detected distance */
			float waterLevel = this.getWaterLevelFromDistance(distance);
			
			/* Dam Mode checking : if new State isn't ALARM it can be MANUAL */
			if(this.model.getMode().equals(Mode.MANUAL) && !state.equals(State.ALARM)) {
				this.model.setMode(Mode.AUTO);
			}
			/* State setting */
			this.model.setState(state);
			
			/* DBMS insert */
			this.dbmsController.insertData(timestamp, waterLevel, this.model.getMode().toString(), state.toString(), damOpening);
			
			/* Msg back if success */
			response.setStatusCode(200).end();
			
			/* Forward message to Dam Controller (Arduino) */
			this.channel.sendMsg(res.encodePrettily());
		}
		
	}
	
	private int getDamOpeningFromDistance(float distance) {
		if(distance > D2_IN_M - DELTAD_IN_M && distance <= D2_IN_M) {
			return 20;
		} else if(distance > D2_IN_M - 2 * DELTAD_IN_M && distance < D2_IN_M - DELTAD_IN_M) {
			return 40;
		} else if(distance > D2_IN_M - 3 * DELTAD_IN_M && distance < D2_IN_M - 2 * DELTAD_IN_M) {
			return 60;
		} else if(distance > D2_IN_M - 4 * DELTAD_IN_M && distance < D2_IN_M - 3 * DELTAD_IN_M) {
			return 80;
		} else if(distance < D2_IN_M - 4 * DELTAD_IN_M) {
			return 100;
		}
		return 0;
	}
	
	private float getWaterLevelFromDistance(float distance) {
		return RIVER_HEIGHT_IN_M - distance;
	}
	
	private void handleGetData(RoutingContext routingContext) {
		HttpServerRequest request = routingContext.request();
		
		/* DB query : we'll get all data onwards a given t (timestamp) */
		Long timestamp = Long.parseLong(request.getParam("t"));
		
		Map<Integer, List<Pair<String, String>>> data = this.dbmsController.getDataFromTimestampOnwards(timestamp);
		
		/* We build a json array from collected data */
		JsonArray arr = new JsonArray();
		for(Entry<Integer, List<Pair<String, String>>> e : data.entrySet()) {
			JsonObject json = new JsonObject();
			for(Pair<String, String> p : e.getValue()) {
				json.put(p.getX(), p.getY());
			}
			arr.add(json);
		}
		
		/* Response to DamDashboard with a json file */
		routingContext.response()
			.putHeader("content-type", "application/json")
			.end(arr.encodePrettily());
	}
	
	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[HTTP SERVER " + msg + "]");
	}

}