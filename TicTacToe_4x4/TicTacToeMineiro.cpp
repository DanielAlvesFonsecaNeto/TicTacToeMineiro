#include "Funcoes.hpp"

class TicTac {
    private:
        No* raiz;
        std::vector<std::vector<char>> estadoInicial;
        char SimboloInicial;
        int PosicaoInicial;
        int PesoXO;
        int PesoV;
    
    public:
        // Construtor com valores padrão para pesoXO (10) e pesoV (3)
        TicTac(const std::vector<std::vector<char>>& estado, char simboloInicial, int posicaoInicial, int pesoXO = 10, int pesoV = 3)
            :  estadoInicial(estado), SimboloInicial(simboloInicial), PosicaoInicial(posicaoInicial), PesoXO(pesoXO), PesoV(pesoV) {
                
                raiz = new No;
            }
    
        void playerVsIa(bool PlayerPrimeiro) {
            JogadorVsIa(raiz, estadoInicial, PosicaoInicial, SimboloInicial, PlayerPrimeiro, PesoXO, PesoV);
        }


        void IaVsIa() {
            IAvsIA(raiz, estadoInicial, PosicaoInicial, SimboloInicial,PesoXO, PesoV);
        }
    
        // Destrutor
        ~TicTac() {
            deletaNo(raiz);
        }
};
