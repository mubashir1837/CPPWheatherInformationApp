package com.weatherapp.ui;

import com.weatherapp.model.SearchHistory;
import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class SearchHistoryPanel extends JPanel {
    private JTextArea historyArea;
    private ArrayList<SearchHistory> historyList = new ArrayList<>();

    public SearchHistoryPanel() {
        setLayout(new BorderLayout());
        add(new JLabel("Search History:"), BorderLayout.NORTH);
        historyArea = new JTextArea(8, 20);
        historyArea.setEditable(false);
        add(new JScrollPane(historyArea), BorderLayout.CENTER);
    }

    public void addHistory(String city) {
        historyList.add(0, new SearchHistory(city));
        if (historyList.size() > 10) historyList.remove(10);
        
        StringBuilder sb = new StringBuilder();
        for (SearchHistory h : historyList) {
            sb.append(h.toString()).append("\n");
        }
        historyArea.setText(sb.toString());
    }
}
