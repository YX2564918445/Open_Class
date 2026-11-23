/* 调用头文件 */
#include "reg52.h"
#include "Uart.h"

/* 引脚端口宏定义 */
// 8 个 LED 全接在 P2 口，共阳极，低电平点亮
#define LED P2

/* 变量定义 */
unsigned char cmd_buf[16];		// 存放从接收缓冲区取出来的命令


/**
  * @brief	把字符串里的小写字母全部换成大写
  * @param	str  要处理的字符串（原地修改）
  * @retval	无
  * @note	这样不管上位机发 ON、on 还是 On，板子都能认
*/
void To_Upper(unsigned char *str)
{
	while(*str != '\0')
	{
		if(*str >= 'a' && *str <= 'z')
		{
			*str = *str - 'a' + 'A';		// 小写字母的 ASCII 码比大写大 32
		}
		str++;
	}
}


/**
  * @brief	比较两个字符串是否完全相同
  * @param	a  字符串1
  * @param	b  字符串2
  * @retval	1 = 相同；0 = 不同
*/
unsigned char Str_Equal(unsigned char *a, unsigned char *b)
{
	while(*a == *b)
	{
		if(*a == '\0')			// 比到两个都结束了，说明完全一样
		{
			return 1;
		}
		a++;
		b++;
	}
	return 0;
}


/**
  * @brief	回复上位机："LEDn ON" 或 "LEDn OFF"
  * @param	n   灯的序号 1~8
  * @param	on  1 = 亮，0 = 灭
  * @retval	无
  * @note	用循环拼接而不是 sprintf，省下好几 KB 的程序空间
*/
void Send_LED_Reply(unsigned char n, unsigned char on)
{
	Uart_SendString("LED");
	Uart_SendByte('0' + n);			// 数字 1~8 加上 '0' 就变成字符 '1'~'8'

	if(on == 1)
	{
		Uart_SendString(" ON\r\n");
	}
	else
	{
		Uart_SendString(" OFF\r\n");
	}
}


/**
  * @brief	把字符串里的空格全部删掉
  * @param	str  要处理的字符串（原地修改）
  * @retval	无
  * @note	这样 "ALL ON" 和 "ALLON"、"1 ON" 和 "1ON" 就成了同一个串，
  			后面解析的时候不用再到处跳空格
*/
void Remove_Space(unsigned char *str)
{
	unsigned char *p = str;		// p 负责往后读
	unsigned char *q = str;		// q 负责往前写，只会追平或者落后于 p

	while(*p != '\0')
	{
		if(*p != ' ')				// 遇到空格就跳过，不往 q 那里写
		{
			*q = *p;
			q++;
		}
		p++;
	}

	*q = '\0';					// 新的结尾（字符串可能被缩短了）
}


/**
  * @brief	解析一条命令并执行
  * @param	cmd  命令字符串：单颗灯用 "1ON"、"1OFF"，整排用 "ALL ON"、"ALL OFF"
  * @retval	无
  * @note	命令里有没有空格都行，"1ON" 和 "1 ON"、"ALLON" 和 "ALL ON" 都认；
  			命令不合法时回一条 ERR
*/
void Do_Command(unsigned char *cmd)
{
	unsigned char n;
	unsigned char mask;
	unsigned char *p;

	To_Upper(cmd);			// 先统一转大写，后面就只需要比 "ON" 和 "OFF"
	Remove_Space(cmd);		// 再把空格全删掉，"ALL ON" 就变成了 "ALLON"
	p = cmd;

	// 整排控制：8 颗灯一起开、或者一起关
	if(Str_Equal(p, "ALLON") == 1)
	{
		LED = 0x00;			// 8 位全清 0 → 8 颗灯全亮
		Uart_SendString("ALL ON\r\n");
		return;
	}

	if(Str_Equal(p, "ALLOFF") == 1)
	{
		LED = 0xFF;			// 8 位全置 1 → 8 颗灯全灭
		Uart_SendString("ALL OFF\r\n");
		return;
	}

	// 单颗控制：第 1 个字符必须是 1~8，表示要点第几颗灯
	if(*p < '1' || *p > '8')
	{
		Uart_SendString("ERR: 1ON/1OFF/ALLON/ALLOFF\r\n");
		return;
	}

	n = *p - '0';				// 字符 '3' 减去 '0' 就是数字 3
	mask = 1 << (n - 1);		// 第 n 颗灯对应的那一位：LED1→0x01，LED8→0x80
	p++;

	if(Str_Equal(p, "ON") == 1)
	{
		LED &= (unsigned char)(~mask);	// 这一位清 0 → 输出低电平 → 灯亮
		Send_LED_Reply(n, 1);
	}
	else if(Str_Equal(p, "OFF") == 1)
	{
		LED |= mask;				// 这一位置 1 → 输出高电平 → 灯灭
		Send_LED_Reply(n, 0);
	}
	else
	{
		Uart_SendString("ERR: 1ON/1OFF/ALLON/ALLOFF\r\n");
	}
}


/**
  * @brief	粗略的 1ms 延时
  * @param	无
  * @retval	无
  * @note	只用来给"空闲判帧"数拍子，不要求精确，差个零点几毫秒没关系。
  			用 volatile 是怕编译器觉得这个循环什么都没干，直接优化掉
*/
void Delay_1ms(void)
{
	volatile unsigned char i;

	for(i = 0; i < 110; i++);
}


/**
  * @brief	主函数
  * @param	无
  * @retval	无
  * @note	收字节在中断里做，解析和回显在主循环里做。
  			这一点很重要：如果发送也放在中断里，发一条回显要 9ms 左右，
  			这 9ms 内上位机再发字节就会丢
*/
int main(void)
{
	LED = 0xFF;			// 上电先把 8 颗灯全部熄灭（低电平才点亮，所以写 1 是灭）
	Uart_Init();		// 串口初始化：9600、8 位数据、无校验、1 位停止

	while(1)
	{
		Uart_CheckIdle();					// 数静默时间：勾不勾"发送新行"都能判出命令结束

		if(Uart_GetCommand(cmd_buf) == 1)	// 收到一条完整命令
		{
			Do_Command(cmd_buf);				// 解析 → 控制灯 → 回显
		}

		Delay_1ms();						// 给上面"数拍子"用的，主循环别有长延时
	}
}
