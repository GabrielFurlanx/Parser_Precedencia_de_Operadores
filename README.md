# Precedência de Operadores

  Nesse trabalho abordaremos como é o funcionamento da **Procedência de Operadores**, que é um analisador sintático do tipo **ascendente**, isso é, um analisador que realiza o reconhecimento sintático a partir da palavra até a palavra inicial.

## 1.  É o único analisador sintático que trabalha com **gramáticas ambíguas**, através da tabela de precedência de operadores. 

  É um analisador que necessita de operadores entre varáveis, ou seja, nãao pode possuir variáveis adjacentes. Isso ocorre pois as decisões de shift e reduce são tomadas baseadas na relação de precedência de símbolos terminais vizinhos. ENtão, dessa forma, se surgirem varáveis lado a lado, sem um operador entre elas, o parser não será capaz de defterminar corretamente onde se inicia e onde termina o handle, tornando a analise sintática ambígua para o método.

  Um exemplo do que não pode ser feito seria E -> EAFG , pois possui dois terminais juntos.

## 2.
