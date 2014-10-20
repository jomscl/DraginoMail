/*
Configure Email account
In SSH access, install sSmtp package by running:
    opkg update
    opkg install ssmtp
    opkg install mutt

We need to edit, /etc/ssmtp/ssmtp.conf:
    root=username@gmail.com
    mailhub=smtp.gmail.com:587
    rewriteDomain=
    hostname=username@gmail.com
    UseSTARTTLS=YES
    AuthUser=username
    AuthPass=password
    FromLineOverride=YES

Then add each account that you want to be able to send mail from by editing, /etc/ssmtp/revaliases:
    root:username@gmail.com:smtp.gmail.com:587

To test if the email is configured properly, run below command:
echo "a test mail" | mutt -s "Test Mail" -a /etc/config/wireless -- you@yourmail.com

// pines usados del dragino
1+ 5v
1- gnd
2+ arduino 3, tierra=alarma activa

*/
#include <Process.h>
#include <Metro.h>

#define led 13
#define alarma 3
#define latencia 20 // valor * 500ms: tiempo entre envíos de correo, aunque la alarma siga sonando
String email="jom@jom.cl";
String locacion="Casa EDR";

Metro beat = Metro(500); 

int delta=0;

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Console.begin(); 
  Console.println("Encendido");
  pinMode(led,OUTPUT);
  digitalWrite(led, HIGH);
  pinMode(alarma,INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (beat.check()==1){
    digitalWrite(led, HIGH);
    delay(50);    
    digitalWrite(led, LOW);
    
    // revision del puerto
    if (delta==0){
      if (leePuerto(alarma)==true){
        // alarma sonando
        delta=latencia;
        Console.println("Alarma");
        mandaMail();
      }
    }
    else{delta--;}
  }
}

boolean leePuerto(int pin){
  return !digitalRead(pin);  
}

void mandaMail(){
  String salida="";
  salida="echo \"Alarma Draguino\" | mutt ";
  salida +="-s \"Alarma ";
  salida += locacion; 
  salida +=getTimeStamp();
  salida +="\" ";
  salida +=email;
  //salida +="\"";
  Console.println(salida);
  Process p;
  //p.runShellCommand("echo \"a test mail\" | mutt -s \"Test Alarma\" jom@jom.cl");
  p.runShellCommand(salida);
}

// This function return a string with the time stamp
String getTimeStamp() {
  String result=" ";
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin("date");
  time.addParameter("+%d/%m/%Y %T");  // parameters: D for the complete date mm/dd/yy
                                //             T for the time hh:mm:ss    
  time.run();  // run the command

  // read the output of the command
  while(time.available()>0) {
    char c = time.read();
    if(c != '\n')
      result += c;
  }

  return result;
}
