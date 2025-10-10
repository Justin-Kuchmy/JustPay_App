#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class PayrollFormulaView : public BaseView {
    Q_OBJECT

public:
    explicit PayrollFormulaView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
