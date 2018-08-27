#include "user_config.h"

#define NETWORK_TEST "Network test"
#define HELLO_WORLD_TEST "HELLO WORLD TEST"
#define TIME_STRING "Display current time"

MenuStruct mainMenu;
static OledStringStruct helloWorldTest;
static OledStringStruct networkingTest;
static OledStringStruct timeTest;


void ICACHE_FLASH_ATTR
initFunction()
{
    i2c_init();
    OledFunctions_init();
    OledFunctions_eraseScreen();
    OledFunctions_returnCursor();
    InterruptConfig_setupInterrupt();
    Network_SetupNetwork();
}

void ICACHE_FLASH_ATTR
helloworld_function(void)
{
    OledFunctions_eraseScreen();
    OledFunctions_returnCursor();
    static OledStringStruct * helloWorld;
    helloWorld = (OledStringStruct *)os_zalloc(sizeof(OledStringPtr));
    helloWorld->len = (OledStringLen)os_strlen("Hello World\n");
    helloWorld->ptr = (OledStringPtr *)os_zalloc(helloWorld->len);
    OledFunctions_stringToOledString("Hello World\n", helloWorld->ptr);
    uint8 counter = 0;
    for(counter = 0; counter < 8; counter++)
    {
        OledFunctions_writeString(helloWorld);
    }
    os_free(helloWorld->ptr);
    os_free(helloWorld);
}

void ICACHE_FLASH_ATTR
networking_function(void)
{
    OledFunctions_eraseScreen();
    OledFunctions_returnCursor();
    Network_connectToServer();
}

void ICACHE_FLASH_ATTR
time_function(void)
{
    static OledStringStruct * tempTimeString;
    tempTimeString = (OledStringStruct *)os_zalloc(sizeof(OledStringStruct));
    tempTimeString->len = (uint8)os_strlen(sntp_get_real_time(realTime));
    tempTimeString->ptr = (OledStringPtr *)os_zalloc(tempTimeString->len);
    OledFunctions_stringToOledString(sntp_get_real_time(realTime), tempTimeString->ptr);
    OledFunctions_eraseScreen();
    OledFunctions_returnCursor();
    OledFunctions_writeString(tempTimeString);
    os_free(tempTimeString->ptr);
    os_free(tempTimeString);

}

void user_init(void)
{
    system_uart_swap();
    initFunction();

    mainMenu.currentElement = 0;
    mainMenu.totalElements = 0;
    //for default menu behavior
    Interface_setNextPointer(Interface_defaultNext);
    Interface_setSelectPointer(Interface_defaultSelect);

    //sets the menu item strings
    helloWorldTest.len = (OledStringLen)os_strlen(HELLO_WORLD_TEST);
    helloWorldTest.ptr = (OledStringPtr *)os_zalloc(helloWorldTest.len);
    OledFunctions_stringToOledString(HELLO_WORLD_TEST, helloWorldTest.ptr);
    mainMenu.totalElements++;

    networkingTest.len = (OledStringLen)os_strlen(NETWORK_TEST);
    networkingTest.ptr = (OledStringPtr *)os_zalloc(networkingTest.len);
    OledFunctions_stringToOledString(NETWORK_TEST, networkingTest.ptr);
    mainMenu.totalElements++;

    timeTest.len = (OledStringLen)os_strlen(TIME_STRING);
    timeTest.ptr = (OledStringPtr *)os_zalloc(timeTest.len);
    OledFunctions_stringToOledString(TIME_STRING, timeTest.ptr);
    mainMenu.totalElements++;

    OledFunctions_optionSet(&mainMenu,0, helloworld_function, &helloWorldTest); //setting all the parameters for the main menu
    OledFunctions_optionSet(&mainMenu,1, networking_function, &networkingTest);
    OledFunctions_optionSet(&mainMenu,2, time_function, &timeTest);
    OledFunctions_setGlobalMenu(&mainMenu);
}
