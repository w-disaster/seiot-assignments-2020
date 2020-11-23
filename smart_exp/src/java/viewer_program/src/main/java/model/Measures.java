package model;

enum Measures {
    POSITION("Posizione", "m"),
    SPEED("Velocità", "m/s"),
    ACCELERATION("Accelerazione", "m/s^2");

    private final String name;
    private final String unitOfMeasure;

    Measures(final String name, final String unitOfMeasure) {
        this.name = name;
        this.unitOfMeasure = unitOfMeasure;
    }

    public String getName() {
        return this.name;
    }

    public String getUnitOfMeasure() {
        return this.unitOfMeasure;
    }
}
