#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWaitCondition>
#include <QMutex>
#include "Joy/Gamepad.h"

int joyIndex(Gamepad_device * device)
{
    for(int i = 0; i < Gamepad_numDevices(); i++)
    {
        if(device==Gamepad_deviceAtIndex(i))return i;
    }
    return -1;
}

bool onButtonDown(void * sender, const char * eventID, void * eventData, void * context)
{
 struct Gamepad_buttonEvent * event;
 MainWindow *mw=(MainWindow*)context;

        event = (Gamepad_buttonEvent *) eventData;

        mw->PressJoyButton(joyIndex(event->device),event->buttonID);

        return true;
}

bool onButtonUp(void * sender, const char * eventID, void * eventData, void * context)
{
        return true;
}

bool onAxisMoved(void * sender, const char * eventID, void * eventData, void * context)
{
 MainWindow *mw=(MainWindow*)context;
 static int direct=0;
 static int dev=-1,ind=-1;
 struct Gamepad_axisEvent * event;

 int curr_dev, curr_ind, curr_dir;

        event = (Gamepad_axisEvent *) eventData;

        curr_dev=joyIndex(event->device);
        curr_ind=event->axisID;
        if(event->value<-0.5)curr_dir=-1;
        else if(event->value>0.5)curr_dir=1;
        else
        {
            direct=0;
            return true;
        }

        if(dev!=curr_dev || ind!=curr_ind || direct!=curr_dir)
        {
            dev=curr_dev;
            ind=curr_ind;
            direct=curr_dir;
            mw->PressJoyAxis(dev,ind,direct);
        }

        return true;
}


bool onDeviceAttached(void * sender, const char * eventID, void * eventData, void * context)
{
        struct Gamepad_device * device;

        device = (Gamepad_device *)eventData;

        device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_BUTTON_DOWN, onButtonDown, context);
        device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_BUTTON_UP, onButtonUp, context);
        device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_AXIS_MOVED, onAxisMoved, context);

        return true;
}

bool onDeviceRemoved(void * sender, const char * eventID, void * eventData, void * context)
{
        struct Gamepad_device * device;

        device = (Gamepad_device *)eventData;
        return true;
}

void Sleep(int ms)
{
     QWaitCondition sleep;
     QMutex mutex;
     mutex.lock();
     sleep.wait(&mutex, ms);
     mutex.unlock();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    glout=new GLWidget(this);
    setCentralWidget(glout);
    joyconf=new InputConf(this);       

    _emulator_init();
    _emulator_key_state_reset();

    emu=new EmuThread;
    connect(emu,SIGNAL(SendPaintMess()),glout,SLOT(BuffRedraw()));
    connect(glout,SIGNAL(UpdateFrameComplited()),emu,SLOT(UpdateFrameComplited()));
    connect(this,SIGNAL(SelectGame(QString)),emu,SLOT(SelectGame(QString)));
    connect(this,SIGNAL(SelectBios(QString,int)),emu,SLOT(SelectBios(QString,int)));

    emu->start();

    Gamepad_eventDispatcher()->registerForEvent(Gamepad_eventDispatcher(), GAMEPAD_EVENT_DEVICE_ATTACHED, onDeviceAttached, this);
    Gamepad_eventDispatcher()->registerForEvent(Gamepad_eventDispatcher(), GAMEPAD_EVENT_DEVICE_REMOVED, onDeviceRemoved, this);
    Gamepad_init();

    up_joy_devices=new QTimer(this);
    connect(up_joy_devices, SIGNAL(timeout()), this, SLOT(updateDevices()));
    up_joy_devices->start(1000);

    up_joy_inputs=new QTimer(this);
    connect(up_joy_inputs, SIGNAL(timeout()), this, SLOT(updateJoyGadgets()));
    up_joy_inputs->start(20);

    opt=new QSettings("freedo.cfg",QSettings::IniFormat);
    joyconf->cfg=opt;

    joyconf->UpdateInputList();

    //инициализация иконки
    setWindowIcon(QIcon(":/pics/logo.png"));


    emit SelectGame(opt->value("Game","").toString());
    emit SelectBios(opt->value("BIOS/Main","").toString(),0);
    emit SelectBios(opt->value("BIOS/Lang","").toString(),1);

    glout->keep_aspect=opt->value("Video/KeepAspect",true).toBool();
    ui->actionKeep_Aspect->setChecked(glout->keep_aspect);
}

void MainWindow::PressJoyAxis(int dev, int ind, int dir)
{
    joyconf->PressJoyAxis(dev, ind, dir);
}

void MainWindow::PressJoyButton(int dev, int ind)
{
    joyconf->PressJoyButton(dev,ind);
}

void MainWindow::updateDevices()
{
    Gamepad_detectDevices();
}

void MainWindow::updateJoyGadgets()
{
    Gamepad_processEvents();
    _emulator_fill_pbus(joyconf->GetInputList());
}

MainWindow::~MainWindow()
{
    up_joy_devices->stop();
    up_joy_inputs->stop();
    delete up_joy_devices;
    delete up_joy_inputs;
    Gamepad_shutdown();

    emu->stop();
    while(!emu->isFinished())
    {
        Sleep(10);
    }
    delete glout;
    delete ui;

    _emulator_exit();

    delete opt;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       tr("About FreeDO"),
                       tr("Copyright (C) 2011 FreeDO Team."
                       "<p>Original project site: www.freedo.org"
                       ));
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_actionStart_triggered()
{    
    emu->RunStopCPU();
    _emulator_key_state_reset();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    _emulator_key_state(event->key(),true);

    if(event->key()==Qt::Key_F9)
    {
        ui->actionFullscreen->setChecked(!ui->actionFullscreen->isChecked());
        on_actionFullscreen_triggered();
    }
    else if(event->key()==Qt::Key_F4)
    {
        on_actionScreenshot_triggered();
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    _emulator_key_state(event->key(),false);
}

void MainWindow::on_actionInput_triggered()
{
    joyconf->show();
}

void MainWindow::on_actionOpen_ISO_triggered()
{
 QString game=QFileDialog::getOpenFileName(this,"Open game image",opt->value("Game","").toString());
    emit SelectGame(game);
    opt->setValue("Game",game);
}

void MainWindow::on_actionPause_Resume_triggered()
{
    emu->PauseResumeCPU();
}

void MainWindow::on_actionSelect_BIOS_triggered()
{
 QString name=QFileDialog::getOpenFileName(this,"Open main BIOS ROM",opt->value("BIOS/Main","").toString());
    emit SelectBios(name,0);
    opt->setValue("BIOS/Main",name);
}

void MainWindow::on_actionSelect_Lang_BIOS_triggered()
{
 QString name=QFileDialog::getOpenFileName(this,"Open lang BIOS ROM",opt->value("BIOS/Lang","").toString());
       emit SelectBios(name,1);
       opt->setValue("BIOS/Lang",name);
}

void MainWindow::on_actionFullscreen_triggered()
{
    if(ui->actionFullscreen->isChecked())
    {
        this->showFullScreen();
        this->statusBar()->setVisible(false);
        this->menuBar()->setVisible(false);
        this->setCursor(QCursor(Qt::BlankCursor));
    }
    else
    {
        this->showNormal();
        this->statusBar()->setVisible(true);
        this->menuBar()->setVisible(true);
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void MainWindow::on_actionScreenshot_triggered()
{
    QString str;
    QDateTime time;

        time=time.currentDateTime();

        str=opt->value("Game","").toString();
        str+="_"+time.toString("dd-MM-yyyy-hh-mm-ss-zzz");
        str+=".png";

        glout->ScreenShot(str);
}

void MainWindow::on_actionKeep_Aspect_triggered()
{
    glout->keep_aspect=ui->actionKeep_Aspect->isChecked();
    opt->setValue("Video/KeepAspect",glout->keep_aspect);
    glout->BuffRedraw();
}
