#include "include/Generated/ui_email_login_widget.h"
#include "UI/Email_Login_Widget.h"

EmailLoginWidget::EmailLoginWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::EmailLoginWidget)
{
    ui->setupUi(this);
    ui->appPasswordEdit->setEchoMode(QLineEdit::Password);
    connect(ui->saveButton, &QPushButton::clicked, this, &EmailLoginWidget::onSaveButtonClicked);
    connect(ui->passCheckBox, &QCheckBox::clicked, this, &EmailLoginWidget::onPassCheckBoxClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    emailCrudentials = AuthService::getInstance().createOrReadSettingsFile();
    ui->emailLineEdit->setText(QString::fromStdString(emailCrudentials.companyEmail));
    ui->appPasswordEdit->setText(QString::fromStdString(emailCrudentials.appPassword));
}

EmailLoginWidget::~EmailLoginWidget()
{
    delete ui;
}

void EmailLoginWidget::onSaveButtonClicked()
{

    emailCrudentials.companyEmail = ui->emailLineEdit->text().toStdString();
    emailCrudentials.appPassword = ui->appPasswordEdit->text().toStdString();
    AuthService::getInstance().writeJsonFile(emailCrudentials);
}

void EmailLoginWidget::onPassCheckBoxClicked()
{
    if (ui->passCheckBox->isChecked())
    {
        ui->appPasswordEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->appPasswordEdit->setEchoMode(QLineEdit::Password);
    }
}
