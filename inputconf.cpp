#include "inputconf.h"
#include "ui_inputconf.h"

InputConf::InputConf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputConf)
{
    ui->setupUi(this);
    joyform = new joyconfig(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    edjoy=new InputEdit(this);


}

InputConf::~InputConf()
{
    delete ui;
    delete joyform;
    delete edjoy;
}

void InputConf::PressJoyButton(int dev, int ind)
{
    joyform->PressJoyButton(dev, ind);
}

void InputConf::PressJoyAxis(int dev, int ind, int dir)
{
    joyform->PressJoyAxis(dev, ind, dir);
}

void InputConf::changeEvent(QEvent *e)
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

void InputConf::on_pushButton_clicked()
{
    joyform->exec();
    ui->listWidget->addItem(joyform->cfgstr);
}

void InputConf::on_pushButton_2_clicked()
{
    delete ui->listWidget->currentItem();
}

void InputConf::on_pushButton_3_clicked()
{
    if(!ui->listWidget->selectedItems().size())return;
    joyform->exec();
    ui->listWidget->currentItem()->setText(joyform->cfgstr);
}

void InputConf::on_pushButton_4_clicked()
{
    if(!ui->listWidget->selectedItems().size())return;
    edjoy->SetTextLine(ui->listWidget->currentItem()->text());
    edjoy->exec();
    ui->listWidget->currentItem()->setText(edjoy->GetTextLine());
}

void InputConf::showEvent ( QShowEvent * event )
{
 int i,n;
 QString tmp;

    n=cfg->beginReadArray("Input");
    ui->listWidget->clear();
    jlist.clear();
    for(i=0;i<n;i++)
    {
        cfg->setArrayIndex(i);
        tmp=cfg->value("conf").toString();
        ui->listWidget->addItem(tmp);
        jlist.append(tmp);
    }
    cfg->endArray();
}

void InputConf::closeEvent (QCloseEvent * e)
{
 int i,num=ui->listWidget->count();
    jlist.clear();
    cfg->remove("Input");
    cfg->beginWriteArray("Input",num);
    for(i=0;i<num;i++)
    {
        jlist.append(ui->listWidget->item(i)->text());
        cfg->setArrayIndex(i);
        cfg->setValue("conf",ui->listWidget->item(i)->text());
    }
    cfg->endArray();
}

void InputConf::on_buttonBox_accepted()
{
    closeEvent(NULL);
}
