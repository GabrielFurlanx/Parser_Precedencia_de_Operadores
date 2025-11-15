# Precedência de Operadores

  Nesse trabalho abordaremos como é o funcionamento da **Procedência de Operadores**, que é um analisador sintático do tipo **ascendente**, isso é, um analisador que realiza o reconhecimento sintático a partir da palavra até a palavra inicial.

## 1. Como funciona?

 - É o único analisador sintático que trabalha com **gramáticas ambíguas**, através da tabela de precedência de operadores. 

 - É um analisador que necessita de operadores entre varáveis, ou seja, nãao pode possuir variáveis adjacentes. Isso ocorre pois as decisões de shift e reduce são tomadas baseadas na relação de precedência de símbolos terminais vizinhos. ENtão, dessa forma, se surgirem varáveis lado a lado, sem um operador entre elas, o parser não será capaz de defterminar corretamente onde se inicia e onde termina o handle, tornando a analise sintática ambígua para o método.

  - Um exemplo do que não pode ser feito seria E -> EAFG , pois possui dois terminais juntos.

  - Não pode ter produções lambda/vazias porque ao permitir que um não terminal seja eliminado, os vizinhos entre símbolos terminais tornam-se imprevisívei.

  - É possível transormar gramáticas que não podem ser usadas no parseer de precedência de operadores, por exemplo:

       Gramática de operador não aceita, pois há variáveis adjsacentes

              S -> SAS l b
              A -> bSb l b

       Já a gramática de operador abaixo é aceita, pois não há variáveis adjacentes:

              S -> SbSbS l SbS l b

## 2. Tabela de precedência de Operadores

  - Trabalhando nesse exemplo de gramática: E -> E+E l E*E l id ; Podemos identificar na tabela dessa forma:

             id	  +	  *  	$
          id -	  >	  >  	>
           + >    >   <   >
           * <    >   >   >
           $ <    <   <   -
           
    O identificador é uma não variável, e sempre irá possuir maior precedência sobre todos os operadores matemáticos.

    $ têm a menor precedência de todos.

    Durante o processo, não se compara os identificadores/operadores quando não é possível fazer a gramática. No exemplo acima, não seria possível fazer **idid**, isso porque sempre existe um operador entre dois identificadores. Ou seja, a linguagem não gera sentenças com identificadores adjacentes, então a configuração **idid** nunca ocoorre e não haverá relaçao de precedência definida entre dois **id** consegutivos.

    Outro detalhe é que quando comparado a 2 operadores iguais, a associatividade à esquerda que é a que conta, aentão a parte esquerda da tabela é a prioritária.
