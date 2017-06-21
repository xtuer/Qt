#include "Comment.h"

Comment::Comment() {

}

QString Comment::toString() const {
    return QString("%1-%2-%3").arg(userId).arg(time).arg(content);
}
