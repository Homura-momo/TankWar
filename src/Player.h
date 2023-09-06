#pragma once

#include "Tank.h"

class Player : public Tank {
Q_OBJECT

protected:
    bool moving = false;//是否在移动

    bool move() override;

public:
    Player(int x, int y, Enum::Direction direction, QTimer *moveTimer, Camp camp, const QString &imgName, Bullet *bullet);

    //设置方向
    void setDirection(Enum::Direction direction);

    //停止移动
    void stop();
};