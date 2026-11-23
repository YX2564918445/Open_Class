/* 调用头文件 */
#include "reg52.h"
#include "Delay.h"

/* 引脚定义 */
// 注意：P2.5 上还挂着蜂鸣器（插上跳线帽才会响），本讲只动 P2.0 和 P2.1，
// 不会踩到蜂鸣器；如果改成 8 颗灯一起呼吸，第 5 颗亮的时候蜂鸣器会跟着叫
sbit LED_BREATH = P2^0;		// 呼吸灯 D1，共阳极，低电平点亮
sbit LED_BLINK  = P2^1;		// 另一颗灯 D2，由同一个定时器每 500ms 翻转一次


/* 变量定义 */
volatile unsigned char pwm_cnt = 0;			// PWM 计数器，0~99 循环
volatile unsigned char duty = 0;			// 占空比 0~100：数值越大，灯越亮
unsigned char step_cnt = 0;					// 数 PWM 周期用的，只在中断里用


/**
  * @brief	定时器0中断服务函数：每 100us 进来一次
  * @param	无
  * @retval	无
  * @note	本函数做两件事：① 产生 PWM 波形；② 顺便数 500ms 让另一颗灯闪。
  			同一个定时器可以同时管好几个"节拍"，这是它比软件延时强的地方
*/
void Timer0_ISR(void) interrupt 1
{
	// 模式1 不会自动重装初值，每次进来都要自己写
	// 100us 的初值：100us / 1.085us ≈ 92 个机器周期，65536 - 92 = 65444 = 0xFFA4
	TH0 = 0xFF;
	TL0 = 0xA4;

	/* 一、用一个 0~99 的计数器循环，产生 PWM 波形 */
	pwm_cnt++;
	if(pwm_cnt >= 100)			// 100 个节拍 × 100us = 10ms，也就是 100Hz
	{
		pwm_cnt = 0;

		// 下面这几句每 10ms 才执行一次，放在这个 if 里面是为了省时间：
		// 中断函数要尽量短，能少做一次就少做一次
		step_cnt++;
		if(step_cnt >= 50)		// 50 × 10ms = 500ms
		{
			step_cnt = 0;
			LED_BLINK = !LED_BLINK;		// 另一颗灯 0.5 秒亮、0.5 秒灭
		}
	}

	if(pwm_cnt < duty)			// 计数器小于占空比 → 输出低电平 → 灯亮
	{
		LED_BREATH = 0;
	}
	else						// 否则输出高电平 → 灯灭
	{
		LED_BREATH = 1;
	}
}


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned char dir = 1;		// 1 = 正在变亮，0 = 正在变暗

	/* 定时器0 初始化：每 100us 中断一次，作为 PWM 的时间基准 */
	TMOD &= 0xF0;		// TMOD 高 4 位是定时器1 的设置，原样保留；低 4 位清零
	TMOD |= 0x01;		// 定时器0 → 模式1（16 位定时器），用内部时钟定时
	TH0 = 0xFF;			// 100us 初值：100us / 1.085us ≈ 92，65536 - 92 = 0xFFA4
	TL0 = 0xA4;
	ET0 = 1;			// 允许定时器0 中断
	EA  = 1;			// 打开总中断
	TR0 = 1;			// 启动定时器0

	while(1)
	{
		// 主循环只负责"改参数"：每 10ms 让占空比变化 1 级。
		// 这里的延时只决定"变亮变暗的快慢"，PWM 的周期和占空比由定时器保证，
		// 所以调节奏完全不会让灯闪 —— 这正是硬件 PWM 比第 6 讲软件 PWM 强的地方
		Delay_ms(10);

		if(dir == 1)				// 正在变亮
		{
			duty++;
			if(duty >= 100)			// 到最亮，转为变暗
			{
				dir = 0;
			}
		}
		else						// 正在变暗
		{
			duty--;
			if(duty == 0)			// 到最暗，转为变亮
			{
				dir = 1;
			}
		}
	}
}