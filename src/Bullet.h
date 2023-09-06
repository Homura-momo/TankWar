#pragma once

#include "MovableObject.h"

class Bullet : public MovableObject {
Q_OBJECT
protected:
    bool active = false;//子弹是否激活

    QPixmap img[directionNum];

    //检查子弹是否击中某些物体
    Enum::Terrain hasAttacked();

public:

    Bullet(int speed, QTimer *timer, Camp camp, const QString &imgName);

    //发射子弹
    void shoot(int x, int y, Enum::Direction direction);

    //子弹是否激活
    [[nodiscard]] bool isActive() const;

    void display(QPainter &painter) override;

signals:

    void attack(QRect &collider, Camp camp);

public slots:

    void move();
};