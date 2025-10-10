#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class ReportLogsView : public BaseView {
    Q_OBJECT

public:
    explicit ReportLogsView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
