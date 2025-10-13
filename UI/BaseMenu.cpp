#include "../Include/BaseMenu.h"


BaseMenu::BaseMenu(const MenuData& data, QWidget* parent)
    : QWidget(parent)
{
    
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    
    auto* container = new QWidget(this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto* layout = new QVBoxLayout(container);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* titleLabel = new QLabel(data.title);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    
    for (const auto& opt : data.options)
    {
        auto* btn = new QPushButton(opt.label);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [this, opt]() {
            qDebug() << "Clicked action:" << opt.action
                     << "| Submenu:" << opt.submenu;

            if (!opt.submenu.isEmpty())
                emit submenuRequested(opt.submenu);
        });
    }

    // Back button
    auto* backButton = new QPushButton("Back", this);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    backButton->setStyleSheet("width: 90%; margin: 0 auto;");
    layout->addSpacing(10);
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit backRequested();
    });

    
    outerLayout->addStretch(1);
    outerLayout->addWidget(container, 9);  
    outerLayout->addStretch(1);
}