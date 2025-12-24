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
    AddOvertimeHoursDialog(const AddOvertimeHoursDialog&) = delete; 
    AddOvertimeHoursDialog& operator=(const AddOvertimeHoursDialog&) = delete;
    std::string getJsonString();
private slots:
    void onOkayClicked();
    void onCancelClicked();
private:
    Ui::AddOvertimeHoursDialog* ui;
    std::string jsonString;
};

#endif