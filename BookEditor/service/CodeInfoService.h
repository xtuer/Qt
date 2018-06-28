#ifndef CODEINFOSERVICE_H
#define CODEINFOSERVICE_H

#include <QList>
#include <QStandardItemModel>

class CodeInfo;

class CodeInfoService {
public:
    /**
     * Model 有 2 列，第一列为名字，第二列为 code，读取 model 中的每一行生成一个 CodeInfo 对象
     *
     * @param model
     * @return 返回 CodeInfo 的列表
     */
    static QList<CodeInfo> getCodeInfos(QStandardItemModel *model);

    /**
     * 校验 CodeInfo 数组中的 code 是否被重复使用，如果有重复使用时返回 false 并且 error 中记录重复使用的 code:info 数据，
     * 没有重复使用则返回 true
     *
     * @param codeInfos CodeInfo 数组
     * @param error 校验不通过的错误信息，不能为 NULL
     * @return 校验通过返回 true，不通过返回 false
     */
    static bool validateCodeInfos(QList<CodeInfo> *codeInfos, QString *error);

private:
    static void travelCodeInfoModel(QStandardItem *nameItem,
                                    QStandardItem *codeItem,
                                    QList<CodeInfo> *codeInfos);
};

#endif // CODEINFOSERVICE_H
