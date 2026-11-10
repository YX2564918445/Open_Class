/* 调用头文件 */
#include "reg52.h"
#include <intrins.h>	// 提供_nop_()函数

/* 引脚端口宏定义 */
// LED接在P2.0~P2.7，共阳极，低电平点亮
#define LED P2

/* 函数声明 */
void Delay_ms(unsigned int ms);

/**
  * @brief	外部中断0服务函数（下降沿触发）
  * @param	无
  * @retval	无
*/
void EX0_ISR(void) interrupt 0
{
	unsigned char j;
	/* 所有LED闪烁3次（亮500ms→灭500ms） */
	for(j = 0;j < 3;j++)
	{
		LED = 0x00;			// 全部LED点亮
		Delay_ms(500);
		LED = 0xFF;			// 全部LED熄灭
		Delay_ms(500);
	}
}


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned char i,led;

	led = 0x01;		// 0000 0001 作为移位的基础

	/* 初始化外部中断0 */
	IT0 = 1;		// 外部中断0 下降沿触发
	EX0 = 1;		// 使能外部中断0
	EA  = 1;		// 开启全局中断

	while(1)
	{
		// LED1→LED8（左移）
		for(i = 0;i < 8;i++)
		{
			LED = ~(led << i);
			Delay_ms(500);			// 保证每个LED点亮500ms
		}
		// LED7→LED2（右移）（避免两头LED重复点亮）
		for(i = 6;i > 0;i--)
		{
			LED = ~(led << i);
			Delay_ms(500);			// 保证每个LED点亮500ms
		}
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