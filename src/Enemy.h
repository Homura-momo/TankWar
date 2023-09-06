#pragma once

#include "Player.h"

class Enemy : public Tank {
protected:
    Player *player;//玩家的指针
    Enemy(int x, int y, int life, int speed, QTimer *moveTimer, Player *player, const QString &imgName, Bullet *bullet);

public:

    bool move() override;
};