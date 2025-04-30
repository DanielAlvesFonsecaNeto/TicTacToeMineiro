#include "Funcoes.hpp"

void IAvsIA(No *raiz, const std::vector<std::vector<char>> estadoInicial, int posiInicial, char simboloInicial, int mXO, int mV)
{

    // mXO e mV são multiplicadore para decisaão das ramificações na arvore , XO para escolha do X e O , V para velha

    if (raiz->filhos.empty())
    {
        gerarArvoreDecisao(raiz, estadoInicial, posiInicial, simboloInicial);
    }

    No *atual = raiz;

    std::cout << "Simulando partida IA vs IA...\n";

    while (atual && atual->estadoJogo == ' ')
    {
        std::vector<std::vector<char>> estadoAtual = recriarEstadoPorPosiSimb(estadoInicial, atual);

        // Exibe estado atual
        if(atual->Marca == 'x'){
            std::cout << "\nJogada do 'X':\n";
        }
        else if(atual->Marca == 'o'){
            std::cout << "\nJogada da 'O':\n";
        }

        
        for (const auto &linha : estadoAtual)
        {
            for (char c : linha)
                std::cout << "[" << c << "]";
            std::cout << '\n';
        }

        // Escolhe próxima jogada com base no jogador atual
        if (atual->Marca == 'x')
        {
            atual = escolhaIA_o(atual,mXO,mV); // O joga depois de X
        }
        else
        {
            atual = escolhaIA_x(atual,mXO,mV); // X joga depois de O
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

void JogadorVsIa(No *raiz, const std::vector<std::vector<char>> estadoInicial, int posiInicial, char simboloInicial, bool JogadorPrimeiro, int mXO, int mV)
{

    char simboloJogador;
    char simboloIA;

    bool VezJogador = true;

    std::cout << "Player VS IA \n\n";

    if (JogadorPrimeiro)
    {
        int Posicao;
        // Exibe estado atual
        std::cout << "\nEstado atual:\n";
        for (const auto &linha : estadoInicial)
        {
            for (char c : linha)
                std::cout << "[" << c << "]";
            std::cout << '\n';
        }

        do
        {
            std::cout << "Escolha posicao : ";
            std::cin >> Posicao;
        } while (Posicao < 0 || Posicao > 15);

        gerarArvoreDecisao(raiz, estadoInicial, Posicao, simboloInicial);

        VezJogador = false;
        simboloJogador = simboloInicial;
        if (simboloJogador == 'x')
        {
            simboloIA = 'o';
        }
        else
        {
            simboloIA = 'x';
        }
    }
    else
    {
        // caso a arvore ainda não tenha sido criada
        if (raiz->filhos.empty())
        {
            gerarArvoreDecisao(raiz, estadoInicial, posiInicial, simboloInicial);
            No *atual = raiz;
        }
        simboloIA = simboloInicial;
        if (simboloIA == 'x')
        {
            simboloJogador = 'o';
        }
        else
        {
            simboloJogador = 'x';
        }
    }

    No *atual = raiz;

    while (atual && atual->estadoJogo == ' ')
    {
        std::vector<std::vector<char>> estadoAtual = recriarEstadoPorPosiSimb(estadoInicial, atual);

        // Exibe estado atual
        std::cout << "\nEstado atual:\n";
        if(atual->Marca == simboloJogador){
            std::cout << "\nJogada do Player: "<<atual->Marca<<"\n";
        }
        else if(atual->Marca == simboloIA){
            std::cout << "\nJogada da IA: "<<atual->Marca<<"\n";
        }

        for (const auto &linha : estadoAtual)
        {
            for (char c : linha)
                std::cout << "[" << c << "]";
            std::cout << '\n';
        }

        if (VezJogador)
        {
            atual = jogadorEscolhe(atual, estadoInicial, simboloJogador);
            VezJogador = false;
        }
        else
        {
            // Escolhe próxima jogada com base no jogador atual
            if (simboloIA == 'o')
            {
                atual = escolhaIA_o(atual,mXO,mV); // O joga depois de X
            }
            else
            {
                atual = escolhaIA_x(atual,mXO,mV); // X joga depois de O
            }
            VezJogador = true;
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

