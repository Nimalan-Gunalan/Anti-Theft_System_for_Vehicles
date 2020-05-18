#include <Key.h>
#include <Keypad.h>


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

void setup() {
  keypad.setHoldTime(3000); 
  Serial.begin(9600);
  pincode[0]='1';
  pincode[1]='2';
  pincode[2]='3';
  pincode[3]='4';
  pincode[4]='5';
  pincode[5]='6';
  pincode[6]='\0';
}

void loop() {
  
  char key = keypad.getKey();
  
  if (key){Serial.println(key);} // just print the pressed key
  
  if (key =='*'){
    if (MenuEntries <3){
      MenuEntries++;
      if(MenuEntries == 3) //Entering into Menu option
      {
        //Serial.println(MenuEntries);
        MenuEntries = 0; // Reset the counter value to 0
        //Validating Pincode
        int menuPinCounter=0;
        char pinInput[7];
        pinInput[6]='\0';
        while(menuPinCounter<6)
        {
          //keypad.waitForKey();
          while(true)
          {
            char input = keypad.getKey();
            pinInput[menuPinCounter]=input;
            Serial.println(input);
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
          while(true)
          {
            char input = keypad.getKey();
            Serial.println(input);
            if (input=='1')
            {
              break;
            }
            if (input=='2')
            {
              break;
            }
            if (input=='Q'){break;}
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
