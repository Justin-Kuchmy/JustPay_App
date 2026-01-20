#include "Utils/BaseMenu.h"

BaseMenu::BaseMenu(const MenuData &data, bool isRootMenu, QWidget *parent) : QWidget(parent)
{

    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);
    outerLayout->setAlignment(Qt::AlignCenter);

    auto *container = new QWidget(this);
    container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    auto *layout = new QVBoxLayout(container);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *titleLabel = new QLabel(data.title);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    for (const auto &opt : data.options)
    {
        auto *btn = new QPushButton(opt.label);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn->setStyleSheet(R"(
            QPushButton {
                border-radius: 6px;         
                border: 1px solid #808080;
                padding: 6px 12px;   
                max-width: 60em;        
            }
            QPushButton:hover {
                border-width: 2px;
                border-color: #808080;
            }
            QPushButton:pressed {
                border-style: inset;      
            }
        )");
        auto *hLayout = new QHBoxLayout();
        hLayout->addStretch();
        hLayout->addWidget(btn);
        hLayout->addStretch();

        layout->addLayout(hLayout);

        connect(btn, &QPushButton::clicked, this, [this, opt]()
                {
                    if (!opt.submenu.isEmpty())
                        emit submenuRequested(opt.submenu);
                    if (!opt.action.isEmpty())
                        emit actionRequested(opt.action); });
    }

    // Back button
    QPushButton *backButton = nullptr;
    if (isRootMenu)
    {
        backButton = new QPushButton("Exit", this);
    }
    else
    {
        backButton = new QPushButton("Back", this);
    }

    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    backButton->setStyleSheet(R"(
        QPushButton {
                border-radius: 6px;         
                border: 1px solid #808080;
                padding: 6px 12px;  
                min-width: 60em;
                    
            }
            QPushButton:hover {
                border-width: 2px;
                border-color: #808080;
            }
            QPushButton:pressed {
                border-style: inset;      
            }
        )");
    layout->addSpacing(10);
    auto *hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(backButton);
    hLayout->addStretch();

    layout->addLayout(hLayout);
    // layout->addWidget(backButton);
    connect(backButton, &QPushButton::clicked, this, [this]()
            { emit backRequested(); });

    outerLayout->addStretch(1);
    outerLayout->addWidget(container, 9);
    outerLayout->addStretch(1);
}