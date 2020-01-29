clc

f=tdfread("hashtabletimeevolution.tsv","\t");
tempo=f.tempo;
tempo=cumsum(tempo);
percentagemOcupacao=f.percentagemOcupacao*100;

figure(1);
plot(tempo,percentagemOcupacao,"Color","#EDB120","LineWidth",2);
title("Evolução da relação elementos adicionados/tamanho ao longo do tempo na hash table");
xlabel("Tempo");
ylabel("Relação elementos adicionados/tamanho (%)");