#define FinalArray
#ifdef FinalArray

#include <iostream>
#include <array>
#include <vector>
#include <algorithm> // for std::shuffle
#include "Random.h"  // for Random::mt
#include <string_view>

namespace Settings
{
	constexpr int scoreLimit{ 21 };
	constexpr int dealerLimit{ 17 };
}

struct Card
{
	enum Ranked
	{
		rank_ace,
		rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10,
		rank_jack,
		rank_queen,
		rank_king,
		max_rank
	};

	enum Suits
	{
		spades, diamonds, clubs, hearts, max_suits
	};

	static constexpr std::array allRanks
	{
		rank_ace,
		rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10,
		rank_jack,
		rank_queen,
		rank_king
	};
	static constexpr std::array allSuits
	{
		spades, diamonds, clubs, hearts
	};

	static constexpr std::array<std::string_view, max_rank> named_ranks{
			"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	static constexpr std::array<char, max_suits> named_suits{
		'S', 'D', 'C', 'H' };

	Ranked ranked_card{};
	Suits card_type{};

	friend std::ostream& operator<<(std::ostream& out, const Card& card)
	{
		out << card.named_ranks[card.ranked_card] << card.named_suits[card.card_type];
		return out;
	}
};

class Deck
{
private:
	int card_counter{ 0 }; // Random::get(1, 52)
	std::array<Card, 52> m_cards{};
public:
	Deck() = default;
	Deck(Card cards)
		: m_cards{cards}
	{}

	const Card& dealCard() const
	{
		return m_cards[card_counter];
	}
	void shuffle()
	{
		std::shuffle(m_cards.begin(), m_cards.end(), Random::mt);
	}
	std::array<Card, 52>& getMCards() { return m_cards; }
	
	void initializeCards()
	{
		std::cout << "\t\t\t\t\t TWENTY ONE - BLACK JACK ONBOARD\n\n";
		std::size_t count{ 0 };
		for (auto suit : Card::allSuits)
		{
			for (auto rank : Card::allRanks)
				getMCards()[count++] = Card{ rank, suit };
		}
	}
	void printCards() 
	{
		for (const auto& c : getMCards())
			std::cout << c << '\n';
	}
};

class Player
{
private:
	int p_score{ 0 };
public:
	Player() = default;
	Player(int score) : p_score{score} { }

	void hit(int s) 
	{ 
		if (s >= 10) { s = 10; p_score += s; return;}
		if (s == 0) 
		{ 
			if ((p_score + 11) > 21) { s = 1; p_score += s; return; }
			else { s = 11; p_score += s; return; }
		}
		p_score += (s+1); 
	}
	constexpr int show_score() const { return p_score; }
};

void cleanInput()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void runVingtUn(Deck& d, Player& player, Player& dealer)
{
	char hit{};
	bool hitCondition{true};
	d.initializeCards();

	d.shuffle();
	dealer.hit(d.dealCard().ranked_card);
	std::cout << "The dealer is showing: " << dealer.show_score() << "\n\n";

	d.shuffle();
	player.hit(d.dealCard().ranked_card);
	std::cout << "The player has score : " << player.show_score() << '\n';
	
	while (hitCondition)
	{
		std::cout << "(h) to hit, or (s) to stand: ";
		std::cin >> hit;

		cleanInput();

		if (hit != 'h')
		{
			hitCondition = false;
			break;
		}
		d.shuffle();
		player.hit(d.dealCard().ranked_card);
		std::cout << "Player HITS!\nThe player was dealt " << d.dealCard()
			<< ". Your new score :"   << player.show_score() << '\n';
		if (player.show_score() > Settings::scoreLimit)
		{
			std::cout << "Player busts. Dealer Wins!\n";
			return;
		}
	}
	while (dealer.show_score() < Settings::dealerLimit)
	{
		d.shuffle();
		dealer.hit(d.dealCard().ranked_card);
		std::cout << "The dealer was dealt: " << d.dealCard()
			<< ". They now have: " << dealer.show_score() << '\n';
		if (dealer.show_score() > Settings::scoreLimit)
		{
			std::cout << "Dealer busts. Player WINS!\n";
			return;
		}
	}
	std::cout << ((player.show_score() > Settings::scoreLimit || player.show_score() <= dealer.show_score()) ?
		"Dealer Wins.\n" : "Player WINS.\n"); // caps WINS on player to encourage on gambling addiction :}

	std::cout << '\n';
}

int main()
{
	Deck d{};
	Player player{};
	Player dealer{};

	runVingtUn(d, player, dealer);

	return 0;
}

#endif