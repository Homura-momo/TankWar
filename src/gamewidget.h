#pragma once

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QIcon>
#include <QTimer>
#include <QList>
#include <QMap>
#include <QCoreApplication>
#include <QMessageBox>
#include "Player0.h"
#include "Player1.h"
#include "EnemySpeed.h"
#include "EnemyLife.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWidget; }
QT_END_NAMESPACE

class GameWidget : public QWidget {
Q_OBJECT
protected:

    Mode mode = Mode::Double;

    Ui::GameWidget *ui;

    Player *player0 = nullptr;//玩家0
    Player *player1 = nullptr;//玩家1
    QList<Enemy *> enemies;//敌人列表

    QPixmap img[terrainNum];//地形图片
    QPainter painter;//画笔

    //定时器
    QTimer bulletTimer;
    QTimer moveTimer;
    QTimer updateTimer;
    QTimer createEnemyTimer;

    int enemyNum;//敌人数量
    int gate;//关卡
    bool isPause = false;//是否暂停

    //开始关卡
    void startGate();

    //加载地图
    static void loadMap();

    //结束关卡
    void endGate();

    //创建玩家
    void createPlayer();

    //创建敌人
    void createEnemy();

    //游戏结束，camp为胜利阵营
    void endGame(Camp camp);

    //绘画事件
    void paintEvent(QPaintEvent *) override;

    //绘制地图
    void drawMap();

    //绘制信息面板
    void drawPanel();




    void startTimer();

    void stopTimer();

public:
    explicit GameWidget(QWidget *parent = nullptr);

    ~GameWidget() override;

    //开始游戏
    void startGame(Mode mode);

public slots:

    //子弹和坦克碰撞检测
    void collisionCheck(QRect &bulletCollider, Camp camp);

    //敌人死亡
    void enemyDie(class Tank *tank);

    //重新开始
    void restart();

    //暂停
    void pause();

    //键盘按下事件
    void keyPressEvent(QKeyEvent *) override;

    //键盘释放事件
    void keyReleaseEvent(QKeyEvent *event) override;

signals:

    //返回开始界面
    void returnStart();

    //游戏结束信号
    void GameOver(Camp camp);
};