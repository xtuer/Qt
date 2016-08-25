#include "DateItemDelegate.h"

#include <QDateEdit>

struct DateItemDelegatePrivate {
    QString format;
    QDate defaultDate;
};

DateItemDelegate::DateItemDelegate(const QDate &defaultDate, QObject *parent) :
    QItemDelegate(parent), data(new DateItemDelegatePrivate) {
    data->format = "yyyy年MM月dd日";
    data->defaultDate = defaultDate;
}

DateItemDelegate::~DateItemDelegate() {
    delete data;
}

QWidget *DateItemDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);

    QDateEdit *dateEdit = new QDateEdit(parent);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(data->defaultDate);
    dateEdit->setDisplayFormat(data->format);

    return dateEdit;
}

void DateItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
    QString dateString = index.data().toString();
    QDate date = QDate::fromString(dateString, data->format);
    dateEdit->setDate(date);
}

void DateItemDelegate::setModelData(QWidget *editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex &index) const {
    QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
    QString dateString = dateEdit->date().toString(data->format);
    model->setData(index, dateString);
}
