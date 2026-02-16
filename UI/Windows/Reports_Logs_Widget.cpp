#include "include/Generated/ui_reports_logs_widget.h"
#include "UI/Reports_Logs_Widget.h"

ReportsLogsWidget::ReportsLogsWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::ReportsLogsWidget)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
}
ReportsLogsWidget::~ReportsLogsWidget()
{
}