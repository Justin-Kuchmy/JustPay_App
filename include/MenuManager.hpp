#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include <iostream>
#include <ostream>
#include <stack>
#include <string>
#include "Parser.hpp"
#include "ActionDispatcher.hpp"

class MenuManager {
    Parser& p;
    ActionDispatcher& dispatcher;
    std::stack<std::string> menuStack;
    Menu* currMenu;
    friend std::ostream& operator<<(std::ostream& os, std::stack<std::string> stack);
    friend std::ostream& operator<<(std::ostream& os, Option option);
    friend std::ostream& operator<<(std::ostream& os, Menu menu);

    public:
        MenuManager(Parser& Parser, ActionDispatcher& disp);
        ~MenuManager();
        void run();

    private:
        void displayMenu();
        int getUserChoice(int menuSize);
        void handleChoice(int choice);
};

#endif // MENU_MANAGER_HPP

void clearScreen() {
    std::cout << "\033[2J\033[1;1H\n\n"; 
}


inline std::ostream& operator<<(std::ostream& os, std::stack<std::string> stack)
{
    while (!stack.empty())
    {
        if(stack.size() > 1)
        {
            os << stack.top() << " "; 
        }
        else
        {
            os << stack.top();
        }
        stack.pop(); 
    }
    return os; 
}

inline std::ostream& operator<<(std::ostream& os, Option option)
{
    os << "Label: " << option.label << std::endl;
    if(option.submenu.has_value())
    {
        os << "SubMenu: " << option.submenu.value() << std::endl;
    }
    else if (option.action.has_value())
    {
        os << "SubMenu: " << option.action.value() << std::endl;
    }
    return os;
}
inline std::ostream& operator<<(std::ostream& os, Menu menu)
{
    os << "title: " << menu.title << std::endl;
    for (size_t i = 0; i < menu.options.size(); i++)
    {
        os << "Option " << i+1 << " "<< menu.options[i] << std::endl;
        
    }
    return os;
    
}

inline MenuManager::MenuManager(Parser& Parser, ActionDispatcher& disp): p(Parser), dispatcher(disp) 
{
    //std::cout << "___________MenuManager  Created___________" << std::endl;
    menuStack.push("main");
    currMenu = &p.menuMap["main"];
};

inline MenuManager::~MenuManager()
{
std::cout << "___________MenuManager  Destroyed___________" << std::endl;
};
inline void MenuManager::run() {
    int menuSize = currMenu->options.size();
    while (!menuStack.empty()) {
        displayMenu();
        int choice = getUserChoice(menuSize);
        handleChoice(choice);
    }
}
inline void MenuManager::displayMenu()
{
    clearScreen();
    std::cout << "Stack: [" << menuStack << "]" << std::endl;
    std::cout << currMenu->title << std::endl;
    for (size_t i = 0; i < p.menuMap[menuStack.top()].options.size(); i++)
    {
        std::cout << i+1 <<". " << p.menuMap[menuStack.top()].options[i].label << std::endl;
    }
};
inline int MenuManager::getUserChoice(int maxOption)
{
    int choice = 0;
    while(true)
    {
        std::cout << "Pick an Option: ";
        if(std::cin >> choice && choice >= 1 && choice <= maxOption)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again.\n";
    }
    return choice;
};
inline void MenuManager::handleChoice(int choice)
{
    Option chosen = currMenu->options[choice-1];
    if(chosen.submenu.has_value())
    {
        if(menuStack.top() != chosen.submenu.value())
            menuStack.push(chosen.submenu.value());
        currMenu = &p.menuMap[menuStack.top()];
    }
    else if (chosen.action.has_value())
    {
        if (chosen.action == "back" || chosen.action == "exit") 
        {
            menuStack.pop(); // same behavior
            currMenu = &p.menuMap[menuStack.top()];
        }
        else 
        {
            int delim = menuStack.top().find_first_of("_");
            dispatcher.runAction(menuStack.top().substr(0, delim), chosen.action.value()); 
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
        }
    }
};

