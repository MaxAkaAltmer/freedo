#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "glwidget.h"
#include "emuthread.h"
#include "inputconf.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void PressJoyButton(int dev, int ind);
    void PressJoyAxis(int dev, int ind, int dir);

signals:
    void SelectGame(QString game);    
    void SelectBios(QString name, int i);

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;
    GLWidget *glout;
    EmuThread *emu;
    InputConf *joyconf;
    QTimer *up_joy_devices,*up_joy_inputs;
    QSettings *opt;

    //QSettings

private slots:
    void on_actionKeep_Aspect_triggered();
    void on_actionScreenshot_triggered();
    void on_actionFullscreen_triggered();
    void on_actionSelect_Lang_BIOS_triggered();
    void on_actionSelect_BIOS_triggered();
    void on_actionPause_Resume_triggered();
    void on_actionOpen_ISO_triggered();
    void on_actionInput_triggered();
    void on_actionStart_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void updateDevices();
    void updateJoyGadgets();
};

#endif // MAINWINDOW_H
