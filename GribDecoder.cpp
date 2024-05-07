#include "GribDecoder.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>

GribDecoder::GribDecoder() {}

void GribDecoder::analyzeFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    decodeIndicatorSection(file);
    decodeProductDefinitionSection(file);
    decodeGridDescriptionSection(file);
    decodeBinaryDataSection(file);

    file.close();
}

void GribDecoder::decodeIndicatorSection(std::ifstream& file) {
    char indicator[5];
    file.seekg(0, std::ios::beg);
    file.read(indicator, 4);
    indicator[4] = '\0';

    std::cout << "Coding info: " << indicator << std::endl;
}

void GribDecoder::decodeProductDefinitionSection(std::ifstream& file) {
    file.seekg(8, std::ios::beg);
    int pdsLength = readInt(file, 3);

    std::cout << "\n======================\n==  Section 1 Data  ==\n======================\n";
    std::cout << "Section 1 length: " << pdsLength << std::endl;

    int paramTableVersion = readInt(file, 1);
    std::cout << "Parameter table Version: " << paramTableVersion << std::endl;

    int centerId = readInt(file, 2);
    std::string centerName = getCenterName(centerId);
    std::cout << "Identification of Centre: " << centerName << std::endl;

    int processId = readInt(file, 2);
    std::string processName = getProcessName(processId);
    std::cout << "Process ID number: " << processName << std::endl;

    int gridId = readInt(file, 1);
    std::cout << "Grid Identification: " << gridId << std::endl;


    int indicatorOfTypeLevel = readInt(file, 1);
    std::cout << "Indicator of type of level or layer: " << indicatorOfTypeLevel << std::endl;

    int levelValue = readInt(file, 2);
    std::cout << "Height, pressure, etc. of the level or layer: " << levelValue << std::endl;
}

void GribDecoder::decodeGridDescriptionSection(std::ifstream& file) {
    file.seekg(8, std::ios::beg);
    int pdsLength = readInt(file, 3);
    int gdsLength = readInt(file, 3);

    file.seekg(8 + pdsLength, std::ios::beg);
    std::cout << "\n======================\n==  Section 2 Data  ==\n======================\n";
    std::cout << "Section 2 length: " << gdsLength << std::endl;

    int numVerticalCoords = readInt(file, 1);
    std::cout << "Number of Vertical Coordinates: " << numVerticalCoords << std::endl;

    int dataRepType = readInt(file, 1);
    std::cout << "Data representation type: " << dataRepType << std::endl;

    int numPointsLatCircle = readInt(file, 2);
    std::cout << "Numbers of points along a latitude circle: " << numPointsLatCircle << std::endl;

    int numPointsLongMeridian = readInt(file, 2);
    std::cout << "Numbers of points along a longitude meridian: " << numPointsLongMeridian << std::endl;

    int latFirstPoint = readInt(file, 3);
    std::cout << "Latitude: " << latFirstPoint << " degrees" << std::endl;

    int longFirstPoint = readInt(file, 3);
    std::cout << "Longtitude: " << longFirstPoint << " degrees" << std::endl;

    int resolutionComponentFlags = readInt(file, 1);
    std::cout << "Resolution and component flags: " << resolutionComponentFlags << std::endl;

    int latLastPoint = readInt(file, 3);
    std::cout << "Latitude of last grid point: " << latLastPoint << " degrees" << std::endl;

    int longLastPoint = readInt(file, 3);
    std::cout << "Longitude of last grid point: " << longLastPoint << " degrees" << std::endl;

    int longDirectionIncrement = readInt(file, 3);
    std::cout << "Longitudinal Direction Increment: " << longDirectionIncrement << std::endl;

    int latDirectionIncrement = readInt(file, 3);
    std::cout << "Latitudinal Direction Increment: " << latDirectionIncrement << std::endl;

    int scanningModeFlags = readInt(file, 1);
    std::cout << "Scanning mode flags: ";
    if (scanningModeFlags == 0) {
        std::cout << "Points scan in +i and -j direction. Fortran:(I,J)";
    }
    else {
        std::cout << "Other scanning mode (update as needed)";
    }
    std::cout << std::endl;
    for (int i = 1; i <= 73; ++i) {
        int numPointsInRow = readInt(file, 2);
        std::cout << "Wiersz " << i << " Punktow: " << numPointsInRow << std::endl;
    }
}

void GribDecoder::decodeBinaryDataSection(std::ifstream& file) {
    file.seekg(8, std::ios::beg);
    int pdsLength = readInt(file, 3);
    int gdsLength = readInt(file, 3);
    int bdsLength = readInt(file, 3);

    file.seekg(8 + pdsLength + gdsLength, std::ios::beg); 
    std::cout << "\n======================\n==  Section 4 Data  ==\n======================\n";
    std::cout << "Section 4 length: " << bdsLength << std::endl;

    int flagToDecode = readInt(file, 1);
    std::cout << "Flag to decode: " << flagToDecode << std::endl;

    int binaryScaleFactor = readInt(file, 1);
    std::cout << "Binary Scale Factor: " << binaryScaleFactor << std::endl;
}

int GribDecoder::readInt(std::ifstream& file, int numBytes) {
    int value = 0;
    char byte;

    for (int i = 0; i < numBytes; ++i) {
        file.get(byte);
        value = (value << 8) | (unsigned char)byte;
    }

    return value;
}

std::string GribDecoder::readString(std::ifstream& file, int length) {
    std::vector<char> buffer(length + 1);
    file.read(buffer.data(), length);
    buffer[length] = '\0';

    return std::string(buffer.data());
}

std::string GribDecoder::getCenterName(int centerId) {
    switch (centerId) {
    case 7:
        return "US National Weather Service - NCEP (WMC)";
    default:
        return "Unknown";
    }
}

std::string GribDecoder::getProcessName(int processId) {
    switch (processId) {
    case 0:
        return "Analysis from GFS (Global Forecast System)";
    default:
        return "Unknown";
    }
}
