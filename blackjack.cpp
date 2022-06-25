#include "blackjack.h"

////////////////////////////////////////////////////
// Card
Card::Card(CardSuit suit, CardRank rank)
        : m_suit{suit}, m_rank{rank} {

}

const QString Card::toString() const {
        QString s{};
        switch (m_rank)
        {
        case RANK_2:		s += '2'; break;
        case RANK_3:		s += '3'; break;
        case RANK_4:		s += '4'; break;
        case RANK_5:		s += '5'; break;
        case RANK_6:		s += '6'; break;
        case RANK_7:		s += '7'; break;
        case RANK_8:		s += '8'; break;
        case RANK_9:		s += '9'; break;
        case RANK_10:		s += 'T'; break;
        case RANK_JACK:		s += 'J'; break;
        case RANK_QUEEN:    s += 'Q'; break;
        case RANK_KING:		s += 'K'; break;
        case RANK_ACE:		s += 'A'; break;
        case MAX_RANKS:		s += 'x'; break;
        }
        s += "+";
        switch (m_suit)
        {
        case SUIT_CLUB:		s += 'C'; break;
        case SUIT_DIAMOND:	s += 'D'; break;
        case SUIT_HEART:	s += 'H'; break;
        case SUIT_SPADE:	s += 'S'; break;
        case MAX_SUITS:		s += 'x'; break;
        }
        return s;
}

//template<typename TPrintHelper>
//void Card::print() const {
//        TPrintHelper printfunc;
//        printfunc("card: "+toString());
//}

std::array<int, 2> Card::getValue() const {
    switch (m_rank)
    {
    case RANK_2:		return {2,0};
    case RANK_3:		return {3,0};
    case RANK_4:		return {4,0};
    case RANK_5:		return {5,0};
    case RANK_6:		return {6,0};
    case RANK_7:		return {7,0};
    case RANK_8:		return {8,0};
    case RANK_9:		return {9,0};
    case RANK_10:
    case RANK_JACK:
    case RANK_QUEEN:
    case RANK_KING:
        return {10,0};
    case RANK_ACE:
        return {1,11};
    case MAX_RANKS:
        return {0,0};
    }
    return {0,0};
}

Card::CardSuit& operator++(Card::CardSuit& suit){
    switch (suit)
    {
    case Card::SUIT_CLUB:		return suit = Card::SUIT_DIAMOND;
    case Card::SUIT_DIAMOND:	return suit = Card::SUIT_HEART;
    case Card::SUIT_HEART:	return suit = Card::SUIT_SPADE;
    case Card::SUIT_SPADE:	return suit = Card::MAX_SUITS;
    case Card::MAX_SUITS:		return suit = Card::MAX_SUITS;
    }
    assert(false);
    return suit = Card::MAX_SUITS;
}

Card::CardSuit operator++(Card::CardSuit& suit, int){
    Card::CardSuit tmp(suit);
    ++suit;
    return tmp;
}

Card::CardRank& operator++(Card::CardRank& rank){
    switch (rank)
    {
    case Card::RANK_2:	return rank = Card::RANK_3;
    case Card::RANK_3:		return rank = Card::RANK_4;
    case Card::RANK_4:		return rank = Card::RANK_5;
    case Card::RANK_5:		return rank = Card::RANK_6;
    case Card::RANK_6:		return rank = Card::RANK_7;
    case Card::RANK_7:		return rank = Card::RANK_8;
    case Card::RANK_8:		return rank = Card::RANK_9;
    case Card::RANK_9:		return rank = Card::RANK_10;
    case Card::RANK_10:		return rank = Card::RANK_JACK;
    case Card::RANK_JACK:		return rank = Card::RANK_QUEEN;
    case Card::RANK_QUEEN:    return rank = Card::RANK_KING;
    case Card::RANK_KING:		return rank = Card::RANK_ACE;
    case Card::RANK_ACE:		return rank = Card::MAX_RANKS;
    case Card::MAX_RANKS:		return rank = Card::MAX_RANKS;
    }
    assert(false);
    return rank = Card::MAX_RANKS;
}

Card::CardRank operator++(Card::CardRank& rank, int){
    Card::CardRank tmp(rank);
    ++rank;
    return tmp;
}


////////////////////////////////////////////////////
// Deck
Deck::Deck(){
    size_t i{};
    for(Card::CardSuit suit{}; suit<Card::MAX_SUITS; ++suit)
        for(Card::CardRank rank{}; rank<Card::MAX_RANKS; ++rank)
            m_deck[i++] = {suit, rank};
    resetDeck();
}

void Deck::resetDeck(){
        m_cardPtr = m_deck.begin();
    }

void Deck::SwapCard(size_t index1, size_t index2){
        assert(index1<m_deck.size());
        assert(index2<m_deck.size());
        Card tmp = m_deck[index1];
        m_deck[index1] = m_deck[index2];
        m_deck[index2] = tmp;
    }

void Deck::ShuffleDeck(){
        srand(time(nullptr));
        for(int i=0;i<10;++i)
            for(size_t index=0;index<m_deck.size();++index)
                SwapCard(index, rand()%52);
        resetDeck();
    }

//template<typename TPrintHelper>
//void Deck::print() const {
//        TPrintHelper printfunc;
//        QString s{};
//        for(size_t index=0; index<m_deck.size();++index){
//            s += m_deck[index].toString()+"  ";
//            if((index+1)%4 == 0)
//                s += "\n";
//        }
//        printfunc(s);
//    }

const Card* Deck::dealCard(){
        if(m_cardPtr==m_deck.end())
            return nullptr;
        return m_cardPtr++;
    }

////////////////////////////////////////////////////
// Player
Player::Player(const QString& name) : m_name{name}, m_cardValues{} {
}

void Player::resetValues(){
    m_cardValues = {0, 0};
}

int Player::getMaxValue() const
{
    return (m_cardValues[0]>m_cardValues[1]) ? m_cardValues[0] : m_cardValues[1];
}

int Player::getMinValue() const
{
    if( m_cardValues[0]!=0 && m_cardValues[0]<m_cardValues[1] )
        return m_cardValues[0];
    else
        return m_cardValues[1];
}

bool Player::isLoss() const
{
    return m_cardValues[0]>21 && (m_cardValues[1]>21 || m_cardValues[1]==0);
}

bool Player::isIWin(const Player &anotherPlayer) const
{
    int myValue = (getMaxValue()<=21) ? getMaxValue() : getMinValue();
    if( myValue>21 ) myValue = 0;
    int anotherValue = (anotherPlayer.getMaxValue()<=21) ? anotherPlayer.getMaxValue() : anotherPlayer.getMinValue();
    if( anotherValue>21 ) myValue = 0;
    return myValue > anotherValue;
}

bool Player::isDealerEnough() const
{
    return getMaxValue()>=17;
}

const std::array<int, 2>& Player::addCard(const Card& card){
        std::array<int, 2> cardValue = card.getValue();
        if( cardValue[1]!=0 ){
            m_cardValues[1] = m_cardValues[0] + cardValue[1];
            m_cardValues[0] += cardValue[0];
        }
        else{
            m_cardValues[0] += cardValue[0];
            if(m_cardValues[1]!=0)
                m_cardValues[1] += cardValue[0];
        }
        return m_cardValues;
    }

//template<typename TPrintHelper>
//void Player::printValues() const {
//        TPrintHelper printfunc;
//        QString s = "player "+m_name+"has ";
//        for(auto i : m_cardValues){
//            if(i!=0)
//                s += QString::number(i)+"   ";
//        }
//        printfunc(s);
//    }
