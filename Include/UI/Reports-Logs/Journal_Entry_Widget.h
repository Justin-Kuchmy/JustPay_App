#ifndef JOURNALENTRYWIDGET_H
#define JOURNALENTRYWIDGET_H
#include <QWidget>
#include "Utils/BaseContentWidget.h"
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

private:
    Ui::JournalEntryWidget *ui;
};
#endif