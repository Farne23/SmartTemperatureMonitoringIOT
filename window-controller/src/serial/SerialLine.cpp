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
    // in order to keep track of no value of a certain type is received or not.
    bool tempRcv = false, percRcv = false, modeRcv = false;

    while (MsgService.isMsgAvailable())
    {
        Msg *msg = MsgService.receiveMsg();
        String content = msg->getContent();
        delete msg;

        if (content.length() > 0)
        {
            char firstChar = content.charAt(0);
            // update flags.
            tempRcv = firstChar == 'T';
            percRcv = firstChar == 'P';
            modeRcv = firstChar == 'M';
            switch (firstChar) {
                case 'M':
                    changeMode = modeRcv ? content.charAt(1) : DEF_MODE;
                    // this->changeMode = content.charAt(1);
                    break;
                case 'T':
                    // replacing "," with ".".
                    content.replace(",", ".");
                    this->temp = tempRcv ? strtod(content.substring(1).c_str(), &endptr) : DEF_TEMP;
                    // this->temp = strtod(content.substring(1).c_str(), &endptr);
                    break;
                case 'P':
                    long val = percRcv ? strtol(content.substring(1).c_str(), &endptr, 10) : DEF_PERC;
                    // long val = strtol(content.substring(1).c_str(), &endptr, 10);
                    this->perc = (double)val / 100.0;
                    break;
            };
        }
    }
}

char SerialLine::getChangeStatus()
{
    char tmp = this->changeMode;
    // clean up
    this->changeMode = DEF_MODE;
    return tmp;
}

double SerialLine::getTemp()
{
    return this->temp;
}

double SerialLine::getPerc()
{
    return this->perc;
}

void SerialLine::clearPerc()
{
    this->perc = DEF_PERC;
}