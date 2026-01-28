#include "include/Generated/ui_email_login_widget.h"
#include "UI/Email_Login_Widget.h"

EmailLoginWidget::EmailLoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::EmailLoginWidget)
{
    ui->setupUi(this);
    ui->appPasswordEdit->setEchoMode(QLineEdit::Password);
    connect(ui->loginButton, &QPushButton::clicked, this, &EmailLoginWidget::onLoginButtonClicked);
    connect(ui->passCheckBox, &QCheckBox::clicked, this, &EmailLoginWidget::onPassCheckBoxClicked);
}

EmailLoginWidget::~EmailLoginWidget()
{
    delete ui;
}

void EmailLoginWidget::onLoginButtonClicked()
{
    qDebug() << "Login Button Clicked";
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
