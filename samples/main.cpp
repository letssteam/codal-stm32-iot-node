
#include "STM32IotNode.h"

using namespace codal;
STM32IotNode IotNode;

int main()
{
    IotNode.init();
    IotNode.serial.printf("*** STM32_IOT_NODE BLINKY TEST ***\r\n");

    while (1)
    {
        IotNode.io.led.setDigitalValue(1);
        IotNode.sleep(2000);
        IotNode.io.led.setDigitalValue(0);
        IotNode.sleep(1000);
    }
}
