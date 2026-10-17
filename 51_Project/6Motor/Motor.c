/* 调用头文件 */
#include "reg52.h"
#include "Delay.h"

/* 引脚定义 */
sbit Motor = P1^0;

/**
  * @brief	关闭电机
  * @param	无
  * @retval	无
*/
void Motor_Off(void)
{
	Motor = 0;
}


/**
  * @brief	电机转动
  * @param	无
  * @retval	无
*/
void Motor_ON(void)
{
	Motor = 1;
}

