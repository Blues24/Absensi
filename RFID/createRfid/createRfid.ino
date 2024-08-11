
// Jangan lupa beri komentar agar tidak bingung 
// Ingatlah anak-anak tetaplah berada dijalan setan -Mermaidman

//Bagian library
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
//Batas penambahan library

//GPIO 0 --> PIN D3
//GPIO 2 --> PIN D4
const uint8_t RST_PIN = D3;
const uint8_t SS_PIN = D4;
//--------------------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;        
//--------------------------------------------------
/* Hati-hati dalam mengerjakan Sector Trailer Blocks */
int blockNum = 4;
/* Create array to read data from Block */
/* Length of buffer should be 4 Bytes 
more than the size of Block (16 Bytes) */
byte bufferLen = 18;
byte readBlockData[18];
//--------------------------------------------------
MFRC522::StatusCode status;
//--------------------------------------------------

void setup() 
{
  //------------------------------------------------------
  //Initialize serial communications with PC
  Serial.begin(9600);
  //------------------------------------------------------
  //Initialize SPI bus
  SPI.begin();
  //------------------------------------------------------
  //Initialize MFRC522 Module
  mfrc522.PCD_Init();
  Serial.println("Tempelkan kartu untuk menulis data...");
  //------------------------------------------------------
}

/****************************************************************************************************
 * loop() function
 ****************************************************************************************************/
void loop()
{
  //------------------------------------------------------------------------------
  /* Prepare the key for authentication */
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++){
    key.keyByte[i] = 0xFF;
  }
  //------------------------------------------------------------------------------
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent()){return;}
  //------------------------------------------------------------------------------
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) {return;}
  //------------------------------------------------------------------------------
  Serial.print("\n");
  Serial.println("**Card Detected**");
  /* Print UID of the Card */
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++){
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print("\n");
  /* Print type of card (for example, MIFARE 1K) */
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
    
  //------------------------------------------------------------------------------
  byte buffer[18];
  byte len;
  //wait until 20 seconds for input from serial
  Serial.setTimeout(20000L);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Masukan nomor absen, berakhiran dengan tanda #"));
  len = Serial.readBytesUntil('#', (char *) buffer, 16);
  //add empty spaces to the remaining bytes of buffer
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 4;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Masukan nama, berakhiran dengan tanda #"));
  len = Serial.readBytesUntil('#', (char *) buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 5;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Masukan kelas, berakhiran dengan tanda #"));
  len = Serial.readBytesUntil('#', (char *) buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  blockNum = 6;
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  WriteDataToBlock(blockNum, buffer);
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);
}


/****************************************************************************************************
 * Write() function
 ****************************************************************************************************/
void WriteDataToBlock(int blockNum, byte blockData[]) 
{
   //Serial.print("Writing data on block ");
   //Serial.println(blockNum);
  //------------------------------------------------------------------------------
  /* Authenticating the desired data block for write access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK){
    Serial.print("Gagal untuk menulis data harap cek kembali kartunya: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //------------------------------------------------------------------------------
  else {
    //Serial.print("Authentication OK - ");
  }
  //------------------------------------------------------------------------------
  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Gagal menulis data ke blok memory: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else {
    Serial.println("OK");
  }
  //------------------------------------------------------------------------------
}





/****************************************************************************************************
 * ReadDataFromBlock() function
 ****************************************************************************************************/
void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
   //Serial.print("Reading data from block ");
   //Serial.println(blockNum);
  //----------------------------------------------------------------------------
  /* Prepare the key for authentication */
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  //------------------------------------------------------------------------------
  /* Authenticating the desired data block for Read access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  //------------------------------------------------------------------------------
  if (status != MFRC522::STATUS_OK){
   Serial.print("Gagal untuk membaca kartu: ");
   Serial.println(mfrc522.GetStatusCodeName(status));
   return;
  }
  else {
    Serial.print("Berhasil membaca kartu");
  }
  //------------------------------------------------------------------------------
  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK){
    Serial.print("Gagal membaca kartu: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else {
    //readBlockData[16] = ' ';
    //readBlockData[17] = ' ';
    //Serial.println("Read OK");  
  }
  //------------------------------------------------------------------------------
}



/****************************************************************************************************
 * dumpSerial() function
 ****************************************************************************************************/
void dumpSerial(int blockNum, byte blockData[]) 
{
  Serial.print("\n");
  Serial.print("Data saved on block");
  Serial.print(blockNum);
  Serial.print(": ");
  for (int j=0 ; j<16 ; j++){
    Serial.write(readBlockData[j]);
  }
  Serial.print("\n");

  //Empty readBlockData array
  for( int i = 0; i < sizeof(readBlockData);  ++i )
   readBlockData[i] = (char)0; //empty space
}
