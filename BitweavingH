#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <chrono>

class BitWeavingH {
private:
    std::vector<uint64_t> data; // Storage for data, considering 64-bit words
    int bitsPerValue; // Bits per value, not including the delimiter
    int valuesPerWord; // How many values fit in a 64-bit word
    size_t currentValueIndex; // Tracks the current index for the next value insertion

public:
    BitWeavingH(int bitsPerValue)
            : bitsPerValue(bitsPerValue),
              valuesPerWord(64 / (bitsPerValue + 1)), // +1 for the delimiter bit
              currentValueIndex(0) {
        data.resize(1, 0); // Initialize with a single word
    }

    void readValuesFromFile(const std::string &filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (getline(inFile, line)) {
            // Convert binary string to a uint32_t integer
            uint32_t value = std::bitset<32>(line).to_ulong();
            addValue(value);
        }
        inFile.close();
    }

    void addValue(uint32_t value) {
        int wordIndex = currentValueIndex / valuesPerWord;
        int bitOffset = (currentValueIndex % valuesPerWord) * (bitsPerValue + 1);

        if (wordIndex >= data.size()) {
            data.push_back(0);
        }

        // Mask the input value to fit
        uint64_t maskedValue = value & ((1ULL << bitsPerValue) - 1);
        data[wordIndex] |= (maskedValue << bitOffset);

        currentValueIndex++;
    }

    void scanLessThan(uint64_t threshold) {
        for (size_t i = 0; i < currentValueIndex; ++i) {
            int wordIndex = i / valuesPerWord;
            int bitOffset = (i % valuesPerWord) * (bitsPerValue + 1);
            uint64_t value = (data[wordIndex] >> bitOffset) & ((1ULL << bitsPerValue) - 1);

            if (value < threshold) {
                std::cout << "Value " << value << " at position " << i << " is less than " << threshold << std::endl;
            }
        }
    }
};

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();

    BitWeavingH bwh(32); // 32 bits per value

    bwh.readValuesFromFile("/Users/yuliang/CLionProjects/databaseBitweavingH/donations-compressed-reg-int"); 
    bwh.scanLessThan(1000); // threshold

    // End the timer
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Print the duration
    std::cout << "Overall time taken: " << duration << " milliseconds" << std::endl;

    return 0;
}
