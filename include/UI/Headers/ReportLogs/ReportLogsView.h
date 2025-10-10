#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class ReportLogsModuleView : public BaseView {
    Q_OBJECT

public:
    explicit ReportLogsModuleView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
