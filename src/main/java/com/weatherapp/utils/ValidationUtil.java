package com.weatherapp.utils;

public class ValidationUtil {
    public static boolean isValidCityName(String city) {
        return city != null && !city.trim().isEmpty();
    }
}
