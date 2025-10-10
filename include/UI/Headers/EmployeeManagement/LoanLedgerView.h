#ifndef LOANLEDGER_H
#define LOANLEDGER_H

#include <QApplication>
#include <QScreen>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../BaseView.h"

class LoanLedgerView : public BaseView {
    Q_OBJECT

public:
    explicit LoanLedgerView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};


#endif