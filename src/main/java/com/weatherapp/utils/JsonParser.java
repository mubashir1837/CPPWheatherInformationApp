package com.weatherapp.utils;

public class JsonParser {
    public static double extractDouble(String json, String key) {
        return extractDouble(json, null, key);
    }

    public static double extractDouble(String json, String startKey, String key) {
        int startIndex = 0;
        if (startKey != null) {
            startIndex = json.indexOf(startKey);
            if (startIndex == -1) startIndex = 0;
        }

        int index = json.indexOf(key, startIndex);
        if (index == -1) return 0.0;
        index += key.length();
        
        // Skip spaces or colons if any
        while (index < json.length() && (json.charAt(index) == ' ' || json.charAt(index) == ':')) {
            index++;
        }

        // If we hit a quote, it might be a string (like a unit), which we can't parse as double.
        if (index < json.length() && json.charAt(index) == '"') {
             // We hit a string value. Maybe the actual data we want is later in the JSON.
             // Try searching again from after this point.
             return extractDouble(json.substring(index), null, key);
        }
        
        int endIndex = index;
        while (endIndex < json.length() && (Character.isDigit(json.charAt(endIndex)) || json.charAt(endIndex) == '.' || json.charAt(endIndex) == '-')) {
            endIndex++;
        }
        
        try {
            return Double.parseDouble(json.substring(index, endIndex));
        } catch (Exception e) {
            return 0.0;
        }
    }
}
