#ifndef UNIT_H
#define UNIT_H

#include <QString>

/**
 * 考试单元
 */
class Unit {
public:
    QString toString() const;

    QString examCode;
    QString unit;
    QString startTime;
    QString endTime;
};

#endif // UNIT_H
