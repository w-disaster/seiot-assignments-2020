package model;

public class ModelImpl implements Model {

	private Mode mode;
	private State state;
	private int damOpening;
	
	public ModelImpl(Mode mode, State state, int damOpening) {
		super();
		this.mode = mode;
		this.state = state;
	}

	@Override
	public State getState() {
		return this.state;
	}

	@Override
	public Mode getMode() {
		return this.mode;
	}

	@Override
	public synchronized void setState(State state) {
		this.state = state;
	}

	@Override
	public synchronized void setMode(Mode mode) {
		this.mode = mode;
	}
	
	@Override
	public synchronized void setDamOpening(int damOpening) {
		this.damOpening = damOpening;
	}
	
	@Override
	public int getDamOpening() {
		return this.damOpening;
	}
	
}
