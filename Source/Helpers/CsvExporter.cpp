#include "Helpers/CsvExporter.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

void CsvExporter::exportModel(QAbstractItemModel *model, QWidget *parent)
{
    if (!model)
        return;

    QString fileName = QFileDialog::getSaveFileName(
        parent,
        "Save CSV",
        "",
        "CSV files (*.csv);;All files (*)");

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".csv", Qt::CaseInsensitive))
        fileName += ".csv";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(parent, "Error", "Failed to open file for writing");
        return;
    }

    QTextStream out(&file);
    int columns = model->columnCount();
    int rows = model->rowCount();

    for (int j = 0; j < columns; ++j)
    {
        out << "\"" << model->headerData(j, Qt::Horizontal).toString() << "\"";
        if (j < columns - 1)
            out << ",";
    }
    out << "\n";

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            QString text = model->data(model->index(i, j)).toString();
            // QStandardItem *item = model->item(i, j);
            out << "\"" << text << "\"";
            if (j < columns - 1)
                out << ",";
        }
        out << "\n";
    }

    file.close();
    QMessageBox::information(parent, "Export CSV", "CSV file saved successfully!");
}