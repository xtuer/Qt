#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QMutex>

class Comment;

namespace Ui {
class CommentWidget;
}

class CommentWidget : public QWidget {
    Q_OBJECT

public:
    explicit CommentWidget(QWidget *parent = 0);
    ~CommentWidget();

private:
    void loadSettings(); // 加载配置信息
    QStringList loadUsers();    // 加载用户
    QStringList loadDefaultComments(); // 加载预先定义评价
    QStringList loadCustomComments();  // 加载自定义评价

    QList<QDate> generateDates() const; // 生成评价时间
    QList<Comment> generateComments(); // 生成评价

    void submitComments();
    void oneRequestFinish();

    void showStatus(const QString status); // 显示状态
    void shuffleList(QStringList &strs);

    Ui::CommentWidget *ui;
    QStringList users;
    QStringList defaultComments;
    bool isProductPlatform;

    int totalCount;
    int finishedCount;
    QNetworkAccessManager * networkAccessManager;
    QMutex mutex;
};

#endif // COMMENTWIDGET_H
