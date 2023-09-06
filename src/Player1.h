#pragma once

#include "Player.h"

class Player1 : public Player {

public:
    Player1(QTimer *moveTimer, QTimer *bulletTimer);
};
