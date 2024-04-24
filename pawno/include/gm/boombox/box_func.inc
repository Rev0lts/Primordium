PlaceBoombox(playerid)
{
	if (BoomboxInfo[playerid][bUsed] != 0) return SendErrorMessage(playerid, "Você não pode colocar mais de um boombox.");
	if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "O boombox não pode ser colocado aqui.");

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || !BoomboxInfo[i][bUsed]) continue;
		if (IsValidDynamicArea(BoomboxInfo[i][bArea])) return SendErrorMessage(playerid, "Alguém já colocou o boombox nas proximidades.");
	}

	new id = -1;
	if ((id = GetNearInsideHouse(playerid)) != -1 && HouseInfo[id][hRadioOn]) return SendErrorMessage(playerid, "O rádio já está tocando na casa.");
	else if ((id = GetNearInsideBiz(playerid)) != -1 && BizInfo[id][bizRadioOn]) return SendErrorMessage(playerid, "O rádio já está tocando no negócio.");
	if (!RemovePlayerItem(playerid, INV_BOOMBOX, 1)) return SendErrorMessage(playerid, "Você não tem um boombox.");

	SendSyntaxMessage(playerid, "Você colocou um boombox, use a tecla H para controlar.");

	GetXYInFrontOfPlayer(playerid, BoomboxInfo[playerid][bPosX], BoomboxInfo[playerid][bPosY], BoomboxInfo[playerid][bPosZ], 1.0);
	BoomboxInfo[playerid][bInt] = GetInterior(playerid);
	BoomboxInfo[playerid][bVW] = GetWorld(playerid);
	BoomboxInfo[playerid][bUsed] = 1;
	BoomboxInfo[playerid][bUrl] = EOS;
	BoomboxInfo[playerid][bObject] = CreateDynamicObject(2226, BoomboxInfo[playerid][bPosX], BoomboxInfo[playerid][bPosY], BoomboxInfo[playerid][bPosZ] - 1.0, 0.0, 0.0, 0.0, BoomboxInfo[playerid][bVW], BoomboxInfo[playerid][bInt], -1, 300.0);
	BoomboxInfo[playerid][bArea] = CreateDynamicSphere(BoomboxInfo[playerid][bPosX], BoomboxInfo[playerid][bPosY], BoomboxInfo[playerid][bPosZ], 50.0, BoomboxInfo[playerid][bVW], BoomboxInfo[playerid][bInt]);
	pTemp[playerid][pBoombox] = playerid;
	StopAudioStreamForPlayer(playerid);
	Iter_Add(boomboxIter, playerid);
	return 1;
}


ShowBoomboxMenu(playerid)
{
	if (BoomboxInfo[playerid][bUsed] == 0 || Boombox_Nearest(playerid) == 0) return 1;

	Dialog_Show(playerid, BoomBox, DIALOG_STYLE_LIST, "{FFFFFF}Boombox", "Alterar estação de rádio\nMinhas músicas\nDesligar o boombox\nElevar o boombox", "Selecionar", "Cancelar");
	return 1;
}


ChangeUrlBoombox(playerid, url[])
{
    if (BoomboxInfo[playerid][bUsed] != 0 && Boombox_Nearest(playerid) != 0)
	{
		format(BoomboxInfo[playerid][bUrl], 156, url);

		foreach (new i : Player)
		{
	    	if (!IsPlayerLogged(i) || pTemp[i][pBoombox] != playerid) continue;
	        
        	StopAudioStreamForPlayer(i);
			PlayAudioStreamForPlayer(i, BoomboxInfo[playerid][bUrl], BoomboxInfo[playerid][bPosX], BoomboxInfo[playerid][bPosY], BoomboxInfo[playerid][bPosZ], 50.0, 1);
		}
		return 1;
	}
	
	new vehicle = GetPlayerVehicleID(playerid), id;

    if (IsDriver(playerid))
	{
		if (!VehicleInfo[vehicle][carRadio]) return SendErrorMessage(playerid, "Não há rádio neste veículo.");

		VehicleInfo[vehicle][carRadioOn] = 1;
	    format(VehicleInfo[vehicle][carRadioURL], 156, url);
		SetPlayerChatBubble(playerid, "mudou a estação de rádio.", COLOR_PURPLE, 20.0, 4000);
	    foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || GetPlayerVehicleID(i) != vehicle) continue;

			StopAudioStreamForPlayer(i);
			PlayAudioStreamForPlayer(i, url);
		}
		return 1;
	}

	if ((id = GetNearInsideHouse(playerid)) != -1 && HouseInfo[id][hOwnerID] == PlayerInfo[playerid][pID])
	{
	    if (HouseInfo[id][hRadio] == 0) return SendErrorMessage(playerid, "Não há rádio em sua casa.");

		HouseInfo[id][hRadioOn] = 1;
		format(HouseInfo[id][hRadioURL], 156, url);
        SetPlayerChatBubble(playerid, "mudou a estação de rádio.", COLOR_PURPLE, 20.0, 4000);
	 	foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || !IsPlayerInsideHouse(i, id)) continue;

            StopAudioStreamForPlayer(i);
			PlayAudioStreamForPlayer(i, url);
			pTemp[i][pBoombox_house] = id;
		}
	    return 1;
	}
	else if ((id = GetNearInsideBiz(playerid)) != -1 && BizInfo[id][bizOwnerID] == PlayerInfo[playerid][pID])
	{
	    if (BizInfo[id][bizRadio] == 0) return SendErrorMessage(playerid, "Não há rádio em sua empresa.");

		BizInfo[id][bizRadioOn] = 1;
		format(BizInfo[id][bizRadioURL], 156, url);
        SetPlayerChatBubble(playerid, "mudou a estação de rádio.", COLOR_PURPLE, 20.0, 4000);
	 	foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || !IsPlayerBizInside(i, id)) continue;

            StopAudioStreamForPlayer(i);
			PlayAudioStreamForPlayer(i, url);
			pTemp[i][pBoombox_biz] = id;
		}
	    return 1;
	}
	return 1;
}


Destroy_Boombox(playerid)
{
    if (BoomboxInfo[playerid][bUsed] == 0) 			return 1;
    if (GiveInvItem(playerid, INV_BOOMBOX, 1) == -1) return SendErrorMessage(playerid, "Inventário cheio.");
	
    BoomboxInfo[playerid][bUsed] = 0;
    
    foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || pTemp[i][pBoombox] != playerid) continue;

		StopAudioStreamForPlayer(i);
	}
    
	if (IsValidDynamicArea(BoomboxInfo[playerid][bArea]))		DestroyDynamicArea(BoomboxInfo[playerid][bArea]);
    if (IsValidDynamicObject(BoomboxInfo[playerid][bObject]))	DestroyDynamicObject(BoomboxInfo[playerid][bObject]);

	Iter_Remove(boomboxIter, playerid);
        
	return 1;
}

Boombox_Nearest(playerid)
{
	return (BoomboxInfo[playerid][bUsed] == 0 || !IsPlayerInRangeOfPoint(playerid, 2.0, BoomboxInfo[playerid][bPosX], BoomboxInfo[playerid][bPosY], BoomboxInfo[playerid][bPosZ]) || GetWorld(playerid) != BoomboxInfo[playerid][bVW] || GetInterior(playerid) != BoomboxInfo[playerid][bInt])?(0):(1);
}

ShowBoomBoxStations(playerid)
{
    return Dialog_Show(playerid, RadioStations, DIALOG_STYLE_LIST, "Escolha o gênero", "Pop/Dance/Dubstep\nJazz/Country/Rock\nHip-hop/RnB/Reggae\nMinhas músicas\nDesligar o rádio", "Selecionar", "Fechar");
}


StopCarBoomBox(vehicle)
{
	if (VehicleInfo[vehicle][carRadioOn] != 1)	return 1;

    VehicleInfo[vehicle][carRadioOn] = 0;

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || GetPlayerVehicleID(i) != vehicle)	continue;

		StopAudioStreamForPlayer(i);
	}
	return 1;
}

StopHouseBoomBox(houseid)
{
	if (HouseInfo[houseid][hRadioOn] != 1)	return 1;

	HouseInfo[houseid][hRadioOn] = 0;
	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || pTemp[i][pBoombox_house] != houseid) continue;

	 	StopAudioStreamForPlayer(i);
		pTemp[i][pBoombox_house] = -1;
	}
	return 1;
}

StopBusinessBoomBox(biz)
{
	if (BizInfo[biz][bizRadioOn] != 1)	return 1;

	BizInfo[biz][bizRadioOn] = 0;

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || pTemp[i][pBoombox_biz] != biz)	continue;

		StopAudioStreamForPlayer(i);
		pTemp[i][pBoombox_biz] = -1;
	}
	return 1;
}