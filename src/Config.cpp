#include "Config.h"

QMetaEnum Enum::directionEnum = QMetaEnum::fromType<Enum::Direction>();
QMetaEnum Enum::terrainEnum = QMetaEnum::fromType<Enum::Terrain>();

Enum::Terrain map[SceneHeight][SceneWidth];
Enum::Terrain terrainMap[MapHeight][MapWidth];

QMap<Qt::Key, Enum::Direction> keyToDirectionPlayer0 = {
        {Qt::Key::Key_W, Enum::Up},
        {Qt::Key::Key_S, Enum::Down},
        {Qt::Key::Key_A, Enum::Left},
        {Qt::Key::Key_D, Enum::Right},
};

QMap<Qt::Key, Enum::Direction> keyToDirectionPlayer1 = {
        {Qt::Key::Key_Up,    Enum::Up},
        {Qt::Key::Key_Down,  Enum::Down},
        {Qt::Key::Key_Left,  Enum::Left},
        {Qt::Key::Key_Right, Enum::Right},
};


QRect scene(0, 0, SceneWidth, SceneHeight);

bool empty(const QRect &rect) {
    if (!scene.contains(rect)) {
        return false;
    }

    for (int i = rect.top(); i <= rect.bottom(); i++) {
        for (int j = rect.left(); j <= rect.right(); j++) {//只要有物体，就不为空
            if (map[i][j] != Enum::BLANK) {
                return false;
            }
        }
    }

    return true;
}

void setMap(const QRect &rect, Enum::Terrain terrain) {
    for (int i = rect.top(); i <= rect.bottom(); i++) {
        for (int j = rect.left(); j <= rect.right(); j++) {
            map[i][j] = terrain;
        }
    }
}

void setTerrainMap(int x, int y, Enum::Terrain terrain) {
    setMap(QRect(x * BaseSize, y * BaseSize, BaseSize, BaseSize), terrain);
    terrainMap[y][x] = terrain;
}