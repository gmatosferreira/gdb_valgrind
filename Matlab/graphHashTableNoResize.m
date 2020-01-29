clc

f=tdfread("hashtablenoresize100.tsv","\t");
indices=f.x0xC30x8Dndice;
comprimentos=f.Comprimento_da_linked_list;

figure(1);
s=stem(indices,comprimentos,"Color","#EDB120");
set(get(get(s(1),'Annotation'),'LegendInformation'),'IconDisplayStyle','off'); %Ignore legend for this graph
hold on
plot(0:99,ones(100,1)*24314/100,"black","LineWidth",1)
title("Hash Table de tamanho 100 sem resize");
xlabel("Índice da tabela");
ylabel("Número de nós da lista ligada");
legend("Média");