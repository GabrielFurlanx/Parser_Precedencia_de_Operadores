#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

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

string print_vector_to_string(const vector<string> &vec)
{
    stringstream ss;
    for (const string &s : vec)
    {
        ss << s << " ";
    }
    return ss.str();
}

string print_input_to_string(const vector<string> &input, int index)
{
    stringstream ss;
    for (size_t i = index; i < input.size(); ++i)
    {
        ss << input[i] << " ";
    }
    return ss.str();
}

void analise_sintatica(const vector<vector<string>> &tabela,
                       const string &palavra,
                       const vector<string> &lista_simbolos)
{
    vector<string> palavra_lista;
    stringstream ss(palavra);
    string tok;

    while (ss >> tok)
    {
        palavra_lista.push_back(tok);
    }
    palavra_lista.push_back("$");

    vector<string> pilha = {"$"};
    int indice_lookahead = 0;
    int count = 0;

    cout << "\n### Análise Sintática Passo a Passo ###\n"
         << endl;
    cout << left << setw(10) << "ITER."
         << left << setw(25) << "PILHA"
         << left << setw(25) << "ENTRADA"
         << left << setw(10) << "RELAÇÃO"
         << left << setw(15) << "AÇÃO" << endl;
    cout << string(85, '-') << endl;

    while (true)
    {
        string lookahead = palavra_lista[indice_lookahead];
        string topo_pilha = pilha.back();

        int indice_linha = obter_indice(lista_simbolos, lookahead);
        int indice_coluna = obter_indice(lista_simbolos, topo_pilha);
        string precedencia = tabela[indice_linha][indice_coluna];

        string pilha_str = print_vector_to_string(pilha);
        string entrada_str = print_input_to_string(palavra_lista, indice_lookahead);

        cout << left << setw(10) << count
             << left << setw(25) << pilha_str
             << left << setw(25) << entrada_str
             << left << setw(10) << precedencia;

        if (precedencia == ">")
        {
            cout << left << setw(15) << "Empilha (Shift)" << endl;
            pilha.push_back(lookahead);
            indice_lookahead++;
        }
        else if (precedencia == "<")
        {
            cout << left << setw(15) << "Reduz (Reduce)" << endl;
            if (!pilha.empty())
                pilha.pop_back();
        }
        else if (lookahead != "$")
        {
            cout << left << setw(15) << "Erro" << endl;
            throw runtime_error("Palavra não parseável");
        }
        else
        {
            cout << left << setw(15) << "Aceita" << endl;
        }

        if (lookahead == "$" && topo_pilha == "$")
        {
            cout << string(85, '-') << endl;
            cout << ">>> Palavra: " << palavra << " parseada com sucesso! <<<" << endl;
            cout << string(85, '-') << endl;
            break;
        }

        count++;
    }
}

void ler_palavra(const string &palavra)
{
    vector<string> palavra_lista = converter_para_lista(palavra);
    vector<string> linha = palavra_lista;
    vector<string> coluna = palavra_lista;

    vector<vector<string>> tabela = montar_tabela(linha, coluna);

    cout << "### Tabela de Precedência (Linha=Lookahead, Coluna=TopoPilha) ###" << endl;

    cout << left << setw(5) << "";
    for (const string &col_header : coluna)
    {
        cout << left << setw(5) << col_header;
    }
    cout << endl
         << string((coluna.size() + 1) * 5, '-') << endl;

    for (int i = 0; i < linha.size(); i++)
    {
        cout << left << setw(5) << linha[i];
        for (int j = 0; j < coluna.size(); j++)
        {
            cout << left << setw(5) << tabela[i][j];
        }
        cout << endl;
    }

    analise_sintatica(tabela, palavra, palavra_lista);
}

int main() {
    ler_palavra("id + id * id");
    // ler_palavra("id + id id * id"); Palavra não válida no algoritmo
    return 0;
}
