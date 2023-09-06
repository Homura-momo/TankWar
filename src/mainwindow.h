#pragma once

#include <QWidget>
#include "Config.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void startGame(Mode mode);

private:
    Ui::MainWindow *ui;

public slots:

    void restart();

    void returnStart();

    //结束游戏
    void end(Camp camp);

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

};
