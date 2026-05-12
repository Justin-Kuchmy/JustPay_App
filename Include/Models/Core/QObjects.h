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
#include "Services/EmployeeService.h"
#include "Models/Core/DataObjects.h"
#include "Models/payroll.h"
#include "Models/reporting.h"
#include <QLocale>

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
    QLocale phLocale{QLocale::English, QLocale::Philippines};
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
    std::vector<PayrollCalculationResults> *getFiltered_Model()
    {
        return this->m_model;
    }

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
        return COLUMN_COUNT;
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
                return QString("OT Pay");
            case 8:
                return QString("Adjustments");
            case 9:
                return QString("Gross Income");
            case 10:
                return QString("Employee SSS ");
            case 11:
                return QString("Employee PHIC");
            case 12:
                return QString("Employee HDMF");
            case 13:
                return QString("Employer SSS");
            case 14:
                return QString("Employer PHIC");
            case 15:
                return QString("Employer HDMF");
            case 16:
                return QString("Loans");
            case 17:
                return QString("Withholding Tax");
            case 18:
                return QString("Deductions");
            case 19:
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
    static constexpr int COLUMN_COUNT = 17;
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
            return QString::fromStdString(department_to_string(item.employeeDepartment));
        case 3:
            return QString::fromStdString(item.payPeriodDate);
        case 4:
            return item.payPeriodHalf == 1 ? QString::fromStdString("First Half") : QString::fromStdString("Second Half");
        case 5:
            return item.monthlyBasicSalary;
        case 6:
            return item.monthlyAllowances;
        case 7:
            return item.overTimePay;
        case 8:
            return item.adjustments;
        case 9:
            return item.grossIncome;
        case 10:
            return item.sssPremium_EE;
        case 11:
            return item.philHealthPremium_EE;
        case 12:
            return item.hdmfPremium_EE;
        case 13:
            return item.sssPremium_ER;
        case 14:
            return item.philHealthPremium_ER;
        case 15:
            return item.hdmfPremium_ER;
        case 16:
            return item.loanDeductionsPerPayroll;
        case 17:
            return item.withHoldingTax;
        case 18:
            return item.totalDeductions;
        case 19:
            return item.netPay;
        }
        return QVariant();
    }
};
class JournalEntryModel : public QAbstractTableModel
{
public:
    JournalEntryModel(QObject *parent) : QAbstractTableModel(parent), m_model{} {};
    JournalEntryModel(QObject *parent, std::vector<JournalEntry> *journalEntry) : QAbstractTableModel(parent), m_model(journalEntry) {}
    ~JournalEntryModel() = default;
    JournalEntryModel(const JournalEntryModel &) = delete;
    JournalEntryModel &operator=(const JournalEntryModel &) = delete;
    std::vector<JournalEntry> *getFiltered_Model()
    {
        return this->m_model;
    }

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
        return COLUMN_COUNT;
    };

    // https://doc.qt.io/qt-6/qabstractitemmodel.html#data
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {

        // check for valid index
        if (!index.isValid())
        {
            return QVariant();
        }
        // make sure its a displayroll and call valueforcolumn
        if (role == Qt::DisplayRole)
        {
            return valueForColumn(index.row(), index.column());
        }
        if (role == Qt::TextAlignmentRole)
        {
            // Center all columns
            return Qt::AlignCenter;
        }
        // check if its a userrole then return the id
        if (role == Qt::UserRole)
        {
            return QVariant(m_model->at(static_cast<size_t>(index.row())).entryId);
        }
        return QVariant();
    };

    // https://doc.qt.io/qt-6/qabstractitemmodel.html#headerData
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {

        // if its not a display roll then return default?
        if (role != Qt::DisplayRole)
        {
            return QVariant();
        }
        // if orientation is horizontal enter switch case for the section and check columns, then return qstrings
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Account Type");
            case 1:
                return QString("Account Name");
            case 2:
                return QString("Debit");
            case 3:
                return QString("Credit");
            case 4:
                return QString("Pay Period Text");
            case 5:
                return QString("Pay Period Half");
            }
        }
        return QVariant();
    }
    void reloadData(std::vector<JournalEntry> *journalEntry)
    {
        if (!m_model)
            return;
        beginResetModel();
        m_model = journalEntry;
        endResetModel();
    }

private:
    std::vector<JournalEntry> *m_model;
    static constexpr int COLUMN_COUNT = 6;
    QLocale phLocale{QLocale::English, QLocale::Philippines};
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        // use the row and column index to search m_model to get the data
        auto &item = m_model->at(rowIndex);

        switch (columnIndex)
        {
        case 0:
            return QVariant(QString::fromStdString(AccountType_to_string(static_cast<int>(item.accountType))));
        case 1:
            return QVariant(QString::fromStdString(item.accountName));
        case 2:
        {
            if (item.debit == 0.0)
                return QVariant(QString::fromStdString("---"));
            else
            {
                return QVariant(QString("₱%1").arg(phLocale.toString(item.debit, 'f', 2)));
            }
        }
        case 3:
        {
            if (item.credit == 0.0)
                return QVariant(QString::fromStdString("---"));
            else
            {
                return QVariant(QString("₱%1").arg(phLocale.toString(item.credit, 'f', 2)));
            }
        }
        case 4:
            return QVariant(QString::fromStdString(item.periodHalf));
        case 5:
            return QVariant(QString::fromStdString(item.periodText));
        }
        // row is the model index and use column in the switch
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
class JournalEntryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    JournalEntryFilterProxyModel(QObject *parent) {};
    ~JournalEntryFilterProxyModel() {};
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

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        QModelIndex periodIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 4, sourceParent);
        QModelIndex periodHalfIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 5, sourceParent);
        QString period = QSortFilterProxyModel::sourceModel()->data(periodIndex).toString();
        QString periodHalf = QSortFilterProxyModel::sourceModel()->data(periodHalfIndex).toString();

        if (!m_payPeriod.isEmpty() && period != m_payPeriod)
            return false;
        if (!m_payPeriodHalf.isEmpty() && periodHalf != m_payPeriodHalf)
            return false;
        return true;
    };

private:
    QString m_payPeriod;
    QString m_payPeriodHalf;
};
class GovernmentRemittanceModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    GovernmentRemittanceModel(QObject *parent) : QAbstractItemModel(parent), m_model{} {};
    GovernmentRemittanceModel(QObject *parent, std::vector<GovernmentRemittance> *remits) : QAbstractItemModel(parent), m_model(remits) {}
    ~GovernmentRemittanceModel() = default;
    GovernmentRemittanceModel(const GovernmentRemittanceModel &) = delete;
    GovernmentRemittanceModel &operator=(const GovernmentRemittanceModel &) = delete;

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
            return m_model->at(static_cast<size_t>(index.row())).id;
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
                return QString("Contribution Type");
            case 2:
                return QString("Employee Share");
            case 3:
                return QString("Employer Share");
            case 4:
                return QString("Total");
            case 5:
                return QString("Period");
            }
        }
        return QVariant();
    }
    void reloadData(const std::vector<GovernmentRemittance> &newRemits)
    {
        beginResetModel();
        *m_model = newRemits;
        endResetModel();
    }

private:
    std::vector<GovernmentRemittance> *m_model;
    const int m_columnCount = 6;
    QLocale phLocale{QLocale::English, QLocale::Philippines};
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        const auto &item = m_model->at(rowIndex);
        QString employeeId{""};
        switch (columnIndex)
        {
        case 0:
            return QString::fromStdString(item.employeeId);
        case 1:
            return QString::fromStdString(RemittanceType_to_string(item.contrib_Type));
        case 2:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.employee_Contrib, 'f', 2)));
        case 3:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.employer_Contrib, 'f', 2)));
        case 4:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.total_Contrib, 'f', 2)));
        case 5:
            return QVariant(QString::fromStdString(item.payPeriodDate));
        }
        return QVariant();
    }
};
class GovernmentRemittanceFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    GovernmentRemittanceFilterProxyModel(QObject *parent) {};
    ~GovernmentRemittanceFilterProxyModel() {};

    void setEmployeeFilter(const QString &filter)
    {
        m_employeeId = filter;
        QSortFilterProxyModel::invalidateFilter();
    }
    void setPayPeriodFilter(const QString &period)
    {

        m_payPeriod = period;
        QSortFilterProxyModel::invalidateFilter();
    }

    void setcontributionTypeFilter(QString contrib)
    {
        m_contributionType = contrib;
        QSortFilterProxyModel::invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        QModelIndex periodIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 5, sourceParent);
        QModelIndex contribIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 1, sourceParent);
        QModelIndex employeeIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 0, sourceParent);
        QString period = QSortFilterProxyModel::sourceModel()->data(periodIndex).toString();
        QString contrib = QSortFilterProxyModel::sourceModel()->data(contribIndex).toString();
        QString employee = QSortFilterProxyModel::sourceModel()->data(employeeIndex).toString();

        if (!m_payPeriod.isEmpty() && period != m_payPeriod)
            return false;
        if (!m_contributionType.isEmpty() && contrib != m_contributionType)
            return false;
        if (!m_employeeId.isEmpty() && !employee.contains(m_employeeId, Qt::CaseInsensitive))
            return false;
        return true;
    };

private:
    QString m_payPeriod;
    QString m_contributionType;
    QString m_employeeId;
};
class YearEndBenefitsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit YearEndBenefitsModel(QObject *parent = nullptr) : QAbstractTableModel(parent), m_model{} {}
    YearEndBenefitsModel(QObject *parent, std::vector<YearEndBenefits> *data) : QAbstractTableModel(parent), m_model(data) {}
    ~YearEndBenefitsModel() = default;
    YearEndBenefitsModel(const YearEndBenefitsModel &) = delete;
    YearEndBenefitsModel &operator=(const YearEndBenefitsModel &) = delete;

    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_model ? static_cast<int>(m_model->size()) : 0; }
    int columnCount(const QModelIndex & = QModelIndex()) const override { return m_columnCount; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {

        if (!index.isValid())
        {
            return QVariant();
        }
        if (role == Qt::DisplayRole)
        {
            return valueForColumn(index.row(), index.column());
        }
        if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }
        if (role == Qt::UserRole)
        {
            return QVariant(QString::fromStdString(m_model->at(static_cast<size_t>(index.row())).employeeId));
        }
        return QVariant();
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {

        if (role != Qt::DisplayRole)
        {
            return QVariant();
        }
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Employee ID");
            case 1:
                return QString("Year");
            case 2:
                return QString("Total Basic Salary");
            case 3:
                return QString("13th Month Pay");
            case 4:
                return QString("Unused Leave Days");
            case 5:
                return QString("Daily Rate");
            case 6:
                return QString("Mometized Leave Value");
            }
        }
        return QVariant();
    }
    void reloadData(std::vector<YearEndBenefits> *yearEndBenefits)
    {
        if (!m_model)
            return;
        beginResetModel();
        m_model = yearEndBenefits;
        endResetModel();
    }

private:
    QLocale phLocale{QLocale::English, QLocale::Philippines};
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        // use the row and column index to search m_model to get the data
        auto &item = m_model->at(rowIndex);
        switch (columnIndex)
        {
        case 0:
            return QVariant(QString::fromStdString(item.employeeId));
        case 1:
            return QVariant(QString::fromStdString(std::to_string(item.year)));
        case 2:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.totalBasicSalary, 'f', 2)));
        case 3:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.thirteenthMonthPay, 'f', 2)));
        case 4:
            return QVariant(QString::fromStdString(std::format("{:.2f}", item.unusedLeaveDays)));
        case 5:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.dailyRate, 'f', 2)));
        case 6:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.monetizedLeaveValue, 'f', 2)));
        }
        return QVariant();
    }

    std::vector<YearEndBenefits> *m_model = nullptr;
    static constexpr int m_columnCount = 7;
};
class YearEndBenefitsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit YearEndBenefitsFilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    ~YearEndBenefitsFilterProxyModel() = default;

    void setYearFilter(const QString &year)
    {
        m_yearFilter = year;
        QSortFilterProxyModel::invalidateFilter();
    };
    void setEmployeeFilter(const QString &empId)
    {
        m_employeeFilter = empId;
        QSortFilterProxyModel::invalidateFilter();
    };

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        QModelIndex yearIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 1, sourceParent);
        QString year = QSortFilterProxyModel::sourceModel()->data(yearIndex).toString();

        QModelIndex employeeIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 0, sourceParent);
        QString employee = QSortFilterProxyModel::sourceModel()->data(employeeIndex).toString();
        if (!m_employeeFilter.isEmpty() && !employee.contains(m_employeeFilter, Qt::CaseInsensitive))
            return false;

        if (!m_yearFilter.isEmpty() && year != m_yearFilter)
            return false;
        return true;
    };

private:
    QString m_yearFilter;
    QString m_employeeFilter;
};
class TaxReconciliationReportModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TaxReconciliationReportModel(QObject *parent = nullptr) : QAbstractTableModel(parent), m_model{} {}
    TaxReconciliationReportModel(QObject *parent, std::vector<TaxReconciliationReport> *data) : QAbstractTableModel(parent), m_model(data) {}
    ~TaxReconciliationReportModel() = default;
    TaxReconciliationReportModel(const TaxReconciliationReportModel &) = delete;
    TaxReconciliationReportModel &operator=(const TaxReconciliationReportModel &) = delete;

    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_model ? static_cast<int>(m_model->size()) : 0; }
    int columnCount(const QModelIndex & = QModelIndex()) const override { return m_columnCount; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {

        if (!index.isValid())
        {
            return QVariant();
        }
        if (role == Qt::DisplayRole)
        {
            return valueForColumn(index.row(), index.column());
        }
        if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }
        if (role == Qt::UserRole)
        {
            return QVariant(QString::fromStdString(m_model->at(static_cast<size_t>(index.row())).employeeId));
        }
        return QVariant();
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {

        if (role != Qt::DisplayRole)
        {
            return QVariant();
        }
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Employee ID");
            case 1:
                return QString("TIN");
            case 2:
                return QString("Gross Income");
            case 3:
                return QString("13th Month & Leave");
            case 4:
                return QString("De Minimis");
            case 5:
                return QString("Gov't Contributions");
            case 6:
                return QString("Non-Taxable Total");
            case 7:
                return QString("Basic Pay");
            case 8:
                return QString("Taxable 13th Month");
            case 9:
                return QString("Overtime Pay");
            case 10:
                return QString("Taxable De Minimis");
            case 11:
                return QString("Taxable Total");
            case 12:
                return QString("Tax Due");
            case 13:
                return QString("Tax Withheld");
            case 14:
                return QString("Tax Variance");
            }
        }
        return QVariant();
    }
    void reloadData(std::vector<TaxReconciliationReport> *taxReconciliationReport)
    {
        if (!m_model)
            return;
        beginResetModel();
        m_model = taxReconciliationReport;
        endResetModel();
    }

private:
    QLocale phLocale{QLocale::English, QLocale::Philippines};
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        // use the row and column index to search m_model to get the data
        auto &item = m_model->at(rowIndex);

        switch (columnIndex)
        {
        case 0:
            return QVariant(QString::fromStdString(item.employeeId));
        case 1:
            return QVariant(QString::fromStdString(item.tin));
        case 2:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.grossIncome, 'f', 2)));
        case 3:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.thirteenthMonthAndLeave, 'f', 2)));
        case 4:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.deMinimis, 'f', 2)));
        case 5:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.govtContributions, 'f', 2)));
        case 6:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.nonTaxableTotal, 'f', 2)));
        case 7:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.basicPay, 'f', 2)));
        case 8:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.taxable13thMonth, 'f', 2)));
        case 9:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.overtimePay, 'f', 2)));
        case 10:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.taxableDeMinimis, 'f', 2)));
        case 11:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.taxableTotal, 'f', 2)));
        case 12:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.taxDue, 'f', 2)));
        case 13:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.taxWithheld, 'f', 2)));
        case 14:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.taxVariance, 'f', 2)));
        }
        return QVariant();
    }

    std::vector<TaxReconciliationReport> *m_model = nullptr;
    static constexpr int m_columnCount = 15;
};
class TaxReconciliationReportFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TaxReconciliationReportFilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    ~TaxReconciliationReportFilterProxyModel() = default;

    void setEmployeeFilter(const QString &employee)
    {
        m_employeeFilter = employee;
        QSortFilterProxyModel::invalidateFilter();
    };

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        QModelIndex employeeIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 0, sourceParent);

        QString employee = QSortFilterProxyModel::sourceModel()->data(employeeIndex).toString();

        if (!m_employeeFilter.isEmpty() && !employee.contains(m_employeeFilter))
            return false;
        return true;
    };

private:
    QString m_yearFilter;
    QString m_departmentFilter;
    QString m_employeeFilter;
};
class MonthlyBudgetUtilizationReportModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MonthlyBudgetUtilizationReportModel(QObject *parent = nullptr) : QAbstractTableModel(parent), m_model{} {}
    MonthlyBudgetUtilizationReportModel(QObject *parent, std::vector<MonthlyBudgetUtilizationReport> *data) : QAbstractTableModel(parent), m_model(data) {}
    ~MonthlyBudgetUtilizationReportModel() = default;
    MonthlyBudgetUtilizationReportModel(const MonthlyBudgetUtilizationReportModel &) = delete;
    MonthlyBudgetUtilizationReportModel &operator=(const MonthlyBudgetUtilizationReportModel &) = delete;

    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_model ? static_cast<int>(m_model->size()) : 0; }
    int columnCount(const QModelIndex & = QModelIndex()) const override { return m_columnCount; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {

        if (!index.isValid())
        {
            return QVariant();
        }
        if (role == Qt::DisplayRole)
        {
            return valueForColumn(index.row(), index.column());
        }
        if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }
        if (role == Qt::UserRole)
        {
            return QVariant(QString::number(m_model->at(static_cast<size_t>(index.row())).departmentId));
        }
        return QVariant();
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {

        if (role != Qt::DisplayRole)
        {
            return QVariant();
        }
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Department");
            case 1:
                return QString("Period");
            case 2:
                return QString("Basic Pay");
            case 3:
                return QString("13th Month Pay");
            case 4:
                return QString("Allowances");
            case 5:
                return QString("Adjustments");
            case 6:
                return QString("Employer SSS Premium");
            case 7:
                return QString("Employer PHIC Premium");
            case 8:
                return QString("Employer HDMF Premium");
            case 9:
                return QString("Grand Total");
            }
        }
        return QVariant();
    }
    void reloadData(std::vector<MonthlyBudgetUtilizationReport> *monthlyBudgetUtilizationReport)
    {
        if (!m_model)
            return;
        beginResetModel();
        m_model = monthlyBudgetUtilizationReport;
        endResetModel();
    }

private:
    QLocale phLocale{QLocale::English, QLocale::Philippines};
    QVariant valueForColumn(size_t rowIndex, size_t columnIndex) const
    {
        // use the row and column index to search m_model to get the data
        auto &item = m_model->at(rowIndex);

        switch (columnIndex)
        {
        case 0:
            return QVariant(QString::fromStdString(item.departmentName));
        case 1:
            return QVariant(QString::fromStdString(item.periodLabel));
        case 2:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.totalBasicPay, 'f', 2)));
        case 3:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.total13MonthPay, 'f', 2)));
        case 4:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.totalAllowances, 'f', 2)));
        case 5:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.totalAdjustments, 'f', 2)));
        case 6:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.total_sssPremium_Employer, 'f', 2)));
        case 7:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.total_phicPremium_Employer, 'f', 2)));
        case 8:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.total_hdmfPremium_Employer, 'f', 2)));
        case 9:
            return QVariant(QString("₱%1").arg(phLocale.toString(item.grandTotal, 'f', 2)));
        }
        return QVariant();
    }

    std::vector<MonthlyBudgetUtilizationReport> *m_model = nullptr;
    static constexpr int m_columnCount = 12;
};
class MonthlyBudgetUtilizationReportFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MonthlyBudgetUtilizationReportFilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    ~MonthlyBudgetUtilizationReportFilterProxyModel() = default;

    void setDepartmentFilter(const QString &department)
    {
        m_departmentFilter = department;
        QSortFilterProxyModel::invalidateFilter();
    };
    void setPeriodFilter(const QString &period)
    {
        m_periodFilter = period;
        QSortFilterProxyModel::invalidateFilter();
    };

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        QModelIndex departmentIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 0, sourceParent);
        QModelIndex periodIndex = QSortFilterProxyModel::sourceModel()->index(sourceRow, 1, sourceParent);

        QString department = QSortFilterProxyModel::sourceModel()->data(departmentIndex).toString();
        QString period = QSortFilterProxyModel::sourceModel()->data(periodIndex).toString();
        qDebug() << "period:" << period << "len:" << period.length();
        qDebug() << "filter:" << m_periodFilter << "len:" << m_periodFilter.length();

        if (!m_departmentFilter.isEmpty() && department != m_departmentFilter)
            return false;
        if (!m_periodFilter.isEmpty() && !period.contains(m_periodFilter, Qt::CaseInsensitive))
            return false;
        return true;
    };

private:
    QString m_departmentFilter;
    QString m_periodFilter;
};
#endif