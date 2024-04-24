Dialog:MDCSearchPerson(playerid, response, listitem, inputtext[])
{
	if (!response) 
		return 1;

	new 
		userid,
		query[128]
	;

	if (sscanf(inputtext, "u", userid))	
		return Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Buscar por nome", "{B4B5B7}BUSCAR PESSOA PELO NOME\n\nDigite o nome:\n\nFormato de entrada: Nome_Sobrenome ((ou ID do jogador))", "Selecionar", "Cancelar");
	
	if (!IsPlayerLogged(userid))
	{
		mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e' LIMIT 1", inputtext);
		mysql_tquery(dbHandle, query, "MDCSearchPersonInfo", "i", playerid);
		return 1;
	}
	
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `id` = %i LIMIT 1", PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query, "MDCSearchPersonInfo", "i", playerid);
	return 1;
}

Dialog:MDCSearchVehicleType(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	switch(listitem)
	{
		case 0:
		{
		    Dialog_Show(playerid, MDCSearchVehiclePlayer, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Busca de Veículo", "{B4B5B7}BUSCAR INFORMAÇÕES SOBRE O VEÍCULO\n\nDigite o nome:\n\nFormato de entrada: Nome_Sobrenome ((ou ID do jogador))", "Selecionar", "Retornar");
		}
		case 1:
		{
		    Dialog_Show(playerid, MDCSearchVehicleNum, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Busca de Veículo", "{B4B5B7}BUSCAR INFORMAÇÕES SOBRE O VEÍCULO\n\nDigite a placa do veículo:\n\n(( Placas de veículos ))", "Selecionar", "Retornar");
		}

	}

	return 1;
}

Dialog:MDCSearchVehiclePlayer(playerid, response, listitem, inputtext[])
{
	if (!response) 
		return Dialog_Show(playerid, MDCSearchVehicleType, DIALOG_STYLE_TABLIST, "{8D8DFF}MDC - Busca de Veículo", "Busca de veículo por proprietário\nBusca de veículo por placas", "Selecionar", "Retornar");

    new 
		userid,
		query[164]
	;

	if (sscanf(inputtext, "u", userid))	
		return Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Busca de Veículo", "{B4B5B7}BUSCAR INFORMAÇÕES SOBRE O VEÍCULO\n\nDigite o nome:\n\nFormato de entrada: Nome_Sobrenome ((ou ID do jogador))", "Selecionar", "Retornar");
	
	SetPVarInt(playerid, #MDC_VEH_BACK, 1);
	MdcInfo[playerid][mdcListPage] = 0;

	if (!IsPlayerLogged(userid))
	{
		mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `users` WHERE `name` = '%e' LIMIT 1", inputtext);
		mysql_tquery(dbHandle, query, "MDCSearchVehiclesList_GetID", "i", playerid);
		return 1;
	}

	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `owner` = %i AND `registered`=1 LIMIT 3", PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query, "MDCSearchVehiclesList", "i", playerid);
	return 1;
}

Dialog:MDCSearchVehicleNum(playerid, response, listitem, inputtext[])
{
	if (!response)
		return Dialog_Show(playerid, MDCSearchVehicleType, DIALOG_STYLE_TABLIST, "{8D8DFF}MDC - Busca de Veículo", "Buscar por proprietário de veículo\nBuscar por número de placa", "Selecionar", "Retornar");
	
	if (!strlen(inputtext) || strlen(inputtext) > 10)
	{
		PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________"); //ñòðîêà ïîèñêà

		for(new a = 0; a < 2; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
		for(new b = 0; b < 12; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][b]);

		for(new c = 0; c < 3; c++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][c]);
        for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);
		for(new d = 0; d < 12; d++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][d]);

		MdcInfo[playerid][mdcPage] = PAGE_LOOKUP;
		return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
	}

	SetPVarInt(playerid, #MDC_VEH_BACK, 0);

	SetPVarString(playerid, #MDC_VEHICLE_NUM, inputtext);
	PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], inputtext);

	new query[156];
	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `plate` = '%e' AND `registered`=1", inputtext);
	mysql_tquery(dbHandle, query, "MDCSearchVehicle", "i", playerid);
	return 1;
}

Dialog:MDC_Record(playerid, response, listitem, inputtext[])
{
	if (!response)												return 1;
	if (sscanf(inputtext, "u", pTemp[playerid][pMDCPlayer]))	return Dialog_Show(playerid, MDC_Record, DIALOG_STYLE_INPUT, "{8D8DFF}MDC  - Denúncia", "{B4B5B7}ACUSAÇÃO\n\nDigite o nome:\n\nFormato de entrada: Nome_Sobrenome ((ou ID do jogador))", "Selecionar", "Cancelar");
	if (!IsPlayerLogged(pTemp[playerid][pMDCPlayer]))			return Dialog_Show(playerid, MDC_Record, DIALOG_STYLE_INPUT, "{8D8DFF}MDC  - Denúncia", "{B4B5B7}ACUSAÇÃO\n\nDigite o nome:\n\nFormato de entrada: Nome_Sobrenome ((ou ID do jogador))\n"EMBED_RED"Esta pessoa não está no banco de dados.", "Selecionar", "Cancelar");

	new mes[256];
	format(mes, sizeof(mes), "{B4B5B7}ACUSAÇÃO\n\nAcusado: %s\n\nDigite o texto da acusação: ((Um crime / 128 caracteres))", GetNameEx(pTemp[playerid][pMDCPlayer]));
	Dialog_Show(playerid, MDC_Record2, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Denúncia", mes, "Digite", "Cancelar");
	return 1;
}


Dialog:MDC_Record2(playerid, response, listitem, inputtext[])
{
	if (!response)										return 1;
    if (!IsPlayerLogged(pTemp[playerid][pMDCPlayer]))	return 1;
	if (CheckTextDrawString(inputtext))					return SendErrorMessage(playerid, "O caractere '~' não pode ser usado.");

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127)
	{
	    new mes[256];
		format(mes, sizeof(mes), "{B4B5B7}ACUSAÇÃO\n\nAcusado: %s\n\nDigite o texto da acusação: ((Um crime / 128 caracteres))", GetNameEx(pTemp[playerid][pMDCPlayer]));
		Dialog_Show(playerid, MDC_Record2, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Denúncia", mes, "Digite", "Cancelar");
		return 1;
	}

    AddCriminalRecord(pTemp[playerid][pMDCPlayer], inputtext, PlayerInfo[playerid][pUsername], PlayerInfo[playerid][pFaction]);
    SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "[WANTED] Acusado: %s, Promotor: %s, Crime: %s.", GetNameEx(pTemp[playerid][pMDCPlayer]), GetNameEx(playerid), inputtext);
	SendWarningMessage(playerid, "O acusado foi adicionado com sucesso à lista de procurados.");
	pTemp[playerid][pMDCPlayer] = INVALID_PLAYER_ID;
	return 1;
}


Dialog:MDC_Add_Note(playerid, response, listitem, inputtext[])
{
	if (!response)											return 1;
	if (strlen(inputtext) < 1 || strlen(inputtext) > 127) return Dialog_Show(playerid, MDC_Add_Note, DIALOG_STYLE_INPUT, "{8D8DFF}MDC  - Adição de registro de veículo", "{B4B5B7}ADICIONANDO UM REGISTRO DE VEÍCULO\n\nDigite o texto da sua nota:", "Concluído", "Retornar");
	if (CheckTextDrawString(inputtext)) return SendErrorMessage(playerid, "O caractere '~' não pode ser usado.");


	new string[7], 
		query[200],
		plates[24];

	GetPVarString(playerid, #MDC_VEHICLE_NUM, plates, 24);

	format(string, sizeof(string), "nota %i", GetPVarInt(playerid, #MDC_FREE_NOTE));

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `cars` SET `%s` = '%e %s' WHERE `plate` = '%e'", string, inputtext, GetFullDate(), plates);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `plate` = '%e' AND `registered`=1", plates);
	mysql_tquery(dbHandle, query, "MDCSearchVehicle", "i", playerid);

	return 1;
}

Dialog:MDC_Del_Note(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new string[7], 
		query[200],
		plates[24];

	GetPVarString(playerid, #MDC_VEHICLE_NUM, plates, 24);

	format(string, sizeof(string), "note%i", listitem+1);

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `cars` SET `%e` = 'None' WHERE `plate` = '%e'", string, plates);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `plate` = '%e' AND `registered`=1", plates);
	mysql_tquery(dbHandle, query, "MDCSearchVehicle", "i", playerid);

	return 1;
}

Dialog:MDC_ROSTER_CHANGE(playerid, response, listitem, inputtext[])
{
	if (!response)						return 1;
	if (isnull(inputtext))                return Dialog_Show(playerid, MDC_ROSTER_CHANGE, DIALOG_STYLE_INPUT, "Alterar callsign", "{FFFFFF}Digite o novo callsign:", "Aceitar", "Cancelar");
	if (CheckTextDrawString(inputtext))    return SendErrorMessage(playerid, "O caractere '~' não pode ser usado.");

	cmd::callsign(playerid, inputtext);
	MDC_PAGE(playerid, PAGE_ROSTER);
	return 1;
}

Dialog:APB_CREATE_FIRST(playerid, response, listitem, inputtext[])
{
	if (!response)											return DeletePVar(playerid, #MDC_ID);
	if (strlen(inputtext) < 1 || strlen(inputtext) > 23) return Dialog_Show(playerid, APB_CREATE_FIRST, DIALOG_STYLE_INPUT, "{098d18}Criação de APB", "{B4B5B7}Digite o nome do suspeito:\n- Número máximo de caracteres: 24", ">>>", "Cancelar");
	if (CheckTextDrawString(inputtext)) return SendErrorMessage(playerid, "O caractere '~' não pode ser usado.");

	new apb = GetPVarInt(playerid, #MDC_ID);

	format(ApbInfo[apb][apbOfficer], MAX_PLAYER_NAME, "%s", GetNameEx(playerid));
	format(ApbInfo[apb][apbSuspect], MAX_PLAYER_NAME, "%s", inputtext);
	ApbInfo[apb][apbFaction] = PlayerInfo[playerid][pFaction];
	format(ApbInfo[apb][apbDate], 32, "%s", GetFullDate());
	
	Dialog_Show(playerid, APB_CREATE_SECOND, DIALOG_STYLE_INPUT, "{098d18}Criação de APB", "{B4B5B7}Digite uma descrição do suspeito:\n- Número máximo de caracteres: 128", "Concluído", "Retornar");
	return 1;
}

Dialog:APB_CREATE_SECOND(playerid, response, listitem, inputtext[])
{
	if (!response) return Dialog_Show(playerid, APB_CREATE_FIRST, DIALOG_STYLE_INPUT, "{098d18}Criação de APB", "{B4B5B7}Digite o nome do suspeito:\n- Número máximo de caracteres: 24", ">>>", "Cancelar");
	if (strlen(inputtext) < 1 || strlen(inputtext) > 127) return Dialog_Show(playerid, APB_CREATE_SECOND, DIALOG_STYLE_INPUT, "{098d18}Criação de APB", "{B4B5B7}Digite uma descrição do suspeito:\n- Número máximo de caracteres: 128", "Concluído", "Retornar");
	if (CheckTextDrawString(inputtext)) return SendErrorMessage(playerid, "O caractere '~' não pode ser usado.");

	new apb = GetPVarInt(playerid, #MDC_ID);
	format(ApbInfo[apb][apbCharge], 128, "%s", inputtext);
	
	SendSyntaxMessage(playerid, "Você criou uma nota de APB [NUM: %i].", apb);
	SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s adicionou uma nova nota de APB, número %i.", GetRankName(playerid), GetNameEx(playerid), apb);
	
	new query[128 + 128];
	mysql_format(dbHandle,query, sizeof(query), "INSERT INTO `apbs` (faction, charge, suspect, officer, date) VALUES (%i, '%s', '%s', '%s', '%s')", ApbInfo[apb][apbFaction], ApbInfo[apb][apbCharge], ApbInfo[apb][apbSuspect], GetNameEx(playerid), GetFullDate());
	mysql_tquery(dbHandle, query, "ApbCreate", "i", apb);

	DeletePVar(playerid, #MDC_ID);
	
	MdcInfo[playerid][mdcPage] = PAGE_NONE;
	MDC_PAGE(playerid, PAGE_APB);
	return 1;
}

Dialog:MDC_APB_SEARCH_NAME(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], inputtext);
	PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);
	for(new b = 0; b < 3; b++) PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
	for(new apb = 0; apb < 20; apb++)  PlayerTextDrawHide(playerid, MDC_APB_TD[playerid][apb]);

	SetPVarString(playerid, #MDC_APB_TEXT, inputtext);

	new count,
		string[256];

	for(new i = MAX_APBS-1; i >= 0; i--)
	{
		if (ApbInfo[i][apbOn] != 0 && strfind(ApbInfo[i][apbSuspect], inputtext, true) != -1)
		{
			if (count < 5)
			{
				format(string, sizeof(string), "%s, #%i~n~%.60s%s~n~%s, %s", ApbInfo[i][apbSuspect], ApbInfo[i][apbID]+90214, ApbInfo[i][apbCharge], (strlen(ApbInfo[i][apbCharge]) > 60)?("_.."):("_"), ApbInfo[i][apbOfficer], ApbInfo[i][apbDate]);
				PlayerTextDrawSetString(playerid, MDC_APB_TD[playerid][count*4+1], string);

				MdcInfo[count][mdcList] = i;

				for(new j = count*4; j < count*4+4; j++) PlayerTextDrawShow(playerid, MDC_APB_TD[playerid][j]);
			}
			count++;
		}
	}

	if (!count)	return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);

	if (count > 5)
	{
		for(new b = 0; b < 3; b++)
		{
			PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
			PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
		}

		count = floatround(float(count)/5.0, floatround_ceil);
		SetPVarInt(playerid, #MAX_MDC_PAGE, count);
		

		format(string, sizeof(string), "1_/_%i", count);
		PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], string);

		SetPVarInt(playerid, #MDC_APB_S, 1);
		MdcInfo[playerid][mdcListPage] = 0;
	}

	return 1;
}

Dialog:MDC_APB_SEARCH_CHARGE(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], inputtext);
	PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);
	for(new b = 0; b < 3; b++)
	{
		PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
		PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
	}
	for(new apb = 0; apb < 20; apb++)  PlayerTextDrawHide(playerid, MDC_APB_TD[playerid][apb]);

	SetPVarString(playerid, #MDC_APB_TEXT, inputtext);

	new 
		count,
		string[256]
	;

	for(new i = MAX_APBS-1; i >= 0; i--)
	{
		if (ApbInfo[i][apbOn] != 0 && strfind(ApbInfo[i][apbCharge], inputtext, true) != -1)
		{
			if (count < 5)
			{
				format(string, sizeof(string), "%s, #%i~n~%.60s%s~n~%s, %s", ApbInfo[i][apbSuspect], ApbInfo[i][apbID]+90214, ApbInfo[i][apbCharge], (strlen(ApbInfo[i][apbCharge]) > 60)?("_.."):("_"), ApbInfo[i][apbOfficer], ApbInfo[i][apbDate]);
				PlayerTextDrawSetString(playerid, MDC_APB_TD[playerid][count*4+1], string);

				MdcInfo[count][mdcList] = i;

				for(new j = count*4; j < count*4+4; j++) PlayerTextDrawShow(playerid, MDC_APB_TD[playerid][j]);
			}
			count++;
		}
	}

	if (!count)	return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);

	if (count > 5)
	{
		for(new b = 0; b < 3; b++)
			PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
		
		count = floatround(float(count)/5.0, floatround_ceil);
		SetPVarInt(playerid, #MAX_MDC_PAGE, count);

		format(string, sizeof(string), "1_/_%i", count);
		PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], string);

		SetPVarInt(playerid, #MDC_APB_S, 1);
		MdcInfo[playerid][mdcListPage] = 0;
	}

	return 1;
}

Dialog:MDC_APB_DETAIL(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	return Dialog_Show(playerid, MDC_APB_OPTIONS, DIALOG_STYLE_TABLIST, "{098d18}Edição de APB", "Editar a linha 'suspeito'\nEditar a linha 'descrição'", "Selecionar", "Retornar");
}

Dialog:MDC_APB_OPTIONS(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		new
			mes[256],
			i_apb = MdcInfo[GetPVarInt(playerid, #MDC_ID)][mdcList]
		;

		format(mes, sizeof(mes), "{a9c4e4}#%i\n\n{a9c4e4}Suspect:{ffffff}\t %s\n{a9c4e4}Description:{ffffff}\t\t %s\n\n{a9c4e4}Added by:{ffffff}\t\t %s\n{a9c4e4}Date:{ffffff}\t\t\t %s\n", ApbInfo[i_apb][apbID]+90214, ApbInfo[i_apb][apbSuspect], ApbInfo[i_apb][apbCharge], ApbInfo[i_apb][apbOfficer], ApbInfo[i_apb][apbDate]);
		Dialog_Show(playerid, MDC_APB_DETAIL, DIALOG_STYLE_MSGBOX, "{098d18}Edição de APB", mes, "Editar", "Cancelar");
		return 1;
	}

	switch(listitem)
	{
		case 0:
		{
			Dialog_Show(playerid, APB_EDIT_NAME, DIALOG_STYLE_INPUT, "{098d18}Edição de APB", "{B4B5B7}Digite o nome do suspeito:\n- Número máximo de caracteres: 24", "Concluído", "Retornar");
		}

		case 1:
		{
			Dialog_Show(playerid, APB_EDIT_CHARGE, DIALOG_STYLE_INPUT, "{098d18}Edição de APB", "{B4B5B7}Digite uma descrição do suspeito:\n- Número máximo de caracteres: 128", "Concluído", "Retornar");
		}
	}

	return 1;
}

Dialog:APB_EDIT_NAME(playerid, response, listitem, inputtext[])
{
	if (!response) return Dialog_Show(playerid, MDC_APB_OPTIONS, DIALOG_STYLE_TABLIST, "{098d18}Edição de APB", "Editar a linha 'suspeito'\nEditar a linha 'descrição'", "Selecionar", "Retornar");

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127) Dialog_Show(playerid, APB_EDIT_NAME, DIALOG_STYLE_INPUT, "{098d18}Edição de APB", "{B4B5B7}Digite o nome do suspeito:\n- Número máximo de caracteres: 24", "Concluído", "Retornar");
	if (CheckTextDrawString(inputtext)) return SendErrorMessage(playerid, "O caractere '~' não pode ser usado.");

	new number = GetPVarInt(playerid, #MDC_ID);

	SendSyntaxMessage(playerid, "Você alterou a nota de APB [NUM: %i].", number);
	SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s alterou a nota de APB número %i.", GetRankName(playerid), GetNameEx(playerid), number);

	new query[128];
	mysql_format(dbHandle,query, sizeof(query), "UPDATE `apbs` SET `suspect` = '%e' WHERE `id` = %i", inputtext, ApbInfo[number][apbID]);
	mysql_tquery(dbHandle, query);
	
	DeletePVar(playerid, #MDC_ID);
	return 1;
}

Dialog:APB_EDIT_CHARGE(playerid, response, listitem, inputtext[])
{
	if (!response) return Dialog_Show(playerid, MDC_APB_OPTIONS, DIALOG_STYLE_TABLIST, "{098d18}Edição de APB", "Editar a linha 'suspeito'\nEditar a linha 'descrição'", "Selecionar", "Retornar");

	if (strlen(inputtext) < 1 || strlen(inputtext) > 23) return Dialog_Show(playerid, APB_EDIT_CHARGE, DIALOG_STYLE_INPUT, "{098d18}Edição de APB", "{B4B5B7}Digite uma descrição do suspeito:\n- Número máximo de caracteres: 128", "Concluído", "Retornar");
	if (CheckTextDrawString(inputtext)) return SendErrorMessage(playerid, "O caractere '~' não pode ser usado.");

	new number = GetPVarInt(playerid, #MDC_ID);

	SendSyntaxMessage(playerid, "Você alterou a nota de APB [NUM: %i].", number);
	SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s alterou a nota de APB número %i.", GetRankName(playerid), GetNameEx(playerid), number);
		
	new query[128 + 50];
	mysql_format(dbHandle,query, sizeof(query), "UPDATE `apbs` SET `charge` = '%s' WHERE `id` = %i", inputtext, ApbInfo[number][apbID]);
	mysql_tquery(dbHandle, query);

	DeletePVar(playerid, #MDC_ID);
	return 1;
}