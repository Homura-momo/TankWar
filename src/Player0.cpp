#include "Player0.h"

Player0::Player0(QTimer *moveTimer, QTimer *bulletTimer) : Player(Player0X, Player0Y, Enum::Up, moveTimer, Camp::Player0, QString(Player0Img), new BulletSpeed(bulletTimer, Camp::Player0)) {

}