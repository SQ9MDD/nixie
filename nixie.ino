/*
 arduino nixie clock
 (c) Ryszard Labus 2014
 cztero cyfrowy zegar arduino steruje BCD MH74141
 czwarta cyfra minuty zakres 0-9 4 bity
 trzecia cyfra dziesiątki minut zakres 0-5 3 bity
 druga cyfra godziny zakres 0-9 4bity
 pierwsza cyfra dziesiątki godzin zakres 0-2 2bity

 CHANGELOG 
 2016.07.02 Dodanie obsługi RTC, czyszczenie kodu
 2015.12.13 Testy zadziałania, ustawianie godzin
 2014.12.14 Pierwsza wersja programu naliczanie minutowe.
*/

#define version 1.3                   //wersja softu
#define DEBUG

//biblioteki
#include <Wire.h>
#include "RTClib.h"                   // zegar RTC

int mins = 0;                         //zmienna przechowuje minuty
int hr = 0;                           //zmienna przechowuje godziny
long time_to_tick = millis();         //zmienna z czasem odpalenia funkcji naliczania czasu
int save_time_flag = 0;               //flaga zapisu czasu do RTC

const int s_min_a = 2;                //czwarta cyfra najmłodszy bit
const int s_min_b = 3;                //czwarta cyfra drugi bit
const int s_min_c = 4;                //czwarta cyfra trzeci bit
const int s_min_d = 5;                //czwarta cyfra najstarszy bit
const int p_min_a = 6;                //trzecia cyfra najmłodszy bit
const int p_min_b = 7;                //trzecia cyfra drugi bit
const int p_min_c = 8;                //trzecia cyfra najstarszy bit
const int s_hr_a = 9;                 //druga cyfra najmłodszy bit
const int s_hr_b = 10;                //druga cyfra drugi bit
const int s_hr_c = 11;                //druga cyfra trzeci bit
const int s_hr_d = 12;                //druga cyfra najstarszy bit
const int p_hr_a = 13;                //pierwsza cyfra pierwszy bit
const int p_hr_b = A0;                //pierwsza cyfra drugi bit
const int min_add_pin = A1;           //wejscie do ustawiania minut
const int hr_add_pin = A2;            //wejscie do ustawiania godzin

//inicjalizacja bibliotek
RTC_DS1307 RTC;

//obsługa wyświetlania minut
void show_min(){
 int p_min = mins / 10;                //wyliczam dziesiątki minut
 int s_min = mins % 10;                //wyliczam minuty
 switch(p_min){
    case 0:
      digitalWrite(p_min_a,LOW);
      digitalWrite(p_min_b,LOW);
      digitalWrite(p_min_c,LOW);
    break;
    case 1:
      digitalWrite(p_min_a,HIGH);
      digitalWrite(p_min_b,LOW);
      digitalWrite(p_min_c,LOW);   
    break;
    case 2:
      digitalWrite(p_min_a,LOW);
      digitalWrite(p_min_b,HIGH);
      digitalWrite(p_min_c,LOW);   
    break; 
    case 3:
      digitalWrite(p_min_a,HIGH);
      digitalWrite(p_min_b,HIGH);
      digitalWrite(p_min_c,LOW);   
    break; 
    case 4:
      digitalWrite(p_min_a,LOW);
      digitalWrite(p_min_b,LOW);
      digitalWrite(p_min_c,HIGH);   
    break; 
    case 5:
      digitalWrite(p_min_a,HIGH);
      digitalWrite(p_min_b,LOW);
      digitalWrite(p_min_c,HIGH);   
    break;     
 }
 //minuty
 switch(s_min){
    case 0:
      digitalWrite(s_min_a,LOW);
      digitalWrite(s_min_b,LOW);
      digitalWrite(s_min_c,LOW);
      digitalWrite(s_min_d,LOW);
    break;
    case 1:
      digitalWrite(s_min_a,HIGH);
      digitalWrite(s_min_b,LOW);
      digitalWrite(s_min_c,LOW);
      digitalWrite(s_min_d,LOW);
    break;
    case 2:
      digitalWrite(s_min_a,LOW);
      digitalWrite(s_min_b,HIGH);
      digitalWrite(s_min_c,LOW);
      digitalWrite(s_min_d,LOW);
    break;  
    case 3:
      digitalWrite(s_min_a,HIGH);
      digitalWrite(s_min_b,HIGH);
      digitalWrite(s_min_c,LOW);
      digitalWrite(s_min_d,LOW);
    break;   
    case 4:
      digitalWrite(s_min_a,LOW);
      digitalWrite(s_min_b,LOW);
      digitalWrite(s_min_c,HIGH);
      digitalWrite(s_min_d,LOW);
    break;    
    case 5:
      digitalWrite(s_min_a,HIGH);
      digitalWrite(s_min_b,LOW);
      digitalWrite(s_min_c,HIGH);
      digitalWrite(s_min_d,LOW);
    break; 
    case 6:
      digitalWrite(s_min_a,LOW);
      digitalWrite(s_min_b,HIGH);
      digitalWrite(s_min_c,HIGH);
      digitalWrite(s_min_d,LOW);
    break;   
    case 7:
      digitalWrite(s_min_a,HIGH);
      digitalWrite(s_min_b,HIGH);
      digitalWrite(s_min_c,HIGH);
      digitalWrite(s_min_d,LOW);
    break; 
    case 8:
      digitalWrite(s_min_a,LOW);
      digitalWrite(s_min_b,LOW);
      digitalWrite(s_min_c,LOW);
      digitalWrite(s_min_d,HIGH);
    break;
    case 9:
      digitalWrite(s_min_a,HIGH);
      digitalWrite(s_min_b,LOW);
      digitalWrite(s_min_c,LOW);
      digitalWrite(s_min_d,HIGH);
    break;     
 } 
}

//obsługa wyświetlania godzin 
void show_hr(){
  int p_hr = hr / 10;
  int s_hr = hr % 10;
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
  #ifdef DEBUG 
    Serial.begin(9600);
  #endif
  pinMode(s_min_a,OUTPUT);
  pinMode(s_min_b,OUTPUT);
  pinMode(s_min_c,OUTPUT);
  pinMode(s_min_d,OUTPUT);
  pinMode(p_min_a,OUTPUT);
  pinMode(p_min_b,OUTPUT);
  pinMode(p_min_c,OUTPUT);
  pinMode(s_hr_a,OUTPUT);
  pinMode(s_hr_a,OUTPUT);
  pinMode(s_hr_b,OUTPUT);
  pinMode(s_hr_c,OUTPUT);
  pinMode(s_hr_d,OUTPUT);
  pinMode(p_hr_a,OUTPUT);
  pinMode(p_hr_b,OUTPUT);
  pinMode(min_add_pin, INPUT_PULLUP); 
  pinMode(hr_add_pin, INPUT_PULLUP);

  //RTC ON
  Wire.begin();
  RTC.begin();
  DateTime now = DateTime(F(__DATE__), F(__TIME__));
  
  //unlock only in first programming to set RTC time
  //RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (! RTC.isrunning()){
    #ifdef DEBUG
      Serial.println("Zegar nie uruchomiony");
    #endif
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }else{ //jesli zegar dziala odczytujemy z niego aktualny czas 
    #ifdef DEBUG
      Serial.println("Zegar dziala!");
    #endif    
   now = RTC.now();
  }
  mins = now.minute();
  hr = now.hour();
  // debugowanie
  #ifdef DEBUG
    Serial.println(String(hr) + ":" + String(mins));
  #endif 
  show_hr();     
  show_min(); 
  time_to_tick = millis() + 60000; 
}

//pętla główna tutaj naliczamy czas, naliczanie minutowe
void loop(){
  //obsluga klawisza dodawania minut
  if(digitalRead(min_add_pin) == LOW){
    delay(50);
    if(digitalRead(min_add_pin) == LOW){
      mins++;
      if(mins >= 60){
        mins = 0;
      }
      show_min();
      save_time_flag = 1;
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
      save_time_flag = 1;
      delay(200);
    }
  }

  //naliczanie czasu
  if(millis() >= time_to_tick){    
     time_to_tick = millis() + 60000;  //ma być 60000
     mins++;     
     if(mins >= 60){
       mins = 0;
       hr++;
       if(hr >= 24){
        hr = 0; 
       }
     }
     //jesli flaga save_time_flag jest 1 to zapisujemy biezacy czas do RTC i reset flagi
     if(save_time_flag ==1){
      DateTime now = RTC.now();
      RTC.adjust(DateTime(now.year(), now.month(), now.day(), hr, mins, 0));
      save_time_flag = 0;
     }else{
      DateTime now = RTC.now();
      mins = now.minute();
      hr = now.hour();
     }
   show_hr();     
   show_min(); 
    #ifdef DEBUG
      Serial.println(String(hr) + ":" + String(mins));
    #endif   
  }
}
