#pragma once

#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QString>
#include "Config.h"

class MovableObject : public QObject {
protected:
    Enum::Direction direction;//方向
    QRect collider;//碰撞体
    int speed;//移动速度
    Camp camp;//阵营

    MovableObject(int x, int y, int width, int height, int speed, Enum::Direction direction, Camp camp);

    //尝试根据方向移动
    void tryMove();

    //绘制
    virtual void display(QPainter &paint) = 0;

public:
    //获取碰撞体
    [[nodiscard]] const QRect &getCollider() const;

    //获取方向
    [[nodiscard]] Enum::Direction getDirection() const;

    //获取宽度
    [[nodiscard]] int getWidth() const;

    //获取高度
    [[nodiscard]] int getHeight() const;
};