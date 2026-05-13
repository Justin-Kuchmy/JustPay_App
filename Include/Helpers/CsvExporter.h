#pragma once
#include <QStandardItemModel>
#include <QWidget>

namespace CsvExporter
{
    void exportModel(QAbstractItemModel *model, QWidget *parent);
}