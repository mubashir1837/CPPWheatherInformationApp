package com.weatherapp.utils;

public class IconManager {
    public static String getWeatherIconAndDescription(int code) {
        switch (code) {
            case 0: return "☀️ Clear sky";
            case 1: case 2: case 3: return "⛅ Partly cloudy";
            case 45: case 48: return "🌫️ Fog";
            case 51: case 53: case 55: return "🌧️ Drizzle";
            case 61: case 63: case 65: return "🌧️ Rain";
            case 71: case 73: case 75: return "❄️ Snow";
            case 95: return "⛈️ Thunderstorm";
            default: return "☁️ Unknown";
        }
    }
}
