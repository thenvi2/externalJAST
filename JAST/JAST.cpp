//
// Created by vincent on 24/08/2021.
//

#include "JAST.h"

////version réduite de ta fonction to_json
void to_json(nlohmann::json& j, const FileDescription& fd)  {
    j = nlohmann::json {
        {"data_path", fd.dataPath},
        {"network", {
            {"nb_inputs", fd.nbInputs},
            {"nb_neurons", fd.nbNeurons},
            {"connections_per_neuron", fd.connectionsPerNeuron}
        }
        }
    };
}
////version réduite de ta fonction from_jason
void from_json(const nlohmann::json& j, FileDescription& fd) {
    j.at("data_path").get_to(fd.dataPath);
    j.at("network").at("nb_inputs").get_to(fd.nbInputs);
    j.at("network").at("nb_neurons").get_to(fd.nbNeurons);
    j.at("network").at("connections_per_neuron").get_to(fd.connectionsPerNeuron);
}

////fonction a appelé quand on veut réaliser l'apprentissage. Prend en paramètre deux vecteurs, soit les spikes en entrée,
////et les neurones ayant dépassés leur seuil d'apprentissage. Les autres fichiers sont passés en tant que membres de la classe
////JAST.
void JAST::learn(const std::vector<unsigned int>& inputBlock, const std::vector<unsigned int>& learningNeurons) {
    std::fstream file;
    ////Ici, à chaque fois je charge depuis le fichier des connections backward,
    //// les connections d'un neurone en particulier, pour n'avoir que connectionsPerNeuron à conserver en RAM.
    std::vector<unsigned int> neuronConnections;
    file.open(backwardDataPath, std::ios::in | std::ios::out | std::ios::binary);
    if(file.fail()){
        throw std::logic_error("Could not open \"" + backwardDataPath + "\" for reading");
    }

    for(auto& neuron : learningNeurons){
        ////je prend en mémoire toutes les connexions du neurone en question, que je met dans un vecteur sur lequel je vais
        ////travailler.
        neuronConnections = readConnectionsForNeuron(file,neuron);
        ////Ici, la comparaison avec 0 met permet de savoir si il y a un une connection à modifier. Si une de ces fonctions vaut zero,
        ////cela vaut dire que soit toutes les connections recoivent un spike, ou bien qu'il n'y a pas de nouvelles connections à trouver.
        ////En fait, chooseUnusedConnection et chooseNewConnections renvoient des index.
        if(!(chooseUnusedConnection(inputBlock,neuronConnections) == 0 || chooseNewConnection(inputBlock,neuronConnections) == 0)){
            neuronConnections[chooseUnusedConnection(inputBlock,neuronConnections)] = inputBlock[chooseNewConnection(inputBlock,neuronConnections)];
            if(!(writeConnectionsForNeuron(file,neuron,neuronConnections))) {
                std::cout << "Could not modify connection file" << std::endl;
            }
        }
    }
    ////Il reste encore à incrémenter le seuil d'apprentissage des neurones, mais je peux pas le faire la
    ////et à actualiser les forward connections
    file.close();
}

////expliquer dans le .h
std::vector<unsigned int> JAST::readConnectionsForNeuron(std::fstream& inFile, const unsigned int& neuron) const{
    std::vector<unsigned int> connectionList;
    for(unsigned int i = neuron*connectionsPerNeuron; i < (neuron+1)*connectionsPerNeuron; i++){
        connectionList.emplace_back(readConnectionInBinary(inFile,i));
    }
    return connectionList;
}

////expliquer dans le .h
bool JAST::writeConnectionsForNeuron(std::fstream& file, const unsigned int& neuron, std::vector<unsigned int> neuronConnections) {
    for(unsigned int i = 0; i < connectionsPerNeuron; i++){
        if(!(writeConnectionInBinary(file,(neuron*connectionsPerNeuron)+i,neuronConnections[i]))){
            return false;
        }
    }
    return true;
}

////voir .h
unsigned int JAST::chooseUnusedConnection(const std::vector<unsigned int>& inputBlock, const std::vector<unsigned int>& neuronConnection) const{
    ////je met l'index à 0, et s'il n'est pas modifié c'est qu'aucune connection était inutilisée (au cas ou un neurone dépasserais
    ////son seuil d'apprentissage alors que toutes ces connections ont reçu des spikes).
    unsigned int indexUnusedConnection = 0;
    for(unsigned int i = 0; i <= connectionsPerNeuron; i++){
        ////le if renvoie true si une connection du neurone n'est pas trouvée dans la liste des spikes du block.
        if(!(std::find(inputBlock.begin(), inputBlock.end(),neuronConnection[i]) != inputBlock.end())) {
            indexUnusedConnection = i;
            break;
        }else{
            continue;
        }
    }
    return indexUnusedConnection;
}

////voir .h
unsigned int JAST::chooseNewConnection(const std::vector<unsigned int>& inputBlock, const std::vector<unsigned int>& neuronConnections){
    ////même principe que fonction en haut.
    unsigned int indexNewConnection = 0;
    for(unsigned int i = 0; i < inputBlock.size(); i++) {
        if (!(std::find(neuronConnections.begin(), neuronConnections.end(), inputBlock[i]) != neuronConnections.end())) {
            indexNewConnection = i;
            break;
        }else{
            continue;
        }
    }
    return indexNewConnection;
}



