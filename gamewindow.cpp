#include "GameWindow.h"
#include "ui_GameWindow.h"
#include "core/Player.h"

GameWindow::GameWindow(const QString &p1,
                       const QString &p2,
                       QWidget *parent)
    : QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    Player player1(p1);
    Player player2(p2);

    gameManager = new GameManager(player1, player2);
}

GameWindow::~GameWindow()
{
    delete gameManager;
    delete ui;
}
