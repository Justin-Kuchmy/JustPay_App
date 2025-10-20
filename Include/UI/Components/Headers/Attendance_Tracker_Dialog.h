#ifndef ATTENFANCETRACKERDIALOG_H
#define ATTENFANCETRACKERDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class AttendanceTrackerDialog;
}

class AttendanceTrackerDialog : public QDialog {
    Q_OBJECT

public:
    explicit AttendanceTrackerDialog(QWidget *parent = nullptr);
    ~AttendanceTrackerDialog();
    
private slots:


private:
    Ui::AttendanceTrackerDialog *ui;
};


#endif