#include "MainWindow.h"
#include <QPixmap>
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Undaunted - Phase 1 + 2");
    resize(1200, 850);
    setMinimumSize(1000, 700);

    stackWidget = new QStackedWidget(this);
    setCentralWidget(stackWidget);

    setupSplashPage();
    setupLoginPage();
    setupGamePage();

    switchToState(AppState::SPLASH);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupSplashPage() {
    splashPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(splashPage);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(40);

    splashTitle = new QLabel("Undaunted", splashPage);
    splashTitle->setFont(QFont("Arial", 52, QFont::Bold));
    splashTitle->setStyleSheet("color: #2c3e50;");
    layout->addWidget(splashTitle);

    splashImageLabel = new QLabel(splashPage);
    splashImageLabel->setFixedSize(520, 320);
    splashImageLabel->setStyleSheet("background: #34495e; border-radius: 12px; color: white;");
    splashImageLabel->setAlignment(Qt::AlignCenter);
    splashImageLabel->setText("Game Cover Art");
    layout->addWidget(splashImageLabel);

    playButton = new QPushButton("PLAY GAME", splashPage);
    playButton->setFixedSize(280, 80);
    playButton->setStyleSheet(
        "QPushButton { background: #27ae60; color: white; font-size: 28px; font-weight: bold; border-radius: 15px; }"
        "QPushButton:hover { background: #2ecc71; }"
        );
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    layout->addWidget(playButton);

    stackWidget->addWidget(splashPage);
}

void MainWindow::setupLoginPage() {
    loginPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(loginPage);
    layout->setContentsMargins(80, 60, 80, 60);
    layout->setSpacing(30);

    QLabel* title = new QLabel("Enter Player Names", loginPage);
    title->setFont(QFont("Arial", 26, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Player 1
    name1Edit = new QLineEdit(loginPage);
    name1Edit->setPlaceholderText("Player 1 (min 8 chars, letter start, digit + special + upper/lower)");
    name1Edit->setFixedHeight(50);
    layout->addWidget(new QLabel("Player 1 Name:"));
    layout->addWidget(name1Edit);

    // Player 2
    name2Edit = new QLineEdit(loginPage);
    name2Edit->setPlaceholderText("Player 2 (min 8 chars, letter start, digit + special + upper/lower)");
    name2Edit->setFixedHeight(50);
    layout->addWidget(new QLabel("Player 2 Name:"));
    layout->addWidget(name2Edit);

    // Map selection
    layout->addWidget(new QLabel("Select Map:"));
    mapList = new QListWidget(loginPage);
    mapList->setFixedHeight(160);
    loadMapList();
    layout->addWidget(mapList);

    startGameButton = new QPushButton("START GAME", loginPage);
    startGameButton->setFixedHeight(65);
    startGameButton->setStyleSheet(
        "QPushButton { background: #2980b9; color: white; font-size: 22px; font-weight: bold; border-radius: 12px; }"
        "QPushButton:hover { background: #3498db; }"
        );
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    layout->addWidget(startGameButton);

    stackWidget->addWidget(loginPage);
}

void MainWindow::loadMapList() {
    QDir dir("resources/boards");
    if (!dir.exists()) {
        mapList->addItem("Default Board");
        return;
    }

    QStringList filters = {"*.txt"};
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    for (auto& f : files) {
        mapList->addItem(f.fileName());
    }
    if (mapList->count() > 0) mapList->setCurrentRow(0);
}

void MainWindow::setupGamePage() {
    gamePage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(gamePage);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // HUD
    QHBoxLayout* hud = new QHBoxLayout();
    player1HUD = new QLabel("Player 1");
    player2HUD = new QLabel("Player 2");
    statusLabel = new QLabel("Waiting for game start...");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50;");

    hud->addWidget(player1HUD);
    hud->addStretch();
    hud->addWidget(statusLabel);
    hud->addStretch();
    hud->addWidget(player2HUD);
    mainLayout->addLayout(hud);

    // Board + Sidebar
    QSplitter* splitter = new QSplitter(Qt::Horizontal);

    // Board
    QWidget* boardContainer = new QWidget();
    boardLayout = new QGridLayout(boardContainer);
    boardLayout->setSpacing(3);
    splitter->addWidget(boardContainer);

    // Right panel: Cards + End Turn
    QWidget* right = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(right);
    rightLayout->setContentsMargins(15, 0, 15, 10);

    cardList = new QListWidget();
    cardList->setStyleSheet("font-size: 16px;");
    connect(cardList, &QListWidget::currentRowChanged, this, &MainWindow::onCardClicked);
    rightLayout->addWidget(new QLabel("Your Hand:"));
    rightLayout->addWidget(cardList);

    endTurnButton = new QPushButton("END TURN");
    endTurnButton->setFixedHeight(55);
    endTurnButton->setStyleSheet("background: #e74c3c; color: white; font-size: 18px; font-weight: bold;");
    connect(endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    rightLayout->addWidget(endTurnButton);

    splitter->addWidget(right);
    splitter->setStretchFactor(0, 3);
    mainLayout->addWidget(splitter);

    stackWidget->addWidget(gamePage);
}

void MainWindow::switchToState(AppState state) {
    currentState = state;
    stackWidget->setCurrentIndex(static_cast<int>(state));

    if (state == AppState::GAME) {
        engine.startGame();
        updateHUD();
        updateBoardUI();
        updateCardsUI();
    }
}

bool MainWindow::validateName(const string& name) const {
    if (name.length() < 8) return false;
    if (!isalpha(name[0])) return false;

    bool hasDigit = false, hasSpecial = false, hasUpper = false, hasLower = false;
    for (char c : name) {
        if (isdigit(c)) hasDigit = true;
        if (!isalnum(c)) hasSpecial = true;
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
    }
    return hasDigit && hasSpecial && hasUpper && hasLower;
}

void MainWindow::onPlayClicked() {
    switchToState(AppState::LOGIN);
}

void MainWindow::onStartGameClicked() {
    string n1 = name1Edit->text().toStdString();
    string n2 = name2Edit->text().toStdString();

    if (!validateName(n1) || !validateName(n2)) {
        QMessageBox::warning(this, "Invalid Names", "Names must be at least 8 chars, start with letter, contain digit, special char, upper and lower case.");
        return;
    }

    if (mapList->currentItem() == nullptr) {
        QMessageBox::warning(this, "No Map", "Select a map.");
        return;
    }

    string mapFile = "resources/boards/" + mapList->currentItem()->text().toStdString();
    engine.init(mapFile);

    engine.setPlayerName(0, n1);
    engine.setPlayerName(1, n2);

    switchToState(AppState::GAME);
}

void MainWindow::updateBoardUI() {
    // Clear old buttons
    while (QLayoutItem* item = boardLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    const Board& b = engine.getBoard();

    for (int r = 0; r < b.rows; ++r) {
        for (int c = 0; c < b.cols; ++c) {
            Cell* cell = b.getCell(r, c);
            if (!cell) continue;

            QPushButton* btn = new QPushButton(QString::fromStdString(cell->getDisplayText()));
            btn->setFixedSize(68, 68);
            btn->setFont(QFont("Arial", 14, QFont::Bold));

            QString style = "border: 2px solid #7f8c8d; border-radius: 6px;";

            // Terrain color
            if (cell->terrain == 0) style += "background: #a9dfbf;";
            else if (cell->terrain == 1) style += "background: #f9e79f;";
            else style += "background: #f5b7b1;";

            // Controlled
            if (cell->controlledBy == PlayerId::PLAYER1) style += "border: 4px solid #2980b9;";
            if (cell->controlledBy == PlayerId::PLAYER2) style += "border: 4px solid #c0392b;";

            // Visible for current player
            if (cell->isVisibleFor(engine.getCurrentTurnIndex() == 0 ? PlayerId::PLAYER1 : PlayerId::PLAYER2))
                style += "font-weight: bold; color: black;";

            btn->setStyleSheet(style);

            connect(btn, &QPushButton::clicked, this, [this, r, c]() {
                onCellClicked(r, c);
            });

            boardLayout->addWidget(btn, r, c);
        }
    }
}

void MainWindow::updateCardsUI() {
    cardList->clear();
    Player& p = engine.getCurrentPlayer();
    for (const Card& card : p.deck.getHand()) {
        cardList->addItem(QString::fromStdString(card.toString()));
    }
}

void MainWindow::updateHUD() {
    player1HUD->setText(QString("Player 1: %1").arg(QString::fromStdString(engine.getCurrentPlayer().getStatus())));
    player2HUD->setText(QString("Player 2: %1").arg(QString::fromStdString(engine.getOpponent().getStatus())));
    statusLabel->setText(QString::fromStdString(engine.getTurnStatus()));
}

void MainWindow::onCellClicked(int row, int col) {
    Cell* cell = engine.getBoard().getCell(row, col);
    if (!cell) return;

    selectedCell = cell;

    if (selectedAction != ActionType::MOVE && selectedAction != ActionType::SCOUT) {
        statusLabel->setText("Select a card first");
        return;
    }

    bool success = engine.performAction(selectedAction, selectedCell, selectedCell);  // for scout it's same cell
    if (success) {
        updateBoardUI();
        updateCardsUI();
        if (engine.checkWin()) {
            QMessageBox::information(this, "Game Over", "Game has ended!");
        }
    }
}

void MainWindow::onCardClicked(int index) {
    if (index < 0) return;
    Player& p = engine.getCurrentPlayer();
    const Card& card = p.deck.getHand()[index];
    selectedAction = card.type;

    statusLabel->setText("Selected: " + QString::fromStdString(card.toString()) + " → Click a cell");
}

void MainWindow::onEndTurnClicked() {
    engine.nextTurn();
    updateHUD();
    updateCardsUI();
    updateBoardUI();
}
