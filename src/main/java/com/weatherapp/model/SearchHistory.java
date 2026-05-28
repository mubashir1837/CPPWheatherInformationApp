package com.weatherapp.model;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class SearchHistory {
    private String cityName;
    private String timestamp;

    public SearchHistory(String cityName) {
        this.cityName = cityName;
        this.timestamp = LocalDateTime.now().format(DateTimeFormatter.ofPattern("HH:mm:ss"));
    }

    @Override
    public String toString() {
        return "[" + timestamp + "] " + cityName;
    }
}
