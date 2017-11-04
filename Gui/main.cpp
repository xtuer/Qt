#include <QApplication>
#include <QDebug>
#include "Widget.h"
#include <stdlib.h>

struct Point {
    int x;
    int y;
};

struct Line {
    Point *p1;
    Point *p2;
};

struct Stage {
    int attr1;
    int attr2;
    Line *line;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    int size = sizeof(Stage) + sizeof(Line) + sizeof(Point) * 2;
    Stage *stage = (Stage*) malloc(size);
    stage->line  = (Line*)(stage + 1);
    stage->line->p1 = (Point*)(stage->line + 1);
    stage->line->p2 = (Point*)(stage->line->p1 + 1);

    stage->line->p1->x = 10;
    stage->line->p1->y = 20;

    qDebug() << stage->line->p1->y;

    return 0;
}
