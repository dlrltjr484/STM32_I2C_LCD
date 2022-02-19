# STM32_I2C_LCD

***************************************************************************

2022-02-20

Code for I2C LCD

Used STM32F103RB and STM32F411RE
I2C1

GND
VCC(5V)
SDA
SCL
_________________________________________________
0x00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
0x40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
_________________________________________________

LCD init after i2c1 init --> Init_LCD();

write_string( 0x02 , "ex");
write_string( 0x4b , "ex");
write_string( 0x09 , num_to_str_5d(variable));

***************************************************************************