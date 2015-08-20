#include "move.h"
#include "card.h"
#include "pile.h"
#include <algorithm>

QStack<Move*> Move::history;

extern int dealNumber;

void Move::undo()
{
    int n = repeat;
    Move *m = this;
    if (n > 1) m = history.pop();
    while (n)
    {


        Card *u = m->pile->Top();
        if (u)
            u->Faceup(m->underFaceup);
        m->card->Faceup(m->faceup);
        m->pile->acceptCard(m->card, false, m->pile->Type() == WASTE?n % dealNumber == 0:false, false);
        if (n > 1)
        {
            delete m;
            m = history.pop();
        }
        n--;
    }
    delete m;
}

void Move::Undo()
{
    if (!history.isEmpty())
    {
        Move *m = history.pop();
        m->undo();
    }
}

void Move::NewGame()
{
    qDeleteAll(history.begin(),history.end());
    history.clear();
}

Move::Move(Card *c, TreePile *p) :
    card(c), pile(p), repeat(1)
{

    underFaceup = c->Under()?c->Under()->Faceup():0;
    faceup = c->Faceup();
    history.push(this);

}

Move::Move(int n) :
    card(0), pile(0), repeat(n), underFaceup(0), faceup(0)
{

    history.push(this);
}




Move::~Move()
{

}

