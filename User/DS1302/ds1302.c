#include "bsp.h"


static uint8_t readAddr[5]={0x8d, 0x89, 0x87, 0x85, 0x83};//���ꡢ�¡��ա�ʱ���ֵļĴ�����ַ
static uint8_t writeAddr[5]={0x8c, 0x88, 0x86, 0x84, 0x82};//д�ꡢ�¡��ա�ʱ���ֵļĴ�����ַ

static void write_1302byte(uint8_t dat);//дһ���ֽڵ�����sck������д����
static uint8_t read_1302(uint8_t add);//������
static void write_1302(uint8_t add,uint8_t dat);//��ָ���Ĵ���д��һ���ֽڵ�����
/*PA4.6Ϊ���*/
/*PA5����Ϊ��©ģʽ����ģʽ���ܹ�ʵ��������˫��IO��*/
void ds1302_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin = ds1302clk|ds1302rst;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DS1302_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Pin = ds1302dat;
  GPIO_Init(DS1302_PORT, &GPIO_InitStruct);
  
}

void write_1302byte(uint8_t dat)//дһ���ֽڵ�����sck������д����
{
  uint8_t i=0;
  GPIO_ResetBits(DS1302_PORT,ds1302clk);
  //ds1302clk=0;
  bsp_DelayUS(2);//��ʱ��Լ2us
  for(i=0;i<8;i++)
  {
    GPIO_ResetBits(DS1302_PORT,ds1302clk);
    //ds1302clk=0;
    if(dat&0x01)
      GPIO_SetBits(DS1302_PORT,ds1302dat);
    else
      GPIO_ResetBits(DS1302_PORT,ds1302dat);
    //ds1302dat=(dat&0x01);
    bsp_DelayUS(2);
    GPIO_SetBits(DS1302_PORT,ds1302clk);
    //ds1302clk=1;
    dat>>=1;
    bsp_DelayUS(1);
  }	
}
uint8_t read_1302(uint8_t add)//������
{
  uint8_t i=0,dat1=0x00;
  GPIO_ResetBits(DS1302_PORT,ds1302rst);
  GPIO_ResetBits(DS1302_PORT,ds1302clk);
  //ds1302rst=0;
  //ds1302clk=0;
  bsp_DelayUS(3);//��΢��ʱ2us
  GPIO_SetBits(DS1302_PORT,ds1302rst);
  //ds1302rst=1;
  bsp_DelayUS(3);//ʱ��Ҫ��Լ3us
  write_1302byte(add);//��д�Ĵ����ĵ�ַ
  for(i=0;i<8;i++)
  {
    GPIO_SetBits(DS1302_PORT,ds1302clk);
    //ds1302clk=1;
    dat1>>=1;
    GPIO_ResetBits(DS1302_PORT,ds1302clk);
    //ds1302clk=0;//����ʱ���ߣ��Ա������ݵĶ���
    if(GPIO_ReadInputDataBit(DS1302_PORT,ds1302dat)==1)//�����ߴ�ʱΪ�ߵ�ƽ
    {dat1=dat1|0x80;}
  }
  bsp_DelayUS(1);
  GPIO_ResetBits(DS1302_PORT,ds1302rst);
  //ds1302rst=0;
  return dat1;
}
void write_1302(uint8_t add,uint8_t dat)//��ָ���Ĵ���д��һ���ֽڵ�����
{
  GPIO_ResetBits(DS1302_PORT,ds1302rst);
  GPIO_ResetBits(DS1302_PORT,ds1302clk);
  //ds1302rst=0;
  //ds1302clk=0;
  bsp_DelayUS(1);//��΢��ʱ
  GPIO_SetBits(DS1302_PORT,ds1302rst);
  //ds1302rst=1;
  bsp_DelayUS(2);//ʱ���Լ2us
  write_1302byte(add);
  write_1302byte(dat);
  GPIO_ResetBits(DS1302_PORT,ds1302rst);
  GPIO_ResetBits(DS1302_PORT,ds1302clk);
  //ds1302clk=0;
  //ds1302rst=0;
  bsp_DelayUS(1);
  
}
void ds1302_settime(uint8_t *time, uint8_t len)//��ʼ��1302
{
  uint8_t i=0,j=0;
  write_1302(0x8e,0x00);//ȥ��д����
  for(i=0;i<len;i++)//תBCD��
  {
    j=time[i]%10;//��λ������
    time[i]=(time[i]/10)*16+j;	
  }
  for(i=0;i<len;i++)//���ж�ʱ
  {
    write_1302(writeAddr[i],time[i]);
    
  }
  write_1302(0x8e,0x80);//��д����
}
void ds1302_readtime(uint8_t *time, uint8_t len)//�������ݲ�ͨ�����ڴ�ӡ
{
  uint8_t i=0;
  //static uint8_t s=1;
  for(i=0;i<len;i++)
  {
    time[i]=read_1302(readAddr[i]);
  }//�������Ѿ����
  
  //BCDת10����
  for(i=0;i<len;i++)
  {
    time[i]=time[i]%16 + (time[i]/16)*10;//���λ����+��ʮλ����*10
  }
  //��ʱ��ת����10��������g[i]�����ŵ������ʱ��������ĸ���λ����
  //����ʱ��time��i�������ŵ��������ʱ���������ʮλ����
  
//  if(s!=(time[0]+g[0]))
//    printf("20%d%d��%d%d��%d%d��%d%d:%d%d:%d%d ����%d\r\n",\
//      time[6],g[6],time[4],g[4],time[3],g[3],time[2],g[2],time[1],g[1],time[0],g[0],g[5]);
//  s=time[0]+g[0];	
}
