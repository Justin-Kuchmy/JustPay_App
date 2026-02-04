#ifndef EMAILLOGINWIDGET_H
#define EMAILLOGINWIDGET_H

#include "../Include/Models/DataObjects.h"
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include "Utils/BaseContentWidget.h"
#include "Utils/AuthService.h"

namespace Ui
{
    class EmailLoginWidget;
}

class EmailLoginWidget : public BaseContentWidget
{
    Q_OBJECT

public:
    EmailLoginWidget(QWidget *parent = nullptr);
    ~EmailLoginWidget();
    EmailLoginWidget(const EmailLoginWidget &) = delete;
    EmailLoginWidget &operator=(const EmailLoginWidget &) = delete;
private slots:
    void onSaveButtonClicked();
    void onPassCheckBoxClicked();

private:
    Ui::EmailLoginWidget *ui;
    EmailCrudentials emailCrudentials;
};

#endif