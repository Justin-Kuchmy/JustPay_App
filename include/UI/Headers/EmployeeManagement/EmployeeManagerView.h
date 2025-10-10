#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include "../Baseview.h"

class EmployeeModuleView : public Baseview {
    Q_OBJECT

public:
    explicit EmployeeModuleView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
