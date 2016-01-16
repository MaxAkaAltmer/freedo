#include "joyconfig.h"
#include "ui_joyconfig.h"

joyconfig::joyconfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::joyconfig)
{
    ui->setupUi(this);
    curr_state=11;
}

void joyconfig::showEvent ( QShowEvent * event )
{
    ui->label->setText("Press - Up");
    curr_state=0;
    cfgstr="Joy";

}

joyconfig::~joyconfig()
{
    delete ui;
}

void joyconfig::changeEvent(QEvent *e)
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

void joyconfig::PressJoyAxis(int dev, int ind, int dir)
{
    if(curr_state>10)return;
    if(dev==-1)return;

    switch(curr_state++)
    {
    case 0:
        ui->label->setText("Press - Down");
        break;
    case 1:
        ui->label->setText("Press - Left");
        break;
    case 2:
        ui->label->setText("Press - Right");
        break;
    case 3:
        ui->label->setText("Press - L-Shift");
        break;
    case 4:
        ui->label->setText("Press - R-Shift");
        break;
    case 5:
        ui->label->setText("Press - X-Button");
        break;
    case 6:
        ui->label->setText("Press - P-Button");
        break;
    case 7:
        ui->label->setText("Press - A-Button");
        break;
    case 8:
        ui->label->setText("Press - B-Button");
        break;
    case 9:
        ui->label->setText("Press - C-Button");
        break;
    case 10:
        this->close();
        break;
    }

    QString str,str1;

    str.setNum(dev,10);
    str+="a";
    str1.setNum(ind,10);
    str+=str1;
    if(dir<0)str+="-";
    else str+='+';

    if(curr_state==1) cfgstr+="=j"+str;
    else cfgstr+=",j"+str;
}

void joyconfig::PressJoyButton(int dev, int ind)
{
    if(curr_state>10)return;
    if(dev==-1)return;
    switch(curr_state++)
    {
    case 0:
        ui->label->setText("Press - Down");
        break;
    case 1:
        ui->label->setText("Press - Left");
        break;
    case 2:
        ui->label->setText("Press - Right");
        break;
    case 3:
        ui->label->setText("Press - L-Shift");
        break;
    case 4:
        ui->label->setText("Press - R-Shift");
        break;
    case 5:
        ui->label->setText("Press - X-Button");
        break;
    case 6:
        ui->label->setText("Press - P-Button");
        break;
    case 7:
        ui->label->setText("Press - A-Button");
        break;
    case 8:
        ui->label->setText("Press - B-Button");
        break;
    case 9:
        ui->label->setText("Press - C-Button");
        break;
    case 10:
        this->close();
        break;
    }

    QString str,str1;

    if(curr_state==1) cfgstr+="=j"+str.setNum(dev,10)+"b"+str1.setNum(ind,10);
    else cfgstr+=",j"+str.setNum(dev,10)+"b"+str1.setNum(ind,10);
}

void joyconfig::keyPressEvent(QKeyEvent* event)
{    

    switch(curr_state++)
    {
    case 0:
        ui->label->setText("Press - Down");
        break;
    case 1:
        ui->label->setText("Press - Left");
        break;
    case 2:
        ui->label->setText("Press - Right");
        break;
    case 3:
        ui->label->setText("Press - L-Shift");
        break;
    case 4:
        ui->label->setText("Press - R-Shift");
        break;
    case 5:
        ui->label->setText("Press - X-Button");
        break;
    case 6:
        ui->label->setText("Press - P-Button");
        break;
    case 7:
        ui->label->setText("Press - A-Button");
        break;
    case 8:
        ui->label->setText("Press - B-Button");
        break;
    case 9:
        ui->label->setText("Press - C-Button");
        break;
    case 10:
        this->close();
        break;
    }

    QString str;

    str.setNum(event->key(),16);

    if(curr_state==1) cfgstr+="=k"+str;
    else cfgstr+=",k"+str;

}
