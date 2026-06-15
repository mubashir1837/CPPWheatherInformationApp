#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFrame>
#include <QScrollArea>
#include <QDate>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QVector>
#include <QVector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchClicked();
    void onLocationGeocoded(QNetworkReply* reply);
    void onWeatherFetched(QNetworkReply* reply);
    void fetchCurrentLocation();
    void onCurrentLocationFetched(QNetworkReply* reply);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUI();
    void applyStyles();
    void fetchCoordinates(const QString& cityName);
    void fetchWeather(double latitude, double longitude, const QString& cityName);
    void clearForecastLayout();
    QString interpretWeatherCode(int code);
    void drawHistoricalChart(const QVector<double>& data);

    QWidget* centralWidget;
    QLineEdit* searchInput;
    QPushButton* searchButton;
    
    QLabel* logoLabel;
    QPixmap originalLogoPixmap;
    
    QFrame* weatherCard;
    QLabel* cityLabel;
    QLabel* dateTimeLabel;
    QLabel* tempLabel;
    QLabel* descLabel;
    QLabel* detailsLabel;
    QLabel* aqiLabel;
    
    QScrollArea* forecastScroll;
    QLabel* forecastTitle;
    QWidget* forecastWidget;
    QHBoxLayout* forecastLayout;
    
    QLabel* chartTitle;
    QChartView* chartView;
    
    QNetworkAccessManager* networkManager;
};

#endif
