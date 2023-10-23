# campo-minado 💣
## O jogo
O jogo campo minado é um clássico dos jogos eletrônicos. O jogo consiste de uma matriz quadrada com "células" que podem ter bombas ou posições livres.\
Cada célula possui um número que indica quantas bombas tem nas 8 posições ao seu redor. O objetivo do jogo é encontrar todas as posções livres sem que\
se encontre nenhuma bomba. Caso uma bomba seja encontrada, é fim de jogo.

## Comandos
O jogo apresenta uma matriz com coordenadas horizontais (inteiros) e verticais (letras). O jogador indica o comando que ele quer fazer e qual a coordenada\
que ele quer fazer. Os comandos são:
```MARK``` ou ```m```: faz uma marcação visível para o usuário mas sem que afete o jogo.\
Exemplo: ```MARK A 13```

```OPEN``` ou ```o```: abre uma célula do campo. Se for uma casa livre, aumenta o contador de casas vazias e exibe a quantidade de bombas vizinhas.\
Caso seja uma bomba, é fim de jogo e exibe a posição de todas as bombas do tabuleiro.\

## Como jogar?
```gcc campo_minado.c -o campo_minado```
```./campo_minado```


## Autor
[@augustodsgv](https://github.com/augustodsgv/)
