#include "Unit.h"

QString Unit::toString() const {
    return QString("examCode: %1, unit: %2, startTime: %3, endTime: %4")
            .arg(examCode).arg(unit).arg(startTime).arg(endTime);
}
