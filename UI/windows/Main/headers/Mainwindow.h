#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void applyTheme(const QString &path);
    
private slots:

private:
    QStackedWidget *stackedWidget;

};

#endif // MAINWINDOW_H
