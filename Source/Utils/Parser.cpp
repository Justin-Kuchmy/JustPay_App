
#include "Utils/Parser.h"
#include <qstring.h>

#include <qfile.h>


Parser::Parser(): menuMap{}
{
    QFile file(":/resources/menu.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cerr << "Error: could not open menu.json\n";
        return;
    }

    QByteArray data = file.readAll();
    json j = json::parse(data.toStdString());

    for (auto& [menuKey, menuVal] : j["menus"].items())
    {
        MenuData m{};
        m.title = QString::fromStdString(menuVal["title"].get<std::string>());

        for (auto& optVal : menuVal["options"])
        {
            MenuOption opt{};
            opt.label = QString::fromStdString(optVal["label"].get<std::string>());

            if (optVal.contains("submenu")) {
                opt.submenu = QString::fromStdString(optVal["submenu"].get<std::string>());
            }

            if (optVal.contains("action")) {
                opt.action = QString::fromStdString(optVal["action"].get<std::string>());
            }

            m.options.push_back(opt);
        }

        menuMap[menuKey] = m;
    }
}


Parser::~Parser()
{

};
