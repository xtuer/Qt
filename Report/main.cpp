#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include <string>
#include <iostream>
#include <inja/inja.hpp>

using namespace std;
using namespace inja;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // 模板由 blocks 组成，每个 block 都有属性 title 和 type，type 表示类型
    //    1: 行数据
    //    2: 图片
    //    3: 图表 - 设备数据
    json blocks = json::array();

    // [1] 第 1 个 block 是行数据
    json block1_row1; // 第 1 行
    block1_row1["name"]  = "Calibration number";
    block1_row1["value"] = "3779-0007";
    json block1_row2; // 第 2 行
    block1_row2["name"]  = "Calibration date";
    block1_row2["value"] = "17-Aug-2019";
    json block1_row3; // 第 3 行
    block1_row3["name"]  = "Start time";
    block1_row3["value"] = "3779-0007";
    json block1_row4; // 第 4 行
    block1_row4["name"]  = "End time";
    block1_row4["value"] = "15:08";

    json block1_rows = json::array();
    block1_rows.push_back(block1_row1);
    block1_rows.push_back(block1_row2);
    block1_rows.push_back(block1_row3);
    block1_rows.push_back(block1_row4);

    json block1;
    block1["type"]  = 1;
    block1["title"] = "General information";
    block1["rows"]  = block1_rows;
    blocks.push_back(block1);

    // [2] 第 2 个 block 是图片
    json block2;
    block2["type"] = 2;
    block2["title"] = "Probe Layout";
    block2["imageSrc"] = "probe-layout.png";
    blocks.push_back(block2);

    // [3] 第 3 个 block 是图表 - 设备数据
    // 表头
    json block3Headers = json::array();
    block3Headers.push_back("");
    block3Headers.push_back("");
    block3Headers.push_back("");
    block3Headers.push_back("Measured");
    block3Headers.push_back("Deviation (t90-t )");

    // 表的数据: 5 列
    // 第 1 行
    json block3Row1 = json::array();
    block3Row1.push_back("#ee0000");
    block3Row1.push_back("Ch1");
    block3Row1.push_back("A1");
    block3Row1.push_back("93.85°C");
    block3Row1.push_back("-1.15°C");
    // 第 2 行
    json block3Row2 = json::array();
    block3Row2.push_back("#ee00dd");
    block3Row2.push_back("Ch1");
    block3Row2.push_back("A1");
    block3Row2.push_back("93.85°C");
    block3Row2.push_back("-1.15°C");

    json block3Rows = json::array();
    block3Rows.push_back(block3Row1);
    block3Rows.push_back(block3Row2);

    json block3;
    block3["type"]    = 3;
    block3["title"]   = "Chanel information";
    block3["headers"] = block3Headers;
    block3["rows"]    = block3Rows;
    blocks.push_back(block3);

    // 根数据
    json data;
    data["title"]  = "Temperature Calibration Report for (q)PCR"; // 标题
    data["blocks"] = blocks;

    // 模板 + 数据生成 HTML
    inja::Environment env;
    Template temp = env.parse_template("report-demo-1-template.html");
    std::string result = env.render(temp, data);
    QString html = QString::fromStdString(result);

    // 输出到文件，浏览器打开查看效果
    QFile file("report-demo-1-output.html");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << html;
    file.close();

    return 0;
}
