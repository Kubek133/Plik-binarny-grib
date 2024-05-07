#include "GribDecoder.h"

int main() {
    std::string filename = "all.grib";
    GribDecoder decoder;
    decoder.analyzeFile(filename);

    return 0;
}
