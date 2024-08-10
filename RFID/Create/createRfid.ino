
// Jangan lupa beri komentar agar tidak bingung 
// Ingatlah anak-anak tetaplah berada dijalan setan -Mermaidman

//Bagian library
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//Batas penambahan library

//Mendefinsikan pin
#define RST 3
#define SDA 1
#define LCD_ADDR 0x27 //bisa diganti sesuaikan dengan alamat I2C masing-masing untuk caranya cari tau sendiri
//Batas deklarasi

//Membuat objek untuk komponen
MFRC522 create(SDA, RST);
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
//Batas pembuatan objek

void setup() {
  // Setup awal
  Serial.begin(115200);
  SPI.begin();
  create.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Silahkan tempelkan kartu....:)");
  Serial.println("Tempelkan kartu ke sensor...");
}

void loop() {
  // Mengecek apakah kartu sudah tersedia
  if (!create.PICC_IsNewCardPresent()) {
    return;
  }

  // Memilih satu kartu dari reader
  if (!create.PICC_ReadCardSerial()) {
    return;
  }
  
  //Menunggu input dari Serial Monitor
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    byte data[16];
    input.toCharArray((char *)data, 16); // Mengubah input menjadi array

    // Menulis data ke blok 4
    MFRC522::StatusCode status = create.MIFARE_Write(4, data, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Gagal memasukan data:");
      Serial.println(create.GetStatusCodeName(status));
      return;
    }
    
    // Menampilkan pesan di lcd jika kartu berhasil diubah datanya
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Menulis Data:");
    lcd.setCursor(0, 1);
    lcd.print(input);
    Serial.println("Berhasil menulis data ke kartu: " + input);

    // Akhir dari komunikasi dengan kartu
    create.PICC_HaltA();
  }

}
