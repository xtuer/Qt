#ifndef CLASSDAO_H
#define CLASSDAO_H

#include <QList>
#include <QVariant>
#include <QVariantMap>

class Class;

class ClassDao {
public:
    ClassDao();

    static QList<Class> selectAllClasses();
    static QString selectClassName(int classId);

    static Class mapToClass(const QVariantMap &rowMap);
};

#endif // CLASSDAO_H
