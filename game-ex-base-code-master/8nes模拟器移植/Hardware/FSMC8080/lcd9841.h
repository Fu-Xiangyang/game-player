#ifndef __LCD9841_H
#define __LCD9841_H	

#include "gd32f4xx.h"
#include "systick.h"
#include "lcd9841.h"
#include "stdio.h"



/********���������λ�˿��벢�ڴ���ѡ��,��������ѡ��*********/
/************************������ѡ��**************************/
/******��һ��ѡ��*********/
//���ڣ�����ѡ��
//1������:��8λ��16λ, ��8λ
//0������:3��,4��
#define  PS 1

/*******������ѡ��********
//LCD�ӿ��ж�ѡ�񣺶�д���ܺ���  
//MCU=0,00: L8BIT 0K
//MCU=1,01: 16BIT	OK
//MCU=2,10: W4SPI	OK
//MCU=3,11: W3SPI	OK
//MCU=4,--: H8BIT	OK
//MCU=5,--: PC0-7���������ݿڣ�DB0-7	OK
***************/
#define  MCU  1

/********������ѡ��*******/
//����8/16λѡ�񣺣ƣӣͣ�����
//1��16λ
//0��L8λ
//REG16,��8λ,���ڲ������
#define  MCu8or16BIT  1

/*******���Ĳ�ѡ��********/
//SD���Զ����£���������ѡ��20171125//
//1���������»�����
//0���Զ����»�����
#define keypress 	1

/*********************�ţģ�*********************/
/*********************�ţģ�*********************/


/************************************************/
//LCD��Ҫ������
typedef struct  
{										    
	uint16_t  width;		//LCD ���
	uint16_t  height;		//LCD �߶�
	uint16_t  id;				//LCD ID
	uint8_t   dir;			//���������������ƣ�0��������1��������	
	uint16_t 	wramcmd;	//��ʼдgramָ��
	uint16_t   setxcmd;	//����x����ָ��
	uint16_t   setycmd;	//����y����ָ��  
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint16_t   POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t   BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

////////////////////////////////////////////////
//-----------------LCD�˿ڶ���----------------//

/**************************************************************/
/*                STM32F4�ڲ�FSCM����                       */
/***************************************************************
STM32F4 �� FSMC �洢�� 1�� Bank1������Ϊ 4 ������ÿ�������� 64M �ֽڿռ䣬
ÿ�������ж����ļĴ����������ӵĴ洢���������á�
Bank1 �� 256M �ֽڿռ��� 28 ����ַ�ߣ� HADDR[27:0]��Ѱַ��
���� HADDR ���ڲ� AHB ��ַ ���ߣ����� HADDR[25:0] �����ⲿ�洢����ַFSMC_A[25:0]��
�� HADDR[26:27]�� 4 ��������Ѱַ��FSMC_A[25:0]��2^26 =0X0400 0000 = 64MB,
ÿ��BANK ��4*64MB = 256MB,�� HADDR[26:27]�� 4 ��������Ѱַ��������ʾ��
Bank1 ��ѡ�� Ƭѡ�ź� ��ַ��Χ [25:0]	 HADDR[27:26]
�� 1 �� FSMC_NE1 0X6000,0000~63FF,FFFF 00
�� 2 �� FSMC_NE2 0X6400,0000~67FF,FFFF 01    
�� 3 �� FSMC_NE3 0X6800,0000~6BFF,FFFF 10
�� 4 �� FSMC_NE4 0X6C00,0000~6FFF,FFFF 11

LCD_CS�ӵ�PD7(FSCM_NE1)����ѡ��NORSRAM1
ѡ��BANK1-BORSRAM1 ����TFT,��ַ��Χ0X6000 0000 ~ 0X3FFF FFFF  
FSMC_A11��LCD��DC(�Ĵ�������/��ַѡ���)
16λ:A0��Ч,��ʱA1�൱������A0,ע������ʱSTM32�ڲ�������һλ����!
RS:A11  0x3FF    ������Ч��A0λ�õ�0x3FF<<1=0x7FE 
�Ĵ�����ַ = 0X6000 0000
RAM����ַ =  0X6001 FFFE = 0X6000 0000+2^11��A11���Ծ���11�η���*2 = 0X6000 0000 + 0X001 FFFE = 0X6001 FFFE
��ַ�߲�ͬ��Ҫ���¼���RAM����ַ
*****************************************************************/
/****************************************************************/
/****************************************************************/

//====��λ������λѡ��20171125====//
#if MCu8or16BIT   //16λ

//LCD��ַ�ṹ�� 16BIT
typedef struct
{
	uint16_t  LCD_REG;
	uint16_t  LCD_RAM;
} LCD_TypeDef;

//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 			    
//LCD>>16BIT FSMC   OK
#define LCD_BASE    ((uint32_t )(0x6c000000 |  0X000007FE))  //    111 1111 1111
#define LCD         ((LCD_TypeDef *) LCD_BASE)

#else	//8λ

//LCD��ַ�ṹ��8BIT
typedef struct
{
	uint8_t  LCD_REG;
	uint8_t  LCD_RAM;
} LCD_TypeDef;

//LCD>>8BIT FSMC  OK
#define LCD_BASE        ((uint32_t )(0x60000000 | 0X0FFFF))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

#endif


//#define LCD_BLK PBout(1)	// DS0 
#define LCD_BLK_ON  gpio_bit_set(GPIOD,GPIO_PIN_13)	// PD13
#define LCD_BLK_OFF gpio_bit_reset(GPIOD,GPIO_PIN_13)	// PD13

//===SPI===//
#define SPI_CS(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)
						
#define SPI_SCL(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)
						
#define SPI_SDA(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)
						
#define SPI_A0(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)

//#define SPI_SCLK PDout(5)
//						
//#define SPI_SDIN PDout(4)	

//#define SPI_A0IN PDout(11)					
//						
/*
//�ο�д��
//���ʹ�ÿ���IO�������¾䣬�����ʹ�ã���ȥ�����ɣ�
//ʹ�ÿ���IO��ˢ�����ʿ��Դﵽ28֡ÿ�룡
//��ͨIO��ֻ��14֡ÿ�룡   		
*/
						
//#ifdef LCD_FAST_IO //����IO

#define	LCD_CS_SET  GPIOD->BSRRH=1<<7    //Ƭѡ�˿�  	 PD7
#define	LCD_RS_SET	GPIOD->BSRRH=1<<11   //����/����  	 PD11	   
#define	LCD_WR_SET	GPIOD->BSRRH=1<<5    //д����			 PD5
#define	LCD_RD_SET	GPIOD->BSRRH=1<<4    //������			 PD4
#define	LCD_RES_SET	GPIOD->BSRRH=1<<13   //������		   PD13
								    
#define	LCD_CS_CLR  GPIOD->BSRRL=1<<7     //Ƭѡ�˿�  	 PD7
#define	LCD_RS_CLR	GPIOD->BSRRL=1<<11    //����/����		 PD11	   
#define	LCD_WR_CLR	GPIOD->BSRRL=1<<5     //д����			 PD5
#define	LCD_RD_CLR	GPIOD->BSRRL=1<<4     //������			 PD4
#define	LCD_RES_CLR	GPIOD->BSRRL=1<<13    //������		   PD13

#define DATAOUTD(x) GPIOD->ODR=x; //�������    ��
#define DATAIND     GPIOD->IDR;   //��������	 	

#define DATAOUTE(x) GPIOE->ODR=x; //�������    
#define DATAINE     GPIOE->IDR;   //��������	 		

//#else //����IO

#define	LCD_CS	PDout(7)  //Ƭѡ�˿�  	    
#define	LCD_RS	PDout(11)  //����/����      	   
#define	LCD_WR	PDout(5)  //д����			
#define	LCD_RD	PDout(4)  //������			
							    
//#endif

/*
//PB0~15,��Ϊ������
#define DATAOUT(x) GPIOB->ODR=x; //�������    ��δ����
#define DATAIN     GPIOB->IDR;   //��������	 	δ����				

//���ο�������
//д8λ���ݺ���
//�ú궨��,����ٶ�.

#ifdef LCD_FAST_IO //����IO
  #define LCD_WR_DATA8(DH,DL){\
      LCD_RS_SET;\
      LCD_CS_CLR;\
      DATAOUT(DH);\
      LCD_WR_CLR;\
      LCD_WR_SET;\
      DATAOUT(DL);\
      LCD_WR_CLR;\
      LCD_WR_SET;\
      LCD_CS_SET;\
      }

  #define LCD_WR_DATA(data){\
      unsigned char m,n;	\
      m=data>>8;\
      n=data;\
      LCD_WR_DATA8(m,n);\
     }  
#else  //����IO
  #define LCD_WR_DATA8(DH,DL){\
      LCD_RS=1;\
      LCD_CS=0;\
      DATAOUT(DH);\
      LCD_WR=0;\
      LCD_WR=1;\
      DATAOUT(DL);\
      LCD_WR=0;\
      LCD_WR=1;\
      LCD_CS=1;\
      } 
  #define LCD_WR_DATA(data){\
       unsigned char m,n;	\
       m=data>>8;\
       n=data;\
       LCD_WR_DATA8(m,n);\
      }  	
#endif		
*/

///////////////////////////////////////////						
//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define BRED         0XF81F
#define GRED 			   0XFFE0
#define GBLUE			   0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

//*******************************************************************************//
void LCD_CtrlLinesConfig(void);
void LCD_FSMCConfig(void);

void LCD_Config(void);//�̣ã����ò���

void LCD_SPICtrlLinesConfig(void); //��ԣӣУɵ�������
void LCD_H8BitCtrlLinesConfig(void); //��Ը�8λ����������20171204

void LCD_PC0_7CtrlLinesConfig(void); //���PC0_7����������20180314

void LCD_RESET(void);  //�̣ãĽ��и�λ����

void LCD_WriteByteSPI(unsigned char byte);

/*****4W-SPI******/
void SPI4W_WriteRegIndex(uint16_t  Index);
void SPI4W_WriteData(uint16_t  dat);
void SPI4W_WriteDisplayData(uint16_t  dat);

/*****3W-SPI******/
void SPI3W_WriteRegIndex(uint16_t  Index);
void SPI3W_WriteData(uint16_t  dat);
void SPI3W_WriteDisplayData(uint16_t  dat);

/*****��8BIT***20171203***/
void LCD_WR_REG8(uint16_t  Index);
void LCD_WR_DATA8(uint16_t  Data);

/*****8/16BIT******/
void LCD_WR_REG(uint16_t  regval); //16BIT REG
void LCD_WR_8REG(uint16_t  regval);//8BIT REG
void LCD_WR_DATA(uint16_t  data);
void LCD_WR_8DATA(uint16_t  data);

void LCD_WriteReg(uint16_t  LCD_Reg, uint16_t  LCD_RegValue);

uint16_t  LCD_RD_DATA(void);
uint16_t  LCD_RD_DATA8(void);

uint16_t  LCD_SPI_RD_DATA(void);
	
uint16_t  LCD_ReadReg(uint16_t  LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t  RGB_Code);
uint16_t  LCD_BGR2RGB(uint16_t  c);
void opt_delay(uint8_t  i);
uint16_t   LCD_ReadPoint(uint16_t  x,uint16_t  y); 	//���� 

void LCD_Clear(uint16_t  Color);	//����
void LCD_Display(uint16_t  color);
void LCD_DisplayJiuGongGe(void);
void LCD_Frame(uint16_t  RGB_Data,uint16_t  f_data);
void Show_Gray(void);
void GRAY_TEST16(void);

void Check_Crosstalk(void);
void Check_Flicker(void);

void LCD_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  color);		   				//��䵥ɫ
void LCD_Color_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  *color);				//���ָ����ɫ

void LCD_Fill_Area( uint16_t  Xpos_S, uint16_t  Ypos_S, uint16_t  Xpos_E, uint16_t  Ypos_E, uint16_t  color );

void DisplayPicture(void);
void DisplayPicture1(void);

//void LCD_SetCursor(uint16_t  Xpos, uint16_t  Ypos);	//�Ʒ�����TFTLCD_Init.c    20171120//���ù��

void LCD_DrawPoint(uint16_t  x,uint16_t  y);											//����
void LCD_Fast_DrawPoint(uint16_t  x,uint16_t  y,uint16_t  color);								//���ٻ���
void LCD_DrawLine(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2);							//����
void LCD_DrawRectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2);		   				//������
void LCD_Draw_Circle(uint16_t  x0,uint16_t  y0,uint8_t  r);						 			//��Բ

void LCD_ShowChar(uint16_t  x,uint16_t  y,uint8_t  num,uint8_t  size,uint8_t  mode);						//��ʾһ���ַ�
uint32_t  LCD_Pow(uint8_t  m,uint8_t  n);
void LCD_ShowNum(uint16_t  x,uint16_t  y,uint32_t  num,uint8_t  len,uint8_t  size);  						//��ʾһ������
void LCD_ShowxNum(uint16_t  x,uint16_t  y,uint32_t  num,uint8_t  len,uint8_t  size,uint8_t  mode);				//��ʾ ����
void LCD_ShowString(uint16_t  x,uint16_t  y,uint16_t  width,uint16_t  height,uint8_t  size,uint8_t  *p);		//��ʾһ���ַ���,12/16����

void LCD_SSD_BackLightSet(uint8_t  pwm);							//SSD1963 �������
void SSD1963_SetRGB(void);

//LCD�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		210		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		45		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		34		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		10		//��ֱǰ��
//���¼����������Զ�����
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

///////////SD///////////
uint16_t  pic_get_tnum(uint8_t  *path);
void SD_DisPic(void);
void Bin_ShowPic(unsigned short x,unsigned short y,char * fname);//20171201
void ShowPicBin(void);


void display_colorbar_H(void);

#endif
