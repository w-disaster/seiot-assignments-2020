package controllers.serial;

import java.util.HashMap;
import java.util.Map;
import com.google.gson.Gson;

import model.Model;
import model.Mode;

public class SerialCommChannelControllerRunnable implements Runnable {

	private Model model;
	private CommChannel channel;
	
	public SerialCommChannelControllerRunnable(Model model, CommChannel channel) {
		super();
		this.model = model;
		this.channel = channel;
	}
	
	@Override
	public void run() {
		while (true){
			String sendJson;
			try {
				/* Incoming message */
				String msg = this.channel.receiveMsg();
				
				System.out.println(msg);
				
				/* We deserialize it as a Json */
				//Mode mode = this.getModeFromJson(msg);
				/* We set the mode of the model, if possible */
				//setEnvironmentMode(this.model, mode);
								
				/* Json for response */
				//sendJson = this.getJsonFromCurrentModeAsString();
				
		        /* We send back the current Mode of the Environment that serves as an ACK/NAK */
                //channel.sendMsg(sendJson);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	/* This method will be called for send a message back to Dam Controller*/
	private String getJsonFromCurrentModeAsString() {
		Gson gson = new Gson();
        Map<String, Mode> map = new HashMap<>();
        map.put("Mode", this.model.getMode());
        
        return gson.toJson(map, Map.class);
	}
	
	private Mode getModeFromJson(String json) {
		/* We read the Json file with the mode requested by Dam Controller */
		Gson gson = new Gson();
        Map<String, String> jsonMap = gson.fromJson(json, Map.class);
        
        /* We get the Mode and we set it if it's possible */
        return Mode.valueOf(jsonMap.get("Mode"));
	}
	
	
	public static void setEnvironmentMode(Model model, Mode mode) {
        /* Always possible switch to AUTO mode, only when state == ALARM to MANUAL mode */
        switch(mode) {
		case AUTO:
			model.setMode(mode);
			break;
		case MANUAL:
			switch(model.getState()) {
			case ALARM:
				model.setMode(mode);
				break;
			case NORMAL:
			case PREALARM:
				break;
			}
			break;
		} 
	}

}
