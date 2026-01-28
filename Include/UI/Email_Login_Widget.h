#ifndef EMAILLOGINWIDGET_H
#define EMAILLOGINWIDGET_H

#include "../Include/Models/DataObjects.h"
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

namespace Ui
{
    class EmailLoginWidget;
}

class EmailLoginWidget : public QWidget
{
    Q_OBJECT

public:
    EmailLoginWidget(QWidget *parent = nullptr);
    ~EmailLoginWidget();
    EmailLoginWidget(const EmailLoginWidget &) = delete;
    EmailLoginWidget &operator=(const EmailLoginWidget &) = delete;
private slots:
    void onLoginButtonClicked();
    void onPassCheckBoxClicked();

private:
    Ui::EmailLoginWidget *ui;
};

#endif