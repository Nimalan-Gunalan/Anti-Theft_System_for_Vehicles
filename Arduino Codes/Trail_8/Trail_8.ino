#include <DFRobot_sim808.h>
#include <sim808.h>
#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>



//********Global Declaration for LCD Crystal****************************************************************
//const int rs = 42, en = 43, d4 = 47, d5 = 46, d6 = 41, d7 = 44;

const int rs = 42, en = 43, d4 = 44, d5 = 41, d6 = 46, d7 = 47;
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
byte colPins[COLS] = {31,30,29,28}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//****************************************************************************************************

//*****Global declaration for Tracking****************************************************************
#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;
char phone[16];
char datetime[24];
char regnum[] = "+60169120636";//"+60173349706";
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
//uint8_t id;
boolean FM = false;
boolean StoreChk;
char id="";

SoftwareSerial mySerial(50,51);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//*****************************************************************************************************

int IgnitionChecker = 0; 
int PowerState = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);

  //*****for LCD Crystal******************************************************************************
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  Serial.println("LCD Crystal Setup Done!!");
  
  /*int TN1=1;
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
      lcd.setCursor(0,1);lcd.print("                   ");
      lcd.setCursor(0,0);
      for(int d1=TN1 ; d1 < sizeof(displayWords1) ; d1++ )
      {
        lcd.print(displayWords1[d1]);
      }
      TN1++;
    }
    delay(100);
  }
  delay(700);
  lcd.setCursor(0,0);lcd.print("                   ");
  lcd.setCursor(0,1);lcd.print("                   ");*/
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
  
  /*int TN2=1;
  char displayWords2[]="Keypad Setup Completed!!!";
  for(int d=0 ; d < sizeof(displayWords2)-1 ; d++ )
  {
    if(d<16)
    {
      lcd.setCursor(d,0);lcd.print(displayWords2[d]);
    }
    else
    {
      lcd.setCursor(0,0);lcd.print("                   ");
      lcd.setCursor(0,1);lcd.print("                   ");
      lcd.setCursor(0,0);
      for(int d1=TN2 ; d1 < sizeof(displayWords2) ; d1++ )
      {
        lcd.print(displayWords2[d1]);
      }
      TN2++;
    }
    delay(100);
  }
  delay(700);
  lcd.setCursor(0,0);lcd.print("                   ");*/
  //**********************************************************************************************
  

  //*****for Tracking*****************************************************************************
  
  GSMSerial.begin(9600);
  //******** Initialize sim808 module *************
  while(!sim808.init())
  {
      Serial.print("Sim808 initialization error\r\n");
      
      lcd.setCursor(0,0);lcd.print("Sim808 error");
      delay(500);
      lcd.setCursor(0,0);lcd.print("                   "); 
      lcd.setCursor(0,1);lcd.print("                   ");  
  }
  Serial.println("GSM Setup Done!!");
  
  /*int TN3=1;
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
    delay(100);
  }
  delay(700);
  lcd.setCursor(0,0);lcd.print("                   ");*/
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

  /*int TN4=1;
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
    delay(100);
  }
  delay(700);
  lcd.setCursor(0,0);lcd.print("                   ");*/

  digitalWrite(5, HIGH);
  delay(200);
  pinMode(5, OUTPUT);

  lcd.setCursor(0,0);lcd.print("ALL SETUP DONE");
  delay(1000);
  lcd.setCursor(0,0);lcd.print("                   ");   
  //**********************************************************************************************
}

void loop() 
{
  
  for(int x=0;x<2000;x++)
  {
    MenuOption();
    IgnitionCheck();
    Serial.println(x);
    
  }
  lcd.setCursor(0,0);lcd.print("                   ");   
  lcd.setCursor(0,1);lcd.print("                   ");
  lcd.setCursor(0,0);lcd.print("Wait...");
  lcd.setCursor(0,1);lcd.print("                   ");
  TrackingFunction();
  lcd.setCursor(0,0);lcd.print("                   ");   
  lcd.setCursor(0,1);lcd.print("                   ");
}

//***Function Declarations******************************************************************************************

//***Ignition Check***********************************************************************************
void IgnitionCheck(void)
{
  //**************************************
  PowerState = digitalRead(33);
  
  if(PowerState == HIGH)
  { digitalWrite(2, HIGH); }
  else
  { digitalWrite(2, LOW); }
  //**************************************
  IgnitionChecker = digitalRead(32);
  if (IgnitionChecker != LOW)
  {
    for(int i = 1; i <= 3; i++)
    {
      Serial.println("\nWaiting for valid finger...");
      lcd.setCursor(0,0);lcd.print("Waiting for FGPT");
      
      if(getFingerprintIDez() == true && PowerState == HIGH)
      {
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");
          tone(49,1000);
          delay(200);
          noTone(49);
        digitalWrite(4, HIGH);
        Serial.println("Ignition turned ON");
        lcd.setCursor(0,0);lcd.print("Ignition ON");
        delay(1500);
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");
        digitalWrite(4, LOW);
        i=4;
      }
      else
      {
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");   
        lcd.setCursor(0,0);lcd.print("No FGPT Match");
        tone(49,1000);
        delay(800);
        noTone(49);
        delay(200);
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");   
        Serial.println("Fingerprint did not match");
        Serial.print("Failed Trail #");
        Serial.println(i);
        lcd.setCursor(0,0);lcd.print("Failed Trial #");
        lcd.setCursor(14,0);lcd.print(i);
        delay(1200);
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");   
        if(i==3)
        {
          AlertTrackingFunction();
          break;
        }
      }
    }
  }
}
//****************************************************************************************************

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
            lcd.setCursor(0,0);lcd.print("                   ");
            lcd.setCursor(0,0);
            for(int d1=TN4 ; d1 < sizeof(displayWords4) ; d1++ )
            {
              lcd.print(displayWords4[d1]);
            }
            TN4++;
          }
          delay(150);
        }
        delay(700);
        lcd.setCursor(0,0);lcd.print("                   ");
        
        MenuEntries = 0; // Reset the counter value to 0
        
        //Validating Pincode
        int menuPinCounter=0;
        char pinInput[7];
        pinInput[6]='\0';
        Serial.println("Enter PinCode");
        lcd.setCursor(0,0);lcd.print("Enter PinCode");
        delay(1500);
        lcd.setCursor(0,0);lcd.print("                   ");
        int ps=0;
        while(menuPinCounter<6)
        {
          //keypad.waitForKey();
          while(true)
          {
            char input = keypad.getKey();
            pinInput[menuPinCounter]=input;
            if (input)Serial.print(input);
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
          Serial.println(" ");
          Serial.println("Entered into menu option");
            tone(49,1000);
            delay(200);
            noTone(49);
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
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,0);
              for(int d1=TN5 ; d1 < sizeof(displayWords5) ; d1++ )
              {
                lcd.print(displayWords5[d1]);
              }
              TN5++;
            }
            delay(150);
          }
          delay(700);
          lcd.setCursor(0,0);lcd.print("                   ");

          Serial.println("Press '1' to change registered Phone Number");
          Serial.println("Press '2' to change Pincode");
          Serial.println("Press '3' add Fingerprint");
          for(int i=0; i<1; i++)
          {
            lcd.setCursor(0,0);lcd.print("1.Change Number");
            delay(1000);
            lcd.setCursor(0,0);lcd.print("1.Change Number");
            lcd.setCursor(0,1);lcd.print("2.Change Pincode");
            delay(1000);
            lcd.setCursor(0,0);lcd.print("                 ");
            lcd.setCursor(0,1);lcd.print("                 ");
            
            lcd.setCursor(0,0);lcd.print("2.Change Pincode");
            lcd.setCursor(0,1);lcd.print("3.Add FingerPrint");
            delay(1000);
          }
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
              lcd.setCursor(0,1);lcd.print("Entered Option 1");
              delay(1500);
              lcd.setCursor(0,0);lcd.print("                   ");
              
              Serial.println("Enter New Phone Number then 'Ent'");
              int TN51=1;
              char displayWords51[]="Enter New Phone Number then 'Ent'";
              for(int d=0 ; d < sizeof(displayWords51)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords51[d]);
                }
                else
                {
                  lcd.setCursor(0,0);lcd.print("                   ");
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
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
              
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
                    lcd.setCursor(0,0);lcd.print("Exiting Option 1");
                    delay(1000);
                    lcd.setCursor(0,0);lcd.print("                   ");
                    break;
                  }
                  else
                  {
                    char tempregnum[10];
                    
                    if (opt1input!='E' && opt1input!='d' && opt1input!='u' && opt1input!='*' && opt1input!='#' && opt1input!='a' && opt1input!='b')
                    {
                      //Serial.print(opt1input); 
                      tempregnum[t]=opt1input;
                      lcd.setCursor(t,0);
                      lcd.print(tempregnum[t]);
                      Serial.println(tempregnum);
                      t++;
                    }
                    
                    else if(opt1input=='E')
                    {
                      for(int i=0;i<10;i++)
                      {
                        if(tempregnum[i]!='\0')
                        {
                          regnum[i+2]=tempregnum[i];
                        }
                      }
                      Serial.println("New Number Registered");
                      Serial.println(tempregnum);
                      Serial.println(regnum);
              
              int TN11=1;
              char displayWords11[]="New Number Registered";
              for(int d=0 ; d < sizeof(displayWords11)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords11[d]);
                }
                else
                {
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN11 ; d1 < sizeof(displayWords11) ; d1++ )
                  {
                    lcd.print(displayWords11[d1]);
                  }
                  TN11++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
                      lcd.setCursor(0,0);lcd.print("Exiting Option 1");
                      delay(1000);
                      lcd.setCursor(0,0);
                      lcd.print("                   ");
                      break;
                    } 
                  }
                }
                opt1input='\0';
              }break;
            }

            //***Option 2***************************************
            if (input=='2')
            {
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
              Serial.println("Entered Option 2");
              lcd.setCursor(0,1);lcd.print("Entered Option 2");
                           
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
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");

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
                    lcd.setCursor(0,0);lcd.print("Exiting Option 2");
                    delay(1000);
                    lcd.setCursor(0,0);lcd.print("                   ");
                    lcd.setCursor(0,1);lcd.print("                   ");
                    break;
                  }
                  else
                  {
                    char tempPincode[6];
                    
                    if (opt2input!='E')
                    {
                      Serial.println(opt2input); 
                      tempPincode[t2]=opt2input;
                       
                      lcd.setCursor(t2,0);lcd.print(tempPincode[t2]);
                      //Serial.println(tempPincode[t2]);
                      t2++;
                    }
                    
                    else
                    {
                      for(int i=0;i<6;i++)
                      {
                        pincode[i]=tempPincode[i];
                      }
                      Serial.println("New PINCODE Registered");
                      Serial.print(tempPincode);
                      Serial.print(pincode);
                      
                      int TN21=1;
                      char displayWords21[]="New PINCODE Registered";
                      for(int d=0 ; d < sizeof(displayWords21)-1 ; d++ )
                      {
                        if(d<16)
                        {
                          lcd.setCursor(d,0);lcd.print(displayWords21[d]);
                        }
                        else
                        {
                          lcd.setCursor(0,0);
                          lcd.print("                   ");
                          lcd.setCursor(0,0);
                          for(int d1=TN21 ; d1 < sizeof(displayWords21) ; d1++ )
                          {
                            lcd.print(displayWords21[d1]);
                          }
                          TN21++;
                        }
                        delay(200);
                      }
                      delay(700);
                      lcd.setCursor(0,0);
                      lcd.print("                   ");
                      break;
                    }
                  }
                }
                opt2input='\0';
              }break;
            }

            //***Option 3***************************************
            if (input=='3')
            {
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
              Serial.println("Entered Option 3");
              lcd.setCursor(0,1);lcd.print("Entered Option 3"); 
                      
              Serial.println("Enrolling New FingerPrint ID");
              
              int TN52=1;
              char displayWords6[]="Enrolling New FingerPrint ID";
              for(int d=0 ; d < sizeof(displayWords6)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords6[d]);
                }
                else
                {
                  lcd.setCursor(0,0);lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN52 ; d1 < sizeof(displayWords6) ; d1++ )
                  {
                    lcd.print(displayWords6[d1]);
                  }
                  TN52++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);lcd.print("                   ");
              lcd.setCursor(0,1);lcd.print("                   ");
              
              Serial.println("Please type in the ID # (from 1 to 9) you want to save this finger as...");

              lcd.setCursor(0,0);lcd.print("Please type ID #");
              delay(2000);
              lcd.setCursor(0,0);lcd.print("                   ");

              
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
                
                lcd.setCursor(0,0);
                lcd.print("Enrolling ID #");
                lcd.setCursor(14,0);
                lcd.print(id);
                delay(1500);
                
                while (getFingerprintEnroll()==321);
                  
                if(StoreChk==true)
                {
                  Serial.print("Successfully added new fingerprint.");
                  
                  int TN61=1;
                  char displayWords11[]="Successfully added new fingerprint.";
                  for(int d=0 ; d < sizeof(displayWords11)-1 ; d++ )
                  {
                    if(d<16)
                    {
                      lcd.setCursor(d,0);lcd.print(displayWords11[d]);
                    }
                    else
                    {
                      lcd.setCursor(0,0);
                      lcd.print("                   ");
                      lcd.setCursor(0,0);
                      for(int d1=TN61 ; d1 < sizeof(displayWords11) ; d1++ )
                      {
                        lcd.print(displayWords11[d1]);
                      }
                      TN61++;
                    }
                    delay(200);
                  }
                  delay(700);
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                }
                else if(StoreChk==false)
                {
                  Serial.print("Fail to added new fingerprint. Try again...");
                  
                  int TN33=1;
                  char displayWords9[]="Fail to added new fingerprint. Try again...";
                  for(int d=0 ; d < sizeof(displayWords9)-1 ; d++ )
                  {
                    if(d<16)
                    {
                      lcd.setCursor(d,0);lcd.print(displayWords9[d]);
                    }
                    else
                    {
                      lcd.setCursor(0,0);
                      lcd.print("                   ");
                      lcd.setCursor(0,0);
                      for(int d1=TN33 ; d1 < sizeof(displayWords9) ; d1++ )
                      {
                        lcd.print(displayWords9[d1]);
                      }
                      TN33++;
                    }
                    delay(200);
                  }
                  delay(700);
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                }
                
                break;
              }
              else if(id==0)
              {
                Serial.print("ID #0 not allowed, try again!");
                
                int TN54=1;
                char displayWords8[]="ID #0 not allowed, try again!";
                for(int d=0 ; d < sizeof(displayWords8)-1 ; d++ )
                {
                  if(d<16)
                  {
                    lcd.setCursor(d,0);lcd.print(displayWords8[d]);
                  }
                  else
                  {
                    lcd.setCursor(0,0);
                    lcd.print("                   ");
                    lcd.setCursor(0,0);
                    for(int d1=TN54 ; d1 < sizeof(displayWords8) ; d1++ )
                    {
                      lcd.print(displayWords8[d1]);
                    }
                    TN54++;
                  }
                  delay(200);
                }
                delay(700);
                lcd.setCursor(0,0);
                lcd.print("                   ");
                
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
            tone(49,1000);
            delay(800);
            noTone(49);
            
          Serial.println(pinInput);
          Serial.println(pincode);
          lcd.setCursor(0,0);lcd.print("Wrong PINCODE");
          delay(1500);
          Serial.println("Failed to enter into menu option");
          lcd.setCursor(0,0);lcd.print("                   ");   
          lcd.setCursor(0,1);lcd.print("                   ");
          
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
  GSMSerial.begin(9600);
  //******** Initialize sim808 module *************
  while(!sim808.init())
  {
      
  }
  Serial.println("GSM Setup Done!!");
  
  while(!sim808.attachGPS())
  {
      Serial.print(". ");
  }
  if(sim808.attachGPS())
  {
      Serial.println("Open the GPS power success");
  }
  else 
  {
      Serial.println("Open the GPS power failure");
  }
 
  Serial.println("Checking For SMS");
  
  //*********** Detecting unread SMS ************************
   
    
   messageIndex = sim808.isSMSunread();
   Serial.println(messageIndex);
   
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
          GSMSerial.begin(9600);
          //******** Initialize sim808 module *************
          while(!sim808.init())
          {
              Serial.print("Sim808 initialization error\r\n");
              
              lcd.setCursor(0,0);lcd.print("Sim808 error");
              delay(500);
              lcd.setCursor(0,0);lcd.print("                   ");   
              lcd.setCursor(0,1);lcd.print("                   ");
          }
          Serial.println("GSM Setup Done!!");
          if(sim808.attachGPS())
          {
            Serial.println("Open the GPS power success");
          }
          else 
          {
              Serial.println("Open the GPS power failure");
          }
          /*while(!sim808.getGPS())
          {
            //Serial.println("Waiting for GPS information");
          }*/  
          /*Serial.print(sim808.GPSdata.year);
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
          Serial.println(sim808.GPSdata.centisecond);*/
          Serial.print("latitude :");Serial.println("2.821241");
          //Serial.println(sim808.GPSdata.lat);
          Serial.print("longitude :");Serial.println("101.778194");
          //Serial.println(sim808.GPSdata.lon);
          //Serial.print("speed_kph :");
          //Serial.println(sim808.GPSdata.speed_kph);
          //Serial.print("heading :");
          //Serial.println(sim808.GPSdata.heading);
          Serial.println();
    
          float la = 2.821241;//sim808.GPSdata.lat;
          float lo = 101.778194;//sim808.GPSdata.lon;
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

      lcd.setCursor(0,0);lcd.print("Place the Finger");
      /*int TN321=1;
      char displayWords321[]="Waiting for valid finger to enroll";
      for(int d=0 ; d < sizeof(displayWords321)-1 ; d++ )
      {
        if(d<16)
        {
          lcd.setCursor(d,0);lcd.print(displayWords321[d]);
        }
        else
        {
          lcd.setCursor(0,0);
          lcd.print("                   ");
          lcd.setCursor(0,0);
          for(int d1=TN321 ; d1 < sizeof(displayWords321) ; d1++ )
          {
            lcd.print(displayWords321[d1]);
          }
          TN321++;
        }
        delay(200);
      }*/
      delay(2000);
        
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Image taken");
        delay(1500); 
        lcd.setCursor(0,0);lcd.print("                  ");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print("");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Comm error");
        delay(1500); 
        lcd.setCursor(0,0);lcd.print("                  ");
        getFingerprintEnroll(); 
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Imaging error");
        delay(1500); 
        lcd.setCursor(0,0);lcd.print("                  ");
        break;
      default:
        Serial.println("Unknown error");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Unknown error");
        delay(1500); 
        lcd.setCursor(0,0);lcd.print("                  ");
        break;
    }
  }
  // OK success!

  p = finger.image2Tz(1);
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.setCursor(0,0);lcd.print("                  ");
      lcd.setCursor(0,0);lcd.print("Image converted");
      delay(1500); 
      lcd.setCursor(0,0);lcd.print("                  ");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.setCursor(0,0);lcd.print("                  ");
      lcd.setCursor(0,0);lcd.print("Image too messy");
      delay(1500);
      lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.setCursor(0,0);lcd.print("                  ");
      lcd.setCursor(0,0);lcd.print("Com error");
      delay(1500);
      lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.setCursor(0,0);lcd.print("                  ");
      lcd.setCursor(0,0);lcd.print("Could not find fingerprint features");
      delay(1500);
      lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.setCursor(0,0);lcd.print("                  ");
      lcd.setCursor(0,0);lcd.print("Could not find");
      delay(1500);
      lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.setCursor(0,0);lcd.print("                  ");
      lcd.setCursor(0,0);lcd.print("Unknown error");
      delay(1500);
      lcd.setCursor(0,0);lcd.print("                  ");
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.setCursor(0,0);lcd.print("                  ");
  lcd.setCursor(0,0);lcd.print("Remove finger");
  delay(2000);
  lcd.setCursor(0,0);lcd.print("                  ");
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }
  Serial.print("ID "); 
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");

  int TN332=1;
              char displayWords322[]="Place same finger again";
              for(int d=0 ; d < sizeof(displayWords322)-1 ; d++ )
              {
                if(d<16)
                {
                  lcd.setCursor(d,0);lcd.print(displayWords322[d]);
                }
                else
                {
                  lcd.setCursor(0,0);
                  lcd.print("                   ");
                  lcd.setCursor(0,0);
                  for(int d1=TN332 ; d1 < sizeof(displayWords322) ; d1++ )
                  {
                    lcd.print(displayWords322[d1]);
                  }
                  TN332++;
                }
                delay(200);
              }
              delay(700);
              lcd.setCursor(0,0);
              lcd.print("                   ");
  
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
      case FINGERPRINT_OK:     
        Serial.println("Image taken");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Image taken");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print("");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Comm error");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Imaging error");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
        break;
      default:
        Serial.println("Unknown error");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Unknown error");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Image converted");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
      break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Image too messy");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Comm error");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Could not find fingerprint features");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
        lcd.setCursor(0,0);lcd.print("                  ");
        lcd.setCursor(0,0);lcd.print("Could not find fingerprint features");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
      return p;
    default:
      Serial.println("Unknown error");
        lcd.setCursor(0,0);lcd.print("                  ");
        Serial.println("Unknown error");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                  ");
      return p;
  }
  // OK converted!
  
  Serial.print("Creating model for ID #");  
  Serial.println(id);
 
  p = finger.createModel();
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Prints matched!");
    StoreChk = true;
    lcd.setCursor(0,0);lcd.print("                  ");
    lcd.setCursor(0,0);lcd.print("Prints matched!");
    delay(1000);
    lcd.setCursor(0,0);lcd.print("                  ");
    
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    Serial.println("Communication error");
    StoreChk = false;
    lcd.setCursor(0,0);lcd.print("                  ");
    lcd.setCursor(0,0);lcd.print("Comm error");
    delay(1000);
    lcd.setCursor(0,0);lcd.print("                  ");
    return p;
  } 
  else if (p == FINGERPRINT_ENROLLMISMATCH) 
  {
    Serial.println("Fingerprints did not match");
    StoreChk = false;
    lcd.setCursor(0,0);lcd.print("                  ");
    lcd.setCursor(0,0);lcd.print("Did not match");
    delay(1000);
    lcd.setCursor(0,0);lcd.print("                  ");
    return p;
  } 
  else 
  {
    Serial.println("Unknown error");
    StoreChk = false;
    lcd.setCursor(0,0);lcd.print("                  ");
    lcd.setCursor(0,0);lcd.print("Unknown error");
    delay(1000);
    lcd.setCursor(0,0);lcd.print("                  ");
    return p;
    return p;
  }
  

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Stored!");
    StoreChk = true;
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    Serial.println("Communication error");
    StoreChk = false;
    return p;
  } 
  else if (p == FINGERPRINT_BADLOCATION) 
  {
    Serial.println("Could not store in that location");
    StoreChk = false;
    return p;
  } 
  else if (p == FINGERPRINT_FLASHERR) 
  {
    Serial.println("Error writing to flash");
    StoreChk = false;
    return p;
  } 
  else 
  {
    Serial.println("Unknown error");
    StoreChk = false;
    return p;
  }
  //Serial.println(p);
  int r=321;
  return r; 
}



// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() 
{
  //delay(2000);
  uint8_t p = finger.getImage();
  
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
        resetFunction(); 
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        resetFunction(); 
        break;
      default:
        Serial.println("Unknown error");
        resetFunction(); 
        break;
    }
  }

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

//***For Alert Tracking Function**********************************************************************

void AlertTrackingFunction(void)
{
  GSMSerial.begin(9600);
  //******** Initialize sim808 module *************
  while(!sim808.init())
  {
      Serial.print("Sim808 initialization error\r\n");
      
      lcd.setCursor(0,0);lcd.print("Sim808 error");
      delay(500);
      lcd.setCursor(0,0);lcd.print("                   ");   
      lcd.setCursor(0,1);lcd.print("                   ");
  }
  Serial.println("GSM Setup Done!!");
  //************* Turn on the GPS power ************
  while (sim808.attachGPS())
  {
    
  }
  Serial.println("Open the GPS power success");
  

  while (!sim808.getGPS())
    {
      delay(1);
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

  float la = /*2.821241;//*/sim808.GPSdata.lat;
  float lo = /*101.778194;//*/sim808.GPSdata.lon;
  float ws = sim808.GPSdata.speed_kph;

  dtostrf(la, 6, 6, lat); //put float value of la into char array of lat. 6 = number of digits before decimal sign. 2 = number of digits after the decimal sign.
  dtostrf(lo, 6, 6, lon); //put float value of lo into char array of lon
  dtostrf(ws, 6, 2, wspeed);  //put float value of ws into char array of wspeed

  sprintf(MESSAGE, "ALERT MESSAGE!!!\nYour vehicle is being used by unknown user.\nYour vehicle's current location.\nLatitude : %s\nLongitude : %s\nhttp://maps.google.com/maps?q=%s,%s\n", lat, lon, lat, lon);

  Serial.println("Sim808 init success");
  Serial.println("Start to send message ...");
  sim808.sendSMS(regnum, MESSAGE);
  Serial.println(MESSAGE);
  Serial.println(regnum);

  //************* Turn off the GPS power ************
  sim808.detachGPS();

  delay(2000);

  if (sim808.attachGPS())
  {
    Serial.println("Open the GPS power success");
  }
  else
  {
    Serial.println("Open the GPS power failure");
  }

  Serial.println("Please send SMS message to me!");
  Serial.println(sim808.isSMSunread());

  //*********** Detecting unread SMS ************************
  messageIndex = sim808.isSMSunread();

  //*********** Waits for detecting unread SMS ************************
  while (messageIndex == -1)
  {
    messageIndex = sim808.isSMSunread();
    Serial.println(messageIndex);
    delay(1000);
  }

  //*********** At least, there is one UNREAD SMS ***********
  if (messageIndex > 0)
  {
    Serial.print("messageIndex: ");
    Serial.println(messageIndex);

    sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);

    //*********** Reading received message from Sim**********

    Serial.print("From number: ");
    Serial.println(phone);
    Serial.print("Datetime: ");
    Serial.println(datetime);
    Serial.print("Received Message: ");
    Serial.println(message);

    int o=0;
    char AllowCmd[] = "allow";
    for(int i=0; i<sizeof(AllowCmd); i++)
    {
      if(AllowCmd[i]==message[i])
      {
       o++; 
      }
    }
    Serial.println(sizeof(AllowCmd));
    Serial.println(o);
    if (o==6)
    {
      Serial.println("Allowed");
      lcd.setCursor(0,0);lcd.print("Allowed");
      delay(1000);
      lcd.setCursor(0,0);lcd.print("                   ");   
      lcd.setCursor(0,1);lcd.print("                   ");
      int menuPinCounter = 0;
      char pinInput[7];
      pinInput[6] = '\0';
      Serial.println("Enter PinCode");
      lcd.setCursor(0,0);lcd.print("Enter PinCode");
      delay(1000);
      lcd.setCursor(0,0);lcd.print("                   ");   
      lcd.setCursor(0,1);lcd.print("                   ");
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
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");
          tone(49,1000); //Short Beep
          delay(200);
          noTone(49);
        digitalWrite(4, HIGH);
        Serial.println("Ignition turned ON");
        lcd.setCursor(0,0);lcd.print("Ignition ON");
        delay(2000);
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");
        digitalWrite(4, LOW);
      }
      else{
        tone(49,1000); //Short Beep
        delay(800);
        noTone(49);
        Serial.println("Wrong PINCODE");
        lcd.setCursor(0,0);lcd.print("Wrong PINCODE");
        delay(1000);
        lcd.setCursor(0,0);lcd.print("                   ");   
        lcd.setCursor(0,1);lcd.print("                   ");
      }
    }
    else
    {
      return -1;
    }
  }

  //************* Turn off the GPS power ************
  sim808.detachGPS();
}
//****************************************************************************************************

void resetFunction(void)
{
 digitalWrite(5, LOW); 
}
