#include "Player1.h"

Player1::Player1(QTimer *moveTimer, QTimer *bulletTimer) : Player(Player1X, Player1Y, Enum::Down, moveTimer, Camp::Player1, QString(Player1Img), new BulletSpeed(bulletTimer, Camp::Player1)) {

}