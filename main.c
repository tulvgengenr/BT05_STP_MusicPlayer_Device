#include "main.H"
/*myproject
 *���ߣ�HNU������
 *QQ��871005218
*/
  //ȫ�ֱ���
unsigned char Music_PM=90;//����
unsigned char Music_tone=0xFC;//����

unsigned char song1[2] = {0,0};//��λ������������

int xiege = 0;//�Ƿ���д��ģʽ
unsigned char num_cunchu = 0;//�Ѿ��洢���ֽ�����
xdata unsigned char cunchu[1001] = {0x21,0x04,
  0x23,0x08,
  0x23,0x04,
  0x23,0x20};//������ʧ�洢�������ݴ�Ӳ�̶����ڴ�

int bofang = 0;//�Ƿ��ǲ���ģʽ
int index = 0;//��ǰ���ŵĸ��������

  //�ӳٺ���
void Delay10ms(){
	unsigned char i, j;
	i = 108;
	j = 145;
	do{
		while(--j);
		
	}while(--i);
}

	//����2�յ����ݵĻص�����
void myUart2Rxd_callback(){
	PlayTone(Music_tone, Music_PM , song1[0], song1[1]);
	if(xiege == 1){
		if(num_cunchu <= 0xfe){
		M24C02_Write(num_cunchu, song1[0]);
		num_cunchu++;
		Delay10ms();
		M24C02_Write(num_cunchu, song1[1]);
	  num_cunchu++;
		Delay10ms();
	  Seg7Print(num_cunchu,0,0,0,0,0,song1[0],song1[1]);
		}
	}
}


  /*����1����д��ģʽ�ٰ�һ���˳�д��ģʽ
   *����2�������ֲ���ģʽ�ٰ�һ���˳����ֲ���ģʽ
  */
void myKey_callback(){
	if(GetKeyAct(enumKey1) == enumKeyPress){
		if(xiege == 0) xiege = 1;
		else xiege = 0;
		if(bofang == 1) bofang = 0;
	}
	if(GetKeyAct(enumKey2) == enumKeyPress){
		if(bofang == 0) bofang = 1;
		else bofang = 0;
		if(xiege == 1) xiege = 0;
		
		if(bofang == 1){
			unsigned char i = 0;
			while(i != num_cunchu)
			{
				cunchu[i] = M24C02_Read(i);
				i++;
			  Delay10ms();
			}
			Seg7Print(cunchu[0],cunchu[1],cunchu[2],cunchu[3],cunchu[4],cunchu[5],cunchu[6],cunchu[7]);
			Delay10ms();
			SetPlayerMode(enumModePlay);
	  }
		else{
			SetPlayerMode(enumModeStop);
		}
  }
}
void main(){
	//����2��EXT��9600����������
	Uart2Init(9600,Uart2UsedforEXT);
	//���ֲ���������
	MusicPlayerInit();
	BeepInit();
	//�������ʾ����
	DisplayerInit();
	SetDisplayerArea(0,7);
	//��������
	Key_Init();
  //���ô���2���ջص�����
  SetEventCallBack(enumEventUart2Rxd, myUart2Rxd_callback); 
  //���ô���2�������������ݰ����λ�á���С����ͷƥ���ַ���ƥ���ַ�����
	SetUart2Rxd(&song1, 2, 0);	       
	//����K1ֹͣ����
	SetEventCallBack(enumEventKey, myKey_callback);
  //��ʼ������
	Seg7Print(6,6,6,6,6,6,6,6);
	LedPrint(0);			
	//��������
	SetMusic(Music_PM, Music_tone, cunchu,sizeof(cunchu), enumMscDrvLed);
	//MySTC_OS
	MySTC_Init();
	while(1){
	MySTC_OS();
	}
}