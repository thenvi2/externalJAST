//
// Created by pierre on 18/06/2021.
//

#include "FileIO.h"

////ici, j'ai principalement repris ta bibliothèque mais j'ai enlevé et rajouté des trucs... désolé si c'est confus

////Ca je l'utilise pas
std::vector<unsigned int> textToSpikes(std::fstream& s) {
    std::vector<unsigned int> out;

    char c;
    while (s.get(c)) {
        if (c >= 'a' && c <= 'z') {
            out.emplace_back(c - 'a');
        } else if (c >= 'A' && c <= 'Z') {
            out.emplace_back(c - 'A');
        }
    }

    return out;
}

////ca je l'utilise pas
std::vector<unsigned int> textToSpikes(const std::string& s) {
    std::vector<unsigned int> out;

    for (const char& c : s) {
        if (c >= 'a' && c <= 'z') {
            out.emplace_back(c - 'a');
        } else if (c >= 'A' && c <= 'Z') {
            out.emplace_back(c - 'A');
        }
    }
    return out;
}

////Ca j'ai reppris ta fonction mais que le cas ou le contenu du fichier était numérique.
std::vector<unsigned int> readFile(const std::string& path) {
    std::vector<unsigned int> content;
    std::fstream inFile;
    inFile.open(path, std::ios::in);
    if (!inFile) {
        std::cerr << "The file \"" << path << "\" does not exist or cannot be opened.";
        exit(-5);
    }
    unsigned int readContent;
    while (inFile >> readContent)
        content.emplace_back(readContent);
    inFile.close();
    return content;
}

////Cette fonction lit une connection dans un fichier binaire selon la position demandée.
unsigned int readConnectionInBinary(std::fstream& inFile, const unsigned int& pos) {
    if (inFile.is_open()) {
        uint32_t connection;
        inFile.seekg(sizeof(connection) * pos);
        inFile.get((char *) &connection, sizeof(connection));
        return connection;
    } else {
        return -1;
    }
}

////Cette fonction écrit une connection spécifiée dans un fichier binaire selon la position demandée.
bool writeConnectionInBinary(std::fstream& outFile, const unsigned int& pos, const unsigned int& connection){
    if(outFile.is_open()){
        outFile.seekp(sizeof(connection)*pos);
        outFile.write(reinterpret_cast<const char *>(&connection),sizeof(connection));
        return true;
    } else {
        return false;
    }
}

