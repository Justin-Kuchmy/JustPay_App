#ifndef ATTENFANCETRACKERITEM_H
#define ATTENFANCETRACKERITEM_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class AttendanceTrackerItem;
}

class AttendanceTrackerItem : public QDialog {
    Q_OBJECT

public:
    explicit AttendanceTrackerItem(QWidget *parent = nullptr);
    ~AttendanceTrackerItem();
    
private slots:


private:
    Ui::AttendanceTrackerItem *ui;
};


#endif