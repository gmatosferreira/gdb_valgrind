#Hash Table

##Instruções

###Base
Professor vai fornecer ficheiro de texto com obras titulos de todas as obras do Sherlock Holmes em Inglês e código para o ler.

O projeto consiste em implementar, para cada palavra distinta:

		Quantas vezes aparece;

		Onde é que apareceu pela primeira vez (posição no ficheiro);

		Onde é que apareceu pela última vez (posição no ficheiro);

		Distâncias mínima e máxima.

Vamos implementar com separate chaining, utilizando uma lista ligada.
Quando informação exceder tamanho do array, redimensionar. Ou seja, deve crescer à medida das necessidades.

Consideramos como uma palavra qualquer conjunto de caracteres separados por espaços. Um traço é uma palavra. E uma palavra com ponto final é outra palavra.

###Bónus
Na pesquisa, quando encontramos um elemento, deslocá-lo um índice para trás. Assim os mais frequentes vão ter tendência a ficar no início da lista ligada.

Ao criar linked list, em vez de alocar memória só para essa, alocar para vários e cada vez que criamos nova ir buscar próximo endereço alocado, aumentando a eficiencia.

Implementar com árvore binária ordenada.

##Referências
https://www.youtube.com/watch?v=T9gct6Dx-jo