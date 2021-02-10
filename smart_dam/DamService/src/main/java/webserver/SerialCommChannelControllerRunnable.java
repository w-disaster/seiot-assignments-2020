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
				String msg = this.channel.receiveMsg();
				sendJson = this.setEnvironment(msg);
		        /* We send back the current Mode of the Environment that serves as an ACK/NAK */
                channel.sendMsg(sendJson);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private String setEnvironment(String json) {
		/* We read the Json file with the mode requested by Dam Controller */
		Gson gson = new Gson();
        Map<String, String> jsonMap = gson.fromJson(json, Map.class);
        
        /* We get the Mode and we set it if it's possible */
        
        Mode mode = Mode.valueOf(jsonMap.get("Mode"));
        
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
                
        /* Return message to Dam Controller*/
        Map<String, Mode> map = new HashMap<>();
        map.put("Mode", this.environment.getMode());
        
        return gson.toJson(map, Map.class);
	}

}
