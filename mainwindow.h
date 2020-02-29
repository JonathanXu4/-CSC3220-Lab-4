#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

#include "httpmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setCurrentTime();
    void setGlobalTime();
    void changeImage();
    void processImage(QPixmap *);
    void processWeatherJson(QJsonObject *json);
    void processIcon(QPixmap *);

    void on_updateButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *imageTimer;
    HTTPManager *httpManager;

};

#endif // MAINWINDOW_H
