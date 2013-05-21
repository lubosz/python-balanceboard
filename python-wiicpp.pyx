cdef extern from "BalanceBoard.h":
    cdef cppclass BalanceBoard:
        BalanceBoard()
        int getLength()
        int getHeight()
        int getArea()
        void move(int, int)
 
cdef class PyBalanceBoard:
    cdef BalanceBoard *thisptr      # hold a C++ instance which we're wrapping
    def __cinit__(self):
        self.thisptr = new BalanceBoard()
    def __dealloc__(self):
        del self.thisptr
    def getLength(self):
        return self.thisptr.getLength()
    def getHeight(self):
        return self.thisptr.getHeight()
    def getArea(self):
        return self.thisptr.getArea()
    def move(self, dx, dy):
        self.thisptr.move(dx, dy)
