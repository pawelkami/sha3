#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>
#include <fstream>

#define HEX2DIGIT(c) ((c) <= '9' ? (c) & 0xF : ((c) - 'a' + 10) & 0xF)

/**
* Funkcja konwertuje lancuch znakow do lancucha heksadecymalnego
* @param lancuch znakow
* @return lancuch w zapisie heksadecymalnym
*/
std::string bin2hex(const std::vector<unsigned char>& bin);

/**
* Funkcja konwertuje lancuch heksadecymalny
* @param lancuch heksadecymalny
* @return tablica bajtow
*/
std::vector<unsigned char> hex2bin(std::string hex);

/**
* Funkcja sprawdza czy podany string to lancuch w zapisie heksadecymalnym
* @param lancuch znakow
* @return true - zapis heksadecymalny, w p.p. false
*/
bool checkIfHexString(const std::string& hex);

/**
* Funkcja czytajaca podany plik w calosci
* @param sciezka do pliku
* @return tablica bajtow
*/
std::vector<unsigned char> readFile(const std::string& filepath);

/**
* Funkcja czytajaca konkertna liczbe bajtow podanego pliku poczynajac od zadanej pozycji
* @param sciezka do pliku
* @param offset
* @param liczba bitow do przeczytania
* @return tablica bajtow
*/
std::vector<unsigned char> readFile(const std::string& filepath, unsigned from, unsigned bytesToRead);

/**
* Funkcja zwraca rozmiar podanego pliku
* @param sciezka do pliku
* @return rozmiar pliku
*/
long getFileSize(const std::string& filename);

/**
* Funkcja sprawdza czy podany plik istnieje
* @param sciezka do pliku
* @return true - istnieje, w p.p. false
*/
bool doFileExist(const std::string& filepath);


#endif