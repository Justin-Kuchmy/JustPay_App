#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <optional>   
#include <unordered_map>
#include <vector>
using json = nlohmann::json;

struct Option {
    std::string label;
    std::optional<std::string> submenu;
    std::optional<std::string> action;  
};

struct Menu {
    std::string title;
    std::vector<Option> options;
};

class parser
{
    public:
        parser();
        ~parser();
        std::unordered_map<std::string, Menu> menuMap;
};

parser::parser()
{
    std::ifstream file("menu.json");
    if(!file.is_open())
    {
        std::cerr << "Error: could not open menu.json\n";
        return;
    }
    json j;
    file >> j;

    for (auto& [menuKey, menuVal] : j["menus"].items()) 
    {
        Menu m;
        m.title = menuVal["title"].get<std::string>();
        std::cout << m.title << std::endl;

        for (auto& optVal : menuVal["options"]) 
        {
            Option opt;
            opt.label = optVal["label"].get<std::string>();

            if (optVal.contains("submenu")) {
                opt.submenu = optVal["submenu"].get<std::string>();
            }
            if (optVal.contains("action")) {
                opt.action = optVal["action"].get<std::string>();
            }

            m.options.push_back(opt);
        }

        menuMap[menuKey] = m;
    }
};


parser::~parser()
{

};
