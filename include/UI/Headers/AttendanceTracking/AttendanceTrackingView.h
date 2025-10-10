#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class AttendanceTrackingView : public BaseView {
    Q_OBJECT

public:
    explicit AttendanceTrackingView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
