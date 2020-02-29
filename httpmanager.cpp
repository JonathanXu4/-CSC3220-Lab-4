#include "httpmanager.h"
#include <QJsonDocument>
#include <QJsonObject>

HTTPManager::HTTPManager(QObject *parent) :
    QObject(parent),
    imageDownloadManager(new QNetworkAccessManager),
    weatherAPIManager(new QNetworkAccessManager),
    iconDownloadManager(new QNetworkAccessManager),
    timeDownloadManager(new QNetworkAccessManager)
{
    connect(imageDownloadManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(ImageDownloadedHandler(QNetworkReply*)));

    connect(weatherAPIManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(WeatherDownloadedHandler(QNetworkReply*)));

    connect(iconDownloadManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(IconDownloadedHandler(QNetworkReply*)));

    connect(timeDownloadManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(TimeDownloadedHandler(QNetworkReply*)));
}

HTTPManager::~HTTPManager()
{
    delete imageDownloadManager;
    delete weatherAPIManager;
}

void HTTPManager::sendImageRequest(QString zip)
{
    QNetworkRequest request;
    QString address = "https://dev.virtualearth.net/REST/V1/Imagery/Map/Road/" +
            zip +
            "/13?mapSize=650,650&mapLayer=TrafficFlow&format=png&key=AshZIhw3HksUsw9OSlQJNnF-6_Wqf2a8RTnAiVdWyKdL9BiRBBYC_TBWMxLQiDCt";

    request.setUrl(QUrl(address));
    imageDownloadManager->get(request);
    qDebug() << "Image Request Sent to Address " << request.url();
}

void HTTPManager::sendWeatherRequest(QString zip)
{
    QNetworkRequest request;
    QString address = "http://api.openweathermap.org/data/2.5/weather?zip=" +
            zip +
            ",us&units=imperial&appid=92bf69ee666fa67e27fb57300dcab879";

    request.setUrl(QUrl(address));
    weatherAPIManager->get(request);
    qDebug() << "Weather Request Sent to Address " << request.url();
}

void HTTPManager::sendIconRequest(QString icon)
{
    QNetworkRequest request;
    QString address = "http://openweathermap.org/img/wn/" +
            icon +
            "@2x.png";

    request.setUrl(QUrl(address));
    iconDownloadManager->get(request);
    qDebug() << "Icon Request Sent to Address " << request.url();
}

void HTTPManager::sendTimeRequest(QString zone)
{
    QNetworkRequest request;
    QString address = "https://api.ipgeolocation.io/timezone?apiKey=db1f6b667b1c464994039d9b75a2a0cf&tz=" + zone;
    request.setUrl(QUrl(address));
    timeDownloadManager->get(request);
}

// After receving the image
void HTTPManager::ImageDownloadedHandler(QNetworkReply *reply)
{
    qDebug() << "Image Reply has arrived";
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    qDebug() << "Download was successful";
    QPixmap *image = new QPixmap();
    image->loadFromData(reply->readAll());

    emit ImageReady(image);
}

// After receiving the weather
void HTTPManager::WeatherDownloadedHandler(QNetworkReply *reply)
{
    qDebug() << "Weather Reply has arrived";
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    qDebug() << "Download was successful";
    QString answer = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject *jsonObj = new QJsonObject(jsonResponse.object());

    emit WeatherJsonReady(jsonObj);
}

// After receving the icon
void HTTPManager::IconDownloadedHandler(QNetworkReply *reply)
{
    qDebug() << "Icon Reply has arrived";
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    qDebug() << "Download was successful";
    QPixmap *image = new QPixmap();
    image->loadFromData(reply->readAll());

    emit IconReady(image);
}

// After receving the time
void HTTPManager::TimeDownloadedHandler(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject *jsonObj = new QJsonObject(jsonResponse.object());

    emit TimeJsonReady(jsonObj);
}
