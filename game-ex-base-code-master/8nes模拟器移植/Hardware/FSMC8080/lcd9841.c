#include "lcd9841.h"
#include "stdlib.h" 

#include "font.h"
#include "TFTLCD_Init.h"
#include "gImage_128128.h"
#include "flowerPic.h" //����ͼƬ240*320ͼƬ

#define Pic  0    //��Ӧ���������ͼƬ��1Ϊ���Ͽ���ͼƬ320*320ͼƬ��Ҫô240*320ͼƬ,���أӣĲ��ܴ�320320Pic���ڴ治��

//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint16_t  POINT_COLOR=0x0000;	//������ɫ
uint16_t  BACK_COLOR=0xFFFF;  //����ɫ 
  
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void LCD_CtrlLinesConfig(void)
{ 	
		/* ���� PD13 */
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP,GPIO_PIN_13);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13);

	 //rcu_periph_clock_enable(RCU_GPIOD);
	 rcu_periph_clock_enable(RCU_GPIOE);
	 rcu_periph_clock_enable(RCU_GPIOG);
	
	/* ���Ÿ������� */
	gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_0|GPIO_PIN_1
	|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15); // EXMC����

	gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_7|GPIO_PIN_8
		|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15); // EXMC����
	
	gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_0|GPIO_PIN_12); // EXMC����
	
	//FSMC-D0--D15: 
  // D0   D1   D2   D3   D4   D5   D6   D7   D8   D9   D10   D11   D12   D13  D14   D15       
  //PD14 PD15  PD0  PD1  PE7  PE8  PE9 PE10 PE11 PE12  PE13  PE14  PE15  PD8  PD9   PD10 
  //FSMC_CMD:
  //FSMC_NOE      FSMC_NWE       FSMC_NE4     FSMC_A10
  //LCD_RD        LCD_WR         LCD_CS       LCD_RS  
  //PD4            PD5           PG12          PG0	

   gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_0|GPIO_PIN_1
		|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15);
   gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_1
		|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15);
	
	

   gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_7|GPIO_PIN_8
		|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
   gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7|GPIO_PIN_8
		|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	 

		/*EXMC_NE3  PG12   EXMC_A10  PG0 */
	
   gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_0|GPIO_PIN_12);
   gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_12);
	 

		LCD_BLK_ON;  // �������� 
	//LCD_BLK_OFF; // �رձ���
}

void LCD_FSMCConfig(void)
{
	exmc_norsram_parameter_struct nor_init_struct; // EXMC�����ṹ��
  exmc_norsram_timing_parameter_struct readWriteTiming; // EXMCʱ������ṹ��
	exmc_norsram_timing_parameter_struct writeTiming; // EXMCʱ������ṹ��
	
	 /* EXMC clock enable */
	rcu_periph_clock_enable(RCU_EXMC);
	
 /* configure timing parameter */
	/* ��дʱ�� */
	readWriteTiming.asyn_access_mode = EXMC_ACCESS_MODE_A; //ģʽA    
	readWriteTiming.syn_data_latency = EXMC_DATALAT_2_CLK;
	readWriteTiming.syn_clk_division = EXMC_SYN_CLOCK_RATIO_2_CLK;
	readWriteTiming.bus_latency = 0;
	readWriteTiming.asyn_data_setuptime = 72;			//��Ҫ360ns���ݱ���ʱ��Ϊ72��HCLK	=5*72=360ns
	readWriteTiming.asyn_address_holdtime = 0x00; //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	readWriteTiming.asyn_address_setuptime = 18; //��Ҫ96ns  ��ַ����ʱ�䣨ADDSET��Ϊ19��HCLK 1/200M=5ns*19=96ns	

//	readWriteTiming.asyn_data_setuptime = 150;			//��Ҫ360ns���ݱ���ʱ��Ϊ72��HCLK	=5*72=360ns
//	readWriteTiming.asyn_address_holdtime = 0x00; //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
//	readWriteTiming.asyn_address_setuptime = 50; //��Ҫ96ns  ��ַ����ʱ�䣨ADDSET��Ϊ19��HCLK 1/200M=5ns*19=96ns	

	
	/* ��дʱ�� */
	writeTiming.asyn_access_mode = EXMC_ACCESS_MODE_A; //ģʽA    
	writeTiming.syn_data_latency = EXMC_DATALAT_2_CLK;
	writeTiming.syn_clk_division = EXMC_SYN_CLOCK_RATIO_2_CLK;
	writeTiming.bus_latency = 0;
//	writeTiming.asyn_data_setuptime = 11;			//��Ҫ54ns���ݱ���ʱ��Ϊ11��HCLK	=5*11=54ns
//	writeTiming.asyn_address_holdtime = 0x00; //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
//	writeTiming.asyn_address_setuptime = 10; //��Ҫ54ns  ��ַ����ʱ�䣨ADDSET��Ϊ11��HCLK 1/200M=5ns*11=54ns	


	writeTiming.asyn_data_setuptime = 4;			//��Ҫ18ns���ݱ���ʱ��Ϊ11��HCLK	=5*4=18ns
	writeTiming.asyn_address_holdtime = 0x00; //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	writeTiming.asyn_address_setuptime = 3; //��Ҫ18ns  ��ַ����ʱ�䣨ADDSET��Ϊ11��HCLK 1/200M=5ns*4=18ns	


	/* configure EXMC bus parameters */
	nor_init_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION3;
	// FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;// FSMC_Bank1_NORSRAM4;//  ����ʹ��NE1 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
	nor_init_struct.address_data_mux = DISABLE; // ���������ݵ�ַ
	nor_init_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;  // FSMC_MemoryType_SRAM;  //SRAM 
	
	switch(MCU)   
	{	
			case 0:{nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_8B; }  break; //�洢�����ݿ��Ϊ8bit  
			case 1:{nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;}  break; //�洢�����ݿ��Ϊ16bit 
			default:nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;; break;
	} 
		
	  nor_init_struct.burst_mode = DISABLE;
		nor_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
		nor_init_struct.asyn_wait = DISABLE; 
		nor_init_struct.wrap_burst_mode = DISABLE;	
		nor_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;
		nor_init_struct.memory_write = ENABLE;//  �洢��дʹ��
	  nor_init_struct.nwait_signal = DISABLE;	
	  nor_init_struct.extended_mode = ENABLE;  // ��дʹ�ò�ͬ��ʱ��
		nor_init_struct.write_mode = EXMC_ASYN_WRITE;
	
	nor_init_struct.read_write_timing = &readWriteTiming;//��дʱ��
	nor_init_struct.write_timing = &writeTiming;//дʱ��
	
	
   exmc_norsram_init(&nor_init_struct); //��ʼ��FSMC����
   /* enable the EXMC bank0 NORSRAM */
   exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION3);   // ʹ��BANK1 	
	
}

/*******************************************************************************
* Function Name  : void LCD_Config(void)   
* Description    : �ԣ̣ãĲ��ڣ���������ѡ��PS��1�����ڣ�0������ , �̣ã�����ѡ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Config(void)
{
		//����8/16BITҪ������
		LCD_CtrlLinesConfig();     //����STM32�ڶ�Ӧ�ԣƣԽӿ�
		LCD_FSMCConfig();          //��ʼ��FSMC��Ӧ�ԣƣ�	
//	printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);
		
}

/*******************************************************************************
* Function Name  : LCD_Reset
* Description    : �ԣ̣ãĽ��и�λ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_RESET(void)
{   	
	/* ��λRST PD12 */
	rcu_periph_clock_enable(RCU_GPIOD);
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_12);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12);
	
	gpio_bit_set(GPIOD, GPIO_PIN_12);	
	delay_1ms(10);
	gpio_bit_reset(GPIOD, GPIO_PIN_12);	  
	delay_1ms(50);					   
	gpio_bit_set(GPIOD, GPIO_PIN_12);			 
	delay_1ms(120);		 
}

/*******************************************************************************
* Function Name  : LCD_WR_REG����
* Description    : д�Ĵ�������,regval:�Ĵ���ֵ
* Input          : regval
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_REG(uint16_t  regval)			//��ʼ��ʱ��Ӧ��16BIT REG
{  
//	printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);
	LCD->LCD_REG=regval;  
	//printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);

}

void LCD_WR_8REG(uint16_t  regval)  //��ʼ��ʱ��Ӧ��8BIT  REG
{   	
		// printf("LCD->LCD_REG: %p\r\n",&LCD->LCD_REG);
		LCD->LCD_REG=regval; 		
}

/*******************************************************************************
* Function Name  : LCD_WR_DATA   
* Description    : дLCD���ݺ���,data:Ҫд���ֵ
* Input          : data
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_DATA(uint16_t  data)  //��ʼ��ʱ��Ӧ��16BIT DATA
{
  LCD->LCD_RAM=data;
}

void LCD_WR_8DATA(uint16_t  data)  //��ʼ��ʱ��Ӧ��8BIT DATA
{
  LCD->LCD_RAM=data;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg    
* Description    : д�Ĵ�������,LCD_Reg:�Ĵ�����ַ��дLCD���ݺ���,LCD_RegValue:Ҫд�������
* Input          : LCD_Reg,LCD_RegValue
* Output         : None
* Return         : None
*��һ�����ڣ���λ�Ĵ�����ʽ�ϵĶ࣬���ô����Σ�Ҫ�ǣ�λ�Ĵ����ɣ��ڸ�һ��ֻ��һ�εģ�
*******************************************************************************/
void LCD_WriteReg(uint16_t  LCD_Reg,uint16_t  LCD_RegValue)  
{	
		LCD->LCD_REG = LCD_Reg;		   	 
		LCD->LCD_RAM = LCD_RegValue; 		
}

/*******************************************************************************
* Function Name  : LCD_RD_DATA     �٣�
* Description    : ��LCD���ݺ���
* Input          : None
* Output         : None
* Return         : ������ֵ
*******************************************************************************/
uint16_t  LCD_RD_DATA(void)  //�⺯����
{
	uint16_t  ram;			//��ֹ���Ż�
	ram=LCD->LCD_RAM;	
	return ram;	 
}


void delay_us(uint16_t us)
{
	while(us--){
		__NOP;
		__NOP;
		__NOP;
		__NOP;
		__NOP;
		__NOP;
	}
}

/*******************************************************************************
* Function Name  : LCD_ReadReg     
* Description    : ��IC�Ĵ�������,���Ĵ���
* Input          : LCD_Reg:�Ĵ�����ַ
* Output         : None
* Return         : ������ֵ
*******************************************************************************/
uint16_t  LCD_ReadReg(uint16_t  LCD_Reg)
{		
  uint16_t  i = 0;	
	uint16_t  info;


		LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
		delay_us(5);
		info=LCD_RD_DATA();
		info<<=8;
		info|=LCD_RD_DATA();
		i = 2;
	
  if(i == 1)
    return LCD_RD_DATA();	
	else if(i == 2)
		return info;
	else
	  return 0;
}   

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare  ��ʼдGRAM   �ģ٣�
* Description    : ��ʼдIC-GRAM�Ĵ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	
	//TFT_WriteRAM(); //��TFTLCD_Init.c����
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM    
* Description    : LCDдGRAM�Ĵ�������
* Input          : RGB_Code:��ɫֵ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(uint16_t  RGB_Code)
{
  LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}

/*******************************************************************************
* Function Name  : LCD_BGR2RGB     �٣�
* Description    : GBR��ʽ����.��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ,ͨ���ú���ת��.
* Input          : c:GBR��ʽ����ɫֵ
* Output         : None
* Return         : ����ֵRGB��ʽ����ɫֵ
*******************************************************************************/
uint16_t  LCD_BGR2RGB(uint16_t  c)
{
	uint16_t   r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 

/*******************************************************************************
//��mdk -O1ʱ���Ż�ʱ��Ҫ����    �٣�
//��ʱi
*******************************************************************************/
void opt_delay(uint8_t  i)
{
	while(i--);
}

/*******************************************************************************
* Function Name  : LCD_ReadPoint    �٣�
* Description    :��ȡ��ĳ�����ɫֵ
* Input          : x,y:����
* Output         : None
* Return         : ����ֵ�˵����ɫ����ɫֵ
*******************************************************************************/
uint16_t  LCD_ReadPoint(uint16_t  x,uint16_t  y)
{
 	uint16_t  r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X2E);//9341/6804/3510/1963 ���Ͷ�GRAMָ��
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 ���Ͷ�GRAMָ��
	else LCD_WR_REG(0X22);      		 			//����IC���Ͷ�GRAMָ��
	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,��ʱ2us	    
 	r=LCD_RD_DATA();								//dummy Read	   
	if(lcddev.id==0X1963)return r;					//1963ֱ�Ӷ��Ϳ��� 
	opt_delay(2);	  
 	r=LCD_RD_DATA();  		  						//ʵ��������ɫ
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)		//9341/NT35310/NT35510Ҫ��2�ζ���
 	{
		opt_delay(2);	  
		b=LCD_RD_DATA(); 
		g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	} 
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0XB505||lcddev.id==0XC505)return r;	//�⼸��ICֱ�ӷ�����ɫֵ
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
	else return LCD_BGR2RGB(r);						//����IC
}	
	

/*******************************************************************************
* Function Name  : LCD_Clear   modify YZ
* Description    ://��������   
* Input          : color:Ҫ���������ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(uint16_t  color)
{
	//uint16_t  i,j;
	uint32_t index=0;     
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	
	//LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	BlockWrite(0,lcddev.width,0,lcddev.height);
	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	
	
		for(index=0;index<totalpoint;index++)
		{
			LCD_WriteRAM(color);
		 // LCD->LCD_RAM=color;
		}		
} 

/*******************************************************************************
* Function Name  : LCD_Display
* Description    : ��ʾ���ɫ
* Input          : color:Ҫ��ʾ�����ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display(uint16_t  color)
{
	uint16_t  i,j;
	
	//uint16_t  sx=0,sy=0;
	//uint16_t  twidth,theight;
	//twidth=sx+lcddev.width-1;
	//theight=sy+lcddev.height-1;

	BlockWrite(0,lcddev.width,0,lcddev.height);
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);  //for SPFD5408  ILI9481 OK  20180329    ����BlockWrite����д��20180727
	
	for(i=0;i<lcddev.height;i++)
	{
		for(j=0;j<lcddev.width;j++)
		{	
			LCD_WriteRAM(color);
      //LCD->LCD_RAM=color;			
		}
	}
}

/*******************************************************************************
* Function Name  : LCD_DisplayJiuGongGe
* Description    : �Ź�����ʾ:RGB-BGR-RGB
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayJiuGongGe(void)
{
	uint16_t  Row,Column,aa;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);
	
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);  //for SPFD5408  ILI9481 OK  20180329    ����BlockWrite����д��20180727
	
	aa=lcddev.height/3;

	for(Row=0;Row<aa;Row++)
	{
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800);
      //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0);
      //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);
      //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width%3);Column++) 	LCD_WriteRAM(0x001f);
      //LCD->LCD_RAM=0x001f;
	}

	for(Row=0;Row<aa;Row++)
	{			  
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);
    //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800);		
    //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0);
    //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0x07e0); 
    //LCD->LCD_RAM=0x07e0;
	}

	for(Row=0;Row<aa;Row++)
	{			  				   
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0); 
    //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);  
    //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800); 
    //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0xf800);  
    //LCD->LCD_RAM=0xf800;
	}

	for(Row=0;Row<(lcddev.height%3);Row++)
	{			  				   
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0); 
    //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);  
    //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800); 
    //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0xf800);  
    //LCD->LCD_RAM=0xf800;
	}
}

/*******************************************************************************
* Function Name  : LCD_Frame
* Description    : �ı��ߣ��м���������䵥����ɫ  
* Input          :RGB_DataΪ�м䴿ɫ�� f_dataΪ�ı߿���ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Frame(uint16_t  RGB_Data,uint16_t  f_data)
{
	uint16_t  i,j;

	//BlockWrite(50,100,50,100);
	uint16_t  x = 100;
	uint16_t  y = 100;
	BlockWrite(0,x,0,y);
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);  //for SPFD5408 ,ILI9481 OK 20180326     ����BlockWrite����д��20180727
	
//	for(i=0;i<1;i++)  // ����һ�� 
//	{
//		for(j=0;j<x;j++)	LCD_WriteRAM(f_data);	
//  //LCD->LCD_RAM=f_data;
//	}
	
	for (i=0;i<y;i++)
	{
	//	for (j=0;j<1;j++)	LCD_WriteRAM(f_data);  // ���һ��
    //LCD->LCD_RAM=f_data;
		for (j=0;j<x;j++)	
		LCD_WriteRAM(RGB_Data);	
    //LCD->LCD_RAM=RGB_Data;
//			
//		for (j=0;j<1;j++)	LCD_WriteRAM(f_data);  // �ұ�һ��
//    //LCD->LCD_RAM=f_data;
	}

//	for(i=0;i<1;i++)  // ����һ�� 
//	{
//		for(j=0;j<x;j++)	LCD_WriteRAM(f_data);
//    //LCD->LCD_RAM=f_data;
//	}	
}	

/*******************************************************************************
* Function Name  : Show_Gray
* Description    : �ҽ���ʾ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Gray(void) 
{
	unsigned int i,j,k,c16h;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);	//FOR 9481 TEST 20180328    ����BlockWrite����д��20180727
	for(j=0;j<(lcddev.height);j++)  
		{
		c16h=0x0000;
		for(k=0;k<16;k++)
		{
		    for(i=0;i<(lcddev.width/16);i++)
				{
					LCD_WriteRAM(c16h);	
          //LCD->LCD_RAM=c16h;
				}
			c16h=c16h+0x1082;//32�� 0841 16�� 1082 8��2104
		}
	}
}	

//--------Display GRAY------------//	
//���ϰ벿��Ϊ���Ұ�ߺڣ����߰�,�°���Ϊ�Ҽ�ɫ
void GRAY_TEST16(void)
{
	unsigned int i,j,k,c16h;
	POINT_COLOR=RED;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);	

  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);	//FOT 9481  20180328    ����BlockWrite����д��20180727
	
	for(j=0;j<(lcddev.height/2);j++)
	{
		for(i=0;i<(lcddev.width/2);i++)
		{
			LCD_WriteRAM(0xffff);//	
		}
		for(i=0;i<(lcddev.width/2);i++)
		{
			LCD_WriteRAM(0x0000);//
		}
	}
				
	for(j=0;j<(lcddev.height/2);j++)
	{
		c16h=0x0000;
		for(k=0;k<16;k++)
		{
		  for(i=0;i<(lcddev.width/16);i++)
			{
				LCD_WriteRAM(c16h);
			}
			c16h=c16h+0x0841;//32�� 0841 16�� 1082 8��2104
		}
	}
		
	LCD_ShowString((lcddev.width/6),(lcddev.height/4),210,16,16,"White");
	LCD_ShowString((lcddev.width*2/3),(lcddev.height/4),210,16,16,"Black");		
	LCD_ShowString((lcddev.width/6),(lcddev.height*2/3),210,16,16,"Gray");	
}
//--------END Display GRAY------------//

/*******************************************************************************
* Function Name  : Check_Crosstalk
* Description    : ����Ƿ���crosstalk����   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Check_Crosstalk(void)
{
	unsigned short m,n,i,j;
   
	BlockWrite(0,lcddev.width,0,lcddev.height);     
 
  for(m=0;m<lcddev.height/4;m++)
  {
       for(n=0;n<lcddev.width;n++)	LCD_WriteRAM(0x8410);
  }
	for(m=0;m<lcddev.height/2;m++)
  {
       for(n=0;n<lcddev.width/4;n++)	LCD_WriteRAM(0x8410);
       for(i=0;i<lcddev.width/2;i++)	LCD_WriteRAM(0x0000);
       for(j=0;j<lcddev.width/4;j++)	LCD_WriteRAM(0x8410);
	}
  for(m=0;m<lcddev.height/4;m++)
  {
       for(n=0;n<lcddev.width;n++)	LCD_WriteRAM(0x8410);
	}   
}

/*******************************************************************************
* Function Name  : Check_Flicker
* Description    : ����LCD��˸	flick
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Check_Flicker(void)
{
	unsigned short i,j;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);     
 
	for(i=0;i<lcddev.height;i++) 
  { 
		for(j=0;j<lcddev.width;j++) LCD_WriteRAM(0x8410);
    i++;
    for(j=0;j<lcddev.width;j++) LCD_WriteRAM(0x0000);
  }      
}

/*******************************************************************************
* Function Name  : LCD_Fill   Modify YZ
* Description    : ��ָ����������䵥����ɫ��(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
* Input          : color:Ҫ������ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  color)
{          
	uint16_t  i,j;
	uint16_t  xlen=0;
	//uint16_t  temp;

	xlen=ex-sx+1;
	
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)	
		LCD_WriteRAM(color);
		
    //LCD->LCD_RAM=color;	//��ʾ��ɫ 	    
	}	
} 

/*******************************************************************************
//��ָ�����������ָ����ɫ��		YZ	 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
*******************************************************************************/
void LCD_Color_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  *color)
{  
	uint16_t  height,width;
	uint16_t  i,j;
	
	
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
	
// 	for(i=0;i<height;i++)
//	{
// 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
//		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
//		for(j=0;j<width;j++)
//	//	LCD_WriteRAM(color[i*width+j]);
//    LCD->LCD_RAM = color[i*width+j];//д������ 
//	}		 

//	width = ex-sx-2+1;
//	height = ey-sy-2+1;			//�߶�

	  BlockWrite(sx,ex,sy,ey);
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width ; j++)
        {
         // LCD->LCD_RAM = color;  //д������
					LCD_WriteRAM(color[i*width+j]);
					//LCD->LCD_RAM=RGB_Data;
					/* ʹ��DMA�������ݴ��� */
        }
    }
			
} 

/*******************************************************************************
* Function Name  : LCD_Fill_Area   20180329
* Description    : ��ָ�����������ָ����ɫ									
* Input          : Xpos_S---x����ʼ����
*				   Ypos_S---y����ʼ����
*				   Xpos_E---x���������
*				   Ypos_E---y���������	
*                  color----�����ɫ
* Output         : None
* Return         : None
*******************************************************************************/

#define LCD_Use_Horizontal 1
void LCD_Fill_Area( uint16_t  Xpos_S, uint16_t  Ypos_S, uint16_t  Xpos_E, uint16_t  Ypos_E, uint16_t  color )
{                    

} 

/*******************************************************************************
* Function Name  : DisplayPicture
* Description    : ͼƬ��������Ļ����ʾ 
* Input          : 
* Output         : None
* Return         : None
*һ��TFT�Ĳ�ɫ,һ����ռ2���ֽ�RGB565,Ҳ����һ���������2^16��ɫ��,һ�����������ͼƬ
* ͼƬΪ���������������� �����м���ʾ
*******************************************************************************/
void DisplayPicture(void)
{
	uint16_t  temp,pos,t;  

//	BlockWrite(0,lcddev.width,0,lcddev.height);
	
	#if Pic    //320-320ͼƬ
	
	BlockWrite((lcddev.width-320)/2,(lcddev.width+320)/2-1,(lcddev.height-320)/2,(lcddev.height+320)/2-1); //OK 20171101��320-320ͼƬ
	for (pos=0;pos<320;pos++) //(pos=0;pos<lcddev.height;pos++)
	{
		for (t=0;t<320*2;t++) //(t=0;t<lcddev.width*2;t++) //
		{		
			temp=gImage[pos*320*2+t]*256+gImage[pos*320*2+t+1];
		
			t++;                                
			LCD_WriteRAM(temp);                             
		}                                
	}         	
	
	#else   //240-320ͼƬ

//	BlockWrite((lcddev.width-240)/2,(lcddev.width+240)/2-1,(lcddev.height-320)/2,(lcddev.height+320)/2-1); //OK 20171021  240-320ͼƬ
	BlockWrite((lcddev.width-240)/2,(lcddev.width+240)/2,(lcddev.height-320)/2,(lcddev.height+320)/2);  //20180727��Ӧ�ɣ̣ɣ�����������BlockWrite����д��20180727
	for (pos=0;pos<320;pos++) // (pos=0;pos<lcddev.height;pos++)//
	{
		for(t=0;t<240*2;t++) //(t=0;t<lcddev.width*2;t++) 
		{
			temp=gImage[pos*240*2+t]*256+gImage[pos*240*2+t+1];
	
			t++;                                
			LCD_WriteRAM(temp);          
		}
	}
	
	#endif  	
	
}	

//ͼƬ128-128��������Ļ�����м���ʾ 
void DisplayPicture1(void)
{

	uint16_t  temp,pos,t;  

//	BlockWrite(113,207,193,287);//OK 20170927
	BlockWrite((lcddev.width-128)/2,(lcddev.width+128)/2-1,(lcddev.height-128)/2,(lcddev.height+128)/2-1); //OK 20171005	
//	BlockWrite((lcddev.width-128)/2,(lcddev.width+128)/2,(lcddev.height-128)/2,(lcddev.height+128)/2); //OK 20180105 for GC9106
	
	for(pos=0;pos<128;pos++)
	{
		for(t=0;t<128*2;t++)
		{
			temp=gImage_128128[pos*128*2+t]*256+gImage_128128[pos*128*2+t+1];  
			t++;                                
			LCD_WriteRAM(temp);                                 
		}                                
	}

}	
/*******************************************************************************
// �Ʒ�����TFTLCD_Init.c    20171120
//���ù��λ��    MODIFY YZ
//Xpos:������
//Ypos:������    
*******************************************************************************/
/*
void LCD_SetCursor(uint16_t  Xpos, uint16_t  Ypos)
{	 
	
//	LCD_Set(); 

//	LCD_WR_REG(lcddev.setxcmd); 
//	LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
//	LCD_WR_REG(lcddev.setycmd); 
//	LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); //��д����һ����һֱ��ʾ����,��� 20170824

//	LCD_WR_8REG(lcddev.setxcmd); 
//	LCD_WR_8DATA(Xpos>>8);LCD_WR_8DATA(Xpos&0XFF); 			 
//	LCD_WR_8REG(lcddev.setycmd); 
//	LCD_WR_8DATA(Ypos>>8);LCD_WR_8DATA(Ypos&0XFF);   //OK20171005
//	LCD_WR_8REG(0x2C);
	
	//ILI9488 ,HX8357C,TEST 20171101  OK   Ҫ�Ӻ����013F,01DF
	LCD_WR_8REG(0x2a);   
	LCD_WR_8DATA(Xpos>>8);
	LCD_WR_8DATA(Xpos&0xff);
	LCD_WR_8DATA(0x01);
	LCD_WR_8DATA(0x3F);

	LCD_WR_8REG(0x2b);   
	LCD_WR_8DATA(Ypos>>8);
	LCD_WR_8DATA(Ypos&0xff);
	LCD_WR_8DATA(0x01);
	LCD_WR_8DATA(0xDF);
	
	LCD_WR_8REG(0x2C);	
	
} 

*/

//����Ϊ��������
/*******************************************************************************
//����			modify	YZ
//x,y:����
//POINT_COLOR:�˵����ɫ
*******************************************************************************/
void LCD_DrawPoint(uint16_t  x,uint16_t  y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	
	LCD_WriteRAM(POINT_COLOR);
	
//	LCD->LCD_RAM=POINT_COLOR; 
	
}

/*******************************************************************************
//���ٻ���  modify YZ
//x,y:����
//color:��ɫ
*******************************************************************************/
void LCD_Fast_DrawPoint(uint16_t  x,uint16_t  y,uint16_t  color)
{	  
	LCD_SetCursor(x,y);	

	LCD_WriteRAM(color);
	
//	LCD->LCD_RAM=color;	
	
}

/*******************************************************************************
//����				YZ
//x1,y1:�������
//x2,y2:�յ�����  
*******************************************************************************/
void LCD_DrawLine(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2)
{
	uint16_t  t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

/*******************************************************************************
//������	  YZ
//(x1,y1),(x2,y2):���εĶԽ�����
*******************************************************************************/
void LCD_DrawRectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

/*******************************************************************************
//��ָ��λ�û�һ��ָ����С��Բ  YZ
//(x,y):���ĵ�
//r    :�뾶
*******************************************************************************/
void LCD_Draw_Circle(uint16_t  x0,uint16_t  y0,uint8_t  r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

/*******************************************************************************
//��ָ��λ����ʾһ���ַ�   YZ
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//���ͣ�
//1���ַ����(size/2)���߶�(size)һ�롣
//2���ַ�ȡģΪ����ȡģ����ÿ��ռ�����ֽڣ���󲻹������ֽ�����ռһ�ֽڡ�
//3���ַ���ռ�ռ�Ϊ��ÿ����ռ�ֽڣ�������
//csize=(size/8+((size%8)?1:0))*(size/2)
//�˺�*ǰΪ����ÿ����ռ�ֽ������˺�*��Ϊ�������ַ��߶�һ�룩
*******************************************************************************/
void LCD_ShowChar(uint16_t  x,uint16_t  y,uint8_t  num,uint8_t  size,uint8_t  mode)
{  							  
    uint8_t  temp,t1,t;
	uint16_t  y0=y;
	uint8_t  csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}  

/*******************************************************************************
//m^n����  YZ
//����ֵ:m^n�η�.
*******************************************************************************/
uint32_t LCD_Pow(uint8_t  m,uint8_t  n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	

/*******************************************************************************
//��ʾ����,��λΪ0,����ʾ  YZ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);
*******************************************************************************/
void LCD_ShowNum(uint16_t  x,uint16_t  y,uint32_t num,uint8_t  len,uint8_t  size)
{         	
	uint8_t  t,temp;
	uint8_t  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

/*******************************************************************************
//��ʾ����,��λΪ0,������ʾ  YZ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
*******************************************************************************/
void LCD_ShowxNum(uint16_t  x,uint16_t  y,uint32_t num,uint8_t  len,uint8_t  size,uint8_t  mode)
{  
	uint8_t  t,temp;
	uint8_t  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 

/*******************************************************************************
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
// *p:�ַ�����ʼ��ַ		
//˵�����ַ����е���ɵģ������ַ������в��ϵ��û��㺯��LCD_DrawPoint();�����㣨��LCD->LCD_RAM��д��ɫֵ�������ɫֵ��lcd.h�ﱻ����Ϊ16λ�ĵ�ַ����
*******************************************************************************/
void LCD_ShowString(uint16_t  x,uint16_t  y,uint16_t  width,uint16_t  height,uint8_t  size,uint8_t  *p)
{         
	uint8_t  x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/***********************************End FUNCTION********************************************/

//��ʾˮƽ����
void display_colorbar_H(void)	
{  
	uint16_t  x,y;
	
//	BlockWrite(0,480,0,100);
//	LCD->LCD_RAM = 0xf800;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	
//	
//	
//	BlockWrite(0,480,100,200);
//	LCD->LCD_RAM = 0x07E0;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	

//	
//	BlockWrite(0,480,200,300);
//	LCD->LCD_RAM = 0x001F;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	

//	
//	BlockWrite(0,480,300,400);
//	LCD->LCD_RAM = 0xFFE0;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	
//	
//	
//	BlockWrite(0,480,400,500);
//	LCD->LCD_RAM = 0x07FF;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	

//	
//	BlockWrite(0,480,500,600);
//	LCD->LCD_RAM = 0xF81F;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	

//	
//	BlockWrite(0,480,600,700);
//	LCD->LCD_RAM = 0xFFFF;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	

//	
//	BlockWrite(0,480,700,800);
//	LCD->LCD_RAM = 0x0000;
//	LCD_WriteRAM_Prepare();     		  //��ʼд��GRAM	 	

		
	for( y = 0 ; y < 100 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xf800;
			 }
	
	for( y = 100 ; y < 200 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x07E0;
			 }
			 
			 
	for( y = 200 ; y < 300 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x001F;
			 }	 
			 
			 
	for( y = 300 ; y < 400 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xFFE0;
			 }			 
			 
	for( y = 400 ; y < 500 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x07FF;
			 }			 
			 
	for( y = 500 ; y < 600 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xF81F;
			 }	
	for( y = 600 ; y < 700 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xFFFF;
			 }	
	for( y = 700 ; y < 800 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x0000;
			 }		 		 
}





