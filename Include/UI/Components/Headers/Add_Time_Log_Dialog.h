#ifndef ADDTIMELOGDIALOG_H
#define ADDTIMELOGDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class AddTimeLogDialog;
}

class AddTimeLogDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTimeLogDialog(QWidget *parent = nullptr);
    ~AddTimeLogDialog();
    
private slots:


private:
    Ui::AddTimeLogDialog *ui;
};


#endif