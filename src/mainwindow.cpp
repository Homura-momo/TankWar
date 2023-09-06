// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //设置窗口
    setWindowTitle("坦克大战");
    setWindowIcon(QIcon(":/pic/icon.png"));

    //绑定按钮
    connect(ui->endExitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->startExitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->endReturnButton, &QPushButton::clicked, this, &MainWindow::returnStart);
    connect(ui->restartButton, &QPushButton::clicked, this, &MainWindow::restart);

    //与游戏窗口的信号与槽绑定
    connect(ui->modeSingleButton, &QPushButton::clicked, this, [this] {
        startGame(Mode::Single);
    });
    connect(ui->modeDoubleButton, &QPushButton::clicked, this, [this] {
        startGame(Mode::Double);
    });

    connect(ui->gameWindow, &GameWidget::returnStart, this, &MainWindow::returnStart);
    connect(ui->gameWindow, &GameWidget::GameOver, this, &MainWindow::end);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startGame(Mode mode) {
    ui->gameWindow->startGame(mode);
    ui->stackedWidget->setCurrentWidget(ui->gameWindow);
}

void MainWindow::restart() {
    ui->gameWindow->restart();
    ui->stackedWidget->setCurrentWidget(ui->gameWindow);
}

void MainWindow::returnStart() {
    ui->stackedWidget->setCurrentWidget(ui->startWindow);
}

void MainWindow::end(Camp camp) {
    ui->stackedWidget->setCurrentWidget(ui->endWindow);

    //显示结果
    if (camp == Camp::Player0) {
        ui->resultLable->setText("Player0 Win!");
    } else if (camp == Camp::Player1) {
        ui->resultLable->setText("Player1 Win!");
    } else {
        ui->resultLable->setText("Player Lose!");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (ui->stackedWidget->currentWidget() == ui->gameWindow) {
        ui->gameWindow->keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (ui->stackedWidget->currentWidget() == ui->gameWindow) {
        ui->gameWindow->keyReleaseEvent(event);
    }
}
