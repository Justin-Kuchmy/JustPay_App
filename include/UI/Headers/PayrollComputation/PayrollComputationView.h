#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class PayrollComputationView : public BaseView {
    Q_OBJECT

public:
    explicit PayrollComputationView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
