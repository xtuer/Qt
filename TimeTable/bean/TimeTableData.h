#ifndef TIMETABLEDATA_H
#define TIMETABLEDATA_H

#include "bean/Accessor.h"

#include <QList>
#include <QString>
#include <QMetaType>
#include <QDataStream>

class TimeTableData {
public:
    TimeTableData();

    GETTER(row,       getRow, int)
    GETTER(col,       getCol, int)
    GETTER(rests,     getRests, QList<int>)
    GETTER(dayTexts,  getDayTexts,  QList<QString>)
    GETTER(timeTexts, getTimeTexts, QList<QString>)
    GETTER(restTexts, getRestTexts, QList<QString>)

    SETTER(row,        setRow, int)
    SETTER(col,        setCol, int)
    SETTER(rests,      setRests, QList<int>)
    SETTER(dayTitles,  setDayTexts,  QList<QString>)
    SETTER(timeTitles, setTimeTexts, QList<QString>)
    SETTER(restTitles, getRestTexts, QList<QString>)

    bool isRest(int row) const;
    void appendRest();
    void removeRest();

    QString getDayText(int index) const;
    QString getTimeText(int index) const;
    QString getRestText(int index) const;

    void setDayText(int index, const QString &text);
    void setTimeText(int index, const QString &text);
    void setRestText(int index, const QString &text);

    friend QDataStream& operator<<(QDataStream &out, const TimeTableData &data);
    friend QDataStream& operator>>(QDataStream &in, TimeTableData &data);

private:
    int row;
    int col;

    QList<int> rests;
    QList<QString> dayTexts;
    QList<QString> timeTexts;
    QList<QString> restTexts;
};

Q_DECLARE_METATYPE(TimeTableData)

#endif // TIMETABLEDATA_H
