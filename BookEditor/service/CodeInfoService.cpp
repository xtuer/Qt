#include "CodeInfoService.h"
#include "bean/Book.h"

#include <QDebug>

// Model 有 2 列，第一列为名字，第二列为 code，读取 model 中的每一行生成一个 CodeInfo 对象
QList<CodeInfo> CodeInfoService::getCodeInfos(QStandardItemModel *model) {
    QList<CodeInfo> codeInfos;

    for (int i = 0; i < model->rowCount(); ++i) {
        QStandardItem *nameItem = model->item(i, 0);
        QStandardItem *codeItem = model->item(i, 1);

        CodeInfoService::travelCodeInfoModel(nameItem, codeItem, &codeInfos);
    }

    return codeInfos;
}

// 校验 CodeInfo 数组中的 code 是否被重复使用，如果有重复使用时返回 false 并且 error 中记录重复使用的 code:info 数据
// 没有重复使用则返回 true
bool CodeInfoService::validateCodeInfos(QList<CodeInfo> *codeInfos, QString *error) {
    // 1. 先排序：先根据 code 排序，code 相同再根据 info 排序
    // 2. 对有序数组进行扫描，找出重复的数据

    // [1] 先排序：先根据 code 排序，code 相同再根据 info 排序
    std::sort(codeInfos->begin(), codeInfos->end(), [](const CodeInfo & a, const CodeInfo & b) -> bool {
        if (a.code == b.code) {
            return a.info < b.info;
        } else {
            return a.code < b.code;
        }
    });

    // 打印出所有的 CodeInfo 信息，便于调试查看
    for (const CodeInfo &ci : *codeInfos) {
        qDebug().noquote() << QString("%2 (%1)").arg(ci.code).arg(ci.info);
    }

    // [2] 对有序数组进行扫描，找出重复的数据
    // 逻辑:
    //     1. j 初始化为 i+1，从 i 下一个开始进行比较
    //     2. 当第 j 行和第 i 个不同时，结束 j 所在循环，并赋值 i 为 j，进行 i 所在的循环
    //     3. 当第 j 个和第 i 个相同时，记录数据，继续 j 所在的循环
    bool ok = true;
    int length = codeInfos->size();
    for (int i = 0, j = 0; i < length; i = j) {
        bool    first = true;
        QString firstCode = codeInfos->at(i).code;
        QString firstInfo = codeInfos->at(i).info;

        for (j = i+1; j < codeInfos->size(); ++j) {
            QString currentCode = codeInfos->at(j).code;
            QString currentInfo = codeInfos->at(j).info;

            if (currentCode != firstCode) {
                // 找到不同数据，结束循环
                break;
            } else {
                // 找到相同数据，记录重复数据信息
                // 错误信息格式: 高中 > 化学 > 人教版 > 选修4 (gz_hx_rjb_xx04)
                ok = false;

                if (first) {
                    first = false;
                    error->append(QString("%2 (%1)\n").arg(firstCode).arg(firstInfo));
                }

                error->append(QString("%2 (%1)\n").arg(currentCode).arg(currentInfo));
            }
        }
    }

    return ok;
}

void CodeInfoService::travelCodeInfoModel(QStandardItem *nameItem, QStandardItem *codeItem, QList<CodeInfo> *codeInfos) {
    if (NULL == nameItem || NULL == codeItem) { return; }

    QString name = nameItem->data(Qt::DisplayRole).toString();
    QString code = codeItem->data(Qt::DisplayRole).toString();
    codeInfos->append(CodeInfo(code, name));

    // 递归遍历子节点
    for (int i = 0; i < nameItem->rowCount(); ++i) {
        QStandardItem *childNameItem = nameItem->child(i, 0);
        QStandardItem *childCodeItem = nameItem->child(i, 1);
        travelCodeInfoModel(childNameItem, childCodeItem, codeInfos);
    }
}
