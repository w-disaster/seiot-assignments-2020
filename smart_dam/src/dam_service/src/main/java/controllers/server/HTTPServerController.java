package controllers.server;

import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import controllers.dbms.DBMSController;
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
import model.Pair;
import model.Data;
import model.DataJson;
import model.State;
import view.View;

/*
 * Data Service as a vertx event-loop 
 */
public class HTTPServerController extends AbstractVerticle {

	private int port;
	private Data data;
	private DBMSController dbmsController;
	private CommChannel channel;
	
	private static final float D2_IN_M = 0.4f;
	private static final float DELTAD_IN_M = 0.04f;
	private static final float RIVER_HEIGHT_IN_M = 5f;
		
	public HTTPServerController(int port, Data data, DBMSController dbmsController, CommChannel channel) {
		this.port = port;
		this.data = data;
		this.dbmsController = dbmsController;
		this.channel = channel;
	}

	@Override
	public void start() {		
		Router router = Router.router(vertx);
		router.route().handler(BodyHandler.create());
		router.post("/api/data").handler(this::handleAddNewData);
		router.get("/api/data/:t").handler(this::handleGetData);
		router.get().handler(this::handleGet);
		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(this.port);

		log("ready");
	}
	
	@Override
    public void stop() throws Exception {
        System.out.println("MyVerticle stopped!");
    }
	
	private void handleGet(RoutingContext routingContext) {
		HttpServerRequest request = routingContext.request();
		HttpServerResponse response = request.response();

		Pair<Pair<String, String>, String> file = View.getFile(request.path());
		response.putHeader(file.getX().getX(), file.getX().getY());
		String fileName = file.getY();
		response.sendFile(fileName);
	}
	
	private void handleAddNewData(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		JsonObject res = routingContext.getBodyAsJson();
		if (res == null) {
			sendError(400, response);
		} else {
			/* Msg back to RH: success */
			response.setStatusCode(200).end();
			
			/* Set data State, timestamp and possibly the Mode */
			State state = State.values()[res.getInteger("S")];
			
			/* Dam Mode checking : if new State isn't ALARM it can't be MANUAL */
			if(this.data.getMode().equals(Mode.MANUAL) && !state.equals(State.ALARM)) {
				this.data.setMode(Mode.AUTO);
			}
			this.data.setState(state);
			long timestamp = res.getLong("T") * 1000;
			this.data.setTimestamp(timestamp);
			
			// Default
			float distance = 0;
			float waterLevel = 0;
			
			switch(state) {
			case NORMAL:
				this.data.setDamOpening(0);
				break;
			case PRE_ALARM:
				/* Only when PRE_ALARM or ALARM we calculate the water level */
				distance = res.getFloat("D");
				waterLevel = this.getWaterLevelFromDistance(distance);
				this.data.setWaterLevel(waterLevel);
				this.data.setDamOpening(0);
				break;
			case ALARM:
				/* Only when PRE_ALARM or ALARM we calculate the water level */
				distance = res.getFloat("D");
				waterLevel = this.getWaterLevelFromDistance(distance);
				this.data.setWaterLevel(waterLevel);
				
				/* Calculate the dam opening only when the mode is AUTO */
				if(this.data.getMode().equals(Mode.AUTO)) {
					this.data.setDamOpening(this.getDamOpeningFromDistance(distance));
				}
				break;
			}
			/* DBMS insert */
			this.dbmsController.executeInsert(this.data);
			
			/* Build the message to forward to Dam Controller from the data fields just set and send it */
			JsonObject sendJson = DataJson.getDataAsJson(this.data);
			this.channel.sendMsg(sendJson.encode());
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
		return (float) (Math.round((RIVER_HEIGHT_IN_M - distance) * 1000.0) / 1000.0);
	}
	
	private void handleGetData(RoutingContext routingContext) {
		/* DB query : we'll get all data onwards a given t (timestamp) */
		String t = routingContext.pathParam("t");
		Long timestamp = Long.parseLong(t.split("=")[1]);
		if(timestamp > 0) {
			Map<Integer, List<Pair<String, String>>> data = this.dbmsController.getDataFromTimestampOnwards(timestamp);
			
			JsonArray arr = new JsonArray();
			for(Entry<Integer, List<Pair<String, String>>> e : data.entrySet()) {
				JsonObject json = new JsonObject();
				for(Pair<String, String> p : e.getValue()) {
					json.put(p.getX(), p.getY());
				}
				arr.add(json);
			}
			
			/* Response to DamDashboard with a json array*/
			routingContext.response()
				.putHeader("content-type", "application/json")
				.end(arr.encodePrettily());
			
		} else {
			Map<String, String> data = this.dbmsController.getLastData();
			JsonObject json = new JsonObject();
			
			for(Entry<String, String> e : data.entrySet()) {
				json.put(e.getKey(), e.getValue());
			}
			
			/* Response to DamDashboard with a json object */
			routingContext.response()
				.putHeader("content-type", "application/json")
				.end(json.encodePrettily());
		}
	}
	
	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[HTTP SERVER " + msg + "]");
	}

}