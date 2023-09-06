#include "EnemyLife.h"

EnemyLife::EnemyLife(int x, int y, QTimer *moveTimer, QTimer *bulletTimer, Player *player) : Enemy(x, y, EnemyLifeLife, EnemyLifeSpeed, moveTimer, player, QString(EnemyLifeImg), new BulletATK(bulletTimer, Camp::Enemy)) {

}