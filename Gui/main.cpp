#include <QApplication>
#include <QDebug>
#include <inja/inja.hpp>

using namespace inja;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    json data;
    data["name"] = "world";
    data["showF0"] = true;
    data["showF1"] = false;

    qDebug() << QString::fromStdString(render("{% if showF0 %} F0 {{name}} {% endif %}  --- {% if showF1 %} F1 {% endif %}", data)); // Returns std::string "Hello world"

    return 0;
}
