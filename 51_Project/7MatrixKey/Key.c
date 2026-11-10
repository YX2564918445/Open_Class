/* 调用头文件 */
#include "reg52.h"
#include "Delay.h"

/* 引脚定义 */
#define KEY_PORT P1		// 4x4矩阵键盘接P1口
sbit KEY_ROW1 = P1^7;	// 第1行
sbit KEY_ROW2 = P1^6;	// 第2行
sbit KEY_ROW3 = P1^5;	// 第3行
sbit KEY_ROW4 = P1^4;	// 第4行
sbit KEY_COL1 = P1^3;	// 第1列
sbit KEY_COL2 = P1^2;	// 第2列
sbit KEY_COL3 = P1^1;	// 第3列
sbit KEY_COL4 = P1^0;	// 第4列


/**
  * @brief	矩阵按键扫描：检测是否有按键被按下，返回按键编号
  * @param	无
  * @retval	key_num  按键编号（1~16）
  			0         没有按键被按下
*/
unsigned char MatrixKey_Scan(void)
{
	unsigned char key_num = 0;
	unsigned char row;

	/* 逐行扫描 */
	KEY_PORT = 0x0F;					// 列置高、行置低，准备读列状态

	/* 判断是否有键按下：检查任一列是否为低 */
	if(KEY_PORT != 0x0F)
	{
		Delay_ms(10);					// 消抖
		if(KEY_PORT != 0x0F)			// 确认有键按下
		{
			/* 逐行驱动为低，读取列状态 */
			for(row = 0; row < 4; row++)
			{
				KEY_PORT = ~(0x80 >> row);	// 行(P1.7~P1.4)逐行置低，其余行置高，列置高
				if(KEY_COL1 == 0)
				{
					key_num = row * 4 + 1;
					break;
				}
				else if(KEY_COL2 == 0)
				{
					key_num = row * 4 + 2;
					break;
				}
				else if(KEY_COL3 == 0)
				{
					key_num = row * 4 + 3;
					break;
				}
				else if(KEY_COL4 == 0)
				{
					key_num = row * 4 + 4;
					break;
				}
			}
			/* 等待按键释放 */
			KEY_PORT = 0x0F;			// 重新拉低所有行，准备检测释放
			while(KEY_PORT != 0x0F);
			Delay_ms(10);				// 松开消抖
		}
	}

	return key_num;
}