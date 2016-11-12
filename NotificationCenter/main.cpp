#include <QApplication>
#include <QDebug>

#include "NotificationCenter.h"
#include "FooObserver.h"
#include "BarObserver.h"
#include "Message.h"
#include "NotifyThread.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    NotificationCenter &nc = Singleton<NotificationCenter>::getInstance();
    FooObserver foo; // 通知的观察者
    BarObserver bar; // 通知的观察者

    // [1] 测试添加观察者
    // 通知 1 有 2 个观察者 foo and bar
    // 通知 1 有 1 个观察者 bar
    qDebug() << "---------------- foo and bar 都能收到通知 1 和 2 ----------------";
    nc.addObserver(1, &foo);
    nc.addObserver(1, &bar);
    nc.addObserver(2, &bar);
    nc.notify(1, QString("One").toUtf8()); // foo and bar 都收到消息
    NOTIFY_CROSS_THREAD(2, QString("Two").toUtf8()); // bar 收到消息(使用宏发送通知)

    // [2] 测试删除观察者
    // 删除通知 1 的观察者 bar，它还剩下观察者 foo
    // 通知 2 有观察者 bar
    qDebug() << "---------------- bar 不再收到通知 1，但能收到通知 2 ----------------";
    nc.removeObserver(1, &bar); // nc.removeObserver(&bar) 则 bar 不在接收到任何通知
    nc.notify(1, QString("Three").toUtf8()); // foo 收到消息
    nc.notify(2, QString("Four").toUtf8());  // bar 收到消息，删除通知 1 的观察者，通知 2 的观察者不受影响

    // [3] 测试把对象序列化和反序列化
    qDebug() << "---------------- 序列化例子 ----------------";
    nc.addObserver(3, &foo);
    Message msg(12345, "你是谁？你从哪里来？你要到哪里去？");
    nc.notify(3, msg.toByteArray());

    // [4] 跨线程发送通知
    qDebug() << "---------------- 跨线程发送通知 ----------------";
    nc.addObserver(4, &foo);
    NotifyThread thread;
    thread.start();

    return app.exec();
}
