#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <chrono>
#include <limits>

class ByteSlice {
private:
    std::vector<uint64_t> data; // Storage for data, considering 64-bit words
    int bitsPerValue; // Bits per value

public:
    ByteSlice(int bitsPerValue)
            : bitsPerValue(bitsPerValue) {}

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
        int wordIndex = data.size();
        int bitOffset = 0;

        // Determine the number of words needed
        int wordsNeeded = (bitsPerValue + 63) / 64;

        // Add zero words if needed
        if (wordIndex + wordsNeeded > data.size()) {
            data.resize(wordIndex + wordsNeeded, 0);
        }

        // Mask the input value to fit the bitsPerValue and insert it
        uint64_t maskedValue = value & ((1ULL << bitsPerValue) - 1);
        data[wordIndex] |= (maskedValue << bitOffset);
    }

    void scanLessThan(uint64_t threshold) {
        for (size_t i = 0; i < data.size(); ++i) {
            uint64_t value = data[i];
            if (value < threshold) {
                std::cout << "Value " << value << " at position " << i << " is less than " << threshold << std::endl;
            }
        }
    }
};

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();

    ByteSlice bs(32); // Initialize with a large enough number of bits per value

    bs.readValuesFromFile("/Users/yuliang/CLionProjects/databaseByteSlice/donations-compressed-reg-int"); 
    bs.scanLessThan(1000); // Use the maximum value of uint32_t as the threshold

    // End the timer
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Print the duration
    std::cout << "Overall time taken: " << duration << " milliseconds" << std::endl;

    return 0;
}
