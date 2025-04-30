// Funcoes.hpp
#ifndef FUNCOES_HPP
#define FUNCOES_HPP

#include <vector>
#include "No.hpp"
#include <iostream>
#include <limits>
#include <queue>
#include <stack>

char avaliarEstado(const std::vector<std::vector<char>> &estado);
std::vector<std::vector<char>> recriarEstadoPorPosiSimb(const std::vector<std::vector<char>> &estadoInicial, No *no);
void gerarArvoreDecisao(No *raiz, const std::vector<std::vector<char>> &estadoInicial, int posiSimbolo, char simboloInicial);
No *escolhaIA_x(No *noAtual, int multiplicador_xo, int multiplicador_velha);
No *escolhaIA_o(No *noAtual, int multiplicador_xo, int multiplicador_velha);
No* jogadorEscolhe(No* noAtual, const std::vector<std::vector<char>>& estadoInicial, char Simbolo);
void avaliarPontuacoesDFS(No *raiz);
void IAvsIA(No *raiz, const std::vector<std::vector<char>> estadoInicial, int posiInicial, char simboloInicial, int mXO, int mV);
void JogadorVsIa(No *raiz, const std::vector<std::vector<char>> estadoInicial, int posiInicial, char simboloInicial, bool JogadorPrimeiro, int mXO, int mV);

#endif
