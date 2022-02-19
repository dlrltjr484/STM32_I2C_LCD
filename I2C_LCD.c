#include "I2C_LCD.h"

I2C_HandleTypeDef hi2c1;

void wr_PCF8574(uint8_t dev_add, uint8_t wdata) // hardware I2C1
{
	uint8_t hal_status;
	// 1 byte write
	hal_status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)dev_add, &wdata, 1, 10000);

	//HAL_StatusTypeDef HAL_I2C_Master_Transmit

	if( hal_status != HAL_OK)
	Error_Handler();
}

void write_LCDcmd_4b(uint8_t lcd_cmd) // 4bit X 1
{
	uint8_t lcd_cmd_h;
	lcd_cmd_h = ( lcd_cmd & 0xF0);
	//WR MSB
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_h | 0x08 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=0
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_h | 0x0C ); // D7, D6, D5, D4, BL=1, E=1, RW=0, RS=0
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_h | 0x08 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=0
}

void write_LCDcmd_8b(uint8_t lcd_cmd)
{
	uint8_t lcd_cmd_h, lcd_cmd_l;
	lcd_cmd_h = ( lcd_cmd & 0xF0);
	lcd_cmd_l = ( lcd_cmd<<4 ) & 0xF0;
	//WR high 4bit
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_h | 0x08 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=0
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_h | 0x0C ); // D7, D6, D5, D4, BL=1, E=1, RW=0, RS=0
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_h | 0x08 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=0
	//WR low 4bit
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_l | 0x08 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=0
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_l | 0x0C ); // D7, D6, D5, D4, BL=1, E=1, RW=0, RS=0
	wr_PCF8574(PCF8574_ADD0, lcd_cmd_l | 0x08 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=0
}

void write_LCDdata_8b (uint8_t lcd_dat)
{
	uint8_t lcd_data_h, lcd_data_l;
	lcd_data_h = ( lcd_dat & 0xF0);
	lcd_data_l = ( lcd_dat<<4 ) & 0xF0;
	//WR high 4bit
	wr_PCF8574(PCF8574_ADD0, lcd_data_h | 0x09 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=1
	wr_PCF8574(PCF8574_ADD0, lcd_data_h | 0x0D ); // D7, D6, D5, D4, BL=1, E=1, RW=0, RS=1
	wr_PCF8574(PCF8574_ADD0, lcd_data_h | 0x09 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=1
	//WR low 4bit
	wr_PCF8574(PCF8574_ADD0, lcd_data_l | 0x09 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=1
	wr_PCF8574(PCF8574_ADD0, lcd_data_l | 0x0D ); // D7, D6, D5, D4, BL=1, E=1, RW=0, RS=1
	wr_PCF8574(PCF8574_ADD0, lcd_data_l | 0x09 ); // D7, D6, D5, D4, BL=1, E=0, RW=0, RS=1
}

void Init_LCD(void)
{
	HAL_Delay(20); // wait 20ms
	write_LCDcmd_4b(0x30); HAL_Delay(5); // Fuction set, 4bit IF
	write_LCDcmd_4b(0x30); HAL_Delay(1); // Fuction set, 4bit IF
	write_LCDcmd_4b(0x30); // Fuction set, 4bit IF
	write_LCDcmd_4b(0x20); HAL_Delay(10); // 4bit I/F
	write_LCDcmd_8b(0x28); HAL_Delay(1); // 4bit data, 1/16 duty, 5x7dot
	write_LCDcmd_8b(0x0c); HAL_Delay(10); // display on, cursor off
	write_LCDcmd_8b(0x01); HAL_Delay(10); // Display clear
	write_LCDcmd_8b(0x06); // Entry mode set, cursor increment move, no shift display
}

void write_string (uint8_t start_x, char *p)
{
	uint8_t h;
	write_LCDcmd_8b (0x80 | start_x); HAL_Delay(3); // set display position(cursor)
		for (h = 0; *p != 0; h++)
		{
		write_LCDdata_8b (*p);
		p++;
		}
}

char *num_to_str_2d(uint16_t disp_value) 
{
	digit3[0]=(disp_value/10) % 10 +'0';
	if ( digit3[0] == ' ')
	 digit3[0] = ' ';

	digit3[1]= disp_value % 10 +'0';
	digit3[2]= 0;
	return (digit3) ;
}

char *num_to_str_3d(uint16_t disp_value) 
{
	digit[0]=(disp_value/100) % 10 +'0';
	if ( digit[0] == '0' )
	 digit[0] = ' ';

	digit[1]=(disp_value/10) % 10 +'0';
	if ( (digit[0] == ' ') && (digit[1] == '0') )
	 digit[1] = ' ';

	digit[2]= disp_value % 10 +'0';
	digit[3]= 0;
	return (digit) ;
}

char *num_to_str_5d(uint16_t disp_value) 
{
	digit2[0]=(disp_value/10000) % 10 +'0';
	if ( digit2[0] == '0' )
	 digit2[0] = ' ';

	digit2[1]=(disp_value/1000) % 10 +'0';
	if ( (digit2[0] == ' ') && (digit2[1] == '0') )
	 digit2[1] = ' ';

	digit2[2]=(disp_value/100) % 10 +'0';
	if (( digit2[1] == ' ' ) && (digit2[2] == '0') )
	 digit2[2] = ' ';

	digit2[3]=(disp_value/10) % 10 +'0';
	if ((digit2[2] == ' ') && (digit2[3] == '0') )
	 digit2[3] = ' ';

	digit2[4]= disp_value % 10 +'0';
	digit2[5]= 0;
	return (digit2) ;
}


