package com.weatherapp.ui;

import com.weatherapp.model.WeatherData;
import com.weatherapp.model.ForecastData;
import com.weatherapp.service.WeatherService;
import com.weatherapp.service.ForecastService;
import com.weatherapp.service.BackgroundManager;
import com.weatherapp.service.UnitConverter;
import com.weatherapp.utils.IconManager;
import com.weatherapp.utils.ValidationUtil;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;

public class WeatherDashboard extends JFrame {
    private JTextField locationField;
    private JButton searchButton;
    private JPanel weatherDisplayPanel;
    private JLabel conditionLabel;
    private JLabel temperatureLabel;
    private JLabel humidityLabel;
    private JLabel windLabel;

    private SettingsPanel settingsPanel;
    private ForecastPanel forecastPanel;
    private SearchHistoryPanel historyPanel;

    private WeatherService weatherService = new WeatherService();
    private ForecastService forecastService = new ForecastService();
    private BackgroundManager backgroundManager = new BackgroundManager();

    private boolean isCelsius = true;
    private WeatherData currentData = null;

    public WeatherDashboard() {
        setTitle("Weather Information App");
        setSize(700, 700);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Top Panel
        JPanel topPanel = new JPanel(new FlowLayout());
        topPanel.setBackground(new Color(45, 52, 54));
        JLabel locationLabel = new JLabel("Location:");
        locationLabel.setForeground(Color.WHITE);
        topPanel.add(locationLabel);
        
        locationField = new JTextField(15);
        topPanel.add(locationField);
        
        searchButton = new JButton("Search");
        topPanel.add(searchButton);

        settingsPanel = new SettingsPanel();
        topPanel.add(settingsPanel);

        add(topPanel, BorderLayout.NORTH);

        // Center Panel
        weatherDisplayPanel = new JPanel();
        weatherDisplayPanel.setLayout(new BoxLayout(weatherDisplayPanel, BoxLayout.Y_AXIS));
        weatherDisplayPanel.setBorder(new EmptyBorder(20, 20, 20, 20));
        backgroundManager.applyBackground(weatherDisplayPanel, true, 0);

        conditionLabel = new JLabel("Enter a city to get weather.");
        conditionLabel.setFont(new Font("SansSerif", Font.BOLD, 24));
        conditionLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        
        temperatureLabel = new JLabel("-- °C");
        temperatureLabel.setFont(new Font("SansSerif", Font.BOLD, 48));
        temperatureLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        
        humidityLabel = new JLabel("Humidity: --%");
        humidityLabel.setFont(new Font("SansSerif", Font.PLAIN, 18));
        humidityLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        
        windLabel = new JLabel("Wind: -- km/h");
        windLabel.setFont(new Font("SansSerif", Font.PLAIN, 18));
        windLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        weatherDisplayPanel.add(conditionLabel);
        weatherDisplayPanel.add(Box.createRigidArea(new Dimension(0, 20)));
        weatherDisplayPanel.add(temperatureLabel);
        weatherDisplayPanel.add(Box.createRigidArea(new Dimension(0, 20)));
        weatherDisplayPanel.add(humidityLabel);
        weatherDisplayPanel.add(windLabel);

        add(weatherDisplayPanel, BorderLayout.CENTER);

        // Bottom Panel
        JPanel bottomPanel = new JPanel(new GridLayout(1, 2, 10, 0));
        bottomPanel.setBorder(new EmptyBorder(10, 10, 10, 10));

        forecastPanel = new ForecastPanel();
        historyPanel = new SearchHistoryPanel();

        bottomPanel.add(forecastPanel);
        bottomPanel.add(historyPanel);
        add(bottomPanel, BorderLayout.SOUTH);

        // Event Listeners
        searchButton.addActionListener(e -> fetchWeather());

        settingsPanel.getUnitToggleButton().addActionListener(e -> {
            isCelsius = !settingsPanel.getUnitToggleButton().isSelected();
            settingsPanel.getUnitToggleButton().setText(isCelsius ? "Switch to °F" : "Switch to °C");
            updateDisplay();
        });
    }

    private void fetchWeather() {
        String city = locationField.getText().trim();
        if (!ValidationUtil.isValidCityName(city)) {
            JOptionPane.showMessageDialog(this, "Please enter a location.", "Input Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        try {
            currentData = weatherService.getWeatherForCity(city);
            String rawJson = weatherService.getRawWeatherJson(city);
            ForecastData forecastData = forecastService.getShortTermForecast(rawJson);
            
            historyPanel.addHistory(city);
            forecastPanel.updateForecast(forecastData);
            
            updateDisplay();
            
            backgroundManager.applyBackground(weatherDisplayPanel, currentData.isDay(), currentData.getWeatherCode());
            
            Color textColor = currentData.isDay() ? Color.BLACK : Color.WHITE;
            conditionLabel.setForeground(textColor);
            temperatureLabel.setForeground(textColor);
            humidityLabel.setForeground(textColor);
            windLabel.setForeground(textColor);

        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve data: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void updateDisplay() {
        if (currentData == null) return;

        conditionLabel.setText(currentData.getCityName().toUpperCase() + " - " + IconManager.getWeatherIconAndDescription(currentData.getWeatherCode()));
        humidityLabel.setText("Humidity: " + currentData.getHumidity() + "%");

        if (isCelsius) {
            temperatureLabel.setText(String.format("%.1f °C", currentData.getTemperature()));
            windLabel.setText(String.format("Wind: %.1f km/h", currentData.getWindSpeed()));
        } else {
            double tempF = UnitConverter.celsiusToFahrenheit(currentData.getTemperature());
            double windMph = UnitConverter.kmhToMph(currentData.getWindSpeed());
            temperatureLabel.setText(String.format("%.1f °F", tempF));
            windLabel.setText(String.format("Wind: %.1f mph", windMph));
        }
    }
}
