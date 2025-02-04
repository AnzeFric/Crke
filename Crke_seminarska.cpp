// Crke_seminarska.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <string>

std::string files[] = {"crke01.in", "crke02.in", "crke03.in", "crke04.in", "crke05.in", "crke06.in", "crke07.in" };

struct Pair {
    std::string first;
    std::string second;
};

int M;
std::vector<std::string> firstRow;
std::vector<std::string> secondRow;

void parseData(std::string filename) {
    std::ifstream inputFile(filename);

    inputFile >> M;

    std::string temp;
    for (int i = 0; i < M; i++) {
        inputFile >> temp;
        firstRow.push_back(temp);
    }

    for (int i = 0; i < M; i++) {
        inputFile >> temp;
        secondRow.push_back(temp);
    }

    inputFile.close();
}

bool isStringNumeric(const std::string& str) {
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

// Return found number and string or
// Strings containing random(for checking both rows if they are the same) or 
// Empty strings
Pair isColumnNumAndStr(int index, bool checkForEqualStr) {
    bool isFirstNum = isStringNumeric(firstRow[index]);
    bool isSecondNum = isStringNumeric(secondRow[index]);

    if (isFirstNum && !isSecondNum) {
        // Number and string
        return { firstRow[index], secondRow[index] };
    }
    else if (!isFirstNum && isSecondNum) {
        // String and number
        return { secondRow[index], firstRow[index] };
    }

    // aren't numbers and strings are different
    if (checkForEqualStr && (!isFirstNum && !isSecondNum) && firstRow[index] != secondRow[index]) {
        return { "random", "random" };
    }

    // (string and string) or (number and number)
    return { "", "" };
}

std::vector<Pair> bruteForce() {
    std::vector<Pair> result;

    // For whole row
    for (int i = 0; i < M; i++) {

        Pair temp = isColumnNumAndStr(i, false);
        if (temp.first != "" && temp.second != "") {
            // Find all matching numbers
            for (int j = 0; j < M; j++) {
                if (firstRow[j] == temp.first) {
                    firstRow[j] = temp.second;
                }
                else if (secondRow[j] == temp.first) {
                    secondRow[j] = temp.second;
                }
            }
            result.push_back(temp);

            i = -1; // Reset loop to 0, when one is found
        }
    }
    return result;
}

bool compareByFirst(const Pair& a, const Pair& b) {
    return std::stoi(a.first) < std::stoi(b.first);
}

void outputIsVectorEqualized(std::vector<Pair> vec) {
    // Open output file
    std::ofstream outputFile("out.txt", std::ios::binary);

    for (int i = 0; i < M; i++) {
        Pair temp = isColumnNumAndStr(i, true);
        if (temp.first != "" && temp.second != "") {
            outputFile << "NE" << std::endl;
            return;
        }
    }

    std::sort(vec.begin(), vec.end(), compareByFirst);

    outputFile << "DA" << std::endl;
    for (int i = 0; i < vec.size(); i++) {
        outputFile << vec[i].first << " " << vec[i].second << std::endl;
    }
    outputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];

    auto start = std::chrono::high_resolution_clock::now();
    parseData(inputFilename);

    std::vector<Pair> result = bruteForce();

    outputIsVectorEqualized(result);

    auto end = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "time: " << time / 1000 << std::endl;
    return 0;
}