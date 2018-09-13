#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
#include "serial.h"

char* _cl_port = NULL;
int _fd = -1;

unsigned char openSerial(const char * name)
{
    struct termios serial_configs;

    _cl_port = strdup(name);

    /* open the port */
    _fd = open(_cl_port, O_RDWR | O_NOCTTY | O_NDELAY);

    if (_fd == -1)
    {
        /*
         * Could not open the port.
         */
        printf("Error Unable to open %s\n", name);
        free(_cl_port);
        exit(1);
    }
    else
    {
        fcntl(_fd, F_SETFL, 0);
    }

    /* Configuring the serial port */
    serial_configs.c_cflag = B9600 | CS8 | CLOCAL | CREAD; /* Baudrate = 9600 */

    /* No parity (8N1) */
    serial_configs.c_cflag &= ~PARENB;
    serial_configs.c_cflag &= ~CSTOPB;
    serial_configs.c_cflag &= ~CSIZE;
    serial_configs.c_cflag |= CS8;

    serial_configs.c_iflag = 0; /* Input options */
    serial_configs.c_oflag = 0; /* Output options */
    serial_configs.c_lflag = 0; /* Line options */

    /* Control characters */
    serial_configs.c_cc[VMIN] = 0;   /* The VTIME value specifies the time to wait for every character read. */
    serial_configs.c_cc[VTIME] = 5;  /* 0.5 seconds read timeout */

    /* now clean the modem line and activate the settings for the port */
    tcflush(_fd, TCIOFLUSH);
    tcsetattr(_fd, TCSANOW, &serial_configs);

    return 1;
}

void closeSerial(void)
{
    close(_fd);
}

void serialSendChar(char ch)
{
    ssize_t r;

    r = write(_fd, &ch, 1);

    if (r < 0)
    {
        printf("Error: write() unknown!\n");
    }
}

void serialSendStr(char *str,unsigned int length)
{
    ssize_t r;
    const char *pos = (const char *)str;

    while (length)
    {
        r = write(_fd, pos, length);

        if (r < 0)
        {
            printf("Error: write() unknown!\n");
        }

        length -= r;
        pos += r;
    }
}

void serialReadChar(char *ch)
{
    ssize_t r;

    r = read(_fd, ch, 1);

    if (r == 0)
    {
        printf("Error: read() timeout!\n");
    }
    else if (r < 1)
    {
        printf("Error: read() unknown!\n");
    }
    else
    {
        (void)r;
    }
}

