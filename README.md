# Funções a Todo Vapor

Projeto Integrador - BCC

Jogo educacional feito em C com a biblioteca Allegro 5.

O jogo é sobre transformação de funções (transformação de gráficos). Em cada exercício aparece uma função original, por exemplo f(x) = x², e uma função transformada, por exemplo g(x) = (x - 2)². No plano cartesiano a função original aparece em cinza e tem 3 linhas coloridas (A, B e C). O jogador tem que descobrir qual linha é o gráfico da g(x).

Se acertar, vai para a próxima questão. Se errar, o jogo mostra a resposta certa e explica a transformação. São 5 exercícios.

## Controles

ENTER: começa o jogo.
F11: liga e desliga a tela cheia.
ESC: fecha o jogo.

## Como compilar

Precisa ter o MinGW e o Allegro 5 instalados.

gcc main.c -o jogo -lallegro -lallegro\_primitives -lallegro\_font -lallegro\_image -lm

O jogo tem que ser aberto de dentro da pasta do projeto, porque ele carrega a pasta imagens.

# Integrantes:

Emerson Pereira,
João de Oliveira

