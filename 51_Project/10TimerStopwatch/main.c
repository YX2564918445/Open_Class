/* 调用头文件 */
#include "reg52.h"
#include "Nixie.h"
#include "Delay.h"

/* 引脚端口宏定义 */
// 独立按键（K1、K2 与串口共用，本例题程不用，只用 K3 和 K4）
sbit KEY_PAUSE = P3^2;		// K3：暂停 / 继续
sbit KEY_CLEAR = P3^3;		// K4：清零

#define SMG_MINUS 0x40		// 字符 "-" 的段码：共阴极数码管只有中间那一横（g 段）亮

/* 数码管左右方向开关 */
// 显示缓冲区里 buf[0] 放最左边一位，buf[7] 放最右边一位。
// 但 SMG_Display 的 position 对应板上哪一位，取决于排线的实际方向，
// 换一块板子可能整排左右颠倒。上板后发现数字反了，把下面的 0 改成 1，
// 显示部分其它地方都不用动。
#define SMG_DIR 0


/* 变量定义 */
unsigned char smgd_code[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
							0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};	// 共阴极数码管0~F段码表

// 下面这几个变量中断里会改、主函数里会读，必须加 volatile。
// 不加的话编译器可能认为"主函数里没人改它"，只读一次，数字就停着不动了
volatile unsigned int  tenth = 0;		// 0.1 秒的计数，显示时除以 10 就是秒数
volatile unsigned char running = 1;		// 1 = 正在计时，0 = 暂停
volatile unsigned char dot_on = 1;		// 小数点当前是否点亮（暂停时用来闪烁）

unsigned char tick10 = 0;				// 数 10ms 的次数，凑够 10 次就是 0.1 秒
unsigned char dot_cnt = 0;				// 小数点闪烁的计数
// tick10 和 dot_cnt 只在中断里用，主函数不碰，所以不用加 volatile


/**
  * @brief	定时器0中断服务函数：每 10ms 自动进来一次
  * @param	无
  * @retval	无
  * @note	本函数由硬件自动调用，主函数里不要调用它
*/
void Timer0_ISR(void) interrupt 1
{
	// 模式1 的定时器不会自动重装初值，所以每次进中断都要自己重新写一遍
	// 10ms 的初值：10ms / 1.085us ≈ 9216 个机器周期，65536 - 9216 = 56320 = 0xDC00
	TH0 = 0xDC;
	TL0 = 0x00;

	if(running == 1)					// 正在计时
	{
		// 为什么一次中断不是直接数 0.1 秒？
		// 因为 0.1 秒要 92160 个机器周期，早就超过 16 位定时器的最大值 65536 了，
		// 所以先在中断里数 10 次 10ms，凑够一次 0.1 秒
		tick10++;
		if(tick10 >= 10)				// 10 × 10ms = 100ms = 0.1 秒
		{
			tick10 = 0;
			tenth++;					// 0.1 秒计数加 1
			if(tenth >= 36000)			// 满 3600.0 秒（1 小时）就回到 0，防止数字溢出
			{
				tenth = 0;
			}
		}
		dot_on = 1;						// 计时过程中小数点一直亮着
	}
	else								// 暂停：数值不再变化
	{
		// 让小数点每 0.3 秒闪一下，一眼就能看出"停住了"，不是程序跑飞了
		dot_cnt++;
		if(dot_cnt >= 30)				// 30 × 10ms = 300ms
		{
			dot_cnt = 0;
			dot_on = !dot_on;			// 亮 → 灭 → 亮，来回翻转
		}
	}
}


/**
  * @brief	把秒表的数值转换成段码，放进显示缓冲区
  * @param	buf  显示缓冲区，buf[0] 对应最左边一位，buf[7] 对应最右边一位
  * @retval	无
  * @note	显示成 123.4 这种样子：小数点固定在倒数第二位右边，
  			左边还没有数字的位一律显示 "-"，而不是让它黑着
*/
void Update_DispBuf(unsigned char *buf)
{
	unsigned int v;			// 0.1 秒的计数
	unsigned char i;

	// 第一步：先把 8 位全部填成 "-"
	for(i = 0; i < 8; i++)
	{
		buf[i] = SMG_MINUS;
	}

	// 第二步：把数值读出来。
	// tenth 是 16 位的，51 一次只能读一个字节，如果读到一半中断把它加了 1，
	// 就会读出一个乱七八糟的数，所以先关中断，读完再打开
	EA = 0;
	v = tenth;
	EA = 1;

	// 第三步：从最右边一位开始往前填数字
	buf[7] = smgd_code[v % 10];			// 十分位
	v = v / 10;
	buf[6] = smgd_code[v % 10] | 0x80;	// 秒个位，或上 0x80 就点亮了它右边的小数点
	v = v / 10;

	// 第四步：剩下的秒十位、百位……只有真正有数字时才写进去，为 0 就留着刚才的 "-"
	i = 6;
	while(v > 0 && i > 0)
	{
		i--;
		buf[i] = smgd_code[v % 10];
		v = v / 10;
	}

	// 暂停时让小数点按 dot_on 闪；计时中 dot_on 一直是 1，小数点就常亮
	if(dot_on == 0)
	{
		buf[6] &= 0x7F;			// 把最高位清 0，小数点熄灭
	}
}


/**
  * @brief	按键扫描：K3 暂停/继续，K4 清零
  * @param	无
  * @retval	无
*/
void Key_Scan(void)
{
	// 51 单片机读引脚前要先写 1，把引脚拉成弱上拉，否则读回来可能一直是 0
	KEY_PAUSE = 1;
	KEY_CLEAR = 1;

	if(KEY_PAUSE == 0)					// K3 被按下了
	{
		Delay_ms(10);					// 消抖：先等 10ms 看它是不是真的还按着
		if(KEY_PAUSE == 0)
		{
			running = !running;			// 在"计时"和"暂停"之间来回切换
			if(running == 0)
			{
				dot_cnt = 0;			// 刚按下暂停：小数点从"亮"开始闪
				dot_on = 1;
			}
			else
			{
				dot_on = 1;				// 继续计时：小数点恢复常亮
			}
		}
		while(KEY_PAUSE == 0);			// 等松手，保证按住不放也只算按了一次
	}
	else if(KEY_CLEAR == 0)				// K4 被按下了
	{
		Delay_ms(10);
		if(KEY_CLEAR == 0)
		{
			// tenth 是两个字节，改到一半被中断打断的话会留下半截数据，
			// 所以先关总中断，清完再打开。
			// tick10 是中断里在用的分频计数，也一起放在临界区里清，免得正好清到一半又被加 1
			EA = 0;
			tenth = 0;
			tick10 = 0;
			EA = 1;
		}
		while(KEY_CLEAR == 0);
	}
}


/**
  * @brief	主函数
  * @param	无
  * @retval	无
*/
int main(void)
{
	unsigned char disp_buf[8];		// 显示缓冲区：buf[0] 最左，buf[7] 最右
	unsigned char i;

	/* 定时器0 初始化：产生 10ms 一次的中断 */
	TMOD &= 0xF0;		// TMOD 高 4 位是定时器1 的设置，原样保留；低 4 位清零
	TMOD |= 0x01;		// 定时器0 工作在模式1（16 位定时器）；GATE=0、C/T=0 表示"用内部时钟定时"
	TH0 = 0xDC;			// 10ms 初值：10ms / 1.085us ≈ 9216，65536 - 9216 = 0xDC00
	TL0 = 0x00;
	ET0 = 1;			// 允许定时器0 中断
	EA  = 1;			// 打开总中断
	TR0 = 1;			// 启动定时器0，它开始一个机器周期加 1

	while(1)
	{
		Update_DispBuf(disp_buf);		// 把最新的秒表数值转成段码

		// 动态扫描：8 位轮流点亮，轮一圈只要几毫秒，人眼看到的是同时显示
		for(i = 0; i < 8; i++)
		{
			// SMG_DIR=0 时 buf[0] 送位置1、buf[7] 送位置8；
			// SMG_DIR=1 时反过来送，用来应付左右排线相反的情况
			SMG_Display(SMG_DIR ? (8 - i) : (i + 1), disp_buf[i]);
			Delay_ms(1);						// 让这一位亮约 1ms
			SMG_Display(SMG_DIR ? (8 - i) : (i + 1), 0x00);	// 消隐：切下一位之前先把段码清 0，防止拖影
		}

		Key_Scan();		// 每扫描完一圈就查一次按键
	}
}