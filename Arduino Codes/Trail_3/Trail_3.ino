#include <DFRobot_sim808.h>
#include <sim808.h>
#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>

//********Global Declaration for LCD Crystal****************************************************************
const int rs = 42, en = 43, d4 = 47, d5 = 46, d6 = 41, d7 = 44;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//****************************************************************************************************

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
//*****************************************************************************************************

//*****Global declaration for FingerPrint*****************************************************************
uint8_t id;
boolean FM = false;

SoftwareSerial mySerial(50,51);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//*****************************************************************************************************



void setup() {
  Serial.begin(9600);

  //*****for LCD Crystal******************************************************************************
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  Serial.println("LCD Crystal Setup Done!!");
  
  int TN1=1;
  char displayWords1[]="LCD Crystal Setup Completed!!!";
  for(int d=0 ; d < sizeof(displayWords1)-1 ; d++ )
  {
    if(d<16)
    {
      lcd.setCursor(d,0);lcd.print(displayWords1[d]);
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("                   ");
      lcd.setCursor(0,0);
      for(int d1=TN1 ; d1 < sizeof(displayWords1) ; d1++ )
      {
        lcd.print(displayWords1[d1]);
      }
      TN1++;
    }
    delay(200);
  }
  delay(700);
  lcd.setCursor(0,0);
  lcd.print("                   ");
  //**********************************************************************************************
  
  //*****for Keypad******************************************************************************
  pincode[0]='1';
  pincode[1]='2';
  pincode[2]='3';
  pincode[3]='4';
  pincode[4]='5';
  pincode[5]='6';
  pincode[6]='\0';
  Serial.println("Keypad Setup Done!!");
  
  /*lcd.setCursor(0,0);lcd.print("Keypad Setup Done!!");
  delay(500);
  lcd.setCursor(0,0);lcd.print("                   ");
  delay(500);*/
  
  int TN2=1;
  char displayWords2[]="Keypad Setup Completed!!!";
  for(int d=0 ; d < sizeof(displayWords2)-1 ; d++ )
  {
    if(d<16)
    {
      lcd.setCursor(d,0);lcd.print(displayWords2[d]);
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("                   ");
      lcd.setCursor(0,0);
      for(int d1=TN2 ; d1 < sizeof(displayWords2) ; d1++ )
      {
        lcd.print(displayWords2[d1]);
      }
      TN2++;
    }
    delay(200);
  }
  delay(700);
  lcd.setCursor(0,0);
  lcd.print("                   ");
  //**********************************************************************************************
  

  //*****for Tracking*****************************************************************************
  
  GSMSerial.begin(9600);
  //******** Initialize sim808 module *************
  while(!sim808.init())
  {
      Serial.print("Sim808 initialization error\r\n");
  }
  Serial.println("GSM Setup Done!!");
  
  int TN3=1;
  char displayWords3[]="GSM Setup Completed!!!";
  for(int d=0 ; d < sizeof(displayWords3)-1 ; d++ )
  {
    if(d<16)
    {
      lcd.setCursor(d,0);lcd.print(displayWords3[d]);
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("                   ");
      lcd.setCursor(0,0);
      for(int d1=TN3 ; d1 < sizeof(displayWords3) ; d1++ )
      {
        lcd.print(displayWords3[d1]);
      }
      TN3++;
    }
    delay(200);
  }
  delay(700);
  lcd.setCursor(0,0);
  lcd.print("                   ");
  
  //**********************************************************************************************

  //***for FingerPrint Scanner********************************************************************
  while (!Serial);
  //delay(100);
  Serial.println("\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) 
  {
    Serial.println("Found fingerprint sensor!");
  } 
  else 
  {
    Serial.println("Did not find fingerprint sensor :(");
    while(1) 
    { 
      delay(1); 
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount); 
  Serial.println(" templates");
  Serial.println("FingerPrint Scanner Setup Done!!");

  int TN4=1;
  char displayWords4[]="FingerPrint Scanner Setup Completed!!!";
  for(int d=0 ; d < sizeof(displayWords4)-1 ; d++ )
  {
    if(d<16)
    {
      lcd.setCursor(d,0);lcd.print(displayWords4[d]);
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("                   ");
      lcd.setCursor(0,0);
      for(int d1=TN4 ; d1 < sizeof(displayWords4) ; d1++ )
      {
        lcd.print(displayWords4[d1]);
      }
      TN4++;
    }
    delay(200);
  }
  delay(700);
  lcd.setCursor(0,0);
  lcd.print("                   ");

  //**********************************************************************************************
}

void loop() 
{
  for(int x=0;x<1000;x++)
  {
    MenuOption();
    Serial.println(x);
  }
  lcd.setCursor(0,0);lcd.print("Wait...");
  TrackingFunction();
  lcd.setCursor(0,0);lcd.print("                   ");
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
        int TN4=1;
        char displayWords4[]="Entering Menu Option";
        for(int d=0 ; d < sizeof(displayWords4)-1 ; d++ )
        {
          if(d<16)
          {
            lcd.setCursor(d,0);lcd.print(displayWords4[d]);
          }
          else
          {
            lcd.setCursor(0,0);
            lcd.print("                   ");
            lcd.setCursor(0,0);
            for(int d1=TN4 ; d1 < sizeof(displayWords4) ; d1++ )
            {
              lcd.print(displayWords4[d1]);
            }
            TN4++;
          }
          delay(100);
        }
        delay(700);
        lcd.setCursor(0,0);
        lcd.print("                   ");
        MenuEntries = 0; // Reset the counter value to 0
        //Validating Pincode
        int menuPinCounter=0;
        char pinInput[7];
        pinInput[6]='\0';
        Serial.println("Enter PinCode");
        int TN5=1;
        char displayWords5[]="Enter PinCode";
        for(int d=0 ; d < sizeof(displayWords5)-1 ; d++ )
        {
          if(d<16)
          {
            lcd.setCursor(d,0);lcd.print(displayWords5[d]);
          }
          else
          {
            lcd.setCursor(0,0);
            lcd.print("                   ");
            lcd.setCursor(0,0);
            for(int d1=TN5 ; d1 < sizeof(displayWords5) ; d1++ )
            {
              lcd.print(displayWords5[d1]);
            }
            TN5++;
          }
          delay(200);
        }
        delay(700);
        lcd.setCursor(0,0);
        lcd.print("                   ");
        int ps=0;
        while(menuPinCounter<6)
        {
          //keypad.waitForKey();
          while(true)
          {
            char input = keypad.getKey();
            pinInput[menuPinCounter]=input;
            if (input)Serial.println(input);
            if (input)
            {
              lcd.setCursor(ps++,0);
              lcd.print("*");
              
            }
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

          int TN5=1;
          char displayWords5[]="Entered into menu option ";
          for(int d=0 ; d < sizeof(displayWords5)-1 ; d++ )
          {
            if(d<16)
            {
              lcd.setCursor(d,0);lcd.print(displayWords5[d]);
            }
            else
            {
              lcd.setCursor(0,0);
              lcd.print("                   ");
              lcd.setCursor(0,0);
              for(int d1=TN5 ; d1 < sizeof(displayWords5) ; d1++ )
              {
                lcd.print(displayWords5[d1]);
              }
              TN5++;
            }
            delay(200);
          }
          delay(700);
          lcd.setCursor(0,0);
          lcd.print("                   ");

          Serial.println("Press '1' to change registered Phone Number");
          Serial.println("Press '2' to change Pincode");
          Serial.println("Press '3' add Fingerprint");
          lcd.setCursor(0,0);lcd.print("1.Change Number");
          delay(1000);
          lcd.setCursor(0,0);lcd.print("1.Change Number");
          lcd.setCursor(0,1);lcd.print("2.Change Pincode");
          delay(1000);
          lcd.setCursor(0,0);lcd.print("2.Change Pincode");
          lcd.setCursor(0,1);lcd.print("3.Add FingerPrint");
          delay(1000);
          
          while(true)
          {
            char input = keypad.getKey();
            if(input)Serial.println(input);
            //***Option 1***************************************
            if (input=='1')
            {
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
              Serial.println("Entered Option 1");
              int TN5=1;
              char displayWords5[]="Entered Option 1";
              for(int d=0 ; d < sizeof(displayWords5)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords5[d]);
                }
                else
                {
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN5 ; d1 < sizeof(displayWords5) ; d1++ )
                  {
                    lcd.print(displayWords5[d1]);
                  }
                  TN5++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);
              lcd.print("                   ");
              
              Serial.println("Enter Phone Number then 'Enter' ");
              int TN51=1;
              char displayWords51[]="Enter Phone Number then 'Enter' ";
              for(int d=0 ; d < sizeof(displayWords51)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords51[d]);
                }
                else
                {
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN51 ; d1 < sizeof(displayWords51) ; d1++ )
                  {
                    lcd.print(displayWords51[d1]);
                  }
                  TN51++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);
              lcd.print("                   ");
              
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
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
              Serial.println("Entered Option 2");
              int TN5=1;
              char displayWords5[]="Entered Option 2";
              for(int d=0 ; d < sizeof(displayWords5)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords5[d]);
                }
                else
                {
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN5 ; d1 < sizeof(displayWords5) ; d1++ )
                  {
                    lcd.print(displayWords5[d1]);
                  }
                  TN5++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);
              lcd.print("                   ");
              
              Serial.println("Enter New Pincode then 'Enter' ");
              int TN51=1;
              char displayWords51[]="Enter New Pincode then 'Enter' ";
              for(int d=0 ; d < sizeof(displayWords51)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords51[d]);
                }
                else
                {
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN51 ; d1 < sizeof(displayWords51) ; d1++ )
                  {
                    lcd.print(displayWords51[d1]);
                  }
                  TN51++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);
              lcd.print("                   ");

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
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
              Serial.println("Entered Option 3");
              int TN5=1;
              char displayWords5[]="Entered Option 3";
              for(int d=0 ; d < sizeof(displayWords5)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords5[d]);
                }
                else
                {
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN5 ; d1 < sizeof(displayWords5) ; d1++ )
                  {
                    lcd.print(displayWords5[d1]);
                  }
                  TN5++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);
              lcd.print("                   ");
              
              Serial.println("Enrolling New FingerPrint ID");
              Serial.println("Please type in the ID # (from 1 to 9) you want to save this finger as...");

              char checker='F';
              while(checker=='F')
              {
                id=keypad.getKey();
                if(id)checker='T';
              }

              if(id!='0' && id!='Q')
              {
                Serial.print("Enrolling ID #");
                Serial.println(id);
                while (getFingerprintEnroll()==321);
                break;
              }
              else if(id==0)
              {
                Serial.print("ID #0 not allowed, try again!");
                return;
              }
              else if(id=='Q')
              {
                Serial.println("Exiting Option 3");
              }
              
              
            }
            
            if (input=='Q')
            {
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
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
//****************************************************************************************************************

//***FingerPrintOption***************************************************************************************

//Function 1
uint8_t readnumber(void) 
{
  uint8_t num = 0;
  
  while (num == 0) 
  {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

//Function 2
uint8_t getFingerprintEnroll() 
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as ID #"); 
  Serial.println(id);
  delay(3000);    
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print("");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }
  // OK success!

  p = finger.image2Tz(1);
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }
  Serial.print("ID "); 
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print("");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  // OK converted!
  
  Serial.print("Creating model for ID #");  
  Serial.println(id);
 
  p = finger.createModel();
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Prints matched!");
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    Serial.println("Communication error");
    return p;
  } 
  else if (p == FINGERPRINT_ENROLLMISMATCH) 
  {
    Serial.println("Fingerprints did not match");
    return p;
  } 
  else 
  {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); 
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Stored!");
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    Serial.println("Communication error");
    return p;
  } 
  else if (p == FINGERPRINT_BADLOCATION) 
  {
    Serial.println("Could not store in that location");
    return p;
  } 
  else if (p == FINGERPRINT_FLASHERR) 
  {
    Serial.println("Error writing to flash");
    return p;
  } 
  else 
  {
    Serial.println("Unknown error");
    return p;
  }
  //Serial.println(p);
  int r=321;
  return r; 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  
    return -1;
  
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);
 
  boolean FM = true;
  return FM; 
}

//****************************************************************************************************
