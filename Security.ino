#include <ESP8266WiFi.h>                                                
#include <ESP8266WebServer.h>                                           
#include <FS.h>                                                         
#include <ESP8266FtpServer.h>                                          

const byte security_active = 1;                                                   
const byte look_active = 13;
const byte look_noactive = 2;
const char *ssid = "VAZ-2103";                                         
const char *passPhrase = "DGYvaz03091995";

ESP8266WebServer HTTP(80);                                              
FtpServer ftpSrv;                                                      

void setup() {
   Serial.begin(9600);
  pinMode(security_active,OUTPUT);                                                                                                 
  pinMode(look_active,OUTPUT);
  pinMode(look_noactive,OUTPUT);
  WiFi.softAP(ssid, passPhrase);                                        
  digitalWrite(security_active, LOW);
  digitalWrite(look_active, HIGH);
  digitalWrite(look_noactive, HIGH);
  
  SPIFFS.begin();                                                                               
  HTTP.begin();                                                         
  ftpSrv.begin("relay","relay");                                        

  Serial.print("\nMy IP to connect via Web-Browser or FTP: ");          
  Serial.println(WiFi.softAPIP());                                      
  Serial.println("\n");

// Обработка HTTP-запросов
  HTTP.on("/security_switch_active", [](){                                        
      HTTP.send(200, "text/plain", security_switch_active());                     
  });
  HTTP.on("/security_switch_noactive", [](){                                       
      HTTP.send(200, "text/plain", security_switch_noactive());                    
  });
  HTTP.on("/look_on", [](){                                        
      HTTP.send(200, "text/plain", look_on());                      
  });
   HTTP.on("/look_off", [](){                                        
      HTTP.send(200, "text/plain", look_off());                     
  });
  HTTP.onNotFound([](){                                                 
  if(!handleFileRead(HTTP.uri()))                                       
      HTTP.send(404, "text/plain", "Not Found");                       
  });
}

void loop() {
    HTTP.handleClient();                                                
    ftpSrv.handleFTP();                                                 


String security_switch_active() {                                                 
byte security_state;                                                   
 digitalWrite(security_active, HIGH);                                          
 digitalWrite(look_active, LOW);
 delay(250);
 digitalWrite(look_active, HIGH);
 return String(security_state);                                                
}
String security_switch_noactive() {                                                 
byte security_state;                                                   
 digitalWrite(security_active, LOW);                                          
 digitalWrite(look_noactive, LOW);
 delay(250);
 digitalWrite(look_noactive, HIGH);
 return String(security_state);                                                
}
String look_on() {                                                 
byte security_state;                                                    
 digitalWrite(security_active, HIGH);                                          
 digitalWrite(look_active, LOW);
 delay(250);
 digitalWrite(look_active, HIGH);
 return String(security_state);                                         
}
String look_off() {                                                 
byte security_state;                                                   
 digitalWrite(security_active, LOW);                                         
 digitalWrite(look_noactive, LOW);
 delay(250);
 digitalWrite(look_noactive, HIGH);
 return String(security_state);                                                 
}




bool handleFileRead(String path){                                      
  if(path.endsWith("/")) path += "index.html";                          
  String contentType = getContentType(path);                            
  if(SPIFFS.exists(path)){                                              
    File file = SPIFFS.open(path, "r");                                 
    size_t sent = HTTP.streamFile(file, contentType);                   
    file.close();                                                       
    return true;                                                        
  }
  return false;                                                         
}

String getContentType(String filename){                                 
  if (filename.endsWith(".html")) return "text/html";                   
  else if (filename.endsWith(".css")) return "text/css";                
  else if (filename.endsWith(".js")) return "application/javascript";   
  else if (filename.endsWith(".png")) return "image/png";               
  else if (filename.endsWith(".jpg")) return "image/jpeg";              
  else if (filename.endsWith(".gif")) return "image/gif";               
  else if (filename.endsWith(".ico")) return "image/x-icon";            
  return "text/plain";                                                   
}
