#include "Bullet.h"

Bullet::Bullet(int speed, QTimer *timer, Camp camp, const QString &imgName) : MovableObject(0, 0, BulletWidth, BulletHeight, speed, Enum::Up, camp) {
    //加载子弹图片
    for (int i = 0; i < directionNum; ++i) {
        img[i] = QPixmap(":/pic/" + imgName + "/" + QString::fromLocal8Bit(Enum::directionEnum.key(i)).toLower() + ".gif").scaled(collider.width(), collider.height());
    }

    //绑定定时器
    connect(timer, &QTimer::timeout, this, &Bullet::move);
}

void Bullet::move() {
    if (active) {
        MovableObject::tryMove();

        //如果子弹超出边界或者与其他碰撞体重叠，则子弹消失
        if (!scene.contains(collider, true)) {
            active = false;
        } else {
            Enum::Terrain terrain = hasAttacked();
            if (terrain != Enum::BLANK) {
                active = false;
                //子弹击中坦克后，触发坦克的攻击信号
                if (terrain == Enum::TANK) {
                    emit attack(collider, camp);
                }
            }
        }
    }
}

bool Bullet::isActive() const {
    return active;
}

Enum::Terrain Bullet::hasAttacked() {
    //遍历碰撞体
    for (int i = collider.top(); i <= collider.bottom(); ++i) {
        for (int j = collider.left(); j < collider.right(); ++j) {
            if (map[i][j] != Enum::BLANK) {
                if (map[i][j] == Enum::BRICK) {
                    //子弹击中砖块后，将砖块变为空地
                    setTerrainMap(j / BaseSize, i / BaseSize, Enum::BLANK);
                    //直接返回Enum::BRICK,如果是map[i][j]，则返回的是Enum::BLANK
                    return Enum::BRICK;
                } else {
                    return map[i][j];
                }
            }
        }
    }

    //如果子弹未击中任何碰撞体，则返回Enum::BLANK
    return Enum::BLANK;
}

void Bullet::shoot(int x, int y, Enum::Direction direction) {
    if (!active) {
        active = true;
        collider.moveTo(x, y);
        this->direction = direction;
    }
}

void Bullet::display(QPainter &paint) {
    if (active) {
        paint.drawPixmap(collider.x(), collider.y(), img[direction]);
    }
}