#ifndef EMUTHREAD_H
#define EMUTHREAD_H

#include <QThread>
#include <QStringList>

class EmuThread : public QThread
{
    Q_OBJECT
public:
    EmuThread();
    void stop();
    void RunStopCPU();
    void PauseResumeCPU();
public slots:
    void SelectGame(QString game);
    void SelectBios(QString name, int i);
    void UpdateFrameComplited();
signals:
    void SendPaintMess();
protected:
    void run();
private:
    volatile bool stopflag;
    volatile bool execflag, inoffstate, pausestate;
    volatile int fver1,fver2;
    QString game_name;
};

void Sleep(int ms);


void _emulator_init();
void _emulator_exit();
unsigned int* _emulator_fb();
void _emulator_key_state(int code, bool state);
void _emulator_key_state_reset();
void _emulator_fill_pbus(QStringList &list);


#endif // EMUTHREAD_H
