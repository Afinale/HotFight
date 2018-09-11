#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "guiflyitem.h"
#include "rule.h"
#include "player1p.h"
#include "player2p.h"
#include <QSound>
#include <QEvent>
#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QGraphicsScene>



enum NETFLAG{SERVER, CLIENT};
class GameScene: public QGraphicsScene
{

public:
    GameScene();
    ~GameScene();
protected:
    //与游戏规则有关的函数
    bool isAttacked( PlayerItem& attackingitem, PlayerItem& attackeditem2); //判断攻击的函数
    //判断1P和2P是否碰上，如果item1的左边被挡住就返回-1，右边被挡住返回0,没有被挡住返回1(同时该函数也能够设定item2的状态，但是返回的是Item1的）
    int isCollided( PlayerItem& originalItem1, PlayerItem& targetItem2);
    GuiFlyItem* m_guiFlyItem;

private: 
    // scene大小
    QRectF size;
    // 音效
    QSound *m_riotHitSound;
    QSound *m_swordHitSound;
};


#endif // GAMESCENE_H
