%%The
clc

f=tdfread("theoccurences.tsv","\t");
indice=f.indice;
ocorrencias=f.ocorrencias;

figure('Name','Ocorrências da palavra the');
stem(flip(indice),flip(ocorrencias),"Color","#EDB120");
title("Ocorrências da palavra 'the'");
camroll(90);
xlabel("Índice das palavras lidas");
ylabel("Número de ocorrências");


%% Window
clc

fw=tdfread("windowoccurences.tsv","\t");
indicew=fw.indice;
ocorrenciasw=fw.ocorrencias;

figure('Name','Ocorrências da palavra window');
stem(indicew,ocorrenciasw,"Color","#d49a11");
title("Ocorrências da palavra 'window'");
camroll(90);
xlabel("Índice das palavras lidas");
ylabel("Número de ocorrências");

%% Wood

clc

fw=tdfread("woodoccurences.tsv","\t");
indicewood=fw.indice;
ocorrenciaswood=fw.ocorrencias;

figure('Name','Ocorrências da palavra wood');
stem(indicewood,ocorrenciaswood,"Color","#a5780d");
title("Ocorrências da palavra 'wood'");
camroll(90);
xlabel("Índice das palavras lidas");
ylabel("Número de ocorrências");