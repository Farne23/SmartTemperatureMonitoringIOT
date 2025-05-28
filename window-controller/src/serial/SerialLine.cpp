#include "SerialLine.h"
#include "settings/DomainDefaults.h"

SerialLine::SerialLine()
{
    MsgService.init();
    // if no message is available, then every value is default.
    changeMode = DEF_MODE;
    perc = DEF_PERC;
    temp = DEF_TEMP;
}

void SerialLine::sendData(String msg)
{
    MsgService.sendMsg(msg);
}

void SerialLine::getData()
{
    // variable definition to avoid errors
    char* endptr = nullptr;

    while (MsgService.isMsgAvailable())
    {
        Msg *msg = MsgService.receiveMsg();
        String content = msg->getContent();
        delete msg;

        if (content.length() > 0)
        {
            char firstChar = content.charAt(0);
            switch (firstChar) {
                case 'M':
                    changeMode = firstChar == 'M' ? changeMode = content.charAt(1) : DEF_MODE;
                    break;
                case 'T':
                    // replacing "," with ".".
                    content.replace(",", ".");
                    this->temp = strtod(content.substring(1).c_str(), &endptr);
                    break;
                case 'P':
                    long val = strtol(content.substring(1).c_str(), &endptr, 10);
                    this->perc = (double)val / 100.0;
                    break;
            };
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