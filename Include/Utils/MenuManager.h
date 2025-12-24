#ifndef MENUMANAGER_HPP 
#define MENUMANAGER_HPP
#include <QWidget>
#include <QStackedWidget>
#include <QString>
#include <QStack>
#include <unordered_map>
#include "BaseMenu.h"
#include "Parser.h"
#include "BaseContentWidget.h"
#define DEBUG_LOGS
#include "Log.h"

class MenuManager: public QWidget
{
    Q_OBJECT
public:
    explicit MenuManager(Parser& parser, QWidget* parent = nullptr);
    MenuManager(const MenuManager&) = delete; 
    MenuManager& operator=(const MenuManager&) = delete;
    void showMenu(const QString& name);
    BaseMenu* getMenu(const QString& name) const;

    void showContentWidget(QWidget* widget);
private: 
    void buildMenus(Parser& parser);
    std::unique_ptr<QStackedWidget> stacked;
    QWidget* current;
    QWidget* previous;
    QStack<QWidget*> history; 
    QHash<QString, BaseMenu*> menuLookup;
};

#endif 

