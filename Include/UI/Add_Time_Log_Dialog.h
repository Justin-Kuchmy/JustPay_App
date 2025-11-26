#ifndef  ADDTIMELOGDIALOG_H
#define  ADDTIMELOGDIALOG_H

#include <QDialog> 
#include <QWidget> 

namespace Ui
{
    class AddTimeLogDialog;
}

class AddTimeLogDialog: public QDialog
{
    Q_OBJECT;
    public:
        AddTimeLogDialog(QWidget *parent = nullptr);
        ~AddTimeLogDialog();
private slots:
        void onOkayClicked();
        void onCancelClicked();
        void onSelectTypeClicked();
private:
    Ui::AddTimeLogDialog* ui;        
};
#endif