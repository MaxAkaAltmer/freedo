#include "emuthread.h"
#include "OutSound.h"
#include <QImage>
#include <QFile>

#include "core/freedocore.h"
#include "Joy/Gamepad.h"

typedef unsigned char uint8;


volatile unsigned int *emu_buff; //буффер кадра эмуляции
unsigned char *bios_buff;
VDLFrame *_3do_Frame;

void _emulator_set_baner()
{
    QImage img;

    img.load(":/pics/baner.png");

    for(int i=0;i<img.height() && i<480;i++)
    {
            for(int j=0;j<img.width() && j<640;j++)
            {
                    emu_buff[(480-i-1)*640+j]= ((unsigned int*)img.bits())[i*img.width()+j];
            }
    }
}

void _emulator_LoadBios(QString fname, int ind)
{
    QFile hand(fname);
    if(hand.open(QIODevice::ReadOnly))
    {
        if(!ind)hand.read((char*)bios_buff,1024*1024);
        else hand.read((char*)bios_buff+1024*1024,1024*1024);
        hand.close();
    }
}

void _emulator_init()
{
    emu_buff=new unsigned int[640*480];
    _3do_Frame=new VDLFrame;

    _emulator_set_baner();

    _sndal_Init();


    bios_buff=new unsigned char[1024*1024*2];
    memset(bios_buff,0xff,1024*1024*2);

}

void _emulator_exit()
{
    _sndal_Destroy();
    delete []emu_buff;
    delete []bios_buff;
    delete _3do_Frame;
}

unsigned int* _emulator_fb()
{
    return (unsigned int*)emu_buff;
}

void _3do_DPrint(const char *str)
{

}

unsigned int __bswap(unsigned int x)
{
        return (x>>24) | ((x>>8)&0x0000FF00L) | ((x&0x0000FF00L)<<8) | (x<<24);
}

void _3do_SaveNVRAM(const void *buff)
{
        QFile hand("nvram.bin");
        if(hand.open(QIODevice::WriteOnly))
        {
            hand.write((char*)buff,65536/2);
            hand.close();
        }
}
unsigned char nvramhead[]=
{
        0x01,0x5a,0x5a,0x5a,0x5a,0x5a,0x02,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0x6e,0x76,0x72,0x61,0x6d,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0xff,0xff,0xff,0xff,0,0,0,1,
        0,0,0x80,0,0xff,0xff,0xff,0xfe,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0x84,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0x85,0x5a,2,0xb6,0,0,0,0x98,0,0,0,0x98,
        0,0,0,0x14,0,0,0,0x14,0x7A,0xa5,0x65,0xbd,0,0,0,0x84,
        0,0,0,0x84,0,0,0x76,0x68,0,0,0,0x14
};
void _3do_ResetNVRAM(void *buff)
{
        memset(buff,0,65536/2);
        memcpy(buff,nvramhead,sizeof(nvramhead));
}
void _3do_LoadNVRAM(void *buff)
{
    QFile hand("nvram.bin");
    if(hand.open(QIODevice::ReadOnly))
    {
        hand.read((char*)buff,65536/2);
        hand.close();
    }
    else
    {
        _3do_ResetNVRAM(buff);
    }
}

void _3do_PushSample(unsigned int val)
{
        while(!_sndal_Update(val))
        {
                Sleep(1);
        }
}

unsigned int VDLP_CLUTColNew(unsigned short pixel, const VDLLine &line)
{
        unsigned int col;

        if(!(pixel&0x7fff))return line.xBACKGROUND;

        col  = line.xCLUTB[(pixel&0x1f)]<<16;
        col = col|((int)line.xCLUTG[(pixel>>5)&0x1f]<<8);
        col = col|((int)line.xCLUTR[(pixel>>10)&0x1f]);

        return col;
}

__inline unsigned int VDLP_InterpColNew(unsigned short pixel, const VDLLine &line)
{
        unsigned int col;
                if(!(pixel>>15)) return  VDLP_CLUTColNew(pixel, line);

                col=(pixel&31)<<19;
                col=col|(((int)(pixel>>5)&31)<<11);
                col=col|(((int)(pixel>>10)&31)<<3);

                return col;
}

bool frame_complite;
void _vdl_FrameToScreenInterpolate(VDLFrame *frame, unsigned int *screen)
{
 int i,j;
 unsigned int *bufp1;
 unsigned int *bufp2;
 unsigned int rgb0,rgb1,rgb2,rgb3;

 unsigned short b00,b01,b10,b11;
 unsigned short g00,g01,g10,g11;
 unsigned short r00,r01,r10,r11;

        for(i=0;i<frame->srch;i++)
        {
                bufp2 = (unsigned int*)((unsigned char *)screen + (frame->srch-i-1)*frame->srcw*4*4);
                bufp1 = (unsigned int*)((unsigned char *)screen + (frame->srch-i-1)*frame->srcw*4*4+frame->srcw*4*2);
                if(frame->lines[i].xCLUTDMA&(1<<21))
                {
                        if(frame->lines[i].xOUTCONTROLL&(1<<25))
                        {
                                rgb0=VDLP_InterpColNew(frame->lines[i].line[0],frame->lines[i]);
                                rgb2=VDLP_InterpColNew(frame->lines[(i+1)%frame->srch].line[0],frame->lines[(i+1)%frame->srch]);
                                for(j=0;j<frame->srcw;j++)
                                {
                                        rgb1=VDLP_InterpColNew(frame->lines[i].line[(j+1)%frame->srcw],frame->lines[i]);
                                        rgb3=VDLP_InterpColNew(frame->lines[(i+1)%frame->srch].line[(j+1)%frame->srcw],frame->lines[(i+1)%frame->srch]);

                                        r00=(rgb0>>16)&0xff;
                                        r01=(rgb1>>16)&0xff;
                                        r10=(rgb2>>16)&0xff;
                                        r11=(rgb3>>16)&0xff;

                                        g00=(rgb0>>8)&0xff;
                                        g01=(rgb1>>8)&0xff;
                                        g10=(rgb2>>8)&0xff;
                                        g11=(rgb3>>8)&0xff;

                                        b00=(rgb0)&0xff;
                                        b01=(rgb1)&0xff;
                                        b10=(rgb2)&0xff;
                                        b11=(rgb3)&0xff;

                                        ((uint8*)bufp1)[0+0]=(b00);
                                        ((uint8*)bufp1)[1+0]=(g00);
                                        ((uint8*)bufp1)[2+0]=(r00);

                                        ((uint8*)bufp1)[3+0]=((uint8*)bufp1)[3+4]=((uint8*)bufp2)[3+0]=((uint8*)bufp2)[3+4]=0;

                                        ((uint8*)bufp1)[0+4]=(b00+b01)>>1;
                                        ((uint8*)bufp1)[1+4]=(g00+g01)>>1;
                                        ((uint8*)bufp1)[2+4]=(r00+r01)>>1;

                                        ((uint8*)bufp2)[0+0]=(b00+b10)>>1;
                                        ((uint8*)bufp2)[1+0]=(g00+g10)>>1;
                                        ((uint8*)bufp2)[2+0]=(r00+r10)>>1;


                                        if((b00&0xf0)==(b11&0xf0) &&
                                                ((b00&0xf0)==(b01&0xf0) || (b00&0xf0)==(b10&0xf0) || (b01&0xf0)!=(b10&0xf0)))((uint8*)bufp2)[0+4]=(b00+b11)>>1;
                                        else if((b01&0xf0)==(b10&0xf0) &&
                                                ((b01&0xf0)==(b00&0xf0) || (b01&0xf0)==(b11&0xf0) || (b00&0xf0)!=(b11&0xf0)))((uint8*)bufp2)[0+4]=(b10+b01)>>1;
                                        else
                                                ((uint8*)bufp2)[0+4]=(b10+b01+b11+b00)>>2;

                                        if((g00&0xf0)==(g11&0xf0) &&
                                                ((g00&0xf0)==(g10&0xf0) || (g00&0xf0)==(g01&0xf0) || (g01&0xf0)!=(g10&0xf0)))((uint8*)bufp2)[1+4]=(g00+g11)>>1;
                                        else if((g01&0xf0)==(g10&0xf0) &&
                                                ((g01&0xf0)==(g00&0xf0) || (g01&0xf0)==(g11&0xf0) || (g00&0xf0)!=(g11&0xf0)))((uint8*)bufp2)[1+4]=(g10+g01)>>1;
                                        else
                                                ((uint8*)bufp2)[1+4]=(g10+g01+g11+g00)>>2;

                                        if((r00&0xf0)==(r11&0xf0) &&
                                                ((r00&0xf0)==(r10&0xf0) || (r00&0xf0)==(r01&0xf0) || (r01&0xf0)!=(r00&0xf0)))((uint8*)bufp2)[2+4]=(r00+r11)>>1;
                                        else if((r01&0xf0)==(r10&0xf0) &&
                                                ((r01&0xf0)==(r00&0xf0) || (r01&0xf0)==(r11&0xf0) || (r00&0xf0)!=(r11&0xf0)))((uint8*)bufp2)[2+4]=(r10+r01)>>1;
                                        else
                                                ((uint8*)bufp2)[2+4]=(r10+r01+r11+r00)>>2;


                                        bufp1+=2;
                                        bufp2+=2;
                                        rgb0=rgb1;
                                        rgb2=rgb3;
                                }

                        }
                        else
                        {
                                rgb0=VDLP_InterpColNew(frame->lines[i].line[0],frame->lines[i]);
                                rgb2=VDLP_InterpColNew(frame->lines[(i+1)%frame->srch].line[0],frame->lines[(i+1)%frame->srch]);
                                for(j=0;j<frame->srcw;j++)
                                {

                                        rgb1=VDLP_CLUTColNew(frame->lines[i].line[(j+1)%frame->srcw],frame->lines[i]);
                                        rgb3=VDLP_CLUTColNew(frame->lines[(i+1)%frame->srch].line[(j+1)%frame->srcw],frame->lines[i]);


                                        r00=(rgb0>>16)&0xff;
                                        r01=(rgb1>>16)&0xff;
                                        r10=(rgb2>>16)&0xff;
                                        r11=(rgb3>>16)&0xff;

                                        g00=(rgb0>>8)&0xff;
                                        g01=(rgb1>>8)&0xff;
                                        g10=(rgb2>>8)&0xff;
                                        g11=(rgb3>>8)&0xff;

                                        b00=(rgb0)&0xff;
                                        b01=(rgb1)&0xff;
                                        b10=(rgb2)&0xff;
                                        b11=(rgb3)&0xff;

                                        ((uint8*)bufp1)[0+0]=(b00);
                                        ((uint8*)bufp1)[1+0]=(g00);
                                        ((uint8*)bufp1)[2+0]=(r00);

                                        ((uint8*)bufp1)[3+0]=((uint8*)bufp1)[3+4]=((uint8*)bufp2)[3+0]=((uint8*)bufp2)[3+4]=0;

                                        ((uint8*)bufp1)[0+4]=(b00+b01)>>1;
                                        ((uint8*)bufp1)[1+4]=(g00+g01)>>1;
                                        ((uint8*)bufp1)[2+4]=(r00+r01)>>1;

                                        ((uint8*)bufp2)[0+0]=(b00+b10)>>1;
                                        ((uint8*)bufp2)[1+0]=(g00+g10)>>1;
                                        ((uint8*)bufp2)[2+0]=(r00+r10)>>1;


                                        if((b00&0xf0)==(b11&0xf0) &&
                                                ((b00&0xf0)==(b01&0xf0) || (b00&0xf0)==(b10&0xf0) || (b01&0xf0)!=(b10&0xf0)))((uint8*)bufp2)[0+4]=(b00+b11)>>1;
                                        else if((b01&0xf0)==(b10&0xf0) &&
                                                ((b01&0xf0)==(b00&0xf0) || (b01&0xf0)==(b11&0xf0) || (b00&0xf0)!=(b11&0xf0)))((uint8*)bufp2)[0+4]=(b10+b01)>>1;
                                        else
                                                ((uint8*)bufp2)[0+4]=(b10+b01+b11+b00)>>2;

                                        if((g00&0xf0)==(g11&0xf0) &&
                                                ((g00&0xf0)==(g10&0xf0) || (g00&0xf0)==(g01&0xf0) || (g01&0xf0)!=(g10&0xf0)))((uint8*)bufp2)[1+4]=(g00+g11)>>1;
                                        else if((g01&0xf0)==(g10&0xf0) &&
                                                ((g01&0xf0)==(g00&0xf0) || (g01&0xf0)==(g11&0xf0) || (g00&0xf0)!=(g11&0xf0)))((uint8*)bufp2)[1+4]=(g10+g01)>>1;
                                        else
                                                ((uint8*)bufp2)[1+4]=(g10+g01+g11+g00)>>2;

                                        if((r00&0xf0)==(r11&0xf0) &&
                                                ((r00&0xf0)==(r10&0xf0) || (r00&0xf0)==(r01&0xf0) || (r01&0xf0)!=(r00&0xf0)))((uint8*)bufp2)[2+4]=(r00+r11)>>1;
                                        else if((r01&0xf0)==(r10&0xf0) &&
                                                ((r01&0xf0)==(r00&0xf0) || (r01&0xf0)==(r11&0xf0) || (r00&0xf0)!=(r11&0xf0)))((uint8*)bufp2)[2+4]=(r10+r01)>>1;
                                        else
                                                ((uint8*)bufp2)[2+4]=(r10+r01+r11+r00)>>2;

                                        bufp1+=2;
                                        bufp2+=2;
                                        rgb0=rgb1;
                                        rgb2=rgb3;
                                }
                        }
                }
                else
                {
                        for(j=0;j<(frame->srcw*2);j++) bufp1[j]=frame->lines[i].xBACKGROUND;
                        for(j=0;j<(frame->srcw*2);j++) bufp2[j]=frame->lines[i].xBACKGROUND;
                }
        }
        frame_complite=true;
}

VDLFrame* _3do_SwapFrame(VDLFrame* frame)
{
        _vdl_FrameToScreenInterpolate(_3do_Frame,(unsigned int*)emu_buff);

        //paint!

        return _3do_Frame;
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
#include <QHash>
#include <QStringList>
QHash<int,bool> _emu_vk_table;
volatile unsigned int pbus_buffer[5];

void _emulator_key_state_reset()
{
    _emu_vk_table.clear();
}

void _emulator_key_state(int code, bool state)
{
    _emu_vk_table[code]=state;
}

QString __Getpref(QString val, int &start, char sep)
{
 QString str="";
    if(start>=val.length())return str;
    while(val[start]!=sep)
    {
        if(val.length()<=start)break;
        str+=val[start++];
    }
    start++;
    return str;
}

bool _joy_get_status(QString el, QList<Gamepad_device*> &joys)
{
 bool rv,low;
 int ind,i,dev;
 char tmp;
    if(el[0]=='k')
    {
        el=el.remove(0,1);
        ind=el.toInt(&rv,16);
        if(_emu_vk_table.end()==_emu_vk_table.find(ind))
        {
            return false;
        }
        else
        {
            return _emu_vk_table[ind];
        }
    }
    else if(el[0]=='j')
    {
        i=1;
        dev=0;
        while(el[i]<='9' && el[i]>='0' && i<el.length())
        {
            dev*=10;
            tmp=el[i].toLatin1();
            dev+=tmp-'0';
            i++;
        }
        if(joys.count()<=dev)return false;
        if(i>=el.length())return false;
        if(el[i]=='b')
        {
            el=el.remove(0,i+1);
            ind=el.toInt(&rv,10);
            if(joys[dev]->numButtons<=ind)return false;
            return joys[dev]->buttonStates[ind];
        }
        else if(el[i]=='a')
        {
            el=el.remove(0,i+1);
            if(!el.length())return false;
            if(el[el.length()-1]=='-')low=true;
            else low=false;
            el=el.remove(el.length()-1,1);
            ind=el.toInt(&rv,10);
            if(joys[dev]->numAxes<=ind)return false;
            if(low)
            {
                if(joys[dev]->axisStates[ind]<-0.5)return true;
            }
            else
            {
                if(joys[dev]->axisStates[ind]>0.5)return true;
            }
        }
    }
    return false;
}

void _emulator_fill_pbus(QStringList &list)
{
 int i;
 int jcnt=Gamepad_numDevices();
 QList<Gamepad_device*> joys;
 QString str,tmp;
 int index;
 unsigned int joybin;


    for(i=0;i<jcnt;i++)
    {
        joys.append(Gamepad_deviceAtIndex(i));
    }

    for(i=0;i<list.count();i++)
    {
        if(i>=8)break;

        index=0;
        str=list[i];
        joybin=0x8000;

        tmp=__Getpref(str,index,'=');
        if(tmp!="Joy")continue;

        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x800;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x1000;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x200;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x400;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x4;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x8;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x10;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x20;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x100;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x80;
        tmp=__Getpref(str,index,',');
        if(_joy_get_status(tmp,joys))joybin|=0x40;

        if(i&1)pbus_buffer[i>>1]=(pbus_buffer[i>>1]&0xffff0000)|joybin;
        else pbus_buffer[i>>1]=(pbus_buffer[i>>1]&0xffff)|(joybin<<16);
    }
    for(;i<10;i++)
    {
        if(i&1)pbus_buffer[i>>1]=(pbus_buffer[i>>1]&0xffff0000)|0xffff;
        else pbus_buffer[i>>1]=(pbus_buffer[i>>1]&0xffff)|(0xffff<<16);
    }
}

unsigned int _3do_GetPBUSLeinght()
{
        return 20;
}
unsigned char* _3do_GetPBUSData()
{        
        return (unsigned char*)pbus_buffer;
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------

#include "cd/discdata.h"
QFile game_hand;
unsigned int _game_sector_size,_game_data_offset;

#define MAGIC_NUMBER_LOW        0x5A5A5A01
#define MAGIC_NUMBER_HEIGHT     0x00015A5A

void _emulation_game_open(QString game)
{
 unsigned int tmp,cur_pos;
 char data[3000];
 FS3DO_DiscLabel *lp;

    if(game_hand.isOpen())
    {
        game_hand.close();
    }
    _game_sector_size=0;
    game_hand.setFileName(game);    
    if(!game_hand.open(QIODevice::ReadOnly)) return;


    while(game_hand.read((char*)&tmp,4)==4)
    {
        if(tmp==MAGIC_NUMBER_LOW)
        {
           if(game_hand.read((char*)&tmp,4)!=4) break;

           if((tmp&0xffff)==(MAGIC_NUMBER_HEIGHT&0xffff))
           {
               cur_pos=game_hand.pos();
               cur_pos-=8;
               game_hand.seek(cur_pos);

               _game_data_offset=cur_pos;
               game_hand.read(data,sizeof(FS3DO_DiscLabel));

               lp=(FS3DO_DiscLabel*)data;
               _game_sector_size=lp->sectorCount;

               game_hand.seek(_game_data_offset);

               return;

           }
        }
    }
    game_hand.close();

}

void _emulation_game_close()
{
    if(game_hand.isOpen())
    {
        game_hand.close();
    }
}

int _emulation_game_sector_count()
{
    return _game_sector_size;
}

void _emulation_game_on_sector(int ind)
{
    if(_game_sector_size)
    {
        if(_game_data_offset%2048)
            game_hand.seek(ind*2352+_game_data_offset);
        else
            game_hand.seek(ind*2048+_game_data_offset);
    }
}

int _emulation_game_read(char *data)
{    
 char buff[512];
    if(_game_sector_size)
    {
        game_hand.read(data,2048);
        if(_game_data_offset%2048) game_hand.read(buff,2352-2048);
    }
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------


void* with_freedo_interface(int proc, void *datum)
{
        switch(proc)
        {
         case EXT_READ_ROMS:
                memcpy(datum,bios_buff,1024*1024*2);
                break;
         case EXT_READ_NVRAM:
                _3do_LoadNVRAM(datum);
                break;
         case EXT_WRITE_NVRAM:
                _3do_SaveNVRAM(datum);
                break;
         case EXT_SWAPFRAME:
                return _3do_SwapFrame((VDLFrame*)datum);
         case EXT_PUSH_SAMPLE:
                _3do_PushSample((size_t)datum);
                break;
         case EXT_GET_PBUSLEN:
                return (void*)_3do_GetPBUSLeinght();
         case EXT_GETP_PBUSDATA:
                return _3do_GetPBUSData();
         case EXT_KPRINT:
                break;
         case EXT_DEBUG_PRINT:
                break;
         case EXT_READ2048:
                _emulation_game_read((char*)datum);
                break;
         case EXT_GET_DISC_SIZE:
                return (void*)_emulation_game_sector_count();
         case EXT_ON_SECTOR:
                _emulation_game_on_sector((ptrdiff_t)datum);
                break;
        };
        return NULL;
}
//----------------------------------------------------------

EmuThread::EmuThread()
{
    stopflag=false;
    execflag=false;
    inoffstate=true;
    pausestate=false;
    fver2=fver1=0;
}

void EmuThread::stop()
{
    stopflag=true;
}

void EmuThread::RunStopCPU()
{
    execflag=!execflag;
    if(!execflag)pausestate=false;
}

void EmuThread::PauseResumeCPU()
{
    pausestate=!pausestate;
}

void EmuThread::SelectGame(QString game)
{
    game_name=game;
}

void EmuThread::SelectBios(QString name, int i)
{
    _emulator_LoadBios(name,i);
}

void EmuThread::UpdateFrameComplited()
{
    fver2=fver1;
}

void EmuThread::run()
{

    frame_complite=false;
    memset((void*)pbus_buffer,0xff,20); //!!!


    while(!stopflag) //флаг завершения потока
    {
        if(execflag) //управляет ходом эмуляции
        {
            if(inoffstate) //признак, что эмулятор остановлен
            {
                _emulation_game_open(game_name);
                _freedo_Interface(FDP_INIT, (void*)&with_freedo_interface);
                inoffstate=false;
            }

            if(!pausestate) //приостановка процесса эмуляции
                _freedo_Interface(FDP_DO_EXECFRAME,_3do_Frame);
            else usleep(100);

            if(frame_complite)
            {
                if(fver2==fver1)
                {
                    emit SendPaintMess();
                    frame_complite=false;
                    fver1++;
                }
            }
        }
        else
        {
            if(!inoffstate)
            {
                _freedo_Interface(FDP_DESTROY,NULL);
                _emulation_game_close();
                _emulator_set_baner();
                emit SendPaintMess();
                inoffstate=true;
            }
            usleep(100);
        }
    }

    if(!inoffstate)
    {
        _freedo_Interface(FDP_DESTROY,NULL);
        _emulation_game_close();
    }

}
