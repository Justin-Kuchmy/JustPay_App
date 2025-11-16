#ifndef QOBJECTS_H
#define QOBJECTS_H

#include <QString>
#include <QVector>
#include <QDebug>
#include "./DataObjects.h"

struct MenuOption {
    QString label;
    QString action;
    QString submenu;
};

struct MenuData {
    QString title;
    QVector<MenuOption> options;
};


#endif