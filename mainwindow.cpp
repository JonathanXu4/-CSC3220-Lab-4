#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QTimeZone>

int imageNum = 0;
QString zone = "UTC";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer),
    imageTimer(new QTimer),
    httpManager(new HTTPManager)
{
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(setCurrentTime()));

    connect(timer, SIGNAL(timeout()),
            this, SLOT(setGlobalTime()));

    connect(imageTimer, SIGNAL(timeout()),
            this, SLOT(changeImage()));

    setCurrentTime();
    changeImage();
    timer->start(1000);
    imageTimer->start(10000);


    connect(httpManager, SIGNAL(ImageReady(QPixmap *)),
            this, SLOT(processImage(QPixmap *)));

    connect(httpManager, SIGNAL(WeatherJsonReady(QJsonObject *)),
            this, SLOT(processWeatherJson(QJsonObject *)));

    connect(httpManager, SIGNAL(IconReady(QPixmap *)),
            this, SLOT(processIcon(QPixmap *)));

    connect(httpManager, SIGNAL(TimeJsonReady(QJsonObject *)),
                this, SLOT(processTimeJson(QJsonObject *)));

    QString zip = ui->zipcodeEdit->text();
    httpManager->sendImageRequest(zip);
    httpManager->sendWeatherRequest(zip);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCurrentTime()
{
    QTime time = QTime::currentTime();
    QString hour = time.toString("hh");
    QString minute = time.toString("mm");
    QString second = time.toString("ss");

    ui->hourNum->display(hour);
    ui->minNum->display(minute);
    ui->secNum->display(second);
}

void MainWindow::setGlobalTime()
{
    httpManager->sendTimeRequest(zone);
}

void MainWindow::changeImage()
{
    QString address = "C:/Users/Chari/Desktop/Qt/Dashboard/images/" + QString::number(imageNum) + ".png";
    ui->pictureLabel->setPixmap(QPixmap(address).scaled(1140,650));
    imageNum++;
    if (imageNum == 5) {
        imageNum = 0;
    }
}

void MainWindow::processImage(QPixmap *image)
{
    ui->imageLabel->setPixmap(QPixmap(*image).scaled(650,650,Qt::KeepAspectRatio));
}

void MainWindow::processIcon(QPixmap *image)
{
    ui->iconLabel->setPixmap(QPixmap(*image).scaled(150,150,Qt::KeepAspectRatio));
}

void MainWindow::processWeatherJson(QJsonObject *json)
{
    qDebug() << "Json ready";
    QString icon = json->value("weather").toArray()[0].toObject()["icon"].toString();
    double temp = json->value("main").toObject()["temp"].toDouble();
    double humidity = json->value("main").toObject()["humidity"].toDouble();

    httpManager->sendIconRequest(icon);
    ui->weatherLabel->setText(QString::number(temp) + "°");
    ui->humidLabel->setText("humidity: " + QString::number(humidity) + "°");

    /*
     * {
     * http://openweathermap.org/img/wn/ +
     * zip +
     * @2x.png
     *
     *    "coord": {"lon":-122.38,"lat":47.64},
     *    "weather":[{"id":803,"main":"Clouds","description":"broken clouds","icon":"04d"}],
     *    "base":"stations",
     *    "main":{"temp":288.38,"pressure":1015,"humidity":72,"temp_min":287.04,"temp_max":289.82},
     *    "visibility":16093,
     *    "wind":{"speed":2.22,"deg":329.191},
     *    "clouds":{"all":75},
     *    "dt":1558543054,
     *    "sys":{"type":1,"id":3417,"message":0.0113,"country":"US","sunrise":1558527857,"sunset":1558583303},
     *    "timezone":-25200,"id":420040214,"name":"Seattle","cod":200
     * }
     * */
}

void MainWindow::processTimeJson(QJsonObject *json)
{
    QString time = json->value("time_24").toString();
    ui->gHour->display(QString(time.at(0)) + QString(time.at(1)));
    ui->gMin->display(QString(time.at(3)) + QString(time.at(4)));
    ui->gSec->display(QString(time.at(6)) + QString(time.at(7)));
}

void MainWindow::on_updateButton_clicked()
{
    QString zip = ui->zipcodeEdit->text();
    qDebug() << zip;
    httpManager->sendImageRequest(zip);
    httpManager->sendWeatherRequest(zip);
}

void MainWindow::on_timeUp_clicked()
{
    zone = ui->upBox->text();
}
