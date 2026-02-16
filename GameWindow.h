#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "GameEngine.h"

using namespace std;

class GameWindow : public QMainWindow {
    Q_OBJECT

private:
    GameEngine engine;

    QWidget* centralWidget;
    QGridLayout* boardLayout;
    QListWidget* cardList;
    QLabel* statusLabel;
    QLabel* player1Label, *player2Label;

    Cell* selectedCell = nullptr;

    void setupUI();
    void setupBoardUI();
    void updateUI();

    void onCellClicked(int row, int col);
    void onCardClicked(int index);

public:
    GameWindow(QWidget* parent = nullptr);
    ~GameWindow();
};

#endif // GAMEWINDOW_H
