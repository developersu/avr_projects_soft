/*
 * Dmitry Isaenko
 * Ryazan, 2016
 *
 * Distibution under GNU GPL v.3 
 * for more information please visit https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 */

#define F_CPU 12000000L

#include <avr/io.h>
#include <util/delay.h>

void init_UART(void)
{
	//	Установка скорости 9600
	UBRRH=0;		//	UBRR=f/(16*band)-1 f=12000000Гц band=9600, 
	UBRRL=77;		//	нормальный асинхронный двунаправленный режим работы  77 was
			
	UCSRA=0b00000000;
	UCSRB=0b00011000;	//	разрешаем приём и передачу по UART
	UCSRC=0b00000110;	//	посылки длинною в 8 бит 
}

void sendToUart(const char c)		//	Отправка байта
{
	while(!(UCSRA & (1<<UDRE)));	//	Устанавливается, когда регистр свободен
	UDR = c;
}

void sendToUart_str(const char *s)	//	Отправка строки
{
	while (*s != '\0') sendToUart(*s++);
}

void error(char n){
	sendToUart_str("DHT11 send no response on step - ");
	sendToUart(n);
	sendToUart_str("\r\n");
}

int main(void)
{
	int i, j;
	int stor[5] = {0,0,0,0,0};
	char outp[2][3] =	 {0,0,0,	\
				 0,0,0};
	init_UART();				//	инициализация UART

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
			while (PINB & (1<<3));		// ждём пока 1 закончится
			for (i=0;i<5;i++) {					//собираем 5 байт
				for (j=7;j>=0;j--){				
					while (!(PINB & (1<<3)));		//и ждём завершения 0
					_delay_us(30);				// delay 30us - если через 30us будет 1 - тогда 1 пишем, иначе 0 (ничего не пишем)
					if (PINB & (1<<3)){
						stor[i] |= (1<<j);		//пишем 1
						while (PINB & (1<<3));		//и ждём завершения 'кадра'
					}
				}
			}

			sendToUart_str("Hum: ");
			/* вывод на uart и обнуление */
			itoa(stor[0],outp[0],10);
			sendToUart_str(outp[0]);
		        sendToUart_str("%\r\nt: ");                          	// перенос строки CR
			itoa(stor[2],outp[1],10);
			sendToUart_str(outp[1]);
		        sendToUart_str(" *C\r\n");                          	// перенос строки CR

			for (i=0;i<3;i++)
			{
				outp[i][0] = 0;
				outp[i][1] = 0;
			}
		}
	}
	_delay_ms(1000);
    }
}
