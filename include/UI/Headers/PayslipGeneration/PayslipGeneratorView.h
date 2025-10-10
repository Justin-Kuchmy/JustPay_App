#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class PayslipGenerationView : public BaseView {
    Q_OBJECT

public:
    explicit PayslipGenerationView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
