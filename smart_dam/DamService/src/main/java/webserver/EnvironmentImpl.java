package webserver;

public class EnvironmentImpl implements Environment {

	private Mode mode;
	private State state;
	
	public EnvironmentImpl(Mode mode, State state) {
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
	public void setState(State state) {
		this.state = state;
	}

	@Override
	public boolean setMode(Mode mode) {
		switch(mode) {
		case AUTO:
			this.mode = mode;
			return true;
		case MANUAL:
			switch(this.state) {
			case ALARM:
				this.mode = mode;
				return true;
			case NORMAL:
			case PRE_ALARM:
				return false;
			}
		}
		return false;
	}
	
}
