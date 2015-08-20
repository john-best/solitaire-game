#ifndef CARD_H
#define CARD_H
#include <QWidget>
#include <QLabel>
#include <QImage>

class TreePile;
class Pile;

enum c_colors {BLACK, RED};
enum suits{CLUBS, DIAMONDS, HEARTS, SPADES};
enum pips{ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, SUIT_SIZE};
const int CARD_WIDTH = 71;
const int CARD_HEIGHT = 96;

class Card : public QLabel
{
    Q_OBJECT

protected:
    bool moving;
    TreePile* pile;
    bool okToMove;
    int value;
    pips pip;
    suits suit;
    c_colors cardColor;
    Card* over;
    Card* under;

    static QImage faces[53];
    static bool initialized;
    bool faceup;

    bool rcmoved;

    static QPoint startDragPos;
    static QPoint mouseDownOffset;

private:




public:
    Card(int c, QWidget *parent = 0);
    void static Initialize();
    void Faceup(bool makeFaceUp);
    bool Faceup(){return faceup;}
    bool canBeDragged();
    void mousePressEvent(QMouseEvent *event);
    virtual Card* ROver() {return NULL;}
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void CardClicked();
    TreePile* myPile(){return pile;}
    pips Pip() {return pip;}
    suits Suit() {return suit;}
    c_colors Color() {return cardColor;}
    Card* Over() { return over;}
    void Over(Card *c){over = c;}
    Card* Under() {return under;}
    void Under(Card *c){under = c;}
    int Value() {return value;}
    uint getMDistance(QPoint p);

    ~Card();
    friend class Pile;
    friend class TreePile;
};

void Shuffle(Card* Deck[], int n);


class TreeCard : public Card
{

    TreeCard *rover;
    TreeCard *runder;
    int position;

public:
    int Position(){return position;}
    void Position(int n){position = n;}
    TreeCard(int c, QWidget *parent = 0, int pos=0);
    int RightUnder();
    int LeftUnder();
    Card* ROver(){return rover;}
    void ROver(TreeCard *tc){rover = tc;}
    TreeCard* RUnder(){return runder;}
    void RUnder(TreeCard *tc){runder = tc;}
    void mouseReleaseEvent(QMouseEvent *event);




    friend class TreePile;
    friend class Pile;
};

#endif // CARD_H
