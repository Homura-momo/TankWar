#include "Enemy.h"
#include "QRandomGenerator"

Enemy::Enemy(int x, int y, int life, int speed, QTimer *moveTimer, Player *player, const QString &imgName, Bullet *bullet) : Tank(x, y, life, speed, Enum::Down, moveTimer, Camp::Enemy, imgName, bullet), player(player) {
}

bool Enemy::move() {
    if (QRandomGenerator::global()->bounded(4) < 1) {
        //25%的概率追踪玩家

        //directionX为敌人行走的垂直方向，directionY为敌人行走的水平方向，directionSpare来暂存备选方向
        Enum::Direction directionX, directionY, directionSpare;

        //获取玩家左上角和敌人的水平（垂直）对称轴的水平（垂直）距离
        const QRect &playerCollider = player->getCollider();
        int distanceX = collider.left() + collider.width() / 2 - playerCollider.left();
        int distanceY = collider.top() + collider.height() / 2 - playerCollider.top();

        if (distanceX > 0 && distanceX < collider.width()) {
            //如果敌人垂直的对称轴与玩家相交，则敌人向上(下)发动攻击能击中玩家，说明玩家进入敌人的垂直攻击范围
            //如果玩家纵坐标小于敌人的对称轴（在敌人的上方），则敌人方向调整为向上，否则向下
            direction = distanceY > 0 ? Enum::Up : Enum::Down;
            shoot();
            return Tank::move();
        } else if (distanceY > 0 && distanceY < collider.height()) {
            //玩家进入敌人的水平攻击范围
            //如果玩家横坐标小于敌人的对称轴（在敌人的左方），则敌人方向调整为向左，否则向右
            direction = distanceX > 0 ? Enum::Left : Enum::Right;
            shoot();
            return Tank::move();
        } else {
            //如果玩家没有进入敌人攻击范围，则确定玩家在敌人的哪个方向，然后敌人沿折线前进
            directionX = distanceX > 0 ? Enum::Left : Enum::Right;
            directionY = distanceY > 0 ? Enum::Up : Enum::Down;
            if (abs(distanceY) > abs(distanceX)) {
                //垂直距离更大，则垂直方向为首选方向，水平方向为备选方向
                direction = directionX;
                directionSpare = directionY;
            } else {
                //水平距离更大，则水平方向为首选方向，垂直方向为备选方向
                direction = directionY;
                directionSpare = directionX;
            }
            if (Tank::move()) {
                //如果朝首选方向移动成功，则不再朝备选方向移动
                return true;
            } else {
                //如果朝首选方向移动失败，则朝备选方向移动
                direction = directionSpare;
                return Tank::move();
            }
        }
    } else {
        //正常移动
        return Tank::move();
    }
}