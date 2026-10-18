/* 调用头文件 */
#include "reg52.h"
#include "Nixie.h"
#include "Delay.h"

/* 变量定义 */
unsigned char smgd_code[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
							0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};	// 共阴极数码管0~F段码表


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned char i = 1;
	
	SMG_Display(i,smgd_code[15]);	// 	静态数码管显示F
	Delay_ms(5000);
	
	while(1)
	{
		for(i = 1;i <= 8;i++)				// 动态数码管
		{
			SMG_Display(i,smgd_code[i]);	// 按位发送显示数据
			Delay_ms(1);					// 等待显示稳定
			SMG_Display(i,0x00);			// 清空段选
		}
	}
}
