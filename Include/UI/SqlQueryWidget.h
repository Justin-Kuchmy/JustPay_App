#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <QDialog>
#include <QVariant>
#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QListWidget>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <sqlite3.h>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"

namespace Ui {
class SqlQueryWidget;
}

class SqlQueryWidget : public BaseContentWidget {
    Q_OBJECT

public:
    explicit SqlQueryWidget(QWidget *parent = nullptr);
    ~SqlQueryWidget();
    SqlQueryWidget(const SqlQueryWidget&) = delete; 
    SqlQueryWidget& operator=(const SqlQueryWidget&) = delete;

    void setDatabase(const QSqlDatabase &db);         
    void loadSavedQueries();                         
    QString currentQuery() const;                    

private slots:
    void onExecuteClicked();                         
    void onClearClicked();                           
    void onSaveQueryClicked();                       
    void onQueryDoubleClicked(const QModelIndex &);  
    void onExportCSVClicked();                       

private:
    QVariant mapColumn(sqlite3_stmt* stmt, int col);
    void LoadListItems();

private:
    Ui::SqlQueryWidget *ui;

    // SQL & Model:
    sqlite3* m_db;
    QString lastExecutedQuery;
    std::string sqlQuery;
    std::string description;

    // Query history:
    std::unique_ptr<QListWidget> savedQueries;
     
    QStringList listItems;    
    QString historyFilePath;
    std::unordered_map<QString, QString> dict{};
    std::unique_ptr<QStandardItemModel> model;

    //QListWidget* savedQueries = nullptr;
    //QStandardItemModel* model = nullptr;

    // UI state:
    bool autoResizeColumns = true; 
};


#endif