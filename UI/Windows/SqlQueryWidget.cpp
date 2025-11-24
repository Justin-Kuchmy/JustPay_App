#include "include/Generated/ui_sql_module.h"
#include "UI/SqlQueryWidget.h"

SqlQueryWidget::SqlQueryWidget(QWidget *parent): QWidget(parent), ui(new Ui::SqlQueryWidget)
{
    ui->setupUi(this); 
}

SqlQueryWidget::~SqlQueryWidget()
{
    delete ui;
}

void SqlQueryWidget::onExecuteClicked()
{

}

void SqlQueryWidget::onClearClicked()
{

}

void SqlQueryWidget::onSaveQueryClicked()
{

}

void SqlQueryWidget::onQueryDoubleClicked(const QModelIndex &)
{

}

void SqlQueryWidget::onExportCSVClicked()
{

}

void SqlQueryWidget::runQuery(const QString &sql)
{

}

void SqlQueryWidget::showQueryError(const QString &msg)
{

}

void SqlQueryWidget::updateTable()
{

}

void SqlQueryWidget::loadColumnWidths()
{

}

void SqlQueryWidget::saveColumnWidths()
{

}
