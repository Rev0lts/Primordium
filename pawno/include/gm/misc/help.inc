CMD:ajudaarmas(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
			/comprararmas - comprar armas em munição / no mercado negro. \n\
			/arma - editar a arma anexada ao personagem: \n\
			/arma editar [slot] - mudar a posição do objeto da arma; \n\
			/arma corpo [slot] - selecionar a parte do corpo à qual a arma está anexada; \n\
			/arma esconder [slot] - esconder o objeto da arma no personagem; \n\
			/arma resetar [slot] - resetar para as configurações iniciais. \n\
			/recarregar - recarregar a arma. \n\
			/optica - colocar / remover a mira óptica. \n\
			/silenciador - colocar / remover o silenciador. \n\
			/armaprimaria - pegar a arma principal. \n\
			/armasecundaria - pegar uma arma secundária. \n\
			/pegarma(n)ual - pegar uma arma branca. \n\
			/guardar(arma) - colocar armas no inventário. \n\
			/soltar(arma) - jogar a arma no chão. \n\
			{FF6347} /ajudamunicao - ajuda sobre munição ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Armas", mes, "Retornar", "Fechar");
	return 1;
}
CMD:ajudamunicao(playerid, params[])
{
	static const dlg[] = "{ffc46a}M1911 /M1911 (silenciador){FFFFFF}\nMunições disponíveis:\n\		
			- .45 ACP (7 tiros)\n\		
			- .45 ACP .ext (14 tiros)\n\n\		
			{ffc46a}Desert Eagle{FFFFFF}\nMunições disponíveis:\n\		
			- .50AE (7 tiros)\n\		
			- .50AE .ext (14 tiros)\n\n\		
			{ffc46a}Shotgun /Sawn-off Shotgun{FFFFFF}\nMunições disponíveis:\n\		
			- 12-G BIRDSHOT (6 tiros)\n\		
    		- 12-G BUCKSHOT (6 tiros)\n\		
    		- 12-G SLUG (6 tiros)\n\		
    		- 12-G BEANBAG (6 tiros) - não letal\n\n\		
			{ffc46a}Tec-9 /MP5 /UZI{FFFFFF}\nMunições disponíveis:\n\		
			- 9x19mm (30 tiros)\n\		
			- 9x19mm .ext (60 tiros)\n\n\		
			{ffc46a}Country Rifle /Sniper Rifle{FFFFFF}\nMunições disponíveis:\n\		
			- 7.62x51mm (4 tiros)\n\n\		
			{ffc46a}M4{FFFFFF}\nMunições disponíveis:\n\		
			- 5.56x45mm (30 tiros)\n\		
			- 5.56x45mm T (30 tiros)\n\		
			- 5.56x45mm (carregador) (60 tiros)\n\		
			- 5.56x45mm (borracha) (30 tiros) - não letal\n\n\		
			{ffc46a}AK-47{FFFFFF}\nMunições disponíveis:\n\		
			- 7.62x39mm (30 tiros)\n\		
			- 7.62x39mm T (30 tiros)\n\		
			- 7.62x39mm (carregador) (60 tiros)";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Munição", mes, "Fechar", "");
	return 1;
}


CMD:ajudadroga(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
				Possíveis interações via inventário ({FF6347} N {FFFFFF} ou {FF6347} /inv {FFFFFF}> Clique na droga):\n\
				- Uso da droga;\n\
				- Coloque a droga em uma caixa de pizza ou sacola do Burger Shot;\n\
				- Dividir, combinar, descartar e outras ações.\n\n\
				{FF6347} /plantar {FFFFFF} - plantar papoula, maconha, coca\n\n\
				Comandos do Laboratório de Drogas:\n\
				{FF6347} /misturardrogas {FFFFFF} - abrir o menu de centrifugação /preparar mistura de reagentes\n\
				{FF6347} /secardrogas {FFFFFF} - abrir o menu da unidade de secagem /pegar reagentes secos\n\n\
				{FF6347} /pegarmistura {FFFFFF} - pegar uma mistura de reagentes do chão\n\
				{FF6347} /largarmistura {FFFFFF} - colocar uma mistura de reagentes no chão\n\n\
				{FF6347} /reator {FFFFFF} - colocar a mistura de reagentes no reator /pegar a droga pronta no reator\n\
				[! ] Use {FF6347} /reator {FFFFFF} para controlar a temperatura do reator durante a síntese. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Drogas", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudatecnica(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
			/alterarsenha - alterar senha.\n\
			/stats - estatísticas do personagem.\n\
			/propriedade - propriedade do personagem.\n\
			/id - procurar por um jogador.\n\
			/isafk - descobrir o tempo de afk do jogador.\n\
			/mm - menu do jogador.\n\
			/setspawn - alterar local de spawn.\n\
			/helpme - fazer uma pergunta à administração.\n\
			/reportar - apresentar uma queixa contra o jogador.\n\
			/solicitar - pedir algo à administração.\n\
			/caridade - doar dinheiro.\n\
			/admins - lista de administradores na rede.\n\
			/testers - lista de testadores da rede.\n\
			/tog - configurações do personagem. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos Técnicos", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudaprincipal(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
    		/pagar - transferir dinheiro.\n\
			/horas - descobrir a hora.\n\
			/abastecer - abastecer o veículo no posto de gasolina.\n\
			/comprarcomida - comprar comida em um quiosque.\n\
			/endereco - encontrar o endereço da casa.\n\
			(/inv)entario - inventário do personagem /casa /porta-malas /compartimento de luvas.\n\
			/itens - acessórios.\n\
			/tirarroupa - tirar a roupa.\n\
			/comprar - abre o menu de compra.\n\
			/licencas - mostrar carteira de identidade.\n\
			/moeda - jogar uma moeda.\n\
			/dado - lançar o dado.\n\
			/(d)rop (arma) - jogar a arma no chão.\n\
			/(p)egar (item) - pegar uma arma /item.\n\
			/cumprimentar - oferecer um aperto de mão.\n\
			/alugaroutdoor - alugar um outdoor.\n\
			/an - enviar um anúncio.\n\
			/cad - anúncio da empresa.\n\
			/ultimosanuncios - últimos anúncios.\n\
			/mascara - colocar /remover a máscara.\n\
			/comprarroupa - comprar roupas.\n\
			/(d) rop (dinheiro) - jogar dinheiro no chão.\n\
			/revistar - oferecer para revistar o jogador.\n\
			/coh - anexar dinheiro à mão.\n\
			/ddinheiro [dinheiro] - jogar a n-ésima quantia de dinheiro no chão.\n\
			/sa - parar a ação.\n\
			/definirestilo - mudar o estilo de caminhar, HUD, estilo de combate, falar.\n\
			/animlist - lista de animações.\n\
			/removercp - remover marcação de GPS.\n\
			/nota - bloco de notas.\n\
			/multas - multas não pagas.\n\
			(/gr) afite - criar grafite. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos Comuns", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudachat(playerid, params[])
{
    static const dlg[] = "{FFFFFF}\
						/ame - ação (sobre o jogador)\n\
						/me - ação (no chat)\n\
						/my - Igual a /do, mas em um formato diferente (Exemplo: 'O telefone de Test_Test caiu') (no chat)\n\
						/do - ação em terceira pessoa, uma descrição de algo.\n\
						/pame - exibe uma descrição do seu personagem ou de outro personagem no chat.\n\
						/mypame - criar descrição do personagem.\n\
						(/l) ocal - chat local.\n\
						(/s) hout - gritar.\n\
						/low - falar em sussurro.\n\
						(/w) hisper - falar em sussurro para um jogador específico.\n\
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
					(/v) ehicle - comandos de veículo\n\
					/vmenu - menu para gerenciar seu veículo\n\
					/vfa - menu para interação rápida com o veículo\n\
					/attmenu - gerenciar os acessórios do seu veículo\n\
					/mycars (/myv) - lista de todos os seus veículos\n\
					/register - registrar o veículo\n\
					/unregister - remover veículos do registro\n\
					(/en) gine - ligar o motor\n\
					/lights - ligar / desligar os faróis\n\
					/trunk - abrir / fechar o porta-malas\n\
					/hood - abrir / fechar o capô\n\
					/setstation (/ss) - mudar de estação de rádio\n\
					/(wi) ndow - abrir / fechar a janela\n\
					/alarmoff - desligar o alarme\n\
					/unimpound - retirar o veículo do depósito\n\
					/delveh - remover veículos roubados\n\
					/vtickets - multas de trânsito não pagas\n\
					/stealradio - roubar o rádio\n\
					/dropkey - largar chaves duplicadas ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Veículo", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudadesmanche(playerid, params[])
{
    static const dlg[] = "{FFFFFF}\
						HACKEANDO VEÍCULOS:\n\
						{FF6347} (Observe que você precisa obter um emprego de 'Ladrão de Carros'.\n\
						Isto é feito para limitar o roubo de carros.) {FFFFFF}\n\n\
						/delivercar - desmontar peças valiosas de um veículo roubado.\n\
						/dropoff - se livrar do veículo.\n\
						/leavemission - redefinir a missão.\n\n\
						/vbl (n) oisy - hackeamento áspero de veículo.\n\
						/vbl (q) uite - hackeamento silencioso de veículo\n\n\
						(/en) gine - iniciar hackeamento do motor\n\
						(/uns) cramble - para decifrar uma palavra (quando o motor está hackeado)\n\n\
						/stealradio - roubar o rádio\n\n\
						HACKEANDO UMA CASA:\n\
						(/hbreak) lock ou /hbl - invadir uma casa";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Hacking", mes, "Retornar", "Fechar");
	return 1;
}


CMD:ajudaradio(playerid, params[])
{
    static const dlg[] = "{FFFFFF}Dica: Você pode pegar um walkie-talkie na loja 24-7!\n\n\
				/r - fale no rádio.\n\
				/rr - fale no rádio no slot.\n\
				/(rb)aixo - fale no rádio em meio sussurro.\n\
				/(rrb)aixo - fale no rádio em meio sussurro no slot.\n\
				/auth - faça login no canal de ondas de rádio.\n\
				/(ca)anal frequencia - mude o canal de rádio.\n\
				/(ca)anal alugar - mude o canal de rádio.\n\
				/setslot - mude o slot do rádio.\n\
				/(ca)anal sair - saia do canal de ondas de rádio.\n\
				/(ca)anal vender - venda o canal de ondas de rádio. (50% será devolvido)\n\
				/newauth - mude a senha do canal.\n\
				/kickoffradio - expulse do canal.\n\
				/radiomembros - membros do canal de ondas de rádio. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Rádio", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudapesca(playerid, params[])
{
	static const dlg[] = "{FFFFFF}AJUDA: Comandos de Pesca:\n\n\
					/myfish - veja quantos peixes você pescou.\n\
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
			/atm - abrir o menu do caixa eletrônico.\n\
			/balance - verificar o saldo da conta bancária.\n\
			/withdraw - sacar dinheiro da conta bancária.\n\
			/deposit - depositar dinheiro na conta.\n\
			/transfer - transferir a quantia de dinheiro para outra conta.\n\
			/paycheck - sacar dinheiro do pagamento.\n\
			/savings - depositar dinheiro em uma conta poupança.\n\
			/tickets - pagar multas. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Banco", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudacelular(playerid, params[])
{
    static const dlg[] = "{FFFFFF}Dica: Você pode pegar um celular na loja 24-7.\n\n\
				/phone - mostrar /ocultar a interface do celular.\n\
				/pc - mostrar o cursor do mouse.\n\
				/call - ligar para o número especificado.\n\
				/loudspeaker - habilitar /desabilitar o alto-falante.\n\
				/low - falar baixo no celular.\n\
				/(p) ickup - atender o celular. (responder à ligação)\n\
				/(h) angup - desligar. (encerrar a ligação)\n\
				/sms - escrever mensagem de texto.\n\
				/togphone - ligar /desligar o celular.\n\
				/dropcell - largar o celular.\n\
				/showcontact - mostrar um contato específico para o jogador.\n\
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
			/door - abrir uma porta dinâmica (/lock - trancar com uma chave).\n\
			/dram - arrombar uma porta dinâmica.\n\
			/furniture - gerenciamento de móveis.\n\
			/grantbuild - dar /retirar a possibilidade de arranjar móveis em conjunto ao jogador.\n\
			/clothes - guarda-roupa em casa.\n\
			/cache - mudar a localização do esconderijo.\n\
			/h upgrade - melhorias na casa\n\
			/h withdraw e /h deposit - retirar /depositar dinheiro.\n\
			/h sellto - vender a casa para alguém.\n\
			/h sell - vender a casa para o estado. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Casa", mes, "Retornar", "Fechar");
	return 1;
}

CMD:ajudaempresa(playerid, params[])
{
	static const dlg[] = "{FFFFFF}\
			/biz comprar - comprar um negócio.\n\
			/biz vender - vender o negócio.\n\
			/biz melhorar - melhorar o negócio.\n\
			/biz informações - informações sobre o negócio.\n\
			/lock - abrir /fechar o negócio.\n\
			/biz taxa - definir o preço para entrar no negócio. (Se posto de gasolina, custo do combustível)\n\
			/biz nome - alterar o nome do negócio.\n\
			/biz texto - alterar o texto de login do negócio.\n\
			/biz retirar - retirar dinheiro da conta do negócio.\n\
			/biz depositar - doar dinheiro para a conta do negócio.\n\
			/biz comprarprod - encomendar mercadorias para o negócio.\n\
			/biz contratar - oferecer a um jogador trabalhar no negócio.\n\
			/biz demitir - demitir um jogador que trabalha no negócio. ";

	new mes[sizeof(dlg)];
	format(mes, sizeof(mes), dlg);
	
	Dialog_Show(playerid, CmdServer2, DIALOG_STYLE_MSGBOX, "Comandos de Negócios", mes, "Retornar", "Fechar");
	return 1;
}


CMD:ajudafaccao(playerid, params[])
{
	if (PlayerInfo[playerid][pFaction] == 0)
		return SCM(playerid, -1, "Você deve estar em uma facção.");

	SCM(playerid, COLOR_GREEN,"|___________________ Comandos de Facção ___________________|");


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
	1. Comandos Técnicos\n\
	2. Comandos Gerais\n\
	3. Comandos de Chat\n\
	4. Comandos da Facção\n\
	5. Comandos de Casa\n\
	6. Comandos de empresa\n\
	7. Comandos de Veículos\n\
	8. Comandos de Banco\n\
	9. Comandos de Telefone\n\
	10. Comandos de Armas\n\
	11. Comandos de Drogas\n\
	12. Comandos de Trabalho\n\
	13. Comandos de Pesca\n\
	14. Comandos de Rádio\n\
	15. Comandos de Hacking ",
	"Selecionar", "Retornar");
	return 1;
}
