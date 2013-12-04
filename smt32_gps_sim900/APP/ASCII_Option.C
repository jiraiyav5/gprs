#include "includes.h"
#include "ASCII_Option.h"

 
/*********************************************************************
 * Function:		void uitoa(INT16U Value, INT8U *Buffer)
 *
 * PreCondition:	无符号16位整型数转换为ASCII码的算法，例如：123456==>“123456”
 *
 * Input:			Value: 16为整形数
 *					Buffer: 转换后的ASCII字符串存放的位置
 ********************************************************************/
void uitoa(INT16U Value, INT8U *Buffer)
{
	INT8U i;
	INT16U Digit;
	INT16U Divisor;
	INT8U Printed = 0;

	if(Value)
	{
		for(i = 0, Divisor = 10000; i < 5; i++)
		{
			Digit = Value/Divisor;
			if(Digit || Printed)
			{
				*Buffer++ = '0' + Digit;
				Value -= Digit*Divisor;
				Printed = 1;
			}
			Divisor /= 10;
		}
	}
	else
	{
		*Buffer++ = '0';
	}

	*Buffer = '\0';
}			    

// 无符号32位整型数转换为ASCII码的算法，例如：12345678==>“12345678” ,转换结果保存到Buffer
void ultoa(INT32U Value, INT8U *Buffer)
{
	INT8U i;
	INT32U Digit;
	INT32U Divisor;
	INT8U Printed = 0;

	if(Value)
	{
		for(i = 0, Divisor = 1000000000; i < 10; i++)
		{
			Digit = Value/Divisor;
			if(Digit || Printed)
			{
				*Buffer++ = '0' + Digit;
				Value -= Digit*Divisor;
				Printed = 1;
			}
			Divisor /= 10;
		}
	}
	else
	{
		*Buffer++ = '0';
	}

	*Buffer = '\0';
}

//把一个8位整型数的高4位转换为‘0’~‘F’的字符。
INT8U btohexa_high(INT8U b)
{
	b >>= 4;
	return (b>0x9u) ? b+'A'-10:b+'0';
}

//把一个8位整型数的低4位转换为‘0’~‘F’的字符。
INT8U btohexa_low(INT8U b)
{
	b &= 0x0F;
	return (b>9u) ? b+'A'-10:b+'0';
}
















