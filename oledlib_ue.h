// **********************************************
//	* ������������ ���� ���������� ���������� i2c
//	*	
//	* ��� ������������� ���������� �������� ������� ���������� � ���� � �������
//	* ��������� i2c
//	*
//	* �� ������������� ������� ������������� ���������� ��������� �����
//	**********************************************/
#include "stm8s.h"

#ifndef __OLEDLIB_UE_H
#define __OLEDLIB_UE_H
#endif

// �����������

#define I2C_PORT GPIOB			// � ����� ����� ��������� i2c
#define I2C_CLK GPIO_PIN_4 	// ��� ������������ i2c
#define I2C_DAT GPIO_PIN_2	// ��� ������ i2c
#define F_CPU	2000000				// ������� ����������������, ���� �� ������������ �������

#define OLED_Address 0x78 // ����� ������� �� ds 0x78 ��� 0x7A

#define DAT                                                0x60
#define CMD                                                0x00
#define Set_Lower_Column_Start_Address_CMD                 0x00
#define Set_Higher_Column_Start_Address_CMD                0x10
#define Set_Memory_Addressing_Mode_CMD                     0x20
#define Set_Column_Address_CMD                             0x21
#define Set_Page_Address_CMD                               0x22
#define Set_Display_Start_Line_CMD                         0x40
#define Set_Contrast_Control_CMD                           0x81
#define Set_Charge_Pump_CMD                                0x8D
#define Set_Segment_Remap_CMD                              0xA0
#define Set_Entire_Display_ON_CMD                          0xA4
#define Set_Normal_or_Inverse_Display_CMD                  0xA6
#define Set_Multiplex_Ratio_CMD                            0xA8
#define Set_Display_ON_or_OFF_CMD                          0xAE
#define Set_Page_Start_Address_CMD                         0xB0
#define Set_COM_Output_Scan_Direction_CMD                  0xC0
#define Set_Display_Offset_CMD                             0xD3
#define Set_Display_Clock_CMD                              0xD5
#define Set_Pre_charge_Period_CMD                          0xD9
#define Set_Common_HW_Config_CMD                           0xDA
#define Set_VCOMH_Level_CMD                                0xDB
#define Set_NOP_CMD                                        0xE3
#define Horizontal_Addressing_Mode                         0x00
#define Vertical_Addressing_Mode                           0x01
#define Page_Addressing_Mode                               0x02
#define Disable_Charge_Pump                                0x00
#define Enable_Charge_Pump                                 0x04
#define Column_Address_0_Mapped_to_SEG0                    0x00
#define Column_Address_0_Mapped_to_SEG127                  0x01
#define Normal_Display                                     0x00
#define Entire_Display_ON                                  0x01
#define Non_Inverted_Display                               0x00
#define Inverted_Display                                   0x01
#define Display_OFF                                         0x00
#define Display_ON                                          0x01
#define Scan_from_COM0_to_63                                0x00
#define Scan_from_COM63_to_0                                0x08
#define x_size                                             128
#define x_max                                              x_size
#define x_min                                              0
#define y_size                                             64
#define y_max                                              8
#define y_min                                              0
#define ON                                                 1
#define OFF                                                0
#define YES                                                1
#define NO                                                 0
#define ROUND                                              1
#define SQUARE                                             0
#define buffer_size                                        1024//(x_max * y_max)

typedef enum {
	POSITIVE=0,
	NEGATIVE=1
} out_mode;

// ��������� �������

void oled_i2c_setup(void); // ������������� i2c � ��� �����
void OLED_write(uint8_t ctr_dat, uint8_t oled_value); // ����� � OLED �� i2c 
void OLED_Init(void); // ������������� ������� �������� ds
void OLED_fill(uint8_t bmp_data); // ���������� ������� 
void OLED_gotoxy(uint8_t x_pos, uint8_t y_pos); // ����������� ������ �� ����� x, y
void OLED_print_char(uint8_t x_pos, uint8_t y_pos, uint8_t ch, out_mode md); // ������ ������� � ������� x,y
void OLED_print_string(uint8_t x_pos, uint8_t y_pos, uint8_t *ch, out_mode md); // ������ ������ � ������� x, y x � y � ��������, � �� ��������
void OLED_set_contrast(uint8_t contrast); // ���������� ��������
void OLED_print_int(uint8_t x_pos, uint8_t y_pos, int32_t value, out_mode md); // ����� ������
