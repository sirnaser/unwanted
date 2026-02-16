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

    // Core UI container
    QStackedWidget* stackWidget;

    // SPLASH page (Phase 1)
    QWidget* splashPage;
    QLabel* splashTitle;
    QLabel* splashImageLabel;
    QPushButton* playButton;

    // LOGIN page (Phase 1)
    QWidget* loginPage;
    QLineEdit* name1Edit;
    QLineEdit* name2Edit;
    QListWidget* mapList;
    QPushButton* startGameButton;

    // GAME page (Phase 1 board + Phase 2 logic)
    QWidget* gamePage;
    QGridLayout* boardLayout;
    QListWidget* cardList;
    QLabel* statusLabel;
    QLabel* player1HUD;
    QLabel* player2HUD;
    QPushButton* endTurnButton;

    // Selection state
    Cell* selectedCell = nullptr;
    ActionType selectedAction = ActionType::MOVE;  // default

    // Setup functions
    void setupSplashPage();
    void setupLoginPage();
    void setupGamePage();

    // State switching
    void switchToState(AppState state);

    // Phase 1 helpers
    bool validateName(const string& name) const;
    void loadMapList();

    // Phase 1+2: UI refresh
    void updateBoardUI();
    void updateCardsUI();
    void updateHUD();

    // Slots
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
