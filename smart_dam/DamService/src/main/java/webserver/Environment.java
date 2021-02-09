package webserver;

public interface Environment {
	
	State getState();
	
	Mode getMode();
	
	void setState(State state);
	
	boolean setMode(Mode mode);
		
}
