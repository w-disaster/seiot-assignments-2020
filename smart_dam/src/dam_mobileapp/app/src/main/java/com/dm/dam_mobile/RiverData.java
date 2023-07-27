package com.dm.dam_mobile;

public class RiverData {
    private State state;
    private DamMode damMode;
    private int damOpening;
    private float waterLevel;

    public RiverData(State state, DamMode damMode, int damOpening, float waterLevel) {
        this.state = state;
        this.damMode = damMode;
        this.damOpening = damOpening;
        this.waterLevel = waterLevel;
    }

    public RiverData(State state, DamMode damMode, int damOpening) {
        this.state = state;
        this.damMode = damMode;
        this.damOpening = damOpening;
    }

    public State getState() {
        return state;
    }

    public DamMode getDamMode() {
        return damMode;
    }

    public int getDamOpening() {
        return damOpening;
    }

    public float getWaterLevel() {
        return waterLevel;
    }
}
