/* 调用头文件 */
#include "reg52.h"
#include "Buzzer.h"
#include "Key.h"


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned char Mode = 0;		// 状态变量
	while(1)
	{
		Mode = KEY_Scanf();		/* 获取键码值（状态） */
		if(Mode == 1)			/* 状态1 */
		{
			Buzzer_5000Hz(2);	// 5000Hz响2秒
		}
		else if(Mode == 2)		/* 状态2 */
		{
			Buzzer_500Hz(2);	// 500Hz响2秒
		}
		else					/* 关闭蜂鸣器 */
		{
			Buzzer_Off();		// 关闭蜂鸣器
		}
		Mode = 0;				/* 清零状态标志 */
	}
}