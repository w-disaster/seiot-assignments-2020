package model;

/*
 * Class representing the DBMS table RiverData
 */
public class Data  {

	private long timestamp;
	private float waterLevel;
	private Mode mode;
	private State state;
	private int damOpening;
	
	private static final long D_TIMESTAMP = 0;
	private static final float D_WATER_LVL = 0;
	private static final Mode D_MODE = Mode.AUTO;
	private static final State D_STATE = State.NORMAL;
	private static final int D_DAM_OPENING = 0;

	public Data() {
		super();
		this.timestamp = D_TIMESTAMP;
		this.waterLevel = D_WATER_LVL;
		this.mode = D_MODE;
		this.state = D_STATE;
		this.damOpening = D_DAM_OPENING;
	}

	public synchronized long getTimestamp() {
		return timestamp;
	}

	public synchronized void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}

	public synchronized float getWaterLevel() {
		return waterLevel;
	}

	public synchronized void setWaterLevel(float waterLevel) {
		this.waterLevel = waterLevel;
	}

	public synchronized Mode getMode() {
		return mode;
	}

	public synchronized void setMode(Mode mode) {
		this.mode = mode;
	}

	public synchronized State getState() {
		return state;
	}

	public synchronized void setState(State state) {
		this.state = state;
	}

	public synchronized int getDamOpening() {
		return damOpening;
	}

	public synchronized void setDamOpening(int damOpening) {
		this.damOpening = damOpening;
	}
	
}
