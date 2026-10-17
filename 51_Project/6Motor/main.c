/* 调用头文件 */
// 将直流电机两根线接到开发板电机驱动部分的OUT1和5V上
#include "reg52.h"
#include "Key.h"
#include "Motor.h"
#include "Delay.h"


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned char Mode = 1;		/* 状态变量 */
	while(1)
	{
		Mode = Key_Scanf();
		if(Mode == 1)			/* 状态1：电机关闭 */
		{
			Motor_Off();
		}
		else if(Mode == 2)		/* 状态2：电机30%转速 */
		{
			Motor_ON();
			Delay_10us(30);	/* 约等价1KHz 30%占空比PWM */
			Motor_Off();
			Delay_10us(50);
		}
		else if(Mode == 3)		/* 状态3：电机50%转速 */
		{
			Motor_ON();
			Delay_10us(50);	/* 约等价1KHz 50%占空比PWM */
			Motor_Off();
			Delay_10us(50);
		}
		else if(Mode == 4)		/* 状态4：电机100%转速 */
		{
			Motor_ON();		/* 约等价1KHz 100%占空比PWM */
		}
		else					/* 其余：电机关闭 */
		{
			Motor_Off();
		}
	}
}




