#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


int mesaj[2];
int ileri_sayac = 0;
int veri = 0;
RF24 verici(9, 10); // CE ve CSN pinleri
const int kanal = 157;
bool onceki_durum_ileri = LOW;
bool buton_durumu_ileri = LOW;
bool onceki_durum_mod_kapat = LOW;
bool buton_durumu_mod_kapat = LOW;
bool buton_durumu_mod = LOW;
bool onceki_durum_mod = LOW;
bool buton_durumu_geri = LOW;
bool onceki_durum_geri = LOW;
bool buton_durumu_sol = LOW;
bool onceki_durum_sol = LOW;
bool buton_durumu_sag = LOW;
bool onceki_durum_sag = LOW;


void setup() {
  Serial.begin(9600);
  verici.begin();
  verici.openWritingPipe(kanal);
  pinMode(A2, INPUT);  // Yön
  pinMode(A3, INPUT);  // Hız 
  pinMode(7, INPUT_PULLUP); // İleri buton (Pull-up ile)
  pinMode(6, INPUT_PULLUP); // Gönder buton (Pull-up ile) //Direkt gidicek, 8 gitsin, modu kapatsın
  pinMode(5, INPUT_PULLUP); // mod açma buton (Pull-up ile)
  pinMode(4, INPUT_PULLUP); // geri buton (Pull-up ile)
  pinMode(3, INPUT_PULLUP); // sol açma buton (Pull-up ile)
  pinMode(2, INPUT_PULLUP); // sag açma buton (Pull-up ile)  
}

void loop() {
  // Buton değerlerini ters çevirerek 1-0 mantığını düzelt
  buton_durumu_ileri = !digitalRead(2);
  buton_durumu_mod_kapat = !digitalRead(6);
  buton_durumu_mod = !digitalRead(7);
  buton_durumu_geri = !digitalRead(4);
  buton_durumu_sol = !digitalRead(3);
  buton_durumu_sag = !digitalRead(5);

  // İleri butonu kenar tespiti (sadece basıldığında sayaç artır)
  if (buton_durumu_ileri == HIGH && onceki_durum_ileri == LOW) {  
    veri=1;
  }
  onceki_durum_ileri = buton_durumu_ileri;

  // Gönder butonu kenar tespiti (sadece basıldığında veri güncelle)
  if (buton_durumu_mod_kapat == HIGH && onceki_durum_mod_kapat == LOW) {  
    veri = 8;
    Serial.println("Veri güncellendi.");
  }
  onceki_durum_mod_kapat = buton_durumu_mod_kapat;

  // Mod gönderme butonu kenar tespiti (sadece basıldığında veri güncelle)
  if (buton_durumu_mod == LOW && onceki_durum_mod == HIGH) {  
    veri = 9;
    Serial.println("Veri güncellendi.(Modla)");
  }
    onceki_durum_mod = buton_durumu_mod;
   // Mod gönderme butonu kenar tespiti (sadece basıldığında veri güncelle)
  if (buton_durumu_geri == LOW && onceki_durum_geri == HIGH) {  
    veri = 3;
    Serial.println("Veri güncellendi.(Modla)");
  }
    onceki_durum_geri = buton_durumu_geri;  
   // sol butonu kenar tespiti (sadece basıldığında veri güncelle)
  if (buton_durumu_sol == LOW && onceki_durum_sol == HIGH) {  
    veri = 2;
    Serial.println("Veri güncellendi.(Modla)");
  }
    onceki_durum_sol = buton_durumu_sol;  

       // sag butonu kenar tespiti (sadece basıldığında veri güncelle)
  if (buton_durumu_sag == LOW && onceki_durum_sag == HIGH) {  
    veri = 4;
    Serial.println("Veri güncellendi.(Modla)");
  }
    onceki_durum_sag = buton_durumu_sag; 
 

  int hiz_veri = analogRead(A3);
  int yon_veri = analogRead(A2);
  int hiz = map(hiz_veri, 0, 1023, 0, 255);
  int yon = map(yon_veri, 0, 1023, 0, 255);

  Serial.print("Joystick A: ");
  Serial.print(hiz);
  Serial.print(" Joystick B: ");
  Serial.println(yon);

  mesaj[0] = veri; 
  mesaj[1] = hiz;
  mesaj[2] = yon;
  verici.write(&mesaj, sizeof(mesaj));
  veri=0;


  Serial.println(ileri_sayac);
} 