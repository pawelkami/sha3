#ifndef __SHA3_H__
#define __SHA3_H__

#include "HashStrategy.h"
#include <algorithm>
#include <memory>
#include <array>
#include <iostream>
#include <map>
#include "utils.h"

/**
* Klasa implementujaca algorytm sha3 we wszystkich wariantach
*/
class sha3 :
	public HashStrategy
{
private:

	/**
	 * stala definiujaca liczbe kolumn tablicy stanu
	 */
	static const unsigned int X = 5;
	/**
	* stala definiujaca liczbe wierszy tablicy stanu
	*/
	static const unsigned int Y = 5;
	/**
	* stala definiujaca glebokosc tablicy stanu
	*/
	static const unsigned int W = 64;
	/**
	* stala definiujaca liczbe bajtow w tablicy stanu
	*/
	static const unsigned int B = 1600;
	/**
	* stala definiujaca liczbe rund obliczen algorytmu
	*/
	static const unsigned int NUMBER_OF_ROUNDS = 24;
	/**
	* tablica zawierajaca definicje stalych dla funkcji jota w celu jej zoptymalizowania
	*/
	static const uint64_t keccak_round_constants[NUMBER_OF_ROUNDS];
	/**
	* tablica zawierajaca definicje stalych dla funkcji rho w celu jej zoptymalizowania
	*/
	static const uint8_t rho_offset[NUMBER_OF_ROUNDS];
	/**
	* tablica zawierajaca definicje stalych dla funkcji pi w celu jej zoptymalizowania
	*/
	static const uint8_t pi_positions[NUMBER_OF_ROUNDS];

	typedef std::array<uint64_t, X*Y> state_array;

	/**
	* tablica stanu
	*/
	state_array A;
	/**
	* tablica przechowujaca biezace wyniki algorytmu na ktorych podstawie jest na koncu zwracana wartosc funkcji skrotu
	*/
	std::vector<unsigned char> S;
	/**
	* tablica bajtow przechowujaca dane nieprzetworzone przez pojedyncze wywolanie funkcji update, by przetworzyc je w kolejnym wywolaniu
	*/
	std::vector<unsigned char> rest;
	
	/**
	* stala c funkcji keccak
	*/
	unsigned c;
	/**
	* stala rate algorytmu
	*/
	unsigned r;
	/**
	* dlugosc wyniku funkcji skrotu
	*/
	unsigned d;
	unsigned rate;

	/**
	* funkcja odpowiedzialna za rotacje bitowa podanej liczby o zadana liczbe bitow w lewo
	* @param liczba do zmodyfikowania
	* @param liczba miejsc o ile przesunac bity
	* @return liczba po rotacji
	*/
	inline uint64_t rotl64(uint64_t n, int c);
	/**
	* funkcja odpowiedzialna za rotacje bitowa podanej liczby o zadana liczbe bitow w prawo
	* @param liczba do zmodyfikowania
	* @param liczba miejsc o ile przesunac bity
	* @return liczba po rotacji
	*/
	inline uint64_t rotr64(uint64_t n, int c);

	/**
	* funkcja tworzaca tablice stanu na podstawie podanej sekwencji bajtow
	* @param tablica bajtow
	*/
	void convertStringToStateArray(const std::vector<unsigned char>& str);
	/**
	* funkcja rekonstruujaca sekwencje bajtow na podstawie tablicy stanu
	* @return tablica bajtow
	*/
	std::vector<unsigned char> convertStateArrayToString();

	/**
	* funkcja odpowiedzialna za rotacje bitowa podanej liczby o zadana liczbe bitow w lewo
	*/
	void keccakPermutation();
	/**
	* funkcja miesza odpowiednie bity ze sob¹
	*/
	void keccakTheta();
	/**
	* funkcja stosuje rotacje bitowe
	*/
	void keccakRho();
	/**
	* funkcja przestawia bity
	*/
	void keccakPi();
	/**
	* funkcja miesza bity
	*/
	void keccakChi();
	/**
	* funkcja przekszta³ca niektóre bity le¿¹ce w Lane[0,0] o wartoœæ zale¿¹c¹ od rundy
	* @param numer rundy
	*/
	void keccakJota(unsigned int round);
	
	/**
	* funkcja wywolujaca sekwencje funkcji theta, rho, pi, chi oraz jota
	* @param numer rundy
	*/
	void rnd(unsigned int round);

	/**
	* funkcja sponge odpowiada za przygotowanie danej sekwencji bajtow do przetwarzania oraz wykonanie odpowiedniej liczby rund algorytmu
	* @param numer rundy
	*/
	void sponge(const std::vector<unsigned char>& m);
	/**
	* funkcja wywolujaca odpowiednia funcje algorytmu - funkcje sponge
	* @param sekwencja bajtow
	*/
	void keccak(const std::vector<unsigned char>& m);

public:
	sha3();
	/**
	 * Konstruktor kontekstu algorytmu
	 * @param dlugosc skrotu
	 */
	sha3(int size);
	~sha3();

	/**
	* Funkcja wykonujaca odpowiedni kroki algorytmu SHA3 dla podanych bajtow, jednoczesnie oczekujac na kolejne
	* @param lancuch bajtow
	*/
	void update(const std::vector<unsigned char>& data);
	/**
	* Funkcja wykonujaca kolejne kroki algorytmu funkcji skrotu SHA3 dla ostanich bajtow pliku
	* @param ostatni lancuch bajtow
	* @return ostateczny wynik - skrot
	*/
	std::string final(const std::vector<unsigned char>& data);
	/**
	* Funkcja wykonujaca obliczajaca skrot SHA3 podanego ciagu bajtow
	* @param lancuch bajtow
	* @return wynik funkcji skrotu
	*/
	virtual std::string compute(const std::vector<unsigned char>& data);

};

#endif