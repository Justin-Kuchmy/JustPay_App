#include "include/Generated/ui_add_employee_dialog.h"
#include "UI/Add_Employee_Dialog.h"
#include <QWidget>
#include <QListWidgetItem>

AddEmployeeDialog::AddEmployeeDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddEmployeeDialog)
{
    ui->setupUi(this); 
}
AddEmployeeDialog::~AddEmployeeDialog()
{
    delete ui;
};
