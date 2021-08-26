//
// Created by vincent on 24/08/2021.
//

#ifndef JAST_JAST_H
#define JAST_JAST_H

#include <iostream>
#include <fstream>
#include "ext/json.hpp"
#include "FileIO.h"
#include "algorithm"

////ça vient de ton code
enum ConnectionFormat {
    FORWARD,
    BACKWARD
};

////Ca aussi mais j'ai enlevé des paramètres qui m'était pas utiles
struct FileDescription {
    std::string dataPath;
    size_t nbInputs, nbNeurons;
    unsigned int connectionsPerNeuron;
};

void to_json(nlohmann::json& j, const FileDescription& fd);

void from_json(const nlohmann::json& j, FileDescription& fd);


////je crée une classe d'apprentissage qui va garder en paramètre les informations des headers fichiers, et va
//// avoir toutes les méthodes qu'il faut pour faire l'apprentissage.
class JAST{
public:
    JAST(const std::string& backwardJsonPath, const std::string& forwardJsonPath){
        nlohmann::json metadata1,metadata2;
        std::fstream file1,file2;
        file1.open(backwardJsonPath, std::ios::in);
        file2.open(forwardJsonPath, std::ios::in);
        if (file1.fail()){
            throw std::logic_error("Could not open \"" + backwardJsonPath + "\" for reading");
        }
        if (file2.fail()){
            throw std::logic_error("Could not open \"" + forwardJsonPath + "\" for reading");
        }
        file1 >> metadata1;
        file2 >> metadata2;
        file1.close();
        file2.close();

        FileDescription fd1 = metadata1;
        FileDescription fd2 = metadata2;
        nb_inputs = fd1.nbInputs;
        nb_neurons = fd1.nbNeurons;
        connectionsPerNeuron = fd1.connectionsPerNeuron;
        backwardDataPath = fd1.dataPath;
        forwardDataPath = fd2.dataPath;
    }
    ////Les deux prochaines fonctions permettent de lire/ecrire toutes les connections pour un neurone spécifique à partir/dans un
    ////fichier binaires. Elles utilisent deux fonctions présentes dans FileIO qui peuvent faire la même chose mais
    ////connexions par connexions.

    std::vector<unsigned int> readConnectionsForNeuron(std::fstream& inFile, const unsigned int& neuron) const;

    bool writeConnectionsForNeuron(std::fstream& file, const unsigned int& neuron, std::vector<unsigned int> neuronConnections);

    ////Ici ce sont tes fonctions, mais qui ne marchent pas du tout pareil. J'ai juste repris le nom.
    unsigned int chooseUnusedConnection(const std::vector<unsigned int>& inputBlock,const std::vector<unsigned int>& neuronConnection) const;

    unsigned int chooseNewConnection(const std::vector<unsigned int>& inputBlock, const std::vector<unsigned int>& neuronConnections);

    void learn(const std::vector<unsigned int>& inputBlock, const std::vector<unsigned int>& learningNeurons);

private :
    size_t nb_inputs, nb_neurons;
    unsigned int connectionsPerNeuron;
    std::string backwardDataPath;
    std::string forwardDataPath;
};

#endif //JAST_JAST_H
