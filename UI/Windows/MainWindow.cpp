#include "UI/MainWindow.h"
#include "Utils/MenuManager.h"
#include "Utils/Parser.h"
#include <QMenuBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QFile>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setWindowTitle("JustPay");
    resize(1200, 675);

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
    
    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu("&Tools");
    
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

    connect(win98Theme, &QAction::triggered, this, [this]() {applyTheme("../Resources/styles/win98.qss");});
    connect(darkTheme, &QAction::triggered, this, [this]() {applyTheme("../Resources/styles/dark.qss");});
    connect(lightTheme, &QAction::triggered, this, [this]() {applyTheme("../Resources/styles/light.qss");});
    
}

void MainWindow::applyTheme(const QString &path) {
    QFile file(path);
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromUtf8(file.readAll());
        qApp->setStyleSheet(styleSheet);
        qDebug() << "Applied theme:" << path;
    } else {
        qDebug() << "Failed to open theme file:" << path;
    }
}


MainWindow::~MainWindow() {}