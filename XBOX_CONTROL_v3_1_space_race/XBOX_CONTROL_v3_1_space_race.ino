
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>


//Control de Xbox
//XboxSeriesXControllerESP32_asukiaaa::Core xboxController("14:CB:65:F7:18:AC");  //Miguel
//XboxSeriesXControllerESP32_asukiaaa::Core xboxController("14:cb:65:b9:1f:cf"); //felipe
XboxSeriesXControllerESP32_asukiaaa::Core xboxController("F4:6A:D7:A6:A8:D9"); //Angieth
//XboxSeriesXControllerESP32_asukiaaa::Core xboxController("3C:FA:06:48:E3:CB"); //Anaso


//Valores del Control
float LHoN=0,RTrig=0,LTrig=0;
/*const uint16_t Disminuir_rigth=700,Disminuir_left=200,Disminuir_rigth_tur=700,Disminuir_left_tur=700,Disminuir_rigth_back=200,Disminuir_rigth_back_tur=700,Disminuir_left_back=200,Disminuir_left_back_tur=700,Disminuir_Del=700;
const uint16_t Disminuir_del_tur=700,Disminuir_back_turbo=700,Disminuir_back=700;*/
const uint16_t Disminuir_rigth=700,Disminuir_left=200,Disminuir_rigth_tur=700,Disminuir_left_tur=700,Disminuir_rigth_back=200,Disminuir_rigth_back_tur=700,Disminuir_left_back=200,Disminuir_left_back_tur=700,Disminuir_Del=700;
const uint16_t Disminuir_del_tur=700,Disminuir_back_turbo=700,Disminuir_back=700;
//Controlador Motores
const byte Valor_A1 =18 ; //Corriente A1
const byte Valor_PWA = 5; //PWM A
const byte Valor_A2 = 19;//Corriente A2
const byte Valor_B1 =22 ; //Corriente B1
const byte Valor_PWB= 33; //PWM B
const byte Valor_B2= 23;//Coriente B2

//Creación del PWM para la ESP32
const uint16_t Frecuencia = 5000;
const byte Resolucion = 10;
const byte Canal0 = 0;
const byte Canal2 = 2;

//Variables de Control de Velocidad
uint16_t Vel=0;
uint16_t vel2 = 0;


//Disparador
const byte Entrada_1=4;
int tiempo_1=6000;
int time_2=0;

//Turbo
bool Turbo = false, in=true, Co=true;
const byte Led=2;


//Inicializar Variables
void setup() {
  //Serial
  Serial.begin(115200);

  //Xbox
  xboxController.begin();
  
  //Controlador de los Motores
  pinMode(Valor_A1,OUTPUT);
  pinMode(Valor_PWA,OUTPUT);
  pinMode(Valor_A2,OUTPUT);
  pinMode(Valor_B1,OUTPUT);
  pinMode(Valor_PWB,OUTPUT);
  pinMode(Valor_B2,OUTPUT);

  //Disparador
  pinMode(Entrada_1,OUTPUT);
  pinMode(Led,OUTPUT);
  digitalWrite(Entrada_1,LOW);


  //Creación del PWM
  CrearPWM();
  

  Serial.println("Inicializado Correctamente");
}

void loop() {

  xboxController.onLoop();

  //Desactivar Disparador (Preventivo)
  if((millis() - time_2) >= 300){
      digitalWrite(Entrada_1,LOW);
  }

  //Inicio de Logica
  if (xboxController.isConnected()) {

    //Buscar el Control
    if (xboxController.isWaitingForFirstNotification()) {
        Serial.println("waiting for first notification");
    } 
    else {


      //Control Conectado

      //Desactivar Disparador
      if((millis() - time_2) >= 300){
        digitalWrite(Entrada_1,LOW);
      }
      
      //Leer valores del Control
      uint16_t joystickMax = XboxControllerNotificationParser::maxJoy; //Maximo del Joystick
      LHoN=(float)xboxController.xboxNotif.joyLHori / joystickMax; //Joystick Izquierdo Horizontal Normalizado
      RTrig=xboxController.xboxNotif.trigRT;//Triger RT
      LTrig=xboxController.xboxNotif.trigLT;
      if (LHoN >0.6) {
        if(!(RTrig >50 or LTrig > 50)){
          //Motor A
          ledcWrite(Canal0,50);
          digitalWrite(Valor_A1,LOW);
          digitalWrite(Valor_A2,HIGH);
          //Motor B Izquierda
          ledcWrite(Canal2,200);
          digitalWrite(Valor_B1,HIGH);
          digitalWrite(Valor_B2,LOW);
          Serial.println("Derecha");
        }
      }
      else if(LHoN <0.4){
        if(!(RTrig >50 or LTrig > 50)){
          ledcWrite(Canal0,200);
          digitalWrite(Valor_A1,HIGH);
          digitalWrite(Valor_A2,LOW);
          //Motor B Izquierda
          ledcWrite(Canal2,50);
          digitalWrite(Valor_B1,LOW);
          digitalWrite(Valor_B2,HIGH);
          Serial.println("Izquierda");
        }
      }
      if (LHoN >0.6) {
        if(RTrig > 50 ){
          //Gira a la Derecha
          if(!Turbo){
            //Sin Turbo
            Vel = RTrig-Disminuir_rigth;
            vel2 = RTrig;

            if(Vel<0){
              Vel=0;
            }else if(Vel > 100){
              Vel=100;
            }
            if(vel2 > 350){
              vel2 = 350;
            }

            //Motor A
            ledcWrite(Canal0,Vel);
            digitalWrite(Valor_A1,LOW);
            digitalWrite(Valor_A2,HIGH);
            //Motor B Izquierda
            ledcWrite(Canal2,vel2);
            digitalWrite(Valor_B1,HIGH);
            digitalWrite(Valor_B2,LOW);
            Serial.println("Derecha");
          }
          if(Turbo){
            //Gira a la Derecha
            //Motor A Derecha
            Vel = RTrig-Disminuir_rigth_tur;
            vel2 = RTrig;

            if(Vel<0){
              Vel=0;
            }
            if(vel2 < 0){
              vel2=0;
            }
            if(vel2 > 10 && vel2<350){
              vel2=vel2 + 350;
            }
            else if(vel2 >= 700){
              vel2=700;
            }
          
            ledcWrite(Canal0,Vel);
            digitalWrite(Valor_A1,LOW);
            digitalWrite(Valor_A2,HIGH);
            //Motor B Izquierda
            ledcWrite(Canal2,vel2);
            digitalWrite(Valor_B1,HIGH);
            digitalWrite(Valor_B2,LOW);
            Serial.println("Derecha Turbo");
          }
        }
        if(LTrig>50){
          if(!Turbo){
            //Gira a la Derecha Atras
            Vel = LTrig-Disminuir_rigth_back;
            vel2 = LTrig;
            if(Vel<0){
              Vel=0;
            }else if(Vel > 100){
              Vel=100;
            }
            if(vel2 > 350){
              vel2 = 350;
            }
              ledcWrite(Canal0,Vel);
              digitalWrite(Valor_A1,HIGH);
              digitalWrite(Valor_A2,LOW);
              //Motor B Izquierda
              ledcWrite(Canal2,vel2);
              digitalWrite(Valor_B1,LOW);
              digitalWrite(Valor_B2,HIGH);
              Serial.println("Derecha Atras");
          }
          if(Turbo){
             //Gira a la Derecha Atras
              //Motor A Derecha
              //Motor A Derecha
            Vel = LTrig-Disminuir_rigth_back_tur;
            vel2 = LTrig;
            if(Vel<0){
              Vel=0;
            }
            if(vel2 < 0){
              vel2=0;
          }
            if(vel2 > 10 && vel2<350){
              vel2=vel2 + 350;
            }
            else if(vel2 >= 700){
              vel2=700;
            }
              ledcWrite(Canal0,Vel);
              digitalWrite(Valor_A1,HIGH);
              digitalWrite(Valor_A2,LOW);
              //Motor B Izquierda
              ledcWrite(Canal2,vel2);
              digitalWrite(Valor_B1,LOW);
              digitalWrite(Valor_B2,HIGH);
              Serial.println("Derecha Atras Turbo");
          }  
        }  
      }
      else if(LHoN<0.4 ){
        if(RTrig>50){
          if(!Turbo){
            //Gira a la Izquierda
            Vel = RTrig-Disminuir_left;
            vel2 = RTrig;
            if(Vel<0){
              Vel=0;
            }else if(Vel > 100){
              Vel=100;
            }
            if(vel2 > 350){
              vel2 = 350;
            }
            ledcWrite(Canal0,vel2);
            digitalWrite(Valor_A1,HIGH);
            digitalWrite(Valor_A2,LOW);
            //Motor B Izquierda
            ledcWrite(Canal2,Vel);
            digitalWrite(Valor_B1,LOW);
            digitalWrite(Valor_B2,HIGH);
            Serial.println("Izquierda");
          }
          if(Turbo){
            //Gira a la Izquierda
            //Motor A Derecha
            //Motor A Derecha
            Vel = RTrig-Disminuir_left_tur;
            vel2 = RTrig;
            if(Vel<0){
              Vel=0;
            }
            if(vel2 < 0){
              vel2=0;
            }
            if(vel2 > 10 && vel2<350){
              vel2=vel2 + 350;
            }
            else if(vel2 >= 700){
              vel2=700;
            }
            ledcWrite(Canal0,vel2);
            digitalWrite(Valor_A1,HIGH);
            digitalWrite(Valor_A2,LOW);
            //Motor B Izquierda
            ledcWrite(Canal2,Vel);
            digitalWrite(Valor_B1,LOW);
            digitalWrite(Valor_B2,HIGH);
            Serial.println("Izquierda Turbo");
          }
        }
        if(LTrig>50){
          if(!Turbo){
            //Gira a la Izquierda Atras
            //Motor A Derecha
            Vel = LTrig-Disminuir_left_back;
            vel2 = LTrig;
            if(Vel<0){
              Vel=0;
            }else if(Vel > 100){
              Vel=100;
            }
            if(vel2 > 350){
              vel2 = 350;
            }
            ledcWrite(Canal0,vel2);
            digitalWrite(Valor_A1,LOW);
            digitalWrite(Valor_A2,HIGH);
            //Motor B Izquierda
            ledcWrite(Canal2,Vel);
            digitalWrite(Valor_B1,HIGH);
            digitalWrite(Valor_B2,LOW);
            Serial.println("Izquierda Atras");
          }
          if(Turbo){
            //Gira a la Izquierda Atras
            //Motor A Derecha
            Vel = LTrig-Disminuir_left_back_tur;
            vel2 = LTrig;
            if(Vel<0){
              Vel=0;
            }
            if(vel2 < 0){
              vel2=0;
            }
            if(vel2 > 10 && vel2<350){
              vel2=vel2 + 350;
            }
            else if(vel2 >= 700){
              vel2=700;
            }
            ledcWrite(Canal0,vel2);
            digitalWrite(Valor_A1,LOW);
            digitalWrite(Valor_A2,HIGH);
            //Motor B Izquierda
            ledcWrite(Canal2,Vel);
            digitalWrite(Valor_B1,HIGH);
            digitalWrite(Valor_B2,LOW);
            Serial.println("Izquierda Atras Turbo");
          }
        }
      }
      else if(RTrig>50 ){
        if(!Turbo){
          int vel3=0;
          Vel = RTrig-Disminuir_Del;
          vel2 = RTrig;
          if(Vel<0){
            Vel=0;
          }else if(Vel > 100){
            Vel=100;
          }
          if(vel2 > 600){
            vel2 = 600;
          }
          if(vel2 != 0){
            vel3=vel2;
          }
          else{
            vel3=0;
          }
          
          ledcWrite(Canal0,vel2);
          digitalWrite(Valor_A1,HIGH);
          digitalWrite(Valor_A2,LOW);
          //Motor B Izquierda
          ledcWrite(Canal2,vel3);
          digitalWrite(Valor_B1,HIGH);
          digitalWrite(Valor_B2,LOW);
          Serial.println("Adelante");
        }else if(Turbo){
          Vel = RTrig-Disminuir_del_tur;
          vel2 = RTrig;
          if(Vel<0){
            Vel=0;
          }
          if(vel2 < 0){
            vel2=0;
          }
          if(vel2 > 10 && vel2<500){
            vel2=vel2 + 500;
          }
          else if(vel2 >= 1000){
            vel2=1000;
          }
          ledcWrite(Canal0,vel2);
          digitalWrite(Valor_A1,HIGH);
          digitalWrite(Valor_A2,LOW);
          //Motor B Izquierda
          ledcWrite(Canal2,vel2);
          digitalWrite(Valor_B1,HIGH);
          digitalWrite(Valor_B2,LOW);
          Serial.println("Adelante Turbo");
        }
      }
      else if(LTrig>50){
        if(!Turbo){
          Vel = LTrig-Disminuir_back;
          vel2 = LTrig;
          if(Vel<0){
            Vel=0;
          }else if(Vel > 100){
            Vel=100;
          }
          if(vel2 > 500){
            vel2 = 500;
          }
          ledcWrite(Canal0,vel2);
          digitalWrite(Valor_A1,LOW);
          digitalWrite(Valor_A2,HIGH);
          //Motor B Izquierda
          ledcWrite(Canal2,vel2);
          digitalWrite(Valor_B1,LOW);
          digitalWrite(Valor_B2,HIGH);
          Serial.println("Atras");
        }else if(Turbo){
          Vel = LTrig-Disminuir_back_turbo;
          vel2 = LTrig;
          if(Vel<0){
            Vel=0;
          }
          if(vel2 < 0){
            vel2=0;
          }
          if(vel2 > 10 && vel2<500){
            vel2=vel2 + 500;
          }
          else if(vel2 >= 1000){
            vel2=1000;
          }
          ledcWrite(Canal0,vel2);
          digitalWrite(Valor_A1,LOW);
          digitalWrite(Valor_A2,HIGH);
          //Motor B Izquierda
          ledcWrite(Canal2,vel2);
          digitalWrite(Valor_B1,LOW);
          digitalWrite(Valor_B2,HIGH);
          Serial.println("Atras Turbo");
        }
      }
      //Detenido
      if(!(RTrig >50 or LTrig > 50)){
          //ledcWrite(Canal0,0);
          digitalWrite(Valor_A1,LOW);
          digitalWrite(Valor_A2,LOW);
          //Motor B Izquierda
          //ledcWrite(Canal2,0);
          digitalWrite(Valor_B1,LOW);
          digitalWrite(Valor_B2,LOW);
          //Serial.println("Detenido");
      }
      //Iniciar Turbo
      if(xboxController.xboxNotif.btnRB  && Co){
          Turbo=!Turbo;
          Co=false;
          digitalWrite(Led,HIGH);
          Serial.print("Turbo: ");
          Serial.println(Turbo);
      }
      //Apagar Turbo
      else if(xboxController.xboxNotif.btnLB && !Co){
          Turbo=!Turbo;
          Co=true;
          digitalWrite(Led,LOW);
          Serial.print("Sin Turbo: ");
          Serial.println(Turbo);
        }
      //Disparar
      if(xboxController.xboxNotif.btnX){
        if(in){
          digitalWrite(Entrada_1,HIGH);
          time_2=millis();
          //Delay(300);
          //digitalWrite(Entrada_1,LOW);
          Serial.println("Disparador");
          tiempo_1=millis();
          in=false;
        }
        else if((millis() - tiempo_1) > 1200){
          time_2=millis();
          tiempo_1=millis();
          digitalWrite(Entrada_1,HIGH);
          //Delay(300);
          //digitalWrite(Entrada_1,LOW);
          Serial.println("Disparador");
        }
      }
    } 
  
      
  }
  else {
    Serial.println("not connected");
    if (xboxController.getCountFailedConnection() > 2) {
      ESP.restart();
    }
  }  
}

void CrearPWM(){
  ledcSetup(Canal0,Frecuencia,Resolucion); 
  ledcAttachPin(Valor_PWA,Canal0);
  ledcSetup(Canal2,Frecuencia,Resolucion); 
  ledcAttachPin(Valor_PWB,Canal2);
}

