O arquivo do projeto j� est� configurado com o allegro, localizado na sua pasta.
O projeto j� est� configurado para visual studio 2013, basta abrir a soluntion do projeto e executar normalmente o jogo.
Caso ocorra um erro de inclusao na biblioteca do allegro : 
    Adicione a pasta do allegro para o C: do computador e execute os seguintes passos:
	-V� na aba do projeto e abra as propriedades do mesmo
	-V� para aba DEPURA��o e adicione na opcao AMBIENTE e coloque : PATH=c:\allegro5010\bin;%PATH%
	-V� para aba C/C++ e na area GERAL entre na opcao de DIRETORIOS DE INCLUSAO ADICIONAIS e adicione o comando: c:\allegro\include
	-V� para a aba de LINKER e na area GERAL entre na opcao DIRETORIOS DE BIBLIOTECAS ADICIONAIS e adicione o comando: c:\allegro5010\lib libs
	-V� para a aba de LINKER e na area ENTRADA entre na opcao DEPENDENCIAS ADICIONAIS, adicione o comando : allegro-5.0.10-monolith-md-debug.lib

FELIPE SANTANA BATISTA DOS SANTOS
