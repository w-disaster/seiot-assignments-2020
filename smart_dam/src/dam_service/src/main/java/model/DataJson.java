package model;

import io.vertx.core.json.JsonObject;

public class DataJson {

	public static final JsonObject getDataAsJson(Data data) {
		JsonObject dataAsJson = new JsonObject();
		
		if(!data.getState().equals(State.NORMAL)) {
			dataAsJson.put("L", data.getWaterLevel());
		}
		dataAsJson.put("M", data.getMode().ordinal());
		dataAsJson.put("S", data.getState().ordinal());
		if(data.getMode().equals(Mode.AUTO)) {
			dataAsJson.put("DO", data.getDamOpening());
		}
		
		return dataAsJson;
	}
	
}
