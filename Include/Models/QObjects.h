#ifndef QOBJECTS_H
#define QOBJECTS_H

#include <QString>
#include <string>
#include <QVector>
#include <QDebug>
#include <QVariant>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include "Services/AppContext.h"
#include "./DataObjects.h"
#include "Services/EmployeeService.h"

struct MenuOption
{
    QString label{};
    QString action{};
    QString submenu{};
};

struct MenuData
{
    QString title{};
    QVector<MenuOption> options{};
};

class AttendanceLogModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    AttendanceLogModel(QObject *parent) : QAbstractItemModel(parent), m_model{} {};
    AttendanceLogModel(QObject *parent, std::vector<AttendanceLog> &logs) : QAbstractItemModel(parent), m_model(&logs) {}
    ~AttendanceLogModel() = default;
    AttendanceLogModel(const AttendanceLogModel &) = delete;
    AttendanceLogModel &operator=(const AttendanceLogModel &) = delete;

    // QAbstractItemModel Methods
    QModelIndex index(int row, int column, const QModelIndex &) const { return createIndex(row, column); }
    QModelIndex parent(const QModelIndex & = QModelIndex()) const override { return QModelIndex(); }
    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_model ? static_cast<int>(m_model->size()) : 0; }
    int columnCount(const QModelIndex & = QModelIndex()) const override { return m_columnCount; }
    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            return valueForColumn(static_cast<size_t>(index.row()), static_cast<size_t>(index.column()));
        }
        else if (role == Qt::UserRole)
        {
            return m_model->at(static_cast<size_t>(index.row())).logId;
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
            case 0:
                return QString("Employee ID");
            case 1:
                return QString("Employee Name");
            case 2:
                return QString("Date");
            case 3:
                return QString("Late (min)");
            case 4:
                return QString("Undertime (min)");
            case 5:
                return QString("Overtime (min)");
            case 6:
                return QString("Absent");
            case 7:
                return QString("Notes");
            }
        }
        return QVariant();
    }
    void reloadData(const std::vector<AttendanceLog> &newLogs)
    {
        beginResetModel();
        *m_model = newLogs;
        endResetModel();
    }

private:
    std::vector<AttendanceLog> *m_model;
    const int m_columnCount = 8;
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        const auto &item = m_model->at(rowIndex);

        QString empName = "";
        std::optional<Employee> emp = AppContext::instance().employeeService().getEmployeeByID(item.employeeId);
        if (emp.has_value())
            empName = QString::fromStdString(emp->fullName);

        switch (columnIndex)
        {
        case 0:
            return QString::fromStdString(item.employeeId);
        case 1:
            return empName;
        case 2:
            return QString::fromStdString(item.logDate.to_string());
        case 3:
            return item.lateByMinute;
        case 4:
            return item.underTimeByMinute;
        case 5:
            return item.overTimeByMinute;
        case 6:
            return item.isAbsent;
        case 7:
            return QString::fromStdString(item.notes);
        }
        return QVariant();
    }
};

class PayrollRegisterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PayrollRegisterModel(QObject *parent) : QAbstractTableModel(parent), m_model{} {};
    PayrollRegisterModel(QObject *parent, std::vector<PayrollCalculationResults> *payroll) : QAbstractTableModel(parent), m_model(payroll) {}
    ~PayrollRegisterModel() = default;
    PayrollRegisterModel(const PayrollRegisterModel &) = delete;
    PayrollRegisterModel &operator=(const PayrollRegisterModel &) = delete;

    // https://doc.qt.io/qt-6/qabstractitemmodel.html#rowCount
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (m_model)
        {
            // cast from size_t to int
            return static_cast<int>(m_model->size());
        }
        return 0;
    };

    // https://doc.qt.io/qt-6/qabstractitemmodel.html#columnCount
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return m_columnCount;
    };

    // https://doc.qt.io/qt-6/qabstractitemmodel.html#data
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            return valueForColumn(static_cast<size_t>(index.row()), static_cast<size_t>(index.column()));
        }
        else if (role == Qt::UserRole)
        {
            return m_model->at(static_cast<size_t>(index.row())).id;
        }
        return QVariant();
    };

    // https://doc.qt.io/qt-6/qabstractitemmodel.html#headerData
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Employee ID");
            case 1:
                return QString("Employee Name");
            case 2:
                return QString("Department");
            case 3:
                return QString("Pay Period mm/yy");
            case 4:
                return QString("Pay Period Half");
            case 5:
                return QString("Basic Salary");
            case 6:
                return QString("Allowances");
            case 7:
                return QString("Deductions");
            case 8:
                return QString("Net Pay");
            default:
                return QString("");
            }
        }
        return QVariant();
    }
    void reloadData(std::vector<PayrollCalculationResults> *payroll)
    {
        if (!m_model)
            return;
        beginResetModel();
        m_model = payroll;
        endResetModel();
    }

private:
    std::vector<PayrollCalculationResults> *m_model;
    const int m_columnCount = 9;
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        // get the row
        const auto &item = m_model->at(rowIndex);

        // map the properties to the columns
        switch (columnIndex)
        {
        case 0:
            return QString::fromStdString(item.employeeId);
        case 1:
            return QString::fromStdString(item.fullName);
        case 2:
            return QString::fromStdString(item.employeeDepartment);
        case 3:
            return QString::fromStdString(item.payPeriodText);
        case 4:
            return item.payPeriodHalf == 1 ? QString::fromStdString("First Half") : QString::fromStdString("Second Half");
        case 5:
            return item.monthlyBasicSalary;
        case 6:
            return item.monthlyAllowances;
        case 7:
            return item.totalDeductions;
        case 8:
            return item.netPay;
        }
        return QVariant();
    }
};

class PayrollFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    PayrollFilterProxyModel(QObject *parent) {};
    ~PayrollFilterProxyModel() {};
    void setEmployeeFilter(const QString &id)
    {

        m_employeeId = id;
        QSortFilterProxyModel::invalidateFilter();
    }
    void setPayPeriodFilter(const QString &period)
    {

        m_payPeriod = period;
        QSortFilterProxyModel::invalidateFilter();
    }
    void setPayPeriodHalfFilter(const QString &periodhalf)
    {

        m_payPeriodHalf = periodhalf;
        QSortFilterProxyModel::invalidateFilter();
    }
    void setDepartmentFilter(const QString &department)
    {

        m_department = department;
        QSortFilterProxyModel::invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        QModelIndex employeeIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 0, sourceParent);
        QModelIndex departmentIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 2, sourceParent);
        QModelIndex periodIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 3, sourceParent);
        QModelIndex periodHalfIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 4, sourceParent);

        QString employee = QSortFilterProxyModel::sourceModel()->data(employeeIndex).toString();
        QString department = QSortFilterProxyModel::sourceModel()->data(departmentIndex).toString();
        QString period = QSortFilterProxyModel::sourceModel()->data(periodIndex).toString();
        QString periodHalf = QSortFilterProxyModel::sourceModel()->data(periodHalfIndex).toString();

        if (!m_employeeId.isEmpty() && !employee.contains(m_employeeId, Qt::CaseInsensitive))
            return false;

        if (!m_department.isEmpty() && department != m_department)
            return false;

        if (!m_payPeriod.isEmpty() && period != m_payPeriod)
            return false;

        if (!m_payPeriodHalf.isEmpty() && periodHalf != m_payPeriodHalf)
            return false;

        return true;
    };

private:
    QString m_employeeId;
    QString m_payPeriod;
    QString m_payPeriodHalf;
    QString m_department;
};

#endif