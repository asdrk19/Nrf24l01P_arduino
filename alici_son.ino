#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2 LCD için
int mesaj[2];
int veri = 0;
int x_ekseni=0;
int y_ekseni=0;
RF24 alici(9, 10); // CE ve CSN PİNLERİ
const int kanal = 157;
bool komut_modu = false;

const int MAKS_KOMUT = 50; // Maksimum komut sayısı
int komut_dizisi[MAKS_KOMUT];
int komut_indeksi = 0; // Dizideki aktif uzunluk

void setup() {
  Serial.begin(9600);
  alici.begin();
  alici.openReadingPipe(1, kanal);
  alici.startListening();

  pinMode(6, OUTPUT); // sag Enable
  pinMode(7, OUTPUT); // sag 1 
  pinMode(8, OUTPUT); // sag 2
  pinMode(5, OUTPUT); // Sol Enable
  pinMode(4, OUTPUT); // Sol 1 
  pinMode(3, OUTPUT); // Sol 2
  pinMode(A0, OUTPUT); // Buzzer

  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  lcd.init(); // LCD başlat
  lcd.backlight(); // Arka ışığı aç
 
  
}

void komutlariUygula() {
  for (int i = 0; i < komut_indeksi; i++) {
    Serial.print("Komut çalıştırılıyor (");
    Serial.print(i + 1);
    Serial.print("): ");
    Serial.println(komut_dizisi[i]);

      //İleri  
    if (komut_dizisi[i] == 1) {
      // Araç ileri yönde 250 hızla 0.5 saniye hareket etsin
      analogWrite(6, 250);   // Enable pini (motor gücü)
      digitalWrite(7, HIGH); // İleri için motor yönü
      digitalWrite(8, LOW);
      analogWrite(5, 250);   // Enable pini (motor gücü)
      digitalWrite(4, HIGH); // İleri için motor yönü
      digitalWrite(3, LOW);
      delay(500);
      analogWrite(6, 0);     // Durdur
      analogWrite(5, 0);     // Durdur
      Serial.print("1de");
    }
      //Sola    
      if (komut_dizisi[i] == 2) {
      // Araç ileri yönde 250 hızla 0.5 saniye hareket etsin
      analogWrite(6, 250);   // Enable pini (motor gücü)
      digitalWrite(7, HIGH); // İleri için motor yönü
      digitalWrite(8, LOW);
      analogWrite(5, 250);   // Enable pini (motor gücü)
      digitalWrite(4, LOW); // İleri için motor yönü
      digitalWrite(3, HIGH);
      delay(500);
      analogWrite(6, 0);     // Durdur
      analogWrite(5, 0);     // Durdur
      Serial.print("2de");
    }
     //Geriye    
      if (komut_dizisi[i] == 3) {
      // Araç ileri yönde 250 hızla 0.5 saniye hareket etsin
      analogWrite(6, 250);   // Enable pini (motor gücü)
      digitalWrite(7, LOW); // İleri için motor yönü
      digitalWrite(8, HIGH);
      analogWrite(5, 250);   // Enable pini (motor gücü)
      digitalWrite(4, LOW); // İleri için motor yönü
      digitalWrite(3, HIGH);
      delay(500);
      analogWrite(6, 0);     // Durdur
      analogWrite(5, 0);     // Durdur
      Serial.print("3de");
    }
     //Sağa    
      if (komut_dizisi[i] == 4) {
      // Araç ileri yönde 250 hızla 0.5 saniye hareket etsin
      analogWrite(6, 250);   // Enable pini (motor gücü)
      digitalWrite(7, LOW); // İleri için motor yönü
      digitalWrite(8, HIGH);
      analogWrite(5, 250);   // Enable pini (motor gücü)
      digitalWrite(4, HIGH); // İleri için motor yönü
      digitalWrite(3, LOW);
      delay(500);
      analogWrite(6, 0);     // Durdur
      analogWrite(5, 0);     // Durdur
      Serial.print("4de");
    }

    // Buraya başka komutlar da eklenebilir (örneğin 2: geri, 3: sağa dön vb.)

    delay(200); // Komutlar arası bekleme
  }
}

void loop() {
  if (alici.available()) {
    bool done = false;
    while (!done) {
      done = alici.read(&mesaj, sizeof(mesaj));
      veri = mesaj[0];
      x_ekseni = mesaj[1];
      y_ekseni= mesaj[2];

      Serial.print("Geldi: ");
      Serial.println(veri);
      lcd.setCursor(0, 0); 
      lcd.print("Geldi!: ");
      lcd.setCursor(10, 0); 
      lcd.print(veri);
      lcd.setCursor(0, 1); 
      lcd.print("X: ");
      lcd.print(x_ekseni);
      lcd.setCursor(8, 1); 
      lcd.print("Y: ");
      lcd.print(y_ekseni);

      if (veri == 9 && !komut_modu) {
        digitalWrite(A0, HIGH); delay(100);
        digitalWrite(A0, LOW);  delay(100);
        digitalWrite(A0, HIGH); delay(100);
        digitalWrite(A0, LOW);
        Serial.println("Komut Modu Başlatıldı.");
        komut_modu = true;
        komut_indeksi = 0;
      } 
      else if (veri == 8 && komut_modu) {
        digitalWrite(A0, HIGH); delay(100);
        digitalWrite(A0, LOW);  delay(100);
        digitalWrite(A0, HIGH); delay(100);
        digitalWrite(A0, LOW);
        Serial.println("Komut Modu Sonlandırıldı. Gelen Komutlar:");
        for (int i = 0; i < komut_indeksi; i++) {
          Serial.print("Komut ");
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.println(komut_dizisi[i]);
        }
        komut_modu = false;

        // Komutları uygula
        komutlariUygula();
      }

      if (komut_modu) {
        if (veri != 0 && veri != 9) {
          if (komut_indeksi < MAKS_KOMUT) {
            komut_dizisi[komut_indeksi] = veri;
            komut_indeksi++;
            Serial.print("Komut eklendi: ");
            Serial.println(veri);
          } else {
            Serial.println("Komut dizisi dolu!");
          }
        }
      }
      
      delay(270);
    }
  } else {
     lcd.setCursor(0, 0); 
  lcd.print("Bekleniyor");
    Serial.println("Veri gelmedi.");
    delay(150);
  }
}
