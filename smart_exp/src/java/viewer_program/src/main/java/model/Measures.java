package model;

enum Measures {
    POSITION("Posizione", "m", "#0028c9"),
    SPEED("Velocità", "m/s", "#e68e00"),
    ACCELERATION("Accelerazione", "m/s^2", "#31d48a");

    private final String name;
    private final String unitOfMeasure;
    private final String lineColor;

    Measures(final String name, final String unitOfMeasure, final String lineColor) {
        this.name = name;
        this.unitOfMeasure = unitOfMeasure;
        this.lineColor = lineColor;
    }

    public String getName() {
        return this.name;
    }

    public String getUnitOfMeasure() {
        return this.unitOfMeasure;
    }

    public String getGraphLineColor() {
        return this.lineColor;
    }
}
