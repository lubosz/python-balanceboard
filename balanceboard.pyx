from libcpp cimport bool

cdef extern from "BalanceBoard.h":
    cdef cppclass BalanceBoard:
        BalanceBoard()
        
        float total, topLeft, topRight, bottomLeft, bottomRight
        bool isConnected

        void poll()
        void printSensors()
        void inputLoop()
        void connect()

        float getTotal()
        
        bool hasWiiMotes()

cdef class PyBalanceBoard:
    cdef BalanceBoard* thisptr      # hold a C++ instance which we're wrapping
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

    def connect(self):
        self.thisptr.connect()

    property isConnected:
      def __get__(self): return self.thisptr.isConnected

    property topLeft:
      def __get__(self): return self.thisptr.topLeft

    property topRight:
      def __get__(self): return self.thisptr.topRight

    property bottomLeft:
      def __get__(self): return self.thisptr.bottomLeft

    property bottomRight:
      def __get__(self): return self.thisptr.bottomRight
