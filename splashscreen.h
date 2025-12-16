#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

private slots:
    void on_startButton_clicked();

private:
    Ui::SplashScreen *ui;
};

#endif
