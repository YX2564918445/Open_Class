/* 调用头文件 */
#include "reg52.h"

/* 引脚定义 */
sbit LED1 = P2^0;
sbit LED2 = P2^1;
sbit LED3 = P2^2;
sbit LED4 = P2^3;
sbit LED5 = P2^4;
sbit LED6 = P2^5;
sbit LED7 = P2^6;
sbit LED8 = P2^7;

/**
  * @brief	主程序
  * @param	无
  * @retval	无
*/
int main(void)
{
	// LED共阳极接法，引脚输出低电平产生电势差-->LED亮
	LED1 = 0;	// LED1亮
	LED2 = 1;	// LED2灭
	LED3 = 0;	// LED3亮
	LED4 = 1;	// LED4灭
	LED5 = 0;	// LED5亮
	LED6 = 1;	// LED6灭
	LED7 = 0;	// LED7亮
	LED8 = 1;	// LED8灭
	while(1)
	{
	
	}
}