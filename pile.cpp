#include "pile.h"
#include <QLabel>
#include "game.h"
#include "card.h"
#include "rule.h"
#include <QMouseEvent>
#include "move.h"
#include <QWidget>

extern Game *game;

Rule *TreePile::TableauDrop[DROPSIZE];
Rule *TreePile::TableauDrag[DRAGSIZE];
Rule *TreePile::StockDrop[DROPSIZE];
Rule *TreePile::StockDrag[DRAGSIZE];
Rule *TreePile::WasteDrop[DROPSIZE];
Rule *TreePile::WasteDrag[DRAGSIZE];
Rule *TreePile::FoundationDrop[DROPSIZE];
Rule *TreePile::FoundationDrag[DRAGSIZE];
Rule *TreePile::FreeCellDrop[DROPSIZE];
Rule *TreePile::FreeCellDrag[DRAGSIZE];
Rule *TreePile::PyramidDrop[DROPSIZE];
Rule *TreePile::PyramidDrag[DRAGSIZE];


TreePile::TreePile(int x, int y, int dx, int dy, Rule **Drag, Rule **Drop, QWidget *parent):
    QLabel(parent), delta(QPoint(dx,dy)), drag(Drag), drop(Drop), bottom(0)
{
    game->AddPile(this);
    move(x,y);
    resize(CARD_WIDTH,CARD_HEIGHT);
    setFrameShape(Box);
    setLineWidth(2);
    setAutoFillBackground(true);
    show();
}

TreePile::~TreePile()
{
}

bool TreePile::checkDragRules(Card *c)
{
    Rule *r = drag[0];
    int i = 1;
    bool cont = true;
    while (r && cont)
    {
        cont = r->EnforceRule(c, this);
        r = drag[i++];
    }
    return cont;
}

bool TreePile::checkDropRules(Card *c)
{
    Rule *r = drop[0];
    int i = 1;
    bool cont = true;
    while (r && cont)
    {
        cont = r->EnforceRule(c, this);
        r = drop[i++];
    }
    return cont;
}

bool TreePile::checkDropRules(Card *c1, Card *c2)
{
    Rule *r = drop[0];
    int i = 1;
    bool cont = true;
    while (r && cont)
    {
        cont = r->EnforceRule(c1, c2);
        r = drop[i++];
    }
    return cont;
}


Rule *TreePile::DragRule(int i)
{
    return drag[i];
}

void TreePile::DragRule(int i, Rule *r)
{
    drag[i] = r;
}

Rule *TreePile::DropRule(int i)
{
    return drop[i];
}

void TreePile::DropRule(int i, Rule *r)
{
    drop[i] = r;
}

void TreePile::Clicked(Card *c)
{

}

void TreePile::AddDropRules(int n ...)
{
    va_list lp;
    va_start(lp, n);

    int i = 0;
    while (i < n)
        DropRule(i++, va_arg(lp, Rule*));
    DropRule(i, NULL);
    va_end(lp);
}

void TreePile::AddDragRules(int n ...)
{
    va_list lp;
    va_start(lp, n);

    int i = 0;
    while (i < n)
        DragRule(i++, va_arg(lp, Rule*));
    DragRule(i, NULL);
    va_end(lp);
}


uint TreePile::getMDistance(QPoint p)
{

}

TreeCard *TreePile::FindPos(int pos)
{
    int btree[] = {0,
                   1,
                  2, 3,
                 4, 5, 7,
                8, 9,14,15,
              16,17,19,30,31,
             32,33,35,60,62,63,
            64,65,67,71,124,126,127};
    pos = btree[pos];
    if (!pos) return NULL;
    int i = 64;
    while (!(pos&i)) i >>= 1;
    if (0 == i) return NULL;
    Card *t = bottom;
    if (!t) return NULL;
    while(i > 1 && t)
    {
        i >>= 1;
        t = (i&pos)?t->ROver():t->Over();
    }
    return dynamic_cast<TreeCard*>(t);
}

void TreePile::Unlink(Card *c, bool flip, bool recordUndo)
{
    TreeCard *tc = dynamic_cast<TreeCard*>(c);
    int l = tc->LeftUnder();
    int r = tc->RightUnder();
    TreeCard *ltc = FindPos(l);
    TreeCard *rtc = FindPos(r);

    if (recordUndo) new Move(c,this);


    if (l) ltc->ROver(NULL);
    if (r) rtc->Over(NULL);
    tc->Under(NULL);
    tc->RUnder(NULL);
    c->pile = NULL;

    if (bottom == c)
    {
        bottom = NULL;
        game->CheckWin();
    }



}


void TreePile::acceptCard(Card *c, bool flip, bool forceBase, bool recordUndo)
{

    TreeCard *tc = dynamic_cast<TreeCard*>(c);
    if (!tc) return;

    if (tc->pile)
        tc->pile->Unlink(c, flip, recordUndo);

    //    pips spots = c->Pip();
    int l = tc->LeftUnder();
    int r = tc->RightUnder();
    TreeCard *ltc = FindPos(l);
    TreeCard *rtc = FindPos(r);
    if (tc->myPile() != this)
    {
        // link underneath cards to this card
        if (l)
        {
            ltc->ROver(tc);
            tc->Under(ltc);
        }
        if (r)
        {
            rtc->Over(tc);
            tc->RUnder(rtc);
        }
        if (!bottom) bottom = tc;
    }

    tc->pile = this;
    if (l || r)
    {
        if (l)
            tc->move(ltc->pos() + delta);
        else
            tc->move(rtc->pos().x() - delta.x(),rtc->pos().y() + delta.y());
    } else
        tc->move(pos());

    tc->raise();
    tc->show();
    //    if (spots == KING) game->CheckWin();

}

//--------------------------------------------------------------------------

void Pile::acceptCard(Card *c, bool flip, bool forceBase, bool recordUndo)
{
    pips spots = c->Pip();
    if (c->pile)
        c->pile->Unlink(c, flip, recordUndo);

    c->under = top;
    if (top)
        top->over = c;
    else
    {
        bottom = c;
        top = c;
    }

    do
    {
        c->pile = this;
        c->move((top != c && top && !forceBase)?(top->pos() + delta / (top->faceup?1:2)):pos());
        c->raise();
        c->show();
        top = c;
        c = c->over;
    } while (c);
    if (spots == KING) game->CheckWin();
}

QPoint Pile::tops()
{
    return top->pos();
}

uint Pile::getMDistance(QPoint p)
{
    if (Top())
        return (Top()->pos() - p).manhattanLength();
    else
        return (pos() - p).manhattanLength();
}

void Pile::Clicked(Card *c)
{
    if (c) game->OneCardPlayoff(c);

}

void Pile::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        Clicked(NULL);
    }

}

void Pile::Unlink(Card *c, bool flip, bool recordUndo)
{
    Card *oldUnder = c->Under();
    if (!c->under)
    {
        top = NULL;
        bottom = NULL;
    }
    if (recordUndo)
        new Move(c, this);
    if (oldUnder)
    {
        if (flip)
            oldUnder->Faceup(flip);
        oldUnder->over = NULL;
        top = oldUnder;
    }
}



Pile::Pile(int x, int y, int dx, int dy, Rule **Drag, Rule **Drop, QWidget *parent):
    TreePile(x, y, dx, dy, Drag, Drop, parent), top(0)
{

}

Pile::~Pile()
{
    Card *c = top;
    while (c)
    {
        top = c->under;
        delete c;
        c = top;
    }
}



//--------------------------------------------------------------------------

StockPile::StockPile(int x, int y, int dx, int dy, QWidget *parent):
    Pile(x, y, dx, dy, StockDrag, StockDrop, parent)
{

}

//--------------------------------------------------------------------------


FoundationPile::FoundationPile(int x, int y, int dx, int dy, QWidget *parent):
    Pile(x, y, dx, dy, FoundationDrag, FoundationDrop,parent)
{

}

//--------------------------------------------------------------------------


TableauPile::TableauPile(int x, int y, int dx, int dy, QWidget *parent):
    Pile(x, y, dx, dy, TableauDrag, TableauDrop, parent)
{

}

void TableauPile::Clicked(Card *c)
{
    if (c) game->PlayoffTableau(this, c);
}

//--------------------------------------------------------------------------

WastePile::WastePile(int x, int y, int dx, int dy, QWidget *parent):
    Pile(x, y, dx, dy, WasteDrag, WasteDrop, parent)
{

}

//--------------------------------------------------------------------------

void StockPile::Clicked(Card *c)
{
    game->DealCard(this);
}

//--------------------------------------------------------------------------

FreeCellPile::FreeCellPile(int x, int y, int dx, int dy, QWidget *parent):
    Pile(x, y, dx, dy, FreeCellDrag, FreeCellDrop, parent)
{

}

void FreeCellPile::Clicked(Card *c)
{
    if (c) game->PlayoffFreeCell(this, c);
}

//--------------------------------------------------------------------------

PyramidPile::PyramidPile(int x, int y, int dx, int dy, QWidget *parent):
    TreePile(x, y, dx, dy, PyramidDrag, PyramidDrop, parent)
{

}

void PyramidPile::Clicked(Card *c)
{
    if (c && !c->Over() && !c->ROver())
        game->OneCardPlayoff(c);

}
