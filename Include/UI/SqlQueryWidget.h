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

namespace Ui {
class SqlQueryWidget;
}

class SqlQueryWidget : public QWidget {
    Q_OBJECT

public:
    explicit SqlQueryWidget(QWidget *parent = nullptr);
    ~SqlQueryWidget();
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
    sqlite3* m_db = nullptr;
    //QSqlDatabase database;
    //QSqlQueryModel *model;          // Holds the result set
    QString lastExecutedQuery;
    std::string sqlQuery;
    std::string description;

    // Query history:
    QListWidget* savedQueries = nullptr;  
    QStringList listItems;     // Optional: stored in file/settings
    QString historyFilePath;
    std::unordered_map<QString, QString> dict{};
    QStandardItemModel* model = nullptr;

    // UI state:
    bool autoResizeColumns = true;  // User preference
};


#endif