/* 调用头文件 */
#include "reg52.h"
#include <intrins.h> 	// 提供_nop_()函数

/**
 * @brief  10微秒级延时函数
 * @param  ten_us: 延时参数，当 ten_us = 1 时，约延时 10us
 * @note   基于11.0592MHz晶振，实际延时略大于10us
 */
void Delay_10us(unsigned int ten_us)
{
    while(ten_us--);
}


/**
 * @brief  毫秒级延时函数
 * @param  xms: 需要延时的毫秒数
 * @note   基于11.0592MHz晶振，内层循环次数 110 为经验值，约等于 1ms
 */
void Delay_ms(unsigned int xms)
{
    unsigned int i, j;
    for(i = xms; i > 0; i--)
        for(j = 110; j > 0; j--);
}


