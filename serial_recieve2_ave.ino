
#define Up 0
#define Down 1
#define Left 2
#define Right 3
#define Circle 4
#define Cross 5
#define Triangle 6
#define Square 7
#define L1 8
#define R1 9
#define L2 10
#define R2 11
#define L3 12
#define R3 13
#define SELECT 14
#define START 15
#define PS 16

#define RECIEVE_LENGH 10

int lx,ly,rx,ry;//joystick
int L,R;
byte average;//受信成功率
boolean DualShock3[17];
byte recieve_data[RECIEVE_LENGH];
boolean data_check=false;

int ave_cout1,ave_count2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  TCCR1B &= B11111000;//プリスケーラの設定。31kHzで動かす
  TCCR1B |= B00000001;//9,10PINの周期が変わる
  
  TCCR2B &= B11111000;
  TCCR2B |= B00000001;//3,11PIN
 // pinMode(9,OUTPUT);//pwm
 // pinMode(10,OUTPUT);
 // pinMode(3,OUTPUT);
 // pinMode(11,OUTPUT);
  pinMode(13,OUTPUT);//LED
}

// the loop routine runs over and over again forever:
void loop() {
  int count=0;

  byte data;
  while(1){
    if(Serial.available()){
      data=Serial.read();  
      recieve_data[count]=data;
      
      //ave_count2++;
      
      if(data==255){
        if(count==(RECIEVE_LENGH-1))
        {
          data_check=true;
          //for(int i=0;i<10;i++)Serial.write(recieve_data[i]);//データ送信
          Serial.write(recieve_data,10);
          count=0;
          after_recieve();
          
        }
        break;
      }

      count++;
    }
  }

}

void after_recieve(){

  dualshock_input();//コントローラのインプット情報
  motor_ctrl();
  if(DualShock3[PS]||DualShock3[START]||DualShock3[Circle]||DualShock3[R3]||DualShock3[L2])digitalWrite(13, HIGH);
  else digitalWrite(13, LOW);
}

void dualshock_input(void)
{

  int i=0;
  for(i=0;i<8;i++){
    byte mask=0;
    bitSet(mask,i);//maskのiビット目を1にする
    if(0xFF&recieve_data[0]&mask)DualShock3[i]=true;
    else DualShock3[i]=false;
    if(0xFF&recieve_data[1]&mask)DualShock3[8+i]=true;
    else DualShock3[8+i]=false;
  }

  if(recieve_data[2]>0)DualShock3[16]=true;
  else DualShock3[16]=false;

  lx=recieve_data[3]-100;
  ly=recieve_data[4]-100;
  rx=recieve_data[5]-100;
  ry=recieve_data[6]-100;
  L=recieve_data[7]-100;
  R=recieve_data[8]-100;

}

void motor_ctrl(void){
  //Lモータ
  if(L>0){//正転
    analogWrite(9, 127+127*L/100);
    analogWrite(10,0);
  }
  else if(L<0){
    analogWrite(9,0);
    analogWrite(10,127-127*L/100);   
  }
  else{//ストップ
    analogWrite(9,0);
    analogWrite(10,0);
  }
  
  //Rモータ
  if(R>0){//正転
    analogWrite(3, 127+127*R/100);
    analogWrite(11,0);
  }
  else if(R<0){
    analogWrite(3,0);
    analogWrite(11,127-127*R/100);   
  }
  else{//ストップ
    analogWrite(3,0);
    analogWrite(11,0);
  }
    /*
    analogWrite(9, 127-127*L/100);
    analogWrite(10, 127+127*R/100);
    */
}



