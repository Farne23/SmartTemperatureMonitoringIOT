#include <Arduino.h>
#include "tasks/WindowRegTask.h"
#include "tasks/ControlPanelTask.h"
#include "controller/WindowController.h"
#include "scheduler/Scheduler.h"

/**
 * Assignment #3 for Embedded systems and IOT
 * Authors:
 * Casadei Samuele
 * Farneti Michele
 */

Scheduler scheduler;
WindowController *controller;

void setup()
{
  Serial.begin(9600);

  // Container intitialization
  controller = new WindowController();
  // Scheduler initialization
  scheduler.init(50);

  // Window regulation task initialization
  Task *regTask = new WindowRegTask(controller);
  regTask->init(100);

  // Control panel task initialization
  Task *controlTask = new ControlPanelTask(controller);
  controlTask->init(50);

  // Adding tasks to the scheduler
  scheduler.addTask(regTask);
  scheduler.addTask(controlTask);
}

void loop()
{
  scheduler.schedule();
}