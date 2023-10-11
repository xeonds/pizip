#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// A function to read the first n digits of pi from a text file
string read_pi(int n) {
    ifstream fin("pi.txt"); // The file name may vary
    string pi;
    if (fin.is_open()) {
        getline(fin, pi); // Skip the first line
        pi.clear();
        while (n > 0 && getline(fin, pi)) {
            n -= pi.length();
        }
        fin.close();
    }
    return pi.substr(0, pi.length() + n);
}

// A function to convert a byte to a decimal string
string byte_to_decimal(unsigned char byte) {
    string decimal;
    int value = byte;
    while (value > 0) {
        decimal.push_back('0' + value % 10);
        value /= 10;
    }
    reverse(decimal.begin(), decimal.end());
    return decimal;
}

// A function to convert a decimal string to a byte
unsigned char decimal_to_byte(string decimal) {
    unsigned char byte = 0;
    int value = 0;
    for (char c : decimal) {
        value = value * 10 + (c - '0');
    }
    byte = value;
    return byte;
}

// A function to compress a file using pi
void compress(string input_file, string output_file) {
    ifstream fin(input_file, ios::binary); // Open the input file in binary mode
    ofstream fout(output_file); // Open the output file in text mode
    if (fin.is_open() && fout.is_open()) {
        fout << input_file << endl; // Write the input file name to the output file
        unsigned char buffer; // A buffer to store one byte
        string data; // A string to store the converted data
        int length = 0; // The length of the data
        int offset = -1; // The offset of the data in pi
        string pi; // A string to store the digits of pi
        int index = 0; // The index of the current digit in pi
        int digits = 1000; // The number of digits of pi to read at a time
        pi = read_pi(digits); // Read the first digits of pi
        while (fin.read((char*)&buffer, 1)) { // Read one byte from the input file
            data += byte_to_decimal(buffer); // Convert the byte to a decimal string and append it to the data
            length++; // Increase the length of the data by one
            if (offset == -1) { // If the offset is not found yet
                offset = pi.find(data); // Try to find the data in pi
                while (offset == -1) { // If the data is not found in pi
                    index += digits; // Increase the index by the number of digits
                    digits *= 2; // Double the number of digits to read
                    pi = read_pi(digits); // Read more digits of pi
                    offset = pi.find(data); // Try to find the data in pi again
                }
                offset += index; // Add the index to the offset
            }
            else { // If the offset is already found
                if (pi[offset + data.length()] != data.back()) { // If the next digit of pi does not match the last digit of the data
                    fout << length << " " << offset << endl; // Write the length and the offset to the output file
                    data.clear(); // Clear the data
                    length = 0; // Reset the length
                    offset = -1; // Reset the offset
                }
            }
        }
        if (!data.empty()) { // If there is some remaining data
            fout << length << " " << offset << endl; // Write the length and the offset to the output file
        }
        fin.close(); // Close the input file
        fout.close(); // Close the output file
    }
}

// A function to decompress a file using pi
void decompress(string input_file, string output_file) {
    ifstream fin(input_file); // Open the input file in text mode
    ofstream fout(output_file, ios::binary); // Open the output file in binary mode
    if (fin.is_open() && fout.is_open()) {
        string file_name; // A string to store the original file name
        getline(fin, file_name); // Read the original file name from the input file
        int length; // The length of the data
        int offset; // The offset of the data in pi
        string pi; // A string to store the digits of pi
        int index = 0; // The index of the current digit in pi
        int digits = 1000; // The number of digits of pi to read at a time
        pi = read_pi(digits); // Read the first digits of pi
        while (fin >> length >> offset) { // Read the length and the offset from the input file
            string data; // A string to store the data
            while (length > 0) { // While there is some remaining data
                if (offset >= index + digits) { // If the offset is beyond the current digits of pi
                    index += digits; // Increase the index by the number of digits
                    digits *= 2; // Double the number of digits to read
                    pi = read_pi(digits); // Read more digits of pi
                }
                else { // If the offset is within the current digits of pi
                    data.push_back(pi[offset - index]); // Append the corresponding digit of pi to the data
                    offset++; // Increase the offset by one
                    length--; // Decrease the length by one
                }
            }
            unsigned char buffer; // A buffer to store one byte
            for (int i = 0; i < data.length(); i += 3) { // For every three digits in the data
                buffer = decimal_to_byte(data.substr(i, 3)); // Convert them to a byte
                fout.write((char*)&buffer, 1); // Write the byte to the output file
            }
        }
        fin.close(); // Close the input file
        fout.close(); // Close the output file
    }
}

int main() {
    string input_file = "test.txt"; // The name of the file to be compressed, you can change it as you like
    string output_file = "compressed.txt"; // The name of the compressed file, you can change it as you like

    compress(input_file, output_file); // Compress the file using pi

    cout << "The file " << input_file << " has been compressed to " << output_file << endl;

    string decompressed_file = "decompressed.txt"; // The name of the decompressed file, you can change it as you like

    decompress(output_file, decompressed_file); // Decompress the file using pi

    cout << "The file " << output_file << " has been decompressed to " << decompressed_file << endl;

    return 0;
}

