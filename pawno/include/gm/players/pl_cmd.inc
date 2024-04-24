ALTX:setspawn("/spawnpoint");
CMD:setspawn(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type))	return SendHelpMessage(playerid, "/setspawn [0 - ponto de respawn padrão, 1 - casa, 2 - facção, 3 - última saída] ");
	if (type > 3 || type < 0)		return SendHelpMessage(playerid, "/setspawn [0 - ponto de respawn padrão, 1 - casa, 2 - facção, 3 - última saída]");

	if (type == 0)
	{
	    Dialog_Show(playerid, SelectSpawn, DIALOG_STYLE_LIST, "{8D8DFF}Selecione o local de respawn "," Palomino Creek \nMontgomery \nDillimore \nFort Carson ", "Selecionar", "Cancelar");
		return 1;
	}
	else if (type == 1)
	{
        Dialog_Show(playerid, ChangeHouseSpawn, DIALOG_STYLE_INPUT, "{"COL_TITLE"}[Escolha uma casa]", "Digite o ID da casa em que deseja renascer. Você pode descobrir o ID da casa usando /propriedade.", "Selecionar","Cancelar");
	}
	else if (type == 2)
	{
		if (!PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "Você não é membro de uma facção.");

		SendSyntaxMessage(playerid, "Agora você renascerá no ponto de respawn da facção.");
		PlayerInfo[playerid][pSpawnType] = 2;
	}
	else if (type == 3)
	{
		SendSyntaxMessage(playerid, "Agora você renascerá no último local de saída. ");
		PlayerInfo[playerid][pSpawnType] = 3;
	}

	SQL_SetInteger("users", "spawntype", PlayerInfo[playerid][pSpawnType], PlayerInfo[playerid][pID]);
	return 1;
}


ALTX:location("/street","/rua");
CMD:location(playerid, params[])
{
	new location[MAX_ZONE_NAME],
		Float:x, Float:y, Float:z;

	GetPlayerPos(playerid, x, y, z);
	GetZone(x, y, location, MAX_ZONE_NAME);

	if (IsPlayerInAnyDynamicArea(playerid)) {
		new areas[5], street_area[2], j = GetPlayerNumberDynamicAreas(playerid);
		GetPlayerDynamicAreas(playerid, areas, sizeof(areas));

		if (GetStreet(areas, j, street_area)) {
			if (street_area[1] == -1)
				return SendSpecialMessage(playerid, "%s, %s.", location, street_names[street_area[0]]);
			else
				return SendSpecialMessage(playerid, "%s, no cruzamento entre %s e %s.", location, street_names[street_area[0]], street_names[street_area[1]]);
		}
	}

	SendSpecialMessage(playerid, "%s.", location);
	return 1;
}

ALTX:address("/adres", "/adress", "addres","/endereco");
CMD:endereco(playerid, params[])
{
	new houseid;
	if (sscanf(params,"d", houseid)) 			return SendHelpMessage(playerid, "/endereco [ID da Casa]");
	if (houseid < 0 || houseid >= MAX_HOUSES)	return SendErrorMessage(playerid,"Essa casa não existe.");
	if (!HouseInfo[houseid][hID])				return SendErrorMessage(playerid, "Endereço não encontrado.");
	
	if (HouseInfo[houseid][hApart])	Waypoint_Set(playerid, ApartInfo[HouseInfo[houseid][hApart]][aPosX], ApartInfo[HouseInfo[houseid][hApart]][aPosY], ApartInfo[HouseInfo[houseid][hApart]][aPosZ]);
	else 							Waypoint_Set(playerid, HouseInfo[houseid][hPosX], HouseInfo[houseid][hPosY], HouseInfo[houseid][hPosZ]);
			
	SendSyntaxMessage(playerid, "Casa (%s) marcada no GPS. (/removercp)", (!HouseInfo[houseid][hApart])?(GetHouseZone(houseid)):(GetApartmentZone(HouseInfo[houseid][hApart])));
	return 1;
}


CMD:pagar(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) 
		return SendErrorMessage(playerid, "Você não pode usar essa função agora.");
		
	new userid, amount;
 
	if (sscanf(params,"ud", userid, amount))				return SendHelpMessage(playerid, "/pagar [ID/Parte do Nome] [dinheiro]");
	if (userid == playerid)								return 1;
	if (PlayerInfo[playerid][pHours] < 2)				return SendErrorMessage(playerid, "Você jogou menos de 2 horas no servidor. Esta função não está disponível.");
	if (GetPVarInt(playerid, #just_paid) > gettime())	return SendErrorMessage(playerid, "Aguarde 2 segundos.");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (amount > GetCash(playerid) || amount <= 0)		return SendNotCash(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))		return SendNotNearPlayer(playerid);

	SetPVarInt(playerid, #just_paid, gettime() + 2);

	new money1 = PlayerInfo[playerid][pCash], money2 = PlayerInfo[userid][pCash];

	GiveCash(playerid, -amount);
	GiveCash(userid, amount);
	PlaySound(playerid, 1052);
	PlaySound(userid, 1052);
	SendSyntaxMessage(playerid, "Você passou $%i para %s [ID:%i].", amount, GetNameWithMask(userid), userid);
	SendSyntaxMessage(userid, "Você recebeu $%i de %s [ID:%i].", amount, GetNameWithMask(playerid), playerid);
	SendAdmMessage("%s (ID: %i) transferiu para %s (ID: %i) $%i", GetNameEx(playerid), playerid, GetNameEx(userid), userid, amount);

	new mes[256];
	format(mes, 128, "tira dinheiro e entrega para %s.", GetNameWithMask(userid));
	cmd::me(playerid, mes);

    format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro(ERA): %i | Dinheiro(FICOU): %i] transferiu dinheiro para %s [MYSQL: %i | IP: %s | Dinheiro(ERA): %i | Dinheiro(FICOU): %i] -- $%i", 
	GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money1, GetCash(playerid), GetName(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid), money2, GetCash(userid), amount);
    insert_log(mes, MONEY_LOG, 1, playerid);
	return 1;
}

CMD:licencas(playerid, params[])
{
	new userid;
	if (sscanf(params,"u", userid))	return SendHelpMessage(playerid, "/licencas [ID/Parte do Nome]");
	if (userid == playerid)
	{
		cmd::server_ame(playerid, "olha para sua carteira de identidade.");
		ShowLicenses(playerid, playerid);
		return 1;
	}
	
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(8.0, playerid, userid))	return SendNotNearPlayer(playerid);

	new mes[128];
	format(mes, sizeof(mes), "mostra a carteira de identidade de %s.", GetNameWithMask(userid));
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
	ShowLicenses(playerid, userid);
	return 1;
}

CMD:id(playerid, params[])
{
	if (isnull(params))		return SendHelpMessage(playerid, "/id [ID/Parte do Nome]");
	if (isNumeric(params))
	{
	    new userid = strval(params);
	    if (!IsPlayerLogged(userid))	return SendErrorMessage(playerid, "A busca não retornou nenhum resultado.");
		
	    SendClientMessageEx(playerid, COLOR_GREY, "[ID: %i] %s [%s]", userid, GetNameEx(userid), GetAccName(userid));
	    return 1;
	}

	new count;
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || strfind(GetName(i), params, true) == -1) continue;
	        
        SendClientMessageEx(playerid, COLOR_GREY, "[ID: %i] %s [%s]", i, GetNameEx(i), GetAccName(i));
        count = 1;
	}
	
	if (!count) SendErrorMessage(playerid, "A busca não retornou nenhum resultado.");
	return 1;
}
CMD:admins(playerid, params[])
{
	SCM(playerid, COLOR_WHITE, "Administradores online:");

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i)) continue;

		if (PlayerInfo[i][pAdmin] > 0 || !PlayerInfo[i][pAdminHide]) {
			new admin[12];
			
			if (PlayerInfo[i][pAdmin] == DEVELOPER_LEVEL)		admin = "Leadership";
			else if (PlayerInfo[i][pAdmin] == HIGH_ADMIN_LEVEL) 	admin = "Lead Admin";
			else												format(admin, sizeof(admin), "Admin %i", PlayerInfo[i][pAdmin]);

			if (!IsAdminDuty(i))	SendClientMessageEx(playerid, COLOR_LIGHTRED, "{C7C7C7}%s: %s (%s, ID: %i) Status: Indisponível", admin, GetNameEx(i), GetAccName(i), i);
			else				SendClientMessageEx(playerid, COLOR_GREEN2, "{FFFFFF}%s: %s (%s, ID: %i) Status: {669900}Disponível", admin, GetNameEx(i), GetAccName(i), i);
		} else if (PlayerInfo[i][pHelper] > 0 || !PlayerInfo[i][pAdminHide]) {
			if (!IsTesterDuty(i))	SendClientMessageEx(playerid, COLOR_LIGHTRED, "{C7C7C7}Tester: %s (ID: %i, %s) Status: Indisponível", GetNameEx(i), i, GetAccName(i));
			else					SendClientMessageEx(playerid, COLOR_GREEN2, "{FFFFFF}Tester: %s [ID:%i] [%s] (Em serviço)", GetNameEx(i), i, GetAccName(i));
		}
	}
	return 1;
}

CMD:testers(playerid, params[])
{
	SCM(playerid, COLOR_WHITE, "Testers online:");
	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pHelper] < 1) continue;

        if (!IsTesterDuty(i))	SendClientMessageEx(playerid, COLOR_LIGHTRED, "{C7C7C7}Tester: %s (ID: %i, %s) Status: Indisponível", GetNameEx(i), i, GetAccName(i));
		else					SendClientMessageEx(playerid, COLOR_GREEN2, "{FFFFFF}Tester: %s (ID:%i, %s) Status: {669900}Disponível", GetNameEx(i), i, GetAccName(i));
	}
	return 1;
}

CMD:blockpm(playerid, params[])
{
    if (PlayerInfo[playerid][pPremium] < 1) return SendErrorMessage(playerid, "Esta função está disponível apenas para contas premium.");
	
	new userid;

	if (sscanf(params,"u", userid))	return SendHelpMessage(playerid, "/blockpm [ID/Parte do Nome]");
	if (userid == playerid)			return 1;
	if (!IsPlayerLogged(userid)) 	return SendNotConnected(playerid);

    if (people_blocking[playerid][userid] == 1)
    {
		SendSyntaxMessage(playerid, "Você removeu %s [ID: %i] da lista negra de PM.", GetNameEx(userid), userid);
		people_blocking[playerid][userid] = 0;
		return 1;
	}

    SendSyntaxMessage(playerid, "Você adicionou %s [ID: %i] à lista negra de PM.", GetNameEx(userid), userid);
	people_blocking[playerid][userid] = 1;
	return 1;
}
CMD:listanegra(playerid, params[])
{
	if (PlayerInfo[playerid][pPremium] < 1) return SendErrorMessage(playerid, "Este recurso só está disponível para contas premium");

	SCM(playerid, COLOR_LIGHTRED, "|______LISTA NEGRA "ProjectName"______|");
	foreach(new i : Player)
	{
		if (people_blocking[playerid][i] == 0) continue;

		SendClientMessageEx(playerid, COLOR_INFO, "[ID: %i] %s", i, GetNameEx(i));
	}
	return 1;
}

CMD:hora(playerid, params[])
{
	new hour, minute, mes[128];

	gettime(hour,minute);
	format(mes, sizeof(mes), "~w~%02d:%02d~n~Hora do Salário: %02i min", hour, minute, 60 - PlayerInfo[playerid][pPayDayTime]);

	if (PlayerInfo[playerid][pMutedTime] > 0)	format(mes, sizeof(mes), "%s~n~~r~Tempo restante de silêncio: %i", mes, PlayerInfo[playerid][pMutedTime]);

	if (PlayerInfo[playerid][pJailed] == PALOMINO_JAIL)
	{
		format(mes, sizeof(mes), "~y~Tempo restante de prisão: %i~n~%s", PlayerInfo[playerid][pJailTime], mes);
		GameTextForPlayer(playerid, mes, 5000, 1);
		ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
		return 1;
	}
	else if (PlayerInfo[playerid][pIsolateTime] != 0)
	{
		format(mes, sizeof(mes), "~y~Tempo restante de isolamento: %i~n~%s", PlayerInfo[playerid][pIsolateTime], mes);
		GameTextForPlayer(playerid, mes, 5000, 1);
		ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
		return 1;
	}
	else if (PlayerInfo[playerid][pJailed] == PRISON_JAIL)
	{
		format(mes, sizeof(mes), "~y~Tempo restante na prisão: %i~n~%s", PlayerInfo[playerid][pJailTime], mes);
		GameTextForPlayer(playerid, mes, 5000, 1);
		ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
		return 1;
	}
	else
	{
		GameTextForPlayer(playerid, mes, 5000, 1);
		cmd::server_ame(playerid, "olha para o relógio.");
		
		if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)	ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
	}
	return 1;
}

CMD:moeda(playerid, params[])
{
	SendNearbyMessage(Range_Normal, playerid, COLOR_WHITE, "* %s joga uma moeda e sai %s.", GetNameWithMask(playerid), (random(2) == 0)?("cara"):("coroa"));
	return 1;
}

CMD:dado(playerid, params[])
{
	if (!IsPlayerHasItem(playerid, INV_DICE))	return SendErrorMessage(playerid, "Você não tem dados.");

	SendNearbyMessage(Range_Normal, playerid, COLOR_WHITE, "* %s joga o dado e cai %i.", GetNameWithMask(playerid), random(6)+1);
	return 1;
}

CMD:mascara(playerid, params[])
{
	if (!IsPlayerHasItem(playerid, INV_MASK))	return SendErrorMessage(playerid, "Você não tem uma máscara.");
	if (PlayerInfo[playerid][pOnDuty])			return SendErrorMessage(playerid, "Você não pode usar uma máscara em serviço!");

	if (!maskOn{playerid})
	{
		RemoveInvItem(playerid, INV_MASK, 1);

	    cmd::server_ame(playerid, "* Coloca uma máscara. ");
		GameTextForPlayer(playerid, "~w~VOCE ~g~COLOCOU ~w~MASCARA", 4000, 4);
		maskOn{playerid} = true;
		ShowPlayerName(playerid, false);
		format(PlayerInfo[playerid][pMaskname], 32, "Mascara_%i", randomEx(100000, 999999));
		cNametag[playerid] = CreateDynamic3DTextLabel("Coloca uma máscara...", 0xFFFFFFFF, 0.0, 0.0, 0.1, NT_DISTANCE, .attachedplayer = playerid, .testlos = 1);
	}
	else
	{
		GiveInvItem(playerid, INV_MASK, 1);

	    cmd::server_ame(playerid, "* Remove a máscara. ");
		GameTextForPlayer(playerid, "~w~VOCE ~r~REMOVEU ~w~MASCARA", 4000, 4);
		RemovePlayerMask(playerid);
	}
	return 1;
}

CMD:isafk(playerid, params[])
{
	new id;
	
	if (sscanf(params, "u", id))	return SendHelpMessage(playerid, "/isafk [ID/Parte do Nome]");
	if (!IsPlayerLogged(id))		return SendNotConnected(playerid);

	if (IsPlayerAFK(id))	SendClientMessageEx(playerid, COLOR_LIGHTRED, "[ ! ] Jogador %s [ID: %i] está AFK há %i segundos.", GetNameEx(id), id, pTemp[id][pDarkAFK]-2);
	else				SendClientMessageEx(playerid, COLOR_LIGHTRED, "[ ! ] Jogador %s [ID: %i] não está AFK.", GetNameEx(id), id);
	return 1;
}

CMD:mostrarprescricao(playerid, params[])
{
    if (PlayerInfo[playerid][pPrescribe] < gettime())	return SendErrorMessage(playerid, "Você não tem uma prescrição para maconha medicinal. ");

    new userid;

	if (sscanf(params,"u",userid))					return SendHelpMessage(playerid, "/mostrarprescricao [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
		
    SCM(userid, COLOR_GREEN, "__________Receita__________");
	SendClientMessageEx(userid, COLOR_WHITE, "Emitido para: %s Data de Nascimento: %s", GetNameEx(playerid), PlayerInfo[playerid][pBirthdate]);
	SendClientMessage(userid, COLOR_WHITE, "Emitido por: All Saints General Hospital.");
	SendClientMessage(userid, COLOR_WHITE, "Prescrição para a compra, armazenamento e uso de maconha medicinal. ");
	SCM(userid, COLOR_GREEN, "_________________________________");
	
	new mes[128];
	format(mes, sizeof(mes), "mostra a receita para %s.", GetNameWithMask(userid));
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
	return 1;
}

CMD:stats(playerid, params[])
{
	ShowStats(playerid, playerid);
	return 1;
}

CMD:caridade(playerid, params[])
{
	new amount;

	if (sscanf(params,"d",amount))					return SendHelpMessage(playerid, "/caridade [dinheiro]");
	if (amount > GetCash(playerid) || amount <= 0)	return SendNotCash(playerid);

	PlaySound(playerid, 1052);
	SendSyntaxMessage(playerid, "Você doou $%i.", amount);
	AMSG(COLOR_LIGHTRED, "Aviso: %s doou (/caridade) $%i.", GetNameEx(playerid), amount);

	new money = PlayerInfo[playerid][pCash];
	GiveCash(playerid, -amount);

	new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(DEPOIS): %i] doou (/caridade) -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), amount);
    insert_log(mes, MONEY_LOG, 5, PlayerInfo[playerid][pAccID]);
	return 1;
}

CMD:meunumerodeprisao(playerid, params[])
{
	if (PlayerInfo[playerid][pJailed] != PRISON_JAIL)	return SendErrorMessage(playerid, "Você não está na prisão.");

	SendClientMessageEx(playerid, COLOR_GRAD2, "Nome: %s | Número na Prisão: %s%i", GetNameEx(playerid), (PlayerInfo[playerid][pIsolateTime] != 0)?("B"):("A"),PlayerInfo[playerid][pPrisoned]+100);
	return 1;
}

CMD:pergunta(playerid, params[])
{
	if (isnull(params))	return SendHelpMessage(playerid, "/pergunta [texto]");

	SendSyntaxMessage(playerid, "Sua solicitação de assistência foi enviada com sucesso. Aguarde uma resposta.");

    if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i)) || (PlayerInfo[i][pSettings] & togService))	continue;
				
			SendClientMessageEx(i, 0x00CC99AA, "[Pergunta] %s [ID: %i]: %.*s ...", GetNameEx(playerid), playerid, pos, params);
			SendClientMessageEx(i, 0x00CC99AA, "... %s", params[pos]);
		}
	}
	else
	{
	    foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i)) || (PlayerInfo[i][pSettings] & togService))	continue;

			SendClientMessageEx(i, 0x00CC99AA, "[Pergunta] %s [ID: %i]: %s", GetNameEx(playerid), playerid, params);
		}
	}
	return 1;
}

CMD:denuncia(playerid, params[])
{
	if (isnull(params))								return SendHelpMessage(playerid, "/denuncia [texto]");
	if (pTemp[playerid][pReportTime] > gettime())	return SendSpecialMessage(playerid, "Você pode escrever no relatório em %i segundos.", pTemp[playerid][pReportTime] - gettime());

    pTemp[playerid][pReportTime] = gettime() + 60;
	SendSyntaxMessage(playerid, "Sua denúncia foi entregue à administração do servidor, aguarde uma resposta.");

    if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService)) continue;

			SendClientMessageEx(i, COLOR_YELLOWG, "[DENÚNCIA] %s [ID: %i]: %.*s ...", GetNameEx(playerid), playerid, pos, params);
			SendClientMessageEx(i, COLOR_YELLOWG, "... %s", params[pos]);
		}
	}
	else
	{
	    foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService)) continue;

			SendClientMessageEx(i, COLOR_YELLOWG, "[DENÚNCIA] %s [ID: %i] registrou uma denúncia: %s", GetNameEx(playerid), playerid, params);
		}
	}
	return 1;
}

CMD:pedido(playerid, params[])
{
	if (isnull(params)) 				return SendHelpMessage(playerid, "/pedido [texto do pedido]");
	if (request_time > gettime())	return SendSpecialMessage(playerid, "Aguarde %i segundos.", request_time - gettime());

	request_time = gettime() + 60;
	SendSyntaxMessage(playerid, "Seu pedido foi entregue à administração do servidor. Por favor, aguarde uma resposta. ");
	
	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService)) continue;

			SendClientMessageEx(i, 0x00CC99AA, "[AJUDA] %s (ID: %i): %.*s ...", GetNameEx(playerid), playerid, pos, params);
			SendClientMessageEx(i, 0x00CC99AA, "... %s", params[pos]);
		}
	}
 	else
	{
	    foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService)) continue;

			SendClientMessageEx(i, 0x00CC99AA, "[AJUDA] %s (ID: %i) pediu ajuda: %s", GetNameEx(playerid), playerid, params);
		}
	}
	return 1;
}

CMD:vender(playerid, params[])
{
	new bizid;

	if ((bizid = GetNearInsideBiz(playerid)) != -1)
	{
		if (BizInfo[bizid][bizType] == BIZ_PAWN)
		{
			Dialog_Show(playerid, SellPawn, DIALOG_STYLE_LIST, "Casa de Penhores", "• Vender um rádio", "Vender", "Cancelar");
		}
	}
	return 1;
}


CMD:comprar(playerid, params[])
{
	new string[1024], bizid = -1;

	if ((bizid = GetNearInsideBiz(playerid)) != -1)
	{
	    if (BizInfo[bizid][bizType] == BIZ_BUILD)
	    {
	        Dialog_Show(playerid, BuyInterior, DIALOG_STYLE_INPUT, "{FFFFFF}Pedido de Interior", "{FFFFFF}Digite o ID da casa para a qual você quer mudar o interior: \nEsta casa deve ser sua! (/propriedade)", ">>>", "Fechar");
	    }

		if (BizInfo[bizid][bizType] == BIZ_PAWN)
		{
			SendHelpMessage(playerid, "/vender.");
		}

	    else if (BizInfo[bizid][bizType] == BIZ_STORE || BizInfo[bizid][bizType] == BIZ_PHARMACY || BizInfo[bizid][bizType] == BIZ_BAR) //4
	    {
			new type; 
			switch(BizInfo[bizid][bizType])
			{
				case BIZ_STORE: type = 4;		
				case BIZ_BAR: type = 5;			
				case BIZ_PHARMACY: type = 6;	 
			} 
			if (!type) return SendErrorMessage(playerid, "Erro do sistema! (pl_cmd.inc:493)"); //return 1;

			new diatxt[LIMIT_MAX_PRODUCTS*128];

			for(new i = 0; i < max_product[type]; i++)
			{
				format(diatxt, sizeof(diatxt), "%s%i(0.0, 0.0, 63.0, 0.8)\t~w~%s~n~~g~~h~$%i\n", diatxt, model_order[type][i], name_order[type][i], BizInfo[bizid][bizProductPrice][i]);
			}

			new title[32+23];
			format(title, sizeof(title), "%s____-____Selecione o produto", BizInfo[bizid][bizName]);

			Dialog_Show(playerid, StoreDialog, DIALOG_STYLE_PREVIEW_MODEL, title, diatxt, "Comprar", "Cancelar");
	    }
		else if (BizInfo[bizid][bizType] == BIZ_BARBER)
		{
		    if (GetPVarInt(playerid, #buy_attach)) return SendErrorMessage(playerid, "Finalize a operação atual.");
		    if (GetNextToySlot(playerid) == -1)    return SendWarningMessage(playerid, "Todos os espaços para acessórios estão ocupados.");

		    for(new x; x < sizeof(BarberShop); x++)
		    {
		        format(string, sizeof(string), "%s%s - $%i\n", string, BarberShop[x][toyName], BarberShop[x][toyPrice]);
		    }

		    Dialog_Show(playerid, BarberDialog, DIALOG_STYLE_LIST, "{FFFFFF}Selecione o produto", string, "Selecionar", "Fechar");
		}

		else if (BizInfo[bizid][bizType] == BIZ_PIZZA || BizInfo[bizid][bizType] == BIZ_CLUCKIN || BizInfo[bizid][bizType] == BIZ_BURGER || BizInfo[bizid][bizType] == BIZ_DONUT)
		{
			for(new i; i != 4; i++)
			{
			    TextDrawShowForPlayer(playerid, TD_restaurant[i]);
			}

			new mes[128], type = BizInfo[bizid][bizType] - BIZ_PIZZA;

			for(new i; i != max_product[type]; i++)
			{
			    format(mes, sizeof(mes), "%s~n~~r~Saade: +%i~n~~r~Remove fome~n~~p~Preco: $%i~n~", name_order[type][i], 30 + (i * 10), BizInfo[bizid][bizProductPrice][i]);
				PlayerTextDrawSetString(playerid, TD_restaurant_prod[playerid][i], mes);
				PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i]);
	            PlayerTextDrawSetPreviewModel(playerid, TD_restaurant_prod[playerid][i + 4], model_order[type][i]);
	            PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i + 4]);
			}

			SelectTextDraw(playerid, 0x00cb77FF);
			SetPVarInt(playerid, #restaurant, 1);
		}
		else if (BizInfo[bizid][bizType] == BIZ_PHONES) // Ïîêóïêà òåëåôîíà
		{
			new phonebuy_names[9][16] = {"Preto", "Azul", "Vermelho", "Verde", "Laranja", "Azul Escuro", "Verde Escuro", "Rosa", "Amarelo"};

			new diatxt[sizeof(phone_model)*128];

			for(new i = 0; i < sizeof(phone_model); i++)
			{
				format(diatxt, sizeof(diatxt), "%s%i(180.0, 45.0, 0.0, 0.8)\t%s celular~n~~g~~h~$1000\n", diatxt, phone_model[i], phonebuy_names[i]);
			}

			Dialog_Show(playerid, ComputerStore, DIALOG_STYLE_PREVIEW_MODEL, "Selecione um celular", diatxt, "Selecionar", "Fechar");
		}
		else if (BizInfo[bizid][bizType] == BIZ_AMMO) // Ïîêóïêà îðóæèÿ
		{
			cmd::comprararma(playerid, "");
		}
		else if (pTemp[playerid][pBizEntered] != -1 && BizInfo[pTemp[playerid][pBizEntered]][bizType] == BIZ_BARBER)
		{
		    if (GetPVarInt(playerid, #buy_attach))    return SendErrorMessage(playerid, "Finalize a operação atual.");
		    if (GetNextToySlot(playerid) == -1)       return SendErrorMessage(playerid, "Todos os espaços para acessórios estão ocupados.");

		    for(new x; x < sizeof(BarberShop); x++)
		    {
		        format(string, sizeof(string), "%s%s - $%i\n", string, BarberShop[x][toyName], BarberShop[x][toyPrice]);
		    }
		    Dialog_Show(playerid, BarberDialog, DIALOG_STYLE_LIST, "{FFFFFF}Selecione o produto ", string, "Selecionar", "Fechar");
		}
		else if (BizInfo[bizid][bizType] == BIZ_ATTACH)
		{
		    if (GetPVarInt(playerid, #buy_attach))    return SendErrorMessage(playerid, "Finalize a operação atual.");
		    if (GetNextToySlot(playerid) == -1)       return SendErrorMessage(playerid, "Todos os espaços para acessórios estão ocupados.");
		    
		    Dialog_Show(playerid, BizAttachDialog, DIALOG_STYLE_LIST, "{FFFFFF}Selecione uma categoria", "1. Instrumentos musicais\n2. Relógio\n3. Ferramentas\n4. Equipamentos esportivos\n5. Acessórios", "Selecionar", "Fechar");
		}

	}
	
	return 1;
}

CMD:comprarroupas(playerid, params[])
{
	if (PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Você deve estar fora de serviço.");
	new id;

	if ((id = GetNearInsideBiz(playerid)) == -1) return SendErrorMessage(playerid, "Você não está em uma loja de roupas.");
	if (BizInfo[id][bizType] != BIZ_CLOTHES) return SendErrorMessage(playerid, "Você não está em uma loja de roupas.");
	if (GetCash(playerid) < 100) return SendErrorMessage(playerid, "Você não tem dinheiro suficiente. ($100)");

	if (PlayerInfo[playerid][pSex] == 1) 
	{
	    new mes[sizeof(male_skins)*16];
	    for(new i = 0; i < sizeof(male_skins); i++)
	    {
	        format(mes, sizeof(mes), "%s%i\tID: %i\n", mes, male_skins[i], male_skins[i]); 
	    }
	    Dialog_Show(playerid, MODEL_SEL_BUYCLOTHES, DIALOG_STYLE_PREVIEW_MODEL, "Comprando roupas:", mes, "Selecionar", "Fechar");
	}
	else
	{
	    new mes[sizeof(female_skins)*16];
	    for(new i = 0; i < sizeof(female_skins); i++)
	    {
	        format(mes, sizeof(mes), "%s%i\tID: %i\n", mes, female_skins[i], female_skins[i]); 
	    }
	    Dialog_Show(playerid, MODEL_SEL_BUYCLOTHES, DIALOG_STYLE_PREVIEW_MODEL, "Comprando roupas:", mes, "Selecionar", "Fechar");
	}
	return 1;
}

CMD:fogo(playerid, params[])
{
	if (!FireOnline() || !PoliceOnline()) return SendErrorMessage(playerid,"Deve haver pelo menos %i policiais e %i bombeiros em serviço!", NEED_COPS_TO_CRIME, NEED_FIRE_TO_CRIME);

	if (!IsPlayerHasItem(playerid, INV_LIGHTER)) return SendErrorMessage(playerid, "Você não tem um isqueiro.");
	if (!IsPlayerHasItem(playerid, INV_GASCAN)) return SendErrorMessage(playerid, "Você não tem um galão.");
	if (pTemp[playerid][pFireTime] > 0) return SendErrorMessage(playerid, "Você já está ateando fogo.");

	new type = GetNearBuilding(playerid, 0.0, 2.0), building;
	switch(type) {
		case 0: {
			building = GetNearOutsideHouse(playerid);
			if (building == -1 || (gettime()-HouseInfo[building][hFireTimer] <= 0)) return SendErrorMessage(playerid, "Tente mais tarde!");
		}
		case 1: {
			building = GetNearOutsideGarage(playerid);
			if (building == -1 || (gettime()-BizInfo[building][bFireTimer] <= 0)) return SendErrorMessage(playerid,"Tente mais tarde!");
		}
		case 2: { 
			building = GetNearOutsideBiz(playerid);
			if (building == -1 || (gettime()-GarageInfo[building][gFireTimer] <= 0)) return SendErrorMessage(playerid, "Tente mais tarde!");
		}

		default: return SendErrorMessage(playerid, "Vá até a entrada do prédio.");
	}

	if (building == -1) return SendErrorMessage(playerid, "Vá até a entrada do prédio.");

	pTemp[playerid][pFireTime] = 30;
	SetPVarInt(playerid, #firetype, type);
	SetPVarInt(playerid, #firebuildID, building);

	SendSyntaxMessage(playerid, "Você começou a atear fogo. Use {FF6347}/parar{FFFFFF} se quiser desfazer a ação.");


	RemovePlayerItem(playerid, INV_GASCAN, 1);

	SavePlayerPos(playerid);
	FreezePlayer(playerid);
	ApplyAnimation(playerid,"BOMBER","BOM_Plant_Loop", 4.0, 1, 0, 0, 0, 0, 0);
	return 1;
}
CMD:trocarroupa(playerid, params[])
{
    new race;

    if (PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Você não deve estar em serviço.");

    if (PlayerInfo[playerid][pSkin] != GetPlayerSkin(playerid)) 
    {
        if (!PlayerInfo[playerid][pSkin]) return SendErrorMessage(playerid, "Você não pode devolver as roupas porque não as possui.");

        SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
        SendSyntaxMessage(playerid, "Você recuperou suas roupas.");
        return 1;
    }

    if (sscanf(params, "d", race)) return SendHelpMessage(playerid, "/trocarroupa [1-2] [1-preto / 2-branco]");
    if (race < 1 || race > 2) return SendErrorMessage(playerid, "Tipo de pele: 1-preto / 2-branco");

    switch (PlayerInfo[playerid][pSex])
    {
        case 1:
        {
            switch(race)
            {
                case 1: SendSyntaxMessage(playerid, "Você tirou suas roupas"), SetPlayerSkin(playerid, 18);
                case 2:
                {
                    switch(random(3))
                    {
                        case 0: SetPlayerSkin(playerid, 45);
                        case 1: SetPlayerSkin(playerid, 97);
                        case 2: SetPlayerSkin(playerid, 154);
                    }
                    SendSyntaxMessage(playerid, "Você tirou suas roupas");
                }
            }
        }
        case 2:
        {
            switch(race)
            {
                case 1: SendSyntaxMessage(playerid, "Você tirou suas roupas"), SetPlayerSkin(playerid, 139);
                case 2:
                {
                    switch(random(3))
                    {
                        case 0: SetPlayerSkin(playerid, 251);
                        case 1: SetPlayerSkin(playerid, 140);
                        case 2: SetPlayerSkin(playerid, 138);
                    }
                    SendSyntaxMessage(playerid, "Você tirou suas roupas");
                }
            }
        }
    }
    return 1;
}


CMD:definirestilo(playerid, params[])
{
    new option, secoption;

    if (sscanf(params,"iI(-1)", option, secoption))	return SendHelpMessage(playerid, "/definirestilo (1 - Estilo de caminhada(/walk) | 2 - Estilo de conversação | 3 - Estilo de combate | 4 - Estilo do velocímetro)");

    if (option == 1)
    {
        new style;

        if (sscanf(params,"{d}d",style))			return SendHelpMessage(playerid, "/definirestilo 1 [Estilo de caminhada (0-17)]");
        if (style < 0 || style > 17)				return SendErrorMessage(playerid, "Estilo de caminhada de 0 a 17.");

        PlayerInfo[playerid][pWalk] = style;
        
        if (!style)		SendSyntaxMessage(playerid, "Você desativou seu estilo de caminhada.");
        else			SendSyntaxMessage(playerid, "Estilo de caminhada alterado para %i.", style);

        return 1;
    }
    else if (option == 2)
    {
        new style;

        if (sscanf(params,"{d}d",style))		return SendHelpMessage(playerid, "/definirestilo [Estilo de conversação (0-9)]");
        if (style < 0 || style > 9)				return SendErrorMessage(playerid, "Estilo de conversação de 0 a 9.");

        PlayerInfo[playerid][pTalk] = style;

        if (!style)		SendSyntaxMessage(playerid, "Você desligou a animação ao falar.");
        else			SendSyntaxMessage(playerid, "Estilo de conversação alterado para %i.", style);

        return 1;
    }
    else if (option == 3)
    {
        if (PlayerInfo[playerid][pPremium] < 1)	return SendErrorMessage(playerid, "Editar o estilo de combate é possível apenas com uma conta premium.");
            
        new style;

        if (sscanf(params,"{i}i",style))			return SendHelpMessage(playerid, "/definirestilo [Estilo de combate (1-5)]");
        if (style < 1 || style > 5)				return SendErrorMessage(playerid, "Estilo de combate de 1 a 5.");

        SetPlayerFightingStyle(playerid, fight_id[style-1]);
        PlayerInfo[playerid][pFightStyle] = fight_id[style-1];
        SendSyntaxMessage(playerid, "Estilo de combate alterado para %s.", fight_name[style-1]);

        return 1;
    }
    else if (option == 4)
    {
        if (PlayerInfo[playerid][pPremium] < 1)	return SendErrorMessage(playerid, "Você só pode mudar o HUD se tiver uma conta premium.");
            
        new style;

        if (sscanf(params,"{d}d",style))			return SendHelpMessage(playerid, "/definirestilo [Estilo do velocímetro (1-4)]");
        if (style < 1 || style > 4)				return SendSyntaxMessage(playerid, "Estilo do velocímetro de 1 a 4.");

        PlayerInfo[playerid][pHud] = style-1;
        HideHudTextDraw(playerid);
        
        if (!(PlayerInfo[playerid][pSettings] & togHUD))	ShowHudTextDraw(playerid);
        
    }
    return 1;
}
