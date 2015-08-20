#ifndef PILE_H
#define PILE_H
#include <QLabel>
#include <QWidget>


class Card;
class TreeCard;
class Rule;

enum pileType{TABLEAU, STOCK, WASTE, FOUNDATION, FREECELL, PYRAMID};
const int DRAGSIZE = 6;
const int DROPSIZE = 6;

class TreePile : public QLabel
{
protected:
    QPoint delta;
    Card *bottom;
    Rule **drag;
    Rule **drop;
    static Rule *TableauDrop[], *TableauDrag[];
    static Rule *StockDrop[], *StockDrag[];
    static Rule *WasteDrop[], *WasteDrag[];
    static Rule *FoundationDrop[], *FoundationDrag[];
    static Rule *FreeCellDrop[], *FreeCellDrag[];
    static Rule *PyramidDrop[], *PyramidDrag[];
public:

    virtual Card* Top() {return NULL;}
    TreePile(int x, int y, int dx, int dy, Rule **Drag, Rule **Drop, QWidget *parent);
    ~TreePile();
    Card *Bottom(){return bottom;}
    void AddDropRules(int n ...);
    void AddDragRules(int n ...);
    bool checkDragRules(Card *c);
    bool checkDropRules(Card *c);
    bool checkDropRules(Card *c1, Card *c2);
    QPoint Delta() { return delta;}
    Rule *DragRule(int i);
    void DragRule(int i, Rule *r);
    Rule *DropRule(int i);
    void DropRule(int i, Rule *r);
    virtual pileType Type()=0;
    static void ClearRules();
    virtual void Clicked(Card *c);
    virtual void acceptCard(Card *c, bool flip=true, bool forceBase=false, bool recordUndo = true);
    virtual uint getMDistance(QPoint p);
    TreeCard *FindPos(int pos);
    virtual void Unlink(Card *c, bool flip, bool recordUndo);
    friend class Pile;
};


class Pile : public TreePile
{
protected:

    Card *top;



public:
    Pile(int x, int y, int dx, int dy, Rule **Drag, Rule **Drop, QWidget *parent);
    ~Pile();
    Card *Top(){return top;}
    void NullTop() { top = NULL;}
    void acceptCard(Card *c, bool flip=true, bool forceBase=false, bool recordUndo = true);
    uint getMDistance(QPoint p);
    void Clicked(Card *c);
    void mouseReleaseEvent(QMouseEvent *event);
    void Unlink(Card *c, bool flip, bool recordUndo);
    QPoint tops();
    pileType Type()=0;


};

//---------------------------------------------------------

class StockPile : public Pile
{
    static Rule *dragRules[DRAGSIZE];
    static Rule *dropRules[DROPSIZE];

public:
    StockPile(int x, int y, int dx, int dy, QWidget *parent);
    pileType Type() {return STOCK;}
    void Clicked(Card *c);


};

//---------------------------------------------------------

class FoundationPile : public Pile
{
public:
    FoundationPile(int x, int y, int dx, int dy, QWidget *parent);
    pileType Type() {return FOUNDATION;}
    void Clicked(Card *c){}
};

//---------------------------------------------------------

class TableauPile : public Pile
{
public:
    TableauPile(int x, int y, int dx, int dy, QWidget *parent);
    pileType Type() {return TABLEAU;}
    void Clicked(Card *c);
};

//---------------------------------------------------------

class WastePile : public Pile
{
public:
    WastePile(int x, int y, int dx, int dy, QWidget *parent);
    pileType Type() {return WASTE;}
};

//---------------------------------------------------------

class FreeCellPile : public Pile
{
public:
    FreeCellPile(int x, int y, int dx, int dy, QWidget *parent);
    pileType Type() {return FREECELL;}
    void Clicked(Card *c);
};

//---------------------------------------------------------

class PyramidPile : public TreePile
{
public:
    PyramidPile(int x, int y, int dx, int dy, QWidget *parent);
    pileType Type() {return PYRAMID;}
    void Clicked(Card *c);
};


#endif // PILE_H
