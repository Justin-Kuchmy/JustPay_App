#include <iostream>
#include "./include/MenuManager.hpp"

int main()
{
	Parser p;
	ActionDispatcher dispatcher;
	MenuManager menu(p, dispatcher);
    menu.run();
	return 0;
}
