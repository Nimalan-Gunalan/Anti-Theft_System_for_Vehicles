#include <DFRobot_sim808.h>
#include <sim808.h>
#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>


//********Global Declaration for Keypad****************************************************************
char pincode[7];
int MenuEntries=0;
const byte ROWS = 5; //five rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'a','b','#','*'},
  {'1','2','3','u'},
  {'4','5','6','d'},
  {'7','8','9','Q'},
  {'l','0','r','E'}
};

byte rowPins[ROWS] = {23,24,25,26,27}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {28,29,30,31}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//****************************************************************************************************

//*****Global declaration for Tracking****************************************************************
#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;
char phone[16];
char datetime[24];
char regnum[] = "+60173349706";
char keyword[] = "track";
char MESSAGE[300];
char lat[12];
char lon[12];
char wspeed[12];
int i;

#define PIN_TX 52
#define PIN_RX 53

SoftwareSerial GSMSerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&GSMSerial);
//*****************************************************************************************************/

void setup() {
  Serial.begin(9600);
  //*****for Keypad******************************************************************************
  pincode[0]='1';
  pincode[1]='2';
  pincode[2]='3';
  pincode[3]='4';
  pincode[4]='5';
  pincode[5]='6';
  pincode[6]='\0';
  Serial.println("Keypad Setup Done!!");
  //**********************************************************************************************
  

  //*****for Tracking*****************************************************************************
  GSMSerial.begin(9600);
  //******** Initialize sim808 module *************
  while(!sim808.init())
  {
      Serial.print("Sim808 initialization error\r\n");
  }
  Serial.println("GSM Setup Done!!");
  //**********************************************************************************************/
  
}

void loop() {
  
  for(int x=0;x<800;x++)
  {
    MenuOption();
    Serial.println(x);
  }
  TrackingFunction();
  
  
}

//***Function Declarations******************************************************************************************

//***MenuOption***************************************************************************************
void MenuOption(void)
{
  char key = keypad.getKey();
  
  if (key){Serial.println(key);} // just print the pressed key
  
  if (key =='*'){
    if (MenuEntries <3){
      MenuEntries++;
      if(MenuEntries == 3) //Entering into Menu option
      {
        Serial.println("Entering Menu Option");
        MenuEntries = 0; // Reset the counter value to 0
        //Validating Pincode
        int menuPinCounter=0;
        char pinInput[7];
        pinInput[6]='\0';
        Serial.println("Enter PinCode");
        while(menuPinCounter<6)
        {
          //keypad.waitForKey();
          while(true)
          {
            char input = keypad.getKey();
            pinInput[menuPinCounter]=input;
            if (input)Serial.println(input);
            if (input){break;}
          }
          menuPinCounter++;
        }
        delay(500);
        int chk=0;
        for(int i = 0; i < 6; i++)
        {
          if(pincode[i]==pinInput[i])
          chk++;
        }
        if(chk==6)
        {
          //Short Beep
          Serial.println("Entered into menu option");
          Serial.println("Press '1' to change registered Phone Number");
          Serial.println("Press '2' to change Pincode");
          Serial.println("Press '3' add Fingerprint");
          while(true)
          {
            char input = keypad.getKey();
            if(input)Serial.println(input);
            //***Option 1***************************************
            if (input=='1')
            {
              Serial.println("Entered Option 1");
              Serial.println("Enter Phone Number then 'Enter' ");
              int t=0;
              char opt1input='\0';
              while(opt1input=='\0') 
              {
                opt1input = keypad.getKey();

                if(opt1input!='\0')
                {
                  if(opt1input=='Q')
                  {
                    Serial.println("Exiting Option 1");
                    break;
                  }
                  else
                  {
                    char tempregnum[12];
                    
                    if (opt1input!='E')
                    {
                      Serial.println(opt1input); 
                      tempregnum[t]=opt1input;
                      Serial.println(tempregnum[t]);
                      t++;
                    }
                    
                    else
                    {
                      for(int i=0;i<12;i++)
                      {
                        regnum[i+2]=tempregnum[i];
                      }
                      Serial.println("New Registered Number");
                      Serial.print(tempregnum);
                      Serial.print(regnum);
                      break;
                    }
                  }
                }
                opt1input='\0';
              }
            }

            //***Option 2***************************************
            if (input=='2')
            {
              Serial.println("Entered Option 2");
              Serial.println("Enter New Pincode then 'Enter' ");

              int t2=0;
              char opt2input='\0';
              while(opt2input=='\0') 
              {
                opt2input = keypad.getKey();

                if(opt2input!='\0')
                {
                  if(opt2input=='Q')
                  {
                    Serial.println("Exiting Option 2");
                    break;
                  }
                  else
                  {
                    char tempPincode[6];
                    
                    if (opt2input!='E')
                    {
                      Serial.println(opt2input); 
                      tempPincode[t2]=opt2input;
                      Serial.println(tempPincode[t2]);
                      t2++;
                    }
                    
                    else
                    {
                      for(int i=0;i<6;i++)
                      {
                        pincode[i]=tempPincode[i];
                      }
                      Serial.println("New Registered Number");
                      Serial.print(tempPincode);
                      Serial.print(pincode);
                      break;
                    }
                  }
                }
                opt2input='\0';
              }
            }

            //***Option 3***************************************
            if (input=='3')
            {
              Serial.println("Entered Option 3");
            }
            if (input=='Q')
            {
              Serial.println("Exited Menu Option");
              break;
            }
            
          }
          chk=0;
        }
        else
        {
          //Long beep
          Serial.println(pinInput);
          Serial.println(pincode);
          Serial.println("Failed to enter into menu option");
        }
        
        
      }
    } 
  }
  else if(key!='\0' && key!='*')
  {
    Serial.println(key);
    MenuEntries=0;
  }
}
//*****For tracking************************************************************************************
void TrackingFunction(void)
{
  /*if(sim808.attachGPS())
  {
      Serial.println("Open the GPS power success");
  }
  else 
  {
      Serial.println("Open the GPS power failure");
  }*/
  Serial.println("Init Success, please send SMS message to me!");
  //Serial.println(sim808.isSMSunread());
  
  //*********** Detecting unread SMS ************************
   messageIndex = sim808.isSMSunread();


   //*********** At least, there is one UNREAD SMS ***********
   if (messageIndex > 0)
   { 
      Serial.print("messageIndex: ");
      Serial.println(messageIndex);

      sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
                 
      //***********In order not to full SIM Memory, is better to delete it**********
      
      Serial.print("From number: ");
      Serial.println(phone);  
      Serial.print("Datetime: ");
      Serial.println(datetime);        
      Serial.print("Received Message: ");
      Serial.println(message); 

      Serial.println(regnum);
      if(strcmp(regnum,phone) == 0)
      {
        if(strcmp(keyword,message) == 0)
        {
          if(sim808.attachGPS())
          {
            Serial.println("Open the GPS power success");
          }
          else 
          {
              Serial.println("Open the GPS power failure");
          }
          while(!sim808.getGPS())
          {
            //Serial.println("Waiting for GPS information");
          }
          Serial.print(sim808.GPSdata.year);
          Serial.print("/");
          Serial.print(sim808.GPSdata.month);
          Serial.print("/");
          Serial.print(sim808.GPSdata.day);
          Serial.print(" ");
          Serial.print(sim808.GPSdata.hour);
          Serial.print(":");
          Serial.print(sim808.GPSdata.minute);
          Serial.print(":");
          Serial.print(sim808.GPSdata.second);
          Serial.print(":");
          Serial.println(sim808.GPSdata.centisecond);
          Serial.print("latitude :");
          Serial.println(sim808.GPSdata.lat);
          Serial.print("longitude :");
          Serial.println(sim808.GPSdata.lon);
          Serial.print("speed_kph :");
          Serial.println(sim808.GPSdata.speed_kph);
          Serial.print("heading :");
          Serial.println(sim808.GPSdata.heading);
          Serial.println();
    
          float la = sim808.GPSdata.lat;
          float lo = sim808.GPSdata.lon;
          float ws = sim808.GPSdata.speed_kph;
    
          dtostrf(la, 6, 6, lat); //put float value of la into char array of lat. 6 = number of digits before decimal sign. 2 = number of digits after the decimal sign.
          dtostrf(lo, 6, 6, lon); //put float value of lo into char array of lon
          dtostrf(ws, 6, 2, wspeed);  //put float value of ws into char array of wspeed
        
          sprintf(MESSAGE, "Latitude : %s\nLongitude : %s\nWind Speed : %s kph\nhttp://maps.google.com/maps?q=%s,%s\n", lat, lon, wspeed, lat, lon);
        
    
          Serial.println("Sim808 init success");
          Serial.println("Start to send message ...");
      
          Serial.println(MESSAGE);
          Serial.println(phone);
    
          sim808.sendSMS(phone,MESSAGE);
          //for(i=messageIndex; i>0; i--)
          //{
            sim808.deleteSMS(messageIndex);
            //Serial.println(i);
          //}
          
          //************* Turn off the GPS power ************
          sim808.detachGPS();
        }
        else
        {
        sprintf(MESSAGE, "Wrong Keyword!!!");

        Serial.println("Sim808 init success");
        Serial.println("Start to send message ...");
    
        Serial.println(MESSAGE);
        Serial.println(phone);
  
        sim808.sendSMS(phone,MESSAGE);
        for(i=messageIndex; i>0; i--)
        {
          sim808.deleteSMS(i);
          Serial.println(i);
        }
        
        //************* Turn off the GPS power ************
        //sim808.detachGPS();
        }
      }
      else
      {
        sprintf(MESSAGE, "Wrong phone!!!");

        Serial.println("Sim808 init success");
        Serial.println("Start to send message ...");
    
        Serial.println(MESSAGE);
        Serial.println(phone);
  
        sim808.sendSMS(phone,MESSAGE);
        for(i=messageIndex; i>0; i--)
        {
          sim808.deleteSMS(i);
          Serial.println(i);
        }
        
        //************* Turn off the GPS power ************
        sim808.detachGPS();
      }
      
   }
}
//****************************************************************************************************************/
