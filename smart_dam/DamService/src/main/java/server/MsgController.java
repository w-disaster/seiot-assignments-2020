package server;

import java.time.LocalTime;

import environment.State;

public interface MsgController {
	
	public void addData(State state, float level, LocalTime time);
	
	public State getState();
	
	public float getLevel();
	
	public LocalTime getTime();

}
