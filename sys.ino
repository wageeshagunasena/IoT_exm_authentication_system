#include "Nextion.h" //Display LIbrary
#include <SPI.h> //SPI Library
#include <SD.h> //SD card access library
#include <MagStripe.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

/**
 * Fingerprint Sensor Pin configuraion
 * Pin 10 RX Red
 * pin 11 TX White
 * Pin Vin Blue
 * Pin GRD Black
 */
SoftwareSerial softSerial(10, 11);
Adafruit_Fingerprint fingerPrint = Adafruit_Fingerprint(&softSerial);

/**
 * Initialization for the Display
 */
NexPage page8    = NexPage(0, 0, "page8");
NexPage page9    = NexPage(1, 0, "page9");
NexPage page10    = NexPage(2, 0, "page10");
NexPage page11   = NexPage(3, 0, "page11");
NexPage page12    = NexPage(4, 0, "page12");
NexPage page0    = NexPage(5, 0, "page0");
NexPage page1    = NexPage(6, 0, "page1");
NexPage page3    = NexPage(7, 0, "page3");
NexPage page4    = NexPage(8, 0, "page4");
NexPage page6    = NexPage(9, 0, "page6");
NexPage page14    = NexPage(13, 0, "page14");
NexPage page2    = NexPage(14, 0, "page2");
NexPage page16    = NexPage(15, 0, "page16");

//Items
// Page 8
NexButton done8 = NexButton(0, 2, "b0");
NexText txtssid = NexText(0,3,"t1");
// Page 9
NexButton done9 = NexButton(1, 2, "b0");
NexText txtpass = NexText(1,3,"t1");
// Page 11
NexButton next11 = NexButton(3, 2, "b0");
// Page 12
NexButton done12 = NexButton(4, 2, "b0");
NexText txtsubj = NexText(4,3,"t1");
// Page 0
NexButton FPS = NexButton(6, 2, "b0");
NexButton both = NexButton(6,3,"b1");
// Page 3
NexButton next3 = NexButton(8, 5, "b0");
NexButton done3 = NexButton(8, 6, "b1");
NexText txtdept = NexText(8,2,"t0");
NexText txtname = NexText(8,3,"t1");
NexText txtno = NexText(8,4,"t2");
// Page 6
NexButton next6 = NexButton(11, 2, "b0");
NexButton done6 = NexButton(11,3,"b1");
// Page 14
NexButton next14 = NexButton(13, 2, "b1");
// Page 16
NexButton next16 = NexButton(15, 2, "b0");

//Buffers
char buffer[100] = {0};
char buf[100] = {0};

//Wifi
String ssid="";
String pass="";

//Subject ID
String subj="";
String sendda="";

//Mag ID
String studMag="";

boolean Wifi_OK=false;

//Touch listener
NexTouch *nex_listen_list[] = 
{
    &done8,
    &txtssid,
    &done9,
    &txtpass,
    &next11,
    &txtsubj,
    &done12,
    &FPS,
    &both,
    &next3,
    &done3,
    &txtno,
    &txtname,
    &txtdept,
    &next6,
    &done6,
    &next14,
    &next16,
    NULL
};

/**
 * Initializing reading for track 1; can change track by changing the value of card.begin
 * For track 1 reading Purple -> 2; Blue -> 3; Green -> 4; Black -> +5V; White -> GND
 */
MagStripe mgCard;
static const byte DATA_BUFFER_LEN = 108;
static char data[DATA_BUFFER_LEN];

/**
   Magnetic card reader
*/
void scanMagCard() {
  /**
     Initializing reading for track 1; can change track by changing the value of card.begin
     For track 1 reading Purple -> 2; Blue -> 3; Green -> 4; Black -> +5V; White -> GND
  */
  mgCard.begin(1);
  
  // Don't do anything if there isn't a card present...
  Serial.println("Waiting for card to read...");
  short chars = -1;
  while (!mgCard.available() || chars < 0 ) {

    // Read the card into the buffer "data" (as a null-terminated string)...
    chars = mgCard.read(data, DATA_BUFFER_LEN);
//    Serial.println("Read Done");

   
//    if (chars >= 0) {
//      Serial.println(data);
//      int len = studentsDetails[3].length();
//      String subStr = "";
//      for (int i = 0; i < len; i++) {
//        subStr.concat(data[i]);
//      }
//      Serial.println(studentsDetails[3]);
//      return studentsDetails[3].equals(subStr);
//    }

  }

}

/*Send commands to wifi module(esp-01)
 * Serial3 connected 
 * Both green->+3.3v, Both red->+3.3v, Yellow->GND, Brown->14(TX3), Orange->15(RX3)
*/
String command(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
    Serial.println(cmd);
    Serial3.println(cmd); 
    while(Serial3.available())
    {
      if(Serial3.find("OK"))  // Wait till wifi module returns OK
      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8){
    Serial.println("OK");
    return "OK";
  }
  else{
    Serial.println("Error");
    return "ERROR";
  }
}

/*Establishing wifi connection
 * AT commands used
 */
String wifi_init(){
      String stat="";
      command("AT",100);
      command("AT+CWMODE=1",100);
      command("AT+CWQAP",100);  
      command("AT+RST",5000);
//      check4IP(5000);
//      if(!No_IP)
//      {
        Serial.println("Connecting Wifi....");
        if(command("AT+CWJAP=\""+ssid+"\",\""+pass+"\"",7000)=="OK"){
          stat="OK";
          Serial.println("Wifi Connected");
        }
       
      return stat;

}

//Send data through wifi module
void senddata(String webPage){
    int ii=0;
     while(1)
     {
      unsigned int l=webPage.length();
      command("AT+CIPSTART=\"TCP\",\"192.168.8.104\",5000",100);
      Serial.print("AT+CIPSEND=");
      Serial3.print("AT+CIPSEND=");
      Serial.println(l+2);
      Serial3.println(l+2);
      delay(100);
      Serial.println(webPage);
      Serial3.println(webPage);
      while(Serial3.available())
      {
        //Serial.print(Serial.read());
        if(Serial3.find("OK"))
        {
          ii=11;
          sendda="OK";
          break;
        }
      }
      if(ii==11)
      break;
      delay(100);
     }
     command("AT+CIPCLOSE",100);
}

/**
 * Idenfy the ID of the fingerprint
 */
int getFingerprintID() {
  uint8_t p = fingerPrint.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = fingerPrint.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = fingerPrint.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(fingerPrint.fingerID); 
  Serial.print(" with confidence of "); Serial.println(fingerPrint.confidence);
  return fingerPrint.fingerID; 
}

/**
   Fingerprint Scanner
*/
int fingerPrintScanner() {

  //Initializing fingerprint
  dbSerialPrintln("Please wait....");
  fingerPrint.begin(57600);
    if (fingerPrint.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
  }

  delay(2000);
  Serial.println("Place the finger");
  int i = 0;
  while(i<100) {
    int fingerID = getFingerprintID();
    if(fingerID > -1) {
      return fingerID;
      break;
    }
    delay(50);
    i = i+1;
  }

  return -1;
}

// Receive data from server through wifi module
//Start TCP connection with server through port 5000
void receive(){
  command("AT+CIPSTART=\"TCP\",\"192.168.8.104\",5000",100);
  String inp;
  inp=Serial3.readString();
//  Serial.println("out");
  String outs = inp.substring(12);
//  Serial.print(outs);

  String student[10];
  int prev=0;
  int j=0;
  for (int i = 0; i < outs.length(); i++) {  
    if (outs.substring(i, i+1) == ",") {    
      student[j] = outs.substring(prev, i);
      j++;
      prev = i+1;
    }
  }

  String authen=student[0];
  String studID=student[1];
  String studName=student[2];
  Serial.println(authen);
  Serial.println(studID);
  Serial.println(studName);
//  Serial.print("over");

  if(authen=="y"){
     page3.show();

//     Send data to final display
    //Set final department
    memset(buf, 0, sizeof(buf));
    studID.substring(0,2).toCharArray(buf,5);
    txtdept.setText(buf);
    
    //Set final stud no
    memset(buf, 0, sizeof(buf));
    studID.substring(2).toCharArray(buf,10);
    txtno.setText(buf);
    
    //Set final name
    memset(buf, 0, sizeof(buf));
    studName.toCharArray(buf,100);
    txtname.setText(buf);

   
  }else{
    page6.show();
  }
  
}

//SSID page next button pushed
//Save SSID to connect network
void ssidPage(void *ptr) {  
  memset(buffer, 0, sizeof(buffer));
  txtssid.getText(buffer, sizeof(buffer));
  page9.show();
//  ssid = buffer;
  ssid="HAM";
  Serial.println(ssid);
}

//Password page next button pushed
//Save password to connect network
void passPage(void *ptr) {  
  memset(buffer, 0, sizeof(buffer));
  txtpass.getText(buffer, sizeof(buffer));
  page10.show();
//  pass = buffer;
  pass="987654321";
  Serial.println(pass);
  if(wifi_init()=="OK"){
    page11.show();
  }else{
    page14.show();
  }
//  page11.show();
}


//Connection page next butoon pushed
void conPage(void *ptr){
  page12.show();
}

//Subject page next button pushed
//Subject ID save to send to server
void subPage(void *ptr){
  memset(buffer, 0, sizeof(buffer));
  txtsubj.getText(buffer, sizeof(buffer));
//  subj = buffer;
  subj = "EC4421";
  Serial.println(subj);
//  senddata(subj);
//  if(sendda=="OK") page0.show();
  page0.show();
}


//Finger-print only option choosed
//Send finger-print ID to server
void fps(void *ptr){
  senddata("f");
  page1.show();
  senddata(subj);
//  if(sendda=="OK") page0.show();
   String FPID = String(fingerPrintScanner());
//  String FPID = "130";
  senddata(FPID);
  delay(1000);
  receive();
//  page3.show();
}

//Mag-Card and finger-print both option choosed
//Send finger-print ID and Mag-Card ID to server
void bothways(void *ptr){
  senddata("b");
  senddata(subj);
  
  page1.show();
  String FPID = String(fingerPrintScanner());
  Serial.print(FPID);
  
  page4.show();
  scanMagCard();  
  Serial.print(data);
  
  senddata(FPID);
  senddata(data);

  delay(1000);
  receive();
}

//Next student button pushed
void fpsagain(void *ptr){
  page0.show();
}

//Done button pushed
//Email final data to defined Email address
void done(void *ptr){
  senddata("k");
  page2.show();
  delay(4000);
  page16.show();
}

//Next student button pushed
void nextstud(void *ptr){
  page0.show();
}

//Error page next button pushed
//Directed to SSID page again
void error(void *ptr){
  page8.show();
}

//Email send page done button pushed
//Homepage, directed to SSID page
void again(void *ptr){
  page8.show();
}

//Initializing
void setup() {
//  Initialize debug serial
  Serial.begin(115200);

//initializing wifi module serial 
  Serial3.begin(115200);

//  Display fit
  nexInit();

//Action performed from display
  done8.attachPush(ssidPage);
  done9.attachPush(passPage);
  next11.attachPush(conPage);
  done12.attachPush(subPage);
  next3.attachPush(fpsagain);
  done3.attachPush(done);
  FPS.attachPush(fps);
  both.attachPush(bothways);
  next6.attachPush(nextstud);
  done6.attachPush(done);
  next14.attachPush(error);
  next16.attachPush(again);
  Serial.println("setup done"); 

}

//Listin to display actions repeatedly
void loop() {
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);

}
