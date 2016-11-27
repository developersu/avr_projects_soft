#define F_CPU 12000000L


#include <avr/io.h>
#include <util/delay.h>


int init_UART(void)
{
	//	Установка скорости 9600
	UBRRH=0;	//	UBRR=f/(16*band)-1 f=8000000Гц band=9600, 
	UBRRL=77;	//	нормальный асинхронный двунаправленный режим работы  
			// 	Baud Number = (F_CPU / (USART_BAUDRATE * 16)) – 1 
	
//		RXC		-	завершение приёма
//		|TXC		-	завершение передачи
//		||UDRE 		-	отсутствие данных для отправки
//		|||FE		-	ошибка кадра
//		||||DOR		-	ошибка переполнение буфера
//		|||||PE		-	ошибка чётности
//		||||||U2X	-	Двойная скорость
//		|||||||MPCM	-	Многопроцессорный режим
//		76543210
	UCSRA=0b00000000;

//		RXCIE		-	прерывание при приёме данных
//		|TXCIE		-	прерывание при завершение передачи
//		||UDRIE		-	прерывание отсутствие данных для отправки
//		|||RXEN		-	разрешение приёма
//		||||TXEN	-	разрешение передачи
//		|||||UCSZ2	-	UCSZ0:2 размер кадра данных
//		||||||RXB8	-	9 бит принятых данных
//		|||||||TXB8	-	9 бит переданных данных
//		76543210
	UCSRB=0b00011000;	//	разрешен приём и передача по UART

//		URSEL		-	всегда 1
//		|UMSEL		-	режим:1-синхронный 0-асинхронный
//		||UPM1		-	UPM0:1 чётность
//		|||UPM0		-	UPM0:1 чётность
//		||||USBS	-	топ биты: 0-1, 1-2
//		|||||UCSZ1	-	UCSZ0:2 размер кадра данных
//		||||||UCSZ0	-	UCSZ0:2 размер кадра данных
//		|||||||UCPOL	-	в синхронном режиме - тактирование
//		76543210
	UCSRC=0b10000110;	//	8-битовая посылка
}

void send_Uart(unsigned char c)		//	Отправка байта
{
	while(!(UCSRA&(1<<UDRE)))	//	Устанавливается, когда регистр свободен
	{}
	UDR = c;
}

void send_Uart_str(unsigned char *s)	//	Отправка строки
{
	while (*s != 0) send_Uart(*s++);
}

unsigned char getch_Uart(void)		//	Получение байта
{
	while(!(UCSRA&(1<<RXC)))	//	Устанавливается, когда регистр свободен
	{}
	return UDR;
}
			

int main(void)
{
	init_UART();				//	инициализация UART
	_delay_ms(5000);			//	задержка 1c
	send_Uart_str("testing string now");		//	отправка строки
	send_Uart(13);				//	перенос строки CR
	send_Uart(10);				//	перенос строки LF
	send_Uart_str("2013");			//	отправка числа
	send_Uart(13);				//	перенос строки CR
	send_Uart(10);				//	перенос строки LF
    while(1)					//	бесконечный рабочий цикл
    {
		if(UCSRA&(1<<RXC))		//	если пришёл байт по UART
		{
			send_Uart_str("\n\rRecieved\n\r");
			send_Uart(getch_Uart()+1);	//	отвечаем увеличенным на 1 пришедшим байтом
		}		
    }
}

