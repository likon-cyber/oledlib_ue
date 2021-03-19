#include "oledlib_ue.h"
#include "stm8s.h"
#include "fonts.h"


/*************************************************************
 * Бибилотека для работы с OLED дисплеями I2C
 * Работает в режиме опроса без прерываний
 *
 * Вывод текста на экран.
 *
 *
 *
 *
 *************************************************************/

// Функция инициализации i2c и его пинов
void oled_i2c_setup(void)
{
    I2C_DeInit();
    I2C_Init(100000,
             0x01,
             I2C_DUTYCYCLE_2,
             I2C_ACK_CURR,
             I2C_ADDMODE_7BIT,
             (CLK_GetClockFreq() / 1000000));
    I2C_Cmd(ENABLE);
		GPIO_Init(I2C_PORT, I2C_CLK, GPIO_MODE_OUT_OD_HIZ_FAST);
		GPIO_Init(I2C_PORT, I2C_DAT, GPIO_MODE_OUT_OD_HIZ_FAST);
}

// Пишем в OLED по i2c 
// control - это выбор следующим будет команда или данные
// value - следующий байт
void OLED_write(uint8_t ctr_dat, uint8_t oled_value) 
{
 while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
 I2C_GenerateSTART(ENABLE);
 while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
 I2C_Send7bitAddress(OLED_Address, I2C_DIRECTION_TX);
 while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
 I2C_SendData(ctr_dat);
 while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
 I2C_SendData(oled_value);
 while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 I2C_GenerateSTOP(ENABLE);
}

// Инициализация дисплея согласно ds
void OLED_Init(void)
{
 OLED_write(CMD, (Set_Display_ON_or_OFF_CMD | Display_OFF));
 OLED_write(CMD, Set_Multiplex_Ratio_CMD);
 OLED_write(CMD, 0x3F);
 OLED_write(CMD, Set_Display_Offset_CMD);
 OLED_write(CMD, 0x00);
 OLED_write(CMD, Set_Display_Start_Line_CMD);
 OLED_write(CMD, (Set_Segment_Remap_CMD | Column_Address_0_Mapped_to_SEG127));
 OLED_write(CMD, (Set_COM_Output_Scan_Direction_CMD | Scan_from_COM63_to_0));
 OLED_write(CMD, Set_Common_HW_Config_CMD);
 OLED_write(CMD, 0x12);
 OLED_write(CMD, Set_Contrast_Control_CMD);
 OLED_write(CMD, 0x9F);
 OLED_write(CMD, Set_Entire_Display_ON_CMD);
 OLED_write(CMD, Set_Normal_or_Inverse_Display_CMD);
 OLED_write(CMD, Set_Display_Clock_CMD);
 OLED_write(CMD, 0x80);
 OLED_write(CMD, Set_Pre_charge_Period_CMD);
 OLED_write(CMD, 0x25);
 OLED_write(CMD, Set_VCOMH_Level_CMD);
 OLED_write(CMD, 0x20);
 OLED_write(CMD, Set_Page_Address_CMD);
 OLED_write(CMD, 0x00);
 OLED_write(CMD, 0x07);
 OLED_write(CMD, Set_Page_Start_Address_CMD);
 OLED_write(CMD, Set_Higher_Column_Start_Address_CMD);
 OLED_write(CMD, Set_Lower_Column_Start_Address_CMD);
 OLED_write(CMD, Set_Memory_Addressing_Mode_CMD);
 OLED_write(CMD, 0x02);
 OLED_write(CMD, Set_Charge_Pump_CMD);
 OLED_write(CMD, 0x14);
 OLED_write(CMD, (Set_Display_ON_or_OFF_CMD | Display_ON));
}

// Заполнение дисплея 
void OLED_fill(uint8_t bmp_data)
{
    uint8_t x_pos = 0x00;
    uint8_t page = 0x00;
    for(page = 0; page < y_max; page++)
    {
        OLED_gotoxy(x_min, page);
				while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
        I2C_GenerateSTART(ENABLE);
        while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
        I2C_Send7bitAddress(OLED_Address, I2C_DIRECTION_TX);
        while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
        I2C_SendData(DAT);
        while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));   
        for(x_pos = x_min; x_pos < x_max; x_pos++)
        {
           I2C_SendData(bmp_data);
                 while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 
        }
        I2C_GenerateSTOP(ENABLE); 
    }
}

// Перестаивть курсор на точку x, y
void OLED_gotoxy(uint8_t x_pos, uint8_t y_pos)
{
     OLED_write(CMD, (Set_Page_Start_Address_CMD + y_pos));
     OLED_write(CMD, ((x_pos & 0x0F) | Set_Lower_Column_Start_Address_CMD));
     OLED_write(CMD, (((x_pos & 0xF0) >> 0x04) | Set_Higher_Column_Start_Address_CMD));
}

// Выводим на экран символ используя массив font_regular позиция x, y, символ, режим вывода
void OLED_print_char(uint8_t x_pos, uint8_t y_pos, uint8_t ch, out_mode md)
{
  uint8_t s = 0x00;
  uint8_t chr = 0x00;
  chr = (ch - 0x20);
  if(x_pos > (x_max - 0x06))
  {
    x_pos = 0x00;
    y_pos++;
  }
  OLED_gotoxy(x_pos, y_pos);
  for(s = 0x00; s < 0x06; s++)
  {
    if(md==POSITIVE)
		OLED_write(DAT, font_regular[chr][s]);
		else
		OLED_write(DAT, ~font_regular[chr][s]);
  }
}

// Вывод строки позици x, y? строка заканчивающаяся \n и режим вывода
void OLED_print_string(uint8_t x_pos, uint8_t y_pos, uint8_t *ch, out_mode md)
{
  do
  {
    OLED_print_char(x_pos, y_pos, *ch++, md);
    x_pos += 0x06;
  }while((*ch >= 0x20) && (*ch <= 0x7F) && (*ch != '\n'));
}

// Вывод на экран целого позиция x, y, перменная, режим вывода
void OLED_print_int(uint8_t x_pos, uint8_t y_pos, int32_t value, out_mode md)
{
   uint8_t ch[7] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, '\n'};
  if(value < 0)
  {
    ch[0] = 0x2D;
    value = -value;
  }
  else
  {
    ch[0] = 0x20;
  }
  if(value > 9999)
  {
    ch[1] = ((value / 10000) + 0x30); 
    ch[2] = (((value % 10000) / 1000) + 0x30); 
    ch[3] = (((value % 1000) / 100) + 0x30); 
    ch[4] = (((value % 100) / 10) + 0x30); 
    ch[5] = ((value % 10) + 0x30);
  }
  else if((value > 999) && (value <= 9999))
  {
    ch[1] = ((value / 1000) + 0x30); 
    ch[2] = (((value % 1000) / 100) + 0x30); 
    ch[3] = (((value % 100) / 10) + 0x30); 
    ch[4] = ((value % 10) + 0x30);
    ch[5] = 0x20;
  }
  else if((value > 99) && (value <= 999))
  {
    ch[1] = ((value / 100) + 0x30); 
    ch[2] = (((value % 100) / 10) + 0x30); 
    ch[3] = ((value % 10) + 0x30);
    ch[4] = 0x20;
    ch[5] = 0x20;
  }
  else if((value > 9) && (value <= 99))
  {
    ch[1] = ((value / 10) + 0x30); 
    ch[2] = ((value % 10) + 0x30);
    ch[3] = 0x20;
    ch[4] = 0x20;
    ch[5] = 0x20;
  }
  else
  {
    ch[1] = ((value % 10) + 0x30);
    ch[2] = 0x20;
    ch[3] = 0x20;
    ch[4] = 0x20;
    ch[5] = 0x20;
  }
  OLED_print_string(x_pos, y_pos, ch, md);
}
	
// Установка контраста
void OLED_set_contrast(uint8_t contrast)
{
 OLED_write(CMD, Set_Contrast_Control_CMD);
 OLED_write(CMD, contrast);

}