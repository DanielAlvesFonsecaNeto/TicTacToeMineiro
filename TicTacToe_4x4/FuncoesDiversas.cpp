#include "Funcoes.hpp"
#include <iostream>
#include <limits>

// função q avalia estado do tabulerio para definir se teve vencedor V ou O , ou se teve velha ou se ainda esta em andamento
char avaliarEstado(const std::vector<std::vector<char>> &estado)
{
    /*
    Retornos possíveis:
    'x' → vitória do X
    'o' → vitória do O
    'v' → empate (velha)
    ' ' → jogo em andamento
    */

    // Checa linhas para sequências de 3 iguais
    for (int i = 0; i < 4; ++i)
    {
        // Checar posições (0,1,2) da linha
        if (estado[i][0] != ' ' && estado[i][0] == estado[i][1] && estado[i][1] == estado[i][2])
            return estado[i][0];
        // Checar posições (1,2,3) da linha
        if (estado[i][1] != ' ' && estado[i][1] == estado[i][2] && estado[i][2] == estado[i][3])
            return estado[i][1];
    }

    // Checa colunas para sequências de 3 iguais
    for (int i = 0; i < 4; ++i)
    {
        // Checar posições (0,1,2) da coluna
        if (estado[0][i] != ' ' && estado[0][i] == estado[1][i] && estado[1][i] == estado[2][i])
            return estado[0][i];
        // Checar posições (1,2,3) da coluna
        if (estado[1][i] != ' ' && estado[1][i] == estado[2][i] && estado[2][i] == estado[3][i])
            return estado[1][i];
    }

    // Checa diagonais principais para sequência de 4 iguais
    if (estado[0][0] != ' ' && estado[0][0] == estado[1][1] && estado[1][1] == estado[2][2] && estado[2][2] == estado[3][3])
        return estado[0][0];

    if (estado[0][3] != ' ' && estado[0][3] == estado[1][2] && estado[1][2] == estado[2][1] && estado[2][1] == estado[3][0])
        return estado[0][3];

    // Checa diagonais paralelas para formar 4 iguais

    /*
        3 pra 1
        [0,0] [1,3] [2,2] [3,1]
        [0,3] [1,0] [2,1] [3,2]
        [0,1] [1,2] [2,3] [3,0]
        [0,2] [1,1] [2,0] [3,3]

        2 pra 2

        [0,1] [1,0] [2,3] [3,2]
        [0,2] [1,3] [2,0] [3,1]
    */

    // 3 pra 1
    // [0,0] [1,3] [2,2] [3,1]
    if (estado[0][0] != ' ' && estado[0][0] == estado[1][3] && estado[1][3] == estado[2][2] && estado[2][2] == estado[3][1])
    {
        return estado[0][0];
    }
    // [0,3] [1,0] [2,1] [3,2]
    if (estado[0][3] != ' ' && estado[0][3] == estado[1][0] && estado[1][0] == estado[2][1] && estado[2][1] == estado[3][2])
    {
        return estado[0][3];
    }
    // [0,1] [1,2] [2,3] [3,0]
    if (estado[0][1] != ' ' && estado[0][1] == estado[1][2] && estado[1][2] == estado[2][3] && estado[2][3] == estado[3][0])
    {
        return estado[0][1];
    }
    // [0,2] [1,1] [2,0] [3,3]
    if (estado[0][2] != ' ' && estado[0][2] == estado[1][1] && estado[1][1] == estado[2][0] && estado[2][0] == estado[3][3])
    {
        return estado[0][2];
    }

    // 2 pra 2
    // [0,1] [1,0] [2,3] [3,2]
    if (estado[0][1] != ' ' && estado[0][1] == estado[1][0] && estado[1][0] == estado[2][3] && estado[2][3] == estado[3][2])
    {
        return estado[0][1];
    }
    // [0,2] [1,3] [2,0] [3,1]
    if (estado[0][2] != ' ' && estado[0][2] == estado[1][3] && estado[1][3] == estado[2][0] && estado[2][0] == estado[3][1])
    {
        return estado[0][2];
    }

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

        int linha = atual->posiMarca / 4;
        int coluna = atual->posiMarca % 4;
        reconstroiEstado[linha][coluna] = atual->Marca;

        atual = atual->pai;
    }

    return reconstroiEstado;
}

// A escolha que a IA faria , aonde tem uso do MinMax
No *escolhaIA_x(No *noAtual, int multiplicador_xo, int multiplicador_velha)
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
        if (filho->estadoJogo == 'x')
        {
            return filho;
        }

        int valor = ((filho->pontuacao_X * multiplicador_xo) + (filho->pontuacao_V * multiplicador_velha)) - (filho->pontuacao_O * multiplicador_xo);

        if (primeiro || valor > melhorValor)
        {
            melhorValor = valor;
            melhorFilho = filho;
            primeiro = false;
        }
    }

    return melhorFilho;
}

No *escolhaIA_o(No *noAtual, int multiplicador_xo, int multiplicador_velha)
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
        if (filho->estadoJogo == 'o')
        {
            return filho;
        }

        int valor = ((filho->pontuacao_O * multiplicador_xo) + (filho->pontuacao_V * multiplicador_velha)) - (filho->pontuacao_X * multiplicador_xo);

        if (primeiro || valor > melhorValor)
        {
            melhorValor = valor;
            melhorFilho = filho;
            primeiro = false;
        }
    }
    return melhorFilho;
}


No* jogadorEscolhe(No* noAtual, const std::vector<std::vector<char>>& estadoInicial, char Simbolo)
{
    No* escolha = nullptr;
    int Posicao;

    do {
        std::cout << "Escolha posicao: ";
        std::cin >> Posicao;

        if (std::cin.fail()) {
            std::cin.clear(); // limpa estado de erro
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta entrada inválida
            std::cout << "Entrada invalida! Digite um número inteiro.\n";
            continue;
        }

        for (No* filho : noAtual->filhos) {
            if (filho->posiMarca == Posicao) {
                return filho;
            }
        }

        std::cout << "\nPosicao invalida!!!\n";

    } while (true);

    return escolha; // nunca chega aqui, mas mantido por consistência
}
