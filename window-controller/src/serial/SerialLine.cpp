#include "SerialLine.h"

SerialLine::SerialLine()
{
    MsgService.init();
}

void SerialLine::sendData(double openPerc)
{
    MsgService.sendMsg(String(openPerc));
}

void SerialLine::getData()
{
    if (MsgService.isMsgAvailable())
    {
        Msg *msg = MsgService.receiveMsg();
        String content = msg->getContent();
        char text[sizeof(content)];
        strcpy(text, content.c_str());
        if (text[0] == 'M')
        {
            this->changeStatus = true;
        }
        if (text[0] == 'T')
        {
            char* endptr = nullptr;
            this->temp = strtod(content.substring(1).c_str(), &endptr);
        }
        if (text[0] = 'P')
        {
            char* endptr = nullptr;
            this->angle = strtod(content.substring(1).c_str(), &endptr);
        }
    }
}

bool SerialLine::getChangeStatus()
{
    return this->changeStatus;
}

double SerialLine::getTemp()
{
    return this->temp;
}

double SerialLine::getAngle()
{
    return this->angle;
}

