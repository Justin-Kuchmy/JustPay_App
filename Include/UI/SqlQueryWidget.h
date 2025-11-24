#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
class SqlQueryWidget;
}

class SqlQueryWidget : public QWidget {
    Q_OBJECT

public:
    explicit SqlQueryWidget(QWidget *parent = nullptr);
    ~SqlQueryWidget();
   void setDatabase(const QSqlDatabase &db);        // Pass database connection
    void loadSavedQueries();                         // Load previous queries (optional)
    QString currentQuery() const;                    // Get the SQL currently typed

private slots:
    void onExecuteClicked();                         // Runs SQL query
    void onClearClicked();                           // Clears text box
    void onSaveQueryClicked();                       // Saves query to a list
    void onQueryDoubleClicked(const QModelIndex &);  // Load query from history
    void onExportCSVClicked();                       // Export table result

private:
    void runQuery(const QString &sql);               // Internal helper
    void showQueryError(const QString &msg);         // Error popup
    void updateTable();                              // Refresh table view columns
    void loadColumnWidths();                         // Optional: remember table width prefs
    void saveColumnWidths();

private:
    Ui::SqlQueryWidget *ui;

    // SQL & Model:
    QSqlDatabase database;
    QSqlQueryModel *model;          // Holds the result set
    QString lastExecutedQuery;

    // Query history:
    QStringList savedQueries;       // Optional: stored in file/settings
    QString historyFilePath;

    // UI state:
    bool autoResizeColumns = true;  // User preference
};


#endif