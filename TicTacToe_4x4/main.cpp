#include "TicTacToeMineiro.cpp"

int main()
{
    std::vector<std::vector<char>> estadoInicial = {
        {' ', ' ', ' ', ' '},
        {' ', 'o', 'x', ' '},
        {' ', 'x', 'o', ' '},
        {' ', ' ', ' ', ' '}};

    int posiInicial = 0;
    char simboloInicial = 'x';

    TicTac ticTac(estadoInicial, simboloInicial, posiInicial);
    
    ticTac.IaVsIa();

    //ticTac.playerVsIa(true);

    return 0;
}
/*
    [ 0][ 1][ 2][ 3]
    [ 4][ 5][ 6][ 7]
    [ 8][ 9][10][11]
    [12][13][14][15]
*/