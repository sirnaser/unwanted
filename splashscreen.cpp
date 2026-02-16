#include "splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

void SplashScreen::on_startButton_clicked()
{
    LoginPage *login = new LoginPage();
    login->show();
    this->close();
}
