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

private:
    void setupUI();
    void applyStyles();
    void fetchCoordinates(const QString& cityName);
    void fetchWeather(double latitude, double longitude, const QString& cityName);

    QWidget* centralWidget;
    QLineEdit* searchInput;
    QPushButton* searchButton;
    
    QFrame* weatherCard;
    QLabel* cityLabel;
    QLabel* tempLabel;
    QLabel* descLabel;
    QLabel* detailsLabel;
    
    QNetworkAccessManager* networkManager;
};

#endif
