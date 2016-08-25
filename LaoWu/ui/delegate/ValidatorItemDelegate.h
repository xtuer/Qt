#ifndef REGEXPITEMDELEGATE_H
#define REGEXPITEMDELEGATE_H

#include <QItemDelegate>

struct DoubleItemDelegatePrivate;

/**
 * 使用validator的item delegate. Editor会对输入的数据进行验证, 如果验证失败则输入无校.
 * 例如可以传入一个整数, 浮点数的validator, 例如QIntValidator, QDoubleValidator.
 */
class DoubleItemDelegate : public QItemDelegate {
public:
    /**
     * 创建一个ValidatorItemDelegate的对象.
     *
     * @param validator - validator, 例如QIntValidator, QDoubleValidator, QRegExpValidator.
     * @param alignment - 对齐方式.
     * @param parent - parent对象.
     */
    explicit DoubleItemDelegate(const QString &suffix, Qt::Alignment alignment = Qt::AlignCenter, QObject *parent = NULL);
    ~DoubleItemDelegate();

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    DoubleItemDelegatePrivate *data;
};

#endif // REGEXPITEMDELEGATE_H
