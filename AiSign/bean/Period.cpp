#include "Period.h"

Period::Period() {

}

QString Period::toString() const {
    return QString("periodCode: %1, period: %2, unit: %3, startTime: %4, endTime: %5")
            .arg(periodCode).arg(period).arg(unit).arg(startTime).arg(endTime);
}
