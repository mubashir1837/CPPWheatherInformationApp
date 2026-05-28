#include "MainWindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), networkManager(new QNetworkAccessManager(this))
{
    setupUI();
    applyStyles();
    
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(searchInput, &QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    resize(500, 600);
    setWindowTitle("Modern Weather App");

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignTop);

    // Search bar layout
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(10);
    
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Enter city name (e.g., London)...");
    searchInput->setMinimumHeight(45);
    
    searchButton = new QPushButton("Search", this);
    searchButton->setMinimumHeight(45);
    searchButton->setCursor(Qt::PointingHandCursor);
    
    searchLayout->addWidget(searchInput, 1);
    searchLayout->addWidget(searchButton);
    mainLayout->addLayout(searchLayout);

    // Spacer
    mainLayout->addSpacing(20);

    // Weather Card
    weatherCard = new QFrame(this);
    weatherCard->setObjectName("weatherCard");
    QVBoxLayout* cardLayout = new QVBoxLayout(weatherCard);
    cardLayout->setContentsMargins(30, 40, 30, 40);
    cardLayout->setSpacing(15);
    cardLayout->setAlignment(Qt::AlignCenter);

    // Drop shadow for card
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 8);
    weatherCard->setGraphicsEffect(shadow);

    cityLabel = new QLabel("City Name", this);
    cityLabel->setObjectName("cityLabel");
    cityLabel->setAlignment(Qt::AlignCenter);
    
    tempLabel = new QLabel("--°C", this);
    tempLabel->setObjectName("tempLabel");
    tempLabel->setAlignment(Qt::AlignCenter);
    
    descLabel = new QLabel("Ready to search", this);
    descLabel->setObjectName("descLabel");
    descLabel->setAlignment(Qt::AlignCenter);

    detailsLabel = new QLabel("Wind: -- km/hr | Humidity: --%", this);
    detailsLabel->setObjectName("detailsLabel");
    detailsLabel->setAlignment(Qt::AlignCenter);

    cardLayout->addWidget(cityLabel);
    cardLayout->addWidget(tempLabel);
    cardLayout->addWidget(descLabel);
    cardLayout->addWidget(detailsLabel);

    mainLayout->addWidget(weatherCard);
    
    // Bottom Spacer
    mainLayout->addStretch();
}

void MainWindow::applyStyles()
{
    // Beautiful Modern Dark Theme
    QString qss = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1E1E2F, stop:1 #2D2D44);
        }
        QLineEdit {
            background-color: rgba(255, 255, 255, 0.1);
            color: white;
            border: 2px solid rgba(255, 255, 255, 0.2);
            border-radius: 8px;
            padding: 0 15px;
            font-size: 16px;
        }
        QLineEdit:focus {
            border: 2px solid #5C6BC0;
            background-color: rgba(255, 255, 255, 0.15);
        }
        QPushButton {
            background-color: #5C6BC0;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 0 20px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3F51B5;
        }
        QPushButton:pressed {
            background-color: #303F9F;
        }
        #weatherCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 0.1), stop:1 rgba(255, 255, 255, 0.05));
            border-radius: 16px;
            border: 1px solid rgba(255, 255, 255, 0.15);
        }
        #cityLabel {
            color: white;
            font-size: 28px;
            font-weight: bold;
        }
        #tempLabel {
            color: #FFCA28;
            font-size: 64px;
            font-weight: bold;
            margin: 10px 0;
        }
        #descLabel {
            color: #E0E0E0;
            font-size: 20px;
            font-style: italic;
        }
        #detailsLabel {
            color: #BDBDBD;
            font-size: 14px;
            margin-top: 10px;
        }
    )";
    this->setStyleSheet(qss);
}

void MainWindow::onSearchClicked()
{
    QString cityName = searchInput->text().trimmed();
    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a city name.");
        return;
    }

    cityLabel->setText("Loading...");
    tempLabel->setText("--°C");
    descLabel->setText("Fetching coordinates...");
    detailsLabel->setText("");
    
    fetchCoordinates(cityName);
}

void MainWindow::fetchCoordinates(const QString& cityName)
{
    QUrl url("https://geocoding-api.open-meteo.com/v1/search");
    QUrlQuery query;
    query.addQueryItem("name", cityName);
    query.addQueryItem("count", "10");
    query.addQueryItem("format", "json");
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onLocationGeocoded(reply);
    });
}

void MainWindow::onLocationGeocoded(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        cityLabel->setText("Error");
        descLabel->setText("Network error while geocoding location.");
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        cityLabel->setText("Error");
        descLabel->setText("Invalid geocoding response.");
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    if (!jsonObj.contains("results")) {
        cityLabel->setText("Not Found");
        descLabel->setText("Location not found.");
        return;
    }

    QJsonArray results = jsonObj["results"].toArray();
    if (results.isEmpty()) {
        cityLabel->setText("Not Found");
        descLabel->setText("Location not found.");
        return;
    }

    QJsonObject bestLocation;
    int bestScore = -1;

    // Use a custom scoring system to pick the most relevant location
    // e.g., prefer Capitals (PPLC), Countries/States, or high population
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject loc = results[i].toObject();
        int score = 0;
        
        QString featureCode = loc["feature_code"].toString();
        int population = loc["population"].toInt();
        
        if (featureCode == "PPLC" || featureCode == "PCLI") score += 1000;
        else if (featureCode.startsWith("PPLA")) score += 500;
        else if (featureCode == "PPL") score += 100;
        
        // Add some weight for population
        score += (population / 10000); 

        if (score > bestScore) {
            bestScore = score;
            bestLocation = loc;
        }
    }

    if (bestScore == -1) bestLocation = results[0].toObject();

    double latitude = bestLocation["latitude"].toDouble();
    double longitude = bestLocation["longitude"].toDouble();
    QString name = bestLocation["name"].toString();
    QString country = bestLocation["country"].toString();
    
    QString fullName = name;
    if (!country.isEmpty()) fullName += ", " + country;

    cityLabel->setText(fullName);
    descLabel->setText("Fetching weather...");
    
    fetchWeather(latitude, longitude, fullName);
}

void MainWindow::fetchWeather(double latitude, double longitude, const QString& cityName)
{
    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", QString::number(latitude, 'f', 6));
    query.addQueryItem("longitude", QString::number(longitude, 'f', 6));
    query.addQueryItem("current_weather", "true");
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);
    
    // To pass city name safely, capture it
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onWeatherFetched(reply);
    });
}

void MainWindow::onWeatherFetched(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        descLabel->setText("Network error while fetching weather.");
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        descLabel->setText("Invalid weather response.");
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    if (!jsonObj.contains("current_weather")) {
        descLabel->setText("Weather data missing.");
        return;
    }

    QJsonObject currentWeather = jsonObj["current_weather"].toObject();
    double temperature = currentWeather["temperature"].toDouble();
    double windspeed = currentWeather["windspeed"].toDouble();
    int weathercode = currentWeather["weathercode"].toInt();
    
    // Interpret simple weather code
    QString conditions = "Clear";
    if (weathercode > 0 && weathercode <= 3) conditions = "Partly Cloudy";
    else if (weathercode >= 45 && weathercode <= 48) conditions = "Foggy";
    else if (weathercode >= 51 && weathercode <= 67) conditions = "Rainy";
    else if (weathercode >= 71 && weathercode <= 77) conditions = "Snowy";
    else if (weathercode >= 95) conditions = "Thunderstorm";

    tempLabel->setText(QString::number(temperature, 'f', 1) + "°C");
    descLabel->setText(conditions);
    detailsLabel->setText(QString("Wind: %1 km/hr").arg(windspeed));
}
