#include"userFunc.h"


void ledsetup(void){
  DDRB |= 0b00111110;
}
boolean swRead2(){
  return (PINC >> 2) & 0x01 ? OFF : ON;//プルアップの回路だとSWを押してるときPINDは0(LOW)
}


void ledoutput(int ledNum,int ledstate){
  ledstate ? PORTB |= (1 << ledNum) : PORTB &= ~(1 << ledNum);
}

//onなら真、offなら
void comoutput(int comNum,int comstate){
  comstate ? PORTB |= (1 << comNum) : PORTB &= ~(1 << comNum);
}

void delayMs(unsigned int ms){
  volatile unsigned long cnt;
  for(cnt =0;cnt<(COUNT * ms); cnt++);
}

void swSetup(void){
  DDRD &= ~((1 << DDD2) | (1 << DDD4));
  PORTD |= (1 << PORTD4);
}

boolean swRead(int swNum){
  return (PIND >> swNum) & 0x01 ? OFF : ON;//プルアップの回路だとSWを押してるときPINDは0(LOW)
}

void seg7Init(void){
  for(int i = 5; i <= 14; i++)pinMode(i,OUTPUT);
  pinMode(20,OUTPUT);
  pinMode(21,OUTPUT);
}

void seg7Disp(int dispNum){
  unsigned char disp[10] = {0x03 , 0x9F , 0x25 , 0xD , 0x99 , 0x49 , 0x41 , 0x1B , 0x1 , 0x9};

  digitalWrite(12, (disp[dispNum] >> 7) & 0x01);
  digitalWrite(20, (disp[dispNum] >> 6) & 0x01);
  digitalWrite(5, (disp[dispNum] >> 5) & 0x01);
  digitalWrite(10, (disp[dispNum] >> 4) & 0x01);
  digitalWrite(11, (disp[dispNum] >> 3) & 0x01);
  digitalWrite(14, (disp[dispNum] >> 2) & 0x01);
  digitalWrite(6, (disp[dispNum] >> 1) & 0x01);
  }

void seg7Pos(int pos){
    digitalWrite(DIG1, LOW);
    digitalWrite(DIG2, LOW);
    digitalWrite(DIG3, LOW);
    digitalWrite(DIG4, LOW);

    digitalWrite(pos , HIGH);
    }

void int0ExInit(void){
      pinMode(SW1A , INPUT);
      bitClear(EICRA , ISC00);
      bitSet(EICRA , ISC01);
      bitSet(EIMSK , INT0);
    }

ISR(INT0_vect){
   if( cnt % 2 ==0 ){
  bitSet(TCCR1B , CS12);
    cnt++;
   }else{
  bitClear(TCCR1B , CS12);
    cnt++;
   }
  }

  void timer2OVFInit(void){
    bitClear(TCCR2B , WGM22);
    bitClear(TCCR2A , WGM21);
    bitClear(TCCR2A , WGM20);
    bitSet(TCCR2B , CS22);
    bitClear(TCCR2B , CS21);
    bitSet(TCCR2B , CS20);
    bitSet(TIMSK2 , TOIE2);
  }
//LED4桁表示
ISR(TIMER2_OVF_vect){
    digitalWrite(DIG1 , LOW);
    digitalWrite(DIG2 , LOW);
    digitalWrite(DIG3 , LOW);
    digitalWrite(DIG4 , LOW);

    pos = ++pos % 4;
    switch(pos){
      case 0:
        seg7Disp(num / 600);
        break;
      case 1:
        seg7Disp((num /60 ) % 10);
        break;
      case 2:
        seg7Disp((num   / 10 ) % 6);
        break;
      case 3:
        seg7Disp((num / 1) % 10);
        break;
    }
    digitalWrite(digit[pos] , HIGH);
  }

void timer1COMPInit(void){
  //1秒カウントダウン
  bitClear(TCCR1B , WGM13);
  bitSet(TCCR1B , WGM12);
  bitClear(TCCR1A , WGM11);
  bitClear(TCCR1A , WGM10);
  bitClear(TCCR1B , CS12);
  bitClear(TCCR1B , CS11);
  bitClear(TCCR1B , CS10);
    
  OCR1A = CTC_COMP_VALUE;
  bitSet(TIMSK1 , OCIE1A);
}

ISR(TIMER1_COMPA_vect){
  if(swRead2()){
    num--;
  }else{
    num++;
  }
}

void adcInit(void){
  ADMUX = 0b01000001;
  bitSet(DIDR0 , ADC1D);
  bitSet(ADCSRA , ADEN);
}

int adcRead(void){
  bitSet(ADCSRA , ADSC);
  while(bitRead(ADCSRA , ADSC));
  return ADC;
}

int setTime(){
  bitSet(ADCSRA , ADSC);
  switch((adcRead() >> 8) & 0x03){
    case 0:
      return 180;
    case 1:
      return 60 * 7;
    case 2:
      return 60 * 15;
    case 3:
      return 15 * 60;
  }
}

void timer2COMPInit(void){
  //コンペアマッチでブザー出力
  pinMode(BZ , OUTPUT);
  bitClear(TCCR2A , COM2B1);
  bitSet(TCCR2A , COM2B0);
  bitClear(TCCR2B , WGM22);
  bitSet(TCCR2A , WGM21);
  bitClear(TCCR2A , WGM20);
  bitSet(TCCR2B , CS22); 
  bitClear(TCCR2B , CS21);
  bitClear(TCCR2B , CS20);
  num = 0;
}
