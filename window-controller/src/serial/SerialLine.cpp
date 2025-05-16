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
        Serial.println("message received: " + content);
        if (text[0] == 'M')
        {
            // take 'M' or 'A'
            changeMode = content.c_str()[1];
        }
        if (text[0] == 'T')
        {
            char* endptr = nullptr;
            this->temp = strtod(content.substring(1).c_str(), &endptr);
        }
        if (text[0] == 'P')
        {
            char* endptr = nullptr;
            // convert from [100, 0] to [1.0, 0.0]
            this->perc = (double)(strtol(content.substring(1).c_str(), &endptr, 10) / 100);
        }
    }
}

char SerialLine::getChangeStatus()
{
    return this->changeMode;
}

double SerialLine::getTemp()
{
    return this->temp;
}

double SerialLine::getPerc()
{
    return this->perc;
}