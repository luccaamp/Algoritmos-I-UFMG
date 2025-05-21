## UFMG - DCC - Algoritmos I - 2024/1
# Trabalho Prático 2 - Os Reis Amarelos

## Objetivo

Este trabalho implementa um verificador de vitória para a fase de fuga do jogo "Os Reis Amarelos", determinando os movimentos dos monstros e a melhor estratégia possível para os jogadores.

Dado um mapa orientado com custos, monstros e recursos, a tarefa é:

1. **Simular a movimentação dos monstros**, que traçam rotas de custo mínimo e seguem caminhos lexicograficamente mínimos para alcançar o ponto inicial dos jogadores.
2. **Verificar se os jogadores conseguem chegar à capital Bacopolis em até T turnos**, sem dividir espaço com monstros, minimizando recursos gastos. Se não for possível, devem maximizar o número de turnos sobrevividos e, em caso de empate, minimizar os recursos gastos.
3. **Reportar detalhadamente as rotas dos monstros, o resultado do jogo e o caminho tomado pelos jogadores**.

## Entrada

Cada caso de teste consiste em:

- Uma linha com cinco inteiros:  
  `N` (número de espaços no mapa),  
  `M` (número de conexões),  
  `J` (número de monstros),  
  `T` (número máximo de turnos),  
  `K` (recursos recebidos por turno).
- Uma linha com `J` inteiros: posições iniciais dos monstros.
- `M` linhas, cada uma com três inteiros:  
  `xi`, `yi`, `ci` (há um caminho do espaço `xi` para o espaço `yi` com custo de `ci` recursos).

**Restrições:**  
- 1 ≤ N ≤ 10⁴  
- 1 ≤ M ≤ min(N², 10⁵)  
- Espaços são numerados de 1 a N (Bacopolis é o espaço N, a caravana começa em 1)  
- 1 ≤ J ≤ 100  
- 1 ≤ T ≤ 10⁴  
- 1 ≤ K ≤ 1000  
- Custos: 1 ≤ ci ≤ 1000

## Saída

Para cada caso de teste deve imprimir:

1. Uma linha com um inteiro `V` (1 se os jogadores podem vencer, 0 caso contrário).
2. Para cada monstro, uma linha com o tamanho da rota seguido pelos espaços visitados na ordem (rota lexicograficamente mínima de menor comprimento até o espaço inicial dos jogadores).
3. Uma linha com dois inteiros `R` e `F`: recursos gastos e turnos jogados pelos jogadores.
4. Uma linha com a sequência de espaços visitados pela caravana do início ao fim.

## Como usar

Para compilar o programa, digite na linha de comando:
```{bash}  
make
```  
Para rodar o programa, digite o comando abaixo e depois passe uma entrada (o arquivo TP2_doc possui alguns exemplos):
``` {bash}
make run
```  

Exemplo:
```  
make
make run
9 12 2 6 4
6 7
1 3 2
2 1 2
3 4 2
4 2 2
4 5 3
5 6 5
5 9 20
6 4 3
6 9 3
7 2 1
7 8 4
8 1 4
```  