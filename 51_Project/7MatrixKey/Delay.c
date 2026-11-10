/* 调用头文件 */
#include "reg52.h"
#include "Delay.h"
#include <intrins.h>	// 提供_nop_()函数

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