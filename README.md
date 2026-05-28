# 🌤️ Modern C++ Weather Information App

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6.0%2B-41CD52.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen)

A fast, desktop weather application built entirely in **C++** using the **Qt6 Framework**. This high-performance GUI application leverages the open-source **Open-Meteo API** to deliver real-time, highly accurate meteorological data for any city across the globe. 

---

## Key Features

- **Real-Time Weather Tracking**: Get instant updates on current temperature, wind speed, and dynamic weather conditions (Clear, Cloudy, Rainy, Snow, etc.).
- **Smart Geocoding Algorithm**: Employs an intelligent prioritization algorithm that automatically prioritizes capital cities, sovereign nations, and high-population areas when searching for ambiguous location names (e.g., "Vatican").
- **Asynchronous API Integration**: Utilizes `QNetworkAccessManager` for non-blocking HTTP requests, ensuring the user interface remains flawlessly smooth and responsive at all times.
- **Premium Glassmorphism UI**: A gorgeous, custom-styled dark theme utilizing Qt Style Sheets (QSS), complete with rounded corners, interactive hover states, and smooth drop-shadow effects.
- **Native JSON Parsing**: Fast and efficient data extraction using Qt's built-in `QJsonDocument`, completely eliminating the need for third-party dependencies.

---

## Technology Stack

- **Language**: C++17
- **Framework**: Qt6 (Core, Gui, Widgets, Network)
- **Build System**: CMake (Minimum version 3.16)
- **Data Source**: Open-Meteo API & Open-Meteo Geocoding API

---

## Installation & Setup

### Prerequisites
To compile and run this application, ensure you have a C++ compiler, CMake, and the Qt6 development libraries installed.

**For Ubuntu/Debian Linux:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake qt6-base-dev
```

### Build Instructions

1. **Clone the repository** (or navigate to the project root directory).
2. **Create a build directory** to keep your source tree clean:
   ```bash
   mkdir build && cd build
   ```
3. **Configure the project** using CMake:
   ```bash
   cmake ..
   ```
4. **Compile the source code**:
   ```bash
   make
   ```

---

## 🌩️ Usage

After a successful build, launch the application from your terminal:

```bash
./WeatherInformationApp
```

1. Enter a city name, country, or specific location in the top search bar (e.g., `London`, `Tokyo`, `New York`).
2. Press **Enter** or click the **Search** button.
3. Instantly view the accurate, up-to-date weather profile on the beautifully designed interactive dashboard.

---

## Distribution & Releases

Because compiled binaries (like `.deb` files) are generally excluded from Git tracking (via `.gitignore`), we use **GitHub Releases** for distribution. 

### If you are the Developer (Releasing the app):
1. In your `build` directory, run:
   ```bash
   make package
   ```
2. Go to your GitHub Repository -> **Releases** -> **Draft a new release**.
3. Upload the generated `weather-information-app-1.0.0-Linux.deb` file as an asset to your release and publish it.

### If you are a User (Installing the app):
1. Go to the [Releases page](../../releases) of this repository.
2. Download the `.deb` installer file.
3. Double-click the downloaded file, or install it via terminal:
   ```bash
   sudo apt install ./weather-information-app-1.0.0-Linux.deb
   ```

---

## Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the issues page if you want to contribute.

## License
This project is open-source and available under the MIT License.
