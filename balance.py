#!/usr/bin/python

from balanceboard import PyBalanceBoard

foo = PyBalanceBoard()

while foo.hasWiiMotes():
  foo.poll()
  print(foo.topLeft)
  #foo.printSensors()
