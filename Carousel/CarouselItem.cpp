#include "CarouselItem.h"

CarouselItem::CarouselItem(double angle, const QString &imagePath) : angle(angle), pixmap(QPixmap(imagePath)) {

}
