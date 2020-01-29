clc

figure(1);

f=tdfread("hashtablenoresize100.tsv","\t");
indices=f.x0xC30x8Dndice;
comprimentos=f.Comprimento_da_linked_list;
subplot(2,2,1);
s=stem(indices,comprimentos,"Color","#EDB120");
set(get(get(s(1),'Annotation'),'LegendInformation'),'IconDisplayStyle','off'); %Ignore legend for this graph
hold on
plot(0:99,ones(100,1)*24314/100,"black","LineWidth",1)
title("Hash Table de tamanho 100 sem resize | slides teóricos");
xlabel("Índice da tabela");
ylabel("Número de nós da lista ligada");
legend("Média");

f=tdfread("hashtablenoresizedjb2.tsv","\t");
indices=f.x0xC30x8Dndice;
comprimentos=f.Comprimento_da_linked_list;
subplot(2,2,2);
s=stem(indices,comprimentos,"Color","#EDB120");
set(get(get(s(1),'Annotation'),'LegendInformation'),'IconDisplayStyle','off'); %Ignore legend for this graph
hold on
plot(0:99,ones(100,1)*24314/100,"black","LineWidth",1)
title("Hash Table de tamanho 100 sem resize | djb2");
xlabel("Índice da tabela");
ylabel("Número de nós da lista ligada");
legend("Média");

f=tdfread("hashtablenoresizesdbm.tsv","\t");
indices=f.x0xC30x8Dndice;
comprimentos=f.Comprimento_da_linked_list;
subplot(2,2,3);
s=stem(indices,comprimentos,"Color","#EDB120");
set(get(get(s(1),'Annotation'),'LegendInformation'),'IconDisplayStyle','off'); %Ignore legend for this graph
hold on
plot(0:99,ones(100,1)*24314/100,"black","LineWidth",1)
title("Hash Table de tamanho 100 sem resize | sdbm");
xlabel("Índice da tabela");
ylabel("Número de nós da lista ligada");
legend("Média");

f=tdfread("hashtablenoresizeloselose.tsv","\t");
indices=f.x0xC30x8Dndice;
comprimentos=f.Comprimento_da_linked_list;
subplot(2,2,4);
s=stem(indices,comprimentos,"Color","#EDB120");
set(get(get(s(1),'Annotation'),'LegendInformation'),'IconDisplayStyle','off'); %Ignore legend for this graph
hold on
plot(0:99,ones(100,1)*24314/100,"black","LineWidth",1)
title("Hash Table de tamanho 100 sem resize | lose lose");
xlabel("Índice da tabela");
ylabel("Número de nós da lista ligada");
legend("Média");