#ifndef JOYCONFIG_H
#define JOYCONFIG_H

#include <QDialog>
#include <QtGui>

namespace Ui {
    class joyconfig;
}

class joyconfig : public QDialog {
    Q_OBJECT
public:
    joyconfig(QWidget *parent = 0);
    ~joyconfig();

    QString cfgstr;

    void PressJoyButton(int dev, int ind);
    void PressJoyAxis(int dev, int ind, int dir);

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent* event);
    void showEvent ( QShowEvent * event );

private:
    Ui::joyconfig *ui;
    int curr_state;    
};

#endif // JOYCONFIG_H
