package com.weatherapp.service;

import com.weatherapp.api.WeatherApiClient;
import com.weatherapp.exception.ApiException;
import com.weatherapp.exception.InvalidLocationException;
import com.weatherapp.model.WeatherData;
import com.weatherapp.utils.JsonParser;

public class WeatherService {
    private WeatherApiClient apiClient = new WeatherApiClient();

    public WeatherData getWeatherForCity(String city) throws ApiException, InvalidLocationException {
        String geoJson = apiClient.fetchGeocoding(city);
        double lat = JsonParser.extractDouble(geoJson, "\"latitude\":");
        double lon = JsonParser.extractDouble(geoJson, "\"longitude\":");

        String weatherJson = apiClient.fetchWeather(lat, lon);
        
        double temp = JsonParser.extractDouble(weatherJson, "\"temperature_2m\":");
        double humidity = JsonParser.extractDouble(weatherJson, "\"relative_humidity_2m\":");
        double wind = JsonParser.extractDouble(weatherJson, "\"wind_speed_10m\":");
        int code = (int) JsonParser.extractDouble(weatherJson, "\"weather_code\":");
        boolean isDay = ((int) JsonParser.extractDouble(weatherJson, "\"is_day\":")) == 1;

        return new WeatherData(city, temp, humidity, wind, code, isDay);
    }
    
    public String getRawWeatherJson(String city) throws ApiException, InvalidLocationException {
        String geoJson = apiClient.fetchGeocoding(city);
        double lat = JsonParser.extractDouble(geoJson, "\"latitude\":");
        double lon = JsonParser.extractDouble(geoJson, "\"longitude\":");
        return apiClient.fetchWeather(lat, lon);
    }
}
