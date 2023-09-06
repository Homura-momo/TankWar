#include "Player.h"


Player::Player(int x, int y, Enum::Direction direction, QTimer *moveTimer, Camp camp, const QString &imgName, Bullet *bullet) : Tank(x, y, PlayerLife, PlayerSpeed, direction, moveTimer, camp, imgName, bullet) {

}

void Player::setDirection(Enum::Direction direction) {
    this->direction = direction;
    moving = true;
}

void Player::stop() {
    moving = false;
}

bool Player::move() {
    if (moving) {
        return Tank::move();
    }
    return false;
}

