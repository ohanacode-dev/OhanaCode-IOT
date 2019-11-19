#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <base64.h>
#include "config.h"
#include "email_sender.h"

//#define SERIAL_LOG        // Print to Serial only server responce

static WiFiClientSecure client;
static String server_err;
static String serverResponce;
static int smtpPort;
static String smtpServer;
static String smtpLogin;
static String smtpPasswd; 
static String smtpTo;
static String smtpSubject;

static String base64Encode(String strToEncode){
  base64 b;
  return b.encode(strToEncode);
}

static bool AwaitSMTPResponse(const String &resp = "")
{
  uint32_t ts = millis();
  while (!client.available())
  {
    if(millis() > (ts + SMTP_TIMEOUT)) {
      server_err = "SMTP Response TIMEOUT!";
      return false;
    }
  }
  serverResponce = client.readStringUntil('\n');
#if defined(SERIAL_LOG) 
  Serial.println(serverResponce);
#endif
  if (resp && serverResponce.indexOf(resp) == -1) return false;
  return true;
}


bool EMAIL_send(String message)
{
  if(!client.connect(smtpServer, smtpPort)) {
    server_err = "Could not connect to mail server";
    return false;
  }
  if(!AwaitSMTPResponse("220")) {
    server_err = "Connection Error";
    return false;
  }

  client.println("HELO friend");
  if(!AwaitSMTPResponse("250")){
    server_err = "identification error";
    return false;
  }

  client.println("AUTH smtpLogin");
  AwaitSMTPResponse();

  client.println(base64Encode(smtpLogin));
  AwaitSMTPResponse();

  client.println(base64Encode(smtpPasswd));
  if (!AwaitSMTPResponse("235")) {
    server_err = "SMTP AUTH error";
    return false;
  }
  
  String mailFrom = "MAIL FROM: <" + String(smtpLogin) + '>';

  client.println(mailFrom);
  AwaitSMTPResponse();

  String rcpt = "RCPT TO: <" + smtpTo + '>';

  client.println(rcpt);
  AwaitSMTPResponse();

  client.println("DATA");
  if(!AwaitSMTPResponse("354")) {
    server_err = "SMTP DATA error";
    return false;
  }
  
  client.println("From: <" + String(smtpLogin) + '>');
  client.println("To: <" + smtpTo + '>');
  
  client.print("Subject: ");
  client.println(smtpSubject);
  
  client.println("Mime-Version: 1.0");
  client.println("Content-Type: text/html; charset=\"UTF-8\"");
  client.println("Content-Transfer-Encoding: 7bit");
  client.println();
  String body = "<!DOCTYPE html><html lang=\"en\">" + message + "</html>";
  client.println(body);
  client.println(".");
  if (!AwaitSMTPResponse("250")) {
    server_err = "Sending message error";
    return false;
  }
  client.println("QUIT");
  if (!AwaitSMTPResponse("221")) {
    server_err = "SMTP QUIT error";
    return false;
  }
  return true;
}

void EMAIL_config(smtp_setup *cfgData){
  smtpPort = cfgData->port;
  smtpServer = cfgData->smtp_server;
  smtpLogin = cfgData->login;
  smtpPasswd = cfgData->passwd; 
  smtpTo = cfgData->to;
  smtpSubject = cfgData->subject;
}

void EMAIL_init(void){
  EEPROM.begin(EEPROM_SIZE);
  uint8_t i = 0;

  do{
    smtpServer[i] = EEPROM.read(i + SMTP_SERVER_EEPROM_ADDR);
    if((smtpServer[i] < 32) || (smtpServer[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < SMTP_SERVER_SIZE);
  smtpServer[i] = 0;

  if((smtpServer.length() < 8) || (smtpServer.indexOf(".") <= 0)){
    smtpServer = SMTP_SERVER;
  }

  i = 0;
  do{
    smtpLogin[i] = EEPROM.read(i + SMTP_LOGIN_EEPROM_ADDR);
    if((smtpLogin[i] < 32) || (smtpLogin[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < SMTP_LOGIN_SIZE);
  smtpLogin[i] = 0;

  i = 0;
  do{
    smtpPasswd[i] = EEPROM.read(i + SMTP_PASS_EEPROM_ADDR);
    if((smtpPasswd[i] < 32) || (smtpPasswd[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < SMTP_PASS_SIZE);
  smtpPasswd[i] = 0;

  i = 0;
  do{
    smtpTo[i] = EEPROM.read(i + SMTP_TO_EEPROM_ADDR);
    if((smtpTo[i] < 32) || (smtpTo[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < SMTP_TO_SIZE);
  smtpTo[i] = 0;

  i = 0;
  do{
    smtpSubject[i] = EEPROM.read(i + SMTP_SUBJECT_EEPROM_ADDR);
    if((smtpSubject[i] < 32) || (smtpSubject[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < SMTP_SUBJECT_SIZE);
  smtpSubject[i] = 0;

  smtpPort = (EEPROM.read(SMTP_PORT_EEPROM_ADDR) << 8) | EEPROM.read(SMTP_PORT_EEPROM_ADDR + 1);
   
  EEPROM.end();   

}

void EMAIL_saveSetup(void){
  EEPROM.begin(EEPROM_SIZE);
  
  uint16_t addr;
  for(addr = 0; addr < smtpServer.length(); addr++){
    EEPROM.write(addr + SMTP_SERVER_EEPROM_ADDR, smtpServer[addr]);
  }
  EEPROM.write(addr + SMTP_SERVER_EEPROM_ADDR, 0);

  for(addr = 0; addr < smtpLogin.length(); addr++){
    EEPROM.write(addr + SMTP_LOGIN_EEPROM_ADDR, smtpLogin[addr]);
  }
  EEPROM.write(addr + SMTP_LOGIN_EEPROM_ADDR, 0);

  for(addr = 0; addr < smtpPasswd.length(); addr++){
    EEPROM.write(addr + SMTP_PASS_EEPROM_ADDR, smtpPasswd[addr]);
  }
  EEPROM.write(addr + SMTP_PASS_EEPROM_ADDR, 0);

  for(addr = 0; addr < smtpTo.length(); addr++){
    EEPROM.write(addr + SMTP_TO_EEPROM_ADDR, smtpTo[addr]);
  }
  EEPROM.write(addr + SMTP_TO_EEPROM_ADDR, 0);

  for(addr = 0; addr < smtpSubject.length(); addr++){
    EEPROM.write(addr + SMTP_SUBJECT_EEPROM_ADDR, smtpSubject[addr]);
  }
  EEPROM.write(addr + SMTP_SUBJECT_EEPROM_ADDR, 0);

  EEPROM.write(SMTP_PORT_EEPROM_ADDR, ((smtpPort >> 8) & 0xFF));
  EEPROM.write(SMTP_PORT_EEPROM_ADDR + 1, (smtpPort & 0xFF));
   
  EEPROM.commit();
  EEPROM.end();
}

String EMAIL_getError(void){
  return server_err;
}

void EMAIL_getSmtpSetup(smtp_setup *result){  
  result->port = smtpPort;
  result->smtp_server = smtpServer;
  result->login = smtpLogin;
  result->passwd = smtpPasswd; 
  result->to = smtpTo;
  result->subject = smtpSubject;
}

