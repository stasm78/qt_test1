#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <QString>

////////////////////////////////////////////////////
// Card
class Card{
public:
    enum CardSuit{
        SUIT_CLUB,
        SUIT_DIAMOND,
        SUIT_HEART,
        SUIT_SPADE,
        MAX_SUITS
    };
    enum CardRank{
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8,
        RANK_9,
        RANK_10,
        RANK_JACK,
        RANK_QUEEN,
        RANK_KING,
        RANK_ACE,
        MAX_RANKS
    };

private:
    CardSuit m_suit;
    CardRank m_rank;
public:
    Card(CardSuit suit = CardSuit::MAX_SUITS, CardRank rank = CardRank::MAX_RANKS);
    const QString toString() const;
    template<typename TPrintHelper>
    void print() const{
        TPrintHelper printfunc;
        printfunc("card: "+toString());
    }
    std::array<int, 2> getValue() const;
};

Card::CardSuit& operator++(Card::CardSuit& suit);

Card::CardSuit operator++(Card::CardSuit& suit, int);

Card::CardRank& operator++(Card::CardRank& rank);

Card::CardRank operator++(Card::CardRank& rank, int);

////////////////////////////////////////////////////
// Deck
class Deck
{
    std::array<Card, 52> m_deck;
    Card* m_cardPtr;
public:
    Deck();
    void resetDeck();
    void SwapCard(size_t index1, size_t index2);
    void ShuffleDeck();
    template<typename TPrintHelper>
    void print() const{
        TPrintHelper printfunc;
        QString s{};
        for(size_t index=0; index<m_deck.size();++index){
            s += m_deck[index].toString()+"  ";
            if((index+1)%4 == 0)
                s += "\n";
        }
        printfunc(s);
    }
    template<typename TPrintHelper>
    void printTopCard() const{
        m_cardPtr->print<TPrintHelper>();
    }
    const Card* dealCard();
    template<typename TPrintHelper>
    const Card* dealCard(){
            if(m_cardPtr==m_deck.end())
                return nullptr;
            printTopCard<TPrintHelper>();
            return m_cardPtr++;
        }
};

////////////////////////////////////////////////////
// Player
class Player
{
    QString m_name;
    std::array<int, 2> m_cardValues;
public:
    Player(const QString& name);
    void resetValues();
    int getMaxValue() const;
    int getMinValue() const;
    bool isLoss() const;
    bool isIWin(const Player& anotherPlayer) const;
    bool isDealerEnough() const;
    const std::array<int, 2>& addCard(const Card& card);
    template<typename TPrintHelper>
    void printValues() const{
        TPrintHelper printfunc;
        QString s = "player "+m_name+"has ";
        for(auto i : m_cardValues){
            if(i!=0)
                s += QString::number(i)+"   ";
        }
        printfunc(s);
    }
};


#endif // BLACKJACK_H
