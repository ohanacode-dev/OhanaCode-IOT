#ifndef SCHEDULE_H
#define SCHEDULE_H

extern void SCH_init();
extern void SCH_update(uint8_t* source);
extern void SCH_get(uint8_t* dst);
extern void SCH_process();

#endif
