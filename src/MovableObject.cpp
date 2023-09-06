#include "MovableObject.h"

MovableObject::MovableObject(int x, int y, int width, int height, int speed, Enum::Direction direction, Camp camp) : collider(x, y, width, height), speed(speed), direction(direction), camp(camp) {

}

void MovableObject::tryMove() {
    //根据方向和速度移动碰撞体
    switch (direction) {
        case Enum::Up:
            collider.moveTop(collider.top() - speed);
            break;

        case Enum::Down:
            collider.moveBottom(collider.bottom() + speed);
            break;

        case Enum::Left:
            collider.moveLeft(collider.left() - speed);
            break;

        case Enum::Right:
            collider.moveRight(collider.right() + speed);
            break;
    }
}

int MovableObject::getWidth() const {
    return collider.width();
}

int MovableObject::getHeight() const {
    return collider.height();
}

const QRect &MovableObject::getCollider() const {
    return collider;
}

Enum::Direction MovableObject::getDirection() const {
    return direction;
}