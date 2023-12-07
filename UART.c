#include <REGX52.H>

/**
  * @brief  串口初始化，4800bps@12.000MHz
  * @param  无
  * @retval 无
  */
void UART_Init_12M()
{
	SCON=0x50;
	PCON |= 0x80;
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
}
/**
  * @brief  串口初始化，4800bps@11.0592MHz
  * @param  无
  * @retval 无
  */
void UART_Init_110592()
{
	SCON=0x50;
	PCON |= 0x80;
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xF4;		//设定定时初值
	TH1 = 0xF4;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
}

/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UART_SendChar(char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

void UART_SendOne(int num)
{
	SBUF=num+'0';
	while(TI==0);
	TI=0;
}

void UART_SendNum(int num){
	char temp = 0;
	int fnum = 0;
	while(num>0){
		fnum =fnum*10+num%10;
		num = num/10;
	}
	while(fnum>0){
		UART_SendOne(fnum%10);
		fnum = fnum/10;
	}
	UART_SendChar('\r');
	UART_SendChar('\n');
}

void UART_SendString(char* str)
{
	while (*str) {
     UART_SendChar(*str);
     str++;
  }
	UART_SendChar('\r');
	UART_SendChar('\n');
}

