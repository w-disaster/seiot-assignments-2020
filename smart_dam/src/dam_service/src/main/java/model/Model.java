package model;

public interface Model {
	
	State getState();
	
	Mode getMode();
	
	void setState(State state);
	
	void setMode(Mode mode);
	
	void setDamOpening(int damOpening);
	
	int getDamOpening();
		
}
