#ifndef _LED_H_
#define _LED_H_

int ledInit(void);
int ledOnOff(int ledNum, int OnOff);
int ledOnAll(void);
int ledOffAll(void);
int ledExit(void);
int battery(int batNum);

#endif
