package webserver;

import java.util.HashMap;
import java.util.Map;
import com.google.gson.Gson;

public class SerialCommChannelControllerRunnable implements Runnable {

	private Environment environment;
	private CommChannel channel;
	
	public SerialCommChannelControllerRunnable(Environment environment, CommChannel channel) {
		super();
		this.environment = environment;
		this.channel = channel;
	}
	
	@Override
	public void run() {
		this.channel.sendMsg("ready");
		while (true){
			String sendJson;
			try {
				/* Incoming message */
				String msg = this.channel.receiveMsg();

				/* We deserialize it as a Json */
				Mode mode = this.getModeFromJson(msg);
				/* We set the mode of the environment, if possible */
				this.setEnvironmentMode(mode);
				
				System.out.println("SERIAL: " + this.environment.getMode().toString());
				
				/* Json for response */
				sendJson = this.getJsonFromCurrentModeAsString();
				
		        /* We send back the current Mode of the Environment that serves as an ACK/NAK */
                channel.sendMsg(sendJson);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	/* This method will be called for send a message back to Dam Controller*/
	private String getJsonFromCurrentModeAsString() {
		Gson gson = new Gson();
        Map<String, Mode> map = new HashMap<>();
        map.put("Mode", this.environment.getMode());
        
        return gson.toJson(map, Map.class);
	}
	
	private Mode getModeFromJson(String json) {
		/* We read the Json file with the mode requested by Dam Controller */
		Gson gson = new Gson();
        Map<String, String> jsonMap = gson.fromJson(json, Map.class);
        
        /* We get the Mode and we set it if it's possible */
        return Mode.valueOf(jsonMap.get("Mode"));
	}
	
	
	private void setEnvironmentMode(Mode mode) {
        /* Always possible switch to AUTO mode, only when state == ALARM to MANUAL mode */
        switch(mode) {
		case AUTO:
			this.environment.setMode(mode);
			break;
		case MANUAL:
			switch(this.environment.getState()) {
			case ALARM:
				this.environment.setMode(mode);
				break;
			case NORMAL:
			case PRE_ALARM:
				break;
			}
			break;
		} 
	}

}
