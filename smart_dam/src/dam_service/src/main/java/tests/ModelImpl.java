package tests;

import java.time.LocalTime;

import com.google.gson.Gson;

import model.State;

public class ModelImpl implements Model {
	
	State state;
	LocalTime time;
	float level;
	
	public State getState() {
		return state;
	}
	
	public void setState(State state) {
		this.state = state;
	}
	
	public LocalTime getTime() {
		return time;
	}
	
	public void setTime(LocalTime time) {
		this.time = time;
	}
	
	public float getLevel() {
		return level;
	}
	
	public void setLevel(float level) {
		this.level = level;
	}
	
}
