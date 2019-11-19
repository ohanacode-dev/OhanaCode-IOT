#ifndef EMAIL_SENDER
#define EMAIL_SENDER

typedef struct{
  int port;
  String smtp_server;
  String login;
  String passwd; 
  String to;
  String subject;
}smtp_setup;

extern void EMAIL_init(void);
extern void EMAIL_saveSetup(void);
extern void EMAIL_config(smtp_setup *cfgData);
extern bool EMAIL_send(String message);
extern String EMAIL_getError(void);
extern void EMAIL_getSmtpSetup(smtp_setup *result);

#endif // EMAIL_SENDER
