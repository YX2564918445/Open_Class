/* 调用头文件 */
#include "reg52.h"
#include "Key.h"
#include "Nixie.h"
#include "Delay.h"

/* 变量定义 */
unsigned char smgd_code[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
							0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};	// 共阴极数码管0~F段码表

unsigned char disp_buf[8] = {0};	// 显示缓冲区（位置8在左，位置1在右）


/**
  * @brief	按键处理：编号→段码→填入显示缓冲区（右对齐，左移溢出）
  * @param	key_num  按键编号（1~16）
  * @retval	无
*/
void Key_Process(unsigned char key_num)
{
	unsigned char i;
	unsigned char digit;

	if(key_num == 0) return;	// 无按键按下，直接返回

	digit = key_num - 1;		// 按键1→0, 按键2→1, …, 按键16→F

	/* 显示缓冲区左移（最右位空出，最左位丢弃） */
	for(i = 0; i < 7; i++)
	{
		disp_buf[i] = disp_buf[i+1];
	}
	disp_buf[7] = smgd_code[digit];	// 新数字放在最右位
}


/**
  * @brief	数码管动态扫描：将显示缓冲区内容刷新一次
  * @param	无
  * @retval	无
*/
void Display_Buffer(void)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		SMG_Display(i+1, disp_buf[i]);	// 按位发送显示数据
		Delay_ms(5);					// 等待显示稳定
		SMG_Display(i+1, 0x00);			// 清空段选（消隐）
	}
}


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned char key;

	while(1)
	{
		key = MatrixKey_Scan();		// 扫描矩阵按键
		Key_Process(key);			// 更新显示缓冲区
		Display_Buffer();			// 刷新数码管显示
	}
}