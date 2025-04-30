#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits> // acho que nao usei

int multiplicador_x = 10;
int multiplicador_o = 10;
int multiplicador_velha = 10;

//---------- Grafo -------------//
struct No
{
    // std::vector<std::vector<char>> estado;

    int pontuacao_O = 0;
    int pontuacao_X = 0;
    int pontuacao_V = 0;

    char Marca = ' ';          // marca desse estado , no caso seria o filho desse estado tera uma marca oposta
    signed char posiMarca = 0; // seria para armazenar apenas a posição que a marca foi feita por ultimo num 4x4 seria de 0 a 15

    // int posiMarca = 0;

    char estadoJogo = ' '; // caso seja estado final deverá ter : v ou x ou o

    No *pai = nullptr;
    std::vector<No *> filhos;

    // Construtor
    No(No *pai = nullptr);

    ~No(); // Destrutor
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

//////////////////////////---------- ~Grafo~ -------------///////////////////////////

//---------- Funções -------------//

char avaliarEstado(const std::vector<std::vector<char>> &estado)
{
    /*
    Retornos possíveis:
    'x' → vitória do X
    'o' → vitória do O
    'v' → empate (velha)
    ' ' → jogo em andamento
    */

    // Checa linhas e colunas
    for (int i = 0; i < 3; ++i)
    {
        // Checa linha i
        if (estado[i][0] != ' ' && estado[i][0] == estado[i][1] && estado[i][1] == estado[i][2])
            return estado[i][0];

        // Checa coluna i
        if (estado[0][i] != ' ' && estado[0][i] == estado[1][i] && estado[1][i] == estado[2][i])
            return estado[0][i];
    }

    // Checa diagonal principal
    if (estado[0][0] != ' ' && estado[0][0] == estado[1][1] && estado[1][1] == estado[2][2])
        return estado[0][0];

    // Checa diagonal secundária
    if (estado[0][2] != ' ' && estado[0][2] == estado[1][1] && estado[1][1] == estado[2][0])
        return estado[0][2];

    // Verifica se ainda há jogadas disponíveis
    for (const auto &linha : estado)
    {
        for (char celula : linha)
        {
            if (celula == ' ')
                return ' '; // Jogo ainda em andamento
        }
    }

    // Se não houve vencedor e não há espaços vazios: empate
    return 'v';
}

// recria todo o estado do tabuleiro tictactoe com base no nó filho e a posi do seu simbolo até chegar no pai
std::vector<std::vector<char>> recriarEstadoPorPosiSimb(const std::vector<std::vector<char>> &estadoInicial, No *no)
{

    std::vector<std::vector<char>> reconstroiEstado = estadoInicial;

    No *atual = no;

    while (atual != nullptr)
    {

        int linha = atual->posiMarca / 3;
        int coluna = atual->posiMarca % 3;
        reconstroiEstado[linha][coluna] = atual->Marca;

        atual = atual->pai;
    }

    return reconstroiEstado;
}

void avaliarPontuacoesDFS(No *raiz)
{
    if (!raiz)
        return;

    std::stack<No *> pilha1;
    std::stack<No *> pilha2;

    pilha1.push(raiz);

    // Primeira fase: gerar ordem reversa pós-ordem
    while (!pilha1.empty())
    {
        No *atual = pilha1.top();
        pilha1.pop();
        pilha2.push(atual);

        for (No *filho : atual->filhos)
        {
            pilha1.push(filho);
        }
    }

    // Segunda fase: processar de baixo pra cima
    while (!pilha2.empty())
    {
        No *atual = pilha2.top();
        pilha2.pop();

        // Se for estado final, já tem pontuação
        if (atual->estadoJogo != ' ')
            continue;

        int somaX = 0;
        int somaO = 0;
        int somaV = 0;

        for (No *filho : atual->filhos)
        {
            somaX += filho->pontuacao_X;
            somaO += filho->pontuacao_O;
            somaV += filho->pontuacao_V;
        }

        atual->pontuacao_X = somaX;
        atual->pontuacao_O = somaO;
        atual->pontuacao_V = somaV;
    }
}

void certificaRaiz(No *raiz, const std::vector<std::vector<char>> &estadoInicial, int posiSimbolo, char simboloInicial)
{
    raiz->Marca = simboloInicial;
    raiz->posiMarca = posiSimbolo;
}

/**
 * estado incial pode ser um tabuleiro ja preenchido com algumas posições
 * simbolo inicial seria o simbolo X ou O que jogou nesse estado inicial
 * essa arvore é gerado usando logica de geração em largura com FILA
 */
void gerarArvoreDecisao(No *raiz, const std::vector<std::vector<char>> &estadoInicial, int posiSimbolo, char simboloInicial)
{

    int contador = 1;

    std::vector<std::vector<char>> estado = estadoInicial;

    // No *raiz = new No();
    raiz->Marca = simboloInicial;
    raiz->posiMarca = posiSimbolo;

    // Grafo arvore(raiz); // apaga isso, deu merda por causa do destrutor dela

    std::queue<No *> fila;
    fila.push(raiz);

    while (!fila.empty())
    {

        No *atual = fila.front();
        fila.pop();

        std::vector<std::vector<char>> estadoAtual = recriarEstadoPorPosiSimb(estadoInicial, atual);

        char estadoJogo = avaliarEstado(estadoAtual);
        atual->estadoJogo = estadoJogo;

        if (estadoJogo != ' ')
        {
            /*
            // Marcar pontuação
            if (estadoJogo == 'x') atual->pontuacao_X = 1;
            else if (estadoJogo == 'o') atual->pontuacao_O = 1;
            // empate 'v' poderia ter uma pontuacao neutra se eu quiser
            */
            if (estadoJogo == 'x')
                atual->pontuacao_X = 1;
            else if (estadoJogo == 'o')
                atual->pontuacao_O = 1;
            else if (estadoJogo == 'v')
            {
                atual->pontuacao_V = 1;
            }
            continue; // estado final, não tem filhos
        }

        // Alterna a marca
        char proximoSimbolo = (atual->Marca == 'x') ? 'o' : 'x';

        // gerar filhos
        for (int i = 0; i < 9; ++i)
        {
            int linha = i / 3;
            int coluna = i % 3;

            if (estadoAtual[linha][coluna] == ' ')
            {
                No *filho = new No();
                filho->Marca = proximoSimbolo;
                filho->posiMarca = i;
                filho->pai = atual;

                atual->filhos.push_back(filho);

                fila.push(filho);

                contador++;
            }
        } // fim do for que cria novos filhos
    } // fim do while

    std::cout << "contador while BFS : " << contador << "\n";

    // fazendo uma DFS para pontuar toda a arvore MinMax
    avaliarPontuacoesDFS(raiz);

    std::cout << "contador x raiz : " << raiz->pontuacao_X << "\n";
    std::cout << "contador o raiz : " << raiz->pontuacao_O << "\n";
    std::cout << "contador velha raiz : " << raiz->pontuacao_V << "\n";
}

//////////////////////////---------- ~Funções~ -------------///////////////////////////

//---------- Funções IAs e Jogador -------------//


No *escolhaIA_x(No *noAtual)
{
    if (noAtual->filhos.empty())
        return nullptr;
    No *melhorFilho = nullptr;

    /*
        basicamente é aonde o MinMax vai atuar ,
        ele deve escolher o filho que dê mais chances dele ganhar ou mais chances do inimigo perder

            essa flag(primeiro) é pra saber se já entrou pelo menos 1 vez no for e basiar o valor inicial para encontrar o melhor
            ela é necessaria pois o valor inicial nao pode ser 0 ja que pode ter casos em quem nenhum filho é > 0, mas ainda
            precisariamos do melhor
    */
    int melhorValor = 0;
    bool primeiro = true;

    for (No *filho : noAtual->filhos)
    {
        if(filho->estadoJogo == 'x'){
            return filho;
        }

        int valor = (filho->pontuacao_X * multiplicador_x) - (filho->pontuacao_O * multiplicador_o);

        if(valor < 0){
            valor += filho->pontuacao_V * multiplicador_velha;
        }

        if (primeiro || valor > melhorValor)
        {
            melhorValor = valor;
            melhorFilho = filho;
            primeiro = false;
        }
    }

    return melhorFilho;
}

No *escolhaIA_o(No *noAtual)
{
    if (noAtual->filhos.empty())
        return nullptr;
    No *melhorFilho = nullptr;

    /*
        mesma coisa, apenas mudei o cainho da escolha querendo pegar o pior valor para o X ,"troque o sinal de '<' no if"
    */
    int melhorValor = 0;
    bool primeiro = true;

    for (No *filho : noAtual->filhos)
    {
        if(filho->estadoJogo == 'o'){
            return filho;
        }

        int valor = (filho->pontuacao_O * multiplicador_o) - (filho->pontuacao_X * multiplicador_x);

        if(valor < 0){
            valor += filho->pontuacao_V * multiplicador_velha;
        }

        if (primeiro || valor > melhorValor)
        {
            melhorValor = valor;
            melhorFilho = filho;
            primeiro = false;
        }
    }
    return melhorFilho;
}

void IAvsIA(No *raiz, const std::vector<std::vector<char>> estadoInicial, int posiInicial, char simboloInicial)
{

    gerarArvoreDecisao(raiz, estadoInicial, posiInicial, simboloInicial);

    No *atual = raiz;

    std::cout << "Simulando partida IA vs IA...\n";

    while (atual && atual->estadoJogo == ' ')
    {
        std::vector<std::vector<char>> estadoAtual = recriarEstadoPorPosiSimb(estadoInicial, atual);

        // Exibe estado atual
        std::cout << "\nEstado atual:\n";
        for (const auto &linha : estadoAtual)
        {
            for (char c : linha)
                std::cout << "[" << c << "]";
            std::cout << '\n';
        }

        // Escolhe próxima jogada com base no jogador atual
        if (atual->Marca == 'x')
        {
            atual = escolhaIA_o(atual); // O joga depois de X
        }
        else
        {
            atual = escolhaIA_x(atual); // X joga depois de O
        }
    }

    std::cout << "\nResultado final: ";
    if (!atual)
        std::cout << "Erro durante a simulação.\n";
    else if (atual->estadoJogo == 'v')
        std::cout << "Velha!\n";
    else
        std::cout << "Vencedor: " << atual->estadoJogo << "\n";

    // Exibe tabuleiro final
    if (atual)
    {
        auto final = recriarEstadoPorPosiSimb(estadoInicial, atual);
        for (const auto &linha : final)
        {
            for (char c : linha)
                std::cout << "[" << c << "]";
            std::cout << '\n';
        }
    }
}

//////////////////////////---------- ~Funções IAs e Jogador~ -------------///////////////////////////

//---------- MAIN -------------//

int main()
{
    std::vector<std::vector<char>> estadoInicial = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}};

    No *raiz = new No();

    int posiInicial = 1;
    char simboloInicial = 'x';

    /*
    // Aplica a jogada no estado inicial
    int linha = posiInicial / 3;
    int coluna = posiInicial % 3;
    estadoInicial[linha][coluna] = simboloInicial;

    // Também configura a raiz com essas infos!
    raiz->Marca = simboloInicial;
    raiz->posiMarca = posiInicial;
*/

    // gerarArvoreDecisao(raiz, estadoInicial, posiInicial, simboloInicial);

    IAvsIA(raiz, estadoInicial, posiInicial, simboloInicial);
    // std::cout << "estado da RAIZ : "<< raiz->estadoJogo<< "\n";

    return 0;
}
