#!/usr/bin/python

from balanceboard import PyBalanceBoard

foo = PyBalanceBoard()

while foo.hasWiiMotes():
  foo.poll();
  foo.printSensors();
