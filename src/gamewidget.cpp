// You may need to build the project (run Qt uic code generator) to get "ui_GameWidget.h" resolved

#include "gamewidget.h"
#include "ui_GameWidget.h"
#include <QRandomGenerator>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent), ui(new Ui::GameWidget), enemyNum(0), bulletTimer(this), moveTimer(this), updateTimer(this), createEnemyTimer(this) {
    ui->setupUi(this);

    ui->sceneWidget->setFixedSize(SceneWidth, SceneHeight);

    //加载地形图片
    for (int i = 0; i < terrainNum; ++i) {
        img[i] = QPixmap(":/pic/terrain/" + QString::fromLocal8Bit(Enum::terrainEnum.key(i)).toLower() + ".gif").scaled(BaseSize, BaseSize);
    }

    //定时
    connect(&updateTimer, &QTimer::timeout, this, qOverload<>(&GameWidget::update));
    connect(&createEnemyTimer, &QTimer::timeout, this, &GameWidget::createEnemy);

    //发出信号
    connect(ui->returnButton, &QPushButton::clicked, this, [this] {
        endGate();
        emit returnStart();
    });
    connect(ui->pauseButton, &QPushButton::clicked, this, &GameWidget::pause);
}


GameWidget::~GameWidget() {
    delete ui;
}

void GameWidget::loadMap() {
    //随机选择地图
    int mapIndex = QRandomGenerator::global()->bounded(3);

    //修改地形和地图数组
    for (int i = 0; i < MapWidth; i++) {
        for (int j = 0; j < MapHeight; j++) {
            setTerrainMap(i, j, (Enum::Terrain) MAP[mapIndex][j][i]);
        }
    }
}

void GameWidget::drawMap() {
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            painter.drawPixmap(j * BaseSize, i * BaseSize, img[terrainMap[i][j]]);
        }
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {

    //判断按键是否有效
    if (keyToDirectionPlayer0.contains((Qt::Key) event->key())) {
        //如果按下的是玩家0的方向键,则改变玩家0的方向
        player0->setDirection(keyToDirectionPlayer0.value((Qt::Key) event->key()));
        return;
    }

    if (event->key() == Qt::Key::Key_Space) {
        //如果按下的是空格键,则玩家0射击
        player0->shoot();
        return;
    }

    if (mode == Mode::Double) {

        if (keyToDirectionPlayer1.contains((Qt::Key) event->key())) {
            //如果按下的是玩家1的方向键,则改变玩家1的方向
            player1->setDirection(keyToDirectionPlayer1.value((Qt::Key) event->key()));
            return;
        }

        if (event->key() == Qt::Key::Key_Return) {
            //如果按下的是回车键,则玩家1射击
            player1->shoot();
            return;
        }
    }
}


void GameWidget::keyReleaseEvent(QKeyEvent *event) {

    if (keyToDirectionPlayer0.contains((Qt::Key) event->key()) && player0->getDirection() == keyToDirectionPlayer0.value((Qt::Key) event->key())) {
        //如果松开的是玩家0的方向键,且与当前方向匹配,则停止移动
        player0->stop();
        return;
    }

    if (mode == Mode::Double) {
        if (keyToDirectionPlayer1.contains((Qt::Key) event->key()) && player1->getDirection() == keyToDirectionPlayer1.value((Qt::Key) event->key())) {
            //如果松开的是玩家1的方向键,且与当前方向匹配,则停止移动
            player1->stop();
            return;
        }
    }

}

void GameWidget::enemyDie(Tank *tank) {
    //从敌人列表中删除
    for (int i = 0; i < enemies.size(); ++i) {
        if (enemies[i] == tank) {
            enemies[i]->deleteLater();
            enemies.removeAt(i);
            break;
        }
    }

    if (--enemyNum <= 0) {//敌人全部死亡,进入下一关
        if (++gate > MaxGateNum) {
            //玩家胜利
            endGame(Camp::Player0);
        } else {
            //进入下一关
            endGate();
            startGate();
        }
    }
}

void GameWidget::collisionCheck(QRect &bulletCollider, Camp camp) {
    //检查坦克是否在攻击范围内，是则被击中
    if (player0->attacked(bulletCollider, camp)) {
        return;
    }

    if (mode == Mode::Double) {
        if (player1->attacked(bulletCollider, camp)) {
            return;
        }
    } else {
        for (auto &enemy: enemies) {
            if (enemy->attacked(bulletCollider, camp)) {
                return;
            }
        }
    }
}

void GameWidget::startGate() {
    isPause = false;

    //加载地图
    loadMap();

    //创建坦克
    createPlayer();

    if (mode == Mode::Single) {
        enemyNum = MaxEnemyNum;

        for (int i = 0; i < 3; ++i) {
            createEnemy();
        }
    }

    //开启计时器
    startTimer();
}


void GameWidget::endGate() {
    //删除坦克
    player0->deleteLater();
    if (mode == Mode::Double) {
        player1->deleteLater();

    } else {
        for (auto &enemy: enemies) {
            enemy->deleteLater();
        }
        enemies.clear();
    }

    //停止计时器
    stopTimer();
}

//开启计时器
void GameWidget::startTimer() {
    bulletTimer.start(BulletInterval);
    moveTimer.start(MoveInterval);
    updateTimer.start(UpdateInterval);

    if (mode == Mode::Single) {
        createEnemyTimer.start(CreateEnemyInterval);
    }
}

//定时器停止
void GameWidget::stopTimer() {
    bulletTimer.stop();
    moveTimer.stop();
    updateTimer.stop();
    createEnemyTimer.stop();
}


void GameWidget::createPlayer() {
    player0 = new Player0(&moveTimer, &bulletTimer);
    connect(player0, &Player::attack, this, &GameWidget::collisionCheck);
    connect(player0, &Player::die, this, [this] {
        //玩家0死亡,游戏结束,玩家1或者敌人胜利
        if (mode == Mode::Double) {
            endGame(Camp::Player1);
        } else {
            endGame(Camp::Enemy);
        }
    });

    if (mode == Mode::Double) {
        player1 = new Player1(&moveTimer, &bulletTimer);
        connect(player1, &Player::attack, this, &GameWidget::collisionCheck);
        connect(player1, &Player::die, this, [this] {
            endGame(Camp::Player0);
        });
    }
}

void GameWidget::createEnemy() {
    int column = 6 * QRandomGenerator::global()->bounded(4);

    if (empty(QRect(column * BaseSize, 0, BaseSize, BaseSize))) {

        //随机生成敌人
        Enemy *enemy;
        if (QRandomGenerator::global()->bounded(2)) {
            enemy = new EnemyLife(column * BaseSize, 0, &moveTimer, &bulletTimer, player0);
        } else {
            enemy = new EnemySpeed(column * BaseSize, 0, &moveTimer, &bulletTimer, player0);
        }
        enemies.push_back(enemy);

        connect(enemy, &Enemy::attack, this, &GameWidget::collisionCheck);
        connect(enemy, &Enemy::die, this, &GameWidget::enemyDie);
    }
}

void GameWidget::drawPanel() {
    ui->player0LifeLable->setText(QString::number(player0->getLife()));

    if (mode == Mode::Double) {
        ui->player1LifeLable->setText(QString::number(player1->getLife()));
    } else {
        ui->gateNumLable->setText(QString::number(gate));
        ui->enemyNumLable->setText(QString::number(enemyNum));
    }
}

void GameWidget::paintEvent(QPaintEvent *) {
    painter.begin(this);

    //painter坐标系偏置
    painter.translate(ui->sceneWidget->pos());

    drawMap();

    player0->display(painter);

    if (mode == Mode::Double) {
        player1->display(painter);
    } else {
        for (auto &enemy: enemies) {
            enemy->display(painter);
        }
    }

    drawPanel();

    painter.end();
}

void GameWidget::startGame(Mode mode) {
    this->mode = mode;

    //重置数据
    gate = 1;

    //设置信息面板
    if (mode == Mode::Single) {
        ui->gateLable->show();
        ui->gateNumLable->show();
        ui->enemyLable->show();
        ui->enemyNumLable->show();

        ui->player1Lable->hide();
        ui->player1LifeLable->hide();
    } else {
        ui->gateLable->hide();
        ui->gateNumLable->hide();
        ui->enemyLable->hide();
        ui->enemyNumLable->hide();

        ui->player1Lable->show();
        ui->player1LifeLable->show();
    }

    startGate();
}

void GameWidget::endGame(Camp camp) {
    endGate();

    emit GameOver(camp);
}

void GameWidget::restart() {
    startGame(mode);
}

void GameWidget::pause() {
    isPause = !isPause;

    if (isPause) {
        ui->pauseButton->setText("继续");
        stopTimer();
    } else {
        ui->pauseButton->setText("暂停");
        startTimer();
    }
}
