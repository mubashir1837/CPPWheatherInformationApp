package com.weatherapp.service;

import javax.swing.JPanel;
import java.awt.Color;

public class BackgroundManager {
    public void applyBackground(JPanel panel, boolean isDay, int weatherCode) {
        if (!isDay) {
            panel.setBackground(new Color(45, 52, 54)); // Night time
            return;
        }

        // Day backgrounds based on weather conditions
        switch (weatherCode) {
            case 0: 
                // Clear sky
                panel.setBackground(new Color(135, 206, 235)); // Sky blue
                break;
            case 1: case 2: case 3: 
                // Cloudy / partly cloudy
                panel.setBackground(new Color(176, 196, 222)); // Light steel blue
                break;
            case 45: case 48: 
                // Fog
                panel.setBackground(new Color(169, 169, 169)); // Dark gray
                break;
            case 51: case 53: case 55: 
            case 61: case 63: case 65: 
                // Rain / drizzle
                panel.setBackground(new Color(119, 136, 153)); // Light slate gray
                break;
            case 71: case 73: case 75: 
                // Snow
                panel.setBackground(new Color(240, 248, 255)); // Alice blue
                break;
            case 95: 
                // Thunderstorm
                panel.setBackground(new Color(72, 61, 139)); // Stormy dark blue/purple
                break;
            default:
                panel.setBackground(new Color(116, 185, 255)); // Default day blue
        }
    }
}
