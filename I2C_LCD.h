#define PCF8574_ADD0 0x4E // PCF8574: 0x4X, PCF8574A: 0x7X

void wr_PCF8574(uint8_t dev_add, uint8_t wdata);
void write_LCDcmd_4b(uint8_t lcd_cmd);
void write_LCDcmd_8b(uint8_t lcd_cmd);
void write_LCDdata_8b (uint8_t lcd_dat);
void Init_LCD(void);
void write_string (uint8_t start_x, char *p);

char digit[12];
char digit2[12];
char digit3[12];

char *num_to_str_2d(uint16_t disp_value); 
char *num_to_str_3d(uint16_t disp_value); 
char *num_to_str_5d(uint16_t disp_value); 