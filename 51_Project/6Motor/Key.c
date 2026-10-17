/* 调用头文件 */
#include "reg52.h"
#include "Delay.h"

/* 引脚定义 */
sbit KEY1 = P3^1;
sbit KEY2 = P3^0;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;


/**
  * @brief	按键扫描及处理
  * @param	无
  * @retval key_num 按下按键的编号
			0		没有按键被按下
			1		KEY1被按下
			2		KEY2被按下
			3		KEY3被按下
			4		KEY4被按下
*/
int KEY_Scanf(void)
{
	static unsigned char key_num = 1;	// 静态局部变量	
	KEY1 = 1;	// 51单片机读取引脚电平值，最好是先将对应引脚设置为高电平
	KEY2 = 1;
	KEY3 = 1;
	KEY4 = 1;
	if(KEY1 == 0)		/* KEY1 */
	{
		Delay_ms(10);	// 消抖
		if(KEY1 == 0)
		{
			key_num = 1;
		}
		while(KEY1 == 0);// 等待按键释放
	}
	else if(KEY2 == 0)		/* KEY2 */
	{
		Delay_ms(10);	// 消抖
		if(KEY2 == 0)
		{
			key_num = 2;
		}
		while(KEY2 == 0);// 等待按键释放
	}
	else if(KEY3 == 0)		/* KEY3 */
	{
		Delay_ms(10);	// 消抖
		if(KEY3 == 0)
		{
			key_num = 3;
		}
		while(KEY3 == 0);// 等待按键释放
	}
	else if(KEY4 == 0)		/* KEY4 */
	{
		Delay_ms(10);	// 消抖
		if(KEY4 == 0)
		{
			key_num = 4;
		}
		while(KEY4 == 0);// 等待按键释放
	}
	return key_num;
}