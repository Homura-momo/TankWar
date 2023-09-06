#pragma once

#include "Enemy.h"

class EnemyLife : public Enemy {

public:
    EnemyLife(int x, int y, QTimer *moveTimer, QTimer *bulletTimer, Player *player);
};
