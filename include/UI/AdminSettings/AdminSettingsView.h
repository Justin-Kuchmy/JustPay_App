#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class AdminSettingsView: public BaseView {
    Q_OBJECT

public:
    explicit AdminSettingsView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};
