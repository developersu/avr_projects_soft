#define F_CPU 12000000L


#include <avr/io.h>
#include <util/delay.h>


#define BUFFER_SIZE 128 	// ???

void init_UART(void)
{
	//	Установка скорости 9600
	UBRRH=0;	//	UBRR=f/(16*band)-1 f=8000000Гц band=9600, 
	UBRRL=77;	//	нормальный асинхронный двунаправленный режим работы  77 was
			// 	Baud Number = (F_CPU / (USART_BAUDRATE * 16)) – 1 
	UCSRA=0b00000000;
	UCSRB=0b00011000;	//	разрешен приём и передача по UART
	UCSRC=0b00000110;	//	8-битовая посылка

}

void send_Uart(const char c)		//	Отправка байта
{
	while(!(UCSRA & (1<<UDRE)));	//	Устанавливается, когда регистр свободен
	UDR = c;
}

void send_Uart_str(const char *s)	//	Отправка строки
{
	while (*s != '\0') send_Uart(*s++);
}

void error(char n){
	send_Uart_str("DHT11 send no response on step - ");
	send_Uart(n);
	send_Uart_str("\r\n");
}

//unsigned char getch_Uart(void)		//	Получение байта
//{
//	while(!(UCSRA&(1<<RXC)));	//	Устанавливается, когда регистр свободен
//	return UDR;
//}


int main(void)
{
	int i, j;
	int stor[5] = {0,0,0,0,0};
	char outp[5][3] =	 {0,0,0,0,0,	\
				 0,0,0,0,0,	\
				 0,0,0,0,0};
	const char * msg = "Message: ";

	init_UART();				//	инициализация UART
//	_delay_ms(2000);			//	задержка 1c
//	send_Uart_str("Greetings!\r\n");		//	отправка строки
//	send_Uart(13);				//	перенос строки CR
//	send_Uart(10);				//	перенос строки LF
//	send_Uart(13);				//	перенос строки CR
//	send_Uart(10);				//	перенос строки LF

    while(1)					//	бесконечный рабочий цикл
    {
    	
	DDRB |= (1<<3);				//конфигурируем как выход
	PORTB &= ~(1<<3);			// ставим логический 0
	_delay_ms(20);
	PORTB |= (1<<3);				//устанваливаем логический 1
	DDRB &= ~(1<<3);			//конфигурируем как вход и ждём понижения до 0
	_delay_us(35);

	if (PINB & (1<<3))			//должен быть 0, если нет - то выполняется условие
	{ 	
		error('1');
	}
	else {
		_delay_us(80);				//ждём еденицу

		if (!(PINB & (1<<3)))			//должен быть 1, если нет - то выполняется условие
		{ 	
			error('2');
		}
		else {
			while (PINB & (1<<3));
			for (i=0;i<5;i++) {					//собираем 5 байт
				for (j=0;j<8;j++){				// тут где-то гавно
					while (!(PINB & (1<<3)));		//и ждём завершения 0
					_delay_us(30);				// delay 30us
					if (PINB & (1<<3)){
						stor[i] |= (1<<(7-j));		//пишем 1
						while (PINB & (1<<3));		//и ждём завершения 'кадра'
					}
				}
			}

			send_Uart_str(msg);
			
			for (i=0;i<5;i++){
				itoa(stor[i],outp[i],10);
				send_Uart_str(outp[i]);
		        	send_Uart_str(", ");                          	// перенос строки CR
			}

		        send_Uart(13);                          		// перенос строки CR
			send_Uart(10);                          		// перенос строки LF

			for (i=0;i<5;i++)
			{
				outp[i][0];
				outp[i][1];
				outp[i][2];
			}
		}
	}
	_delay_ms(1000);
    }
}
