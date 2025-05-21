#include <algorithm>
#include <set>
#include <stack>
#include <stdio.h>
#include <vector>

using namespace std;

class Graph{
    private:
        int _vertices; // número de vértice do grafo 
        int _tempo;    // contador de tempo para a DFS
        int _num_pontos; // número de pontos de articulação do grafo (n° links de borda da rede)
        int _num_blocos; // número de componentes biconexas (n° clusters da rede)

        std::vector<std::vector<int>> _grafo;

        vector<int> _descobrimento; // vetor com o tempo de descobrimento de cada vértice

        // menor tempo descobrimento alcançável a partir do vértice i (menor valor de profundidade encontrado ao percorrer o grafo)
        vector<int> _min_temp;   

        stack<int> _pilha;

        set<int> _vertice_corte; // conjunto de vértices que são pontos de articulação
        vector<vector<int>> _componentes; // componentes biconexas

        vector<pair<int, int>> _tree_arestas;
    
    public:
        Graph(int vertices) : _vertices(vertices){
            _grafo.resize(vertices);
            _tempo = 0;
            _num_pontos = 0;
            _num_blocos = 0;

            _descobrimento.resize(vertices, 0);
            _min_temp.resize(vertices, 0);
        }

        void lendo_grafo(int arestas){ //O(arestas)
            for (int i=0; i < arestas; ++i) {
                int a, b; 
                if (scanf(" %d %d", &a, &b) != 2 || a < 0 || b < 0) {
                    // Falha na leitura
                    return;
                }
                _grafo[a-1].push_back(b-1);
                _grafo[b-1].push_back(a-1);
            }
        }

        void DFS_visit(int u, int pai) { //O(V + E)
            ++_tempo;
            _descobrimento[u] = ++_tempo;
            _min_temp[u] = ++_tempo;

            _pilha.push(u);

            int filhos = 0;
            
            for (int v : _grafo[u]) {
                
                if (_descobrimento[v] == 0) {
                    DFS_visit(v, u);

                    _min_temp[u] = min(_min_temp[u], _min_temp[v]);
                    
                    if (_min_temp[v] >= _descobrimento[u]) {
                        if (-1 != pai) {
                            _vertice_corte.insert(u);
                            //++_num_pontos;
                        }

                        vector<int> component_aux;
                        int temp;

                        do {
                            temp = _pilha.top();
                            _pilha.pop();
                            component_aux.push_back(temp);
                        } while (temp != v);

                        component_aux.push_back(u);

                        std::sort(component_aux.begin(), component_aux.end());

                        _componentes.push_back(component_aux);

                        //++_num_blocos;
                    }

                    ++filhos;
                } 
                else {
                    _min_temp[u] = min(_min_temp[u], _descobrimento[v]);
                }
            }

            if ((filhos >= 2) && (-1 == pai)) {
                _vertice_corte.insert(u);
                //++_num_pontos;
            }
        }

        void DFS() { //O(V + E)
            for (int i=0; i < _vertices; ++i) {
                if (_descobrimento[i] == 0 ) {
                    DFS_visit(i, -1);
                }
            }

            _num_pontos = _vertice_corte.size();
            _num_blocos = _componentes.size();
        }

        void pontos_de_articulacao(){ //O(num_pontos)
            //std::sort(_vertice_corte.begin(), _vertice_corte.end());
            printf("%d\n", _num_pontos);

            for (int l : _vertice_corte) {
                int num_link = l+1;
                printf("%d\n", num_link);
            }
        }

        void componentes_biconexas(){ //O(num_blocos * tamanho_maior_componente)
            std::sort(_componentes.begin(), _componentes.end()); 

            printf("%d\n", _num_blocos);

            for (int i=0; i < _num_blocos; ++i) {

                int tamanho = _componentes[i].size();

                int  id_cluster = _vertices+i+1;

                printf("%d %d", id_cluster, tamanho);

                for (int j=0; j < tamanho; ++j) {
                    int num_link = _componentes[i][j] + 1;
                    printf(" %d", num_link);
                }
                printf("\n");
            }
        }

        void floresta(){ //O(num_blocos * num_pontos)
            vector<pair<int, int>> tree_arestas;

            for (int v : _vertice_corte) {
                for (int i = 0; i < _num_blocos; ++i)  {
                    if (count(_componentes[i].begin(), _componentes[i].end(), v) > 0) {
                        int num_link = v+1;
                        int id_cluster = _vertices+i+1;
                        tree_arestas.push_back({num_link, id_cluster});
                    }
                }
            }

            printf("%d %d\n", _num_blocos + _num_pontos, (int)tree_arestas.size());
            for (const auto& edge : tree_arestas) {
                printf("%d %d\n", edge.first, edge.second);
            }
        }
};

int main(){
    int vertices, arestas; 

    if (scanf(" %d %d", &vertices, &arestas) != 2 || vertices < 0 || arestas < 0) {
        // Falha na leitura, encerrando o programa
        return 1;
    }

    Graph g(vertices);
       
    g.lendo_grafo(arestas);
    g.DFS(); 

    // 1. Listar todos os links de borda da rede
    g.pontos_de_articulacao();

    // 2. Listar todos os clusters da rede
    g.componentes_biconexas();

    // 3. Construir a Floresta de clusters-bordas da rede
    g.floresta();

    return 0;
}