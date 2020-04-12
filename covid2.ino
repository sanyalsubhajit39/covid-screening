#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>                    // th e blynk header
#include <ESP8266WiFi.h>
//#include <DHT.h>
//#include <Keypad.h>

#include <Servo.h>

//#define DHTPIN 4          //pin where the dht11 is connected
// hospital qeue declaration 
int i=0;
int j=0;
char ch_arr[4][14] = {
                             "BeleGhata ID",
                             "NRS",
                             "P.G.",
                             "beds full"
                           };
                         



int trigpin = D1;                      //this is for opening lid ultrasonic sensor
int echopin = D2;
int buzzerPin = D3;                    //buzzer attach to
int pos=0;                             // servo 1 movement

const char* ssid = "saniyal";
const char* pass = "lollollol";

WiFiServer server(80);
// Creates a client that can connect to to a specified internet IP address and port as defined in client.connect().
//WiFiClient client;
IPAddress specfic_server(1,2,3,4);
int web;
int status;
BlynkTimer timer;

WidgetLCD lcd(V1);

//DHT dht(DHTPIN, DHT11);    //dht11 is the name of the censor board, this is the prototype
int flame_sensor = A0;       // for flame sensor part
int flame_detected;
int t;                       // determining temp from Wleangth
Servo servo;
Servo servo1;


//for getting heights
          int duration, distance,h;       
          int totaldis = 175;         // height of metal detector
          int fh,q,pos1;
          double ot,corner;

char auth[] = "lYeZWoOxDaHCFLytUidSM-0auHgiraOE";                     // put your auth code sent in mail by blynk comms 
int count=0;


const int threshold=400;
String ticketID;

String codeOpen="open";
String codeClose="close";

WiFiClient client;

String textIn="ppp";
BLYNK_WRITE(V0)
{
  String textIn = param.asStr();
  Serial.print(textIn + "\n");
 }

int ambIn=0;
BLYNK_WRITE(V2)
{
  int ambIn = param.asInt();
  Serial.print(ambIn + "\n");
 }


void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

   Blynk.begin(auth, ssid, pass);

  servo.attach(D5);
  servo1.attach(D6);
  pinMode(buzzerPin, OUTPUT); //initialize buzzer as an output

  status = WiFi.status();

  if ( status == WL_CONNECTED) {
    //Tells the server to begin listening for incoming connections.
    server.begin();
    specfic_server = WiFi.localIP();
    web=specfic_server;
    // Print the IP address
    Serial.print("Use this URL to connect: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}
 // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}
                                                                                                                                          
void loop() {
   Blynk.run();
   timer.run();
             }
    
void myTimerEvent()
{                                                                                    // You can send any value at any time.                                                                                    // Please don't send more that 10 values per second
            digitalWrite(trigpin, HIGH);
            delayMicroseconds(1000);
            digitalWrite(trigpin, LOW);
            duration = pulseIn(echopin, HIGH);
            distance = (duration / 2) / 29.1;
            Serial.print(distance);
            Serial.println("CM");
            delay(10);

            fh=distance+4; 
            q= 3*3 + fh*fh;
            ot=sqrt(q);
            corner=acos(9/ot) / 2.14; // corner for servo 2
            h=totaldis-distance;

            if( count==0 && textIn != "ppp" && textIn != codeOpen && textIn != codeClose )
            {
              ticketID=textIn;
              Serial.println("ticket no matched");
              lcd.clear(); 
              lcd.print(1, 0, "ticket no matched"); 
              lcd.print(1, 1, "pls proceed to next step");
              
              //
              if(distance<50)
        {

          Serial.println("height -- "+h );
              lcd.clear(); 
              lcd.print(1, 0,"height -- "+h ); 

          for (pos1 = 0; pos1 <= corner; pos++) {
                    
                  servo.write(pos1);
                  delay(10);
                  } 
                  pos1=corner;
                  Serial.println("targeted");
          delay(100);
           flame_detected = analogRead(flame_sensor);
          for (pos1 = corner; pos1 >= 0; pos--) {
                    servo.write(pos1);
                    delay(10);
                                                         
                  }
                  pos1=0;
                  Serial.print("stepped to prev position");

              t=97056/flame_detected;
              if(flame_detected <= threshold)
              {
                 Serial.println((String)"Temperature:" +t+ " F");
                 delay(1);           
                 Serial.println("thermal range detected");              
                digitalWrite(buzzerPin, HIGH); 
             lcd.clear(); 
             lcd.print(1, 0,(String)"Temperature:"+t+" F" );   
              lcd.print(1, 1, "thermal range detected"); 

// bed denoting
              if(i<10 && j<=3)
              {
                i=i+1;
             //   String hos= ch_arr[j][20];
              }
              else if(i==10 && j<=3)
              {
                j=j+1;
               // String hos=ch_arr[j][14];
                i=0;
                i=i+1;
              }
               if(j>3) 
              {
                // String hos=ch_arr[j][14];
                i=404;
              }
              // till this
              String body= ((String)"**** ATTENTION **** /n the very passenger at gate no.[*1*] with /n **ticket no--# " + ticketID + " # /n carries a potential thermal signature,security please respond /n Use this URL to connect: http://"+ web +"/ /n /n refer the pateint for further tests  /n Slot no. " +i+ "/n hospital : /n-----" +ch_arr[j][14]+" -----");
                Blynk.email("srijoni.kuhu@gmail.com","screening alert",body);
                count=2;
              }
              else
              {
                Serial.println((String)"Temperature: "+t+" F");
                Serial.println("clean");
              lcd.clear(); 
              lcd.print(1, 0,(String)"Temperature: "+t+" F" );
              lcd.print(1, 0, "clean, safe journey"); 
              pos=0;
                 for (pos = 0; pos <= 180; pos++) {
                    
                  servo.write(pos);
                  delay(10);
                  } 
                  pos=180;
                  Serial.println("opened");
                   delay(5000);
                  for (pos = 180; pos >= 0; pos--) {
                    servo.write(pos);
                    delay(10);
                                                         
                  }
                  pos=0;
                  Serial.print("closing");
                  count=0;
                  String textIn="ppp";
                    Serial.println("send new candidate");
                    lcd.clear(); 
              lcd.print(1, 0, "send new candidate"); 
              }
            }
            }
            else if(count==0 && textIn == "ppp")
            {
               Serial.println("enter ticket id");
                lcd.clear(); 
              lcd.print(1, 0, "enter ticket id"); 
              lcd.print(1, 1, "************");
            }
            else if(count==2)
        {
          Serial.println("if the gates are open,pls close the gate ");
        }
            else
            {
               Serial.println("system engaged,pls proceed");
            }
           //

       

          int count1 = 0;

 //  for app lock       
            if(codeOpen==textIn && pos==0)
              {
                digitalWrite(buzzerPin, LOW);
                Serial.println("opening");
                pos=0;
                for (pos = 0; pos <= 180; pos++) {
                  
                          servo.write(pos);
                          delay(10);
                        }                          
                        pos=180;
                        count=2;                                
                }
              else if(codeClose==textIn && pos==180)
                {
                  pos=180;
                  for (pos = 180; pos >= 0; pos--) {
                  servo.write(pos);
                  delay(10);                                     
                  }
                  pos=0;
                   count=0; 
                   String textIn="ppp";
                    Serial.println("send new candidate");
                    lcd.clear(); 
              lcd.print(1, 0, "send new candidate"); 
                  }
          
//   for weblock  
  //Gets a client that is connected to the server and has data available for reading.
  WiFiClient client = server.available();
   
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available() && count1 != 3) {
    delay(1);
    Serial.print(".");
    count++;
  }

  if (count1 == 3) {
    server.begin();
    client = server.available();
    if (!client) {
      return;
    }
  }

  Serial.print(client);

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(buzzerPin, LOW);
    value = HIGH;
    Serial.println("opening");
    pos=0;
    for (pos = 0; pos <= 180; pos++) {
      
              servo.write(pos);
              delay(10);
            }   
            pos=180;
            count=2;                                 
  }
  if ((request.indexOf("/LED=OFF") != -1)&&(pos==180))  {
   
    value = LOW;
    pos=180;
    for (pos = 180; pos >= 0; pos--) {
              servo.write(pos);
              delay(10);                                     //    }
            }
    pos=0;
       count=0;
       String textIn="ppp";
       Serial.println("send new candidate");
       lcd.clear(); 
       lcd.print(1, 0, "send new candidate"); 
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta http-equiv=\"refresh\" \"content=\"5\">");
  client.print("");

  if (value == LOW) {
    client.print("<p style='font-size:100px;'>LED-Off</p>");
  } else {
    client.print("<p style='font-size:100px;'>LED-On</p>");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button style='background:cyan; font-size : 80px; width: 150px; height: 200px;'>On </button></a>");
  
  client.println("<a href=\"/LED=OFF\"\"><button style='background:yellow; font-size : 80px; width: 150px; height: 200px;'>Off </button></a><br />");
  client.println("</meta>");
  client.println("</html>");

  delay(1);
  Serial.println("");

  if(ambIn==1)
  {
     String body1= ((String)"**** /n **** /n send an ambulance for possible corona paiteint to;/n Address :-- Dum-Dum Airport, Gate no 4.");
     Blynk.email("rohan.chakra2000@gmail.com","send ambulance",body1);
     ambIn=0;
  }


}
//Two Library needs
//DHT (1.3.5)
//Adafruit Unified Sensor (1.0.3)

//D1->5
//3v
//GND
