#ifndef RECT3D_H
#define RECT3D_H

#include <QVector3D>
#include <QObject>
#include <QMetaType>

class Rect3D {
public:
    double x;
    double y;
    double z;
    double w;
    double h;
    double angle;

    Rect3D();

    void setRect(const Rect3D &rect) {
        this->x = rect.x;
        this->y = rect.y;
        this->z = rect.z;
        this->w = rect.w;
        this->h = rect.h;
    }

    void setRect(double x, double y, double z, double w, double h) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        this->h = h;
    }

    /**
     * @brief 获取矩形的中心坐标
     * @return 返回矩形的中心坐标
     */
    QVector3D center() {
        return QVector3D(x+w/2, y+h/2, z);
    }
};

Q_DECLARE_METATYPE(Rect3D)

class CarouselItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(Rect3D rect READ getRect WRITE setRect)

public:
    CarouselItem() {}

    Rect3D getRect() const {
        return rect;
    }

    void setRect(const Rect3D &rect) {
        this->rect = rect;
    }

    Rect3D rect;
};

#endif // RECT3D_H
