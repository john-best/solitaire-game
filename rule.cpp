#include "rule.h"
#include "game.h"
#include <QQueue>


extern Game *game;
QQueue<Rule*> Rule::theRules;

bool ProhibitDropSameColor::
EnforceRule(Card *c, TreePile *p)
{
    return p->Top()?!p->Top()->Color() == c->Color():true;

}

bool AllowDropSameColor::
EnforceRule(Card *c, TreePile *p)
{
    return p->Top()?p->Top()->Color() == c->Color():true;

}


bool ProhibitDrop::EnforceRule(Card *c, TreePile *p)
{
    return false;
}


bool AceOnEmpty::EnforceRule(Card *c, TreePile *p)
{
    return p->Top() || c->Pip() == ACE;
}


bool AllowDropSameSuit::EnforceRule(Card *c, TreePile *p)
{
    return p->Top()?p->Top()->Suit() == c->Suit():true;
}

bool ProhibitDropSameSuit::EnforceRule(Card *c, TreePile *p)
{
    return p->Top()?!p->Top()->Suit() == c->Suit():true;
}


bool DropOneUp::EnforceRule(Card *c, TreePile *p)
{
    return !p->Top() || c->Pip() == p->Top()->Pip()+1;
}


bool DropOneDown::EnforceRule(Card *c, TreePile *p)
{
    return !p->Top() || c->Pip() == p->Top()->Pip()-1;
}


bool DropSingle::EnforceRule(Card *c, TreePile *p)
{
    return !c->Over();
}


bool KingOnEmpty::EnforceRule(Card *c, TreePile *p)
{
    return p->Top() || c->Pip() == KING;
}


bool DropWholeSuit::EnforceRule(Card *c, TreePile *p)
{
    Card *temp = c;
    if (p->Top() && c->Value() != p->Top()->Value()+1 ||
            !p->Top() && c->Pip() != ACE)
    {
        return false;
    }
    while (temp->Under()
           && temp->Value() == temp->Under()->Value()-1
           && temp->Pip() != KING)
    {
        temp = temp->Under();
    }
    return temp->Pip() == KING;
}

bool DropEmptyStack::EnforceRule(Card *c, TreePile *p)
{
    return !p->Top();
}

bool DropFreeCellLimit::EnforceRule(Card *c, TreePile *p)
{
    int limit = game->getFreeMoves()>> (p->Top() == 0);
    int count = 1;

    while (c->Over())
    {
        count++;
        c = c->Over();
    }
    return count <= limit;
}


//drag start ------------------------------------------------------------------

bool ProhibitDrag::EnforceRule(Card *c, TreePile *p)
{
    return false;
}


bool DragTopOnly::EnforceRule(Card *c, TreePile *p)
{
    TreeCard *tc = dynamic_cast<TreeCard*>(c);
        return !c->Over() && tc?!tc->ROver():true;
}


bool DragFaceUpOnly::EnforceRule(Card *c, TreePile *p)
{
    return c->Faceup();
}

bool ProhibitDragKing::EnforceRule(Card *c, TreePile *p)
{
    return c->Pip() != KING;
}

bool DragStackDecreasing::EnforceRule(Card *c, TreePile *p)
{
    while (c->Over())
    {
        if (c->Pip() == c->Over()->Pip()+1)
            c = c->Over();
        else return false;
    }
    return true;
}

bool DragStackSameSuit::EnforceRule(Card *c, TreePile *p)
{
    while (c->Over())
    {
        if (c->Suit() == c->Over()->Suit())
            c = c->Over();
        else return false;
    }
    return true;
}

bool DragFreeCellLimit::EnforceRule(Card *c, TreePile *p)
{
    int limit = game->getFreeMoves();
    int count = 1;

    while (c->Over())
    {
        count++;
        c = c->Over();
    }
    return count <= limit;
}


bool DragAlternatingColors::EnforceRule(Card *c, TreePile *p)
{
    while (c->Over())
    {
        if (c->Color() != c->Over()->Color())
            c = c->Over();
        else return false;
    }
    return true;
}

// drag end -------------------------------------------------------------------

Rule::Rule()
{
    theRules.append(this);
}

void Rule::deleteRules()
{
    while (!theRules.isEmpty())
        delete theRules.dequeue();
}

// treecards drag start -------------------------------------------------------


bool TCDropSum13::EnforceRule(Card *c1, Card *c2)
{
    return ((c1->Pip() + c2->Pip() + 2) == 13);

}


bool TCDragTop::EnforceRule(Card *c, TreePile *p)
{
    return (!c->Over() && !c->ROver());
}

