/* 调用头文件 */
#include "reg52.h"
#include "Uart.h"

/* 宏定义 */
#define RX_BUF_SIZE 16		// 接收缓冲区大小：一条命令最长 15 个字符，再加一个结束符
#define CMD_IDLE_MS 100		// 静默超过这么久（约 100ms）就认为一条命令说完了

/* 变量定义 */
unsigned char rx_buf[RX_BUF_SIZE];		// 接收缓冲区，中断里往里面塞字节
volatile unsigned char rx_len = 0;		// 已经收到了多少个字节
volatile unsigned char cmd_ready = 0;	// 1 表示缓冲区里已经攒够一条完整命令
volatile unsigned char tx_busy = 0;		// 1 表示上一个字节还没发完
volatile unsigned char rx_new = 0;		// 1 表示刚刚又收到了一个新字节（空闲判帧要用）
unsigned char idle_ms = 0;				// 主循环累计"已经有多久没收到新字节了"


/**
  * @brief	串口初始化：9600bps、8 位数据位、无校验、1 位停止位
  * @param	无
  * @retval	无
  * @note	波特率由定时器1 的模式2 产生，这个定时器只负责"分频"，不会产生中断
*/
void Uart_Init(void)
{
	SCON = 0x50;		// 串口方式1（8 位 UART，波特率可变）；REN=1 允许接收
	TMOD &= 0x0F;		// 保留定时器0 的设置，把定时器1 的部分清掉
	TMOD |= 0x20;		// 定时器1 → 模式2（8 位自动重装），专门用来产生波特率
	TH1 = 0xFD;			// 9600bps @ 11.0592MHz：256 - 11059200/(12×32×9600) = 256 - 3 = 0xFD
	TL1 = 0xFD;			// 模式2 里 TL1 是计数器，中断时会自动从 TH1 重装
	PCON &= 0x7F;		// SMOD = 0，波特率不加倍（这一句不写也行，复位后默认就是 0）
	TR1 = 1;			// 启动定时1，波特率时钟开始工作
	ES  = 1;			// 允许串口中断（接收和发送共用 interrupt 4）
	EA  = 1;			// 打开总中断
}


/**
  * @brief	串口中断服务函数：接收字节，或者发送完一个字节
  * @param	无
  * @retval	无
  * @note	接收和发送共用一个中断号 4，所以进来后要自己判断是 RI 还是 TI 触发的
*/
void Uart_ISR(void) interrupt 4
{
	unsigned char dat;

	/* 一、接收：硬件收到一个字节后会把 RI 置 1 */
	if(RI == 1)
	{
		RI = 0;						// 一定要软件清零，否则会一直进中断
		dat = SBUF;					// 把收到的字节读走
		rx_new = 1;					// 告诉主循环：刚刚又来了一个新字节

		if(cmd_ready == 1)			// 上一条命令还没被主循环取走，这个字节先不要了
		{
			/* 丢弃，等主循环把上一条取走再说 */
		}
		else if(dat == '\r' || dat == '\n')	// 收到回车或者换行，说明一条命令结束了
		{
			if(rx_len > 0)					// 连续收到两个换行时，前一个已经处理过，这里跳过
			{
				rx_buf[rx_len] = '\0';		// 补上字符串结束标志
				cmd_ready = 1;				// 告诉主循环：可以取命令了
			}
		}
		else if(rx_len < RX_BUF_SIZE - 1)	// 缓冲区还没满，先存起来
		{
			rx_buf[rx_len] = dat;
			rx_len++;
		}
		else								// 缓冲区满了说明这条命令明显不对，丢掉重来
		{
			rx_len = 0;
		}
	}

	/* 二、发送：硬件把一个字节发完后会把 TI 置 1 */
	if(TI == 1)
	{
		TI = 0;				// 同样必须软件清零
		tx_busy = 0;		// 告诉发送函数：上一个字节发完了，可以发下一个
	}
}


/**
  * @brief	发送一个字节
  * @param	dat  要发送的字节
  * @retval	无
  * @note	写 SBUF 就相当于"开始发送"，发送完成后硬件会进中断把 tx_busy 清 0
*/
void Uart_SendByte(unsigned char dat)
{
	while(tx_busy == 1);		// 上一个字节还没发完，先等着
	tx_busy = 1;				// 标记"现在开始发"，发完由中断清 0
	SBUF = dat;
}


/**
  * @brief	发送一个字符串
  * @param	str  要发送的字符串，必须以 '\0' 结束
  * @retval	无
*/
void Uart_SendString(unsigned char *str)
{
	while(*str != '\0')
	{
		Uart_SendByte(*str);
		str++;
	}
}


/**
  * @brief	取一条完整命令（主循环调用）
  * @param	cmd  取出来的命令会放在这里，调用者要保证空间够大（至少 16 字节）
  * @retval	1 = 取到了一条完整命令；0 = 还没收到
  * @note	拷贝完立刻清空接收缓冲，这样"解析命令"的这段时间里，
  			新来的字节不会和上一条混在一起
*/
unsigned char Uart_GetCommand(unsigned char *cmd)
{
	unsigned char i;

	if(cmd_ready == 0)			// 还没攒够一条完整命令
	{
		return 0;
	}

	ES = 0;							// 拷贝期间先关串口中断，防止新字节插进来把缓冲区搞乱

	for(i = 0; i <= rx_len; i++)		// 连结尾的 '\0' 一起拷过去
	{
		cmd[i] = rx_buf[i];
	}

	rx_len = 0;					// 收完立刻清空，准备接收下一条命令
	cmd_ready = 0;

	ES = 1;							// 拷贝完了，重新允许串口中断

	return 1;
}


/**
  * @brief	空闲判帧：主循环每 1ms 左右调用一次
  * @param	无
  * @retval	无
  * @note	上位机不一定每次都发回车换行，所以不能只靠 '\r' '\n' 判断命令结束。
  			这里的办法是"数静默时间"：只要连续 100ms 没有新字节进来，
  			就当这条命令已经说完了。勾不勾"发送新行"都能用。
  			注意：这个函数靠主循环反复调用来数时间，所以主循环里不能有长延时
*/
void Uart_CheckIdle(void)
{
	if(rx_new == 1)					// 这一拍里有新字节进来，说明还没说完，重新数
	{
		rx_new = 0;
		idle_ms = 0;
		return;
	}

	if(rx_len == 0 || cmd_ready == 1)	// 缓冲区空的，或者已经攒好一条了，不用数
	{
		idle_ms = 0;
		return;
	}

	if(idle_ms < CMD_IDLE_MS)		// 慢慢累加，最多加到 CMD_IDLE_MS 就不再往上涨
	{
		idle_ms++;
	}

	if(idle_ms >= CMD_IDLE_MS)		// 静默够久了，就当这条命令说完了
	{
		ES = 0;						// 关串口中断，防止下面两步被新字节打断
		rx_buf[rx_len] = '\0';		// 补上字符串结束标志
		cmd_ready = 1;				// 通知主循环：可以取命令了
		idle_ms = 0;				// 计数器归零，等下一条命令重新数
		ES = 1;
	}
}