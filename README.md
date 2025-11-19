# Precedência de Operadores

Nesse trabalho abordaremos como é o funcionamento da **Procedência de Operadores**, que é um analisador sintático do tipo **ascendente**, isso é, um analisador que realiza o reconhecimento sintático a partir da palavra até o símbolo inicial.

### Membros
- Gabriel Matheus Aguilera Silva
- Gabriel Furlan Costa
- Eduardo Henrique Duarte de Araujo
- Fabrizio Catucci Borghesi
- Carlos Gabriel Augusto Nunes

## 1. Como funciona?

- É o único analisador sintático que trabalha com gramáticas ambíguas, através da tabela de precedência de operadores.

- É um analisador que necessita de operadores entre varáveis, ou seja, não pode possuir variáveis adjacentes. Isso ocorre pois as decisões de shift e reduce são tomadas baseadas na relação de precedência de símbolos terminais vizinhos. Então, dessa forma, se surgirem varáveis lado a lado, sem um operador entre elas, o parser não será capaz de determinar corretamente onde se inicia e onde termina o handle, tornando a analise sintática ambígua para o método.

- Um exemplo do que não pode ser feito seria E -> EAFG , pois possui dois terminais juntos.

- Não pode ter produções lambda/vazias porque ao permitir que um não terminal seja eliminado, os vizinhos entre símbolos terminais tornam-se imprevisível.

- É possível transformar gramáticas que não podem ser usadas no parser de precedência de operadores, por exemplo:

  Gramática de operador não aceita, pois há variáveis adjacentes

            S -> SAS | b
            A -> bSb | b

  Já a gramática de operador abaixo é aceita, pois não há variáveis adjacentes:

            S -> SbSbS | SbS | b

## 2. Tabela de precedência de Operadores

- Trabalhando nesse exemplo de gramática: E -> E+E l E\*E l id ; Podemos identificar na tabela dessa forma:

           id	  +	  *  	$
        id  -   >	  >  	>
         +  >   >   <   >
         *  <   >   >   >
         $  <   <   <   -


  O identificador é uma não variável, e sempre irá possuir maior precedência sobre todos os operadores matemáticos.

  $ têm a menor precedência de todos.

  Durante o processo, não se compara os identificadores/operadores quando não é possível fazer a gramática. No exemplo acima, não seria possível fazer **idid**, isso porque sempre existe um operador entre dois identificadores. Ou seja, a linguagem não gera sentenças com identificadores adjacentes, então a configuração **idid** nunca ocorre e não haverá relação de precedência definida entre dois **id** consecutivos.

  Outro detalhe é que quando comparado a 2 operadores iguais, a associatividade à esquerda que é a que conta, então a parte esquerda da tabela é a prioritária.

## 3. Passos para parsear

- Tendo isso em mente, para realização de nosso trabalho iremos:

1. Escrever o algoritmo de análise sintática em C++ como foi exigido em sala de aula;
2. Gerar a tabela;
3. Realizar a leitura da palavra;
4. Manipular a pilha;
5. Mover o lookahead.

## 4. Como fizemos

Nossas atividades em grupo foram separadas da seguinte maneira: primeiro estudamos a respeito do algoritmo de precedência de operadores em separado, alguns focando mais no conceito, outros em aplicações da lógica e no seu uso em C++.

Depois de contextualizados, nos reunimos para colocar o conceito à prova, estruturamos o esqueleto do código em python a nível mais conceitual para testar se nossa lógica estava correta, nessa etapa todos participaram com sugestões com base no que haviam obtido de informações. Uma vez que a lógica base foi definida e posta à prova, iniciamos a implementação em C++, onde a lógica básica foi de fato implementada totalmente e refinada, os prints envolvidos foram melhor elaborados para facilitar a visualização de cada etapa da execução.

Durante a execução de cada etapa, fomos juntando as informações em um docs para facilitar a criação do README.

### 4.1 Qual a lógica de execução?

Primeiramente, recebemos a palavra e a convertemos em uma lista, onde cada símbolo foi adicionado apenas uma vez em ordem de aparição, com adição do $ no final da lista
Exemplo: id + id _ id foi convertido em ["id", "+", "_", "$"]
Dessa forma, podemos usar essa lista para montar a tabela de precedência

Na tabela de precedência, temos uma matriz (ou lista de listas), onde cada índice representa um símbolo da lista de símbolos da palavra previamente gerada. Para definir qual a precedência de cada símbolo, organizamos a seguinte lógica:

```
Separamos os símbolos em 4 grupos, correspondentes às suas precedências, e então montamos
um método que checa condicionalmente qual a precedência de cada símbolo de 1 a 4, sendo 1 a menor
e 4 a maior precedência

 Se o símbolo lido for "$":
     Precedência = 4
 Se o símbolo lido for "+" ou "-":
     Precedência = 3
 Se o símbolo lido for "*" ou "/":
     Precedência: 4
 Senão (significa que é identificador):
     Precedência: 4
```

A seguir, geramos outro método que usa condicional para definir o sinal (">", "<", "-") da precedência de cada
combinação de símbolos, adicionando-os na matriz que representa a tabela de precedência, a lógica condicional
usada para montar a tabela é a seguinte:

Na Matriz de mesma dimensão da lista de símbolos ([linha][coluna]=[len(lista)][len(lista)]), cada posição (linha, coluna)
corresponde ao símbolo daquela posição na lista de símbolos da palavra

Exemplo: para a lista de símbolos ["id", "+", "*", "$"], a matriz[0][0] corresponde à comparação de "id" com "id",
já matriz[0][1] corresponde à comparação de "id" com "+"...

Iterando sobre a matriz, verificamos cada combinação de símbolos das linhas com os símbolos das colunas (loop for das linhas aninhado com loop for das colunas), podendo assim obter tanto a precedência do símbolo da linha quanto o da coluna, e assim compará-los

A partir disso, se inicia a lógica condicional para definir os sinais de precedência da tabela:

```
Se a precedência do símbolo da linha for maior que a da coluna (ex: 4 > 3):
    Símbolo da Precedência = ">"
Se a precedência do símbolo da linha for menor que a da coluna (ex: 3 < 4):
    Símbolo da Precedência = "<"
Se a precedência do símbolo da linha for igual à da coluna:
    Se a precedência do símbolo da linha for 4 (significa que é um identificador):
        Símbolo da Precedência = "-"
    Senão (tudo que não seja identificador, a precedência da linha prevalece):
        Símbolo da Precedência = ">"

A tabela na posição analisada receberá o símbolo obtido, obtendo algo no fim como:
    id	 +	 *   $
 id  -   >	 >   >
  +  >   >   <   >
  *  <   >   >   >
  $  <   <   <   -
```

Depois, geramos outra lista que quebra a palavra em cada um dos seus caracteres, dessa vez permitindo itens duplicados, já que vai ser literalmente a palavra lida, e adicionamos o $ ao final significando o fim da palavra lida
Exemplo: id + id _ id foi convertida em ["id", "+", "id", "_", "id", "$"]

A partir dessa lista de caracteres, podemos ler cada símbolo da palavra, e a partir da tabela de precedência gerada anteriormente, checamos qual a precedência

Definimos a pilha, primeiramente como ["$"]

Definimos o lookahead como 0, para apontar para a posição 0 da lista que corresponde à palavra lida

Com base no símbolo lido na iteração, comparamos sua precedência com a do elemento na cauda (tail) da pilha, para isso fazemos a checagem da matriz[simbolo_lido][tail_da_pilha] e então realizamos uma condicional:

```
Se o símbolo da precedência for ">" (precedência do símbolo lido for maior que do tail da pilha):
    Adiciona na pilha o símbolo lido;
    Soma 1 ao lookahead;
Se o símbolo da precedência for "<" (precedência do símbolo lido for menor que do tail da pilha):
    Remove o tail da pilha;
    Mantém o lookahead como está;
Se o símbolo da precedência for "-" (precedência do símbolo lido for menor que do tail da pilha):
    Se o símbolo lido for $ (significa que tanto pilha quanto palavra chegaram ao seu final):
        "A palavra foi parseada com sucesso"
    Senão (Significa que na palavra há uma sequência de identificadores, o que não é válido nesse algoritmo):
        Erro: "Palavra não parseável"
```

Dessa forma, garantimos a análise sintática de precedência de operadores da palavra, montando a tabela de precedência, e fazendo as operações com a palavra e pilha conforme as precedências obtidas nela

## 5. Como executar o arquivo
### 5.1 No Linux/MacOS
## Como executar

Certifique-se de ter um compilador C++ instalado (como G++).

```
# 1. Compilar
g++ analisador_precedencia.cpp -o analisador

# 2. Executar (Linux/Mac/PowerShell)
./analisador

# Ou no Windows CMD:
analisador.exe
```

## 6. Referências utilizadas:

Utilizamos o livro **Compiladores: Princípios, Técnicas e Ferramentas** para compreender a estruturação e entender melhor como funciona o analisador sintático. Ele foi a base para a estruturação do projeto e conhecimento do grupo, nele tem até um exemplo de pseudocódigo desse algoritmo de análise de precedência, na página 91, com uma explicação consistente de cada passo a ser realizado.
