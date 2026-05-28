package com.weatherapp.model;

public class ForecastData {
    private double minTemp;
    private double maxTemp;

    public ForecastData(double minTemp, double maxTemp) {
        this.minTemp = minTemp;
        this.maxTemp = maxTemp;
    }

    public double getMinTemp() { return minTemp; }
    public double getMaxTemp() { return maxTemp; }
}
