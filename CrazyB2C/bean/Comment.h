#ifndef COMMENT_H
#define COMMENT_H

#include <QString>

class Comment {
public:
    Comment();

    int productId;   // 产品的 ID
    QString userId;  // 用户名
    QString content; // 评价内容
    qulonglong time; // 评价时间，毫秒

    QString toString() const;
};

#endif // COMMENT_H
