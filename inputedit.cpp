#include "inputedit.h"
#include "ui_inputedit.h"

InputEdit::InputEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputEdit)
{
    ui->setupUi(this);
}

InputEdit::~InputEdit()
{
    delete ui;
}

void InputEdit::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void InputEdit::SetTextLine(QString str)
{
    ui->lineEdit->setText(str);
}

QString InputEdit::GetTextLine()
{
    return ui->lineEdit->text();
}
