#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QSharedMemory>
#include <QSystemSemaphore>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    // 创建信号量
    // 启用信号量，禁止其他实例通过共享内存一起工作
    QSystemSemaphore semaphore("SingleITSemaphore",1);
    semaphore.acquire();

    //  在linux / unix 中程序异常结束共享内存不会回收
    // 在这里需要提供释放内存的接口，就是在程序运行的时候如果有这段内存 先清除掉
    QSharedMemory nix_fix_shared_memory("SingleIT");
    if (nix_fix_shared_memory.attach())
    {
        nix_fix_shared_memory.detach();
    }

    // 创建一个共享内存  “SingleIT”表示一段内存的标识key 可作为唯一程序的标识，并测试是否已经运行，
    bool isRunning = false;
    QSharedMemory sharedMemory("SingleIT");
    // 试图将共享内存的副本附加到现有的段中。
    if (sharedMemory.attach()) {
        // 如果成功，则确定已经存在运行实例
        isRunning = true;
    }else{
        // 否则申请一字节内存
        sharedMemory.create(1);
        // 确定不存在运行实例
        isRunning = false;
    }
    semaphore.release();

    // 如果您已经运行了应用程序的一个实例，那么将通知用户。
    if(isRunning) {
        QMessageBox::warning(nullptr,QObject::tr("温馨提示"),QObject::tr("屏幕提示器在当前用户已经打开，请不要重复打开！"),QObject::tr("确定"));
        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}


// fedora 不正常 疑似没有Qt环境
// deepin 非窗口特效异常
