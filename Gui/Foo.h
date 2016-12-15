#ifndef FOO_H
#define FOO_H

#include <QObject>
namespace xtuer {

class Foo : public QObject {
    Q_OBJECT
public:
    explicit Foo(QObject *parent = 0);
    void say();

signals:
    void hello(int);

public slots:
    void bar();
};
}
#endif // FOO_H
