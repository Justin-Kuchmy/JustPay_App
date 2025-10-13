#include "../../../Include/MainWindow.h"
#include "../../../Include/MenuManager.h"
#include "../../../Include/Parser.h"
#include <QMenuBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("JustPay");
    resize(600, 800);

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
    
    // 🧱 Central widget for layout content
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    
    QVBoxLayout *layout = new QVBoxLayout(central);
    
    // 🧩 Create parser and menu manager
    Parser *parser = new Parser();
    MenuManager *menuManager = new MenuManager(*parser, this);
    
    layout->addWidget(menuManager);
    
    // Show initial menu (optional if handled in MenuManager constructor)
    menuManager->showMenu("main"); // change "main" to whatever your root menu is
    
    central->setLayout(layout);

    // Connect actions to functions or lambdas
    ///home/juku/Desktop/Code/c++/JustPay_Console/UI/resources/dark.qss
    ///home/juku/Desktop/Code/c++/JustPay_Console/UI
    connect(win98Theme, &QAction::triggered, this, [this]() {applyTheme("../UI/resources/win98.qss");});
    connect(darkTheme, &QAction::triggered, this, [this]() {applyTheme("../UI/resources/dark.qss");});
    connect(lightTheme, &QAction::triggered, this, [this]() {applyTheme("../UI/resources/light.qss");});
    
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