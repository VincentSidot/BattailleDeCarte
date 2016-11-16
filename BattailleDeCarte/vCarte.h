#pragma once

#include <string>
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <random>
#include <chrono>

enum class TAILLE : int { GRAND = 52 , PETIT = 32};
enum class PLAYER {PLAYER1 ,PLAYER2,NONAME};
enum class TYPE {CARREAU , PIQUE, COEUR, TREFLE};
enum class VALEUR : size_t 
{
DEUX = 2,
TROIS = 3,
QUATRE = 4,
CINQ = 5,
SIX = 6,
SEPT = 7,
HUIT = 8,
NEUF = 9,
DIX = 10,
VALET = 11,
DAME = 12,
ROI = 13,
AS = 14,	
};

inline std::string getName(const TYPE &t)
{
	switch (t)
	{
	case TYPE::CARREAU:
		return " de carreau";
		break;
	case TYPE::COEUR:
		return " de coeur";
		break;
	case TYPE::PIQUE:
		return " de pique";
		break;
	case TYPE::TREFLE:
		return " de trefle";
		break;
	default:
		break;
	}
}

class vCarte
{
public:
	inline vCarte(VALEUR value, TYPE type) : m_value{ value }, m_type{ type }
	{
		switch (value)
		{
		case VALEUR::DEUX:
			m_name = "Deux" + getName(type);
			break;
		case VALEUR::TROIS:
			m_name = "Trois" + getName(type);
			break;
		case VALEUR::QUATRE:
			m_name = "Quatre" + getName(type);
			break;
		case VALEUR::CINQ:
			m_name = "Cinq" + getName(type);
			break;
		case VALEUR::SIX:
			m_name = "Six" + getName(type);
			break;
		case VALEUR::SEPT:
			m_name = "Sept" + getName(type);
			break;
		case VALEUR::HUIT:
			m_name = "Huit" + getName(type);
			break;
		case VALEUR::NEUF:
			m_name = "Neuf" + getName(type);
			break;
		case VALEUR::DIX:
			m_name = "Dix" + getName(type);
			break;
		case VALEUR::VALET:
			m_name = "Valet" + getName(type);
			break;
		case VALEUR::DAME:
			m_name = "Dame" + getName(type);
			break;
		case VALEUR::ROI:
			m_name = "Roi" + getName(type);
			break;
		case VALEUR::AS:
			m_name = "As" + getName(type);
			break;
		default:
			break;
		}
	}

	inline VALEUR value() const { return m_value; }
	inline TYPE type() const { return m_type; }
	inline std::string name() const { return m_name; }
	inline bool operator<(const vCarte &c)
	{
		return this->m_value < c.value();
	}
	inline bool operator>(const vCarte &c)
	{
		return !operator<(c);
	}
	inline bool operator==(const vCarte &c)
	{
		return m_value == c.value();
	}
	inline bool operator!=(const vCarte &c)
	{
		return !operator==(c);
	}
	inline bool operator<=(const vCarte &c)
	{
		return !operator>(c);
	}
	inline bool operator>=(const vCarte &c)
	{
		return !operator<(c);
	}

	~vCarte() {}

protected:

	VALEUR m_value;
	TYPE m_type;
	std::string m_name;
};

inline std::ostream& operator<<(std::ostream &os, const vCarte &c)
{
	os << c.name() << " ";
	return os;
}

class vPackCarte
{
public:
	inline vPackCarte(const TAILLE &t)
	{
		m_size = static_cast<size_t>(t);
		if (t == TAILLE::GRAND)
		{
			for (size_t i{ 0 }; i < 4; ++i)
			{
				for (size_t j{ 2 }; j < 15; ++j)
				{
					m_jeu.push_back(vCarte(VALEUR(j), TYPE(i)));
				}
			}
		}
		else
		{
			for (size_t i{ 0 }; i < 4; ++i)
			{
				for (size_t j{ 7 }; j < 15; ++j)
				{
					m_jeu.push_back(vCarte(VALEUR(j), TYPE(i)));
				}
			}
		}

	}
	inline vPackCarte() : m_size{ 0 }  {}
	inline vPackCarte(const vPackCarte &c)
	{
		m_jeu = c.list();
		m_size = c.size();
	}
	inline std::list<vCarte> list() const { return m_jeu; }
	inline size_t size() const { return m_size; }
	static inline vPackCarte shuffle(const vPackCarte &p,const unsigned &seed)
	{
		vPackCarte temp(p);
		temp.shuffle(seed);
		return temp;
	}
	inline void shuffle(const unsigned &seed)
	{
		std::vector<int> temp;
		std::vector<vCarte> tmp{};
		for (int i{ 0 }; i < m_size; ++i)
		{
			temp.push_back(i);
		}
		std::shuffle(temp.begin(), temp.end(),std::default_random_engine(seed));
		for (size_t i{ 0 }; i < m_size; ++i)
		{
			tmp.push_back(m_jeu.back());
			m_jeu.pop_back();
		}
		for (int i{ 0 }; i < m_size; ++i)
		{
			m_jeu.push_front(tmp[temp[i]]);
		}
	}
	inline void distrib(vPackCarte &p1, vPackCarte &p2)
	{
		while (!m_jeu.empty())
		{
			p1.add(m_jeu.front());
			m_jeu.pop_front();
			if (!m_jeu.empty())
			{
				p2.add(m_jeu.front());
				m_jeu.pop_front();
			}
		}
	}
	inline void add(const vCarte &c)
	{
		m_jeu.push_back(c);
	}

private:
	std::list<vCarte> m_jeu;
	size_t m_size;
};
class vGame
{
public:
	inline vGame(const vPackCarte &p1, const vPackCarte &p2, bool dispmode = false, bool waitmode = false) : m_p1{ p1.list() }, m_p2{ p2.list() }, m_tour{ 0 }, m_win{ PLAYER::NONAME }, isDispmode{ dispmode }, isWaitmode{ waitmode } {}
	inline size_t getNum() const { return m_tour; }
	inline std::list<vCarte> p1() const { return m_p1; }
	inline std::list<vCarte> p2() const { return m_p2; }
	inline std::string winner() const { return (m_win == PLAYER::NONAME) ? "Nobody" : ((m_win == PLAYER::PLAYER1) ? "Player 1" : "Player 2"); }
	inline void play()
	{
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
		while (!m_p1.empty() && !m_p1.empty())
		{
			if (!m_p1.empty() && !m_p2.empty())
			{
				if (isWaitmode && !isDispmode)
				{
					std::cout << "Press ENTER" << std::endl;
					getchar();
				}
				else
				{
					if (isDispmode && !m_p1.empty() && !m_p2.empty())
					{
						std::cout << "Player 1 : " << m_p1.front() << " -- Player 2 : " << m_p2.front() << " -- Tour : " << m_tour << " -- " << m_p1.size() << "/" << m_p2.size() << std::endl;
					}
					if (isWaitmode)
					{
						getchar();
					}
				}
				if (m_p1.front() < m_p2.front() && !m_p1.empty() && !m_p2.empty())
				{
					while (!m_stack_p2.empty())
					{
						m_p2.push_back(m_stack_p2.top());
						m_stack_p2.pop();
					}
					while (!m_stack_p1.empty())
					{
						m_p2.push_back(m_stack_p1.top());
						m_stack_p1.pop();
					}
					m_p2.push_back(m_p2.front());
					m_p2.push_back(m_p1.front());
					m_p2.pop_front();
					m_p1.pop_front();
					++m_tour;
				}
				else if (m_p1.front() > m_p2.front() && !m_p1.empty() && !m_p2.empty())
				{
					while (!m_stack_p1.empty())
					{
						m_p1.push_back(m_stack_p1.top());
						m_stack_p1.pop();
					}
					while (!m_stack_p2.empty())
					{
						m_p1.push_back(m_stack_p2.top());
						m_stack_p2.pop();
					}
					m_p1.push_back(m_p1.front());
					m_p1.push_back(m_p2.front());
					m_p2.pop_front();
					m_p1.pop_front();
					++m_tour;
				}
				else if (m_p1.front() == m_p2.front() && (m_p1.size() > 2 && m_p2.size() > 2))
				{
					m_stack_p1.push(m_p1.front());
					m_p1.pop_front();
					m_stack_p1.push(m_p1.front());
					m_p1.pop_front();

					m_stack_p2.push(m_p2.front());
					m_p2.pop_front();
					m_stack_p2.push(m_p2.front());
					m_p2.pop_front();
					++m_tour;
				}
				else if (m_p1.size() > m_p2.size() && !m_p1.empty() && !m_p2.empty())
				{
					m_stack_p1.push(m_p1.front());
					m_p1.pop_front();
					m_stack_p1.push(m_p1.front());
					m_p1.pop_front();

					m_stack_p2.push(m_p2.front());
					m_p2.pop_front();
					m_stack_p2.push(m_p1.front());
					m_p1.pop_front();
					++m_tour;
				}
				else if (m_p1.size() < m_p2.size() && !m_p1.empty() && !m_p2.empty())
				{
					m_stack_p1.push(m_p1.front());
					m_p1.pop_front();
					m_stack_p1.push(m_p2.front());
					m_p2.pop_front();

					m_stack_p2.push(m_p2.front());
					m_p2.pop_front();
					m_stack_p2.push(m_p2.front());
					m_p2.pop_front();
					++m_tour;
				}
				else
				{
					break;
				}

				if (isDispmode && !m_p2.empty() && !m_p1.empty())
				{
					std::cout << std::string("------------>") << std::string((m_p1.front() > m_p2.front()) ? "Player 1" : ((m_p1.front() == m_p2.front()) ? "Equal" : "Player 2")) << std::string(" win this duel") << std::endl;
				}
				else if (isDispmode)
				{
					std::cout << std::string("------------>") << std::string((m_p1.empty()) ? "Player 2" : "Player 1") << std::string(" win this duel and game") << std::endl;
				}
			}
			else
			{
				break;
			}

		}
		end = std::chrono::system_clock::now();
		m_time = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
		if(m_time != 0)
			m_oppers = m_tour / (static_cast<double>(m_time)/1000);
		m_win = (m_p1.empty()) ? PLAYER::PLAYER2 : PLAYER::PLAYER1;
	}
	inline int time() const { return m_time; }
	inline double ops() const { return m_oppers; }
private:
	std::list<vCarte> m_p1;
	PLAYER m_win;
	int m_time;
	double m_oppers;
	std::list<vCarte> m_p2;
	bool isDispmode;
	bool isWaitmode;
	size_t m_tour;
	std::stack<vCarte> m_stack_p1;
	std::stack<vCarte> m_stack_p2;
};
