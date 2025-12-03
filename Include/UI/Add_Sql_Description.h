#ifndef SQLDESCRIPTIONDIALOG_H
#define SQLDESCRIPTIONDIALOG_H 
#include <QDialog>
#include <QMessageBox>

namespace Ui {
    class AddSqlDescriptionDialog;
}

class AddSqlDescriptionDialog: public QDialog {
Q_OBJECT

public: 
    explicit AddSqlDescriptionDialog(QWidget *parent = nullptr);
    ~AddSqlDescriptionDialog();
    std::string getSqlDescription();


private slots: 
    void onSaveClicked();
    void onCancelClicked();

private:
    Ui::AddSqlDescriptionDialog *ui;
    std::string sqlDescription;

};

#endif