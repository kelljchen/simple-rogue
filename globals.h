#ifndef GLOBALS
#define GLOBALS

class Coord
{
    public:
        Coord(int rr, int cc) : m_row(rr), m_col(cc) {};
        int r() { return m_row; };
        int c() { return m_col; };
        void setPos(int rr, int cc) {
            m_row = rr;
            m_col = cc;
        };
    private:
        int m_row;
        int m_col;
};

class Room
{
    public:
        Room(int sr, int sc, int r, int c) : topLeft(new Coord(sr, sc)), numRows(r), numCols(c) {
            center = new Coord(startPt()->r() + (numRows / 2), startPt()->c() + (numCols / 2));        
        };
        ~Room() {
            delete topLeft;
            delete center;
        }
        Coord* startPt() { return topLeft; };
        Coord* getCenter() { return center; };
        int r() const { return numRows; };
        int c() const { return numCols; };

    private:
        Coord* topLeft;
        Coord* center;
        int numRows;
        int numCols;
};

#endif