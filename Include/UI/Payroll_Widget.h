#ifndef PAYROLLWIDGET_H
#define PAYROLLWIDGET_H

#include <QDialog>
#include <QVariant>
#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QListWidget>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <sqlite3.h>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"


namespace Ui{
class PayrollWidget;
}

class PayrollWidget : public BaseContentWidget {
    Q_OBJECT

public:
    explicit PayrollWidget(QWidget *parent = nullptr);
    ~PayrollWidget();                

private slots:
                    
private:

private:
    Ui::PayrollWidget *ui;
};

#endif