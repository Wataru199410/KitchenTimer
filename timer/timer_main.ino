#include"userFunc.h"
unsigned char digit[4] = {DIG1 , DIG2 , DIG3 ,DIG4};
unsigned char pos = 0;
int num = 0;
static int cnt = 0;
int i = 0;
 
void setup() {
  pinMode(SW2A , INPUT_PULLUP);
  pinMode(A2 , INPUT_PULLUP);
  int0ExInit();
  seg7Init();
  adcInit();
  cli();
  timer2OVFInit();
  timer1COMPInit();
  sei();
}

void loop() {
  if(swRead2()){//キッチンタイマーの処理

   if( cnt  == 0 ){
      num = setTime();//ここでディスプレイに移す数字を決めている
   }

//常時SW2を押すと初期状態に戻る
    if(!digitalRead(SW2A)){
            delay(10);

             cnt = 0;
             delay(10);
    }
//タイムが0になったら実行
    if(num == 0){
      
        bitClear(TCCR1B , CS12);//タイマー1を停止状態にする
        //無理やり常時0を表示させる
        seg7Disp(0);
        digitalWrite(DIG1 , HIGH);
        digitalWrite(DIG2 , HIGH);
        digitalWrite(DIG3 , HIGH);
        digitalWrite(DIG4 , HIGH);
        
        //タイマー2を4桁表示からブザー出力に変更する
        timer2COMPInit();
        OCR2A = 48;//比較レジスタの値を変える

        //トグル出力でブザーを鳴らす
        for( i = 0 ; i < 2; i++){
          TCNT2 = 0;
          bitSet(TCCR2B , CS22);
          delay(50);
          bitClear(TCCR2B , CS22);
          delay(50);

          //SW2が押されたらトグル出力をOFFにしブザーを消し、初期化する
          if(!digitalRead(SW2A)){
            delay(10);
            bitClear(TCCR2A , COM2B1);
            bitClear(TCCR2A , COM2B0);
            cnt = 0;
            delay(10);
          }
        }
      timer2OVFInit();//初期化される前にタイマー2をLED4桁表示に変える
    }
  
}else{//ストップウォッチの処理
    if(cnt == 0){
      num = 0;
     }
    if(!digitalRead(SW2A)){
      bitClear(TCCR1B , CS12);
      delay(10);
      cnt = 0;
      num = 0;
      delay(10);
    } 
  }
}
