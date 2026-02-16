#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QMessageBox>
#include <QDir>
#include "GameEngine.h"
#include "GameState.h"

using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    GameEngine engine;
    AppState currentState = AppState::SPLASH;

    // Core UI
    QStackedWidget* stackWidget;

    // SPLASH page (Phase 1)
    QWidget* splashPage;
    QVBoxLayout* splashLayout;
    QLabel* splashTitle;
    QLabel* splashImage;  // QPixmap placeholder
    QPushButton* playButton;

    // LOGIN page (Phase 1)
    QWidget* loginPage;
    QVBoxLayout* loginLayout;
    QLineEdit* name1Edit;
    QLineEdit* name2Edit;
    QListWidget* mapList;
    QPushButton* startGameButton;

    // GAME page (Phase 1 Board + Phase 2 Logic)
    QWidget* gamePage;
    QSplitter* gameSplitter;
    QGridLayout* boardLayout;     // Dynamic square grid
    QListWidget* cardList;        // Hand
    QLabel* statusLabel;          // Turn info
    QLabel* player1HUD;
    QLabel* player2HUD;
    QPushButton* endTurnButton;

    // Selection state for game actions
    Cell* selectedCell = nullptr;

    // Setup methods (clean code-only UI)
    void setupSplashPage();
    void setupLoginPage();
    void setupGamePage();
    void switchToState(AppState state);

    // Phase 1: Login validation
    bool validateName(const string& name) const;
    void loadMapList();

    // Phase 1/2: UI updates
    void updateBoardUI();
    void updateCardsUI();
    void updateHUD();

    // Slots for interactions
    void onPlayClicked();
    void onStartGameClicked();
    void onCellClicked(int row, int col);
    void onCardClicked(int index);
    void onEndTurnClicked();

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
