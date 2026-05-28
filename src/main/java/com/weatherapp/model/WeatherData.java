package com.weatherapp.model;

public class WeatherData {
    private String cityName;
    private double temperature;
    private double humidity;
    private double windSpeed;
    private int weatherCode;
    private boolean isDay;

    public WeatherData(String cityName, double temperature, double humidity, double windSpeed, int weatherCode, boolean isDay) {
        this.cityName = cityName;
        this.temperature = temperature;
        this.humidity = humidity;
        this.windSpeed = windSpeed;
        this.weatherCode = weatherCode;
        this.isDay = isDay;
    }

    public String getCityName() { return cityName; }
    public double getTemperature() { return temperature; }
    public double getHumidity() { return humidity; }
    public double getWindSpeed() { return windSpeed; }
    public int getWeatherCode() { return weatherCode; }
    public boolean isDay() { return isDay; }
}
