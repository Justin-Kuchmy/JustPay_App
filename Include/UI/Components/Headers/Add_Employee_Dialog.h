#ifndef ADDEMPLOYEEDIALOG_H
#define ADDEMPLOYEEDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class AddEmployeeDialog;
}

class AddEmployeeDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddEmployeeDialog(QWidget *parent = nullptr);
    ~AddEmployeeDialog();
    
private slots:


private:
    Ui::AddEmployeeDialog *ui;
};


#endif