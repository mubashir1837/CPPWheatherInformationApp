package com.weatherapp.service;

import com.weatherapp.model.ForecastData;
import com.weatherapp.utils.JsonParser;

public class ForecastService {
    public ForecastData getShortTermForecast(String weatherJson) {
        try {
            int timeIndex = weatherJson.indexOf("\"daily\":");
            String dailyPart = weatherJson.substring(timeIndex);
            double maxTemp = JsonParser.extractDouble(dailyPart, "\"temperature_2m_max\":[");
            double minTemp = JsonParser.extractDouble(dailyPart, "\"temperature_2m_min\":[");
            return new ForecastData(minTemp, maxTemp);
        } catch (Exception e) {
            return null;
        }
    }
}
