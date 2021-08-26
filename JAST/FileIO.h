//
// Created by pierre on 10/05/2021.
//

#ifndef SNN_PROTO_FILEIO_H
#define SNN_PROTO_FILEIO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

//#include "VariableLengthBuffers.h"

////ici j'ai enlevé l'unclusion des VariableLengthBuffer parce que je les utilise pas. Y'a beaucoup de fonctions que j'utilise pas
////non plus ou que j'ai modifié. A voir dans le .cpp, j'ai plus expliqué.


    /**
     * Transforms a text file to a series of spikes ranging from 0 to 25.
     * @param s Text file stream
     * @return Vector of spikes corresponding to the alphabetical characters in the file
     */
    std::vector<unsigned int> textToSpikes(std::fstream& s);

    /**
     * Transforms a string to a series of spikes ranging from 0 to 25.
     * @param s Text string
     * @return Vector of spikes corresponding to the alphabetical characters in the string
     */
    std::vector<unsigned int> textToSpikes(const std::string& s);
    
    enum SpikeFileFormat {
        NUMERICAL,
        ALPHABETICAL,
        BINARY
    };
    
    template<typename T>
    inline std::vector<T> readVector(std::fstream& s) {
        std::vector<unsigned char> bytes(std::istreambuf_iterator<char>(s), {});
        std::vector<T> typedVector = std::vector<unsigned int>(bytes.size() / sizeof(T));
        memcpy(typedVector.data(), bytes.data(), typedVector.size() * sizeof(T));
        return typedVector;
    }
    
    template<typename T>
    inline std::vector<T> readVector(std::fstream& s, size_t vectorSize) {
        std::vector<T> data(vectorSize);
        s.read(reinterpret_cast<char*>(data.data()), vectorSize * sizeof(T));
        if (s.fail())
            throw std::logic_error("Error while reading " + std::to_string(vectorSize * sizeof(T))
                                   + " bytes from stream");
        return data;
    }
    
    inline void writeChunked(std::fstream& s, const char* data, long size, long maxChunkSize = 0) {
        if (maxChunkSize == 0) {
            s.write(data, size);
        } else {
            long written = 0;
            while (written < size) {
                long left = size - written;
                long toWrite = std::min(maxChunkSize, left);
                s.write(data + written, toWrite);
                written += toWrite;
            }
        }
    }
    
    template<typename T>
    inline void writeVector(const std::vector<T>& vec, std::fstream& s, long maxChunkSize = 0) {
        writeChunked(s, reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(T), maxChunkSize);
    }

    /*
    template<typename T>
    inline void writeVariableLengthVectors(const VariableLengthVectors<T>& vvec, std::fstream& s, long maxChunkSize = 0) {
        writeChunked(s, reinterpret_cast<const char*>(vvec.indexData()), (vvec.size() + 1) * sizeof(size_t), maxChunkSize);
        writeChunked(s, reinterpret_cast<const char*>(vvec.data()), vvec.totalSize() * sizeof(T), maxChunkSize);
    }
     */
    
    std::vector<unsigned int> readFile(const std::string& path);

    unsigned int readConnectionInBinary(std::fstream& inFile, const unsigned int& pos);

    bool writeConnectionInBinary(std::fstream& inFile, const unsigned int& pos, const unsigned int& connection_);

#endif //SNN_PROTO_FILEIO_H
