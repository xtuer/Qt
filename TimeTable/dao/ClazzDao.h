#ifndef CLAZZDAO_H
#define CLAZZDAO_H

class Clazz;
class QSqlQuery;
template<typename T> class QList;

class ClazzDao {
public:
    static int findById(int id, Clazz *clazz);
    static int findAllClazzes(QList<Clazz*> *clazzes);

private:
    static void mapRow(Clazz *clazz, QSqlQuery *query);
};

#endif // CLAZZDAO_H
