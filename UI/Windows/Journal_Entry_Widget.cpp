#include "include/Generated/ui_journal_entries.h"
#include "UI/Journal_Entry_Widget.h"
JournalEntryWidget::JournalEntryWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::JournalEntryWidget)
{
    ui->setupUi(this);
}
JournalEntryWidget::~JournalEntryWidget() { delete ui; }
