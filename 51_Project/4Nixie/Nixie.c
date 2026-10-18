/* 调用头文件 */
#include "reg52.h"
#include "Nixie.h"

/* 引脚定义 */
#define SMG_DX P0		// 段选
sbit SMG_A = P2^2;		// 38译码器控制引脚
sbit SMG_B = P2^3;
sbit SMG_C = P2^4;


/**
  * @brief	在数码管指定位置显示一个字符
  * @param	position  指定位置（位）从右向左依次是1~8
			smgd_code 显示的字符
  * @retval	无
*/
void SMG_Display(unsigned char position,unsigned char smgd_code)
{
	switch(position)	// 判断以及发送位选数据
	{
		case 1: SMG_C=1;SMG_B=1;SMG_A=1;break;
		case 2: SMG_C=1;SMG_B=1;SMG_A=0;break;
		case 3: SMG_C=1;SMG_B=0;SMG_A=1;break;
		case 4: SMG_C=1;SMG_B=0;SMG_A=0;break;
		case 5: SMG_C=0;SMG_B=1;SMG_A=1;break;
		case 6: SMG_C=0;SMG_B=1;SMG_A=0;break;
		case 7: SMG_C=0;SMG_B=0;SMG_A=1;break;
		case 8: SMG_C=0;SMG_B=0;SMG_A=0;break;
	}
	SMG_DX = smgd_code;	// 发送段选数据
}