package webserver;

import java.time.LocalTime;

import com.google.gson.Gson;

import jssc.SerialPort;

public class MsgControllerImpl implements MsgController {
	
	private Environment environment;
	private SerialPort serialPort;

	public MsgControllerImpl(Environment environment, SerialPort serialPort) {
		super();
		this.environment = environment;
		this.serialPort = serialPort;
	}

	@Override
	public void addData(State state, float level, LocalTime time) {
		
	}

	@Override
	public State getState() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public float getLevel() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public LocalTime getTime() {
		// TODO Auto-generated method stub
		return null;
	}

}
