#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"

#include "MCAL/DIO/DIO_interface.h"
#include "MCAL/ADC/ADC_interface.h"
#include "MCAL/GIE/GIE_interface.h"

#include "HAL/LCD/LCD_interface.h"
#include "HAL/KPD/KPD_Interface.h"

#include "avr/delay.h"
void APP (void);
u16 ADC_Result;
volatile u8 temp=0;
volatile u32 volt, digital;

void main (void)
{
	u8 Local_u8Key=0;
	u8 Password=0;
	u8 Flag_star=0;
	u8 LCD_Flag =0;
	u8 number_of_tries=0;
	u8 check_flag=0;

	ADC_voidInit();
	GIE_voidEnable();
	HLCD_VoidInit();

	DIO_u8SetPinDirection(DIO_u8_PORTA,DIO_u8_PIN0,DIO_u8_PIN_INPUT);
	DIO_u8SetPinValue(DIO_u8_PORTA,DIO_u8_PIN0,DIO_u8_PIN_LOW);

	DIO_u8SetPinDirection(DIO_u8_PORTD,DIO_u8_PIN5,DIO_u8_PIN_OUTPUT);

	DIO_u8SetPinDirection(DIO_u8_PORTD,DIO_u8_PIN6,DIO_u8_PIN_OUTPUT);
	HLCD_VoidSendString("Welcome Back");
	_delay_ms(1000);
	HLCD_VoidSendCommand(DISPLAYCLEAR);

	HLCD_VoidSendString("Enter Password");
	_delay_ms(1000);
	HLCD_VoidSendCommand(DISPLAYCLEAR);

	while(Local_u8Key !='=')
	{
		Password = Password*10+Local_u8Key;
		if(Flag_star!=0)
		{
			HLCD_VoidSendChar('*');
		}
		do
		{
			Local_u8Key=KPD_u8GetPressedKey();
			Flag_star=1;
		}while(Local_u8Key==KPD_NO_PRESSED_KEY );

	}

	while(1)
	{
		if(Password == 123)
		{
			if(check_flag=0)
			{
				HLCD_VoidSendCommand(DISPLAYCLEAR);
				HLCD_VoidSendString("Welcome Ahmed ");
				_delay_ms(1000);
				HLCD_VoidSendCommand(DISPLAYCLEAR);
			}

			ADC_u16StartConversionAsynchronous(0,APP);

			if(temp >=45 && LCD_Flag !=1)
			{
				LCD_Flag =1;
				HLCD_VoidSendCommand(DISPLAYCLEAR);
				HLCD_VoidSendString("weather is hot");
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString("Fan is turn on");
				DIO_u8SetPinValue(DIO_u8_PORTD,DIO_u8_PIN6,DIO_u8_PIN_HIGH);
				DIO_u8SetPinValue(DIO_u8_PORTD,DIO_u8_PIN5,DIO_u8_PIN_HIGH);

			}

			else if( (temp <40 && temp > 35) && LCD_Flag !=2)
			{
				LCD_Flag = 2;
				HLCD_VoidSendCommand(DISPLAYCLEAR);
				HLCD_VoidSendString("weather is warm");
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString("Fan is turn off");
				DIO_u8SetPinValue(DIO_u8_PORTD,DIO_u8_PIN6,DIO_u8_PIN_LOW);
				DIO_u8SetPinValue(DIO_u8_PORTD,DIO_u8_PIN5,DIO_u8_PIN_HIGH);
			}
			else if( temp < 35 && LCD_Flag !=3 )
			{
				LCD_Flag =3;
				HLCD_VoidSendCommand(DISPLAYCLEAR);

				HLCD_VoidSendString("weather is cool");
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString("Fan is turn off");
				DIO_u8SetPinValue(DIO_u8_PORTD,DIO_u8_PIN6,DIO_u8_PIN_LOW);
				DIO_u8SetPinValue(DIO_u8_PORTD,DIO_u8_PIN5,DIO_u8_PIN_LOW);


			}

		}
		else if(Password != 123 && number_of_tries <2)
		{
			HLCD_VoidSendCommand(DISPLAYCLEAR);
			HLCD_VoidSendString("Wrong password");
			_delay_ms(500);
			HLCD_VoidSendCommand(DISPLAYCLEAR);
			HLCD_VoidSendString("Try again");
			_delay_ms(500);
			HLCD_VoidSendCommand(DISPLAYCLEAR);
			Password=0;
			Local_u8Key =0;
			Flag_star =0;
			while(Local_u8Key !='=')
			{
				Password = Password*10+Local_u8Key;
				if(Flag_star!=0)
				{
					HLCD_VoidSendChar('*');
				}
				do
				{
					Local_u8Key=KPD_u8GetPressedKey();
					Flag_star=1;
				}while(Local_u8Key==KPD_NO_PRESSED_KEY );

			}
			number_of_tries++;

		}
		else
		{
			if(check_flag ==0)
			{
				HLCD_VoidSendCommand(DISPLAYCLEAR);
				HLCD_VoidSendString("exceed ");
				HLCD_VoidGotoXY(1,0);
				HLCD_VoidSendString("valid tries");
				check_flag =1;
			}
		}
	}
}
void APP(void)
{
	HLCD_VoidGotoXY(0,0);
	volt =ADC_GetVolt();
	temp = volt /10;
}
