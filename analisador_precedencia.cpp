#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int obter_ordem_precedencia(const string& token) {
    int precedencia;
    if (token == "$") {
        precedencia = 1;
    } else if (token == "+" || token == "-") {
        precedencia = 2;
    } else if (token == "/" || token == "*") {
        precedencia = 3;
    } else {
        precedencia = 4;
    }
    return precedencia;
}

string ler_precedencia(const string& elemento_linha, const string& elemento_coluna) {
    int prec_linha = obter_ordem_precedencia(elemento_linha);
    int prec_coluna = obter_ordem_precedencia(elemento_coluna);

    if (prec_linha < prec_coluna) {
        return "<";
    } else if (prec_linha > prec_coluna) {
        return ">";
    } else {
        if (prec_linha == 1 || prec_linha == 4)
            return "-";
        else
            return ">";
    }
}

vector<vector<string>> montar_tabela(const vector<string>& linha, const vector<string>& coluna) {
    vector<vector<string>> tabela(linha.size(), vector<string>(coluna.size(), "-"));

    for (int i = 0; i < linha.size(); i++) {
        for (int j = 0; j < coluna.size(); j++) {
            tabela[i][j] = ler_precedencia(linha[i], coluna[j]);
        }
    }
    return tabela;
}

vector<string> converter_para_lista(const string& palavra) {
    vector<string> lista_de_tokens;
    stringstream ss(palavra);
    string tok;
    vector<string> lista_unica_ordenada;

    while (ss >> tok) {
        bool existe = false;
        for (auto& x : lista_unica_ordenada) {
            if (x == tok) {
                existe = true;
                break;
            }
        }
        if (!existe) lista_unica_ordenada.push_back(tok);
    }

    lista_unica_ordenada.push_back("$");
    return lista_unica_ordenada;
}

int obter_indice(const vector<string>& lista, const string& elemento) {
    for (int i = 0; i < lista.size(); i++) {
        if (lista[i] == elemento) return i;
    }
    return -1;
}

void analise_sintatica(const vector<vector<string>>& tabela,
                       const string& palavra,
                       const vector<string>& lista_simbolos)
{
    vector<string> palavra_lista;
    stringstream ss(palavra);
    string tok;

    while (ss >> tok) {
        palavra_lista.push_back(tok);
    }

    palavra_lista.push_back("$");

    vector<string> pilha = {"$"};

    int indice_lookahead = 0;
    int count = -1;

    while (true) {
        count++;
        string lookahead = palavra_lista[indice_lookahead];
        string topo_pilha = pilha.back();

        int indice_linha = obter_indice(lista_simbolos, lookahead);
        int indice_coluna = obter_indice(lista_simbolos, topo_pilha);
        string precedencia = tabela[indice_linha][indice_coluna];

        cout << "lookahead iteração: " << count
             << " indice: " << indice_lookahead
             << " lookahead: " << lookahead << endl;

        cout << "topo da pilha iteração " << count << ": " << topo_pilha << endl;

        if (precedencia == ">") {
            pilha.push_back(lookahead);
            indice_lookahead++;
        }
        else if (precedencia == "<") {
            if (!pilha.empty()) pilha.pop_back();
        }
        else if (lookahead != "$") {
            throw runtime_error("Palavra não parseável");
        }

        cout << "Pilha iteração " << count << ": [";
        for (int i = 0; i < pilha.size(); i++) {
            cout << "'" << pilha[i] << "'";
            if (i < pilha.size() - 1) cout << ", ";
        }
        cout << "]" << endl;

        if (lookahead == "$" && topo_pilha == "$") {
            cout << "Palavra parseável" << endl;
            break;
        }
    }
}

void ler_palavra(const string& palavra) {
    vector<string> palavra_lista = converter_para_lista(palavra);
    vector<string> linha = palavra_lista;
    vector<string> coluna = palavra_lista;

    vector<vector<string>> tabela = montar_tabela(linha, coluna);

    for (auto& lin : tabela) {
        cout << "[";
        for (int i = 0; i < lin.size(); i++) {
            cout << "'" << lin[i] << "'";
            if (i < lin.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }

    analise_sintatica(tabela, palavra, palavra_lista);
}

int main() {
    ler_palavra("id + id * id");
    return 0;
}