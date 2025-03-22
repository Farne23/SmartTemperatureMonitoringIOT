#ifndef __BUTTONPANEL__
#define __BUTTONPANEL__

#include "Button.h"

class ButtonPanel {
 
public: 
  ButtonPanel(Button* openButton, Button* closeButton);
  bool closeRequested();
  bool openRequested();

private:
    Button* openButton;
    Button* closeButton;
    bool actionRequested(Button* button);
};

#endif