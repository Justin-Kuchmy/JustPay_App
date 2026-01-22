#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTextDocument>
#include <QPixmap>
#include <QPainter>

class PayslipPreviewDialog : public QDialog
{
    Q_OBJECT
public:
    PayslipPreviewDialog(const QString &htmlContent, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Payslip Preview - Preview May Reflect Differences From Actual Payslip");
        resize(600, 800);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        layout->addWidget(scrollArea);

        QLabel *previewLabel = new QLabel(this);
        previewLabel->setAlignment(Qt::AlignCenter);
        scrollArea->setWidget(previewLabel);

        QTextDocument doc;
        doc.setHtml(htmlContent);

        QPixmap pixmap(doc.size().toSize());
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        doc.drawContents(&painter);
        painter.end();

        previewLabel->setPixmap(pixmap.scaledToWidth(550, Qt::SmoothTransformation));
    }
};
