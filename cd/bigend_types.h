#ifndef BIGEND_TYPES_HEADER
#define BIGEND_TYPES_HEADER
#include "../core/types.h"

__inline uint32 endswap(uint32 x)
{
	return (x>>24) | ((x>>8)&0x0000FF00L) | ((x&0x0000FF00L)<<8) | (x<<24);
}

class uint32b
{
private:
        uint32 data;
public:
        uint32b()
        {
          data=0;
        };

        uint32b(const uint32b &num)
        {
           data=num.data;
        };

        uint32b(const uint32 &num)
        {
           data=endswap(num);
        };

        uint32b& operator=(const uint32 &x)
        {
               data=endswap(x);
               return *this;
        };

        uint32b& operator+=(const uint32 &x)
        {
               data=endswap(endswap(data)+x);
               return *this;
        };

        uint32b& operator-=(const uint32 &x)
        {
               data=endswap(endswap(data)-x);
               return *this;
        };

        uint32b& operator*=(const uint32 &x)
        {
               data=endswap(endswap(data)*x);
               return *this;
        };

        uint32b& operator/=(const uint32 &x)
        {
               data=endswap(endswap(data)/x);
               return *this;
        };

        uint32b& operator<<=(const uint32 &x)
        {
               data=endswap(endswap(data)<<x);
               return *this;
        };

        uint32b& operator>>=(const uint32 &x)
        {
               data=endswap(endswap(data)>>x);
               return *this;
        };

        uint32b& operator%=(const uint32 &x)
        {
               data=endswap(endswap(data)%x);
               return *this;
        };

        uint32b& operator&=(const uint32 &x)
        {
               data=endswap(endswap(data)&x);
               return *this;
        };

        uint32b& operator|=(const uint32 &x)
        {
               data=endswap(endswap(data)|x);
               return *this;
        };

        uint32b& operator^=(const uint32 &x)
        {
               data=endswap(endswap(data)^x);
               return *this;
        };

        operator uint32()
        {
               return endswap(data);
        };


};

class int32b
{
private:
        int32 data;
public:
        int32b()
        {
          data=0;
        };

        int32b(const int32b &num)
        {
           data=num.data;
        };

        int32b(const int32 &num)
        {
           data=endswap(num);
        };

        int32b& operator=(const int32 &x)
        {
               data=endswap(x);
               return *this;
        };

        int32b& operator+=(const int32 &x)
        {
               data=endswap(endswap(data)+x);
               return *this;
        };

        int32b& operator-=(const int32 &x)
        {
               data=endswap(endswap(data)-x);
               return *this;
        };

        int32b& operator*=(const int32 &x)
        {
               data=endswap(endswap(data)*x);
               return *this;
        };

        int32b& operator/=(const int32 &x)
        {
               data=endswap(endswap(data)/x);
               return *this;
        };

        int32b& operator<<=(const int32 &x)
        {
               data=endswap(endswap(data)<<x);
               return *this;
        };

        int32b& operator>>=(const int32 &x)
        {
               data=endswap(endswap(data)>>x);
               return *this;
        };

        int32b& operator%=(const int32 &x)
        {
               data=endswap(endswap(data)%x);
               return *this;
        };

        int32b& operator&=(const int32 &x)
        {
               data=endswap(endswap(data)&x);
               return *this;
        };

        int32b& operator|=(const int32 &x)
        {
               data=endswap(endswap(data)|x);
               return *this;
        };

        int32b& operator^=(const int32 &x)
        {
               data=endswap(endswap(data)^x);
               return *this;
        };

        operator int32()
        {
               return endswap(data);
        };


};

typedef int32b	Item;


#endif
