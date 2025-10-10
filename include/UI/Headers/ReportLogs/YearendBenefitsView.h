#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class YearEndBenefitsView : public BaseView {
    Q_OBJECT

public:
    explicit YearEndBenefitsView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
