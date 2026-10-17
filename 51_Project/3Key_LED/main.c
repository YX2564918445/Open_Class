/* 调用头文件 */
#include "reg52.h"
#include <intrins.h>	// 提供_nop_()函数

/* 引脚定义 */
// LED1 -- LED4
sbit LED1 = P2^0;
sbit LED2 = P2^1;
sbit LED3 = P2^2;
sbit LED4 = P2^3;
// KEY1 -- KEY4
sbit KEY1 = P3^1;
sbit KEY2 = P3^0;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;

/* 函数声明 */
void Delay_ms(unsigned int ms);
void KEY_Scanf(void);

/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
    while(1)
    {
		KEY_Scanf();	// 按键扫描及处理
    }
}


/**
  * @brief	延时函数：产生一段时间的延时效果
  * @param	ms 想要产生延时的毫秒数
  * @retval	无
*/
void Delay_ms(unsigned int ms)
{
	unsigned int i,j;
	for(i = ms;i > 0;i--)
	{
		// 11.0592MHz晶振，921个机器周期约为1ms
		for(j = 110;j > 0;j--)	
		{
			_nop_();		// 空操作，消耗1个机器周期
		}
	}
}


/**
  * @brief	按键扫描及逻辑处理
  * @param	无
  * @retval 无
*/
void KEY_Scanf(void)
{
	// 51单片机读取引脚电平值，最好是先将对应引脚设置为高电平
	KEY1 = 1;
	KEY2 = 1;
	KEY3 = 1;
	KEY4 = 1;
	// KEY1
	if(KEY1 == 0)
	{
		Delay_ms(10);	// 消抖
		if(KEY1 == 0)
		{
			LED1 = ~LED1;// LED1 状态取反
		}
		while(KEY1 == 0);// 等待按键释放
	}
	// KEY2
	else if(KEY2 == 0)
	{
		Delay_ms(10);	// 消抖
		if(KEY2 == 0)
		{
			LED2 = ~LED2;// LED2 状态取反
		}
		while(KEY2 == 0);// 等待按键释放
	}
	// KEY3
	else if(KEY3 == 0)
	{
		Delay_ms(10);	// 消抖
		if(KEY3 == 0)
		{
			LED3 = ~LED3;// LED3 状态取反
		}
		while(KEY3 == 0);// 等待按键释放
	}
	// KEY4
	else if(KEY4 == 0)
	{
		Delay_ms(10);	// 消抖
		if(KEY4 == 0)
		{
			LED4 = ~LED4;// LED4 状态取反
		}
		while(KEY4 == 0);// 等待按键释放
	}
}
