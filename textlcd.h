#ifndef _LCDTEXT_H_
#define _LCDTEXT_H_

#define TEXTLCD_DRIVER_NAME		"/dev/peritextlcd"

int textlcdWrite();
int textlcdInit(int number, char* text);

#endif _LCDTEXT_H_
