Dialog:LSPDElevator(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
    if (!IsPlayerInRangeOfPoint(playerid, 2.0, 2268.0640,2448.2449,3.5313) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2781.0559,-1056.2823,1028.4969) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2874.6665,-940.6707,1035.9259) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2872.8813,-1072.9934,1037.1782)) return 1;
		
	new world_list[] = {2, 1, 2, 3};

	switch(listitem)
	{
	    case 0: SetPlayerPosEx(playerid, 2268.0640,2448.2449,3.5313), SetPlayerFacingAngle(playerid, 181.2476);
	    case 1: SetPlayerPosEx(playerid, 2780.9036,-1056.2358,1028.4969), SetPlayerFacingAngle(playerid, 87.9492);
	    case 2: SetPlayerPosEx(playerid, 2873.9063,-940.8126,1035.9259), SetPlayerFacingAngle(playerid, 84.3340);
	    case 3: SetPlayerPosEx(playerid, 2872.7168,-1072.2004,1037.1770), SetPlayerFacingAngle(playerid, 0.2384);
	}

	SetWorld(playerid, world_list[listitem]);
	SetInterior(playerid, (listitem == 0)?(0):(1));
	return 1;
}

Dialog:MPCarVoice(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	    
    MegaphoneSoundVoice(playerid, listitem + 1);
	return 1;
}

/*Dialog:MainMDC(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem)
	{
		case 0:	Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïîèñê ïî èìåíè", "{B4B5B7}ÏÎÈÑÊ ÏÎ ÈÌÅÍÈ\n\nÂâåäèòå èìÿ:\n\nÔîğìà ââîäà: Èìÿ_Ôàìèëèÿ (( èëè ID èãğîêà ))", "Done", "Return");
		case 1:	Dialog_Show(playerid, MDCRecord, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïğåäúÿâëåíèå îáâèíåíèÿ", "{B4B5B7}ÏĞÅÄÚßÂËÅÍÈÅ ÎÁÂÈÍÅÍÈÉ\n\nÂâåäèòå èìÿ:\n\nÔîğìà ââîäà: Èìÿ_Ôàìèëèÿ (( èëè ID èãğîêà ))", "Done", "Return");
		case 2:	Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïğåäóïğåæäåíèå íà ëèöåíçèş", "{B4B5B7}ÄÎÁÀÂÈÒÜ ÏĞÅÄÓÏĞÅÆÄÅÍÈß ÍÀ ËÈÖÅÍÇÈŞ\n\nÂâåäèòå èìÿ:\n\n(( Èìÿ_Ôàìèëèÿ / ID èãğîêà ))", "Done", "Return");
		case 3:	Dialog_Show(playerid, MDCVehicle, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïîèñê àâòîìîáèëÿ", "{B4B5B7}ÏÎÈÑÊ ÈÍÔÎĞÌÀÖÈÈ ÎÁ ÀÂÒÎÌÎÁÈËÅ\n\nÂâåäèòå íîìåğíîé çíàê:\n\n(( Íîìåğ ÒÑ ))", "Done", "Return");
		case 4: Get_Calls(playerid);
		case 5:
		{
			new string[MAX_CALLS_AND_ARREST * 32], count;

			for(new i = 0; i != MAX_CALLS_AND_ARREST; i++)
			{
				if (!ArrestInfo[i][arrestExists]) continue;

				format(string, sizeof(string), "%s\n%i. %s", string, count+1, GetNameEx(ArrestInfo[i][arrestSuspect]));
			}

			Dialog_Show(playerid, MDCLastArrest, DIALOG_STYLE_LIST, "{8D8DFF}MDC - Ïîñëåäíèå àğåñòû", string, "Select", "Return");
		}
		case 6:
		{
		    new string[1024], count;

		    for(new i = 0; i != MAX_PATROL; i++)
		    {
				if (!PatrolInfo[GetPatrolID(playerid)][i][patrulExists]) continue;

				format(string, sizeof(string), "%s\n{8c8cfe}%s:{FFFFFF}  ", string, PatrolInfo[GetPatrolID(playerid)][i][patrulName]);

				if (IsPlayerLogged(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0]))	format(string, sizeof(string), "%s%s  ", string, GetNameEx(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0]));
                if (IsPlayerLogged(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1]))	format(string, sizeof(string), "%s%s  ", string, GetNameEx(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1]));

				count++;
			}

			if (!count) return SendErrorMessage(playerid, "Àêòèâíûå ïàòğóëè íå íàéäåíû.");

			Dialog_Show(playerid, None, DIALOG_STYLE_LIST, "{8D8DFF}MDC - Àêòèâíûå ïàòğóëè", string, "Close", "");
		}
		case 7: ShowWeaponLicense(playerid, 0);
	}
	return 1;
}

Dialog:MDCLastArrest(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new count;

    for(new i = 0; i != MAX_CALLS_AND_ARREST; i++)
	{
		if (!ArrestInfo[i][arrestExists]) continue;

		if (count == listitem)
		{
            new string[500];

			format(string, sizeof(string), "{B4B5B7}ÈÍÔÎĞÌÀÖÈß ÎÁ ÀĞÅÑÒÅ\n\nÂğåìÿ àğåñòà: %s\nÀğåñòîâàíûé: %s\nÏğîèçâåë àğåñò: %s\nÄàòà: %s", ConvertSecond(ArrestInfo[i][arrestTime]), ArrestInfo[i][arrestSuspect], ArrestInfo[i][arrestOfficer], ArrestInfo[i][arrestDate]);
			Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{8D8DFF}MDC - Èíôîğìàöèÿ îá àğåñòå", string, "-", "");
		    return 1;
		}
		count++;
	}

	SendErrorMessage(playerid, "Îøèáêà àğåñòà.");
	return 1;
}

Dialog:MDCWeapon(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::mdc(playerid, "");
	    
	if (listitem == 0)
	{
	    if (GetPVarInt(playerid, #weapon_list) < 1) 	ShowWeaponLicense(playerid, 0);
		else										ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list)-1);
	    
		return 1;
	}

    else if (listitem == 1)
	{
	    return ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list)+1);
	}
	
	listitem -= 2;

	if (pTemp[playerid][pWeaponListed][listitem] == 0)	return SendErrorMessage(playerid, "Îøèáêà çàïğîñà.");
	    
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `id`=%i", pTemp[playerid][pWeaponListed][listitem]);
	mysql_tquery(dbHandle, query, "ShowWeaponLicensePlayer", "i", playerid);

	for(new i; i != 30; i++)
	{
		pTemp[playerid][pWeaponListed][i] = 0;
	}
	return 1;
}*/


Dialog:MDCCall(playerid, response, listitem, inputtext[])
{
	if (!response)						return 1;
    if (!IsPlayerFactionPolice(playerid) &&
		!IsPlayerFactionMedic(playerid) &&
		!IsPlayerFactionFire(playerid))	return 1;

	new mes[512], count, type = (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid))?(2):(1);

    for(new i = 0; i != MAX_CALLS_AND_ARREST; i++)
	{
	    if (!CallInfo[i][callExists]) continue;
		if (CallInfo[i][callType] != 3) {
			if (CallInfo[i][callType] != type) continue;
		}

		if (listitem == count)
		{
			new statusmes[65];
			if (CallInfo[i][callAccept]) format(statusmes, sizeof(statusmes), "%s", CallInfo[i][callAcceptBy]);
			else format(statusmes, sizeof(statusmes), "{A9C4E4}Chamada não aceita{B4B5B7}");

			format(mes, sizeof(mes), "{B4B5B7}INFORMAÇÃO DA CHAMADA 911 - #%i\n\n\
            						Chamada de:\t\t%s\n\
            						Número de telefone:\t%i\n\
           							Situação:\t\t%s\n\
            						Localização:\t%s\n\
            						Data:\t\t\t%s\n\n\
									Status:\t\t\t%s\n\
            						A chamada foi atendida %i vezes", 
			i+1, CallInfo[i][callName], CallInfo[i][callPhone], CallInfo[i][callText], CallInfo[i][callLocation], CallInfo[i][callDate], statusmes, CallInfo[i][callAccept]);
			
			Dialog_Show(playerid, MDCCall2, DIALOG_STYLE_MSGBOX, "{8D8DFF}MDC - Informação da chamada", mes, "Opções", "Retornar");
			SetPVarInt(playerid, #last_call, i);
		    return 1;
		}
		count++;
	}
	return 1;
}

Dialog:MDCCall2(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    Dialog_Show(playerid, MDCCallRespond, DIALOG_STYLE_LIST, "{8D8DFF}MDC - Informação da chamada", "- Atender a chamada de emergência\n- Apagar chamada", "Selecionar", "Retornar");
	return 1;
}


Dialog:MDCCallRespond(playerid, response, listitem, inputtext[])
{
	if (!response) 																													
		return 1;

    if (!IsPlayerFactionPolice(playerid) &&
		!IsPlayerFactionMedic(playerid) &&
		!IsPlayerFactionFire(playerid))
		return 1;

	new type = (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid))? (2) : (1), 
		id = GetPVarInt(playerid, #last_call);

    if (!CallInfo[id][callExists]) return 1;
	if (CallInfo[id][callType] != 3) {
		if (CallInfo[id][callType] != type) return 1;
	}

	if (!listitem)
	{
		if (pTemp[playerid][pPatrol])	SendFactionMessageF(FACTION_POLICE, COLOR_LSPD, "* [%s] %s %s [%s] aceitou a chamada de emergência no 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], id+1);
		else							SendFactionMessageF(FACTION_POLICE, COLOR_LSPD, "* [%s] %s %s aceitou a chamada de emergência no 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid), id+1);
		
		if (pTemp[playerid][pPatrol])    format(CallInfo[id][callAcceptBy], 64, "Atendido por %s (%s)", GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName]);
        else                             format(CallInfo[id][callAcceptBy], 64, "Atendido por %s", GetNameEx(playerid));
		
		Waypoint_Set(playerid, CallInfo[id][callPosX],CallInfo[id][callPosY],CallInfo[id][callPosZ], 3.0);

	    CallInfo[id][callAccept]++;
		pTemp[playerid][pPoliceCalls]++;

		MDC_PAGE(playerid, PAGE_CALLS);
	}
	else
	{
		SendFactionMessageF(FACTION_POLICE, COLOR_LSPD, "* [%s] %s %s removeu a chamada para o 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid), id+1);
		
		SendWarningMessage(playerid, "Chamada [ID: %i] foi removida.", id+1);
	    CallInfo[id][callExists] = 0;
 		CallInfo[id][callType] = 0;
		CallInfo[id][callAccept] = 0;

		MDC_PAGE(playerid, PAGE_MAIN);
	}
	return 1;
}


/*Dialog:MDCSearch(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    new userid;
	if (sscanf(inputtext, "u", userid))	return Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Busca por nome", "{B4B5B7}BUSCA POR NOME\n\nDigite o nome:\n\nFormato de entrada: Nome_Sobrenome (( ou ID do jogador ))", "Concluir", "Cancelar");
	if (!IsPlayerLogged(userid))			return Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Busca por nome", "{B4B5B7}BUSCA POR NOME\n\nDigite o nome:\n\nFormato de entrada: Nome_Sobrenome (( ou ID do jogador ))\n"EMBED_RED"Esta pessoa não está presente no banco de dados.", "Concluir", "Cancelar");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `criminalrecords` WHERE `userid` = %i", PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query, "PersonInfo", "ii", playerid, userid);
	return 1;
}*/

Dialog:MDCRecord(playerid, response, listitem, inputtext[])
{
	if (!response)												return 1;
	if (sscanf(inputtext, "u", pTemp[playerid][pMDCPlayer]))	return SendErrorMessage(playerid, "Este jogador não está online!");
	if (!IsPlayerLogged(pTemp[playerid][pMDCPlayer]))			return SendErrorMessage(playerid, "O jogador não está autorizado!");

	new mes[256];
	format(mes, sizeof(mes), "{B4B5B7}ACUSAÇÃO\n\nAcusado: %s\n\nDigite o texto da acusação: (( Um crime / 128 caracteres))", GetNameEx(pTemp[playerid][pMDCPlayer]));
	Dialog_Show(playerid, MDCRecord2, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Acusação", mes, "Inserir", "Cancelar");
	return 1;
}

Dialog:MDCRecord2(playerid, response, listitem, inputtext[])
{
	if (!response)										return 1;
    if (!IsPlayerLogged(pTemp[playerid][pMDCPlayer]))	return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127)
	{
	    new mes[256];
		format(mes, sizeof(mes), "{B4B5B7}ACUSAÇÃO\n\nAcusado: %s\n\nDigite o texto da acusação: (( Um crime / 128 caracteres))", GetNameEx(pTemp[playerid][pMDCPlayer]));
		Dialog_Show(playerid, MDCRecord2, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Acusação", mes, "Inserir", "Cancelar");
		return 1;
	}

    AddCriminalRecord(pTemp[playerid][pMDCPlayer], inputtext, PlayerInfo[playerid][pUsername], PlayerInfo[playerid][pFaction]);
    SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "[PROCURADO] Acusado: %s, Acusador: %s, Crime: %s.", GetNameEx(pTemp[playerid][pMDCPlayer]), GetNameEx(playerid), inputtext);
	SendWarningMessage(playerid, "O acusado foi adicionado com sucesso à lista de procurados.");
	pTemp[playerid][pMDCPlayer] = INVALID_PLAYER_ID;
	return 1;
}


Dialog:MDCAddWarn(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new userid;

    //if (sscanf(inputtext, "u", userid))	return Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïğåäóïğåæäåíèå íà ëèöåíçèş", "{B4B5B7}ÄÎÁÀÂÈÒÜ ÏĞÅÄÓÏĞÅÆÄÅÍÈß ÍÀ ËÈÖÅÍÇÈŞ\n\nÂâåäèòå èìÿ:\n\n(( Èìÿ_Ôàìèëèÿ / ID èãğîêà ))\n"EMBED_RED"Äàííûé ÷åëîâåê îòñóòñòâóåò â áàçå äàííûõ.", "Done", "Cancel");
	//if (!IsPlayerLogged(userid))			return Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïğåäóïğåæäåíèå íà ëèöåíçèş", "{B4B5B7}ÄÎÁÀÂÈÒÜ ÏĞÅÄÓÏĞÅÆÄÅÍÈß ÍÀ ËÈÖÅÍÇÈŞ\n\nÂâåäèòå èìÿ:\n\n(( Èìÿ_Ôàìèëèÿ / ID èãğîêà ))\n"EMBED_RED"Äàííûé ÷åëîâåê îòñóòñòâóåò â áàçå äàííûõ.", "Done", "Cancel");
    //if (!PlayerInfo[userid][pCarLic])	return Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïğåäóïğåæäåíèå íà ëèöåíçèş", "{B4B5B7}ÄÎÁÀÂÈÒÜ ÏĞÅÄÓÏĞÅÆÄÅÍÈß ÍÀ ËÈÖÅÍÇÈŞ\n\nÂâåäèòå èìÿ:\n\n(( Èìÿ_Ôàìèëèÿ / ID èãğîêà ))\n"EMBED_RED"Ó äàííîãî ÷åëîâåêà îòñóòñòâóåò ëèöåíçèÿ íà âîæäåíèå.", "Done", "Cancel");

	new mes[128];
	format(mes, sizeof(mes), "emitiu uma advertência na carteira de motorista de %s.", GetNameEx(userid));
	cmd::me(playerid, mes);

	SendSyntaxMessage(playerid, "Você adicionou uma advertência à sua carteira de motorista %s.", GetName(userid));

	PlayerInfo[userid][pDriveWarns]++;
	SQL_SetInteger("users", "drivewarns", PlayerInfo[userid][pDriveWarns], PlayerInfo[userid][pID]);

	if (PlayerInfo[userid][pDriveWarns] >= 3)
	{
		SendSyntaxMessage(playerid, "Como houve 3 advertências na carteira de motorista de %s, ela foi automaticamente revogada.", GetNameEx(userid));
		PlayerInfo[userid][pDriveWarns] = 0;
		PlayerInfo[userid][pCarLic] = 0;

		SQL_SetInteger("users", "drivewarns", PlayerInfo[userid][pDriveWarns], PlayerInfo[userid][pID]);
		SQL_SetInteger("users", "carlic", PlayerInfo[userid][pCarLic], PlayerInfo[userid][pID]);
	}
	return 1;
}

/*Dialog:MDCVehicle(playerid, response, listitem, inputtext[])
{
	if (!response)										return 1;
	if (!strlen(inputtext) || strlen(inputtext) > 23)	return Dialog_Show(playerid, MDCVehicle, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Ïîèñê àâòîìîáèëÿ", "{B4B5B7}ÏÎÈÑÊ ÈÍÔÎĞÌÀÖÈÈ ÎÁ ÀÂÒÎÌÎÁÈËÅ\n\nÂâåäèòå íîìåğíîé çíàê:\n\n(( Íîìåğ ÒÑ ))", "Done", "Return");
	    
	new query[156];
	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `plate` = '%e' AND `registered`=1", inputtext);
	mysql_tquery(dbHandle, query, "OnCheckNumber", "d", playerid);
	return 1;
}*/

Dialog:MDCWeaponPlayer(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list));
	    
	if (listitem == 5)
	{
	    if (IsPlayerFactionPolice(playerid))	 return SendErrorMessage(playerid, "Somente LSPDs estão envolvidos no cancelamento, por favor envie um pedido.");
		if (PlayerInfo[playerid][pAuthwep] != 1) return SendErrorMessage(playerid, "Você não tem autoridade para revogar uma licença de arma.");
		    
		new userid;

	    if ((userid = GetIDByID(pTemp[playerid][pMDCWepPlayer])) != INVALID_PLAYER_ID)
		{
		    SendSyntaxMessage(userid, "%s %s revogou sua licença de arma.", GetRankName(playerid), GetNameEx(playerid));
			PlayerInfo[userid][pWepLic] = 0;
			PlayerInfo[userid][pWepWarns] = 0;
			PlayerInfo[userid][pWepCount] = 0;
		}
	    new query[156];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `weplic`='0',`wepwarns`='0',`wep_officer`='-',`wep_data`='-',`wep_price`='0',`wep_count`='0' WHERE `id` = %i", pTemp[playerid][pMDCWepPlayer]);
		mysql_tquery(dbHandle, query);
		
		SendSyntaxMessage(playerid, "Você revogou sua licença de arma %s.", pTemp[playerid][pMDCWepName]);
	    return 1;
	}

	else if (listitem == 6)
	{
		new userid;
	    if ((userid = GetIDByID(pTemp[playerid][pMDCWepPlayer])) != INVALID_PLAYER_ID)
	    {
	        SendSyntaxMessage(userid, "%s %s escreveu um aviso em sua licença de arma.", GetRankName(playerid), GetNameEx(playerid));
			PlayerInfo[userid][pWepWarns]++;
		}
		
	    new query[128];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `wepwarns`= `wepwarns` + 1 WHERE `id` = %i", pTemp[playerid][pMDCWepPlayer]);
		mysql_tquery(dbHandle, query);

		SendSyntaxMessage(playerid, "Você emitiu um aviso para uma licença de arma %s.", pTemp[playerid][pMDCWepName]);
	    return 1;
	}
	
	ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list));
	return 1;
}


Dialog:ShowTrace(playerid, response, listitem, inputtext[])
{
	if (pTemp[playerid][pTraceStage] < 7)
	{
		Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Sistema de Rastreamento - Pressione ESC / ENTER para sair", "{B4B5B7}Você foi desconectado do sistema de rastreamento.", "Aceitar", "");
		
		if (pTemp[playerid][pTraceGZ] != -1)	GangZoneDestroy(pTemp[playerid][pTraceGZ]);
 		
		pTemp[playerid][pTraceGZ] = -1;
		pTemp[playerid][pTracePlayer] = INVALID_PLAYER_ID;
 		pTemp[playerid][pTraceStage] = 0;
	}
	return 1;
}

Dialog:LSPDGUN(playerid, response, listitem, inputtext[])
{
	if (!response)															return cmd::duty(playerid, "");
    if (!IsPlayerFactionPolice(playerid))									return 1;
	if (!PlayerInfo[playerid][pOnDuty])										return SendErrorMessage(playerid, "Primeiro você precisa entrar em serviço.");
	if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))	return SendErrorMessage(playerid, "Você não está no vestiário / no veículo oficial.");
	
	GameTextForPlayer(playerid, "~r~-", 1500, 4);

	switch(listitem)
	{
		case 0: if (!GivePlayerWeaponEx(playerid, 3, 1, GetDefaultAmmoType(3), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventário está cheio!");
		case 1: if (!GivePlayerWeaponEx(playerid, 41, 999, GetDefaultAmmoType(41), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventário está cheio!");
		case 2: if (!GivePlayerWeaponEx(playerid, 24, 85, GetDefaultAmmoType(24), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventário está cheio!");
		case 3: if (GiveInvItem(playerid, 85, CheckAmmo(85)) == -1) return SendErrorMessage(playerid, "Inventário está cheio!");// desert eagle patrons 85
		case 4: if (!GivePlayerWeaponEx(playerid, 29, 91, GetDefaultAmmoType(29), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventário está cheio!");
		case 5: if (GiveInvItem(playerid, 91, CheckAmmo(91)) == -1) return SendErrorMessage(playerid, "Inventário está cheio!");// MP5 patrons 91
		case 6: if (!GivePlayerWeaponEx(playerid, 25, 89, GetDefaultAmmoType(25), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventário está cheio!");
		case 7: if (GiveInvItem(playerid, 86, CheckAmmo(86)) == -1) return SendErrorMessage(playerid, "Inventário está cheio!");// Shotgun patrons 86
		case 8: if (GiveInvItem(playerid, 89, CheckAmmo(89)) == -1) return SendErrorMessage(playerid, "Inventário está cheio!");// Shotgun neletalnoe patrons 89
		case 9: if (!GivePlayerWeaponEx(playerid, 31, 98, GetDefaultAmmoType(31), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventário está cheio!");
		case 10: if (GiveInvItem(playerid, 98, CheckAmmo(98)) == -1) return SendErrorMessage(playerid, "Inventário está cheio!");// M4 patrons 98
		case 11: if (GiveInvItem(playerid, 99, CheckAmmo(99)) == -1) return SendErrorMessage(playerid, "Inventário está cheio!");// M4 neletalnoe patrons 99
		case 12: if (!GivePlayerWeaponEx(playerid, 34, 96, GetDefaultAmmoType(34), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventário está cheio!");
		case 13: if (GiveInvItem(playerid, 96, CheckAmmo(96)) == -1) return SendErrorMessage(playerid, "Inventário está cheio!");// Sniper Rifle patrons 96
		case 14: if (!GivePlayerWeaponEx(playerid, 17, 1, GetDefaultAmmoType(17), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventário está cheio!");
		case 15: if (!GivePlayerWeaponEx(playerid, 43, 50, GetDefaultAmmoType(43), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventário está cheio!");
		case 16: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~g~+", 1500, 4);

	Dialog_Show(playerid, LSPDGUN, DIALOG_STYLE_LIST, "Arma", "{FFFFFF}• Cacetete\n• Spray de pimenta\n• Desert Eagle ( .50AE .ext )\n\t{ffc46a}- Pegar carregador extras ( .50AE .ext )\n{FFFFFF}• MP5 ( 9x19mm .ext )\n\t{ffc46a}- Pegar carregador extras ( 9x19mm .ext )\n{FFFFFF}• Shotgun ( 12-G BIRDSHOT )\n\t{ffc46a}- Pegar carregador extras ( 12-G BIRDSHOT )\n\t{ffc46a}- Pegue munição não letal ( 12-G BEANBAG )\n{FFFFFF}• M4 ( 5.56x45mm (drum) )\n\t{ffc46a}- Pegar carregador extras ( 5.56x45mm (drum) )\n\t{ffc46a}- Pegue munição não letal ( 5.56x45mm (rubber) )\n{FFFFFF}• Rifle de Sniper (7.62x51mm)\n\t{ffc46a}- Pegar carregador extras ( 7.62x51mm )\n{FFFFFF}• Granada de gás\n• Câmera\n{FF6347}• Entregar todas as armas", "Selecionar", "Fechar");
	return 1;
}


Dialog:LSPDArmoury(playerid, response, listitem, inputtext[])
{
	if (!response)															return cmd::duty(playerid, "");
    if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))	return SendErrorMessage(playerid, "Você não está no vestiário / no veículo oficial.");
	
	switch(listitem) {
		case 0: {
			GameTextForPlayer(playerid, "~r~-", 1500, 4);
			if (GiveInvItem(playerid, INV_RADIO, 1) == -1) return SendErrorMessage(playerid, "Inventário está cheio!"); // rádio
			GameTextForPlayer(playerid, "~g~+", 1500, 4);
		} 
		case 1: {
			return Dialog_Show(playerid, LSPDGUN, DIALOG_STYLE_LIST, "Arma", "{FFFFFF}• Cassetete\n• Spray de pimenta\n• Desert Eagle ( .50AE .ext )\n\t{ffc46a}- Pegar carregador extras ( .50AE .ext )\n{FFFFFF}• MP5 ( 9x19mm .ext )\n\t{ffc46a}- Pegar carregador extras ( 9x19mm .ext )\n{FFFFFF}• Shotgun ( 12-G BIRDSHOT )\n\t{ffc46a}- Pegar carregador extras ( 12-G BIRDSHOT )\n\t{ffc46a}- Pegar munição não letal ( 12-G BEANBAG )\n{FFFFFF}• M4 ( 5.56x45mm (drum) )\n\t{ffc46a}- Pegar carregador extras ( 5.56x45mm (drum) )\n\t{ffc46a}- Pegar munição não letal ( 5.56x45mm (rubber) )\n{FFFFFF}• Sniper Rifle (7.62x51mm)\n\t{ffc46a}- Pegar carregador extras ( 7.62x51mm )\n{FFFFFF}• Granada de gás\n• Câmera\n{FF6347}• Rendir todas as armas", "Selecionar", "Fechar");
		}
		case 2: {
			if (!PlayerInfo[playerid][pSwat] && !isLeader(playerid)) return SendErrorMessage(playerid, "O conjunto S.W.A.T. não está disponível para você!");

			if (!PlayerInfo[playerid][pSwatDuty])
			{
				PlayerInfo[playerid][pSwatDuty] = 1;
				SetPlayerArmour(playerid, 150);
				PlayerInfo[playerid][pChar] = (!IsPlayerFactionPolice(playerid))?(287):(285);
				SetPlayerSkin(playerid, PlayerInfo[playerid][pChar]);
				SetPlayerColor(playerid, RemoveAlpha(Factions[PlayerInfo[playerid][pFaction]][fColor]));
				SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s entrou na equipe S.W.A.T **", GetRankName(playerid), GetNameEx(playerid));
				return 1;
			}
			
			PlayerInfo[playerid][pSwatDuty] = 0;
			SetPlayerArmour(playerid, 100);
			PlayerInfo[playerid][pChar] = 0;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
			SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s desautorizado para a equipe S.W.A.T **", GetRankName(playerid), GetNameEx(playerid));
		}
	}

	return 1;
}
