#pragma once

#include "Enemy.h"

class EnemySpeed : public Enemy {

public:
    EnemySpeed(int x, int y, QTimer *moveTimer, QTimer *bulletTimer, Player *player);
};
