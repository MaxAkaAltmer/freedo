#ifndef INPUTCONF_H
#define INPUTCONF_H

#include <QDialog>
#include "joyconfig.h"
#include "inputedit.h"

namespace Ui {
    class InputConf;
}

class InputConf : public QDialog {
    Q_OBJECT
public:
    InputConf(QWidget *parent = 0);
    ~InputConf();

    void PressJoyButton(int dev, int ind);
    void PressJoyAxis(int dev, int ind, int dir);
    QStringList& GetInputList(){return jlist;};
    QSettings *cfg;
    void UpdateInputList(){showEvent(NULL);};

protected:
    void changeEvent(QEvent *e);
    void closeEvent (QCloseEvent * e);
    void showEvent ( QShowEvent * event );

private:
    Ui::InputConf *ui;    
    InputEdit *edjoy;
    joyconfig *joyform;
    QStringList jlist;

private slots:
    void on_buttonBox_accepted();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();    
};

#endif // INPUTCONF_H
