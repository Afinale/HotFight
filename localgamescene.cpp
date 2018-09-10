#include "localgamescene.h"
#include "rule.h"
#include <QDebug>


LocalGameScene::LocalGameScene()
{
    // 设置Scene大小
    setSceneRect(0,0,800,400);
    // 初始化人物位置
    m_item1.setPositonInfo(200, 350);
    m_item2.setPositonInfo(500, 350);
    //设置血条精力条怒气条的位置
    m_healthBar_1.setType(P1);
    m_healthBar_2.setType(P2);
    m_healthBar_1.setPos(0,0);
    m_healthBar_2.setPos(800-m_healthBar_2.getMaxWdith(),0);

    m_energyBar_1.setType(P1);
    m_energyBar_2.setType(P2);
    m_energyBar_1.setPos(0,m_healthBar_1.getHeight());
    m_energyBar_2.setPos(800-m_energyBar_2.getMaxWdith(),m_healthBar_1.getHeight());

    m_angerBar_1.setType(P1);
    m_angerBar_2.setType(P2);
    m_angerBar_1.setPos(0,m_healthBar_1.getHeight()+m_energyBar_1.getHeight());
    m_angerBar_2.setPos(800-m_angerBar_2.getMaxWdith(),m_healthBar_1.getHeight()+m_energyBar_1.getHeight());
    //增加血条精力条怒气条
    addItem(&m_healthBar_1);
    addItem(&m_healthBar_2);
    addItem(&m_energyBar_1);
    addItem(&m_energyBar_2);
    addItem(&m_angerBar_1);
    addItem(&m_angerBar_2);

    addItem(&m_item1);
    addItem(&m_item2);
    //添加爆炸物,并且设置为不可见
    addItem(&m_explodingitem);
    m_explodingitem.setVisible(false);
    timerId = startTimer(100);
}

LocalGameScene::~LocalGameScene()
{
    killTimer(timerId);
}

void LocalGameScene::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerId)
    {
        //判断硬直状态
        if(m_item1.getTenacity()==0)
        {
            m_item1.setAttackedState(PlayerItem::ISATTACKED);
        }
        if(m_item2.getTenacity()==0)
        {
            m_item2.setAttackedState(PlayerItem::ISATTACKED);
        }
        Rule::recoverEnergy(m_item1);
        Rule::recoverEnergy(m_item2);
        Rule::recoverTenacity(m_item1);
        Rule::recoverTenacity(m_item2);
        qDebug()<<"1:"<<m_item1.getBlood()<<"||"<<m_item1.getEnergy()<<"||"<<m_item1.getTenacity();
        qDebug()<<"2:"<<m_item2.getBlood()<<"||"<<m_item2.getEnergy()<<"||"<<m_item2.getTenacity();
        isCollided(m_item1,m_item2);   //1P 和2P的 碰撞检测，必须放在攻击检测的前面，否则ISATTACKED会被替换成 ISCOLLIDEDLEFT，以后可以考虑整合成一个大函数
        //人物的攻击判定  测试
        m_item1.JudgeingAttack();
        if(m_item1.getAttackedFlag())
        {
            if(isAttacked(m_item1,m_item2))
            {
               if(m_item2.getCollidedState()==PlayerItem::ISCOLLIDEDLEFT)
               {
                   m_explodingitem.setX(m_item2.x());
                   m_explodingitem.setY(m_item2.y()-m_item2.getHeight()*4/5);
                   //把爆炸物状态视为可见
                   m_explodingitem.updatePos();
                   m_explodingitem.isItemVisable=true;
                   qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();
               }
               else if(m_item2.getCollidedState()==PlayerItem::ISCOLLIDEDRIGHT)
               {
                   m_explodingitem.setX(m_item2.x()+m_item2.getWidth());
                   m_explodingitem.setY(m_item2.y()-m_item2.getHeight()*4/5);
                   //把爆炸物状态视为可见
                   m_explodingitem.updatePos();
                   m_explodingitem.isItemVisable=true;
                   qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();
               }
            }
        }
        m_item2.JudgeingAttack();
        if(m_item2.getAttackedFlag())
        {
            if(isAttacked(m_item2,m_item1))
            {
               if(m_item1.getCollidedState()==PlayerItem::ISCOLLIDEDLEFT)
               {
                    m_explodingitem.setX(m_item1.x()+m_item1.getWidth()*2/5);
                    m_explodingitem.setY(m_item1.y()-m_item1.getHeight()*4/5);
                    //把爆炸物状态视为可见
                    m_explodingitem.updatePos();
                    m_explodingitem.isItemVisable=true;
                    qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();
               }
               else if(m_item1.getCollidedState()==PlayerItem::ISCOLLIDEDRIGHT)
               {
                   m_explodingitem.setX(m_item1.x()+m_item1.getWidth()*3/5);
                   m_explodingitem.setY(m_item1.y()-m_item1.getHeight()*4/5);
                   //把爆炸物状态视为可见
                   m_explodingitem.updatePos();
                   m_explodingitem.isItemVisable=true;
                   qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();

               }
            }
        }
//        龟派气功的伤害判定
//        如果还存在的话
        if(m_guiFlyItem!=NULL && m_item1.getState()!=PlayerItem::JUMP)
        {
            //飞行物还没有被销毁就一直保持大招状态
            m_item2.setState(PlayerItem::ULTIMATESKILL);
            //如果飞出场景之外了，那么就销毁，600暂时指的是场景的宽
            if( m_guiFlyItem->getX()<=0 || m_guiFlyItem->getX()>=600)
            {
                m_guiFlyItem->m_isExisting=false;
                delete m_guiFlyItem;
                m_guiFlyItem=NULL;
                m_item2.setState(PlayerItem::STAND);
                //将index重置
                m_item2.setUltimateSkillIndex(0);
            }
            else
            {
                if(m_guiFlyItem->collidesWithItem(&m_item1))
                {
                    //测试用
                    qDebug()<<"撞到了";
                    //伤害计算函数和碰撞效果的接口
                    //碰撞特效
                    m_explodingitem.setX(m_item1.x()+m_item1.getWidth()/2);
                    m_explodingitem.setY(m_item1.y()-m_item1.getHeight()/2);
                    //把爆炸物状态视为可见
                    m_explodingitem.updatePos();
                    m_explodingitem.isItemVisable=true;
                    //伤害和削韧计算
                    Rule::calculateBlood(m_item2,m_item1);
                    Rule::calculateTenacity(m_item2,m_item1);
                    //怒气
                    Rule::calculateAnger(m_item1,m_item1.getAngerIncrease());
                    m_item1.setState(PlayerItem::ISHITTING); //被打中了
                    //打中了之后要销毁
                    m_guiFlyItem->m_isExisting=false;
                    delete m_guiFlyItem;
                    m_guiFlyItem=NULL;
                    m_item2.setState(PlayerItem::STAND);
                    //将index重置
                    m_item2.setUltimateSkillIndex(0);
                }
            }
        }
        //刷新龟派气功
        if(m_guiFlyItem!=NULL)
        {
            m_guiFlyItem->Flying();
            m_guiFlyItem->updatePos();
        }

        //刷新爆炸物
        if(m_explodingitem.isItemVisable)
        {

            m_explodingitem.setVisible(true);
        }
        else
        {

             m_explodingitem.setVisible(false);
        }


        //刷新血条和精力条
        m_healthBar_1.setBlood(m_item1.getBlood());
        m_healthBar_2.setBlood(m_item2.getBlood());
        m_energyBar_1.setEnergy(m_item1.getEnergy());
        m_energyBar_2.setEnergy(m_item2.getEnergy());
        m_angerBar_1.setAnger(m_item1.getAnger());
        m_angerBar_2.setAnger(m_item2.getAnger());

        // 游戏线程 刷新视图和人物跑动 碰撞 跳跃
        m_item1.jump();
        m_item2.jump();
        m_item1.run();
        m_item2.run();
        m_item1.attackingMove();
        m_item2.attackingMove();
        // 碰撞
        m_item1.updatePos();
        m_item2.updatePos();
        qDebug()<<"被攻击状态"<<m_item1.getAttackedState();

        //游戏结束的判定
        if( m_item1.getBlood()<=0 && m_item2.getBlood()>0)
        {
            killTimer(timerId);
            emit gameover1PSignal();
        }
        else if(m_item2.getBlood()<=0 && m_item1.getBlood()>0)
        {
            killTimer(timerId);
            emit gameover2PSignal();
        }
        else if(m_item1.getBlood()<=0 && m_item2.getBlood()<=0)
        {
            killTimer(timerId);
            emit gameoverBothSignal();
        }
    }


}

// 事件分发函数，同时控制两个item人物
bool LocalGameScene::event(QEvent *event)
{
    switch(event->type())
    {
        case QEvent::KeyPress:
            sendEvent(&m_item1, event);
            sendEvent(&m_item2, event);
            return true;
        case QEvent::KeyRelease:
            sendEvent(&m_item1, event);
            sendEvent(&m_item2, event);
            return true;
        default:
            return GameScene::event(event);
    }
}

void LocalGameScene::stopTimer()
{
    killTimer(timerId);
}
