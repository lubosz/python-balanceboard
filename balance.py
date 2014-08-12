#!/usr/bin/python

from balanceboard import PyBalanceBoard

foo = PyBalanceBoard()
foo.connect()

while foo.hasWiiMotes():
  foo.poll()
  #print(foo.topLeft)
  foo.printSensors()
