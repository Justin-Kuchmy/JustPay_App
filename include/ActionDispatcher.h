#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>

class ActionDispatcher {
public:
    ActionDispatcher();
    ~ActionDispatcher();
    void regAction(const std::string& group,const std::string& action,std::function<void()> func);
    void runAction(const std::string& group, const std::string& action); 

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::function<void()>>> actions;
};
