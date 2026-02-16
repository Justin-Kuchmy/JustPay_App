#ifndef REPORTSLOGSWIDGET_H
#define REPORTSLOGSWIDGET_H

#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"

namespace Ui
{
    class ReportsLogsWidget;
}

class ReportsLogsWidget : public BaseContentWidget
{
    Q_OBJECT

public:
    explicit ReportsLogsWidget(QWidget *parent = nullptr);
    ~ReportsLogsWidget();
    ReportsLogsWidget(const ReportsLogsWidget &) = delete;
    ReportsLogsWidget &operator=(const ReportsLogsWidget &) = delete;

private:
    Ui::ReportsLogsWidget *ui;
};

#endif