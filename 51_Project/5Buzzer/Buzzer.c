/* 调用头文件 */
#include "reg52.h"
#include "Delay.h"

/* 引脚定义 */
sbit Buzzer = P2^5;

/**
  * @brief	设置蜂鸣器发生频率为5000Hz
  * @param	time	发声时间（秒）
  * @retval	无
*/
void Buzzer_5000Hz(unsigned char time)
{
	unsigned int i,j;
	for(i = 0;i < time;i++)	// 约响time秒（方波周期约为200us  5000Hz）
	{
		for(j = 0;j < 10000;j++)
		{
			Buzzer = ~Buzzer;			// 蜂鸣器引脚状态翻转
			Delay_10us(10);
		}
	}
}


/**
  * @brief	设置蜂鸣器发生频率为500Hz
  * @param	time	发声时间（秒）
  * @retval	无
*/
void Buzzer_500Hz(unsigned char time)
{
	unsigned int i,j;
	for(i = 0;i < time;i++)	// 约响time秒（方波周期约为2ms  500Hz）
	{	
		for(j = 0;j < 1000;j++)
		{
			Buzzer = ~Buzzer;			// 蜂鸣器引脚状态翻转
			Delay_ms(1);
		}
	}
}


/**
  * @brief	关闭蜂鸣器
  * @param	无
  * @retval	无
*/
void Buzzer_Off(void)
{
	Buzzer = 0;
}

