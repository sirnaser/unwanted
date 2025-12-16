#include "LoginPage.h"
#include "ui_LoginPage.h"
#include "GameWindow.h"
#include <QMessageBox>
#include <QRegularExpression>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}

bool LoginPage::isValidName(const QString &name)
{
    if (name.length() < 8) return false;

    QRegularExpression regex("^[A-Za-z][A-Za-z0-9]*$");
    return regex.match(name).hasMatch();
}

void LoginPage::on_playButton_clicked()
{
    QString p1 = ui->player1Edit->text();
    QString p2 = ui->player2Edit->text();

    if (!isValidName(p1) || !isValidName(p2)) {
        QMessageBox::warning(this, "خطا",
                             "نام بازیکن باید حداقل ۸ کاراکتر، انگلیسی و با حرف شروع شود");
        return;
    }

    GameWindow *game = new GameWindow(p1, p2);
    game->show();
    this->close();
}
