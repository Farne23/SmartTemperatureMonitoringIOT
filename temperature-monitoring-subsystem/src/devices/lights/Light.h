#ifndef __LIGHT__
#define __LIGHT__

/* Interface used to rappresent a simple light*/
class Light {
public:
  virtual void switchOn() = 0;
  virtual void switchOff() = 0;    
};

#endif
