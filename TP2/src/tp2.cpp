#include <algorithm>
#include <queue>
#include <set>
#include <stack>
#include <stdio.h>
#include <vector>

#define INF 0x3f3f3f3f

using namespace std;

typedef pair<int, int> Par;  //(Pesos, Vertices)

struct Tupla{
    int custo;
    int destino;
    int turno_visitado;

    bool operator<(const Tupla& outro) const{
        return custo < outro.custo; // Custo do maior para o menor
    };

    // O operador '>' é o inverso de '<'
    bool operator>(const Tupla& outro) const {
        return custo > outro.custo; // Ordem decrescente pelo custo
    }
};

class Graph{
    private:
        int _num_vertices;
        vector<vector<Par>> _grafo;

        // Para fazer BFS:
        int _num_turnos;
        int _num_monstros;
        vector<vector<int>> _inverso; // grafo transposto sem pesos
        vector<int> _posicao_monstro; // vértices em que cada monstro começa o jogo, [0]=>posição inicial do 1° monstro dado na entrada...

        // Para Dijkstra:
        int _fim; // indice do vértice N, vértice que o jogador deseja chegar
        int _recursos_por_turno; // quantidade de recursos dada por turno
        vector<vector<bool>> _turno_posicao_monstro; // Para cada turno guarda quais são as posições dos monstros naquele turno (se no vértice tem monstro => true, se não => false)

        // Para printar:
        vector<vector<int>> _monstro_caminho;
        stack<int> _Caminho_Jogador;
        bool _GANHOU;
        int _TURNOS_GASTOS;
        int _RECURSOS_GASTOS;
    
    public:
        Graph(int vertices, int turnos, int recursos, int inimigos){
            _num_vertices = vertices;
            _grafo.resize(vertices);

            // Para BFS
            _num_turnos = turnos;
            _num_monstros = inimigos;
            _inverso.resize(vertices);
            _posicao_monstro.resize(inimigos, -1);

            // Para Dijkstra:
            _fim = vertices - 1;
            _recursos_por_turno = recursos;
            _turno_posicao_monstro.resize(turnos+1, vector<bool>(vertices, false));

            // Para printar:
            _monstro_caminho.resize(inimigos);
        };

        int lendo_entrada(int arestas, int num_monstros){
            for(int i=0; i < num_monstros; ++i){
                int temp;
                if((scanf(" %d", &temp) != 1) || temp <= 0 || temp > _num_vertices){
                    return 1; // Falha na leitura
                }
                _posicao_monstro[i] = temp - 1;
            }

            for (int i=0; i < arestas; ++i){ // Adicionar arestas:
                int u, v, peso; 

                if(scanf(" %d %d %d", &u, &v, &peso) != 3 || u < 0 || v < 0){
                    return 1; // Falha na leitura
                }

                _grafo[u-1].push_back(make_pair(peso, v-1)); // grafo direcionado (Pesos, Vertices)
                // Pegar o grafo transposto Gt sem pesos para fazer BFS simples da parte dos monstros
                _inverso[v-1].push_back(u-1);
                // Resolver problema que monstros tem uma predileção por caminhos lexicograficamente pequenos:
                sort(_inverso[v-1].begin(), _inverso[v-1].end());
            }

            // colocar loops para cada vértice
            for(int i=0; i < _num_vertices; ++i){
                _grafo[i].push_back(make_pair(1, i));
            }
            return 0;
        };

        void BFS(){
            int origem = 0; // pq vértice 1 está na posição 0
            queue<int> fila; //armazenar os vértices a serem visitados na BFS
            fila.push(origem);

            vector<int> _distancia(_num_vertices, -1);        
            vector<int> _parente(_num_vertices, -1);
            vector<bool> visitado(_num_vertices, false);

            _distancia[origem] = 0; //distância do vértice de origem para ele mesmo como 0
            visitado[origem] = true;

            while(!fila.empty()){ // enquanto houver vértices a serem visitados
                int u = fila.front(); //Retiramos o primeiro vértice da fila q e o armazenamos em u
                fila.pop();

                for(int v : _inverso[u]){ // Iteramos sobre todos os vizinhos ao vértice u
                    if (!visitado[v]){ //verificamos se sua distância ainda não foi definida
                        visitado[v] = true; 
                        _distancia[v] = _distancia[u] + 1; // Se v ainda não foi visitado, atualizamos sua distância como a distância do vértice u mais 1
                        _parente[v] = u; // definimos o pai do vértice v como o vértice u
                        fila.push(v); // Adicionamos o vértice v à fila q, pois precisamos visitar seus vizinhos na próxima iteração do loop
                    }
                    else if(_distancia[v] == _distancia[u] + 1 && _parente[v] > u){
                        _parente[v] = u;
                    }
                }
            }

            for (int i = 0; i < _num_monstros; i++) {
                int atual = _posicao_monstro[i]; // Já fez -1 na entrada, Ex: vértice 6 está na posição 5 
                for (int v = atual; v != -1; v = _parente[v]){
                    _monstro_caminho[i].push_back(v);
                }
            }
        };

        void gerar_turno_posicao_monstro() {
            for (int i = 0; i < _num_monstros; i++){
                int tamanho_caminho = _monstro_caminho[i].size();

                if(tamanho_caminho == 1){ // Monstro fica parado, não chega na origem
                    int posicao = _monstro_caminho[i][0];

                    for (int t=0; t < _num_turnos+1; t++) {
                        _turno_posicao_monstro[t][posicao] = true;
                    }
                    continue;
                }

                // Se existe caminho do monstro até a origem
                for (int t=0; t < tamanho_caminho; t++) {
                    int posicao = _monstro_caminho[i][t];
                    _turno_posicao_monstro[t][posicao] = true;
                }

                for (int t = tamanho_caminho; t < _num_turnos+1; t++) {
                    _turno_posicao_monstro[t][0] = true;
                }
            }
        };

        void Dijkstra(){
            int origem = 0;

            // Fila de prioridade de Tuplas {custo, destino, turno_visitado}
            std::priority_queue<Tupla, std::vector<Tupla>, std::greater<Tupla>> fila_prioridade;

            vector<vector<int>> dist(_num_vertices, vector<int>(_num_turnos+1, INF)); // Cria um vector de vector para distancias e inicializa ele com todas as distancias como INF
            vector<vector<int>> pred(_num_vertices, vector<int>(_num_turnos+1, -2));  // Cria um vector de vector para armazenar o predecessor de cada vértice

            dist[0][0] = 0;  //dist da origem é 0
            pred[0][0] = -1; //pred da origem não existe, -1
            
            // Insere origem na fila e inicializa {custo, destino, turno_visitado} tudo como 0
            fila_prioridade.push(Tupla{0, origem, 0}); // {custo, destino, turno_visitado}

            Tupla melhor = {0, origem, 0};
            Tupla final = {0, origem, 0};

            bool fez_loop = false;
            
            while(!fila_prioridade.empty()) {
                Tupla tupla = fila_prioridade.top();
                fila_prioridade.pop();

                int u = tupla.destino; // Vértice atual
                int distancia = tupla.custo; // Distância(custo) do vértice atual
                int turno = tupla.turno_visitado; //Turno visitado do vértice atual

                if(dist[u][turno] < distancia){
                    continue;
                }

                // Ordem lexicográfica => turno maior, distancia(custo) menor, destino(vertice ligado) menor
                if (
                    (turno > melhor.turno_visitado) ||
                    (turno == melhor.turno_visitado && distancia < melhor.custo) ||
                    (turno == melhor.turno_visitado && distancia == melhor.custo && u < melhor.destino)
                ) {
                    melhor = tupla;
                }

                // Se chegamos ao destino, podemos parar a busca
                if (u == _fim) {
                    final = tupla;
                    break;
                }

                // Passou do número máximo de turnos
                if (turno == _num_turnos) {
                    continue;
                }

                fez_loop = true;

                // Explorando os vizinhos do vértice atual u
                for (Par edge : _grafo[u]) {
                    int custo = edge.first;
                    int vizinho = edge.second;

                    // Ver se nesse turno e no próximo turno, esse vizinho está com monstro:
                    if (_turno_posicao_monstro[turno][vizinho] || _turno_posicao_monstro[turno+1][vizinho]) {
                        continue;
                    }

                    fez_loop = false; // Não entrou no if acima

                    int Recurso = (turno + 1) * _recursos_por_turno; // quantidade de recurso que tem no momento

                    // Se existe caminho menor de u para v, e custo para isso é menor ou igual a quantidade de recurso que tem no momento
                    if ((dist[vizinho][turno+1] > dist[u][turno] + custo) && (dist[u][turno] + custo <= Recurso)) {
                        dist[vizinho][turno+1] = dist[u][turno] + custo;
                        pred[vizinho][turno+1] = u;
                        fila_prioridade.push({dist[vizinho][turno+1], vizinho, turno+1}); // {custo, destino, turno_visitado}
                    }
                }
            }

            int vertice_alcancado;
            if(final.turno_visitado > 0){ // Vértice final tem turno visitado maior que 0 => Jogador _GANHOU
                _GANHOU = true;
                _TURNOS_GASTOS = final.turno_visitado;
                vertice_alcancado = final.destino;
            }
            else{ // Vértice final tem turno visitado menor que 0, Vértice final não foi alcançado => Jogador perdeu
                _GANHOU = false;
                _TURNOS_GASTOS = melhor.turno_visitado;
                vertice_alcancado = melhor.destino;
            }

            _RECURSOS_GASTOS = dist[vertice_alcancado][_TURNOS_GASTOS];

            _Caminho_Jogador.push(vertice_alcancado+1);
            for (int i=0; i < _TURNOS_GASTOS; ++i) {
                vertice_alcancado = pred[vertice_alcancado][_TURNOS_GASTOS-i];
                _Caminho_Jogador.push(vertice_alcancado+1);
            }

            if(_GANHOU == 0 && fez_loop == true){
                _RECURSOS_GASTOS++;
                _TURNOS_GASTOS++;
            }

            if(_GANHOU == 0 && fez_loop == true){
                _Caminho_Jogador.push(vertice_alcancado+1);
            }
        };

        void resposta(){
            printf("%d\n", _GANHOU);
        };

        void printar_caminhos_monstros(){
            for (int i = 0; i < _num_monstros; i++){
                // Printar o comprimento do caminho do monstro i:
                int comp = _monstro_caminho[i].size();
                printf("%d", comp);
                for(int t = 0; t < comp; t++){
                    // Printar o caminho do monstro i:
                    printf(" %d", _monstro_caminho[i][t]+1);
                }
                printf("\n");
            }
        };

        void printar_Dijkstra(){
            printf("%d %d\n", _RECURSOS_GASTOS, _TURNOS_GASTOS);

            while (!_Caminho_Jogador.empty()) {
                printf("%d ", _Caminho_Jogador.top());
                _Caminho_Jogador.pop();
            }
            printf("\n");
        };
};

int main(){
    int vertices, arestas; // n° espaços no mapa, n° coneões no mapa
    int num_monstros, max_turno, RecursosPorTurno;

    if(scanf(" %d %d %d %d %d", &vertices, &arestas, &num_monstros, &max_turno, &RecursosPorTurno) != 5 || vertices < 0 || arestas < 0){
        return 1; // Falha na leitura, encerrando o programa
    }

    Graph g(vertices, max_turno, RecursosPorTurno, num_monstros);
       
    if(g.lendo_entrada(arestas, num_monstros) == 1){
        return 1;
    }

    g.BFS();
    g.gerar_turno_posicao_monstro();
    g.Dijkstra();
    g.resposta();
    g.printar_caminhos_monstros();
    g.printar_Dijkstra();

    return 0;
}
