// #include "Generated/ui_employee_select_dialog.h"
// #include "UI/Employee_Select_Dialog.h"
// #include <QWidget>
// #include <QListWidgetItem>

// EmployeeSelectDialog::EmployeeSelectDialog(QWidget *parent): QDialog(parent), ui(new Ui::EmployeeSelectDialog)
// {
//     ui->setupUi(this); 
//     connect(ui->employeeList, &QListWidget::itemClicked,
//             this, &EmployeeSelectDialog::onEmployeeSelected);
// }

// void EmployeeSelectDialog::populateEmployeeList() {
//    ui->employeeList->clear();

//     if (!empList) return;
//     for (const auto& emp : *empList) { 
//         ui->employeeList->addItem(QString::fromStdString(emp.fullName));
//     }
// }

// EmployeeSelectDialog::~EmployeeSelectDialog()
// {
//     delete ui;
// }

// QString EmployeeSelectDialog::getSelectedEmployee() const
// {
//     return selectedEmployee;
// }
// void EmployeeSelectDialog::on_buttonBox_accepted()
// {
//      if (ui->employeeList->currentItem())
//         selectedEmployee = ui->employeeList->currentItem()->text();
// }

// void EmployeeSelectDialog::onEmployeeSelected(QListWidgetItem* item)
// {
//     if (!item || !empList) return;

//     QString selectedName = item->text();

//     for (const auto& emp : *empList) {
//         if (QString::fromStdString(emp.fullName) == selectedName) {
//             selectedEmployee = selectedName;
//             qDebug() << "Selected employee:" << selectedEmployee;
//             break;
//         }
//     }
// }
