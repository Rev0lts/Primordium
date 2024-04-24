CMD:ajudaarmas(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
			/comprararmas - comprar armas em muni��o / no mercado negro. \n\
			/arma - editar a arma anexada ao personagem: \n\
			/arma editar [slot] - mudar a posi��o do objeto da arma; \n\
			/arma corpo [slot] - selecionar a parte do corpo � qual a arma est� anexada; \n\
			/arma esconder [slot] - esconder o objeto da arma no personagem; \n\
			/arma resetar [slot] - resetar para as configura��es iniciais. \n\
			/recarregar - recarregar a arma. \n\
			/optica - colocar / remover a mira �ptica. \n\
			/silenciador - colocar / remover o silenciador. \n\
			/armaprimaria - pegar a arma principal. \n\
			/armasecundaria - pegar uma arma secund�ria. \n\
			/pegarma(n)ual - pegar uma arma branca. \n\
			/guardar(arma) - colocar armas no invent�rio. \n\
			/soltar(arma) - jogar a arma no ch�o. \n\
			{FF6347} /ajudamunicao - ajuda sobre muni��o ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Armas", mes, "Retornar", "Fechar");
	return 1;
}
CMD:ajudamunicao(playerid, params[])
{
	static const dlg[] = "{ffc46a}M1911 /M1911 (silenciador){FFFFFF}\nMuni��es dispon�veis:\n\		
			- .45 ACP (7 tiros)\n\		
			- .45 ACP .ext (14 tiros)\n\n\		
			{ffc46a}Desert Eagle{FFFFFF}\nMuni��es dispon�veis:\n\		
			- .50AE (7 tiros)\n\		
			- .50AE .ext (14 tiros)\n\n\		
			{ffc46a}Shotgun /Sawn-off Shotgun{FFFFFF}\nMuni��es dispon�veis:\n\		
			- 12-G BIRDSHOT (6 tiros)\n\		
    		- 12-G BUCKSHOT (6 tiros)\n\		
    		- 12-G SLUG (6 tiros)\n\		
    		- 12-G BEANBAG (6 tiros) - n�o letal\n\n\		
			{ffc46a}Tec-9 /MP5 /UZI{FFFFFF}\nMuni��es dispon�veis:\n\		
			- 9x19mm (30 tiros)\n\		
			- 9x19mm .ext (60 tiros)\n\n\		
			{ffc46a}Country Rifle /Sniper Rifle{FFFFFF}\nMuni��es dispon�veis:\n\		
			- 7.62x51mm (4 tiros)\n\n\		
			{ffc46a}M4{FFFFFF}\nMuni��es dispon�veis:\n\		
			- 5.56x45mm (30 tiros)\n\		
			- 5.56x45mm T (30 tiros)\n\		
			- 5.56x45mm (carregador) (60 tiros)\n\		
			- 5.56x45mm (borracha) (30 tiros) - n�o letal\n\n\		
			{ffc46a}AK-47{FFFFFF}\nMuni��es dispon�veis:\n\		
			- 7.62x39mm (30 tiros)\n\		
			- 7.62x39mm T (30 tiros)\n\		
			- 7.62x39mm (carregador) (60 tiros)";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Muni��o", mes, "Fechar", "");
	return 1;
}


CMD:ajudadroga(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
				Poss�veis intera��es via invent�rio ({FF6347} N {FFFFFF} ou {FF6347} /inv {FFFFFF}> Clique na droga):\n\
				- Uso da droga;\n\
				- Coloque a droga em uma caixa de pizza ou sacola do Burger Shot;\n\
				- Dividir, combinar, descartar e outras a��es.\n\n\
				{FF6347} /plantar {FFFFFF} - plantar papoula, maconha, coca\n\n\
				Comandos do Laborat�rio de Drogas:\n\
				{FF6347} /misturardrogas {FFFFFF} - abrir o menu de centrifuga��o /preparar mistura de reagentes\n\
				{FF6347} /secardrogas {FFFFFF} - abrir o menu da unidade de secagem /pegar reagentes secos\n\n\
				{FF6347} /pegarmistura {FFFFFF} - pegar uma mistura de reagentes do ch�o\n\
				{FF6347} /largarmistura {FFFFFF} - colocar uma mistura de reagentes no ch�o\n\n\
				{FF6347} /reator {FFFFFF} - colocar a mistura de reagentes no reator /pegar a droga pronta no reator\n\
				[! ] Use {FF6347} /reator {FFFFFF} para controlar a temperatura do reator durante a s�ntese. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Drogas", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudatecnica(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
			/alterarsenha - alterar senha.\n\
			/stats - estat�sticas do personagem.\n\
			/propriedade - propriedade do personagem.\n\
			/id - procurar por um jogador.\n\
			/isafk - descobrir o tempo de afk do jogador.\n\
			/mm - menu do jogador.\n\
			/setspawn - alterar local de spawn.\n\
			/helpme - fazer uma pergunta � administra��o.\n\
			/reportar - apresentar uma queixa contra o jogador.\n\
			/solicitar - pedir algo � administra��o.\n\
			/caridade - doar dinheiro.\n\
			/admins - lista de administradores na rede.\n\
			/testers - lista de testadores da rede.\n\
			/tog - configura��es do personagem. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos T�cnicos", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudaprincipal(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
    		/pagar - transferir dinheiro.\n\
			/horas - descobrir a hora.\n\
			/abastecer - abastecer o ve�culo no posto de gasolina.\n\
			/comprarcomida - comprar comida em um quiosque.\n\
			/endereco - encontrar o endere�o da casa.\n\
			(/inv)entario - invent�rio do personagem /casa /porta-malas /compartimento de luvas.\n\
			/itens - acess�rios.\n\
			/tirarroupa - tirar a roupa.\n\
			/comprar - abre o menu de compra.\n\
			/licencas - mostrar carteira de identidade.\n\
			/moeda - jogar uma moeda.\n\
			/dado - lan�ar o dado.\n\
			/(d)rop (arma) - jogar a arma no ch�o.\n\
			/(p)egar (item) - pegar uma arma /item.\n\
			/cumprimentar - oferecer um aperto de m�o.\n\
			/alugaroutdoor - alugar um outdoor.\n\
			/an - enviar um an�ncio.\n\
			/cad - an�ncio da empresa.\n\
			/ultimosanuncios - �ltimos an�ncios.\n\
			/mascara - colocar /remover a m�scara.\n\
			/comprarroupa - comprar roupas.\n\
			/(d) rop (dinheiro) - jogar dinheiro no ch�o.\n\
			/revistar - oferecer para revistar o jogador.\n\
			/coh - anexar dinheiro � m�o.\n\
			/ddinheiro [dinheiro] - jogar a n-�sima quantia de dinheiro no ch�o.\n\
			/sa - parar a a��o.\n\
			/definirestilo - mudar o estilo de caminhar, HUD, estilo de combate, falar.\n\
			/animlist - lista de anima��es.\n\
			/removercp - remover marca��o de GPS.\n\
			/nota - bloco de notas.\n\
			/multas - multas n�o pagas.\n\
			(/gr) afite - criar grafite. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos Comuns", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudachat(playerid, params[])
{
    static const dlg[] = "{FFFFFF}\
						/ame - a��o (sobre o jogador)\n\
						/me - a��o (no chat)\n\
						/my - Igual a /do, mas em um formato diferente (Exemplo: 'O telefone de Test_Test caiu') (no chat)\n\
						/do - a��o em terceira pessoa, uma descri��o de algo.\n\
						/pame - exibe uma descri��o do seu personagem ou de outro personagem no chat.\n\
						/mypame - criar descri��o do personagem.\n\
						(/l) ocal - chat local.\n\
						(/s) hout - gritar.\n\
						/low - falar em sussurro.\n\
						(/w) hisper - falar em sussurro para um jogador espec�fico.\n\
						/b - chat local fora do personagem.\n\
						/ooc - chat global fora do personagem.\n\
						/pm - enviar uma mensagem privada. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Chat", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudaveiculo(playerid, params[])
{
    static const dlg[] = "{FFFFFF}\
					(/v) ehicle - comandos de ve�culo\n\
					/vmenu - menu para gerenciar seu ve�culo\n\
					/vfa - menu para intera��o r�pida com o ve�culo\n\
					/attmenu - gerenciar os acess�rios do seu ve�culo\n\
					/mycars (/myv) - lista de todos os seus ve�culos\n\
					/register - registrar o ve�culo\n\
					/unregister - remover ve�culos do registro\n\
					(/en) gine - ligar o motor\n\
					/lights - ligar / desligar os far�is\n\
					/trunk - abrir / fechar o porta-malas\n\
					/hood - abrir / fechar o cap�\n\
					/setstation (/ss) - mudar de esta��o de r�dio\n\
					/(wi) ndow - abrir / fechar a janela\n\
					/alarmoff - desligar o alarme\n\
					/unimpound - retirar o ve�culo do dep�sito\n\
					/delveh - remover ve�culos roubados\n\
					/vtickets - multas de tr�nsito n�o pagas\n\
					/stealradio - roubar o r�dio\n\
					/dropkey - largar chaves duplicadas ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Ve�culo", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudadesmanche(playerid, params[])
{
    static const dlg[] = "{FFFFFF}\
						HACKEANDO VE�CULOS:\n\
						{FF6347} (Observe que voc� precisa obter um emprego de 'Ladr�o de Carros'.\n\
						Isto � feito para limitar o roubo de carros.) {FFFFFF}\n\n\
						/delivercar - desmontar pe�as valiosas de um ve�culo roubado.\n\
						/dropoff - se livrar do ve�culo.\n\
						/leavemission - redefinir a miss�o.\n\n\
						/vbl (n) oisy - hackeamento �spero de ve�culo.\n\
						/vbl (q) uite - hackeamento silencioso de ve�culo\n\n\
						(/en) gine - iniciar hackeamento do motor\n\
						(/uns) cramble - para decifrar uma palavra (quando o motor est� hackeado)\n\n\
						/stealradio - roubar o r�dio\n\n\
						HACKEANDO UMA CASA:\n\
						(/hbreak) lock ou /hbl - invadir uma casa";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Hacking", mes, "Retornar", "Fechar");
	return 1;
}


CMD:ajudaradio(playerid, params[])
{
    static const dlg[] = "{FFFFFF}Dica: Voc� pode pegar um walkie-talkie na loja 24-7!\n\n\
				/r - fale no r�dio.\n\
				/rr - fale no r�dio no slot.\n\
				/(rb)aixo - fale no r�dio em meio sussurro.\n\
				/(rrb)aixo - fale no r�dio em meio sussurro no slot.\n\
				/auth - fa�a login no canal de ondas de r�dio.\n\
				/(ca)anal frequencia - mude o canal de r�dio.\n\
				/(ca)anal alugar - mude o canal de r�dio.\n\
				/setslot - mude o slot do r�dio.\n\
				/(ca)anal sair - saia do canal de ondas de r�dio.\n\
				/(ca)anal vender - venda o canal de ondas de r�dio. (50% ser� devolvido)\n\
				/newauth - mude a senha do canal.\n\
				/kickoffradio - expulse do canal.\n\
				/radiomembros - membros do canal de ondas de r�dio. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de R�dio", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudapesca(playerid, params[])
{
	static const dlg[] = "{FFFFFF}AJUDA: Comandos de Pesca:\n\n\
					/myfish - veja quantos peixes voc� pescou.\n\
					/unloadfish - entregue os peixes.\n\
					/fish - pesque.\n\
					/stopfish - pare de pescar.\n\
					/fishzone - mostra as zonas de pesca. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Pesca", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudabanco(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
			/atm - abrir o menu do caixa eletr�nico.\n\
			/balance - verificar o saldo da conta banc�ria.\n\
			/withdraw - sacar dinheiro da conta banc�ria.\n\
			/deposit - depositar dinheiro na conta.\n\
			/transfer - transferir a quantia de dinheiro para outra conta.\n\
			/paycheck - sacar dinheiro do pagamento.\n\
			/savings - depositar dinheiro em uma conta poupan�a.\n\
			/tickets - pagar multas. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Banco", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudacelular(playerid, params[])
{
    static const dlg[] = "{FFFFFF}Dica: Voc� pode pegar um celular na loja 24-7.\n\n\
				/phone - mostrar /ocultar a interface do celular.\n\
				/pc - mostrar o cursor do mouse.\n\
				/call - ligar para o n�mero especificado.\n\
				/loudspeaker - habilitar /desabilitar o alto-falante.\n\
				/low - falar baixo no celular.\n\
				/(p) ickup - atender o celular. (responder � liga��o)\n\
				/(h) angup - desligar. (encerrar a liga��o)\n\
				/sms - escrever mensagem de texto.\n\
				/togphone - ligar /desligar o celular.\n\
				/dropcell - largar o celular.\n\
				/showcontact - mostrar um contato espec�fico para o jogador.\n\
				/showcontacts - mostrar todos os contatos para outro jogador. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Celular", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudacasa(playerid, params[])
{
    static const dlg[] = "{FFFFFF} \
			/h buy - comprar uma casa\n\
			/hmenu - menu da casa.\n\
			/lock - abrir /fechar a casa.\n\
			/h backdoor - definir porta traseira\n\
			/rentroom - alugar uma casa.\n\
			/door - abrir uma porta din�mica (/lock - trancar com uma chave).\n\
			/dram - arrombar uma porta din�mica.\n\
			/furniture - gerenciamento de m�veis.\n\
			/grantbuild - dar /retirar a possibilidade de arranjar m�veis em conjunto ao jogador.\n\
			/clothes - guarda-roupa em casa.\n\
			/cache - mudar a localiza��o do esconderijo.\n\
			/h upgrade - melhorias na casa\n\
			/h withdraw e /h deposit - retirar /depositar dinheiro.\n\
			/h sellto - vender a casa para algu�m.\n\
			/h sell - vender a casa para o estado. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Casa", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudaempresa(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
			/biz comprar - comprar um neg�cio.\n\
			/biz vender - vender o neg�cio.\n\
			/biz melhorar - melhorar o neg�cio.\n\
			/biz informa��es - informa��es sobre o neg�cio.\n\
			/lock - abrir /fechar o neg�cio.\n\
			/biz taxa - definir o pre�o para entrar no neg�cio. (Se posto de gasolina, custo do combust�vel)\n\
			/biz nome - alterar o nome do neg�cio.\n\
			/biz texto - alterar o texto de login do neg�cio.\n\
			/biz retirar - retirar dinheiro da conta do neg�cio.\n\
			/biz depositar - doar dinheiro para a conta do neg�cio.\n\
			/biz comprarprod - encomendar mercadorias para o neg�cio.\n\
			/biz contratar - oferecer a um jogador trabalhar no neg�cio.\n\
			/biz demitir - demitir um jogador que trabalha no neg�cio. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Neg�cios", mes, "Retornar", "Fechar");
	return 1;
}


CMD:ajudafaccao(playerid, params[])
{
	if (PlayerInfo[playerid][pFaction] == 0)
		return SCM(playerid, -1, "Voc� deve estar em uma fac��o.");

	SCM(playerid, COLOR_GREEN,"|___________________ Comandos de Fac��o ___________________|");


	if (isLeader(playerid))
	{
		if (Factions[PlayerInfo[playerid][pFaction]][fType] == FACTION_GANG || Factions[PlayerInfo[playerid][pFaction]][fType] == FACTION_MAFIA)
			SCM(playerid, COLOR_YELLOWG,"Leader commands: /editrights /invite /uninvite /giverank /fchat /ranks /towcars /editspawn /editrankname /factionjob");
		else
		    SCM(playerid, COLOR_YELLOWG,"Leader commands: /editrights /invite /uninvite /giverank /fchat /ranks /towcars /editspawn /editrankname /changebadges");
	}
	
	if (IsPlayerFactionPolice(playerid))
	{
		SCM(playerid, COLOR_WHITE, "[SHERIFF] /cuff /uncuff (/m)egaphone /meg /mdc /(gov)ernment /showbadge");
		SCM(playerid, COLOR_WHITE, "[SHERIFF] (/r)adio (/r 10-27) (/r 10-28) /rlow /bort /(b)ort(l)ist /arrest");
		SCM(playerid, COLOR_WHITE, "[SHERIFF] /detain /revoke /apb /prison /toll /(sp)ike /cctv /exitcctv /tow /rope");
		SCM(playerid, COLOR_WHITE, "[SHERIFF] /impound /callsign /color /open(/gate) /authswat /siren /checkplate ");
		SCM(playerid, COLOR_WHITE, "[SHERIFF] /ram /dram /trace /pull /myrights /setplate /ticket /(v)ehicle(ticket) ");
		SCM(playerid, COLOR_WHITE, "[SHERIFF] /checkfines /checkvfines /ane /radar /carfix /members /zond /flashbang /taser");
		SCM(playerid, COLOR_WHITE, "[SHERIFF] /(f)action(a)ttach /chase /frisktrunk /friskgb /friskprop /frisk /carsign /street /speedcam");
	}
    else if (IsPlayerFactionCity(playerid)) 
	{
		SCM(playerid, COLOR_WHITE,"[CITY] /flist /(d)epartments /(f)action /showbadge /(r)adio /rlow /(m)egaphone /(gov)ernment");
		SCM(playerid, COLOR_WHITE,"[CITY] /bort /(b)ort(l)ist /cuff /uncuff /siren");
	}
	else if (IsPlayerFactionMedic(playerid))
	{
		SCM(playerid, COLOR_WHITE,"[FIRE /flist (/d)epartments (/f)action /showbadge (/r)adio /rlow /bort /(b)ort(l)ist /stretcher");
		SCM(playerid, COLOR_WHITE,"[FIRE] /heal /pull /operation /hpc /siren (/m)egaphone (/gov)ernment /prescribe /therapy /(f)action(a)ttach");
	}
	else if (IsPlayerFactionFire(playerid))
	{
		SCM(playerid, COLOR_WHITE,"[EMS] /flist (/d)epartments (/f)action /showbadge (/r)adio /rlow /bort /(b)ort(l)ist /stretcher");
		SCM(playerid, COLOR_WHITE,"[EMS] /heal /pull /operation /hpc /siren (/m)egaphone (/gov)ernment /(f)action(a)ttach /choke");
	}
	else if (IsPlayerFactionDocs(playerid))
	{
		SCM(playerid, COLOR_WHITE,"[DOC] /flist /(d)epartments /(f)action /showbadge /(r)adio /rlow /(m)egaphone /(gov)ernment");
		SCM(playerid, COLOR_WHITE,"[DOC] /siren /detain /bort /(b)ort(l)ist /cuff /uncuff /frisk /taser /prisonlist /prison /free /isolate");
	}
	else if (IsPlayerFactionNews(playerid))
	{
		SCM(playerid, COLOR_WHITE,"[SAN] /flist /(f)action /news /exitnews /interview /exitinterview");
	}
	return 1;
}

CMD:ajuda(playerid, params[])
{
    Dialog_Show(playerid, CmdServer, DIALOG_STYLE_LIST, "Comandos do Servidor", "\
	1. Comandos T�cnicos\n\
	2. Comandos Gerais\n\
	3. Comandos de Chat\n\
	4. Comandos da Fac��o\n\
	5. Comandos de Casa\n\
	6. Comandos de empresa\n\
	7. Comandos de Ve�culos\n\
	8. Comandos de Banco\n\
	9. Comandos de Telefone\n\
	10. Comandos de Armas\n\
	11. Comandos de Drogas\n\
	12. Comandos de Trabalho\n\
	13. Comandos de Pesca\n\
	14. Comandos de R�dio\n\
	15. Comandos de Hacking ",
	"Selecionar", "Retornar");
	return 1;
}