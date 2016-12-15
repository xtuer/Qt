#ifndef BAR_H
#define BAR_H

#include <QObject>

namespace boo {

class Bar : public QObject {
    Q_OBJECT
public:
    explicit Bar(QObject *parent = 0);

public slots:
    void say(int i);
};
}
#endif // BAR_H
