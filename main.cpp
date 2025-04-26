#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

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

void avaliarPontuacoesDFS(No* raiz) {
    if (!raiz) return;

    std::stack<No*> pilha1;
    std::stack<No*> pilha2;

    pilha1.push(raiz);

    // Primeira fase: gerar ordem reversa pós-ordem
    while (!pilha1.empty()) {
        No* atual = pilha1.top();
        pilha1.pop();
        pilha2.push(atual);

        for (No* filho : atual->filhos) {
            pilha1.push(filho);
        }
    }

    // Segunda fase: processar de baixo pra cima
    while (!pilha2.empty()) {
        No* atual = pilha2.top();
        pilha2.pop();

        // Se for estado final, já tem pontuação
        if (atual->estadoJogo != ' ') continue;

        int somaX = 0;
        int somaO = 0;

        for (No* filho : atual->filhos) {
            somaX += filho->pontuacao_X;
            somaO += filho->pontuacao_O;
        }

        atual->pontuacao_X = somaX;
        atual->pontuacao_O = somaO;
    }
}


/**
 * estado incial pode ser um tabuleiro ja preenchido com algumas posições
 * simbolo inicial seria o simbolo X ou O que jogou nesse estado inicial 
 * essa arvore é gerado usando logica de geração em largura com FILA
 */
void gerarArvoreDecisao(const std::vector<std::vector<char>>& estadoInicial, int posiSimbolo, char simboloInicial){

    int contador = 0;
    
    std::vector<std::vector<char>> estado = estadoInicial;

    No *raiz = new No();
    raiz->Marca = simboloInicial;
    raiz->posiMarca = posiSimbolo;
    

    Grafo arvore(raiz);

    std::queue<No*> fila;
    fila.push(raiz);

    while (!fila.empty()) {

        contador ++;

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


    std::cout << "contador while BFS : "<< contador << "\n";

    // fazendo uma DFS para pontuar toda a arvore MinMax
    avaliarPontuacoesDFS(raiz);

    std::cout << "contador x raiz : "<< raiz->pontuacao_X << "\n";
    std::cout << "contador o raiz : "<< raiz->pontuacao_O << "\n";

}

//////////////////////////---------- ~Funções~ -------------///////////////////////////


//---------- MAIN -------------//

int main()
{
     // Tabuleiro inicial vazio
     std::vector<std::vector<char>> estadoInicial = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int posiInicial = 0;       // Posição 0 (primeira casa do tabuleiro)
    char simboloInicial = 'x'; // Quem começa

    gerarArvoreDecisao(estadoInicial, posiInicial, simboloInicial);

    std::cout << "Arvore gerada com sucesso!\n";
    std::cout << "A primeira jogada foi feita por '" << simboloInicial << "' na posicao " << posiInicial << ".\n";

    return 0;
}
