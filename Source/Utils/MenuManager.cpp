

#include "Utils/MenuManager.h"
#include "Utils/DialogFactory.h"
#include <QApplication>


MenuManager::MenuManager(Parser& parser, QWidget* parent)
    : QWidget(parent), stacked(new QStackedWidget(this))
{
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(stacked);
    setLayout(layout);

    buildMenus(parser);

    if (auto* mainMenu = getMenu("main"))
        showMenu("main");
    else
        stacked->setCurrentIndex(0);
};


void MenuManager::buildMenus(Parser& parser)
{

    menuLookup.clear();

    // Create all menu widgets
    for (auto& [key, data] : parser.menuMap)
    {
        bool isRootMenu = (key == "main");
        auto* menuWidget = new BaseMenu(data, isRootMenu, stacked);
        menuWidget->setObjectName(QString::fromStdString(key));
        stacked->addWidget(menuWidget);
        menuLookup[QString::fromStdString(key)] = menuWidget;
    }

     // Connect navigation signals
    for (auto& [key, menu] : parser.menuMap)
    {
        for (const auto& opt : menu.options)
        {
            if (!opt.action.isEmpty()) {
            }
        } 
        auto* menuWidget = menuLookup[QString::fromStdString(key)];

        // Submenu navigation
        connect(menuWidget, &BaseMenu::submenuRequested, this,
                [this](const QString& submenuName) {
                    if (auto* submenu = getMenu(submenuName))
                    {
                        history.push(stacked->currentWidget()); 
                        stacked->setCurrentWidget(submenu);
                    }
                    else
                    {
                        qWarning() << "Submenu not found:" << submenuName;
                    }
                });

        connect(menuWidget,&BaseMenu::actionRequested,this,
            [this](const QString& actionName) {

                DialogFactory::registerDialogs();
                QWidget* w = DialogFactory::create(actionName);
                if (!w) return;

                if (auto dlg = qobject_cast<QDialog*>(w)) {
                    //QDialog
                    dlg->exec();
                    delete dlg;
                } else {
                    //QWidget
                    showContentWidget(w);

                    if (auto content = qobject_cast<BaseContentWidget*>(w)) 
                    {
                        connect(content, &BaseContentWidget::backRequested, this, [this]() {
                            if (!history.isEmpty()) {
                                QWidget* current = stacked->currentWidget();
                                QWidget* prev = history.pop();
                                stacked->setCurrentWidget(prev);
                                stacked->removeWidget(current);
                                current->deleteLater();
                            } else {
                                QApplication::quit();
                            }
                        });
                    }
                }
            });

        // Back navigation
        connect(menuWidget, &BaseMenu::backRequested, this, [this]() {
            if (!history.isEmpty()) {
                QWidget* current = stacked->currentWidget();
                QWidget* previous = history.pop();

                stacked->setCurrentWidget(previous);
            } else {
                QApplication::quit();
            }
        });
    }   


}

void MenuManager::showContentWidget(QWidget* widget)
{
    if (!widget)
            return;

        if (auto current = stacked->currentWidget()) 
        {
            history.push(current);
        }

        stacked->addWidget(widget);
        stacked->setCurrentWidget(widget);
};
void MenuManager::showMenu(const QString& name)
{
    auto* menu = getMenu(name);
    if (menu)
    {
        stacked->setCurrentWidget(menu);
    }
}

BaseMenu* MenuManager::getMenu(const QString& name) const
{
    return stacked->findChild<BaseMenu*>(name);
}

