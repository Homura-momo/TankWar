#include "EnemySpeed.h"

EnemySpeed::EnemySpeed(int x, int y, QTimer *moveTimer, QTimer *bulletTimer, Player *player) : Enemy(x, y, EnemySpeedLife, EnemySpeedSpeed, moveTimer, player, QString(EnemySpeedImg), new BulletSpeed(bulletTimer, Camp::Enemy)) {

}