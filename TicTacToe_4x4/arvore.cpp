#include "Funcoes.hpp"
#include <queue>
#include <stack>

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

/**
 * estado incial pode ser um tabuleiro ja preenchido com algumas posições
 * simbolo inicial seria o simbolo X ou O que jogou nesse estado inicial
 * essa arvore é gerado usando logica de geração em largura com FILA
 */
void gerarArvoreDecisao(No *raiz, const std::vector<std::vector<char>> &estadoInicial, int posiSimbolo, char simboloInicial)
{
    // caso essa função seja chamada mais de uma vez sem querer
    if (!raiz->filhos.empty())
    {
        for (No *filho : raiz->filhos)
        {
            deletaNo(filho);
            raiz->estadoJogo = ' ';
            raiz->pontuacao_O = 0;
            raiz->pontuacao_V = 0;
            raiz->pontuacao_X = 0;
        }
    }

    // isso quebra o jogo e precisa prencher pra não entrar num loop , eu acho , nao testei mas imagina que entraria num loop
    // se nao prencher
    if (posiSimbolo < 0 || posiSimbolo > 15)
    {
        std::cout << "\n\n Intervalo invalido !!! FIM \n\n";
        raiz->Marca = simboloInicial;
        raiz->estadoJogo = 'v';
        raiz->pontuacao_O = 1;
        raiz->pontuacao_V = 1;
        raiz->pontuacao_X = 1;
        raiz->posiMarca = 0;
        return;
    }

    // vai contar quantos Nos tem na arvore
    int contador = 1;

    std::vector<std::vector<char>> estado = estadoInicial;

    // No *raiz = new No();
    raiz->Marca = simboloInicial;
    raiz->posiMarca = posiSimbolo;

    std::queue<No *> fila;
    fila.push(raiz);

    std::cout << "\naguarde....\n";
    while (!fila.empty())
    {

        No *atual = fila.front();
        fila.pop();

        std::vector<std::vector<char>> estadoAtual = recriarEstadoPorPosiSimb(estadoInicial, atual);

        char estadoJogo = avaliarEstado(estadoAtual);
        atual->estadoJogo = estadoJogo;

        if (estadoJogo != ' ')
        {
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
        for (int i = 0; i < 16; ++i)
        {
            int linha = i / 4;
            int coluna = i % 4;

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