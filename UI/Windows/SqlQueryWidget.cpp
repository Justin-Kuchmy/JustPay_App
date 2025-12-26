#include "include/Generated/ui_sql_module.h"
#include "UI/SqlQueryWidget.h"
#include <QApplication>
#include "UI/Add_Sql_Description.h"
#include "Utils/DialogFactory.h"

SqlQueryWidget::SqlQueryWidget(QWidget *parent): BaseContentWidget(parent), 
ui(new Ui::SqlQueryWidget),
m_db(nullptr),
lastExecutedQuery{},
sqlQuery{},
description{},
savedQueries{},
listItems{},
historyFilePath{},
dict{},
model{}
{
    ui->setupUi(this); 
    ui->sqlSplitter->setSizes({300, 900});

    QString exeDir = QCoreApplication::applicationDirPath();
    QString dbPath = QDir(exeDir).filePath("../Resources/" + QString::fromStdString("payroll.db"));
    const char* cpath = dbPath.toStdString().c_str();

    if (sqlite3_open(cpath, &m_db) != SQLITE_OK)
        throw std::runtime_error("Failed to open database");

    QFile historyPath(":/resources/history.txt");
    if (!historyPath.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to open file for writing");
        return;
    }
    else
    {
        QString lineFromFile = "";
        QTextStream historyIn(&historyPath);
        this->listItems.clear();
        QString key   = QString::fromStdString("");
        std::string v = "";


        while (!historyIn.atEnd())
        {
            lineFromFile = historyIn.readLine();
            if(lineFromFile != "--END--")
            {
                qsizetype index = lineFromFile.indexOf('|');
                if(index != -1)
                {
                    key = lineFromFile.left(index);
                }
                v += lineFromFile.mid(index + 1).toStdString()+ " ";    
            }
            else
            {
                this->dict[key] = QString::fromStdString(v);
                v = "";
            }
        }
        
        this->listItems.append(key);
        LoadListItems();
        

    }
    

    connect(ui->btnRun, &QPushButton::clicked, this, &SqlQueryWidget::onExecuteClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &SqlQueryWidget::onClearClicked);
    connect(ui->btnSave, &QPushButton::clicked, this, &SqlQueryWidget::onSaveQueryClicked);
    connect(ui->btnExport, &QPushButton::clicked, this, &SqlQueryWidget::onExportCSVClicked);
    connect(ui->historyList, &QListWidget::clicked, this, &SqlQueryWidget::onQueryDoubleClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    
}

SqlQueryWidget::~SqlQueryWidget()
{
    delete ui;
}

void SqlQueryWidget::onExecuteClicked()
{
    sqlQuery = ui->textSql->toPlainText().toStdString();
    sqlite3_stmt* stmt = nullptr;
    if(sqlite3_prepare_v2(m_db, sqlQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        ui->textErrors->setPlainText("Failed to prepare: " + QString::fromUtf8(sqlite3_errmsg(m_db)));
    }
    else
    {

        int colCount = sqlite3_column_count(stmt);

        QStringList columnNames;
        for (int i = 0; i < colCount; i++) {
            columnNames << QString::fromUtf8(sqlite3_column_name(stmt, i));
        }
        QVector<QVector<QVariant>> rows;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            QVector<QVariant> row;
            for (int i = 0; i < colCount; i++) {
                row.append(mapColumn(stmt, i));
            }
            rows.append(row);
        }
        sqlite3_finalize(stmt);

        model = std::make_unique<QStandardItemModel>(this);
        model->setHorizontalHeaderLabels(columnNames);

        for (const auto& row : rows) {
            QList<QStandardItem*> items;

            for (const QVariant& value : row) {
                QStandardItem* item = new QStandardItem();

                if (value.isNull()) {
                    item->setText("NULL");
                    item->setForeground(Qt::gray);
                } else if (value.typeId() == QMetaType::QByteArray) {
                    item->setText("<BLOB>");
                    item->setForeground(Qt::darkMagenta);
                } else {
                    item->setText(value.toString());
                }

                items.append(item);
            }

            model->appendRow(items);
        }

        ui->tableResults->setModel(model.get());
        ui->tableResults->resizeColumnsToContents();
        ui->tableResults->resizeRowsToContents();
        
    }
}

QVariant SqlQueryWidget::mapColumn(sqlite3_stmt* stmt, int col) {
    int type = sqlite3_column_type(stmt, col);

    switch (type) {
    case SQLITE_INTEGER:
        return QVariant(sqlite3_column_int64(stmt, col));

    case SQLITE_FLOAT:
        return QVariant(sqlite3_column_double(stmt, col));

    case SQLITE_TEXT:
        return QVariant(QString::fromUtf8(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, col))
        ));

    case SQLITE_BLOB: {
        const void* blob = sqlite3_column_blob(stmt, col);
        int size = sqlite3_column_bytes(stmt, col);
        return QVariant(QByteArray(static_cast<const char*>(blob), size));
    }

    case SQLITE_NULL:
    default:
        return QVariant(); 
    }
}

void SqlQueryWidget::onClearClicked()
{

    ui->textSql->clear();
    ui->tableResults->setModel(nullptr); 
    ui->textErrors->clear();
}

void SqlQueryWidget::onSaveQueryClicked()
{

    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("add_sql_description");
    auto sqlDialog = dynamic_cast<AddSqlDescriptionDialog*>(dlg);


    std::string historyPath = "../Resources/history.txt";
    QFile historyfile(QString::fromStdString(historyPath));
    if (!historyfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to open file for writing");
        return;
    }
    QTextStream historyOut(&historyfile);


    if(sqlDialog && sqlDialog->exec() == QDialog::Accepted)
    {
        onExecuteClicked();
        this->description = sqlDialog->getSqlDescription();
        QString qDesc = QString::fromStdString(this->description);
        QString qSql  = QString::fromStdString(this->sqlQuery);
        this->listItems.append(qDesc);
        this->dict[qDesc] = qSql;
        LoadListItems();
        
    }  
    for(const auto& item : dict)
    {
        historyOut << item.first + "|" << item.second + "\n--END--\n";
    }
    historyfile.close();
}

void SqlQueryWidget::LoadListItems()
{
    ui->historyList->clear();
    ui->historyList->addItems(this->listItems);
 
}

void SqlQueryWidget::onQueryDoubleClicked(const QModelIndex &i)
{
    QListWidgetItem* item = ui->historyList->itemFromIndex(i);
    ui->textSql->setText(this->dict[item->text()]);
}

void SqlQueryWidget::onExportCSVClicked()
{
    if (!this->model)
        return;
    QString fileName = QFileDialog::getSaveFileName(
            this,
            "Save CSV",
            "",
            "CSV files (*.csv);;All files (*)"
        );
    if (fileName.isEmpty())
        return;
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive))
    {
        fileName += ".csv";
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to open file for writing");
        return;
    }

    QTextStream out(&file);
    int columns = this->model->columnCount();
    int rows = this->model->rowCount();

    for (int j = 0; j < columns; ++j)
    {
        QString header = this->model->headerData(j, Qt::Horizontal).toString();
        out << "\"" << header << "\"";
        if (j < columns - 1) out << ",";
    }
    out << "\n";
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            QStandardItem * item = this->model->item(i, j);
            QString text = item ? item->text() : "";
            out << "\"" << text << "\""; 
            if (j < columns - 1) out << ",";
        }
        out << "\n";
    }
    file.close();
    QMessageBox::information(this, "Export CSV", "CSV file saved successfully!");
} 
