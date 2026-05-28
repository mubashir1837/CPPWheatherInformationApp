# CS 1103-01 - AY2026-T4: Programming Assignment Unit 8
**Assignment Title:** Weather Information App
**GitHub Repository:** [https://github.com/mubashir1837/WheatherInformationApp](https://github.com/mubashir1837/WheatherInformationApp)

---

## 1. Overview
The **Weather Information App** is a fully functional Java Swing GUI application built using a structured MVC and Layered Architecture approach. It fetches and displays real-time weather data and short-term forecasts for any given location, fulfilling all assignment requirements.

## 2. Implementation Details (Addressing Grading Criteria)

### A. API Integration (20 Points)
- **API Used:** Open-Meteo Geocoding API and Open-Meteo Weather API.
- **Integration:** The app connects to the API using standard Java `HttpURLConnection` and parses the JSON response using a custom `JsonParser` class.
- **Data Fetched:** Successfully retrieves latitude/longitude, current temperature, relative humidity, wind speed, weather condition codes, and day/night status.
- **Note:** Open-Meteo is an open-source API that does not require an API key, which ensures smooth operation without authentication blocks.

### B. GUI Design (20 Points)
- **Framework:** Developed using Java Swing.
- **Layout:** Utilizes a clean, user-friendly interface with `BorderLayout`, `FlowLayout`, and `BoxLayout`.
- **Inputs & Controls:** Features a `JTextField` for city name input, a Search `JButton`, and a `JToggleButton` for toggling units.
- **Navigation:** The interface is divided logically: Top panel for controls, Center panel for current weather, and Bottom panel for forecasts and history.

### C. Logic and Computation (20 Points)
- **Core Operations:** The app accurately takes string input, converts it to geographical coordinates, fetches the weather forecast, parses the data, and maps numeric WMO weather codes to descriptive text and visual Unicode icons (e.g., ☀️ Clear sky, 🌧️ Rain).
- **Unit Conversion:** Accurately converts between metric (°C, km/h) and imperial (°F, mph) dynamically when the user clicks the toggle button, without needing to re-fetch data.
- **Dynamic Backgrounds:** Features a `BackgroundManager` that actively reads the `weatherCode` provided by the API and maps it to different sky colors during the day (e.g., bright blue for clear skies, dark purple for thunderstorms), while switching to a dark background for nighttime.

### D. Program Flow and Structure (20 Points)
- **Architecture:** The project follows strict modular design principles:
  - `ui/`: Contains all visual components (Dashboard, Forecast, History, Settings).
  - `api/`: Manages API URLs and HTTP connections.
  - `model/`: Defines data structures (WeatherData, ForecastData).
  - `service/`: Contains core business logic (WeatherService, ForecastService, UnitConverter, BackgroundManager).
  - `utils/`: Houses utility functions like `JsonParser` and `IconManager`.
- **Error Handling:** Custom exceptions (`ApiException`, `InvalidLocationException`) are implemented to prevent crashes. If a user enters an invalid location (e.g., "East Antarctic Plateau"), the app cleanly throws a `JOptionPane` warning dialog.

### E. Output (10 Points)
- **Screenshots:** The program successfully displays output. *(Please see the attached `dashboard.png` in the submission files / `screenshots` folder).*
- **README:** A comprehensive `README.md` is included in the repository with instructions on how to compile and run the application.

### F. Code Style and Readability (10 Points)
- **Readability:** The code features consistent indentation and formatting conventions.
- **Variable Naming:** Meaningful variable and method names are used throughout the project (e.g., `weatherDisplayPanel`, `currentTempC`, `updateDisplay()`).
- **Comments:** The code is thoroughly commented to explain logic, API calls, and GUI structuring. No redundant or unused code is present in the final submission.

---

## 3. How to Run (Brief)
Using Maven:
```bash
mvn clean compile
mvn exec:java -Dexec.mainClass="com.weatherapp.Main"
```
Or directly using `javac`:
```bash
javac -d out $(find src -name "*.java")
java -cp out com.weatherapp.Main
```
