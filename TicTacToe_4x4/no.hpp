#ifndef NO_HPP
#define NO_HPP

#include <vector>

struct No {
    int pontuacao_O = 0;
    int pontuacao_X = 0;
    int pontuacao_V = 0;

    char Marca = ' ';          // marca desse estado , no caso seria o filho desse estado tera uma marca oposta
    signed char posiMarca = 0; // seria para armazenar apenas a posição que a marca foi feita por ultimo num 4x4 seria de 0 a 15

    char estadoJogo = ' '; // caso seja estado final deverá ter : v ou x ou o

    No* pai = nullptr;
    std::vector<No*> filhos;

    No(No* pai = nullptr);
    ~No();
};

// Declaração da função
void deletaNo(No* no);

#endif