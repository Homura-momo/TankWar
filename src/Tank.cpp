#include "Tank.h"

Tank::Tank(int x, int y, int life, int speed, Enum::Direction direction, QTimer *moveTimer, Camp camp, const QString &imgName, Bullet *bullet) : MovableObject(x, y, BaseSize, BaseSize, speed, direction, camp), life(life), bullet(bullet) {
    //加载坦克图片
    for (int i = 0; i < Enum::directionEnum.keyCount(); ++i) {
        for (int j = 0; j < 2; ++j) {
            img[i][j] = QPixmap(":/pic/" + imgName + "/" + QString::fromLocal8Bit(Enum::directionEnum.key(i)).toLower() + "_" + QString::number(j + 1) + ".gif").scaled(getWidth(), getHeight());
        }
    }

    //修改地图
    setMap(collider, Enum::TANK);

    //绑定移动定时器
    connect(moveTimer, &QTimer::timeout, this, &Tank::move);

    //子弹发送攻击信号时，坦克也发送攻击信号
    connect(bullet, &Bullet::attack, this, &Tank::attack);
}


Tank::~Tank() {
    setMap(collider, Enum::BLANK);
    bullet->deleteLater();
}

bool Tank::move() {
    //复制一份碰撞体
    QRect OldCollider = collider;
    setMap(OldCollider, Enum::BLANK);

    MovableObject::tryMove();

    //如果碰撞体未超出边界且未与其他碰撞体重叠，则移动
    if (empty(collider)) {
        setMap(collider, Enum::TANK);
        return true;
    } else {//否则，将碰撞体恢复到移动前的位置
        collider = OldCollider;
        setMap(collider, Enum::TANK);
        return false;
    }
}

void Tank::shoot() {
    if (!bullet->isActive()) {//子弹未射出

        int x, y;

        switch (direction) {//根据坦克方向确定子弹的初始位置
            case Enum::Up:
                x = collider.x() + (this->getWidth() - bullet->getWidth()) / 2;
                y = collider.y() - bullet->getHeight();
                break;

            case Enum::Down:
                x = collider.x() + (this->getWidth() - bullet->getWidth()) / 2;
                y = collider.y() + this->getWidth();
                break;

            case Enum::Left:
                x = collider.x() - bullet->getWidth();
                y = collider.y() + (this->getHeight() - bullet->getHeight()) / 2;
                break;

            case Enum::Right:
                x = collider.x() + this->getWidth();
                y = collider.y() + (this->getHeight() - bullet->getHeight()) / 2;
                break;
        }

        bullet->shoot(x, y, direction);
    }
}

void Tank::display(QPainter &paint) {
    //两个状态的图片轮流切换，实现履带转动效果
    paint.drawPixmap(collider, img[direction][state]);
    state = !state;

    //显示子弹
    bullet->display(paint);
}

bool Tank::attacked(QRect &bulletCollider, Camp camp) {
    if (collider.intersects(bulletCollider)) {
        //如果是不同阵营的子弹
        if (this->camp != camp) {
            //生命值减1
            if (--life <= 0) {
                emit die(this);
            }

        }
        return true;
    } else {
        return false;
    }
}

int Tank::getLife() const {
    return life;
}