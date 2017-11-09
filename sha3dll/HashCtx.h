#ifndef __HASHCTX__H__
#define __HASHCTX__H__

#include "HashStrategy.h"
#include <memory>
#include <vector>

/**
 * Klasa odpowiedzialna za definicje odpowiedniego wariantu funkcji skrotu oraz odpowiednie skorelowanie danych i obliczen funkcji skrotu
 */
class HashCtx
{
private:
	std::unique_ptr<HashStrategy> strategy;

public:
	enum HashType
	{
		SHA3_224 = 0,
		SHA3_256 = 1,
		SHA3_384 = 2,
		SHA3_512 = 3,
	};

	HashCtx();
	~HashCtx();

	void clean();

	/**
	* Funkcja definiuje odpowiednia wersje funkcji skrotu
	* @param wersja funkcji
	*/
	void setHashAlgo(HashType hashType);
	/**
	* Funkcja pobiera dane z podanego pliku i wywoluje dla nich odpowiednie funkcje algorytmu skrotu
	* @param sciezka do pliku
	* @return wartosc funkcji skrotut
	*/
	std::string computeHash(const std::string& filepath);
	
};


#endif