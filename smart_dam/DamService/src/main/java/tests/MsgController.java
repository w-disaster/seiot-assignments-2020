package tests;

import java.time.LocalTime;

import model.State;

public interface MsgController {
	
	public void addData(State state, float level, LocalTime time);
	
	public State getState();
	
	public float getLevel();
	
	public LocalTime getTime();

}
