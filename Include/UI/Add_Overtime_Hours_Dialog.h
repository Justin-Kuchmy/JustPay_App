#ifndef  ADDOVERTIMEHOURSDIALOG_H
#define  ADDOVERTIMEHOURSDIALOG_H


#include <QDialog>
#include <QWidget> 

namespace Ui
{
    class AddOvertimeHoursDialog;
}

class AddOvertimeHoursDialog: public QDialog
{
    Q_OBJECT;
public:
    AddOvertimeHoursDialog(QWidget *parent = nullptr);
    ~AddOvertimeHoursDialog();
private slots:
    void onOkayClicked();
    void onCancelClicked();
private:
    Ui::AddOvertimeHoursDialog* ui;
};

#endif