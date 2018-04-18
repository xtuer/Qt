#ifndef BOOKEDITOR_H
#define BOOKEDITOR_H

#include <QWidget>

class Json;
class QJsonObject;
class QStandardItem;
class QStandardItemModel;

namespace Ui {
class BookEditor;
}

class BookEditor : public QWidget {
    Q_OBJECT

public:
    explicit BookEditor(QWidget *parent = 0);
    ~BookEditor();

private:
    void handleEvents();
    void createChapters(const Json &json, const QJsonObject &chapter, QStandardItem *parentItem);

    Ui::BookEditor *ui = NULL;
    QStandardItemModel *chapterModel = NULL;
};

#endif // BOOKEDITOR_H
