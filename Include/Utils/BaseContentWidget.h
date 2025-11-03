#ifndef BASECONTENTWIDGET_HPP 
#define BASECONTENTWIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QVector>
#include <unordered_map>
#include <string>
#include "./Models/QObjects.h"


class BaseContentWidget : public QWidget {
    Q_OBJECT
public:
    explicit BaseContentWidget(QWidget* parent = nullptr);
    virtual ~BaseContentWidget();
signals:
    void backRequested();
};

#endif
