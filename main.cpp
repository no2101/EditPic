#include "mainwindow.h"    // 包含主窗口头文件
#include <QApplication>     // 包含Qt应用程序类头文件

// 程序入口函数
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    // 创建Qt应用程序对象，处理命令行参数
    MainWindow w;                  // 创建主窗口对象
    w.show();                      // 显示主窗口

    return a.exec();               // 进入应用程序事件循环，等待用户操作
}
