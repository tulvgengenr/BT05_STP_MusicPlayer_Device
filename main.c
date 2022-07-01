#include "main.H"
/*myproject
 *作者：HNU根根儿
 *QQ：871005218
*/
  //全局变量
unsigned char Music_PM=90;//节奏
unsigned char Music_tone=0xFC;//音调

unsigned char song1[2] = {0,0};//上位机传来的音阶

int xiege = 0;//是否是写歌模式
unsigned char num_cunchu = 0;//已经存储的字节数量
xdata unsigned char cunchu[1001] = {0x21,0x04,
  0x23,0x08,
  0x23,0x04,
  0x23,0x20};//将非易失存储器的内容从硬盘读到内存

int bofang = 0;//是否是播放模式
int index = 0;//当前播放的歌曲的序号

  //延迟函数
void Delay10ms(){
	unsigned char i, j;
	i = 108;
	j = 145;
	do{
		while(--j);
		
	}while(--i);
}

	//串口2收到数据的回调函数
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


  /*按键1进入写歌模式再按一次退出写歌模式
   *按键2进入音乐播放模式再按一次退出音乐播放模式
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
	//串口2，EXT，9600波特率驱动
	Uart2Init(9600,Uart2UsedforEXT);
	//音乐播放器驱动
	MusicPlayerInit();
	BeepInit();
	//数码管显示驱动
	DisplayerInit();
	SetDisplayerArea(0,7);
	//按键驱动
	Key_Init();
  //设置串口2接收回调函数
  SetEventCallBack(enumEventUart2Rxd, myUart2Rxd_callback); 
  //设置串口2接收条件：数据包存放位置、大小、包头匹配字符、匹配字符个数
	SetUart2Rxd(&song1, 2, 0);	       
	//按下K1停止播放
	SetEventCallBack(enumEventKey, myKey_callback);
  //初始化变量
	Seg7Print(6,6,6,6,6,6,6,6);
	LedPrint(0);			
	//播放音乐
	SetMusic(Music_PM, Music_tone, cunchu,sizeof(cunchu), enumMscDrvLed);
	//MySTC_OS
	MySTC_Init();
	while(1){
	MySTC_OS();
	}
}