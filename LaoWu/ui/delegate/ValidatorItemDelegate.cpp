#include "ValidatorItemDelegate.h"

#include <QLineEdit>
#include <QDebug>

struct DoubleItemDelegatePrivate {
    QString suffix;
    Qt::Alignment alignment;
    QDoubleValidator *validator;
};

/**
 * 创建一个ValidatorItemDelegate的对象.
 *
 * @param validator - validator, 例如QIntValidator, QDoubleValidator, QRegExpValidator.
 * @param alignment - 对齐方式.
 * @param parent - parent对象.
 */
DoubleItemDelegate::DoubleItemDelegate(const QString &suffix, Qt::Alignment alignment, QObject *parent)
    : QItemDelegate(parent), data(new DoubleItemDelegatePrivate) {
    data->suffix = suffix;
    data->alignment = alignment;
    data->validator = new QDoubleValidator(this);
}

DoubleItemDelegate::~DoubleItemDelegate() {
    delete data;
}

QWidget* DoubleItemDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index ) const {
    Q_UNUSED(option);
    Q_UNUSED(index);

    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setPlaceholderText(tr("输入数字"));
    lineEdit->setValidator(data->validator);

    return lineEdit;
}

void DoubleItemDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const {
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    QString text = lineEdit->text();

    if (!data->suffix.isEmpty()) { text = text.mid(0, text.indexOf(data->suffix)); }

    model->setData(index, text, Qt::UserRole);
    model->setData(index, text + data->suffix, Qt::DisplayRole);
}

void DoubleItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    lineEdit->setText(index.data(Qt::UserRole).toString());
}

void DoubleItemDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const {
    QStyleOptionViewItem o = option;
    o.displayAlignment = data->alignment;

    QItemDelegate::paint(painter, o, index);
}
