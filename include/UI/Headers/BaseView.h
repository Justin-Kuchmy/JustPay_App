#pragma once
#include <QWidget>
#include <QList>
#include <QString>

class BaseView: public QWidget {
    Q_OBJECT
public:
    using QWidget::QWidget; // inherit constructors

    virtual QList<QString> errors() const { return {}; }
};