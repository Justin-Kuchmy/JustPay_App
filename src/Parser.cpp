
#include "../include/Parser.h"


Parser::Parser()
{
    std::ifstream file("../data/menu.json");
    if (!file.is_open())
    {
        std::cerr << "Error: could not open menu.json\n";
        return;
    }

    json j;
    file >> j;

    for (auto& [menuKey, menuVal] : j["menus"].items())
    {
        MenuData m;
        m.title = QString::fromStdString(menuVal["title"].get<std::string>());

        for (auto& optVal : menuVal["options"])
        {
            MenuOption opt;
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
