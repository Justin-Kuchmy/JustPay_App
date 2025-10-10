#include "../include/ActionDispatcher.h"

ActionDispatcher::ActionDispatcher(){};
ActionDispatcher::~ActionDispatcher(){};

void ActionDispatcher::regAction(const std::string& group,const std::string& action,std::function<void()> func)
{
    actions[group][action] = std::move(func);
}

void ActionDispatcher::runAction(const std::string& group, const std::string& action)
{
    if (actions.contains(group) && actions[group].contains(action)) {
        actions[group][action]();
    } else {
        std::cout << "Unknown action: " << group << "::" << action << std::endl;
    }
}