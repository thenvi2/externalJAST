#include <iostream>
#include "JAST.h"

std::basic_string<char> backwardConnectionFile = "/home/vincent/CLionProjects/JAST/Ressources/backwardConnection1.json";
std::basic_string<char> forwardConnectionFile = "/home/vincent/CLionProjects/JAST/Ressources/forwardConnection1.json";

std::basic_string<char> inputBlockFile = "/home/vincent/CLionProjects/JAST/Ressources/incomingSpikeExample";
std::basic_string<char> learningNeuronsFile = "/home/vincent/CLionProjects/JAST/Ressources/learningNeuronsExample";

std::vector<unsigned int> inputBlock, learningNeurons;

////Voilà une version non débuggé, non testé, du code de l'apprentissage. J'ai repris beaucoup de tes fonctions,
////tout en les modifiants (les signatures sont différentes car pas la même nature des données), donc bon courage pour
////mettre tout ça en ordre (désolé j'ai pas eu le temps et j'ai commencé le programme sans savoir qu'il allait être incorporé
////au tien). Je peux donc appliquer JAST en prenant en argument les fichiers des spikes du block traité (format txt),
////des neurones ayant dépassé leur seuil d'apprentissage (format txt), les connections backward, et les connections forward.
////Pour l'instant les connections forward servent à rien.
////Je sais pas comment les modifier. En sortie du programme, les connections backward sont actualisées.
////Ce que j'ai principalement fait, c'est faire les fonctions pour lire/écrire les connections spécifiques dans les fichiers
////binaires, et que j'ai donc placé dans la bibliothèqe FileIO. J'ai aussi changé l'algorithme de JAST. Je t'explique plus en détail
////plus tard. Il manque la gestion d'erreurs aussi...


int main() {
    inputBlock = readFile(inputBlockFile);
    learningNeurons = readFile(learningNeuronsFile);
    ////Je charge la liste des spikes en entrée ainsi que les neurones ayant dépassés leur seuil d'apprentissage en mémoire
    ////parce que c'est pas ce qui en demande le plus (de mémoire).

    JAST j(backwardConnectionFile,forwardConnectionFile);
    j.learn(inputBlock,learningNeurons);
    return 0;
}


