/* 调用头文件 */
#include "reg52.h"
#include "Nixie.h"
#include "Delay.h"

/* 引脚端口宏定义 */
// LED接在P2.0~P2.7，共阳极，低电平点亮
#define LED P2

/* 变量定义 */
unsigned char smgd_code[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
							0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};	// 共阴极数码管0~F段码表


/**
  * @brief	外部中断1服务函数（低电平触发）
  * @param	无
  * @retval	无
*/
void EX1_ISR(void) interrupt 2
{
	LED = 0x00;			// 全部LED点亮
	Delay_ms(200);
	LED = 0xFF;			// 全部LED熄灭
	Delay_ms(200);
	// 低电平触发：P3.3保持低电平时CPU反复进入此ISR，形成连续闪烁
}


/**
  * @brief	将秒数值转换为数码管显示缓冲区（右对齐，空位不亮）
  * @param	sec  当前秒数值
  * @param	buf  显示缓冲区（buf[0]对应位置8，buf[7]对应位置1）
  * @retval	无
*/
void Update_DispBuf(unsigned int sec, unsigned char *buf)
{
	unsigned char i;
	unsigned int temp = sec;
	for(i = 0; i < 8; i++)
	{
		if(temp == 0 && i > 0)
		{
			buf[7 - i] = 0x00;		// 空位不亮
		}
		else
		{
			buf[7 - i] = smgd_code[temp % 10];
		}
		temp /= 10;
	}
}


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned int seconds = 0;		// 秒表计数值
	unsigned char disp_buf[8];		// 显示缓冲区
	unsigned char i;
	unsigned int t;

	/* 初始化外部中断1 */
	IT1 = 0;		// 外部中断1 低电平触发
	EX1 = 1;		// 使能外部中断1
	EA  = 1;		// 开启全局中断

	while(1)
	{
		Update_DispBuf(seconds, disp_buf);

		// 动态扫描刷新 ~1秒
		for(t = 0; t < 125; t++)
		{
			for(i = 0; i < 8; i++)
			{
				SMG_Display(i + 1, disp_buf[i]);	// 按位发送显示数据
				Delay_ms(1);						// 等待显示稳定
				SMG_Display(i + 1, 0x00);			// 消隐
			}
		}

		seconds++;
	}
}