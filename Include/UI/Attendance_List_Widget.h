#ifndef ATTENDANCELISTWIDGET_H
#define ATTENDANCELISTWIDGET_H

#include <QWidget>

namespace Ui
{
    class AttendanceListWidget;
}

class AttendanceListWidget: public QWidget
{
    Q_OBJECT
public:
    AttendanceListWidget(QWidget *parent = nullptr);
    ~AttendanceListWidget();
private slots:
    void onReloadClicked();
    void onAddClicked();

private:
    Ui::AttendanceListWidget* ui;
};

#endif