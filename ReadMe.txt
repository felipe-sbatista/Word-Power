Aplicação desenvolvida para cadeira de Prática de programação.
O jogo é constituido de palavras em inglês que são embaralhadas e deve-se coloca-las na ordem certa para forma a palavra. 
Caso o jogador erre, é exibida a palavra correta e sua tradução para o aprendizado do player.


Execução
O arquivo do projeto já está configurado com o allegro, localizado na sua pasta.
O projeto já está configurado para visual studio 2013, basta abrir a soluntion do projeto e executar normalmente o jogo.
Caso ocorra um erro de inclusao na biblioteca do allegro: 
    Adicione a pasta do allegro para o C: do computador e execute os seguintes passos:
	-Vá na aba do projeto e abra as propriedades do mesmo
	-Vá para aba DEPURAÇÃo e adicione na opcao AMBIENTE e coloque : PATH=c:\allegro5010\bin;%PATH%
	-Vá para aba C/C++ e na area GERAL entre na opcao de DIRETORIOS DE INCLUSAO ADICIONAIS e adicione o comando: c:\allegro\include
	-Vá para a aba de LINKER e na area GERAL entre na opcao DIRETORIOS DE BIBLIOTECAS ADICIONAIS e adicione o comando: c:\allegro5010\lib libs
	-Vá para a aba de LINKER e na area ENTRADA entre na opcao DEPENDENCIAS ADICIONAIS, adicione o comando : allegro-5.0.10-monolith-md-debug.lib


