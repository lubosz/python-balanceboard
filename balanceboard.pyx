from libcpp cimport bool

cdef extern from "BalanceBoard.h":
    cdef cppclass BalanceBoard:
        BalanceBoard()
        
        float total, topLeft, topRight, bottomLeft, bottomRight

        void poll()
        void printSensors()
        void inputLoop()

        float getTotal()
        
        bool hasWiiMotes()
 
cdef class PyBalanceBoard:
    cdef BalanceBoard *thisptr      # hold a C++ instance which we're wrapping
    def __cinit__(self):
        self.thisptr = new BalanceBoard()
    def __dealloc__(self):
        del self.thisptr
        
    def getTotal(self):
        return self.thisptr.getTotal()
        
    def hasWiiMotes(self):
        return self.thisptr.hasWiiMotes()
        
    def printSensors(self):
        self.thisptr.printSensors()
    def poll(self):
        self.thisptr.poll()
        
    def inputLoop(self):
        self.thisptr.inputLoop()
        
        
    property topLeft:
      def __get__(self): return self.thisptr.topLeft
      def __set__(self, x0): self.thisptr.topLeft = x0
