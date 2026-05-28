package com.weatherapp.ui;

import javax.swing.*;
import java.awt.*;

public class SettingsPanel extends JPanel {
    private JToggleButton unitToggleButton;

    public SettingsPanel() {
        setLayout(new FlowLayout());
        unitToggleButton = new JToggleButton("Switch to °F");
        add(unitToggleButton);
    }

    public JToggleButton getUnitToggleButton() {
        return unitToggleButton;
    }
}
