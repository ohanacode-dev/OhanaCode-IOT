#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H

#include <WebSocketsServer.h>
#include <WebSocketsClient.h>

extern void WS_process(void);
extern void WS_init(void);
extern void WS_broadcast(String msg);

#endif
