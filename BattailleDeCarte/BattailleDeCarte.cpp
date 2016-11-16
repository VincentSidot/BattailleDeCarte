// BattailleDeCarte.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int tries = 1'000'000;


inline std::string winner(const std::vector<std::string> &v,int &p1,int &p2)
{
	int cpt{ 0 };
	p1 = 0;
	p2 = 0;
	for (auto i : v)
	{
		if (i == "Player 1")
		{
			p1++;
			cpt++;
		}
		else if (i == "Player 2")
		{
			p2++;
			cpt--;
		}
	}
	return std::string((cpt > 0) ? "Player 1" : (cpt < 0) ? "Player 2" : "Equality");
}


int main()
{

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	std::vector<std::string> m_winner;
	std::vector<size_t> m_tours;																												//1024*1024 octet
	std::cout << "Requesting " << ((tries * sizeof(size_t) + tries * sizeof(std::string("Player 1")) + tries * sizeof(int) + tries * sizeof(int)) / 1'048'576) << "MO RAM\n";
	m_winner.reserve(tries * 8);
	m_tours.reserve(tries * 8);
	vPackCarte *p1, *p2, *carte;
	vGame *jeu;
	std::cout << "Starting " << tries <<" tests" << std::endl;
	for (size_t i{ 0 }; i < tries; ++i)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		p1 = new vPackCarte;
		p2 = new vPackCarte;
		carte = new vPackCarte(TAILLE::GRAND);
		carte->shuffle(seed);
		carte->distrib(*p1, *p2);
		delete carte;
		jeu = new vGame(*p1, *p2);
		delete p1; delete p2;
		jeu->play();
		m_winner.push_back(jeu->winner());
		m_tours.push_back(jeu->getNum());
		delete jeu;
	//	std::cout << jeu->winner() << " win the " << i << " test(s)" << std::endl;;
	}
	end = std::chrono::system_clock::now();
	int time = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
	std::cout << "Finish ...\nPrinting result\n";

	/*for (auto i{ 0 }; i < m_winner.size(); ++i)
	{
		std::cout << "Game : " << i << " -- Winner : " << m_winner[i] << " -- Time : " << m_time[i] << "ms -- Round : " << m_tours[i] << " -- Round per Second " << m_oppers[i] << "rps\n";
	}*/
	int p1i, p2i;
	std::cout << "Most winner : " << winner(m_winner,p1i,p2i) << std::endl;
	std::cout << "Player 1 win : " << p1i << " times\nPlayer 2 win : " << p2i << " times\n";
	std::cout << "Player 1 win rate : " << ((p1i * 100) / tries) << "%\nPlayer 2 win rate : " << ((p2i * 100) / tries) << "%\n";
	std::cout << "Average time : " << time << "ms"<<std::endl;
	int round{ std::accumulate(m_tours.begin(), m_tours.end(), 0) };
	double average_round{ static_cast<double>( round / m_tours.size()) };
	std::cout << "Average Round : " << average_round << std::endl;
	std::cout << "Round per second average : " << static_cast<double>((round / time))<< " rpMs" <<std::endl;
		
	getchar();
    return 0;
}

