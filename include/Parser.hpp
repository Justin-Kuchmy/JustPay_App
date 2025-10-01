#pragma once
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

class Parser
{
    public:
        Parser();
        ~Parser();
        std::unordered_map<std::string, Menu> menuMap;
};

Parser::Parser()
{
    std::ifstream file("./data/menu.json");
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


Parser::~Parser()
{

};
