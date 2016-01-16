#ifndef PLAY_BY_SAMPLE_INTERFACE
#define PLAY_BY_SAMPLE_INTERFACE

	int  _sndal_BufUsage();

	bool _sndal_Init();
        bool _sndal_Update(unsigned int RL);  //return false if no buff space
	void _sndal_Destroy();

	void _sndal_SetVolume(float val);
	void _sndal_SetSpeed(float val);
	void _sndal_SwapLR(bool val);


#endif
