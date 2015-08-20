#ifndef RULE_H
#define RULE_H
#include "card.h"
#include "pile.h"
#include <QQueue>
class Rule
{
    static QQueue<Rule*> theRules;
public:
    virtual bool EnforceRule(Card *c, TreePile *p){return false;}
    virtual bool EnforceRule(Card *c1, Card *c2){return false;}
    Rule();
    static void deleteRules();
};

class ProhibitDropSameColor : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class AllowDropSameColor : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class AllowDropSameSuit : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class ProhibitDropSameSuit : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class ProhibitDrop : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class AceOnEmpty : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class KingOnEmpty: public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DropOneUp : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DropOneDown : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DropSingle : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DropWholeSuit : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DropEmptyStack : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DropFreeCellLimit : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

// drag

class ProhibitDrag : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};


class DragTopOnly : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DragFaceUpOnly : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class ProhibitDragKing : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DragStackIncreasing : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DragStackSameSuit : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};
class DragStackDecreasing: public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

class DragFreeCellLimit : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};
class DragAlternatingColors : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};


// treecards
// drop

class TCDropSum13 : public Rule
{
    bool EnforceRule(Card *c1, Card *c2);
};

//drag

class TCDragTop : public Rule
{
    bool EnforceRule(Card *c, TreePile *p);
};

#endif // RULES_H
