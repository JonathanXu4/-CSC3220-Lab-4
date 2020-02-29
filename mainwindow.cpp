#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer),
    httpManager(new HTTPManager)
{
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(setCurrentTime()));

    setCurrentTime();
    timer->start(1000);


    connect(httpManager, SIGNAL(ImageReady(QPixmap *)),
            this, SLOT(processImage(QPixmap *)));

    connect(httpManager, SIGNAL(WeatherJsonReady(QJsonObject *)),
            this, SLOT(processWeatherJson(QJsonObject *)));

    connect(httpManager, SIGNAL(IconReady(QPixmap *)),
            this, SLOT(processIcon(QPixmap *)));
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
    ui->minNum->display(hour);
    ui->secNum->display(second);
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
    QString weather = json->value("weather").toArray()[0].toObject()["main"].toString();
    QString weatherDesc = json->value("weather").toArray()[0].toObject()["description"].toString();
    QString icon = json->value("weather").toArray()[0].toObject()["icon"].toString();
    double temp = json->value("main").toObject()["temp"].toDouble();
    double temp_min = json->value("main").toObject()["temp_min"].toDouble();
    double temp_max = json->value("main").toObject()["temp_max"].toDouble();
    double humidity = json->value("main").toObject()["humidity"].toDouble();


    qDebug() << weather;
    qDebug() << weatherDesc;
    qDebug() << temp;
    qDebug() << temp_min;
    qDebug() << temp_max;
    qDebug() << humidity;

    httpManager->sendIconRequest(icon);
    ui->weatherLabel->setText(QString::number(temp) + "°");
    //ui->weatherLabel->setText("Current Weather: " + weather + ", temp: " + QString::number(temp));

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

void MainWindow::on_imageButton_clicked()
{
    QString zip = ui->zipcodeEdit->text();
    qDebug() << zip;
    httpManager->sendImageRequest(zip);
}

void MainWindow::on_weatherButton_clicked()
{
    QString zip = ui->zipcodeEdit->text();
    qDebug() << zip;
    httpManager->sendWeatherRequest(zip);
}
