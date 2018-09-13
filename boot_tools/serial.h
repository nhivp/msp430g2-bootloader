#ifndef SERIAL_H
#define SERIAL_H

#define OPEN_FAIL       0
#define OPEN_SUCCESS    1

unsigned char openSerial(const char * name);
void closeSerial();
void serialReadChar(char *ch);
void serialSendChar(char ch);
void serialSendStr(char *str,unsigned int length);

#endif // SERIAL_H
