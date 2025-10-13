#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class RoleManagerView : public BaseView {
    Q_OBJECT

public:
    explicit RoleManagerView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
