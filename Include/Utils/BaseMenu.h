#ifndef BASEMENU_HPP 
#define BASEMENU_HPP

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QVector>
#include <unordered_map>
#include <string>
#include "./Models/DataObjects.h"



class BaseMenu : public QWidget
{
    Q_OBJECT
public:
    explicit BaseMenu(const MenuData& data, bool isRootMenu,  QWidget* parent = nullptr);

signals:
    void submenuRequested(const QString& submenuName);
    void backRequested();
};

#endif
