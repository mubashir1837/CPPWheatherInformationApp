#include "MainWindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), networkManager(new QNetworkAccessManager(this))
{
    setupUI();
    applyStyles();
    
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(searchInput, &QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);
    
    // Automatically fetch user's location on startup
    fetchCurrentLocation();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    resize(550, 850);
    setMinimumSize(450, 600);
    setWindowTitle("Modern Weather App");
    setWindowIcon(QIcon(":/src/assets/logo-main.png"));

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");
    setCentralWidget(scrollArea);

    centralWidget = new QWidget(scrollArea);
    centralWidget->setObjectName("centralWidget");
    scrollArea->setWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    // App Logo & Title
    QHBoxLayout* logoLayout = new QHBoxLayout();
    logoLayout->setAlignment(Qt::AlignCenter);
    logoLayout->setSpacing(15);
    
    logoLabel = new QLabel(centralWidget);
    originalLogoPixmap = QPixmap(":/src/assets/logo-main.png");
    logoLabel->setPixmap(originalLogoPixmap.scaledToHeight(60, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    logoLayout->addWidget(logoLabel);

    QLabel* appTitleLabel = new QLabel("Weather Mind", centralWidget);
    appTitleLabel->setObjectName("appTitleLabel");
    appTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    logoLayout->addWidget(appTitleLabel);
    
    mainLayout->addLayout(logoLayout);

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
    weatherCard = new QFrame(centralWidget);
    weatherCard->setObjectName("weatherCard");
    weatherCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QVBoxLayout* cardLayout = new QVBoxLayout(weatherCard);
    cardLayout->setContentsMargins(30, 40, 30, 40);
    cardLayout->setSpacing(15);

    cityLabel = new QLabel("City Name", weatherCard);
    cityLabel->setObjectName("cityLabel");
    cityLabel->setAlignment(Qt::AlignCenter);
    
    dateTimeLabel = new QLabel("Date & Time", weatherCard);
    dateTimeLabel->setObjectName("dateTimeLabel");
    dateTimeLabel->setAlignment(Qt::AlignCenter);
    
    tempLabel = new QLabel("--°C", weatherCard);
    tempLabel->setObjectName("tempLabel");
    tempLabel->setAlignment(Qt::AlignCenter);
    
    descLabel = new QLabel("Ready to search", weatherCard);
    descLabel->setObjectName("descLabel");
    descLabel->setAlignment(Qt::AlignCenter);

    detailsLabel = new QLabel("Wind: -- km/hr | Humidity: --%", weatherCard);
    detailsLabel->setObjectName("detailsLabel");
    detailsLabel->setAlignment(Qt::AlignCenter);

    aqiLabel = new QLabel("", weatherCard);
    aqiLabel->setObjectName("aqiLabel");
    aqiLabel->setAlignment(Qt::AlignCenter);
    aqiLabel->hide();

    cardLayout->addWidget(cityLabel);
    cardLayout->addWidget(dateTimeLabel);
    cardLayout->addWidget(tempLabel);
    cardLayout->addWidget(descLabel);
    cardLayout->addWidget(detailsLabel);
    cardLayout->addWidget(aqiLabel);

    mainLayout->addWidget(weatherCard);
    
    // Forecast Section
    forecastTitle = new QLabel("7-Day Forecast", this);
    forecastTitle->setObjectName("forecastTitle");
    forecastTitle->setAlignment(Qt::AlignLeft);
    forecastTitle->hide(); // Hide initially
    mainLayout->addWidget(forecastTitle);

    forecastScroll = new QScrollArea(this);
    forecastScroll->setObjectName("forecastScroll");
    forecastScroll->setWidgetResizable(true);
    forecastScroll->setFixedHeight(130);
    forecastScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    forecastScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    forecastScroll->setFrameShape(QFrame::NoFrame);
    forecastScroll->hide(); // Hide initially
    
    forecastWidget = new QWidget(forecastScroll);
    forecastWidget->setObjectName("forecastWidget");
    forecastLayout = new QHBoxLayout(forecastWidget);
    forecastLayout->setContentsMargins(0, 0, 0, 0);
    forecastLayout->setSpacing(15);
    forecastLayout->setAlignment(Qt::AlignLeft);
    
    forecastScroll->setWidget(forecastWidget);
    mainLayout->addWidget(forecastScroll);
    
    // Chart Section
    chartTitle = new QLabel("80+ Years Historical Temperature Trend (1940-2023)", this);
    chartTitle->setObjectName("chartTitle");
    chartTitle->setAlignment(Qt::AlignLeft);
    chartTitle->hide();
    mainLayout->addWidget(chartTitle);

    chartView = new QChartView(centralWidget);
    chartView->setObjectName("chartView");
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(120);
    chartView->hide();
    mainLayout->addWidget(chartView);
    
    // Bottom Spacer
    mainLayout->addStretch();
}

void MainWindow::applyStyles()
{
    // Beautiful Modern Dark Theme
    QString qss = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1A2980, stop:1 #26D0CE);
        }
        #centralWidget {
            background: transparent;
        }
        #appTitleLabel {
            color: #FFFFFF;
            font-size: 38px;
            font-weight: 900;
            letter-spacing: 2px;
        }
        QLineEdit {
            background-color: rgba(255, 255, 255, 30);
            color: white;
            border: 1px solid rgba(255, 255, 255, 60);
            border-radius: 8px;
            padding: 0 15px;
            font-size: 16px;
        }
        QLineEdit:focus {
            border: 2px solid rgba(255, 255, 255, 120);
            background-color: rgba(255, 255, 255, 40);
        }
        QPushButton {
            background-color: rgba(255, 255, 255, 50);
            color: white;
            border: 1px solid rgba(255, 255, 255, 80);
            border-radius: 8px;
            padding: 0 20px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 80);
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 100);
        }
        #weatherCard {
            background-color: rgba(255, 255, 255, 20);
            border-radius: 16px;
            border: 1px solid rgba(255, 255, 255, 50);
        }
        #cityLabel {
            color: white;
            font-size: 28px;
            font-weight: bold;
        }
        #dateTimeLabel {
            color: #E0E0E0;
            font-size: 16px;
            font-style: italic;
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
        #aqiLabel {
            font-size: 16px;
            font-weight: bold;
            margin-top: 5px;
        }
        #forecastTitle, #chartTitle {
            color: #E0E0E0;
            font-size: 18px;
            font-weight: bold;
            margin-top: 15px;
        }
        #forecastScroll {
            background: transparent;
        }
        #forecastWidget {
            background: transparent;
        }
        #forecastDayCard {
            background-color: rgba(255, 255, 255, 20);
            border-radius: 12px;
            border: 1px solid rgba(255, 255, 255, 40);
        }
        #dayLabel {
            color: #BDBDBD;
            font-size: 14px;
            font-weight: bold;
        }
        #dayTempLabel {
            color: white;
            font-size: 16px;
            font-weight: bold;
        }
        #dayDescLabel {
            color: #FFCA28;
            font-size: 12px;
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
    aqiLabel->hide();
    chartTitle->hide();
    chartView->hide();
    clearForecastLayout();
    
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
    query.addQueryItem("daily", "temperature_2m_max,temperature_2m_min,weathercode");
    query.addQueryItem("timezone", "auto");
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);
    
    // To pass city name safely, capture it
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onWeatherFetched(reply);
    });
    
    // --- AQI Request ---
    QUrl aqiUrl("https://air-quality-api.open-meteo.com/v1/air-quality");
    QUrlQuery aqiQuery;
    aqiQuery.addQueryItem("latitude", QString::number(latitude, 'f', 6));
    aqiQuery.addQueryItem("longitude", QString::number(longitude, 'f', 6));
    aqiQuery.addQueryItem("current", "us_aqi");
    aqiUrl.setQuery(aqiQuery);
    QNetworkRequest aqiReq(aqiUrl);
    QNetworkReply* aqiReply = networkManager->get(aqiReq);
    connect(aqiReply, &QNetworkReply::finished, this, [this, aqiReply]() {
        aqiReply->deleteLater();
        if (aqiReply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(aqiReply->readAll());
            if (doc.isObject() && doc.object().contains("current")) {
                int aqi = doc.object()["current"].toObject()["us_aqi"].toInt();
                QString quality = "Good";
                QString color = "#4CAF50";
                if (aqi > 50) { quality = "Moderate"; color = "#FFC107"; }
                if (aqi > 100) { quality = "Unhealthy"; color = "#FF9800"; }
                if (aqi > 150) { quality = "Very Unhealthy"; color = "#F44336"; }
                if (aqi > 200) { quality = "Hazardous"; color = "#9C27B0"; }
                aqiLabel->setText(QString("AQI: %1 (<font color='%2'>%3</font>)").arg(aqi).arg(color).arg(quality));
                aqiLabel->show();
            }
        }
    });

    // --- Historical Archive Request ---
    QUrl histUrl("https://archive-api.open-meteo.com/v1/archive");
    QUrlQuery histQuery;
    histQuery.addQueryItem("latitude", QString::number(latitude, 'f', 6));
    histQuery.addQueryItem("longitude", QString::number(longitude, 'f', 6));
    histQuery.addQueryItem("start_date", "1940-01-01");
    histQuery.addQueryItem("end_date", "2023-12-31");
    histQuery.addQueryItem("daily", "temperature_2m_max");
    histUrl.setQuery(histQuery);
    QNetworkRequest histReq(histUrl);
    QNetworkReply* histReply = networkManager->get(histReq);
    connect(histReply, &QNetworkReply::finished, this, [this, histReply]() {
        histReply->deleteLater();
        if (histReply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(histReply->readAll());
            if (doc.isObject() && doc.object().contains("daily")) {
                QJsonArray temps = doc.object()["daily"].toObject()["temperature_2m_max"].toArray();
                QVector<double> yearlyAverages;
                double sum = 0;
                int count = 0;
                for (int i = 0; i < temps.size(); ++i) {
                    if (!temps[i].isNull()) {
                        sum += temps[i].toDouble();
                        count++;
                    }
                    if ((i + 1) % 365 == 0) { // Year avg
                        yearlyAverages.push_back(count > 0 ? (sum / count) : 0);
                        sum = 0; count = 0;
                    }
                }
                drawHistoricalChart(yearlyAverages);
            }
        }
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
    
    QString timeStr = currentWeather["time"].toString();
    QDateTime dateTime = QDateTime::fromString(timeStr, Qt::ISODate);
    if (dateTime.isValid()) {
        dateTimeLabel->setText(dateTime.toString("dddd, d MMMM yyyy | hh:mm AP"));
    } else {
        dateTimeLabel->setText("Current Time");
    }

    double temperature = currentWeather["temperature"].toDouble();
    double windspeed = currentWeather["windspeed"].toDouble();
    int weathercode = currentWeather["weathercode"].toInt();
    
    QString conditions = interpretWeatherCode(weathercode);

    tempLabel->setText(QString::number(temperature, 'f', 1) + "°C");
    descLabel->setText(conditions);
    detailsLabel->setText(QString("Wind: %1 km/hr").arg(windspeed));
    
    // Force weatherCard to recalculate size after text changes
    weatherCard->adjustSize();
    
    clearForecastLayout();
    
    if (jsonObj.contains("daily")) {
        forecastTitle->show();
        forecastScroll->show();
        
        QJsonObject daily = jsonObj["daily"].toObject();
        QJsonArray times = daily["time"].toArray();
        QJsonArray maxTemps = daily["temperature_2m_max"].toArray();
        QJsonArray minTemps = daily["temperature_2m_min"].toArray();
        QJsonArray codes = daily["weathercode"].toArray();
        
        for (int i = 0; i < times.size(); ++i) {
            QFrame* dayCard = new QFrame(forecastWidget);
            dayCard->setObjectName("forecastDayCard");
            dayCard->setFixedSize(110, 130);
            
            QVBoxLayout* dayLayout = new QVBoxLayout(dayCard);
            dayLayout->setAlignment(Qt::AlignCenter);
            dayLayout->setSpacing(5);
            
            QDate date = QDate::fromString(times[i].toString(), "yyyy-MM-dd");
            QString dayStr = date.toString("ddd");
            if (i == 0) dayStr = "Today";
            
            QLabel* dayLabel = new QLabel(dayStr, dayCard);
            dayLabel->setObjectName("dayLabel");
            dayLabel->setAlignment(Qt::AlignCenter);
            
            QLabel* dayDescLabel = new QLabel(interpretWeatherCode(codes[i].toInt()), dayCard);
            dayDescLabel->setObjectName("dayDescLabel");
            dayDescLabel->setAlignment(Qt::AlignCenter);
            dayDescLabel->setWordWrap(true);
            
            double minT = minTemps[i].toDouble();
            double maxT = maxTemps[i].toDouble();
            QLabel* dayTempLabel = new QLabel(QString("%1° / %2°").arg(qRound(maxT)).arg(qRound(minT)), dayCard);
            dayTempLabel->setObjectName("dayTempLabel");
            dayTempLabel->setAlignment(Qt::AlignCenter);
            
            dayLayout->addWidget(dayLabel);
            dayLayout->addWidget(dayDescLabel);
            dayLayout->addWidget(dayTempLabel);
            
            forecastLayout->addWidget(dayCard);
        }
    }
}

void MainWindow::clearForecastLayout()
{
    if (!forecastLayout) return;
    QLayoutItem* item;
    while ((item = forecastLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

QString MainWindow::interpretWeatherCode(int code)
{
    if (code > 0 && code <= 3) return "Partly Cloudy";
    else if (code >= 45 && code <= 48) return "Foggy";
    else if (code >= 51 && code <= 67) return "Rainy";
    else if (code >= 71 && code <= 77) return "Snowy";
    else if (code >= 95) return "Thunderstorm";
    return "Clear";
}

void MainWindow::drawHistoricalChart(const QVector<double>& data)
{
    if (data.isEmpty()) return;
    
    chartTitle->show();
    chartView->show();
    
    QLineSeries *series = new QLineSeries();
    for (int i = 0; i < data.size(); ++i) {
        series->append(1940 + i, data[i]);
    }
    
    QPen pen(QColor("#FFCA28"));
    pen.setWidth(2);
    series->setPen(pen);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();
    
    // Transparent background
    chart->setBackgroundBrush(Qt::transparent);
    chart->setPlotAreaBackgroundVisible(false);
    
    // Customize axes colors for dark theme
    QList<QAbstractAxis*> axes = chart->axes();
    for (auto axis : axes) {
        axis->setLabelsBrush(QColor("#BDBDBD"));
        axis->setGridLineColor(QColor(255, 255, 255, 30));
    }
    
    chartView->setChart(chart);
    chartView->setStyleSheet("background: transparent; border: none;");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (!originalLogoPixmap.isNull() && logoLabel) {
        // Responsively scale the logo based on the window size while keeping aspect ratio
        int targetHeight = qMax(40, this->height() / 12);
        QPixmap scaled = originalLogoPixmap.scaledToHeight(targetHeight, Qt::SmoothTransformation);
        logoLabel->setPixmap(scaled);
    }
}

void MainWindow::fetchCurrentLocation()
{
    cityLabel->setText("Detecting location...");
    descLabel->setText("Please wait...");
    
    QNetworkRequest request((QUrl("http://ip-api.com/json/")));
    QNetworkReply* reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onCurrentLocationFetched(reply);
    });
}

void MainWindow::onCurrentLocationFetched(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        cityLabel->setText("Unknown Location");
        descLabel->setText("Could not detect location automatically.");
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QString city = jsonObj["city"].toString();
    QString country = jsonObj["country"].toString();
    
    if (!city.isEmpty()) {
        // Automatically search for the detected city
        searchInput->setText(city + ", " + country);
        onSearchClicked();
    }
}
