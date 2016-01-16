// OutSound1.cpp: implementation of the COutSound class.
//
//////////////////////////////////////////////////////////////////////

#include "OutSound.h"

#ifndef NULL
#define NULL 0
#endif

#include "AL/al.h"
#include "AL/alc.h"

ALCdevice *pDevice;
ALCcontext *pContext;

#define BLOCK_SIZE  (1048)
#define BLOCK_COUNT 8
#define FREQ_VAL	44100

int FREQ_VAL_FREE=FREQ_VAL;

ALuint	sndalBuffs[BLOCK_COUNT], sndalSource;
unsigned int sndalQueue[BLOCK_SIZE], sndalCount;

bool SwapChannels=false;

void _sndal_SetVolume(float val)
{
	alSourcef (sndalSource, AL_GAIN,    val);
}
void _sndal_SetSpeed(float val)
{
	//alSourcef (sndalSource, AL_PITCH,    val);
        FREQ_VAL_FREE=FREQ_VAL*val;
}
void _sndal_SwapLR(bool val)
{
	SwapChannels=!val;
}

bool _sndal_Init()
{
 int i;
 
  // ������� ���������.
  ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

  // �������� ���������.
  ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

  // ���������� ���������. (������ 3 �������� � ����������� ���, ��������� 3 � �������)
  ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

  ALfloat SourcePos[] = { 0, 0, 1 };	

    // ��������� �������� �� ��������� ����������
    pDevice = alcOpenDevice(NULL);
    // �������� �� ������
    if (!pDevice)
    {
        return false;
    }
    // ������� �������� ����������
    pContext = alcCreateContext(pDevice, NULL);
    if ( alcGetError(pDevice) != ALC_NO_ERROR) return false;
  
    // ������ �������� �������
    alcMakeContextCurrent(pContext);

    // ������������� ��������� ���������
    // �������
    alListenerfv(AL_POSITION,    ListenerPos);
    // ��������
    alListenerfv(AL_VELOCITY,    ListenerVel);
    // ����������
    alListenerfv(AL_ORIENTATION, ListenerOri);

	sndalCount=0;
	alGenSources ( 1, &sndalSource );

	alSourcef (sndalSource, AL_PITCH,    1.0f);	//��������
    alSourcef (sndalSource, AL_GAIN,    1.0f); //���������
    alSourcefv(sndalSource, AL_POSITION,  SourcePos);
    alSourcefv(sndalSource, AL_VELOCITY,  ListenerVel);
    alSourcei (sndalSource, AL_LOOPING,  false);

	alGenBuffers(BLOCK_COUNT, sndalBuffs);
	
	sndalQueue[0]=0;

	for(i=0;i<BLOCK_COUNT;i++)
	{
			alBufferData(sndalBuffs[i], AL_FORMAT_STEREO16, sndalQueue, 4, FREQ_VAL_FREE);
	}

	alSourceQueueBuffers(sndalSource, BLOCK_COUNT, sndalBuffs);
	
	alSourcePlay(sndalSource);

    return true;

}
static int      Processed = 0;
bool _sndal_Update(unsigned int RL)
{
 	if(sndalCount<(BLOCK_SIZE-1))
	{
                if(SwapChannels)RL=(RL>>16)|(RL<<16);
        	sndalQueue[sndalCount++]=RL;
		return true;
	}
	else
	{
                ALuint      BufID;

		Processed=0;
		alGetSourcei(sndalSource, AL_BUFFERS_PROCESSED, &Processed);
		if(!Processed)
		{
			return false;
		}

                if(SwapChannels)RL=(RL>>16)|(RL<<16);
		sndalQueue[sndalCount]=RL;
                
                sndalCount=0;
		alSourceUnqueueBuffers(sndalSource, 1, &BufID);
		alBufferData(BufID, AL_FORMAT_STEREO16, sndalQueue, BLOCK_SIZE<<2, FREQ_VAL_FREE);
       		alSourceQueueBuffers(sndalSource, 1, &BufID);

		alGetSourcei(sndalSource, AL_SOURCE_STATE, (int*)&BufID);
		if(BufID!=AL_PLAYING)
		{
			alSourcePlay(sndalSource);
		}
	}
        return true;
}



void _sndal_Destroy()
{
  alSourceStop ( sndalSource );
  // ������� ��� �������
  alDeleteBuffers(BLOCK_COUNT, sndalBuffs);
  alDeleteSources(1, &sndalSource);
  // ��������� ������� ��������
  alcMakeContextCurrent(NULL);
  // ���������� ��������
  alcDestroyContext(pContext);
  // ��������� �������� ����������
  alcCloseDevice(pDevice);
}

int _sndal_BufUsage()
{       
	return (100*Processed)/BLOCK_COUNT;
}
