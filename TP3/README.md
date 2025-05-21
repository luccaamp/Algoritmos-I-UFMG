## UFMG - DCC - Algoritmos I - 2024/1
# Trabalho Prático 3 - A Cifra Carmesim

## Objetivo

Este trabalho consiste em resolver um quebra-cabeça baseado na expansão "A Cifra Carmesim" do jogo "Os Reis Amarelos", onde é necessário ativar cristais de uma malha circular maximizando o brilho total sem ativar cristais adjacentes.

Dada uma caixa representada por uma malha circular de cristais, deve-se selecionar um subconjunto de cristais **não adjacentes** de modo a maximizar a soma de seus brilhos. Os cristais podem ter conexões arbitrárias com seus vizinhos (direita, cima, esquerda, baixo), podendo haver ausência de cristais em algumas posições (malha incompleta).

O problema se resume a encontrar o **conjunto independente máximo ponderado** na malha circular descrita.

## Entrada

Cada caso de teste consiste em:

- Uma linha com três inteiros:  
  `L` (número de linhas),  
  `C` (número de colunas),  
  `N` (número de cristais presentes na caixa).
- Seguem-se `N` linhas, cada uma com 7 inteiros:  
  `xi` `yi` `vi` `di` `ci` `ei` `bi`
    - `(xi, yi)` são as coordenadas do cristal (`1 ≤ xi ≤ L`, `1 ≤ yi ≤ C`)
    - `vi` é o brilho do cristal (`1 ≤ vi ≤ 100`)
    - `di`, `ci`, `ei`, `bi` ∈ {0,1} indicam conexão à direita, cima, esquerda e baixo, respectivamente.

**Atenção**: As conexões usam aritmética modular, ou seja, a borda esquerda pode se conectar à direita e o topo ao fundo.

## Saída

- Primeira linha: dois inteiros, `S` e `V`, onde:
  - `S` é o número de cristais na solução
  - `V` é a soma total dos brilhos dos cristais usados
- Em seguida, devem ser impressas `S` linhas, cada uma com dois inteiros `xi yi`, indicando as coordenadas dos cristais escolhidos, em qualquer ordem.

## Como usar

Para compilar o programa, digite na linha de comando:
```{bash}  
make
```  
Para rodar o programa, digite o comando abaixo e depois passe uma entrada (o arquivo TP3_doc possui alguns exemplos):
``` {bash}
make run
```  

Exemplo:
```  
make
make run
3 4 9
1 1 9 1 0 1 1
1 2 5 1 0 1 1
1 3 3 1 0 1 1
1 4 6 1 0 1 1
2 2 9 1 1 1 1
2 3 2 1 1 1 1
2 4 1 1 1 1 1
3 3 4 0 1 1 1
3 4 2 0 1 1 1
```  