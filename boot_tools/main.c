#include <stdio.h>
#include <string.h>
#include "serial.h"
#include <unistd.h>

typedef unsigned int uint;
typedef unsigned char uchar;

int main(void)
{
    char buffer[10] = {0};

    printf("Please Enter Serial Port. For example: /dev/ttyACM0\n");
    scanf("%s", buffer);
    if(openSerial(buffer) == OPEN_FAIL)
    {
        printf("Open Serial Port Fail!\n");
        return 0;
    }
    else
    {
        printf("Open Serial Port ==> OK!\n");
    }

    char fileName[100];
    char fileBuff[50];

    printf("Please Enter Hex File Path:\n");
    scanf("%s", fileName);

    FILE *file = fopen(fileName,"r");
    if(file == 0)
    {
        printf("File Open Error!\n");
        return 0;
    }
    else
    {
        printf("Open file ==> OK!\n");
    }

    char flag;
    while(!feof(file))
    {
        memset(fileBuff,0,sizeof(fileBuff));
        fgets(fileBuff,sizeof(fileBuff),file);
        uint length = strlen(fileBuff);
        if(length > 0 && fileBuff[length - 1] == '\n')
        {
            length -= 1;
        }

        if(length > 0)
        {
            printf("%s", fileBuff);
            serialSendChar('s');
            serialSendChar((char)length);
            serialSendStr(fileBuff,length);

            serialReadChar(&flag);
            if(flag == 'n')
            {
                printf("Receive OK\n");
            }
            else
            {
                printf("Unknown Error!\n Flash Failed!\n");
                break;
            }
        }
    }

    serialSendChar('f');
    serialReadChar(&flag);
    if(flag == 'n')
    {
        printf("Download Success!\n");
    }

    sleep(1);
    printf("Reboot MSP430\n");

    serialSendStr("r\r",2);
    serialReadChar(&flag);
    if(flag == 'n')
    {
        printf("Reboot Success!\n");
    }

    closeSerial();

    return 0;
}
