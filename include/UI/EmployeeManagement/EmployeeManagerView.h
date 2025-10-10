#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include "../BaseView.h"

class EmployeeManagerView : public BaseView {
    Q_OBJECT

public:
    explicit EmployeeManagerView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
