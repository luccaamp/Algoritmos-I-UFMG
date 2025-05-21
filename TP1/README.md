## UFMG - DCC - Algoritmos I - 2024/1
# Trabalho Prático 1 - Biconectividade

## Objetivo

A tarefa consiste em, dado um grafo representando uma rede, identificar os **links de borda**, os **clusters** da rede, e construir a **Floresta de clusters-bordas** que representa a relação entre eles. Deve-se:

1. **Listar todos os links de borda** de uma rede, ou seja, os links que pertencem a mais de um cluster.
2. **Listar todos os clusters** da rede, que são conjuntos máximos de links que permanecem conectados mesmo que um deles seja removido.
3. **Construir a Floresta de clusters-bordas**, um grafo bipartido entre links de borda e clusters, onde há uma aresta se o link pertence ao cluster.

## Entrada

A entrada consiste em casos de teste, cada um composto por:

- Uma linha com dois inteiros `N` e `M`, representando o número de links (vértices) e conexões (arestas) da rede, respectivamente.
- `M` linhas, cada uma com dois inteiros `xi` e `yi`, representando uma conexão entre os links `xi` e `yi`.

**Restrições:**  
- `1 ≤ N, M ≤ 10^5`
- Não há conexões repetidas e `xi ≠ yi`.

## Saída

Para cada caso de teste, o programa deve imprimir:

1. **Links de borda**:
   - Uma linha com o inteiro `F`, o número de links de borda.
   - `F` linhas com o número de cada link de borda, em ordem crescente.

2. **Clusters**:
   - Uma linha com o inteiro `C`, o número de clusters.
   - Para cada cluster, uma linha com:
     - O identificador do cluster (sempre `N + índice`), o tamanho do cluster, e os links pertencentes ao cluster, em ordem crescente.
   - Os clusters devem ser listados em ordem lexicográfica dos links que os compõem.

3. **Floresta de clusters-bordas**:
   - Uma linha com dois inteiros `Z` e `L`, representando o número de vértices e de arestas da floresta.
   - `L` linhas com dois inteiros representando as arestas (sempre com o menor vértice primeiro), ordenadas lexicograficamente.

## Como usar

Para compilar o programa, digite na linha de comando:
```{bash}  
make
```  
Para rodar o programa, digite o comando abaixo e depois passe uma entrada (o arquivo TP1_doc possui alguns exemplos):
``` {bash}
make run
```  

Exemplo:
```  
make
make run
11 15
1 2
2 3
3 4
3 8
4 1
4 6
4 7
5 6
5 7
6 7
8 9
5 10
5 11
10 11
1 11
```  