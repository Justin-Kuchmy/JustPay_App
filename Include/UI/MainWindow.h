#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <sqlite3.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void applyTheme(const QString &path);
    bool backupDatabase(sqlite3 *m_db, const std::string &backupPath);
private slots:
    void onBackupDatabase();

private:
};

#endif // MainWindow_H
