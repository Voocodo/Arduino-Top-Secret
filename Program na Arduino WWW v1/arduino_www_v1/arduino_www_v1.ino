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

const int chipSelect = 4;
int delay_tick=0;
long liczbaPorzadkowa=0;
float temp=0;
int odczytZSeriala =0;
 int licznik=0;
 
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,88,88);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

 
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
  delay(200);
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
            client.print("<br>Temperatura:");
            client.println(temp);
            client.print("<br>Odczyt z Seriala:");
            odczytSeriala();
             client.println(odczytZSeriala)
            
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
