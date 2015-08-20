#ifndef GAME_H
#define GAME_H
#include <QList>
#include <QWidget>

enum game_types{KLONDIKE, SPIDER, FREECELL_GAME, PYRAMID_GAME, TRIPEAKS};
enum difficulty_type{REPEAT, EASY, MEDIUM, HARD};

class Card;
class TreeCard;
class Pile;
class TreePile;
class Game
{

protected:
    QList<TreePile*> piles;
    QWidget *parent;

public:
    virtual game_types Type()=0;
    virtual void newDeal(difficulty_type h = EASY)=0;
    virtual void DealCard(Pile *p){}
    virtual void setDealNumber(int n){}
    virtual void PlayoffTableau(TreePile *p, Card *c){}
    virtual void Redeal(QWidget *w)=0;
    virtual int getFreeMoves(){return 0;}
    virtual void PlayoffFreeCell(Pile *p, Card *c){}
    virtual TreePile *FoundationTarget (TreePile *p, Card *c){return p;}
    virtual bool OneCardPlayoff(Card *c);
    void DrawStock();
    virtual void CheckWin();
    virtual void Clear();
    void AddPile(TreePile *p);
    virtual void PlayoffAll();
    TreePile *FindBestPile(Card *c);
    virtual bool FindBestCard(TreeCard *c){return false;}
    Game(QWidget *p);
    virtual ~Game();
};

#endif // GAME_H
