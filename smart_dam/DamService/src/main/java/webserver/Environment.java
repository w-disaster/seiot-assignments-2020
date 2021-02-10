package webserver;

public interface Environment {
	
	State getState();
	
	Mode getMode();
	
	void setState(State state);
	
	void setMode(Mode mode);
		
}
