#ifndef EDITDEPENDENTDETAILS_H
#define EDITDEPENDENTDETAILS_H

#include <QDialog>
#include "../Include/Models/DataObjects.h"
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class EditDependentDetails;
}

class EditDependentDetails : public QWidget {
    Q_OBJECT
public:
    explicit EditDependentDetails(QWidget *parent = nullptr);
    ~EditDependentDetails();
    void setDependentContext(std::optional<Dependent> &dependent);
private slots:
    void onSaveClicked();
private:   
    Ui::EditDependentDetails *ui;
    std::unique_ptr<Dependent> m_Dependent;
    
};

#endif