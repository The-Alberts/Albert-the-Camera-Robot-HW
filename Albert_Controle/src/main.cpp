#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Servo.h>



int x;

// This is very similar to Example 3 - Receive with start- and end-markers
//    in Serial Input Basics   http://forum.arduino.cc/index.php?topic=396450.0

/*const byte numChars = 64;
char receivedChars[numChars];

boolean newData = false;

byte ledPin = 13;   // the onboard LED
*/
//===============

/*--------------------------- Constantes ----------------------------*/

#define BAUD            9600    // Frequency of Serial Transmission

#define PIN_MOT1         5
#define PIN_MOT2         6
#define PIN_MOT3         7



/*------------------------- Globale variables -------------------------*/

volatile bool shouldSend_ = false;  // flag ready to send
volatile bool shouldRead_ = false;  // flag ready to read

Servo motor1;
Servo motor2;
Servo motor3;

// Xbox Controller input variables
float LeftJoystickX_    = 0;
float LeftJoystickY_    = 0;
float RightJoystickX_   = 0;
float RightJoystickY_   = 0;
float LeftTrigger_      = 0;
float RightTrigger_     = 0;
bool  LeftBumper_       = 0;
bool  RightBumper_      = 0;
bool  AButton_          = 0;
bool  BButton_          = 0;
bool  XButton_          = 0;
bool  YButton_          = 0;
bool  LeftThumb_        = 0;
bool  RightThumb_       = 0;
bool  LeftDpad_         = 0;
bool  RightDpad_        = 0;
bool  UpDpad_           = 0;
bool  DownDpad_         = 0;
bool  BackButton_       = 0;
bool  StartButton_      = 0;


/*------------------------- Function prototypes -------------------------*/

void timerCallback();
void sendMsg();
void readMsg();
void serialEvent();

void manageSerialCom();
void manageMotors(Servo motor,float inputUp,float inputDown, int minRange, int maxRange, int midRange);
void manageMotors(Servo motor,float input, int minRange, int maxRange, int midRange);




/*------------------------- Main function -------------------------*/

void setup() {

    motor1.attach(PIN_MOT1);
    motor2.attach(PIN_MOT2);
    motor3.attach(PIN_MOT3);

    Serial.begin(BAUD) ;                // serial communication initialisation
    
    while(!Serial) {
    }
}


void loop() {

    manageSerialCom();


    manageMotors(motor1,RightTrigger_,LeftTrigger_,1250,2000,1500);

    manageMotors(motor2,LeftJoystickX_,1250,2000,1500);

    manageMotors(motor3,RightJoystickY_,1250,2000,1500);
}

/*------------------- Function definitions ----------------------*/

void manageMotors(Servo motor,float inputUp,float inputDown, int minRange, int maxRange, int midRange){

    float range;

    if(inputUp>=inputDown)
    {
        range = ((maxRange-midRange) * inputUp) + midRange ;  
    }else{
        range = midRange-((midRange-minRange) * inputDown);
    }

    motor.writeMicroseconds(range);
}

void manageMotors(Servo motor,float input, int minRange, int maxRange, int midRange){

    float range;

    if(input>=0)
    {
        range = ((maxRange-midRange) * input) + midRange ;  
    }else{
        range = midRange-((midRange-minRange) * input);
    }

    motor.writeMicroseconds(range);
}

void manageSerialCom(){

    if (Serial.available() > 0){
        readMsg();

        while (Serial.available() > 0){
            delay(10);
        }
        sendMsg();
    }

}

void serialEvent(){shouldRead_ = true;}

void timerCallback(){shouldSend_ = true;}

void sendMsg(){
    /* Send the Json Message throught the serial port */
    StaticJsonDocument<500> doc;


    // Message Elements
    
    doc["LeftJoystickX"]    = LeftJoystickX_    ;
    //doc["LeftJoystickY"]    = LeftJoystickY_    ;
    //doc["RightJoystickX_"]  = RightJoystickX_   ;
    doc["RightJoystickY_"]  = RightJoystickY_   ;
    doc["LeftTrigger"]      = LeftTrigger_      ;
    doc["RightTrigger"]     = RightTrigger_     ;   
    /*
    doc["LeftBumper"]       = LeftBumper_       ;
    doc["RightBumper"]      = RightBumper_      ;
    
    doc["AButton"]          = AButton_          ;
    doc["BButton"]          = BButton_          ;
    doc["XButton"]          = XButton_          ;
    doc["YButton"]          = YButton_          ;

    doc["LeftThumb"]        = LeftThumb_        ;
    doc["RightThumb"]       = RightThumb_       ;
    doc["LeftDpad"]         = LeftDpad_         ;
    doc["RightDpad"]        = RightDpad_        ;
    doc["UpDpad"]           = UpDpad_           ;
    doc["DownDpad"]         = DownDpad_         ;
    */
    doc["BackButton"]       = BackButton_       ;

    //doc["StartButton"]      = StartButton_      ;
    

    // Serialisation
    serializeJson(doc, Serial);

    // Send
    Serial.println();
    shouldSend_ = false;

}

void readMsg(){
    // Reading Json message
    StaticJsonDocument<500> doc;
    JsonVariant parse_msg;

    // Reading Serial port
    DeserializationError error = deserializeJson(doc, Serial);
    shouldRead_ = false;

    // If Error in message
    if(error) {
        Serial.print("deserialize() failed:");
        Serial.println(error.c_str());
        return;
    }

    // Message analysis

    parse_msg = doc["LeftJoystickX"];
    if(!parse_msg.isNull()){
        LeftJoystickX_ = doc["LeftJoystickX"].as<float>();
    }
    /*
    parse_msg = doc["LeftJoystickY"];
    if(!parse_msg.isNull()){
        LeftJoystickY_ = doc["LeftJoystickY"].as<float>();
    }

    parse_msg = doc["RightJoystickX"];
    if(!parse_msg.isNull()){
        RightJoystickX_ = doc["RightJoystickX"].as<float>();
    }
    */
    parse_msg = doc["RightJoystickY"];
    if(!parse_msg.isNull()){
        RightJoystickY_ = doc["RightJoystickY"].as<float>();
    }

    parse_msg = doc["LeftTrigger"];
    if(!parse_msg.isNull()){
        LeftTrigger_ = doc["LeftTrigger"].as<float>();
    }
    
    parse_msg = doc["RightTrigger"];
    if(!parse_msg.isNull()){
        RightTrigger_ = doc["RightTrigger"].as<float>();
    }
    /*
    parse_msg = doc["LeftBumper"];
    if(!parse_msg.isNull()){
        LeftBumper_ = doc["LeftBumper"].as<bool>();
    }

    parse_msg = doc["RightBumper"];
    if(!parse_msg.isNull()){
        RightBumper_ = doc["RightBumper"].as<bool>();
    }

    
    parse_msg = doc["AButton"];
    if(!parse_msg.isNull()){
        AButton_ = doc["AButton"].as<bool>();
    }

    parse_msg = doc["BButton"];
    if(!parse_msg.isNull()){
        BButton_ = doc["BButton"].as<bool>();
    }

    parse_msg = doc["XButton"];
    if(!parse_msg.isNull()){
        XButton_ = doc["XButton"].as<bool>();
    }

    parse_msg = doc["YButton"];
    if(!parse_msg.isNull()){
        YButton_ = doc["YButton"].as<bool>();
    }
    
    parse_msg = doc["LeftThumb"];
    if(!parse_msg.isNull()){
        LeftThumb_ = doc["LeftThumb"].as<bool>();
    }

    parse_msg = doc["RightThumb"];
    if(!parse_msg.isNull()){
        RightThumb_ = doc["RightThumb"].as<bool>();
    }

    parse_msg = doc["LeftDpad"];
    if(!parse_msg.isNull()){
        LeftDpad_ = doc["LeftDpad"].as<bool>();
    }

    parse_msg = doc["RightDpad"];
    if(!parse_msg.isNull()){
        RightDpad_ = doc["RightDpad"].as<bool>();
    }

    parse_msg = doc["UpDpad"];
    if(!parse_msg.isNull()){
        UpDpad_ = doc["UpDpad"].as<bool>();
    }

    parse_msg = doc["DownDpad"];
    if(!parse_msg.isNull()){
        DownDpad_ = doc["DownDpad"].as<bool>();
    }
    */
    parse_msg = doc["BackButton"];
    if(!parse_msg.isNull()){
        BackButton_ = doc["BackButton"].as<bool>();
    }
    
   /*
    parse_msg = doc["StartButton"];
    if(!parse_msg.isNull()){
        StartButton_ = doc["StartButton"].as<bool>();
    }
   */

    
}

//===============