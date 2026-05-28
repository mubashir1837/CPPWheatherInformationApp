package com.weatherapp.ui;

import com.weatherapp.model.ForecastData;
import javax.swing.*;
import java.awt.*;

public class ForecastPanel extends JPanel {
    private JTextArea forecastArea;

    public ForecastPanel() {
        setLayout(new BorderLayout());
        add(new JLabel("Short-term Forecast:"), BorderLayout.NORTH);
        forecastArea = new JTextArea(8, 20);
        forecastArea.setEditable(false);
        add(new JScrollPane(forecastArea), BorderLayout.CENTER);
    }

    public void updateForecast(ForecastData data) {
        if (data != null) {
            forecastArea.setText("Today:\nMin: " + data.getMinTemp() + "°C\nMax: " + data.getMaxTemp() + "°C\n\n(Data by Open-Meteo)");
        } else {
            forecastArea.setText("Forecast unavailable.");
        }
    }
}
