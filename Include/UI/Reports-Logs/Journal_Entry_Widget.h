#ifndef JOURNALENTRYWIDGET_H
#define JOURNALENTRYWIDGET_H
#include <QWidget>
#include "Utils/BaseContentWidget.h"
#include <QLocale>
namespace Ui
{
    class JournalEntryWidget;
}
class JournalEntryWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    JournalEntryWidget(QWidget *parent = nullptr);
    ~JournalEntryWidget();
    JournalEntryWidget(const JournalEntryWidget &) = delete;
    JournalEntryWidget &operator=(const JournalEntryWidget &) = delete;
    void onPayrollPeriodChanged();
    void onExportCSVClicked();

private:
    Ui::JournalEntryWidget *ui;
    QStringList entryPayPeriods;
    std::vector<JournalEntry> entries;
    std::vector<JournalEntry> *entriesPtr = nullptr;
    double debitTotals{};
    double creditTotals{};

    JournalEntryModel *model;
    JournalEntryFilterProxyModel *m_proxy;
    QLocale phLocale{QLocale::English, QLocale::Philippines};
};
#endif