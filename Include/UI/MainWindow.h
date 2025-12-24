#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void applyTheme(const QString &path);
    
private slots:

private:

};

#endif // MainWindow_H
