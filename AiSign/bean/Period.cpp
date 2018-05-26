#include "PeriodUnit.h"

PeriodUnit::PeriodUnit() {

}

QString PeriodUnit::toString() const {
    return QString("periodUnitCode: %1, period: %2, unit: %3, startTime: %4, endTime: %5")
            .arg(periodUnitCode).arg(period).arg(unit).arg(startTime).arg(endTime);
}
