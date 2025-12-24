#ifndef ATTENDANCELISTWIDGET_H
#define ATTENDANCELISTWIDGET_H

#include <QWidget>
#include <vector>
#include <QSortFilterProxyModel>
#include "Models/DataObjects.h"
#include "Models/QObjects.h"
#include "Utils/BaseContentWidget.h"

namespace Ui
{
    class AttendanceListWidget;
}

class AttendanceListWidget: public BaseContentWidget {
    Q_OBJECT
public:
    AttendanceListWidget(QWidget *parent = nullptr);
    ~AttendanceListWidget();
    AttendanceListWidget(const AttendanceListWidget&) = delete; 
    AttendanceListWidget& operator=(const AttendanceListWidget&) = delete;

private slots:
    void onEmployeeSearchBoxChanged(const QString& text);
    void onReloadLogs();
    void onAddClicked();
    void onRowClicked(const QModelIndex &index);

private:
    Ui::AttendanceListWidget* ui;
    std::vector<AttendanceLog> attendanceLogs;
    AttendanceLogModel* model;
    QSortFilterProxyModel* proxyModel;

};

#endif