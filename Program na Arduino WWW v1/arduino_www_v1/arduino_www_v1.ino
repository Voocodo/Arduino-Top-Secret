/*
25.04.2015 Praca nad zczytwaniem temepratury z seriala
27.05.2015 Dodawanie czujnika DHT22 poprzez dane z seriala
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 */

#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>

bool started = false;
bool ended = false;

const int chipSelect = 4;
int delay_tick=0;
long liczbaPorzadkowa=0;
float temp=0;
int odczytZSeriala =0;
 int licznik=0;
 float temperature =0;
 float humidity= 0;
#define SOP '<'
#define EOP '>'
#define VAR ';'
char inData[4];
int IndexVariable=0;
byte index;
 
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,88,88);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

////////////////////
void readFrame(){
// Read all serial data available, as fast as possible
while(Serial.available() > 0){
    char inChar = Serial.read();
    Serial.println(inChar);
if(inChar == SOP){
       index = 0;
       IndexVariable=0;
       inData[index] = '\0';
       started = true;
       ended = false;}
else if(inChar == EOP){
       //SDsave();
       ended = true;
      // break;
     }     
else{
  if(index <= 22){
    inData[index] = inChar;
       index++;
    inData[index] = '\0';
      }
    }
    
  }

  // We are here either because all pending serial
  // data has been read OR because an end of
  // packet marker arrived. Which is it?
  if(started && ended)
  {
    // The end of packet marker arrived. Process the packet

    // Reset for the next packet
    started = false;
    ended = false;
    index = 0;
    inData[index] = '\0';
  }
}

///////////////////

void czytajDane()
{
 readFrame();
 temperature=inData[0];
 humidity=inData[1];
  
}
 
void setup() {
 
  pinMode(10, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(4,HIGH);
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
    // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print("Initializing SD card...");
    pinMode(10, OUTPUT);
    
    if (!SD.begin(chipSelect)) 
    {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
    }
  Serial.println("card initialized.");
  delay(1000);
}


void loop() 
{
  czytajDane();
  delay(100);
  delay_tick=delay_tick+1;
  
  if (delay_tick>15)
 {
    delay_tick=0;

//Tutaj funkcja do odczytu danych z seriala:




  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  Serial.println(FreeRam());

  File dataFile = SD.open("temp_log.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(temp);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
  } 
 
  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}
  
  
    
  // listen for incoming clients
  EthernetClient client = server.available();



if (client) 
{
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          
          client.println("<html><head><style>");
          client.println(" body { text-align: center; color:white; background: url('http://wallpoper.com/images/00/41/99/62/cosmos_00419962.jpg'); background-size: cover;background-position: center center color:white font-family: Helvetica; h1 {font-size: 75px;}");
          client.println("</style></head>");
          client.println("<body>");
    
          //client.println("<img src='https://asset-6.soup.io/asset/9943/0614_6bba.jpeg' height='250' width='250'>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 1; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("<h1>A");
            client.print(":");
            client.println(sensorReading);
            
            //Wyswietlenie temp i wilg:
            client.print("<br>Temperatura:");
            client.println(temperature);
            client.print("<br>Wilgotnosc:");
            client.println(humidity);
            
            client.println("</h1>");       
          }
          client.println("<h1>Licznik");
          licznik=licznik+1;
          client.print(licznik);
          client.println("</h1></body></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    }
}

void odczytSeriala()
{
while(Serial.available() > 0){
    odczytZSeriala = Serial.read();
    Serial.println("Odczyt z seriala:");
    Serial.println(odczytZSeriala);
}
}

