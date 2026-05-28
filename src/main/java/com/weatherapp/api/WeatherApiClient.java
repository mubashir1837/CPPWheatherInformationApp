package com.weatherapp.api;

import com.weatherapp.exception.ApiException;
import com.weatherapp.exception.InvalidLocationException;
import com.weatherapp.utils.JsonParser;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class WeatherApiClient {

    public String fetchGeocoding(String city) throws ApiException, InvalidLocationException {
        String urlStr = ApiConfig.GEOCODING_API_URL + "?name=" + city.replace(" ", "+") + "&count=1&format=json";
        String response = makeRequest(urlStr);
        if (response == null || !response.contains("\"latitude\"")) {
            throw new InvalidLocationException("Location not found.");
        }
        return response;
    }

    public String fetchWeather(double lat, double lon) throws ApiException {
        String urlStr = ApiConfig.WEATHER_API_URL + "?latitude=" + lat + "&longitude=" + lon 
                + "&current=temperature_2m,relative_humidity_2m,wind_speed_10m,weather_code,is_day"
                + "&daily=weather_code,temperature_2m_max,temperature_2m_min&timezone=auto";
        String response = makeRequest(urlStr);
        if (response == null) {
            throw new ApiException("Failed to fetch weather data.");
        }
        return response;
    }

    private String makeRequest(String urlString) throws ApiException {
        try {
            URL url = new URL(urlString);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            if (conn.getResponseCode() != 200) {
                return null;
            }
            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            String inputLine;
            StringBuilder content = new StringBuilder();
            while ((inputLine = in.readLine()) != null) {
                content.append(inputLine);
            }
            in.close();
            conn.disconnect();
            return content.toString();
        } catch (Exception e) {
            throw new ApiException("HTTP request failed", e);
        }
    }
}
