#include <iostream>
#include <vector>
#include <set>
#include <climits>

class Caixa {
    private:
        struct Cristal {
            int brilho;     
            bool direita, cima, esquerda, baixo;     
    
            Cristal() : brilho(-1), direita(false), cima(false), esquerda(false), baixo(false) {}
            Cristal(int brilho, bool direita, bool cima, bool esquerda, bool baixo) 
                : brilho(brilho), direita(direita), cima(cima), esquerda(esquerda), baixo(baixo) {}
        };
        
        int _LINHAS, _COLUNAS, _NUM_PONTOS, _MAX_COMB; // Parâmetros do problema
        std::vector<std::vector<Cristal>> _Malha; // Malha de cristais
        std::vector<std::vector<std::vector<int>>> _Tabela; // Tabela de memoização para resultados parciais
        std::vector<std::vector<std::vector<std::set<std::pair<int, int>>>>> _Coordenadas; // Coordenadas dos cristais escolhidos

    public:
        Caixa(int linhas, int colunas, int num_pontos) 
            : _LINHAS(linhas), _COLUNAS(colunas), _NUM_PONTOS(num_pontos) 
            {
            _MAX_COMB = (1 << _COLUNAS); // Calcula o número máximo de combinações de máscaras para as colunas
    
            // Inicializa a malha, tabela de memoização e coordenadas
            _Malha.resize(_LINHAS, std::vector<Cristal>(_COLUNAS));
            _Tabela.resize(_LINHAS, std::vector<std::vector<int>>(_MAX_COMB, std::vector<int>(_MAX_COMB, -1)));
            _Coordenadas.resize(_LINHAS, std::vector<std::vector<std::set<std::pair<int, int>>>>(_MAX_COMB, std::vector<std::set<std::pair<int, int>>>(_MAX_COMB)));
        }

        int ler_entrada() {
            // Leitura dos cristais
            for (int i = 0; i < _NUM_PONTOS; ++i) {
                int x, y, v, d, c, e, b;
                if (scanf("%d %d %d %d %d %d %d", &x, &y, &v, &d, &c, &e, &b) != 7) {
                    return 1; // Falha na leitura
                }
                _Malha[x - 1][y - 1] = Cristal(v, d, c, e, b);
            }
            return 0;
        }
        
        void adicionar_cristal(int x, int y, int brilho, int direita, int cima, int esquerda, int baixo) {
            _Malha[x - 1][y - 1] = Cristal(brilho, direita, cima, esquerda, baixo);
        }
    
        // Resolve o problema para encontrar a melhor combinação de cristais ativados
        void resolver() {
            std::set<std::pair<int, int>> Escolhidos; // Conjunto de coordenadas dos cristais escolhidos
            int total = brilho_melhor_combinacao(Escolhidos); // Calcula o melhor brilho possível
    
            // Imprime o resultado
            printf("%ld %d\n", Escolhidos.size(), total);
            for (const auto& es : Escolhidos) {
                printf("%d %d\n", es.first + 1, es.second + 1); // Ajusta para 1-indexed
            }
        }
        
        bool mask_valida(int linha, int mask) {
            for (int i = 0; i < _COLUNAS - 1; ++i) {
                if ((mask >> i) & 1 && _Malha[linha][_COLUNAS - i - 1].brilho == -1) {
                    return false;
                }
                if (((mask >> i) & 1) && ((mask >> (i + 1)) & 1) && _Malha[linha][_COLUNAS - i - 1].esquerda) {
                    return false;
                }
            }
            if ((mask & 1) && ((mask >> (_COLUNAS - 1)) & 1) && _Malha[linha][0].esquerda) return false;
            return true;
        }
        
        bool proxima_mask_valida(int linha, int atual_mask, int next_mask) {
            int res = atual_mask & next_mask;
            for (int i = 0; i < _COLUNAS; ++i) {
                if (((res >> i) & 1) && _Malha[linha][_COLUNAS - i - 1].baixo) {
                    return false;
                }
            }
            return true;
        }
        
        int get_brilho(int linha, int linha_mask) {
            int brilho = 0;
            for (int i = 0; i < _COLUNAS; ++i) {
                if ((linha_mask >> i) & 1) {
                    brilho += _Malha[linha][_COLUNAS - i - 1].brilho;
                }
            }
            return brilho;
        }
    
        // Função recursiva para calcular o melhor brilho possível para a combinação de cristais
        int calcular(int linha, int linha_mask, int primeira_mask) {
            if (_Tabela[linha][linha_mask][primeira_mask] != -1) {
                return _Tabela[linha][linha_mask][primeira_mask];
            }
    
            if (!mask_valida(linha, linha_mask)) {
                return _Tabela[linha][linha_mask][primeira_mask] = INT_MIN;
            }
    
            int brilho = get_brilho(linha, linha_mask);
            if (linha < _LINHAS - 1) {
                int melhor_brilho = -1;
                for (int i = 0; i < _MAX_COMB; ++i) {
                    if (proxima_mask_valida(linha, linha_mask, i)) {
                        int temp = calcular(linha + 1, i, primeira_mask);
                        if (temp > melhor_brilho) {
                            melhor_brilho = temp;
                            _Coordenadas[linha][linha_mask][primeira_mask] = _Coordenadas[linha + 1][i][primeira_mask];
                        }
                    }
                }
                _Tabela[linha][linha_mask][primeira_mask] = brilho + melhor_brilho;
    
                if (melhor_brilho != INT_MIN) {
                    for (int i = 0; i < _COLUNAS; ++i) {
                        if ((linha_mask >> i) & 1) {
                            _Coordenadas[linha][linha_mask][primeira_mask].insert({linha, _COLUNAS - i - 1});
                        }
                    }
                }
    
                return brilho + melhor_brilho;
            }
    
            if (!proxima_mask_valida(linha, linha_mask, primeira_mask)) {
                return _Tabela[linha][linha_mask][primeira_mask] = INT_MIN;
            }
    
            _Tabela[linha][linha_mask][primeira_mask] = brilho;
    
            if (brilho != INT_MIN) {
                for (int i = 0; i < _COLUNAS; ++i) {
                    if ((linha_mask >> i) & 1) {
                        _Coordenadas[linha][linha_mask][primeira_mask].insert({linha, _COLUNAS - i - 1});
                    }
                }
            }
    
            return brilho;
        }
    
        // Encontra a melhor combinação de cristais ativados
        int brilho_melhor_combinacao(std::set<std::pair<int, int>>& Escolhidos) {
            int melhor_brilho = -1;
            for (int i = _MAX_COMB - 1; i >= 0; --i) {
                int temp = calcular(0, i, i);
                if (temp > melhor_brilho) {
                    melhor_brilho = temp;
                    Escolhidos = _Coordenadas[0][i][i];
                }
            }
            return melhor_brilho;
        }
};

int main() {
    int linhas, colunas, num_pontos;
    if (scanf("%d %d %d", &linhas, &colunas, &num_pontos) != 3) {
        return 1; // Falha na leitura
    }

    Caixa caixa(linhas, colunas, num_pontos);

    if(caixa.ler_entrada() == 1){
        return 1; // Falha na leitura
    }

    caixa.resolver();

    return 0;
}