#include "freedoconfig.h"
#include "bitop.h"
#include "arm.h"


uint32 BitReaderBig::Read()
{
 const static uint8 mas[]={0,1,3,7,15,31,63,127,255};
 uint32 retval=0;
 int32 bitcnt=bitset;
 	if(!buf)return retval;
 	if((8-bitpoint)>bitset)
 	{
 		retval=_mem_read8(buf+(point^3));
 		retval>>=8-bitpoint-bitset;
 		retval&=mas[bitset];
 		bitpoint+=bitset;
 		return retval;
 	}
 	if(bitpoint)
 	{
 		retval=_mem_read8(buf+(point^3))&mas[8-bitpoint];
		point++;
 		bitcnt-=8-bitpoint;
 	}
 	while(bitcnt>=8)
 	{
		retval<<=8;
 		retval|=_mem_read8(buf+(point^3));
        point++;
 		bitcnt-=8;
 	}
 	if(bitcnt)
 	{
		retval<<=bitcnt;
 		retval|=_mem_read8(buf+(point^3))>>(8-bitcnt); 		
 	}
 	bitpoint=bitcnt;

 	return retval; 	 	
}

uint32 BitReaderBig::Read(uint8 bits)
{
	SetBitRate(bits);
	return Read();
}

