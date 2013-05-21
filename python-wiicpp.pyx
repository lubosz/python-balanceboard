cdef extern from "python-wiicpp.cpp":
    cdef cppclass BalanceBoard:
        BalanceBoard() except +
        int x0, y0, x1, y1
        int getLength()
        int getHeight()
        int getArea()
        void move(int, int)
