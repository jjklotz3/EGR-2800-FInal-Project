
#include <LiquidCrystal.h>
#include <Wire.h>
LiquidCrystal lcd(7,8,9,10,11,12);                        //intalize lcd pins
const int sensor = A0, sensor1 = A1,sensor2=A2;   // sensor 2 will be 3rd photocell resistor
const double g{9.81},d{-0.298};                            //m/s^2 and meters, distance needs to be changed once fully assembled 
int goButton = 6; 
int finish{0};                                           //finsih variable for timer
int start{0};                                            //start variable for timer                                   
double ans{0.0};                                           //This is what the static friction value is stored in to make it avaible to print in later stage
double val{0.0};                                          //Stores the position of Servo when block passes the first sensor 
const int MPU_addr=0x68;
int16_t axis_X,axis_Y,axis_Z;
int minVal=265;
int maxVal=402;
double x;
double y;
double z;
bool s=true;
bool f =true;
bool a = true;
void st (){
  if(analogRead(sensor1)>200){                     //captures time when block passes the laser. 
  start = millis();
  Serial.println(start);
  s =! s;                                         //Change state of s after run once in order to ensure time is captured once
}
}
void ft (){
  if(analogRead(sensor2)>300){
   finish = millis();     //captures time when block passes laser
  double TT = (finish - start)*0.001;            //total time it took the block to pass both lasers                     
  Serial.println(finish); 
  Serial.println(TT);               //Tells us time so we can test equations on our own
  double acc = (2*d)/(TT*TT);    
  Serial.println(acc);             //parallel acceleration
  double ans1= g*sin(val);                    //store this in a variable to make arithmetic easier 
   Serial.println(ans1);   
  double ans2= g*cos(val);                    //store this in a variable to make arithmetic easier 
   Serial.println(ans2);   
  double kF=(ans1+acc)/(ans2);
  Serial.println(ans);
  Serial.println(kF);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Us= ");
  lcd.print(ans);
  lcd.setCursor(0,1);
  lcd.print("Uk= ");
  lcd.print(kF);
  f =! f;                                          //Change state f after run once in order to ensure time is captured once
}
}
void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16, 2);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0); 
  Wire.endTransmission(true);
  pinMode(goButton,INPUT_PULLUP);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Welcome! Load ");
 lcd.setCursor(0,1);                             //Tells arduino to print the next statement on the second row of LCD
 lcd.println("Materials in Bay");
}

void loop() {
if (digitalRead(goButton)==LOW){                    //Start button 
lcd.clear();                                    //Clears previous message in setup
lcd.setCursor(0,0);                             //Tells Arduino to print message in first row
lcd.print("Calculating.....");
while(analogRead(sensor)>200 && a == true){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  axis_X=Wire.read()<<8|Wire.read();
  axis_Y=Wire.read()<<8|Wire.read();
  axis_Z=Wire.read()<<8|Wire.read(); 
    int xAng = map(axis_X,minVal,maxVal,-90,90);
    int yAng = map(axis_Y,minVal,maxVal,-90,90);
    int zAng = map(axis_Z,minVal,maxVal,-90,90);
      x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
      y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
      z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
     Serial.print("Angle of inclination in X axis = ");
     Serial.print(x+2);
     Serial.println((char)176);
     Serial.println(analogRead(sensor));
     delay(400);
   }
  if(analogRead(sensor)<200){                     //when lasor hits sensor (stop moving)
        a =! a;
        val=((x+2)*0.0174533);   //converts degrees to radians
        Serial.println(x+2);
        Serial.print("Val = ");
        Serial.println(val);
        ans = tan(val);                           //tan theta given in radians 
        Serial.println(ans);
             
        }
  }
  if(s == true){
    st();
  }
  if(f==true){
    ft();
  }
   }
