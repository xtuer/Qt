#include <QTextCodec>
#include <QFile>
#include <QTextStream>

#include <inja/inja.hpp>

using namespace inja;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    // [1] 准备模板需要的数据
    json rows = json::array();

    for (int i = 1; i <= 10; i++) {
        json row = json::array();

        row.push_back(i);
        for (int j = 1; j <= 10; j++) {
            QString col = "列 - " + QString::number(j);
            row.push_back(col.toStdString());
        }

        rows.push_back(row);
    }

    json data;
    data["rows"] = rows;
    data["value1"] = 10;
    data["value2"] = 20;
    data["value3"] = 30;
    data["value4"] = 40;
    data["value5"] = 50;

    // [2] 模板 + 数据生成 HTML
    inja::Environment env;
    Template temp = env.parse_template("./template/report.html");
    std::string result = env.render(temp, data);
    QString html = QString::fromStdString(result);

    // [3] 输出到文件，查看效果
    QFile file("/Users/Biao/Desktop/output.html");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << html;
    file.close();

    return 0;
}
