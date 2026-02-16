#include "MainWindow.h"
#include <QPixmap>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Undaunted - Advanced Programming Project");
    resize(1100, 800);

    stackWidget = new QStackedWidget(this);
    setCentralWidget(stackWidget);

    setupSplashPage();
    setupLoginPage();
    setupGamePage();

    switchToState(AppState::SPLASH);
}

MainWindow::~MainWindow() {
    // cleanup if needed (agents delete in Board destructor)
}

void MainWindow::setupSplashPage() {
    splashPage = new QWidget();
    splashLayout = new QVBoxLayout(splashPage);
    splashLayout->setAlignment(Qt::AlignCenter);
    splashLayout->setSpacing(30);

    // Title
    splashTitle = new QLabel("Undaunted", splashPage);
    QFont titleFont("Arial", 48, QFont::Bold);
    splashTitle->setFont(titleFont);
    splashTitle->setStyleSheet("color: #2c3e50;");
    splashLayout->addWidget(splashTitle, 0, Qt::AlignCenter);

    // Image placeholder (you can replace with real QPixmap later)
    splashImage = new QLabel(splashPage);
    splashImage->setFixedSize(500, 300);
    splashImage->setStyleSheet("background-color: #ecf0f1; border: 2px solid #bdc3c7; border-radius: 10px;");
    splashImage->setAlignment(Qt::AlignCenter);
    splashImage->setText("Undaunted Normandy Cover Art");
    splashLayout->addWidget(splashImage, 0, Qt::AlignCenter);

    // Play button
    playButton = new QPushButton("PLAY", splashPage);
    playButton->setFixedSize(220, 70);
    playButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #27ae60;"
        "   color: white;"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "   border-radius: 12px;"
        "}"
        "QPushButton:hover { background-color: #2ecc71; }"
        );
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    splashLayout->addWidget(playButton, 0, Qt::AlignCenter);

    splashLayout->addStretch();
    stackWidget->addWidget(splashPage);
}

void MainWindow::setupLoginPage() {
    loginPage = new QWidget();
    loginLayout = new QVBoxLayout(loginPage);
    loginLayout->setContentsMargins(60, 40, 60, 40);
    loginLayout->setSpacing(25);

    QLabel* title = new QLabel("Enter Player Names", loginPage);
    title->setFont(QFont("Arial", 28, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    loginLayout->addWidget(title);

    // Player 1
    QLabel* label1 = new QLabel("Player 1 Name:", loginPage);
    label1->setFont(QFont("Arial", 14));
    loginLayout->addWidget(label1);

    name1Edit = new QLineEdit(loginPage);
    name1Edit->setPlaceholderText("At least 8 chars, letter start, number + special char");
    name1Edit->setFixedHeight(45);
    name1Edit->setStyleSheet("font-size: 16px; padding: 8px; border-radius: 6px; border: 1px solid #bdc3c7;");
    loginLayout->addWidget(name1Edit);

    // Player 2
    QLabel* label2 = new QLabel("Player 2 Name:", loginPage);
    label2->setFont(QFont("Arial", 14));
    loginLayout->addWidget(label2);

    name2Edit = new QLineEdit(loginPage);
    name2Edit->setPlaceholderText("At least 8 chars, letter start, number + special char");
    name2Edit->setFixedHeight(45);
    name2Edit->setStyleSheet("font-size: 16px; padding: 8px; border-radius: 6px; border: 1px solid #bdc3c7;");
    loginLayout->addWidget(name2Edit);

    // Map selection
    QLabel* mapLabel = new QLabel("Select Map:", loginPage);
    mapLabel->setFont(QFont("Arial", 14));
    loginLayout->addWidget(mapLabel);

    mapList = new QListWidget(loginPage);
    mapList->setFixedHeight(180);
    mapList->setStyleSheet("font-size: 15px; border: 1px solid #bdc3c7; border-radius: 6px;");
    loginLayout->addWidget(mapList);

    loadMapList();

    // Start button
    startGameButton = new QPushButton("Start Game", loginPage);
    startGameButton->setFixedHeight(60);
    startGameButton->setStyleSheet(
        "QPushButton { background-color: #2980b9; color: white; font-size: 20px; font-weight: bold; border-radius: 10px; }"
        "QPushButton:hover { background-color: #3498db; }"
        );
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    loginLayout->addWidget(startGameButton);

    loginLayout->addStretch();
    stackWidget->addWidget(loginPage);
}

void MainWindow::loadMapList() {
    QDir dir("resources/boards");
    if (!dir.exists()) {
        mapList->addItem("No maps found - using default");
        return;
    }

    QStringList filters;
    filters << "*.txt";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    for (const QFileInfo& file : files) {
        mapList->addItem(file.fileName());
    }

    if (mapList->count() == 0) {
        mapList->addItem("No .txt files found");
    } else {
        mapList->setCurrentRow(0);
    }
}

void MainWindow::setupGamePage() {
    gamePage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(gamePage);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // HUD top
    QHBoxLayout* hudLayout = new QHBoxLayout();
    player1HUD = new QLabel("Player 1: -");
    player1HUD->setStyleSheet("font-size: 16px; color: #2980b9; font-weight: bold;");
    player2HUD = new QLabel("Player 2: -");
    player2HUD->setStyleSheet("font-size: 16px; color: #c0392b; font-weight: bold;");
    statusLabel = new QLabel("Game not started");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #34495e;");

    hudLayout->addWidget(player1HUD);
    hudLayout->addStretch();
    hudLayout->addWidget(statusLabel);
    hudLayout->addStretch();
    hudLayout->addWidget(player2HUD);
    mainLayout->addLayout(hudLayout);

    // Main splitter: board | cards + controls
    gameSplitter = new QSplitter(Qt::Horizontal, gamePage);

    // Left: Board
    QWidget* boardContainer = new QWidget();
    boardLayout = new QGridLayout(boardContainer);
    boardLayout->setSpacing(2);
    gameSplitter->addWidget(boardContainer);

    // Right: Cards + End Turn
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(10, 0, 10, 10);

    cardList = new QListWidget(rightPanel);
    cardList->setStyleSheet("font-size: 15px; border: 1px solid #95a5a6; border-radius: 6px;");
    connect(cardList, &QListWidget::currentRowChanged, this, &MainWindow::onCardClicked);
    rightLayout->addWidget(new QLabel("Your Hand:", rightPanel));
    rightLayout->addWidget(cardList);

    endTurnButton = new QPushButton("End Turn");
    endTurnButton->setFixedHeight(50);
    endTurnButton->setStyleSheet("background-color: #e74c3c; color: white; font-size: 18px; font-weight: bold; border-radius: 8px;");
    connect(endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    rightLayout->addWidget(endTurnButton);
    rightLayout->addStretch();

    gameSplitter->addWidget(rightPanel);
    gameSplitter->setStretchFactor(0, 4);  // board bigger
    gameSplitter->setStretchFactor(1, 1);

    mainLayout->addWidget(gameSplitter);
    stackWidget->addWidget(gamePage);
}

void MainWindow::switchToState(AppState state) {
    currentState = state;
    stackWidget->setCurrentIndex(static_cast<int>(state));

    if (state == AppState::GAME) {
        // Here you can call engine.startGame() after names & map selected
        updateHUD();
        updateBoardUI();
    }
}

void MainWindow::onPlayClicked() {
    switchToState(AppState::LOGIN);
}

bool MainWindow::validateName(const string& name) const {
    if (name.length() < 8) return false;
    if (!isalpha(name[0])) return false;

    bool hasDigit = false;
    bool hasSpecial = false;
    bool hasUpper = false;
    bool hasLower = false;

    for (char c : name) {
        if (isdigit(c)) hasDigit = true;
        if (!isalnum(c)) hasSpecial = true;
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
    }

    return hasDigit && hasSpecial && hasUpper && hasLower;
}

void MainWindow::onStartGameClicked() {
    string n1 = name1Edit->text().toStdString();
    string n2 = name2Edit->text().toStdString();

    if (!validateName(n1) || !validateName(n2)) {
        QMessageBox::warning(this, "Invalid Name",
                             "Each name must:\n"
                             "• At least 8 characters\n"
                             "• Start with a letter\n"
                             "• Contain at least one digit\n"
                             "• Contain at least one special character\n"
                             "• Contain both uppercase and lowercase letters");
        return;
    }

    if (mapList->currentItem() == nullptr) {
        QMessageBox::warning(this, "No Map", "Please select a map.");
        return;
    }

    QString mapFile = "resources/boards/" + mapList->currentItem()->text();

    // Initialize engine with selected map
    engine.init(mapFile.toStdString());

    // Set player names (you may want to add to Player class)
    // For now just proceed
    engine.startGame();   // This should set up players, deck, agents, etc.

    switchToState(AppState::GAME);
}

void MainWindow::updateBoardUI() {
    // Clear previous
    while (QLayoutItem* item = boardLayout->takeAt(0)) {
        if (QWidget* w = item->widget()) delete w;
        delete item;
    }

    const Board& b = engine.getBoard();

    for (int r = 0; r < b.rows; ++r) {
        for (int c = 0; c < b.cols; ++c) {
            Cell* cell = b.getCell(r, c);
            if (!cell) continue;

            QPushButton* btn = new QPushButton(cell->getDisplayText().c_str());
            btn->setFixedSize(60, 60);

            // Basic styling based on terrain & state
            QString style = "border: 1px solid #7f8c8d; font-size: 12px; font-weight: bold;";

            if (cell->terrain == 0) style += "background-color: #a9dfbf;";
            else if (cell->terrain == 1) style += "background-color: #f9e79f;";
            else style += "background-color: #f5b7b1;";

            if (cell->controlledBy == PlayerId::PLAYER1)
                style += "border: 3px solid #2980b9;";
            else if (cell->controlledBy == PlayerId::PLAYER2)
                style += "border: 3px solid #c0392c;";

            if (cell->isVisibleFor(engine.getCurrentTurn() == 0 ? PlayerId::PLAYER1 : PlayerId::PLAYER2))
                style += "font-weight: bold;";

            btn->setStyleSheet(style);

            // Connect click
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
    Player& p1 = engine.players[0];
    Player& p2 = engine.players[1];

    player1HUD->setText(QString("Player 1: %1  (%2 agents)").arg(QString::fromStdString(p1.name)).arg(p1.agents.size()));
    player2HUD->setText(QString("Player 2: %1  (%2 agents)").arg(QString::fromStdString(p2.name)).arg(p2.agents.size()));

    statusLabel->setText(QString("Turn: %1").arg(engine.getCurrentTurn() == 0 ? "Player 1" : "Player 2"));
}

void MainWindow::onCellClicked(int row, int col) {
    Cell* cell = engine.getBoard().getCell(row, col);
    if (!cell) return;

    selectedCell = cell;
    statusLabel->setText(QString("Selected: %1").arg(QString::fromStdString(cell->id)));

    // Here you can highlight possible moves/attacks (future)
    // For now just log
    qDebug() << "Cell clicked:" << QString::fromStdString(cell->id);
}

void MainWindow::onCardClicked(int index) {
    if (index < 0) return;

    Player& p = engine.getCurrentPlayer();
    const Card& selected = p.deck.getHand()[index];

    statusLabel->setText(QString("Selected action: %1").arg(QString::fromStdString(selected.toString())));

    // In real game → wait for cell click to perform action
    // For minimal version → you can auto-apply or show dialog
}

void MainWindow::onEndTurnClicked() {
    engine.nextTurn();
    updateHUD();
    updateCardsUI();
    updateBoardUI();
}
