#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class ContributionTablesView : public BaseView {
    Q_OBJECT

public:
    explicit ContributionTablesView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
