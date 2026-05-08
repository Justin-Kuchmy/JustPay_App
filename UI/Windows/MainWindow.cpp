#include "UI/MainWindow.h"
#include "Utils/MenuManager.h"
#include "Utils/Parser.h"
#include <QMenuBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <iostream>
#include "Utils/Log.h"
#include <QMessageBox>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("JustPay");
    resize(1200, 700);

    // Create menu bar
    QMenuBar *menuBar = new QMenuBar(this);

    // File menu
    QMenu *fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("New");
    fileMenu->addAction("Open");
    fileMenu->addAction("Exit");

    // Edit menu
    QMenu *editMenu = menuBar->addMenu("&Edit");
    editMenu->addAction("Cut");
    editMenu->addAction("Copy");
    editMenu->addAction("Paste");

    // View menu
    QMenu *viewMenu = menuBar->addMenu("&View");
    QMenu *themeMenu = viewMenu->addMenu("&Themes");

    QAction *win98Theme = themeMenu->addAction("Win98");
    QAction *darkTheme = themeMenu->addAction("Dark");
    QAction *lightTheme = themeMenu->addAction("Light");
    QAction *win7Theme = themeMenu->addAction("win7");
    QAction *winxpTheme = themeMenu->addAction("winxp");

    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu("&Tools");
    toolsMenu->addAction("Settings");
    QAction *backupAction = toolsMenu->addAction("Backup Database");
    connect(backupAction, &QAction::triggered, this, &MainWindow::onBackupDatabase);

    // Help menu
    QMenu *helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction("About");

    setMenuBar(menuBar);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);

    Parser *parser = new Parser();
    MenuManager *menuManager = new MenuManager(*parser, this);

    layout->addWidget(menuManager);

    menuManager->showMenu("main");

    central->setLayout(layout);

    connect(win98Theme, &QAction::triggered, this, [this]()
            { applyTheme(":/QtTheme/theme/win98.qss"); });

    connect(darkTheme, &QAction::triggered, this, [this]()
            { applyTheme(":/QtTheme/theme/dark.qss"); });

    connect(lightTheme, &QAction::triggered, this, [this]()
            { applyTheme(":/QtTheme/theme/light.qss"); });

    connect(win7Theme, &QAction::triggered, this, [this]()
            { applyTheme(":/QtTheme/theme/win7.qss"); });

    connect(winxpTheme, &QAction::triggered, this, [this]()
            { applyTheme(":/QtTheme/theme/winxp.qss"); });
}

void MainWindow::applyTheme(const QString &path)
{
    QFile file(path);
    if (!file.exists())
    {
        std::cerr << "Theme file does not exist: " << path.toStdString() << "\n";
        return;
    }

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QString styleSheet = QString::fromUtf8(file.readAll());
        qApp->setStyleSheet(styleSheet);
        std::cout << "Applied theme: " << path.toStdString() << "\n";
    }
    else
    {
        std::cerr << "Failed to open theme file: " << path.toStdString() << "\n";
    }
}

// https://sqlite.org/c3ref/backup_finish.html
bool MainWindow::backupDatabase(sqlite3 *srcDb, const std::string &destPath)
{
    sqlite3 *destDb = nullptr;

    if (sqlite3_open(destPath.c_str(), &destDb) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to open backup destination: " << sqlite3_errmsg(destDb));
        sqlite3_close(destDb);
        return false;
    }

    sqlite3_backup *backup = sqlite3_backup_init(destDb, "main", srcDb, "main");
    if (!backup)
    {
        LOG_DEBUG("Failed to init backup: " << sqlite3_errmsg(destDb));
        sqlite3_close(destDb);
        return false;
    }

    sqlite3_backup_step(backup, -1); // -1 = copy entire DB in one step
    sqlite3_backup_finish(backup);

    int rc = sqlite3_errcode(destDb);
    sqlite3_close(destDb);

    return rc == SQLITE_OK;
}

void MainWindow::onBackupDatabase()
{
    QString backupPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/JustPay/Backups/JustPay_backup_%1.db").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));
    const std::string &backupPathString = backupPath.toStdString();
    QDir().mkpath(QFileInfo(backupPath).absolutePath());
    LOG_DEBUG(backupPath.toStdString());
    bool ok = backupDatabase(AppContext::instance().getDb(), backupPathString);
    if (ok)
        QMessageBox::information(this, "Backup", "Database backed up to:\n" + backupPath);
    else
        QMessageBox::critical(this, "Backup Failed", "Could not create backup.");
}

MainWindow::~MainWindow() {}