

// Author: srijoni.kuhu@gmail.com

#define BLYNK_PRINT Serial


#include <BlynkSimpleEsp8266.h>                    // th e blynk header
                                                              

#include <ESP8266WiFi.h>
#include <Servo.h>

char auth[] = "A6WNjKxLQR7RwA_55kYd2Pc2D1Dh0YM6";                     // put your auth code sent in mail by blynk comms

int LED = D3;   //for indicating lid opening
int LED1 = D8;  //for indicating full bin
int pos = 0;

WiFiServer server(80);
// Creates a client that can connect to to a specified internet IP address and port as defined in client.connect().
//WiFiClient client;
IPAddress specfic_server(1,2,3,4);
int status;


Servo servo;



int trigpin = D4;                      //this is for opening lid ultrasonic sensor
int echopin = D5;

int trigpin1 = D6;                    //this is for garbage measurer
int echopin1 = D7;

int pointIndex=1;
float lat=22.695025;                 // for locating the dustbin,,if you have a physical gps module call its o/p
float lon=88.379015;

                      //hollow bins for presentation
float lat1=22.695548;                 // for locating the dustbin,,if you have a physical gps module call its o/p
float lon1=88.379727;

                     //hollow bins for presentation
float lat2=22.695114;                 // for locating the dustbin,,if you have a physical gps module call its o/p
float lon2=88.378606;

                      //hollow bins for presentation
float lat3=22.695065;                 // for locating the dustbin,,if you have a physical gps module call its o/p
float lon3=88.37950;

int i=0;     // index switch for lock without facing a e-mail spam condition


          int duration, distance;
          int duration1, distance1;
          int totaldis1 = 100;         // depth of the bin
          int dis1p=0;                //percentage meteres
          int fillp=0; //%
          int limit=90;//%
 



const char* ssid = "saniyal";          // my wifi password
const char* pass= "lollollol";

//const char* ssid = "blackwolfjeet";          // my wifi password
//const char* pass= "1234678";

//const char* ssid = "santikunja";          // my wifi password
//const char* pass= "387172@@";

//const char* ssid = "monowifi";          // kuhu frnd wifi password
//const char* pass= "monojit1234@";


BlynkTimer timer;

                                                                                    // This function sends nodeMCU's up time every second to Virtual Pin (1,3).
                                                                                    // In the app, Widget's reading frequency should be set to PUSH. This means
                                                                                    // that you define how often to send data to Blynk App.
void myTimerEvent()
{
                                                                                    // You can send any value at any time.
                                                                                    // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V3, millis() / 1000);
}


BLYNK_WRITE(V1)
{
  
  int pinValue =param.asInt();              // assigning incoming value from pin V1 to a variable
                                                
  Serial.print("V1 Switch value is: ");    //testing
  Serial.println(pinValue);
  Blynk.virtualWrite(V1,pinValue);

   if((pinValue > 0)&&(pos==0))
          {
            Serial.print(pinValue);     //testing
            Serial.println("opening for municipality");
            digitalWrite(LED, HIGH);
            pos=0;
            for (pos = 0; pos <= 180; pos++) {
              servo.write(pos);
              delay(10);             
            } 
            pos=180; 
             pinValue = 0;   
             distance1=totaldis1; 
             i=0;                        
          }
    else
          {
            Serial.println("its already opened");
          }
  
}


void setup() {
  Serial.begin(9600);                                //connectin to wifi
  delay(10);                                                
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
 

  servo.attach(D1);

  pinMode(LED, OUTPUT);                                                                      // defining physical pins for board use
  pinMode(trigpin, OUTPUT);               //lid opener
  pinMode(echopin, INPUT);

  pinMode(trigpin1, OUTPUT);
  pinMode(echopin1, INPUT);                //garbage measurer

  pinMode(LED1, OUTPUT);     // filled bin
  Blynk.email("srijoni.kuhu@gmail.com","smartbin","SYSTEM ONLINE"); // system live notification
  Serial.print("sent initiation mail");
  // put your setup code here, to run once :
  timer.setInterval(1000L, myTimerEvent);                  // beginning blynk with timer initiation as per cloud guidelines

status = WiFi.status();

  if ( status == WL_CONNECTED) {
    //Tells the server to begin listening for incoming connections.
    server.begin();
    specfic_server = WiFi.localIP();
    // Print the IP address
    Serial.print("Use this URL to connect: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}
}

void loop() {

    Blynk.run();
    timer.run();                      // Initiates BlynkTimer
  
          
            digitalWrite(trigpin, HIGH);
            delayMicroseconds(1000);
            digitalWrite(trigpin, LOW);
            duration = pulseIn(echopin, HIGH);
            distance = (duration / 2) / 29.1;
            Serial.print(distance);
            Serial.println("CM");
            delay(10);

            digitalWrite(trigpin1, HIGH);
            delayMicroseconds(1000);
            digitalWrite(trigpin1, LOW);
            duration1 = pulseIn(echopin1, HIGH);
            distance1 = (duration1 / 2) / 29.1;
            Serial.print(distance1);
            Serial.println("CM");
            delay(10);

               dis1p = (100 * distance1) / totaldis1 ;
               fillp= 100-dis1p;
               Blynk.virtualWrite(V3,fillp);
               String msent=((String)"status- "+fillp+ "%");          //useing this for passing a notator to blynk map widget
               Blynk.virtualWrite(V2,1,lat,lon,msent);
               // hollow bins for display...a municipality looks all these will show a 0% value on the map in app
                Blynk.virtualWrite(V2,2,lat1,lon1,"status- 0%");
                Blynk.virtualWrite(V2,3,lat2,lon2,"status- 0%");
               Blynk.virtualWrite(V2,4,lat3,lon3,"status- 0%");

          if ((distance <= 50) && (fillp < limit)&&(pos==0))                            //      for lid opning   {
          {
            digitalWrite(LED, HIGH);
            for (pos = 0; pos <= 180; pos++) {
              
              servo.write(pos);
              delay(10);
            } 
            pos=180;
            Serial.print("opening");//     }
        
          }
          else if ((distance > 50)&&(pos==180))                        //    for lid closing   {
          {
            digitalWrite(LED, LOW);
            for (pos = 180; pos >= 0; pos--) {
              servo.write(pos);
              delay(10);                                     //    }
            }
            pos=0;
            Serial.print("closing");
          }

          if ((fillp >= limit))                           // for indicating locking the bin onece filled
          {
            digitalWrite(LED1, HIGH);                            
          }
          else
          {
            digitalWrite(LED1, LOW);
          }

  
         
         //

       
            if ((fillp >= limit)&& (i==0))                                                   //for trash collection notification
            {
              Serial.println((String)"status: " + fillp+ "%");
          
              Serial.println((String)"its time to collect the trash , bin is "+ fillp + "% filled");
              delay(1000);
              
              String body= ((String)"the bin is"+fillp+ "% filled,collect the trash from latitude=" +lat+ "longitude=" +lon+"\n \n Your Dustbin number 1 is full, \nPlease click on the link below to get location \nlink: http://www.google.com/maps/place/" + lat+  "," +lon);
              Blynk.email("srijoni.kuhu@gmail.com","smartbin alert",body);
              
              i=1;
          
            }
          
            else if ((fillp < limit))
            {
              Serial.println((String)"status: " + fillp+ "%");
            }
            else{
              Serial.println((String)"status: " + fillp+ "%");
            }
 


BLYNK_WRITE(V1);               // for unlock with spl permission from the app whn municipality comes in

    

             int count = 0;
  
  //Gets a client that is connected to the server and has data available for reading.
  WiFiClient client = server.available();
   
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available() && count != 3) {
    delay(1);
    Serial.print(".");
    count++;
  }

  if (count == 3) {
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
    digitalWrite(LED1, HIGH);
    value = HIGH;
    Serial.println("opening");
    pos=0;
    for (pos = 0; pos <= 180; pos++) {
      
              servo.write(pos);
              delay(10);
            }   
            pos=180;
            i=0;                                 
  }
  if ((request.indexOf("/LED=OFF") != -1)&&(pos==180))  {
    digitalWrite(LED1, LOW);
    value = LOW;
    i=0;
    for (pos = 180; pos >= 0; pos--) {
              servo.write(pos);
              delay(10);                                     //    }
            }
    pos=0;
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





}

//v1-- municipality lock ...   v2-- map ...   v3-- left percentage display unit
//D1-- servo data pin //
//int trigpin = D4;                      //this is for opening lid ultrasonic sensor
//int echopin = D5;

//int trigpin1 = D6;                    //this is for garbage measurer
//int echopin1 = D7;
//
