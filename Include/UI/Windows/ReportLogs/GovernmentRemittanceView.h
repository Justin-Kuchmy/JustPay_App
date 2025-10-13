#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class GovernmentRemittanceView : public BaseView {
    Q_OBJECT

public:
    explicit GovernmentRemittanceView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
