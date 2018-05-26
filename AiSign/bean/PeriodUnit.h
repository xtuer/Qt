#ifndef PERIODUNIT_H
#define PERIODUNIT_H

#include <QString>

/**
 * 考期单元
 */
class PeriodUnit {
public:
    PeriodUnit();
    QString toString() const;

    QString periodUnitCode;
    QString period;
    QString unit;
    QString startTime;
    QString endTime;
};

#endif // PERIODUNIT_H
