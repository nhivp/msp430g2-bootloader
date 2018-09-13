#include "bootloader.h"
#include "uart.h"
#include "flash.h"
#include "utils.h"

char *menuStr = "------------MSP430 BOOT MENU------------\r\n"
        " (D):Download Program From Serial\r\n"
        " (E):Erase Flash\r\n"
        " (R):Reboot\r\n";

char *commandStr = "command:";
char *commandErrorStr = "command error!\r\n";

typedef int (*ApplicationMainFunc)();

const ApplicationMainFunc * const appMainFunc = (const ApplicationMainFunc * const ) 0xCA1E;

unsigned int __attribute__((section(".mode_flag"))) isBootMode;

unsigned char currentState;
unsigned int count;
unsigned int preCount;

static void initSystemClock(void);
static void initTimer(void);
static void disableTimer(void);
static char checkApplication(void);
static void eraseApplication(void);
static void downloadProgram(void);
static void reboot(void);

void bootloader_main()
{
    char command[5] = { 0 };
    isBootMode = 1;
    initSystemClock();
    initTimer();
    UART_Initialize();
    FLASH_Initialize();
    _EINT();

    currentState = START_STATE;
    count = 0;
    preCount = count;
    UART_SendString("Press Any Key to Interrupt Boot!\r\n");

    while (1)
    {
        switch (currentState)
        {
            case START_STATE:
                if (count - preCount < 6 && (IFG2 & UCA0RXIFG))
                {
                    UART_ReadChar((char *) 0x00);
                    currentState = MENU_STATE;
                    UART_SendString(menuStr);
                    UART_SendString(commandStr);
                    disableTimer();
                    _DINT();
                }
                else if (count - preCount > 6)
                {
                    currentState = START_APPLICATION_STATE;
                    disableTimer();
                    _DINT();
                }
                break;

            case MENU_STATE:
                UART_ReadString(command, 5);
                if (command[1] != '\0')
                {
                    UART_SendString(commandErrorStr);
                    UART_SendString(commandStr);
                }
                else
                {
                    switch (command[0])
                    {
                        case 'D':
                        case 'd':
                            downloadProgram();
                            UART_SendString(commandStr);
                            break;
                        case 'E':
                        case 'e':
                            eraseApplication();
                            UART_SendString(commandStr);
                            break;
                        case 'R':
                        case 'r':
                            __delay_cycles(500000);
                            reboot();
                            break;
                        default:
                            UART_SendString(commandErrorStr);
                            UART_SendString(commandStr);
                            break;
                    }
                }
                break;

            case START_APPLICATION_STATE:
                if (checkApplication() == 1)
                {
                    currentState = END_STATE;
                    isBootMode = 0;
                    (*appMainFunc)();
                } else {
                    currentState = MENU_STATE;
                    UART_SendString("Boot Error, No Application Found!\r\n");
                    UART_SendString(menuStr);
                    UART_SendString(commandStr);
                }
                break;
        }
    }
}

static void initSystemClock(void)
{
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
}

static void initTimer(void)
{
    TA0CTL |= 0x01 << 2;
    TA0CTL = (0x02 << 8) + (0x03 << 6) + (0x01 << 4); //125000
    TA0CCTL0 |= 0x01 << 4;
    TA0CCR0 = 62500; //0.5s
}

static void disableTimer(void)
{
    TA0CTL |= 0x01 << 2;
    TA0CTL &= ~(0x03 << 4);
    TA0CCTL0 &= ~(0x01 << 4);
}

static char checkApplication(void)
{
    if ((unsigned int) (*appMainFunc) == 0xFFFF)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static void eraseApplication(void)
{
    unsigned char *ptr;

    _DINT();
    UART_SendString("Erase Start\r\n");

    for (ptr = APP_SEGMENT_START; ptr <= APP_SEGMENT_END; ptr += 0x200)
    {
        FLASH_EraseSingleSegments(ptr);
        UART_SendChar('.');
    }

    UART_SendString("\r\nErase Complete!\r\n");
}

static void downloadProgram(void)
{
    ihex_format_t format;
    unsigned int index;
    char buffer[43];
    char command = 0;
    char length;
    char temp;

    unsigned char *ptr;
    UART_SendChar('n');

    while (1)
    {
        UART_ReadChar(&command);
        switch (command)
        {
            case 's':
                UART_ReadChar(&length);

                /*
                 * Get length of data transfer from flasher
                 */
                for (index = 0; index < length; index++)
                {
                    UART_ReadChar(&temp);
                    buffer[index] = temp;
                }

                /*
                 * Parse the Intel Hex format
                 */
                if (!hexStringToBin(buffer, &format))
                {
                    UART_SendChar('e');
                    return;
                }

                /*
                 * Checking the address of application
                 */
                if (((unsigned char *) (format.address)) < APP_ROM_START
                        || ((unsigned char *) (format.address)) > APP_ROM_END)
                {
                    UART_SendChar('e');
                    return;
                }

                /*
                 * Type 0x00 ==> Data record
                 * Just write data record to flash
                 */
                if (format.type == 0)
                {
                    for (index = 0; index < format.length; index++)
                    {
                        ptr = ((unsigned char *) (format.address)) + index;
                        FLASH_WriteByte(ptr, format.data[index]);
                    }
                }

                UART_SendChar('n');
                break;

            case 'f':
                UART_SendChar('n');
                return;
        }
    }
}

static void reboot(void)
{
    /* Write a wrong value to force reset (PUC is generated) */
    WDTCTL = 0x00;
}
