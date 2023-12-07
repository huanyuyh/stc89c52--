#include <REGX52.H>
#include <stdio.h>
#include "Delay.h"
#include "UART.h"
#include "LCD1602.h"	//包含LCD1602头文件
#include "MatrixKey.h"	//包含矩阵键盘头文件
char receiveBuffer[50]; // 接收缓冲区
unsigned char receiveIndex = 0;  // 接收缓冲区的当前索引
int stringToInt(const char *str);
int revNum = 0;
void main()
{
	int oldRevNum = 0;
	unsigned int num = 0;
	unsigned int KeyNum = 0;

	LCD_Init();
	UART_Init_12M();			//串口初始化
	LCD_ShowString(1,1,"send:");
	LCD_ShowString(2,1,"Recv:");
	LCD_ShowNum(1,6,num,4);
	LCD_ShowNum(2,6,revNum,4);
	while(1)
	{
		//UART_SendNum(123);
		//Delay(1000);	
		KeyNum=MatrixKey();				//获取矩阵键盘键码
		if(KeyNum<11){
			num += KeyNum;
			LCD_ShowNum(1,6,num,4);	//LCD显示键码
		}else if(KeyNum==11){
			UART_SendNum(num);
		}else if(KeyNum==12){
			num = 0;
			LCD_ShowNum(1,6,num,4);
			revNum = 0;
		}
		if(oldRevNum!=revNum){
			LCD_ShowNum(2,6,revNum,4);
			oldRevNum=revNum;
		}
	}
}
void UART_Routine() interrupt 4
{
	int i = 0;
	
	char receivedByte = SBUF; // 读取接收到的数据
	static unsigned char end_flag = 0;
	if(RI==1)					//如果接收标志位为1，接收到了数据
	{
		//P2=~SBUF;				//读取数据，取反后输出到LED
		if(receivedByte== '\r'){
			end_flag = 1;
			
		}else if(end_flag == 1){
			if(receivedByte== '\n'){
				receiveBuffer[receiveIndex] = '\0'; // 添加字符串结束符
        // 可以在这里处理接收到的完整数据
				if(stringToInt(receiveBuffer)){
					revNum = stringToInt(receiveBuffer);
					//UART_SendNum(revNum);
				}else{
					//UART_SendString(receiveBuffer);
				}
				receiveIndex = 0; // 重置索引，准备下一次接收
				end_flag = 0;
			}else{
				end_flag = 0;
			}
		}
		else
    {
        receiveBuffer[receiveIndex++] = receivedByte; // 存储数据到缓冲区
        // 确保缓冲区不会溢出
        if (receiveIndex >= sizeof(receiveBuffer) - 1)
        {
            receiveIndex = 0; // 缓冲区溢出，重置索引
        }
    }
		RI=0;					//接收标志位清0
		
	}
}


int stringToInt(const char *str) {
    int result = 0; // 存储结果
    int sign = 1;   // 符号标记，默认为正数

    // 检查负号
    if (*str == '-') {
        sign = -1;
        str++; // 移动到下一个字符
    }

    // 遍历字符直到遇到非数字字符
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
        } else {
            // 非数字字符，跳出循环
            break;
        }
        str++;
    }

    // 返回结果（考虑符号）
    return result * sign;
}