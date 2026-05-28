package com.weatherapp;

import com.weatherapp.ui.WeatherDashboard;
import javax.swing.SwingUtilities;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new WeatherDashboard().setVisible(true);
        });
    }
}
