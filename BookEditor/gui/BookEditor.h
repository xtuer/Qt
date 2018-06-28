#ifndef BOOKEDITOR_H
#define BOOKEDITOR_H

#include <QWidget>
#include <QModelIndex>
#include <QDir>

class Json;
class QJsonObject;
class QStandardItem;
class QStandardItemModel;
class QPushButton;
class BookService;

namespace Ui {
    class BookEditor;
}

class BookEditor : public QWidget {
    Q_OBJECT

public:
    explicit BookEditor(QWidget *parent = 0);
    ~BookEditor();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initialize();   // 初始化
    void handleEvents(); // 事件处理
    void createBooksContextMenu();    // 创建左侧教材树的右键菜单
    void createChaptersContextMenu(); // 创建中间章节树的右键菜单
    void resetBook();  // 重置右边的教材信息

    bool validate() const; // 校验教材和章节的编码是否唯一未被重复使用
    void save(); // 调用 saveBook() and saveBoos()
    void openBooks(); // 打开教材结构显示到左侧的教材目录树中
    void openBook(const QString &bookCode); // 打开教材内容显示到右边

    QModelIndex currentLeftIndex() const;    // 当前教材的 index
    QModelIndex currentChapterIndex() const; // 当前章节的 index

    Ui::BookEditor     *ui = NULL;
    QStandardItemModel *booksModel    = NULL; // 教材的 model
    QStandardItemModel *chaptersModel = NULL; // 章节的 model

    QPushButton *previewButton = NULL; // 预览封面按钮
    BookService *bookService   = NULL;
    QDir booksDir; // 教材所在文件夹
};

#endif // BOOKEDITOR_H
