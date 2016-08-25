#ifndef EXPORTUTIL_H
#define EXPORTUTIL_H

class TimeTableData;
class TimeTableItem;
class QString;
template <typename T> class QList;

class ExportUtil {
public:
    static QString exportTimeTable(const TimeTableData &timeTableData,
                                   const QList<TimeTableItem*> &items,
                                   const QString &htmlTemplate,
                                   const QString &title,
                                   bool  forClazz);
    static void saveTimeTable(const TimeTableData &timeTableData, const QList<int> ids, bool forClazz);
};

#endif // EXPORTUTIL_H
