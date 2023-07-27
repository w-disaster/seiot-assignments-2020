package controllers.serial;

import io.vertx.core.json.JsonObject;
import model.Data;
import model.DataJson;
import model.Mode;

public class SerialCommChannelControllerRunnable implements Runnable {

	private Data data;
	private CommChannel channel;
	
	public SerialCommChannelControllerRunnable(Data data, CommChannel channel) {
		super();
		this.data = data;
		this.channel = channel;
	}
	
	@Override
	public void run() {
		while (true){
			try {
				/* Blocking receive */
				String msg = this.channel.receiveMsg();
				/* Set Model's fields */
				this.deserializeJsonAndSetModel(msg);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	/* Deserialize the received message and set Model damMode and damOpening fields */
	private void deserializeJsonAndSetModel(String msg) {
		JsonObject json = new JsonObject(msg);
		
		/* Send to DM the last sampled data if getLastData is set to true */
		if(json.containsKey("getLastData")) {
			if(json.getInteger("getLastData") == 0) {
				JsonObject sendJson = DataJson.getDataAsJson(this.data);
				this.channel.sendMsg(sendJson.encode());
			}
		}
		/* Set damMode */
		else if(json.containsKey("M")) {
        	this.data.setMode(Mode.values()[json.getInteger("M")]);
        /* If json contains the dam opening then it's implicit that dam mode = MANUAL */
        }else if(json.containsKey("D")) {
        	this.data.setMode(Mode.MANUAL);
        	this.data.setDamOpening(json.getInteger("D"));
        }
	}

}
