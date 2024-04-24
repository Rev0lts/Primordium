ALTX:channel("/ch","/canal");
CMD:channel(playerid, params[])
{
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendSyntaxMessage(playerid, "/canal [set / leave / rent / sell]");
		return 1;
	}

	if (CompareStrings(option, "set"))
	{
		if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um rádio.");
		
		new channelid;

		if (sscanf(secoption, "i", channelid))						return SendHelpMessage(playerid, "(/ch)annel set [channel]");
		if (!ChannelExists(channelid))								return SendErrorMessage(playerid, "Canal não encontrado.");
		if (channelid == 911 && !IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid,"Canal de rádio reservado.");
		if (channelid == 912 && !IsPlayerFactionMedic(playerid))	return SendErrorMessage(playerid, "Canal de rádio reservado.");
		if (channelid == 998 && !IsPlayerFactionFire(playerid))		return SendErrorMessage(playerid, "Canal de rádio reservado.");
		if (channelid == GetPlayerChannel(playerid))				return SendErrorMessage(playerid, "Você já está conectado a este canal.");
		
		if (channelid == 911 && IsPlayerFactionPolice(playerid))
		{
			SendRadioMsg(playerid, "Channel changed [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		else if (channelid == 912 && IsPlayerFactionMedic(playerid))
		{
			SendRadioMsg(playerid, "Canal alterado [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		else if (channelid == 998 && IsPlayerFactionFire(playerid))
		{
			SendRadioMsg(playerid, "Canal alterado [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		PlayerInfo[playerid][pAuthed] = 0;
		SetRadioChannel(playerid, channelid);
		SendRadioMsg(playerid, "Canal alterado [CH: %i].", channelid);
		SendHelpMessage(playerid, "Use /auth para fazer login.");
		return 1;
	}

	else if (CompareStrings(option, "leave"))
	{
		if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um rádio.");
		if (!GetPlayerChannel(playerid))				return SendErrorMessage(playerid, "Você não está conectado a nenhum canal.");

		SendRadioMsg(playerid, "Você saiu do canal [CH: %i].", GetPlayerChannel(playerid));
		SetRadioChannel(playerid, 0);
		PlaySound(playerid, 6400);
		return 1;
	}

	else if (CompareStrings(option, "rent"))
	{
		if (!IsPlayerInRangeOfPoint(playerid, 2.0, 2269.65, -74.306, 26.772))
		{
			Waypoint_Set(playerid, 2269.65, -74.306, 26.772);
			SendWarningMessage(playerid, "Você deveria estar perto da biblioteca da cidade.");
			return 1;
		}

		new channel, pass[33];

		if (sscanf(secoption, "iS()[33]", channel, pass))	return SendHelpMessage(playerid, "(/canal) rent [canal (1-9999)] [senha | deixe em branco se não quiser definir uma senha]");
		if (strlen(pass) > 30)								return SendErrorMessage(playerid, "Senha de até 30 caracteres.");
		if (channel >= MAX_RADIOS || channel < 1)			return SendErrorMessage(playerid, "Canal 1 até", MAX_RADIOS - 1);
		if (PlayerInfo[playerid][pOwnedChannel])			return SendErrorMessage(playerid, "Você já tem um canal, primeiro venda-o");
		if (ChannelExists(channel))							return SendErrorMessage(playerid, "Canal de rádio reservado. ");
		if (StringHasSymbols(pass))							return SendErrorMessage(playerid, "A senha só pode conter letras e números. ");
		
		new cost = 1500;

		if (GetCash(playerid) < cost)						return SendNotCash(playerid);

		new id = -1;

		for(new i = 0; i < MAX_RADIOS; i++)
		{
			if (RadiosInfo[i][radioOn]) continue;

			id = i;
			break;
		}

		if (id == -1)	return SendErrorMessage(playerid, "Todas as frequências estão ocupadas, volte mais tarde.");

		GiveCash(playerid, -cost);
		SendRadioMsg(playerid, "Você alugou um canal [CH: %i] por $%i.", channel, cost);
		SendHelpMessage(playerid, "Use /radiohelp para verificar os comandos de ajuda.");
		RadiosInfo[id][radioChannel] = channel;
		RadiosInfo[id][radioOwner] = PlayerInfo[playerid][pID];
		format(RadiosInfo[id][radioPass], 32, "%s", pass);
		AddChannelToFile(playerid, channel, pass, id);
		return 1;
	}

	else if (CompareStrings(option, "sell"))
	{
		if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "Você não está alugando um canal.");
		
		new id = -1;

		if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)
		{
			PlayerInfo[playerid][pOwnedChannel] = 0;
			SendWarningMessage(playerid, "Canal não encontrado.");
			return 1;
		}

		new award = 750;

		GiveCash(playerid, award);
		SendRadioMsg(playerid, "Você vendeu um canal [CH: %i] por $%i.", RadiosInfo[id][radioChannel], award);

		foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || GetPlayerChannel(i) != RadiosInfo[id][radioChannel] || i == playerid) continue;

			SendRadioMsg(i, "O canal ao qual você está conectado foi vendido.");
			SetRadioChannel(i, 0);
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `channel` = 0, `authed` = 0 WHERE `channel` = %i", RadiosInfo[id][radioChannel]);
		mysql_tquery(dbHandle, query);
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `radios` WHERE `id` = %i", RadiosInfo[id][radioID]);
		mysql_tquery(dbHandle, query);

		RadiosInfo[id][radioOn] = 
		RadiosInfo[id][radioID] = 
		RadiosInfo[id][radioChannel] = 
		RadiosInfo[id][radioOwner] = 0;
		SetRadioChannel(playerid, 0);
		PlayerInfo[playerid][pOwnedChannel] = 0;
		return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "/canal [set / leave / rent / sell]");
	}
	return 1;
}

CMD:setslot(playerid, params[])
{
    if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um rádio.");

	new slot;

	if (sscanf(params,"d",slot))			return SendHelpMessage(playerid, "/setslot [slot (0-100)]");
	if (slot < 0 || slot > 100)			return SendErrorMessage(playerid, "Slot de 0 a 100. ");
	if (slot == GetPlayerSlot(playerid))	return SendErrorMessage(playerid, "Você já está usando este slot.");

	SetRadioSlot(playerid, slot);
	SendRadioMsg(playerid, "Frequência alterada [SLOT: %i]", slot);
	return 1;
}

CMD:auth(playerid, params[])
{
    if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um radio.");
	if (!GetPlayerChannel(playerid))					return SendErrorMessage(playerid, "Seu rádio está desligado.");
	if (PlayerInfo[playerid][pAuthed])				return SendErrorRadioMessage(playerid, "Você já está conectado neste canal. ");

	new id = -1;

	if ((id = ChannelExists(GetPlayerChannel(playerid))) == 0)	return SendErrorMessage(playerid, "Canal não encontrado.");

	if (CompareStrings(RadiosInfo[id][radioPass], ""))
	{
	    SendRadioMsg(playerid, "Você está conectado ao canal %i.", GetPlayerChannel(playerid));
		PlayerInfo[playerid][pAuthed] = 1;
		PlaySound(playerid, 6400);
	    return 1;
	}

	if (isnull(params))										return SendHelpMessage(playerid, "/auth [senha do canal]");
	if (CompareStrings(RadiosInfo[id][radioPass], params))	return SendErrorRadioMessage(playerid, "Você inseriu a senha errada.");

	SendRadioMsg(playerid, "Você está conectado ao canal. [CH: %i].", GetPlayerChannel(playerid));
	PlayerInfo[playerid][pAuthed] = 1;
	PlaySound(playerid, 6400);
	return 1;
}

CMD:newauth(playerid, params[])
{
	if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "Você não está alugando um canal.");

    new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)				return SendErrorMessage(playerid, "Canal não encontrado."); 
	if (isnull(params))																return SendHelpMessage(playerid, "/newauth [nova senha | se deseja remover a senha, então não insira nada]");
	if (strlen(params) < 1 || strlen(params) > 32)									return SendErrorMessage(playerid, "Caracteres de 1 a 32.");
	if (CompareStrings(RadiosInfo[id][radioPass], "") && CompareStrings(params, ""))	return SendErrorMessage(playerid, "Seu canal não possui senha.");
	if (!CompareStrings(params, "") && StringHasSymbols(params))						return SendErrorMessage(playerid, "Apenas letras e números podem ser usados na senha.");

	if (CompareStrings(params, ""))	SendRadioMsg(playerid, "Seu canal agora não possui senha. ");
	else							SendRadioMsg(playerid, "Você alterou a senha do seu canal para %s.", params);

	format(RadiosInfo[id][radioPass], 33, "%s", params);
	SQL_SetString("radios", "pass", RadiosInfo[id][radioPass], RadiosInfo[id][radioID]);
	return 1;
}

CMD:radiomembers(playerid, params[])
{
    if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "Você não está alugando um canal.");

    new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)	return SendErrorMessage(playerid, "Canal não encontrado.");

	new dialog_header[64] = "{FFFFFF}Name\t{FFFFFF}ID\n", dialog_body[2048];

	format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || GetPlayerChannel(i) != RadiosInfo[id][radioChannel]) continue;

	    format(dialog_body, sizeof(dialog_body), "%s%s\t%i\n", GetNameEx(i), i);
	}
	
	Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Lista de jogadores no canal", dialog_body, "Close", "");
	return 1;
}

CMD:kickoffradio(playerid, params[])
{
    if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "Você não está alugando um canal.");

	new userid;

	if (sscanf(params,"u", userid))	return SendHelpMessage(playerid, "/kickoffradio [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))		return SendNotConnected(playerid);
	if (userid == playerid)			return SendErrorMessage(playerid, "Você não pode expulsar a si mesmo do canal.");

    new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)	return SendErrorMessage(playerid, "Canal não encontrado.");
	if (RadiosInfo[id][radioChannel] != GetPlayerChannel(playerid))		return SendErrorMessage(playerid, "Você deve estar em seu canal. [CH: %i].", RadiosInfo[id][radioChannel]);
	if (GetPlayerChannel(userid) != GetPlayerChannel(playerid))			return SendErrorMessage(playerid, "Este jogador não está neste canal. ");

	SendRadioMsg(playerid, "Você kickou %s.", GetNameEx(id));
	SendRadioMsg(userid, "Você foi expulso do canal. ");
	SetRadioChannel(userid, 0);
	return 1;
}

ALTX:r("/radio");
CMD:r(playerid, params[])
{
    if (PlayerInfo[playerid][pMutedTime] > 0)		return SendErrorMessage(playerid,"Seu bate-papo local está bloqueado pelo sistema.");

    new channel = GetPlayerChannel(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um rádio.");
	if (channel == 0)							return SendErrorMessage(playerid, "Você não está na frequência.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorMessage(playerid, "Você não está logado na frequência.");

	if (channel == 911) {
		new tag[6], arg[25];
		if (!sscanf(params, "s[6]s[24]", tag, arg)) { 
			if (CompareStrings(tag, "10-27")) {
				new userid;
				if (sscanf(arg, "u", userid)) return SendErrorMessage(playerid, "/r 10-27 [ID/Part Do Nome]");
				if (!IsPlayerLogged(userid))	 return SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: Nenhum resultado encontrado..");

				SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: %s, sex: %s, age: %i, unpaid fines: %s, search: %s.", GetNameEx(userid), (PlayerInfo[userid][pSex]==1)?("male"):("female"), GetAge(userid), (CountPlayerTickets(userid))?("{FF6347}%i{9189EF}"):("No"), (IsPlayerWanted(userid))?("Yes"):("No"));
				SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: Dispatcher, 10-27 %s.", GetNameWithMask(playerid), GetNameEx(userid));
				return 1;
			} else if (CompareStrings(tag, "10-28")) {
				new plates[10], vehicle;
				if (sscanf(arg, "s[10]", plates))				 return SendErrorMessage(playerid, "/r 10-28 [plates]");
				if ((vehicle = FindVehicleByPlate(plates)) == -1) return SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: Nenhum resultado encontrado..");
				
				PlayCrimeReportForPlayer(playerid, playerid, 13);
				
				SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: %s [%s], owner: %s, unpaid fines: %s, search: %s.", VehicleNames[GetVehicleModel(vehicle)-400], VehicleInfo[vehicle][carPlate], VehicleInfo[vehicle][carOwner], (CountVehicleTickets(vehicle))?("%i"):("No"), (VehicleInfo[vehicle][carWanted])?("Yes"):("No"));
				SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: Dispatcher, 10-28 %s.", GetNameWithMask(playerid), VehicleInfo[vehicle][carPlate]);
				return 1;
			}
		}
	}
	
	if (sscanf(params, "s[144]", params)) 		return SendHelpMessage(playerid, "/r [texto]");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		if (pTemp[playerid][pPatrol] && channel == 911) {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s: %.*s ...", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		} else {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s: %.*s ...", channel, GetNameEx(playerid), pos, params);
		}

		SendRadioMessage(channel, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol] && channel == 911) {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s: %s", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		} else {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s: %s", channel, GetNameEx(playerid), params);
		}
			
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

CMD:rr(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rr [texto]");
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Seu bate-papo local está bloqueado pelo sistema.");

    new channel = GetPlayerChannel(playerid),
		slot = GetPlayerSlot(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um rádio.");
	if (channel == 0)							return SendErrorRadioMessage(playerid, "Você não está na frequência.");
	if (PlayerInfo[playerid][pAuthed] == 0)		return SendErrorRadioMessage(playerid, "Você não está logado na frequência.");

	new mes[128];
	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		if (pos < MAXLEN-1) pos = MAXLEN;
		if (pTemp[playerid][pPatrol])
		    format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s: %.*s ...", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		else
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s: %.*s ...", channel, slot, GetNameEx(playerid), pos, params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);

		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol])
			format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s: %s", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		else
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s: %s", channel, slot, GetNameEx(playerid), params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

ALTX:rlow("/rl","/rb");
CMD:rlow(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rlow [texto]");
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Seu bate-papo local está bloqueado pelo sistema.");

    new channel = GetPlayerChannel(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um rádio.");
	if (!channel)								return SendErrorMessage(playerid, "Você não está na frequência.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorMessage(playerid, "Você não está logado na frequência.");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s [quiet]: %.*s ...", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: 0] %s [quiet]: %.*s ...", channel, GetNameEx(playerid), pos, params);

		SendRadioMessage(channel, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s [quiet]: %s", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: 0] %s [quiet]: %s", channel, GetNameEx(playerid), params);

		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

ALTX:rrlow("/rrl","/rrb");
CMD:rrlow(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rrlow [texto]");
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid,"Seu bate-papo local está bloqueado pelo sistema.");

    new channel = GetPlayerChannel(playerid), slot = GetPlayerSlot(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Você não tem um rádio.");
	if (!channel)								return SendErrorRadioMessage(playerid, "Você não está na frequência.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorRadioMessage(playerid, "Você não está logado na frequência.");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s [quiet]: %.*s ...", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: %i] %s [quiet]: %.*s ...", channel, slot, GetNameEx(playerid), pos, params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s [quiet]: %s", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: %i] %s [quiet]: %s", channel, slot, GetNameEx(playerid), params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %s", GetNameWithMask(playerid), params);
	}
	return 1;
} //î