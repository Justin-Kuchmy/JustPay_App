#ifndef QOBJECTS_H
#define QOBJECTS_H

#include <QString>
#include <string>
#include <QVector>
#include <QDebug>
#include <QVariant>
#include <QModelIndex>
#include "Services/AppContext.h"
#include "./DataObjects.h"
#include "Services/EmployeeService.h"

struct MenuOption {
    QString label;
    QString action;
    QString submenu;
};

struct MenuData {
    QString title;
    QVector<MenuOption> options;
};


 class AttendanceLogModel : public QAbstractItemModel
 {
    Q_OBJECT
public: 
    AttendanceLogModel(QObject *parent): QAbstractItemModel(parent), m_model{}{};
    AttendanceLogModel(QObject* parent, std::vector<AttendanceLog>& logs): QAbstractItemModel(parent), m_model(&logs){}
    ~AttendanceLogModel() = default;
    AttendanceLogModel(const AttendanceLogModel&) = delete; 
    AttendanceLogModel& operator=(const AttendanceLogModel&) = delete;

    //QAbstractItemModel Methods
    QModelIndex index(int row, int column, const QModelIndex&) const {return createIndex(row, column);}
    QModelIndex parent(const QModelIndex&) const {return QModelIndex(); }
    int rowCount(const QModelIndex&) const {return m_model ? static_cast<qsizetype>(m_model->size()) : 0;}
    int columnCount(const QModelIndex&) const {return m_columnCount;}
    QVariant data(const QModelIndex& index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole) 
        {
            return valueForColumn(index.row(), index.column());
        }
        else if (role == Qt::UserRole) 
        {
            return m_model->at(index.row()).logId;
        }
        return QVariant();
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
                case 0: return QString("Employee ID");
                case 1: return QString("Employee Name");
                case 2: return QString("Date");
                case 3: return QString("Late (min)");
                case 4: return QString("Undertime (min)");
                case 5: return QString("Overtime (min)");
                case 6: return QString("Absent");
                case 7: return QString("Notes");
            }
        }
        return QVariant();
    }
    void reloadData(const std::vector<AttendanceLog>& newLogs)
    {
        beginResetModel();
        *m_model = newLogs;  // update the internal vector
        endResetModel();
    }

    

private:
    std::vector<AttendanceLog>* m_model;
    const int m_columnCount = 8;  
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        const auto& item = m_model->at(rowIndex);

        // Example: look up employee name
        QString empName = "";
        std::optional<Employee> emp = AppContext::instance().employeeService().getEmployeeByID(item.employeeId);
        if (emp.has_value()) empName = QString::fromStdString(emp->fullName);

        switch (columnIndex)
        {
            case 0: return QString::fromStdString(item.employeeId);
            case 1: return empName;
            case 2: return QString::fromStdString(item.logDate.to_string());
            case 3: return item.lateByMinute;
            case 4: return item.underTimeByMinute;
            case 5: return item.overTimeByMinute;
            case 6: return item.isAbsent;
            case 7: return QString::fromStdString(item.notes);
        }
        return QVariant();
    }
 };







#endif