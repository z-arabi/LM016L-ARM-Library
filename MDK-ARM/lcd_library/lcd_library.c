#include "lcd_library.h"

#include "main.h"
#include "stm32f4xx_hal.h"

lcd_t lcd_create(GPIO_TypeDef* data_ports[],GPIO_TypeDef* en_port,GPIO_TypeDef* rs_port,GPIO_TypeDef* rw_port,
								 uint16_t data_pins[],uint16_t en_pin,uint16_t rs_pin,uint16_t rw_pin,lcd_dl_t mode)
{
	lcd_t lcd;

	lcd.data_ports = data_ports;
	lcd.data_pins = data_pins;
	
	lcd.en_port = en_port;
	lcd.en_pin = en_pin;
	
	lcd.rs_port = rs_port;
	lcd.rs_pin = rs_pin;
	
	lcd.rw_port=rw_port;
	lcd.rw_pin=rw_pin;
	
	lcd.mode=mode;
	
	lcd_init(&lcd);

	return lcd;
}

void lcd_write_command(lcd_t * lcd, uint8_t command)
{
		HAL_Delay(T);
		
		HAL_GPIO_WritePin(lcd->rs_port,lcd->rs_pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->rw_port,lcd->rw_pin, GPIO_PIN_RESET);		
		HAL_GPIO_WritePin(lcd->data_ports[0],0xff << (lcd->data_pins[0]), GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[0],command << (lcd->data_pins[0]), GPIO_PIN_SET);
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);	
}


void lcd_init(lcd_t*lcd)
{
	if(lcd->mode==1)
	{
		HAL_Delay(T);
			
		lcd_write_command(lcd,0x38); //8-bit interface,2lines,5*7 pixels
		lcd_write_command(lcd,0x38);
		lcd_write_command(lcd,0x38);
	}
	else if(lcd->mode==0)
	{
		HAL_Delay(T);
			
		lcd_write_command(lcd,0x28); //4-bit interface,2lines,5*7 plixels
		lcd_write_command(lcd,0x28);
		lcd_write_command(lcd,0x28);
		lcd_write_command(lcd,0x28);
	}
	
};


void lcd_putchar(lcd_t * lcd,uint8_t character)
{
		HAL_Delay(T);
			
		HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(lcd->rw_port, lcd->rw_pin, GPIO_PIN_RESET);		
		HAL_GPIO_WritePin(lcd->data_ports[0],0xff << (lcd->data_pins[0]), GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lcd->data_ports[0],character << (lcd->data_pins[0]), GPIO_PIN_SET);
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);	
};
	
void lcd_set_curser(lcd_t * lcd,uint16_t row,uint16_t col) 
{
	HAL_Delay(1);
	
	switch(row)
	{
		case 0:
		lcd_write_command(lcd,0x80+col);
		break;
		
		case 1:
		lcd_write_command(lcd,0xc0+col);
		break;
	}
} ;
	
void lcd_clear(lcd_t * lcd)
{
	HAL_Delay(T);
	lcd_write_command(lcd,0x01);
	lcd_write_command(lcd,0x02);
};
	
void lcd_puts(lcd_t * lcd,char *str) 
{
	HAL_Delay(2);
	
	while(*str != 0)
	{
		lcd_putchar(lcd,*str);
		str++;
	}
};
