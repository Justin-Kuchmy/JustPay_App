

#include "../Include/MenuManager.h"


MenuManager::MenuManager(Parser& parser, QWidget* parent)
    : QWidget(parent), stacked(new QStackedWidget(this))
{
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(stacked);
    setLayout(layout);

    buildMenus(parser);

    // Optional: show main menu or first menu
    if (auto* mainMenu = getMenu("main"))
        showMenu("main");
    else
        stacked->setCurrentIndex(0);
};


void MenuManager::buildMenus(Parser& parser)
{
    // Keep a lookup from menu name to widget
    QHash<QString, BaseMenu*> menuLookup;

    // Create all menu widgets
    for (auto& [key, data] : parser.menuMap)
    {
        auto* menuWidget = new BaseMenu(data, stacked);
        menuWidget->setObjectName(QString::fromStdString(key));
        stacked->addWidget(menuWidget);
        menuLookup[QString::fromStdString(key)] = menuWidget;
    }

     // Connect navigation signals
    for (auto& [key, _] : parser.menuMap)
    {
        auto* menuWidget = menuLookup[QString::fromStdString(key)];

        // Submenu navigation
        connect(menuWidget, &BaseMenu::submenuRequested, this,
                [this](const QString& submenuName) {
                    if (auto* submenu = getMenu(submenuName))
                    {
                        history.push(stacked->currentWidget()); // record current menu
                        stacked->setCurrentWidget(submenu);
                    }
                    else
                    {
                        qWarning() << "Submenu not found:" << submenuName;
                    }
                });

        // Back navigation
        connect(menuWidget, &BaseMenu::backRequested, this, [this]() {
            if (!history.isEmpty()) {
                QWidget* prev = history.pop();
                stacked->setCurrentWidget(prev);
            } else {
                qDebug() << "Already at root menu";
            }
        });
    }   


}

void MenuManager::showMenu(const QString& name)
{
    auto* menu = getMenu(name);
    if (menu)
    {
        stacked->setCurrentWidget(menu);
        qDebug() << "Switched to menu:" << name;
    }
    else
    {
        qDebug() << "Menu not found:" << name;
    }
}

BaseMenu* MenuManager::getMenu(const QString& name) const
{
    return stacked->findChild<BaseMenu*>(name);
}

