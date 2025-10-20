#ifndef ADDOVERTIMEDIALOG_H
#define ADDOVERTIMEDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class AddOvertimeHoursDialog;
}

class AddOvertimeHoursDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddOvertimeHoursDialog(QWidget *parent = nullptr);
    ~AddOvertimeHoursDialog();
    
private slots:


private:
    Ui::AddOvertimeHoursDialog *ui;
};


#endif