#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <Wire.h>
#include <Servo.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231 rtc(A4, A5);
Servo servo;
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
char t;
char data[10];
byte datacount =0;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int t1, t2, t3, t4, t5, t6;
bool feed = true;
int r[12];
char key;
char dfs;
const int motorPin=11;
void setup()
{
  Serial.begin(9600);
  pinMode(motorPin,OUTPUT);
  
  servo.attach(10);
  rtc.begin();
  lcd.init();                      // initialize the lcd 
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Time: ");
  
}

void loop()
{
    lcd.setCursor(0, 0);
    //int buttonPress = digitalRead(A2);
    int df=0;
    //new code
    dfs = kpd.getKey(); 
    if (dfs == 'A'){
        setFeedingTime();
        df=1;
    }
    String t = "";
    t = rtc.getTimeStr(); 
    t1 = t.charAt(0)-48;
    t2 = t.charAt(1)-48;
    t3 = t.charAt(3)-48;
    t4 = t.charAt(4)-48;
    t5 = t.charAt(6)-48;
    t6 = t.charAt(7)-48;
    lcd.print("Time: ");
    lcd.print(rtc.getTimeStr());
    lcd.setCursor(0,1);
    lcd.print("Date: ");
    lcd.print(rtc.getDateStr());
    Serial.println(t5);
    Serial.println(t6);
    if ((t1==r[0] && t2==r[1] && t3==r[2] && t4==r[3] &&t5<1 && t6<3) || (t1 == r[4] && t2==r[5] && t3 == r[6] && t4 == r[7] &&t5<1 && t6<3)){ 
    int TimeForMotor=0;
    bool getDown= false;
    while(TimeForMotor<10000){
        PinMode(motorPin,HIGH);
        if (TimeForMotor == 9999){
            getDown = true;
        }
        TimeForMotor++;
    }
    if (getDown)PinMode(motorPin,LOW);
    delay(10000);
    PinMode(motorPin,HIGH);
    servo.write(0);
    delay(1000);  
    PinMode(motorPin,LOW);
    servo.write(90);
    delay(1000);  
 } 

}
void setFeedingTime(){
  feed = true;
  int i=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set feeding Time");
  lcd.clear();
  lcd.print("HH:MM HH:MM");
  lcd.setCursor(0,0);
  while(1){
    key = kpd.getKey();
    char j;
    int d=0;
    if(key){
    data[datacount] = key;
    lcd.setCursor(datacount,1);
    if (key != 'A' && key !='B' && key != 'C' && key != 'D'){
        lcd.print(data[datacount]);
        datacount++;
        r[i] = key-48;
        i++;
      if (datacount==2 || datacount==8){
          lcd.print(":"); datacount++;
        }
      if(datacount==5){
        lcd.print("-");
        datacount++;
        }
    }
    if (key == 'C'){
      int tt=0;
      bool s = false;
      while(tt<1000){
          pinMode(motorPin,HIGH);
          if (tt == 999){
            s = true;
          }
      }
      if (s)pinMode(motorPin,LOW);
      servo.write(0);
      delay(1000);  
      servo.write(90);
      pinMode(motorPin,HIGH);
      delay(1000);  
      servo.write(0);
      delay(1000);  
      pinMode(motorPin,LOW);
      servo.write(90);
      delay(1000);  
    }
  }
  int cnt=0;
  if (datacount == 11 || key =='D'){
    key=0;
    datacount=0;
    lcd.clear(); 
    break;
    }
  }
}

