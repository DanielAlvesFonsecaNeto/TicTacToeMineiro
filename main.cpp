#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <queue>

//---------- Grafo -------------//
struct No
{
    //std::vector<std::vector<char>> estado;

    int pontuacao_O = 0;
    int pontuacao_X = 0;

    char Marca = ' '; // marca desse estado , no caso seria o filho desse estado tera uma marca oposta
    signed char posiMarca = 0;  // seria para armazenar apenas a posição que a marca foi feita por ultimo num 4x4 seria de 0 a 15

    char estadoJogo = ' '; // cajo seja estado final deverá ter : v ou x ou o

    No *pai = nullptr;
    std::vector<No *> filhos;

    // Construtor
    No(No *pai = nullptr);

    ~No(); // Destrutor
};

class Grafo
{
private:
    No *raiz;

public:
    Grafo(No *RAIZ);

    No *referenciaRaiz();

    void addFilho(No *pai, No *filho);

    void reconstruir(No *NOVA_RAIZ);

    void deletaGrafo();

    void deletaNo(No *no);

    void deletaFilhosDoNo(No *no);

    ~Grafo();
};

// Construtor do No
No::No(No *pai)
    : pai(pai) {}

// Destrutor do No
No::~No()
{
    for (No *filho : filhos)
    {
        delete filho; // Deleta recursivamente os filhos
    }
}

// Implementação do construtor da classe Grafo
Grafo::Grafo(No *RAIZ) : raiz(RAIZ) {}

// Implementação da função referenciaRaiz
No *Grafo::referenciaRaiz()
{
    return raiz;
}

void Grafo::addFilho(No *pai, No *filho)
{
    if (pai != nullptr && filho != nullptr)
    {
        pai->filhos.push_back(filho);
        filho->pai = pai;
    }
}

void Grafo::reconstruir(No *NOVA_RAIZ)
{
    if (raiz != nullptr)
    {
        delete raiz;
    }
    raiz = NOVA_RAIZ;
}

void Grafo::deletaGrafo()
{
    reconstruir(nullptr);
}

void Grafo::deletaNo(No *no)
{
    if (no == nullptr)
        return;

    if (no->pai != nullptr)
    {
        auto &filhosDoPai = no->pai->filhos;
        auto it = std::find(filhosDoPai.begin(), filhosDoPai.end(), no);
        if (it != filhosDoPai.end())
        {
            filhosDoPai.erase(it);
        }
    }

    delete no;
}

void Grafo::deletaFilhosDoNo(No *no)
{
    if (no == nullptr)
        return;

    for (No *filho : no->filhos)
    {
        delete filho;
    }
    no->filhos.clear();
}

// Destrutor da classe Grafo
Grafo::~Grafo()
{
    delete raiz;
}

//////////////////////////---------- ~Grafo~ -------------///////////////////////////

//---------- Funções -------------//

char avaliarEstado(const std::vector<std::vector<char>>& estado){
    /*
    ' ' = ninguem ganhou e não é velha
    'x' = x ganhou           --> estado final
    'o' = o bolinha ganhou   --> estado final
    'v' = velha              --> estado final
    */    

    // Verifica linhas e colunas
    for (int i = 0; i < 3; ++i) {
        // Linhas
        if (estado[i][0] != ' ' && estado[i][0] == estado[i][1] && estado[i][1] == estado[i][2])
            return estado[i][0];
        // Colunas
        if (estado[0][i] != ' ' && estado[0][i] == estado[1][i] && estado[1][i] == estado[2][i])
            return estado[0][i];
    }

    // Diagonal principal
    if (estado[0][0] != ' ' && estado[0][0] == estado[1][1] && estado[1][1] == estado[2][2])
        return estado[0][0];

    // Diagonal secundária
    if (estado[0][2] != ' ' && estado[0][2] == estado[1][1] && estado[1][1] == estado[2][0])
        return estado[0][2];

    // Verifica se ainda há jogadas possíveis
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (estado[i][j] == ' ')
                return ' '; // Jogo ainda em andamento

    // Caso nenhuma condição acima seja verdadeira, é velha
    return 'v';
}

// recria todo o estado do tabuleiro tictactoe com base no nó filho e a posi do seu simbolo até chegar no pai
std::vector<std::vector<char>> recriarEstadoPorPosiSimb(const std::vector<std::vector<char>>& estadoInicial, No* no){
    
    std::vector<std::vector<char>> reconstroiEstado = estadoInicial;
    

    No *atual = no;
    
    while(atual != nullptr){

        int linha = atual->posiMarca / 3;
        int coluna = atual->posiMarca % 3;
        reconstroiEstado[linha][coluna] = atual->Marca;

        atual = atual->pai;
    }

    return reconstroiEstado;
}

/**
 * estado incial pode ser um tabuleiro ja preenchido com algumas posições
 * simbolo inicial seria o simbolo X ou O que jogou nesse estado inicial 
 * essa arvore é gerado usando logica de geração em largura com FILA
 */
void gerarArvoreDecisao(const std::vector<std::vector<char>>& estadoInicial, int posiSimbolo, char simboloInicial){
    
    std::vector<std::vector<char>> estado = estadoInicial;

    No *raiz = new No();
    raiz->Marca = simboloInicial;
    raiz->posiMarca = posiSimbolo;
    

    Grafo arvore(raiz);

    std::queue<No*> fila;
    fila.push(raiz);

    while (!fila.empty()) {
        No* atual = fila.front();
        fila.pop();


        std::vector<std::vector<char>> estadoAtual = recriarEstadoPorPosiSimb(estadoInicial, atual);

        char estadoJogo = avaliarEstado(estadoAtual);
        atual->estadoJogo = estadoJogo;

        if (estadoJogo != ' ') {

            // Marcar pontuação
            if (estadoJogo == 'x') atual->pontuacao_X = 1;
            else if (estadoJogo == 'o') atual->pontuacao_O = 1;
            // empate 'v' poderia ter uma pontuacao neutra se eu quiser

            continue; // estado final, não tem filhos
        }

        // Alterna a marca
        char proximoSimbolo = (atual->Marca == 'x') ? 'o' : 'x';

        // gerar filhos 
        for (int i = 0; i < 9; ++i) {
            int linha = i / 3;
            int coluna = i % 3;
        
            if (estadoAtual[linha][coluna] == ' ') {
                No* filho = new No();
                filho->Marca = proximoSimbolo;
                filho->posiMarca = i;
                filho->pai = atual;
        
                arvore.addFilho(atual, filho);

                fila.push(filho);
            }
        } // fim do for que cria novos filhos
    } // fim do while




    /*
        OK ----- função de verificação de estado 
        OK ----- primeiro deve gerar a arvore baseado na raiz,
        depois fazer uma busca em profundidade ponderando todos os nós começando das folhas até a raiz
    */


}

//////////////////////////---------- ~Funções~ -------------///////////////////////////



int main()
{
    std::vector<std::vector<char>> estado = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}};

    No *raiz = new No();
    Grafo grafo(raiz);

    std::cout << "Grafo criado com sucesso.\n";

    return 0;
}

// preciso dessa funções ::
/**
 * função pra gerar estados filhos a partir do estado pai
 * função de avaliação pra dizer se o estado gerado é estado final
 * reestrturar os nós
 * 
 */