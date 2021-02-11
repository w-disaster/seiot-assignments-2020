package tests;

import java.time.LocalTime;

import model.State;

public interface Model {
	
	public State getState();
	
	public float getLevel();
	
	public LocalTime getTime();
	
	public void setState(State state);
	
	public void setLevel(float level);
	
	public void setTime(LocalTime time);

}
