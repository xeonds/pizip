#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <cxxopts.hpp>

const int GROUP_SIZE = 32;  // Number of bits in each group

// Function to convert a decimal to binary (as a string)
std::string decimalToBinary(int decimal, int numBits) {
    std::string binary;
    for (int i = 0; i < numBits; i++) {
        binary.insert(0, std::to_string(decimal % 2));
        decimal /= 2;
    }
    return binary;
}

// Function to compress a file
void compressFile(const std::string& inputFile, const std::string& outputFile) {
    // Step 1: Read the input file
    std::ifstream inFile(inputFile, std::ios::binary);
    std::vector<char> fileData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    // Step 2: Convert the file to bits
    std::string bits;
    for (char byte : fileData) {
        bits += decimalToBinary(static_cast<int>(byte), 8);
    }

    // Step 3: Split bits into groups
    std::vector<std::string> bitGroups;
    for (size_t i = 0; i < bits.length(); i += GROUP_SIZE) {
        bitGroups.push_back(bits.substr(i, GROUP_SIZE));
    }

    // Step 4: Find where the group appeared in Pi (simplified, not accurate)
    std::vector<int> groupOffsets;
    for (const std::string& group : bitGroups) {
        // Simplified: Just use the group as an offset (not actual Pi)
        groupOffsets.push_back(std::stoi(group, nullptr, 2));
    }

    // Step 5: Log the offset and group length as compressed data
    std::ofstream outFile(outputFile, std::ios::binary);
    for (size_t i = 0; i < bitGroups.size(); i++) {
        outFile.write(reinterpret_cast<const char*>(&groupOffsets[i]), sizeof(int));
        outFile.write(bitGroups[i].c_str(), bitGroups[i].length());
    }
    outFile.close();
}

// Function to decompress a file
void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open input file." << std::endl;
        return;
    }

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open output file." << std::endl;
        inFile.close();
        return;
    }

    int offset;
    std::string bits;

    while (inFile.read(reinterpret_cast<char*>(&offset), sizeof(int))) {
        char buffer[GROUP_SIZE + 1];
        inFile.read(buffer, GROUP_SIZE);
        buffer[GROUP_SIZE] = '\0';
        bits += buffer;
        // Decompressing logic: Convert offset back to binary and append it to the bits
        bits += decimalToBinary(offset, GROUP_SIZE);
    }

    // Convert bits back to bytes and write to the output file
    for (size_t i = 0; i < bits.length(); i += 8) {
        std::string byteStr = bits.substr(i, 8);
        char byte = static_cast<char>(std::stoi(byteStr, nullptr, 2));
        outFile.write(&byte, 1);
    }

    inFile.close();
    outFile.close();
}

int main(int argc, char ** argv) {
    cxxopts::Options options("FileCompressor", "A simple file compression tool");
    options.add_options()
        ("v,view", "View the file tree", cxxopts::value<bool>())
        ("x,extract", "Extract files", cxxopts::value<bool>())
        ("c,compress", "Compress files", cxxopts::value<bool>())
        ("i,input", "Input file/directory", cxxopts::value<std::string>())
        ("o,output", "Output file/directory", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    if (result["view"].as<bool>()) {
        std::cout << "Testing..." << std::endl;
    } else if (result["extract"].as<bool>()) {
        std::string inputFile = result["input"].as<std::string>();
        std::string outputFile = result["output"].as<std::string>();
        decompressFile(inputFile, outputFile);
        std::cout << "Extraction complete." << std::endl;
    } else if (result["compress"].as<bool>()) {
        std::string inputFile = result["input"].as<std::string>();
        std::string outputFile = result["output"].as<std::string>();
        compressFile(inputFile, outputFile);
        std::cout << "Compression complete." << std::endl;
    } else {
        std::cerr << "Please specify an operation: -v (view), -x (extract), or -c (compress)." << std::endl;
        return 1;
    }

    return 0;
}

