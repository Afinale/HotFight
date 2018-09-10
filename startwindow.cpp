#include "startwindow.h"
#include "ui_startwindow.h"
#include <QPalette>
#include <QCloseEvent>

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWindow)
{    
    // 设置固定大小
    setFixedSize(QSize(600,500));
    ui->setupUi(this);
    setWindowTitle("开始游戏");
    // 设置窗口背景图
    setAutoFillBackground(true);
    QPalette back = this->palette();
    setStyleSheet("background-repeat:no-repeat;");
    back.setBrush(QPalette::Window, QBrush(
                      QPixmap(":/images/startui_background.gif").scaled(
                          this->size(),
                          Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation)));
    setPalette(back);
    setWindowIcon(QIcon(":/images/icon.png"));
    // 初始化指针
    m_localGame = nullptr;
    m_netConnectDialog = nullptr;
}

StartWindow::~StartWindow()
{
    delete ui;
//    if(m_localGame != nullptr)
//        delete m_localGame;
//    if(m_netConnectDialog != nullptr)
//        delete m_netConnectDialog;
}

void StartWindow::on_btnLocalGame_clicked()
{
    // 打开单机游戏画面
    m_localGame = new LocalGameWindow();
    m_localGame->show();
}

void StartWindow::on_btnNetGame_clicked()
{
   m_netConnectDialog= new NetConnectDialog;
   m_netConnectDialog->show();;
}

void StartWindow::on_btnHelp_clicked()
{
    m_helpWindow = new Help();
    m_helpWindow->show();
}

