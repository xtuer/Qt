#include "UiUtil.h"
#include "Config.h"
#include "HttpClient.h"

#include <QDebug>
#include <QUrl>
#include <QImage>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QApplication>
#include <QCryptographicHash>

#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QStackedWidget>
#include <QSpacerItem>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QModelIndex>
#include <QShortcut>
#include <QDesktopWidget>
#include <QScreen>
#include <QFontDatabase>

// 为整个应用程序加载 QSS
void UiUtil::loadQss() {
    QStringList qssFileNames = ConfigInstance.getQssFiles();
    QString qss;

    for (const QString &name : qssFileNames) {
        qDebug().noquote() << QString("Loading QSS file: %1").arg(name);

        QFile file(name);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug().noquote() << QString("Error: Loading QSS file: %1 failed").arg(name);
            continue;
        }

        qss.append(file.readAll()).append("\n");
        file.close();
    }

    if (!qss.isEmpty()) {
        qApp->setStyleSheet(qss);
    }
}

// 加载字体
void UiUtil::loadFonts() {
    QStringList fontFiles = ConfigInstance.getFontFiles();

    for (const QString &file : fontFiles) {
        qDebug().noquote() << QString("Loading Font file: %1").arg(file);
        QFontDatabase::addApplicationFont(file);
    }
}

// 修改过 widget 的属性后，使此属性对应的 Style Sheet 生效
void UiUtil::updateQss(QWidget *widget) {
    widget->setStyleSheet("/**/");
}

// 安装加载 QSS 的快捷键: Ctrl + L
void UiUtil::installLoadQssShortcut(QWidget *parent) {
    // 按下 Ctrl + L 加载 QSS
    QShortcut *loadQssShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), parent);
    QObject::connect(loadQssShortcut, &QShortcut::activated, [] {
        UiUtil::loadQss();
    });
}

// 把 widget 加入到 stacked widget 里, 可以设置向四个方向的伸展
void UiUtil::addWidgetIntoStackedWidget(QWidget *widget, QStackedWidget *stackedWidget,
                                        bool toLeft,
                                        bool toTop,
                                        bool toRight,
                                        bool toBottom) {
    // 使用 widget 居左上
    QGridLayout *layout = new QGridLayout();

    QSpacerItem *spacer = nullptr;
    if (!toLeft) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout->addItem(spacer, 1, 0);
    }

    if (!toTop) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(spacer, 0, 1);
    }

    if (!toRight) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout->addItem(spacer, 1, 2);
    }

    if (!toBottom) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(spacer, 2, 1);
    }

    QWidget *container = new QWidget();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(widget, 1, 1);
    container->setLayout(layout);
    stackedWidget->addWidget(container);
}

// 把使用上面的函数 addWidgetIntoStackedWidget 添加过的 widget 设置为它的当前 widget
void UiUtil::setCurrentWidgetOfStackedWidget(QWidget *widget, QStackedWidget *stackedWidget) {
    for (; nullptr != widget; widget = widget->parentWidget()) {
        if (widget->parentWidget() == stackedWidget) {
            stackedWidget->setCurrentWidget(widget);
            break;
        }
    }
}

// 设置 widget 的 padding 和 spacing
void UiUtil::setWidgetPaddingAndSpacing(QWidget *widget, int padding, int spacing) {
    // 设置 Widget 的 padding 和 spacing
    QLayout *layout = widget->layout();

    if (nullptr != layout) {
        layout->setContentsMargins(padding, padding, padding, padding);
        layout->setSpacing(spacing);
    }
}

QModelIndex UiUtil::getSelectedIndex(QAbstractItemView *view) {
    return view->selectionModel()->currentIndex();
}

void UiUtil::appendTableViewRow(QTableView *view, int editColumn) {
    QAbstractItemModel *model = view->model();
    int row = model->rowCount();
    model->insertRow(row);

    QModelIndex index = model->index(row, editColumn);
    if (!index.isValid()) { return; }

    view->setCurrentIndex(index);
    view->edit(index);
}

void UiUtil::removeTableViewSelectedRow(QTableView *view) {
    QModelIndex index = getSelectedIndex(view);

    if (index.isValid()) {
        view->model()->removeRow(index.row());
    }
}

// 移动窗口到屏幕的中间
void UiUtil::centerWindow(QWidget *window) {
    // This doesn't show the widget on the screen since you don't relinquish control back to the queue
    // until the hide() happens. In between, the invalidate() computes the correct positions.
    window->show();
    window->layout()->invalidate();
    window->hide();

    // QSize size = qApp->desktop()->availableGeometry().size() - window->size();
    QSize size = qApp->primaryScreen()->availableSize() - window->size();
    int x = qMax(0, size.width() / 2);
    int y = qMax(0, size.height() / 2);
    window->move(x, y);
}

// 查找 w 所在窗口的顶层窗口
QWidget *UiUtil::findWindow(QWidget *w) {
    QWidget *p = w;

    while (nullptr != p->parentWidget()) {
        p = p->parentWidget();
    }

    return p;
}

// 返回 view 在全局坐标 globalPosition 处的 index
QModelIndex UiUtil::indexAt(QAbstractItemView *itemView, const QPoint &globalPosition) {
    QPoint posAtViewport = itemView->viewport()->mapFromGlobal(globalPosition);
    QModelIndex index = itemView->indexAt(posAtViewport);

    return index;
}

// 给 QLineEdit 最右边创建一个按钮
QPushButton *UiUtil::createLineEditRightButton(QLineEdit *edit) {
    QPushButton *button = new QPushButton();
    QHBoxLayout *layout = new QHBoxLayout();
    button->setCursor(Qt::ArrowCursor);
    button->setProperty("class", "LineEditButton");
    layout->addStretch();
    layout->addWidget(button);
    edit->setLayout(layout);
    UiUtil::setWidgetPaddingAndSpacing(edit, 0, 0);

    return button;
}

// 删除 widget 中的所有子 widgets
void UiUtil::emptyWidget(QWidget *widget) {
    QLayout *layout = widget->layout();
    QLayoutItem *item = nullptr;

    if (nullptr == layout) {
        return;
    }

    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        } else {
            delete item;
        }
    }
}
