#ifndef __HASHSTRATEGY__H__
#define __HASHSTRATEGY__H__

#include <vector>
#include <string>

/**
* Klasa abstrakcyjna narzucajaca interfejs klasom implementujacym funkcje skrotu
*/
class HashStrategy
{
public:
	HashStrategy();
	virtual ~HashStrategy();

	/**
	* Funkcja wykonujaca odpowiedni kroki algorytmu dla podanych bajtow, jednoczesnie oczekujac na kolejne
	* @param lancuch bajtow
	*/
	virtual void update(const std::vector<unsigned char>& data) = 0;
	/**
	* Funkcja wykonujaca kolejne kroki algorytmu funkcji skrotu dla ostanich bajtow pliku
	* @param ostatni lancuch bajtow
	* @return ostateczny wynik - skrot
	*/
	virtual std::string final(const std::vector<unsigned char>& data) = 0;
	/**
	* Funkcja wykonujaca obliczajaca skrot podanego ciagu bajtow
	* @param lancuch bajtow
	* @return wynik funkcji skrotu
	*/
	virtual std::string compute(const std::vector<unsigned char>& data) = 0;
};

#endif