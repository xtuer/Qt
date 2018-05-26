#ifndef PERIODUNIT_H
#define PERIODUNIT_H

#include <QString>

/**
 * 考期
 */
class Period {
public:
    Period();
    QString toString() const;

    QString periodCode;
    QString period;
    QString unit;
    QString startTime;
    QString endTime;
};

#endif // PERIODUNIT_H
