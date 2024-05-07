#ifndef GRIB_DECODER_H
#define GRIB_DECODER_H

#include <string>
#include <fstream>

class GribDecoder {
public:
    GribDecoder(); // Konstruktor

    void analyzeFile(const std::string& filename); // Metoda analizuj¹ca plik GRIB

private:
    void decodeIndicatorSection(std::ifstream& file); // Dekodowanie sekcji IS
    void decodeProductDefinitionSection(std::ifstream& file); // Dekodowanie sekcji PDS
    void decodeGridDescriptionSection(std::ifstream& file); // Dekodowanie sekcji GDS
    void decodeBinaryDataSection(std::ifstream& file); // Dekodowanie sekcji BDS

    int readInt(std::ifstream& file, int numBytes); // Odczytaj liczbê ca³kowit¹ z pliku binarnego
    std::string readString(std::ifstream& file, int length); // Odczytaj string z pliku binarnego

    std::string getCenterName(int centerId);
    std::string getProcessName(int processId);
};

#endif // GRIB_DECODER_H
