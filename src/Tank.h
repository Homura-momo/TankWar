#pragma once

#include "BulletATK.h"
#include "BulletSpeed.h"

class Tank : public MovableObject {
Q_OBJECT
protected:
    Bullet *bullet;//子弹
    int life;//生命值
    bool state = false;//履带状态
    QPixmap img[directionNum][2];//坦克图片

    virtual bool move();

    Tank(int x, int y, int life, int speed, Enum::Direction direction, QTimer *moveTimer, Camp camp, const QString &imgName, Bullet *bullet);

    ~Tank() override;

public:

    //如果坦克被子弹击中，返回true
    bool attacked(QRect &bulletCollider, Camp camp);

    void display(QPainter &) override;

    [[nodiscard]] int getLife() const;

signals:

    //坦克攻击
    void attack(QRect &collider, Camp camp);

    //坦克死亡
    void die(Tank *tank);

public slots:

    //发射子弹
    void shoot();
};