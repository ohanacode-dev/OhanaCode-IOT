#ifndef STEP_M_DRV_H
#define STEP_M_DRV_H

void STMDRV_init(void);
void STMDRV_tick(uint8_t sec);
uint8_t STMDRV_getTickCount(void);
void STMDRV_forceTick(void);

#endif
