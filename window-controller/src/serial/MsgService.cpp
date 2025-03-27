#include "Arduino.h"
#include "MsgService.h"

String content;

MsgServiceClass MsgService;

bool MsgServiceClass::isMsgAvailable()
{
  return msgAvailable;
}

Msg *MsgServiceClass::receiveMsg()
{
  if (msgAvailable)
  {
    Msg *msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;
  }
  else
  {
    return NULL;
  }
}

void MsgServiceClass::init()
{
  Serial.begin(9600);
  content.reserve(10);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;
}

/* wrapper per mandare il messaggio direttamente con la stringa */
void MsgServiceClass::sendMsg(const String &msg)
{
  Serial.println(msg);
}

void serialEvent()
{
  while (Serial.available())
  {
    char ch = (char)Serial.read();
    if (ch == '\n')
    {
      MsgService.currentMsg = new Msg(content);
      MsgService.msgAvailable = true;
    }
    else
    {
      content += ch;
    }
  }
}

/* controlla e c'è un messaggio sulla linea seriale */
bool MsgServiceClass::isMsgAvailable(Pattern &pattern)
{
  return (msgAvailable && pattern.match(*currentMsg));
}

/* fetcha il messaggio dalla linea seriale
N.B. è mia responsabilità cancellare il messaggio che ricevo
*/
Msg *MsgServiceClass::receiveMsg(Pattern &pattern)
{
  if (msgAvailable && pattern.match(*currentMsg))
  {
    Msg *msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;
  }
  else
  {
    return NULL;
  }
}
