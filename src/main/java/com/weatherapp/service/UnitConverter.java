package com.weatherapp.service;

public class UnitConverter {
    public static double celsiusToFahrenheit(double celsius) {
        return (celsius * 9 / 5) + 32;
    }

    public static double kmhToMph(double kmh) {
        return kmh * 0.621371;
    }
}
