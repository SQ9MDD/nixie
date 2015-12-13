/*
 arduino nixie clock
 (c) Ryszard Labus 2014
 cztero cyfrowy zegar arduino steruje BCD MH74141
 czwarta cyfra minuty zakres 0-9 4 bity
 trzecia cyfra dziesiątki minut zakres 0-5 3 bity
 druga cyfra godziny zakres 0-9 4bity
 pierwsza cyfra dziesiątki godzin zakres 0-2 2bity

 CHANGELOG 
 2015.12.13 Testy zadziałania, ustawianie godzin
 2014.12.14 Pierwsza wersja programu naliczanie minutowe.
*/
#define version 1.1                   //wersja softu
int minute = 0;                       //zmienna przechowuje minuty
int hr = 0;                           //zmienna przechowuje godziny
long time_to_tick = millis();         //zmienna z czasem odpalenia funkcji naliczania czasu

const int s_minute_a = 2;              //czwarta cyfra najmłodszy bit
const int s_minute_b = 3;              //czwarta cyfra drugi bit
const int s_minute_c = 4;              //czwarta cyfra trzeci bit
const int s_minute_d = 5;              //czwarta cyfra najstarszy bit
const int p_minute_a = 6;              //trzecia cyfra najmłodszy bit
const int p_minute_b = 7;              //trzecia cyfra drugi bit
const int p_minute_c = 8;              //trzecia cyfra najstarszy bit
const int s_hr_a = 9;                  //druga cyfra najmłodszy bit
const int s_hr_b = 10;                 //druga cyfra drugi bit
const int s_hr_c = 11;                 //druga cyfra trzeci bit
const int s_hr_d = 12;                 //druga cyfra najstarszy bit
const int p_hr_a = 13;                 //pierwsza cyfra pierwszy bit
const int p_hr_b = A0;                 //pierwsza cyfra drugi bit
const int minute_add_pin = A1;         //wejscie do ustawiania minut
const int hr_add_pin = A2;             //wejscie do ustawiania godzin

//obsługa wyświetlania minut
void show_minute(){
 int p_minute = minute / 10;           //wyliczam dziesiątki minut
 int s_minute = minute % 10;           //wyliczam minuty
 //Serial.print(p_minute);
 //Serial.print(" ");
 //Serial.println(s_minute);
 //dziesiątki minut
 switch(p_minute){
    case 0:
      digitalWrite(p_minute_a,LOW);
      digitalWrite(p_minute_b,LOW);
      digitalWrite(p_minute_c,LOW);
    break;
    case 1:
      digitalWrite(p_minute_a,HIGH);
      digitalWrite(p_minute_b,LOW);
      digitalWrite(p_minute_c,LOW);   
    break;
    case 2:
      digitalWrite(p_minute_a,LOW);
      digitalWrite(p_minute_b,HIGH);
      digitalWrite(p_minute_c,LOW);   
    break; 
    case 3:
      digitalWrite(p_minute_a,HIGH);
      digitalWrite(p_minute_b,HIGH);
      digitalWrite(p_minute_c,LOW);   
    break; 
    case 4:
      digitalWrite(p_minute_a,LOW);
      digitalWrite(p_minute_b,LOW);
      digitalWrite(p_minute_c,HIGH);   
    break; 
    case 5:
      digitalWrite(p_minute_a,HIGH);
      digitalWrite(p_minute_b,LOW);
      digitalWrite(p_minute_c,HIGH);   
    break;     
 }
 //minuty
 switch(s_minute){
    case 0:
      digitalWrite(s_minute_a,LOW);
      digitalWrite(s_minute_b,LOW);
      digitalWrite(s_minute_c,LOW);
      digitalWrite(s_minute_d,LOW);
    break;
    case 1:
      digitalWrite(s_minute_a,HIGH);
      digitalWrite(s_minute_b,LOW);
      digitalWrite(s_minute_c,LOW);
      digitalWrite(s_minute_d,LOW);
    break;
    case 2:
      digitalWrite(s_minute_a,LOW);
      digitalWrite(s_minute_b,HIGH);
      digitalWrite(s_minute_c,LOW);
      digitalWrite(s_minute_d,LOW);
    break;  
    case 3:
      digitalWrite(s_minute_a,HIGH);
      digitalWrite(s_minute_b,HIGH);
      digitalWrite(s_minute_c,LOW);
      digitalWrite(s_minute_d,LOW);
    break;   
    case 4:
      digitalWrite(s_minute_a,LOW);
      digitalWrite(s_minute_b,LOW);
      digitalWrite(s_minute_c,HIGH);
      digitalWrite(s_minute_d,LOW);
    break;    
    case 5:
      digitalWrite(s_minute_a,HIGH);
      digitalWrite(s_minute_b,LOW);
      digitalWrite(s_minute_c,HIGH);
      digitalWrite(s_minute_d,LOW);
    break; 
    case 6:
      digitalWrite(s_minute_a,LOW);
      digitalWrite(s_minute_b,HIGH);
      digitalWrite(s_minute_c,HIGH);
      digitalWrite(s_minute_d,LOW);
    break;   
    case 7:
      digitalWrite(s_minute_a,HIGH);
      digitalWrite(s_minute_b,HIGH);
      digitalWrite(s_minute_c,HIGH);
      digitalWrite(s_minute_d,LOW);
    break; 
    case 8:
      digitalWrite(s_minute_a,LOW);
      digitalWrite(s_minute_b,LOW);
      digitalWrite(s_minute_c,LOW);
      digitalWrite(s_minute_d,HIGH);
    break;
    case 9:
      digitalWrite(s_minute_a,HIGH);
      digitalWrite(s_minute_b,LOW);
      digitalWrite(s_minute_c,LOW);
      digitalWrite(s_minute_d,HIGH);
    break;     
 } 
}

//obsługa wyświetlania godzin 
void show_hr(){
  int p_hr = hr / 10;
  int s_hr = hr % 10;
  //Serial.print(p_hr);
  //Serial.print(" ");
  //Serial.print(s_hr);
  //Serial.print(":");
  //dziesiątki godzin
  switch(p_hr){
   case 0:
      digitalWrite(p_hr_a,LOW);
      digitalWrite(p_hr_b,LOW);  
   break; 
   case 1:
      digitalWrite(p_hr_a,HIGH);
      digitalWrite(p_hr_b,LOW); 
   break;
   case 2:
      digitalWrite(p_hr_a,LOW);
      digitalWrite(p_hr_b,HIGH);
   break;   
  }
  //godziny
  switch(s_hr){
   case 0:
      digitalWrite(s_hr_a,LOW);
      digitalWrite(s_hr_b,LOW); 
      digitalWrite(s_hr_c,LOW);
      digitalWrite(s_hr_d,LOW);       
   break; 
   case 1:
      digitalWrite(s_hr_a,HIGH);
      digitalWrite(s_hr_b,LOW); 
      digitalWrite(s_hr_c,LOW);
      digitalWrite(s_hr_d,LOW);  
   break; 
   case 2:
      digitalWrite(s_hr_a,LOW);
      digitalWrite(s_hr_b,HIGH); 
      digitalWrite(s_hr_c,LOW);
      digitalWrite(s_hr_d,LOW);   
   break; 
   case 3:
      digitalWrite(s_hr_a,HIGH);
      digitalWrite(s_hr_b,HIGH); 
      digitalWrite(s_hr_c,LOW);
      digitalWrite(s_hr_d,LOW);   
   break; 
   case 4:
      digitalWrite(s_hr_a,LOW);
      digitalWrite(s_hr_b,LOW); 
      digitalWrite(s_hr_c,HIGH);
      digitalWrite(s_hr_d,LOW);   
   break; 
   case 5:
      digitalWrite(s_hr_a,HIGH);
      digitalWrite(s_hr_b,LOW); 
      digitalWrite(s_hr_c,HIGH);
      digitalWrite(s_hr_d,LOW);   
   break; 
   case 6:
      digitalWrite(s_hr_a,LOW);
      digitalWrite(s_hr_b,HIGH); 
      digitalWrite(s_hr_c,HIGH);
      digitalWrite(s_hr_d,LOW);   
   break; 
   case 7:
      digitalWrite(s_hr_a,HIGH);
      digitalWrite(s_hr_b,HIGH); 
      digitalWrite(s_hr_c,HIGH);
      digitalWrite(s_hr_d,LOW);   
   break; 
   case 8:
      digitalWrite(s_hr_a,LOW);
      digitalWrite(s_hr_b,LOW); 
      digitalWrite(s_hr_c,LOW);
      digitalWrite(s_hr_d,HIGH);   
   break; 
   case 9:
      digitalWrite(s_hr_a,HIGH);
      digitalWrite(s_hr_b,LOW); 
      digitalWrite(s_hr_c,LOW);
      digitalWrite(s_hr_d,HIGH);   
   break;    
  }
}

//funkcja setup odpalamy raz przy starcie
void setup(){
  //Serial.begin(9600);
  pinMode(s_minute_a,OUTPUT);
  pinMode(s_minute_b,OUTPUT);
  pinMode(s_minute_c,OUTPUT);
  pinMode(s_minute_d,OUTPUT);
  pinMode(p_minute_a,OUTPUT);
  pinMode(p_minute_b,OUTPUT);
  pinMode(p_minute_c,OUTPUT);
  pinMode(s_hr_a,OUTPUT);
  pinMode(s_hr_a,OUTPUT);
  pinMode(s_hr_b,OUTPUT);
  pinMode(s_hr_c,OUTPUT);
  pinMode(s_hr_d,OUTPUT);
  pinMode(p_hr_a,OUTPUT);
  pinMode(p_hr_b,OUTPUT);
  pinMode(minute_add_pin, INPUT_PULLUP); 
  pinMode(hr_add_pin, INPUT_PULLUP);

  //
  show_hr();     
  show_minute(); 
  time_to_tick = millis() + 60000; 
}

//pętla główna tutaj naliczamy czas, naliczanie minutowe
void loop(){
  //obsluga klawisza dodawania minut
  if(digitalRead(minute_add_pin) == LOW){
    delay(50);
    if(digitalRead(minute_add_pin) == LOW){
      minute++;
      if(minute >= 60){
        minute = 0;
      }
      show_minute();
      delay(200);
    }
  }

  //obsługa klawisza dodawania godzin
  if(digitalRead(hr_add_pin) == LOW){
    delay(50);
    if(digitalRead(hr_add_pin) == LOW){
      hr++;
      if(hr >= 24){
        hr = 0; 
      }
      show_hr();
      delay(200);
    }
  }

  //naliczanie czasu
  if(millis() >= time_to_tick){    
     time_to_tick = millis() + 60000;  //ma być 60000
     minute++;
     if(minute >= 60){
       minute = 0;
       hr++;
       if(hr >= 24){
        hr = 0; 
       }
     }
   show_hr();     
   show_minute(); 
  }
}
