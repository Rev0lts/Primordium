CMD:taser(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))	return SendErrorMessage(playerid, "Você deve ser um policial / agente penitenciário.");
	if (!PlayerInfo[playerid][pOnDuty])							return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");

	if (!pTemp[playerid][pTaser])
	{
		if (PlayerInfo[playerid][pPlayerWeapon] != 0) cmd::putgun(playerid, "");
		GivePlayerWeaponEx(playerid, 23, 1, 1, 0);
		pTemp[playerid][pTaser] = 1;
		SetPlayerChatBubble(playerid, "tira uma arma de choque de um cinto tático.", COLOR_PURPLE, 20.0, 4000);
		SM1(playerid, "Você pegou uma arma de choque. ");
	    return 1;
	}

	ResetPlayerWeapons(playerid);

	if (PlayerInfo[playerid][pPlayerWeapon] != 0) RemovePlayerAttachedObject(playerid, ATTACH_HANDS);

	PlayerInfo[playerid][pPlayerWeapon] = 0;
	PlayerInfo[playerid][pPlayerAmmo] = 0;
	PlayerInfo[playerid][pAmmoType] = 0;
	PlayerInfo[playerid][pWepSerial] = 0;
	
    pTemp[playerid][pTaser] = 0;
	SetPlayerChatBubble(playerid, "coloca a arma de choque no cinto tático.", COLOR_PURPLE, 20.0, 4000);
	SM1(playerid, "Você guardou a arma de choque. ");
	return 1;
}


CMD:rastrear(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))		return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid))				return SendNotInVehicle(playerid);
	if (VehicleInfo[vehicle][carFaction] != faction)	return SendErrorMessage(playerid, "Você não está em um veículo oficial.");

	new trace, targetid;

	if (sscanf(params,"d", trace))									return SendHelpMessage(playerid, "/rastrear [número de telefone]");
	if (pTemp[playerid][pTracePlayer] != INVALID_PLAYER_ID)			return SendErrorMessage(playerid, "Faça logout primeiro (/traceoff).");
	//if (trace == PlayerInfo[playerid][pNumber])						return 1;
    if ((targetid = GetNumberOwner(trace)) == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "O número especificado não foi encontrado no banco de dados.");
    if (PlayerInfo[targetid][pJailed] != 0)							return SendErrorMessage(playerid, "O dono deste número está sob custódia.");

	SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s inicia o rastreamento do número de telefone %i. **", GetRankName(playerid), GetNameEx(playerid), trace);

	Dialog_Show(playerid, ShowTrace, DIALOG_STYLE_MSGBOX, "Sistema de Rastreamento", "{B4B5B7}Rastreamento iniciado . . .", "Fechar", "");

	pTemp[playerid][pTracePlayer] = targetid;
	pTemp[playerid][pTraceStage] = 0;

	new Float:x, Float:y, Float:z;
	
	if (GetWorld(targetid) || GetInterior(targetid))
	{
		x = PlayerInfo[targetid][pLocationPos][0];
		y = PlayerInfo[targetid][pLocationPos][1];
		z = PlayerInfo[targetid][pLocationPos][2];
	}
	else GetPlayerPos(targetid, x, y, z);

	pTemp[playerid][pTraceDist][0] = GetDistanceBetweenPoints(x, y, z, 1459.3712, -783.0401, 93.6459);
	pTemp[playerid][pTraceDist][1] = GetDistanceBetweenPoints(x, y, z, 2667.2908, -1957.5245, 13.5469);
	pTemp[playerid][pTraceDist][2] = GetDistanceBetweenPoints(x, y, z, 923.6055, -1893.5146, 1.5319);
	return 1;
}


CMD:pararrastreio(playerid, params[])
{
	if (pTemp[playerid][pTracePlayer] == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Você não está no sistema de rastreamento.");

	if (pTemp[playerid][pTraceGZ] != -1) GangZoneDestroy(pTemp[playerid][pTraceGZ]);
    
	pTemp[playerid][pTraceGZ] = -1;
    pTemp[playerid][pTracePlayer] = INVALID_PLAYER_ID;
    pTemp[playerid][pTraceStage] = 0;
    SendSyntaxMessage(playerid, "Você saiu do sistema de rastreamento.");
	return 1;
}

CMD:multar(playerid, params[])
{
	new amount, reason[64], userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsPlayerLogged(userid))								return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))				return SendNotNearPlayer(playerid);
	if (!IsFactionPolice(faction))								return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (sscanf(params,"uds[64]", userid, amount, reason))		return SendHelpMessage(playerid, "/multar [ID/Parte do Nome] [valor da multa ($1-$5.000)] [motivo]");
	if (!PlayerInfo[playerid][pOnDuty])							return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");
	if (strlen(reason) < 1 || strlen(reason) > 63)				return SendErrorMessage(playerid, "O motivo não pode ter mais de 63 caracteres.");
	if (amount < 1 || amount > 5000)							return SendErrorMessage(playerid, "O valor da multa varia de $1 a $5.000.");
	if (!GivePlayerTicket(playerid, userid, amount, reason))	return SendErrorMessage(playerid,"O jogador atingiu o número máximo de multas.");

	new mes[128];
	format(mes, sizeof(mes), "emitiu um recibo de multa de %s no valor de $%s Motivo: \"%s\".", GetNameWithMask(userid), FormatNumber(amount), reason);
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

	SM1(playerid, "Você emitiu um recibo de multa para \"%s\" no valor de $%s %s.", reason, FormatNumber(amount), GetNameWithMask(userid));
	SendSpecialMessage(userid, "%s %s emitiu uma multa no valor de $%s para você. Motivo: \"%s\".", GetRankName(playerid), GetNameWithMask(playerid), FormatNumber(amount), reason);
	SendHelpMessage(userid, "Para visualizar a lista de multas, use \"/tickets\".");

	pTemp[playerid][pPoliceFines]++;
	return 1;
}

ALTX:vehiclefine("/vfine","/vmultar");
CMD:vehiclefine(playerid, params[])
{
	new amount, vehicle, vehiclenum[12], reason[64], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))									return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (sscanf(params,"s[12]is[64]", vehiclenum, amount, reason))	return SendHelpMessage(playerid, "/vmultar [número da placa do veículo (/checkplate)] [valor da multa ($1-$5.000)] [motivo]");
	if ((vehicle = FindVehicleByPlate(vehiclenum)) == -1)			return SendErrorMessage(playerid, "Nenhum veículo foi encontrado.");
	if (!PlayerInfo[playerid][pOnDuty])								return SendErrorMessage(playerid, "Você deve estar de serviço (/duty)."); 
	if (0 == GetNearVehicle(playerid, 5.0, vehicle))				return SendErrorMessage(playerid, "Você deve estar próximo ao veículo.");
	if (VehicleInfo[vehicle][carRegistered] != 1)					return SendErrorMessage(playerid, "Este veículo não está registrado.");
	if (strlen(reason) < 1 || strlen(reason) > 63)					return SendErrorMessage(playerid, "O motivo não pode ter mais de 63 caracteres.");
	if (amount < 1 || amount > 5000)								return SendErrorMessage(playerid, "O valor da multa deve ser de $1 a $5.000.");
	if (!GiveVehicleTicket(playerid, vehicle, amount, reason))		return SendErrorMessage(playerid, "Este veículo atingiu o número máximo de multas.");
	
	new mes[128];
	format(mes, sizeof(mes), "emitiu um recibo de multa para %s no valor de $%s.", VehicleNames[GetVehicleModel(vehicle)-400], FormatNumber(amount));
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

    new userid = GetVehicleOwner(vehicle);

    if (userid != INVALID_PLAYER_ID)
    {
	    SendSpecialMessage(userid, "%s %s emitiu uma multa para o seu %s no valor de $%s. Motivo: \"%s\".", GetRankName(playerid), GetNameEx(playerid), VehicleNames[GetVehicleModel(vehicle)-400], FormatNumber(amount), reason);
		SendHelpMessage(userid, "Para visualizar a lista de multas em um veículo, use /vmultas.");
	}

	SM1(playerid, "Você emitiu um recibo de multa \"%s\" para %s no valor de $%s.", reason, VehicleNames[GetVehicleModel(vehicle)-400], FormatNumber(amount));
	
	pTemp[playerid][pPoliceFines]++;
	return 1;
}
ALTX:checkfines("/vermultas");
CMD:checkfines(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");
		
    new userid;

	if (sscanf(params,"u",userid))					return SendHelpMessage(playerid, "/vermultas [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);

	new query[255];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `userid`=%i AND `paid`=0 LIMIT %i", PlayerInfo[userid][pID], MAX_TICKETS);
	mysql_tquery(dbHandle, query, "ShowPlayersTickets", "ii", playerid, 0);

	SetPVarInt(playerid, #tickets_show, userid);
	SetPVarInt(playerid, #ticketsuse, 1);
	return 1;
}

ALTX:checkvfines("/vmultas");
CMD:checkvfines(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");

	new vehicle;

    if (isnull(params))								  return SendHelpMessage(playerid, "/vmultas [número do veículo do estado (/verplaca)]");
	if (strlen(params) > 12)						  return SendErrorMessage(playerid, "Este número de veículo não existe.");
    if ((vehicle = FindVehicleByPlate(params)) == -1) return SendErrorMessage(playerid, "O veículo não foi encontrado.");

	new query[255];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i AND `paid`=0 LIMIT %i", VehicleInfo[vehicle][carID], MAX_TICKETS);
	mysql_tquery(dbHandle, query, "ShowCarsTickets", "i", playerid);

	SetPVarInt(playerid, #tickets_show, vehicle);
	SetPVarInt(playerid, #ticketsuse, 1);
	return 1;
}


ALTX:doorram("/dram","/derrubarporta","/dporta");
CMD:doorram(playerid,params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar esta função neste momento.");

	new building;
	switch(GetNearBuilding(playerid)) {
		case 0: {
			building = GetNearInsideHouse(playerid);
			if(building == -1)	return 1;

			for(new i = 0; i < MAX_FURNITURE; i++)
			{
				if (!IsPlayerInRangeOfPoint(playerid, 1.5, HFurn[building][i][fPosX], HFurn[building][i][fPosY], HFurn[building][i][fPosZ]) || !HFurn[building][i][fOn] || !isHouseDoor(HFurn[building][i][fModel]))
				{
					continue;
				}
				if (HFurn[building][i][fOpened])	return SendWarningMessage(playerid, "A porta já está aberta!");

				cmd::server_ame(playerid,"derruba a porta");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,0,1,1);
				
				SetTimerEx("DoorRamEnd", 1000, false, "iiii", 0, building, i, playerid);
				break;
			}
		}
		case 1: {
			building = GetNearInsideBiz(playerid);
			if(building == -1)	return 1;
			
			for(new i = 0; i < MAX_FURNITURE; i++)
			{
				if (!IsPlayerInRangeOfPoint(playerid, 1.5, BFurn[building][i][fPosX], BFurn[building][i][fPosY], BFurn[building][i][fPosZ]) || !BFurn[building][i][fOn] || !isHouseDoor(BFurn[building][i][fModel]))
				{
					continue;
				}
				if (BFurn[building][i][fOpened])	return SendWarningMessage(playerid, "A porta já está aberta!");

				cmd::server_ame(playerid, "derruba a porta");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,0,1,1);

				SetTimerEx("DoorRamEnd", 1000, false, "iiii", 1, building, i, playerid);
				break;
			}
		}
		case 2: {
			building = GetNearInsideGarage(playerid);
			if(building == -1)	return 1;
			
			for(new i = 0; i < MAX_FURNITURE; i++)
			{
				if (!IsPlayerInRangeOfPoint(playerid, 1.5, GFurn[building][i][fPosX], GFurn[building][i][fPosY], GFurn[building][i][fPosZ]) || !GFurn[building][i][fOn] || !isHouseDoor(GFurn[building][i][fModel]))
				{
					continue;
				}
				if (GFurn[building][i][fOpened])	return SendWarningMessage(playerid, "A porta já está aberta!");

				cmd::server_ame(playerid, "derruba a porta");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,0,1,1);

				SetTimerEx("DoorRamEnd", 1000, false, "iiii", 2, building, i, playerid);
				break;
			}
		}
	}	    
	return 1;
}


this::DoorRamEnd(type, building, i, playerid)
{
	switch(type)
	{
		case 0: {
			HFurn[building][i][fLocked] = 0;
			HFurn[building][i][fOpened] = 1;
				
			MoveDynamicObject(HFurn[building][i][fObject], HFurn[building][i][fPosX], HFurn[building][i][fPosY], HFurn[building][i][fPosZ], 1.5, HFurn[building][i][fPosRX], HFurn[building][i][fPosRY], HFurn[building][i][fPosRZ]-90.0);
				
		}
		case 1: {
			BFurn[building][i][fLocked] = 0;
			BFurn[building][i][fOpened] = 1;
				
			MoveDynamicObject(BFurn[building][i][fObject], BFurn[building][i][fPosX], BFurn[building][i][fPosY], BFurn[building][i][fPosZ], 1.5, BFurn[building][i][fPosRX], BFurn[building][i][fPosRY], BFurn[building][i][fPosRZ]-90.0);
		}
		case 2: {
			GFurn[building][i][fLocked] = 0;
			GFurn[building][i][fOpened] = 1;
				
			MoveDynamicObject(GFurn[building][i][fObject], GFurn[building][i][fPosX], GFurn[building][i][fPosY], GFurn[building][i][fPosZ], 1.5, GFurn[building][i][fPosRX], GFurn[building][i][fPosRY], GFurn[building][i][fPosRZ]-90.0);
		}
	}

	ClearAnimations(playerid);
	GameTextForPlayer(playerid, "~w~VOCE ~y~DERRUBOU~w~ A PORTA", 1000, 1);
	return 1;
}

CMD:arrombar(playerid,params[])
{
	if (!IsPlayerFactionPolice(playerid)) return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (!PlayerInfo[playerid][pOnDuty])	  return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
		    
	new slot;
	
	if (sscanf(params,"d", slot))
	{
		SendHelpMessage(playerid, "/arrombar [opção]");
		SendSyntaxMessage(playerid, "1 - Abrir com uma chave mestra | 2 - Derrubar | 3 - Explodir com C4 | 4 - Abrir a porta do veículo ");
	}

	if (slot < 1 || slot > 4)						 return 1;
    if (slot < 4 && PlayerInfo[playerid][pRank] < 4) return SendErrorMessage(playerid, "Este tipo de arrombamento está disponível a partir da patente 4.");
	if (pTemp[playerid][pRamType])					 return SendErrorMessage(playerid, "Você já está arrombando a porta. ");

    new location[MAX_ZONE_NAME];
	GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);

	if (slot == 4) {
		new vehicle = GetNearVehicle(playerid, 2.0);
		if (!vehicle)							  return SendErrorMessage(playerid, "Não há nenhum carro perto de você.");

		if (VehicleInfo[vehicle][carLocked] == 0)  return SendErrorMessage(playerid, "A fechadura do veículo está aberta.");
		if (VehicleInfo[vehicle][carOwnerID] == 0) return SendErrorMessage(playerid, "O veículo pertence ao estado. ");
		
		cmd::server_ame(playerid, "arromba a fechadura da porta com um grampo.");
		ApplyAnimation(playerid,"OTB","betslp_lkabt", 4.0, 1, 0, 0, 0, 0);

		pTemp[playerid][pRamType] = 1;
		pTemp[playerid][pRamTime] = 30;
		pTemp[playerid][pRamHouse] = -1;
		pTemp[playerid][pRamBiz] = -1;
		pTemp[playerid][pRamGarage] = -1;
		pTemp[playerid][pRamVeh] = vehicle;
		return 1;
	}


	new id = -1;
	if ((id = GetNearOutsideHouse(playerid)) != -1)
	{
	    if (!HouseInfo[id][hOwnerID])									return SendErrorMessage(playerid, "A casa pertence ao estado.");
		if (!HouseInfo[id][hLocked] || HouseInfo[id][hDDoor])			return SendErrorMessage(playerid,"A porta já está aberta.");

		switch(slot)
		{
		    case 1:
		    {
				cmd::server_ame(playerid, "arromba a fechadura da porta com um grampo.");
				ApplyAnimation(playerid,"OTB","betslp_lkabt", 4.0, 1, 0, 0, 0, 0);

				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 30;
				pTemp[playerid][pRamHouse] = id;
				pTemp[playerid][pRamBiz] = -1;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
			case 2:
			{
			    cmd::server_ame(playerid, "derruba a porta com os pés.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,1,1,1);
				HouseInfo[id][hLocked] = 0;
				HouseInfo[id][hDDoor] = 1;
				Save_House(id);
				UpdateHouseDoor(id);
				GameTextForPlayer(playerid, "~w~VOCÊ ~y~DERRUBOU~w~ UMA PORTA", 1000, 1);
				InfoMSG(playerid, "PRESSIONE ~y~\"F\"~w~ OU DIGITE ~y~/ENTRAR", 2);

				new query[128];
				format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] derrubou a porta (/arrombar) casa [H_MYSQL: %i]", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), HouseInfo[id][hID]);
			    insert_log(query, RAM_LOG, 5, PlayerInfo[playerid][pAccID], 0);
			}
			case 3:
			{
			    cmd::server_ame(playerid, "instala C4 na porta.");
				OnPlayAnim(playerid, "BOMBER","BOM_Plant",4.0,0,0,0,0,0);
                pTemp[playerid][pRamObj] = CreateDynamicObject(1654, HouseInfo[id][hPosX], HouseInfo[id][hPosY], HouseInfo[id][hPosZ]-0.8, 0.0, 0.0, 90.0, HouseInfo[id][hPosInt], HouseInfo[id][hPosVW]);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 10;
				pTemp[playerid][pRamHouse] = id;
				pTemp[playerid][pRamBiz] = -1;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
		}
		return 1;
	}

	
	else if ((id = GetNearOutsideBiz(playerid)) != -1)
	{
	    if (BizInfo[id][bizType] == BIZ_GAS_STATION || BizInfo[id][bizType] == BIZ_AUTOSALON || BizInfo[id][bizType] == BIZ_BOATSALON || BizInfo[id][bizType] == BIZ_FLYSALON || BizInfo[id][bizType] == BIZ_TUNING || BizInfo[id][bizType] == BIZ_ADVERT || BizInfo[id][bizType] == BIZ_LICENSE) return 1;
        if (!BizInfo[id][bizOwnerID])							return SendErrorMessage(playerid, "A empresa pertence ao estado.");
		if (!BizInfo[id][bizLocked] || BizInfo[id][bizDDoor])	return SendErrorMessage(playerid, "A porta já está aberta.");

		switch(slot)
		{
		    case 1:
		    {
				cmd::server_ame(playerid, "arromba a fechadura da porta com um grampo.");
				ApplyAnimation(playerid,"OTB","betslp_lkabt", 4.0, 1, 0, 0, 0, 0);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 30;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = id;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
			case 2:
			{
			    cmd::server_ame(playerid, "derruba a porta com os pés.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,1,1,1);
				BizInfo[id][bizLocked] = 0;
				BizInfo[id][bizDDoor] = 1;
				Save_Business(id);
				UpdateBusinessDoor(id);
				GameTextForPlayer(playerid, "~w~VOCÊ ~y~DERRUBOU~w~ UMA PORTA", 1000, 1);
				InfoMSG(playerid, "PRESSIONE ~y~\"F\"~w~ OU DIGITE ~y~/ENTRAR", 2);

				new query[128];
				format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] derrubou a porta (/arrombar) da empresa [B_MYSQL: %i]", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), BizInfo[id][bizID]);
			    insert_log(query, RAM_LOG, 6, PlayerInfo[playerid][pAccID], 0);
			}
			case 3:
			{
			    cmd::server_ame(playerid, "instala C4 na porta.");
				OnPlayAnim(playerid, "BOMBER","BOM_Plant",4.0,0,0,0,0,0);
                pTemp[playerid][pRamObj] = CreateDynamicObject(1654, BizInfo[id][bizPosX], BizInfo[id][bizPosY], BizInfo[id][bizPosZ]-0.8, 0.0, 0.0, 90.0, BizInfo[id][bizPosVW], BizInfo[id][bizPosInt]);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 10;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = id;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
		}
		return 1;
	}


	else if ((id = GetNearOutsideGarage(playerid)) != -1)
	{
        if (!GarageInfo[id][garageOwner])									return SendErrorMessage(playerid, "O garagem pertence ao estado.");
		if (!GarageInfo[id][garageLocked] || GarageInfo[id][garageDDoor])	return SendErrorMessage(playerid, "A porta já está aberta.");

		switch(slot)
		{
		    case 1:
		    {
				cmd::server_ame(playerid, "arromba a fechadura da porta com um grampo.");
				ApplyAnimation(playerid,"OTB","betslp_lkabt", 4.0, 1, 0, 0, 0, 0);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 30;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = -1;
				pTemp[playerid][pRamGarage] = id;
				pTemp[playerid][pRamVeh] = -1;
			}
			case 2:
			{
			    cmd::server_ame(playerid, "derruba a porta com os pés.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,1,1,1);
				GarageInfo[id][garageLocked] = 0;
				GarageInfo[id][garageDDoor] = 1;
				Save_Garage(id);
				UpdateGarageDoor(id);
				GameTextForPlayer(playerid, "~w~VOCÊ ~y~DERRUBOU~w~ UMA PORTA", 1000, 1);
				InfoMSG(playerid, "PRESSIONE ~y~\"F\"~w~ OU DIGITE ~y~/ENTRAR", 2);

				new query[128];
				format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] derrubou a porta (/arrombar) da garagem [B_MYSQL: %i]", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), GarageInfo[id][garageID]);
			    insert_log(query, RAM_LOG, 6, PlayerInfo[playerid][pAccID], 0);
			}
			case 3:
			{
			    cmd::server_ame(playerid, "instala C4 na porta.");
				OnPlayAnim(playerid, "BOMBER","BOM_Plant",4.0,0,0,0,0,0);
                pTemp[playerid][pRamObj] = CreateDynamicObject(1654, GarageInfo[id][garagePlayer_Enter][0], GarageInfo[id][garagePlayer_Enter][1], GarageInfo[id][garagePlayer_Enter][2]-0.8, 0.0, 0.0, 90.0, GarageInfo[id][garageEnterVW], GarageInfo[id][garageEnterInt]);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 10;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = id;
				pTemp[playerid][pRamGarage] = id;
				pTemp[playerid][pRamVeh] = -1;
			}
		}		
		return 1;
	}
	else if (slot == 2) cmd::doorram(playerid, "");
	
	return 1;
}
CMD:revistarportamala(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return 1;
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");

	new vehicle = GetNearVehicle(playerid, 4.0);
	if (vehicle == 0)																															return SendErrorMessage(playerid, "Não há nenhum veículo perto de você.");
	if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))	return SendErrorMessage(playerid, "Este veículo não tem porta-malas. ");
    if (!IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carTrunkLock] == 0)																return SendErrorMessage(playerid, "O porta-malas deste veículo está fechado.");
	if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) 								return SendErrorMessage(playerid, "Você não pode revistar este veículo.");
	
	if (!IsNearTrunk(vehicle, playerid, 2.0)) 	return SendErrorMessage(playerid, "Você não está perto do porta-malas! ");

	PrintVehInv(playerid, vehicle, 1);
	return 1;
}


CMD:revistargb(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid)) return 1;
	if (!PlayerInfo[playerid][pOnDuty])	 return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	
	new vehicle = GetPlayerVehicleID(playerid);
	if (!vehicle && !IsPlayerInAnyVehicle(playerid))																									 return SendErrorMessage(playerid,"Você deve estar em um veículo.");
	if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && VehicleInfo[vehicle][carOwnerID] != 0) return SendErrorMessage(playerid, "Você não pode revistar este veículo.");

	PrintGBInv(playerid, vehicle, 1);
	return 1;
}

CMD:revistarprop(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid)) return 1;
	if (!PlayerInfo[playerid][pOnDuty])	 return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");

	new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Você não está perto de uma casa.");
	if (HouseInfo[houseid][hOwnerID] == PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa pertence a você.");
	if (!IsPlayerInsideHouse(playerid, houseid))					return SendErrorMessage(playerid, "Você deve estar dentro da casa.");

	PrintHouseInv(playerid, houseid, 1);
	return 1;
}

CMD:revistar(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");

	new userid;

	if (sscanf(params,"u", userid))										 return SendHelpMessage(playerid, "/revistar [ID/Parte do nome]");
	if (!IsPlayerLogged(userid))										 return SendNotConnected(playerid);
	if (userid == playerid)												 return 1;
	if (!PlayerNearPlayer(3.0, playerid, userid))						 return SendNotNearPlayer(playerid);
	if (pTemp[userid][pFriskPlayer] == playerid)						 return SendErrorMessage(playerid, "Você já ofereceu para revistar o jogador.");
	if (IsPlayerFactionPolice(playerid) && PlayerInfo[playerid][pOnDuty]) return PrintInvEx(playerid, userid);

    SendWarningMessage(playerid,"Aguarde até que o jogador aceite ou recuse sua oferta de revista.");
	SendSyntaxMessage(userid, "%s quer te revistar.",GetNameEx(playerid));
	SendHelpMessage(userid, "Digite {ffc46a}/aceitarrevista{FFFFFF} para aceitar a ação ou {ffc46a}/cancelarrevista{FFFFFF} para recusar a oferta.");
	
	pTemp[userid][pFriskPlayer] = playerid;
    SetPVarInt(userid, #accept_time, gettime() + 30);
	return 1;
}


CMD:aceitarrevista(playerid, params[])
{
	if (pTemp[playerid][pFriskPlayer] == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Ninguém ofereceu para conduzir uma revista.");

	new userid = pTemp[playerid][pFriskPlayer];

	pTemp[playerid][pFriskPlayer] = INVALID_PLAYER_ID;

	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);

	SendSyntaxMessage(playerid, "Você permitiu que %s conduzisse uma revista em você.", GetNameEx(userid));

	PrintInvEx(playerid, userid);
	return 1;
}

CMD:recusarrevista(playerid, params[])
{
	if (pTemp[playerid][pFriskPlayer] == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Ninguém ofereceu para conduzir uma revista.");

	new userid = pTemp[playerid][pFriskPlayer];
	
	pTemp[playerid][pFriskPlayer] = INVALID_PLAYER_ID;
	SendSyntaxMessage(playerid, "Você recusou a oferta de %s para conduzir uma revista.", GetNameWithMask(userid));
	SendSyntaxMessage(userid, "%s recusou sua oferta de revista.", GetNameWithMask(playerid));
	return 1;
}

CMD:definirplaca(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial.");

    new vehicleid = GetPlayerVehicleID(playerid);

    if (!vehicleid || VehicleInfo[vehicleid][carFaction] != PlayerInfo[playerid][pFaction])	
		return SendErrorMessage(playerid, "Você não está em um veículo fracional.");

    new plate[24];

    format(plate, sizeof(plate), "%s", GetVehicleNumber());

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `plate` FROM `cars` WHERE `plate` = '%e'", plate);
	mysql_tquery(dbHandle, query, "PoliceSetPlate", "iis", playerid, vehicleid, plate);
	return 1;
}

CMD:sonda(playerid, params[])
{
	if (pTemp[playerid][pZond] != 0)		return SetPlayerZond(playerid, 0);
    if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid,"Você deve ser um policial / agente prisional.");
    if (!PlayerInfo[playerid][pSwatDuty])	return SendErrorMessage(playerid, "Esta função está disponível para o S.W.A.T.");

	SetPlayerZond(playerid, 1);
	return 1;
}


CMD:flashbang(playerid, params[])
{
    if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial / agente prisional.");
    if (!PlayerInfo[playerid][pSwatDuty])	return SendErrorMessage(playerid, "Esta função está disponível para o S.W.A.T.");

    new id = -1;

	if ((id = GetNearOutsideBiz(playerid)) != -1 && (BizInfo[id][bizType] != BIZ_AUTOSALON && BizInfo[id][bizType] != BIZ_ADVERT && BizInfo[id][bizType] != BIZ_TUNING && BizInfo[id][bizType] != BIZ_LICENSE && BizInfo[id][bizType] != BIZ_BOATSALON && BizInfo[id][bizType] != BIZ_FLYSALON))
	{
	    if (BizInfo[id][bizLocked])	return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);

		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* %s %s usou uma granada flashbang.", GetRankName(playerid), GetNameEx(playerid));
		cmd::me(playerid, "removendo de seu cinto, joga uma granada flash-ruído dentro do edifício.");
		
		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !IsPlayerBizInside(i, id)) continue;
		    
			SendSyntaxMessage(i, "Você foi cegado por uma flashbang.");
			GameTextForPlayer(i, "~r~!!! VOCÊ FOI ATINGIDO POR UMA FLASHBANG !!!~n~~n~~n~!!! VOCÊ FOI ATINGIDO POR UMA FLASHBANG !!!~n~~n~~n~!!! VOCÊ FOI ATINGIDO POR UMA FLASHBANG !!!", 12000, 3);
            SetPlayerWeather(i, 36);
			SetPlayerTime(i, 22, 0);
			StopPlayerAnims(i);
			FreezePlayer(i);
			SetPlayerDrunkLevel(i, 50000);
			pTemp[i][pFlashTime] = 15;
		}
		return 1;
	}
	if ((id = GetNearOutsideHouse(playerid)) != -1)
	{
	    if (HouseInfo[id][hLocked])	return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);

        SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* %s %s usou uma granada flashbang.", GetRankName(playerid), GetNameEx(playerid));
		cmd::me(playerid, "removendo de seu cinto, joga uma granada flash-ruído dentro do edifício.");
		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !IsPlayerInsideHouse(i, id)) continue;
		    
			SendSyntaxMessage(i, "Você foi cegado por uma flashbang.");
			GameTextForPlayer(i, "~r~!!! VOCÊ FOI ATINGIDO POR UMA FLASHBANG !!!~n~~n~~n~!!! VOCÊ FOI ATINGIDO POR UMA FLASHBANG !!!~n~~n~~n~!!! VOCÊ FOI ATINGIDO POR UMA FLASHBANG !!!", 12000, 3);
            SetPlayerWeather(i, 36);
			SetPlayerTime(i, 22, 0);
			StopPlayerAnims(i);
			FreezePlayer(i);
			SetPlayerDrunkLevel(i, 50000);
			pTemp[i][pFlashTime] = 15;
		}
		return 1;
	}

	SendErrorMessage(playerid, "Use na entrada de sua casa / empresa.");
	return 1;
}


CMD:callsign(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction];
	
    // Verifica se o jogador pertence à facção policial
	if (!IsFactionPolice(faction))	return SendErrorMessage(playerid, "Você deve ser um policial.");
	
	if (pTemp[playerid][pPatrol])
	{
	    new id = pTemp[playerid][pPatrol]-1;

	    pTemp[playerid][pPatrol] = 0;

        // Verifica se a patrulha é válida
	    if (id < 0 || id >= MAX_PATROL || !PatrolInfo[GetPatrolID(playerid)][id][patrulExists])	return 1;

        // Remove o jogador da patrulha
        if (PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] == playerid) 	PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] = INVALID_PLAYER_ID;
        if (PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] == playerid) 	PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] = INVALID_PLAYER_ID;

        // Verifica se a patrulha ainda é válida e, em caso negativo, a remove
		if (PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] == INVALID_PLAYER_ID && PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] == INVALID_PLAYER_ID)	
            PatrolInfo[GetPatrolID(playerid)][id][patrulExists] = 0;

	    // Envia uma mensagem informando que o jogador deixou a patrulha
	    SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "[DISP] %s %s deixou a patrulha sob o callsign %s", GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][id][patrulName]);
		return 1;
	}

    // Verifica se o jogador está de serviço
    if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");

    // Verifica se o parâmetro foi fornecido
	if (isnull(params))	return SendHelpMessage(playerid, "/callsign [marcação]");
	
	for(new i; i != MAX_PATROL; i++)
	{
	    // Verifica se a patrulha existe e se os identificadores coincidem
	    if (!PatrolInfo[GetPatrolID(playerid)][i][patrulExists] || !CompareStrings(params, PatrolInfo[GetPatrolID(playerid)][i][patrulName]))	continue;
        
		if (PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0] != INVALID_PLAYER_ID && PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1] != INVALID_PLAYER_ID)	
			return SendErrorMessage(playerid, "Esta patrulha já está completa.");

        // Adiciona o jogador à patrulha existente
        if (PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0] == INVALID_PLAYER_ID) PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0] = playerid;
        else 	PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1] = playerid;

		pTemp[playerid][pPatrol] = i+1;

        // Envia uma mensagem informando que o jogador entrou na patrulha
		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "[DISP] %s %s entrou na patrulha sob o callsign %s", GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][i][patrulName]);
		return 1;
	}

	new id = -1;
	
    // Procura uma patrulha disponível
	for(new i; i != MAX_PATROL; i++)
	{
	    if (PatrolInfo[GetPatrolID(playerid)][i][patrulExists]) continue;

		id = i;
		break;
	}

	if (id == -1)	return SendErrorMessage(playerid, "O número máximo de patrulhas foi atingido.");

    // Cria uma nova patrulha e adiciona o jogador a ela
    pTemp[playerid][pPatrol] = id+1;
    PatrolInfo[GetPatrolID(playerid)][id][patrulExists] = 1;
    format(PatrolInfo[GetPatrolID(playerid)][id][patrulName], 32, "%s", params);
    PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] = playerid;
    PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] = INVALID_PLAYER_ID;
    
    // Envia uma mensagem informando que o jogador começou uma nova patrulha
    SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "[DISP] %s %s iniciou uma patrulha sob o identificador %s", GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][id][patrulName]);
	return 1;
}


CMD:meg(playerid, params[])
{
    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerFactionPolice(playerid))									return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "Disponível apenas em veículos LSPD/SASD.");


	new type;
	if (sscanf(params,"i",type))
	{
	    new _mes[447] = "1. This is the Los Santos Police Department; Stay where you all are!\n2. This is Buzzard 1, we are taking ground fire!\n3. Four bangers in blue 4door heading back into South Central.\n";
	   
		strcat(_mes, "4. Mayday, mayday! Buzzard 1 is going down!\n5. This is the LSPD do not, HEY! WHAT THE FUCK? TOO LOW, YOU'LL KILL US ALL!\n6. LSPD, stop right... are you insane? You'll kill us all!\n7. This is the Los Santos Police Department; Everybody stay where you are!");
	    Dialog_Show(playerid, MPCarVoice, DIALOG_STYLE_LIST, "{8D8DFF}Megafone", _mes, "Selecionar", "Cancelar");
	    return SendHelpMessage(playerid, "/meg [ID do Som (1-7)]");
	}
	MegaphoneSoundVoice(playerid, type);
	return 1;
}

CMD:revogar(playerid, params[])
{
	new userid, type[24], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))					return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	if (sscanf(params,"us[24]", userid, type))		return SendHelpMessage(playerid, "/revogar [ID/Parte do Nome] [carlic | aviaolic | armas]");
	if (userid == playerid)							return 1;
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(4.0, playerid, userid))	return SendNotNearPlayer(playerid);

	if (CompareStrings(type, "carlic"))
	{
	    if (!PlayerInfo[userid][pCarLic])	return SendErrorMessage(playerid, "O jogador não possui uma carteira de motorista.");

		new mes[128];
		format(mes, sizeof(mes), "tira de %s a carteira de motorista.", GetNameWithMask(userid));
		cmd::me(playerid, mes);

		SendSyntaxMessage(userid, "Sua carteira de motorista foi retirada por %s.", GetNameWithMask(playerid));
		PlayerInfo[userid][pCarLic] = 0;
		SQL_SetInteger("users", "carlic", 0, PlayerInfo[userid][pID]);
		return 1;
	}

	else if (CompareStrings(type, "aviaolic"))
	{
	    if (!PlayerInfo[userid][pFlyLic])	return SendErrorMessage(playerid, "O jogador não possui uma licença de voo.");

	    new mes[128];
		format(mes, sizeof(mes), "tira de %s a licença de voo.", GetNameWithMask(userid));
		cmd::me(playerid, mes);

		SendSyntaxMessage(userid, "Sua licença de voo foi retirada por %s.", GetNameWithMask(playerid));
		PlayerInfo[userid][pFlyLic] = 0;
		SQL_SetInteger("users", "flylic", 0, PlayerInfo[userid][pID]);
		return 1;
	}

	else if (CompareStrings(type, "armas"))
	{
		new count = -1;
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
			{
				count = i;
				break;
			}
		}
		if (count == -1 && !PlayerInfo[playerid][pPlayerWeapon])	return SendErrorMessage(playerid, "O jogador não possui uma arma");

	    new mes[128];
		format(mes, sizeof(mes), "tira de %s a arma.", GetNameWithMask(userid));
		cmd::me(playerid, mes);

		SendWarningMessage(userid, "Todas as suas armas foram retiradas por %s.", GetNameWithMask(playerid));
		ResetPlayerWeaponsEx(userid, 0);
		return 1;
	}
	else 
	{
		SendHelpMessage(playerid, "/revogar [ID/Parte do Nome] [carlic | aviaolic | armas]");
	}
	return 1;
}


CMD:deter(playerid, params[])
{
	new userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))												return SendErrorMessage(playerid, "Você deve ser um policial / agente prisional.");
	if (sscanf(params,"u", userid))																		return SendHelpMessage(playerid, "/deter [ID/Parte do Nome]");
	if (userid == playerid)																				return 1;
	if (!PlayerInfo[playerid][pOnDuty])																	return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	if (!IsPlayerInAnyVehicle(playerid))																return SendNotInVehicle(playerid);
	if (!IsPlayerLogged(userid))																			return SendNotConnected(playerid);
	if (!PlayerNearPlayer(10.0, playerid, userid))														return SendNotNearPlayer(playerid);
	if (!IsPlayerInAnyVehicle(userid) || VehicleInfo[GetPlayerVehicleID(userid)][carFaction] != PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "O jogador não está em um veículo especial.");

	new mes[128];

	if (!IsPlayerFreeze(userid))
	{
		format(mes, sizeof(mes), "tranca todas as portas de %s.", GetNameWithMask(userid));
		FreezePlayer(userid);
	}
	else
	{
		format(mes, sizeof(mes), "%s destrancou todas as portas.", GetNameWithMask(userid));
		UnFreezePlayer(userid);
	}
	cmd::me(playerid, mes);
	return 1;
}

CMD:prender(playerid, params[])
{
	new userid, time, info[128], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))						return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (sscanf(params,"uds[128]", userid, time, info))	return SendHelpMessage(playerid, "/prender [ID/Parte do Nome] [tempo (minutos)] [motivo]");
	if (!PlayerInfo[playerid][pOnDuty])					return SendErrorMessage(playerid, "Você deve estar em serviço.");

	new arrestid = 0;

	if ((arrestid = GetArrestID(playerid)) == 0)	return SendErrorMessage(playerid, "Você deve estar nas câmeras!");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
    if (userid == playerid)							return SendErrorMessage(playerid, "Você não pode usar esta ação em si mesmo.");
    if (PlayerInfo[userid][pJailed])				return SendErrorMessage(playerid, "O jogador já está sob custódia.");
	if (time < 1 || time > 60)						return SendErrorMessage(playerid, "O tempo de prisão não pode exceder de 1 a 60 minutos.");

    Add_Arrest(playerid, userid, time);
	PlayerInfo[userid][pJailed] = arrestid;
	PlayerInfo[userid][pJailTime] = time * 60;
	PlayerInfo[userid][pArrested]++;
	SQL_SetInteger("users", "arrested", PlayerInfo[userid][pArrested], PlayerInfo[userid][pID]);
	AddCriminalRecord(userid, info, PlayerInfo[playerid][pUsername], PlayerInfo[playerid][pFaction], 1);

	PutPlayerInPrison(userid);
	SendErrorMessage(userid, "Você foi preso. Tempo: %s minutos.", FormatNumber(time));
	pTemp[playerid][pPoliceJails]++;
	return 1;
}


CMD:presidio(playerid, params[])
{
	return SendErrorMessage(playerid, "A prisão ainda está desativada!");
	/*new userid, time, info[128], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))		return SendErrorMessage(playerid, "Você deve ser um policial / agente prisional.");
	if (sscanf(params,"uds[128]", userid, time, info))				return SendHelpMessage(playerid, "/prison [ID/Parte do Nome] [tempo (minutos)] [motivo]");
	if (!PlayerInfo[playerid][pOnDuty])								return SendErrorMessage(playerid, "Você deve estar de serviço.");
	if (!PlayerToPoint(5.0, playerid, 241.6535,-315.2582,947.9460))	return SendErrorMessage(playerid, "Você não está na prisão.");
	if (!IsPlayerLogged(userid))									return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))					return SendNotNearPlayer(playerid);
	if (time < 1 || time > 1000)									return SendErrorMessage(playerid, "Tempo de 1 a 1000 minutos.");
	if (userid == playerid)											return 1;

	PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));
	PlayerInfo[userid][pJailed] = PRISON_JAIL;
	PlayerInfo[userid][pJailTime] = time * 60;
	PlayerInfo[userid][pIsolateTime] = 0;
	SendSyntaxMessage(userid, "Você foi preso. Tempo: %s minutos.", FormatNumber(time));
	SendSyntaxMessage(userid, "Seu número de prisioneiro é - A%i(/myprisonnumber).", PlayerInfo[userid][pPrisoned]+100);
	return 1;*/
}

CMD:verplaca(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
	if (!IsFactionPolice(faction))						return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (!PlayerInfo[playerid][pOnDuty])					return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");
	if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)	return SendErrorMessage(playerid, "Você deve estar próximo ao veículo.");

	new vehicle;

	if ((vehicle = GetNearVehicle(playerid, 2.5)) == 0)	return SendErrorMessage(playerid, "Não há veículos próximos a você.");

	if (VehicleInfo[vehicle][carRegistered] == 0)	SendSyntaxMessage(playerid, "A placa do veículo %s não está registrada.", VehicleNames[GetVehicleModel(vehicle)-400]);
	else											SendSyntaxMessage(playerid, "A placa do veículo %s é %s.", VehicleNames[GetVehicleModel(vehicle)-400], VehicleInfo[vehicle][carPlate]);
	
	return 1;
}

CMD:algemar(playerid, params[])
{
	new userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))					return SendErrorMessage(playerid, "Você deve ser um policial / agente prisional.");
	if (sscanf(params,"u",userid))					return SendHelpMessage(playerid, "/algemar [ID/Parte do Nome]");
	if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (userid == playerid)							return 1;
	if (pTemp[userid][pCuffed])						return SendErrorMessage(playerid, "O jogador já está algemado.");
	if (pTemp[userid][pTaserTime])					return SendErrorMessage(playerid, "O jogador está atordoado ou inconsciente. ");

	GameTextForPlayer(userid, "~r~Algema", 2500, 3);
	pTemp[userid][pCuffed] = 1;
	SendSyntaxMessage(userid, "%s algemou você.", GetNameWithMask(playerid));
	SendSyntaxMessage(playerid, "Você algemou %s.", GetNameWithMask(userid));
	SetPlayerAttachedObject(userid, ATTACH_HANDS, 19418,6,0.003999,0.028000,-0.018000,-12.900005,0.000000,-65.100006,0.878000,1.890999,1.652001);
	SetPlayerSpecialAction(userid, SPECIAL_ACTION_CUFFED);
	return 1;
}

CMD:desalgemar(playerid, params[])
{
	new userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))					return SendErrorMessage(playerid, "Você deve ser um policial / agente prisional.");
	if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/desalgemar [ID/Parte do Nome]");
	if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (userid == playerid)							return 1;
	if (!pTemp[userid][pCuffed])					return SendErrorMessage(playerid,"O jogador não está algemado.");

	pTemp[userid][pCuffed] = 0;
	SendSyntaxMessage(userid, "%s removeu suas algemas.", GetNameWithMask(playerid));
	SendSyntaxMessage(playerid, "Você removeu as algemas de %s.", GetNameWithMask(userid));
	RemovePlayerAttachedObject(userid, ATTACH_HANDS);
	SetPlayerSpecialAction(userid, SPECIAL_ACTION_NONE);
	return 1;
}


ALTX:perseguicao("/plist", "/liderpos");
CMD:perseguicao(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
	if (!IsFactionPolice(faction))		return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Você deve estar de serviço (/duty).");
	
	new userid;
	if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/perseguicao [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)							return 1;
	if (GetPVarInt(playerid, #Chase_Time) > 1)		return SendErrorMessage(playerid, "Para %s [ID:%i] a perseguição já está em andamento.", GetNameWithMask(userid), userid);
	if (!PlayerNearPlayer(15.0, playerid, userid))	return SendNotNearPlayer(playerid);

	SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "(( QG: %s %s iniciou uma perseguição contra %s [ID:%i]. ))", GetRankName(playerid), GetNameEx(playerid), GetNameWithMask(userid), userid);

	StartChase(userid);	
	return 1;
}


CMD:sirene(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid), model = GetVehicleModel(vehicle), siren_id;

	if (!IsFactionLegal(faction))											return SendErrorMessage(playerid, "Você deve ser um policial / agente penitenciário / paramédico / governo.");
    if (VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "Você deve estar em um veículo oficial.");
	if (sscanf(params, "i", siren_id))										return SendHelpMessage(playerid, "/sirene [1-3]");
	if (siren_id < 1 || siren_id > 3)										return SendErrorMessage(playerid, "ID de sirene inválido: 1-3.");

	if (siren_id == 1)
	{
		if (IsValidDynamicObject(VehicleInfo[vehicle][carSiren][0]))
		{
		    SendSyntaxMessage(playerid, "A sirene está desligada.");
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][0]);
			VehicleInfo[vehicle][carSiren][0] = INVALID_OBJECT_ID;
		    return 1;
		}

		if (model != 560 && model != 541 && model != 525 && model != 426 && model != 579 && model != 415 && model != 554 && model != 546 && model != 566 && model != 561 && model != 547 && model != 482)
		{
			return SendErrorMessage(playerid, "A sirene só pode ser colocada em Sultan/Bullet/Towtruck/Premier/Huntley/Cheetah/Yosemite/Intruder/Tahoma/Stratum/Primo/Burrito.");
		}

	    VehicleInfo[vehicle][carSiren][0] = CreateDynamicObject((model != 525)?(18646):(19419), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
		switch(model)
	    {
	        case 560: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.571924, 0.068632, 0.860007, 0.000000, 0.000000, -181.148086);
	        case 541: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.442310, -0.091429, 0.670001, 0.000000, -2.299999, -178.874786);
	        case 426: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.570416, -0.012622, 0.890005, 0.000000, 0.000000, -91.267646);
	        case 525: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.0, -0.485, 1.36, 0.000000, 0.000000, 180.0);
	        case 579: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.640568, -0.021741, 1.260003, 0.000000, 0.000000, -90.153976);
	        case 415: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.424245, -0.241412, 0.640021, 3.000001, -3.499999, -181.499969);
	        case 554: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.734659, 0.117240, 1.059349, -4.199999, 0.000000, -87.859664);
	        case 546: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.591740, -0.153342, 0.870005, 0.000000, 0.000000, -269.351074);
	        case 566: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.660138, 0.024635, 0.869999, 0.000000, 0.000000, -88.731002);
	        case 561: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.428671, 0.080074, 0.850004, -2.799999, 0.000000, -88.665367);
	        case 547: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.530513, 0.002143, 0.916606, -3.899999, 0.000000, -89.768531);
	        case 482: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.515804, 0.702674, 0.950006, 0.000000, 0.000000, -92.038864);
	    }

	    Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
	    return SendSyntaxMessage(playerid, "A sirene está instalada.");
	}

	else if (siren_id == 2)
	{
	    if (IsValidDynamicObject(VehicleInfo[vehicle][carSiren][1]))
		{
		    SendSyntaxMessage(playerid, "A sirene está desligada.");
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][1]);
			VehicleInfo[vehicle][carSiren][1] = INVALID_OBJECT_ID;
		    return 1;
		}

	    if (model != 560 && model != 426 && model != 579 && model != 566 && model != 561 && model != 547 && model != 546)
		{
			return SendErrorMessage(playerid, "A sirene só pode ser colocada em Sultan/Premier/Huntley/Tahoma/Stratum/Primo/Intruder.");
		}

        VehicleInfo[vehicle][carSiren][1] = CreateDynamicObject(19419, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
        switch(model)
	    {
	        case 560: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.000000, -1.289999, 0.394999, 0.000000, 0.000000, 0.000000);
	        case 426: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.000000, -1.669998, 0.349999, 0.000000, 0.000000, 0.000000);
	        case 579: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.005192, -2.445963, 0.521951, 0.000000, 0.000000, -359.884796);
	        case 566: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.009878, -1.681756, 0.400007, 0.000000, 0.000000, -0.010757);
	        case 561: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, -0.004864, -2.317730, 0.249995, 0.000000, 0.000000, -0.192703);
	        case 547: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, -0.001344, -1.571307, 0.400003, 0.000000, 0.000000, -0.468620);
	        case 546: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.014445, -1.771190, 0.340004, 0.000000, 0.000000, -359.851013);
	    }
	    Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
		return SendSyntaxMessage(playerid,"A sirene está instalada.");
	}

	else if (siren_id == 3)
	{
	    if (IsValidDynamicObject(VehicleInfo[vehicle][carSiren][2]))
		{
		    SendSyntaxMessage(playerid, "A sirene está desligada.");
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][2]);
			VehicleInfo[vehicle][carSiren][2] = INVALID_OBJECT_ID;
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][3]);
			VehicleInfo[vehicle][carSiren][3] = INVALID_OBJECT_ID;
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][4]);
			VehicleInfo[vehicle][carSiren][4] = INVALID_OBJECT_ID;
		    return 1;
		}

	    if (model != 541 && model != 415 && model != 560)	return SendErrorMessage(playerid, "A sirene só pode ser colocada em Bullet/Cheetah/Sultan.");

        if (model == 541)
		{
			VehicleInfo[vehicle][carSiren][2] = CreateDynamicObject(19620, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][2], vehicle, 0.000000, 0.000000, 0.659999, 0.000000, 0.000000, -359.823394);
			VehicleInfo[vehicle][carSiren][3] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][3], vehicle, 0.006984, -2.280384, -0.190001, 0.000000, 0.000000, -359.824523);
			VehicleInfo[vehicle][carSiren][4] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][4], vehicle, -0.007443, 2.430286, -0.070001, 0.000000, 0.000000, -180.024566);
		}
		if (model == 415)
		{
			VehicleInfo[vehicle][carSiren][2] = CreateDynamicObject(19620, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][2], vehicle, 0.000475, -0.270263, 0.630008, 0.000000, 0.000000, -359.899139);
			VehicleInfo[vehicle][carSiren][3] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][3], vehicle, -0.005538, 2.660761, -0.210004, 0.000000, 0.000000, -180.099121);
			VehicleInfo[vehicle][carSiren][4] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][4], vehicle, 0.003255, -2.460503, -0.140003, 0.000000, 0.000000, 0.266403);
		}
		if (model == 560)
		{
			VehicleInfo[vehicle][carSiren][2] = CreateDynamicObject(19620, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][2], vehicle, -0.000927, 0.110103, 0.870014, 0.000000, 0.000000, -359.517333);
			VehicleInfo[vehicle][carSiren][3] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][3], vehicle, 0.004800, -2.370894, 0.105430, 0.000000, 0.000000, -359.912292);
			VehicleInfo[vehicle][carSiren][4] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][4], vehicle, -0.003252, 2.561954, 0.050001, 0.000000, 0.000000, -179.912200);
		}

		Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
		return SendSyntaxMessage(playerid,"A sirene está instalada.");
	}
	return 1;
}

CMD:licencaarmas(playerid, params[])
{
    new userid, price;
		
	if (sscanf(params,"ui",userid, price))			return SendHelpMessage(playerid, "/licencaarmas [ID/Parte do Nome] [custo]");
	if (IsPlayerFactionPolice(playerid))			return SendErrorMessage(playerid, "Você deve ser um membro da LSPD.");
	if (PlayerInfo[playerid][pAuthwep] != 1)		return SendErrorMessage(playerid, "Você não tem autoridade para emitir licenças de armas.");
	if (price < 1 || price > 10000)					return SendErrorMessage(playerid, "Custo de $ 1 a $ 10.000.");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
    if (PlayerInfo[userid][pWepLic] != 0)			return SendErrorMessage(playerid, "O jogador já tem uma licença de arma.");
	if (pTemp[userid][pWepPlayer] == playerid)		return SendErrorMessage(playerid, "Você já ofereceu ao jogador a compra de uma licença.");
	    
	pTemp[userid][pWepPlayer] = playerid;
	pTemp[userid][pWepPrice] = price;
	SetPVarInt(userid, #accept_time, gettime() + 30);
	SendSyntaxMessage(userid, "%s %s convida você para comprar uma licença de arma por $%i. (Clique em Y para comprar uma licença)", GetRankName(playerid), GetNameEx(playerid), price);
	SendSyntaxMessage(playerid, "Você ofereceu a %s a compra de uma licença de arma por $%i.", GetNameEx(userid), price);
	return 1;
}


CMD:autorizarlicenca(playerid, params[])
{
    new userid;

	if (sscanf(params,"u",userid))			return SendHelpMessage(playerid, "/autorizarlicenca [ID/Parte do Nome]");
	if (IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um membro da LSPD.");
	if (!isLeader(playerid))				return SendErrorMessage(playerid, "Você não é um líder de facção.");
	if (!IsPlayerLogged(userid))			return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))						return SendNotNearPlayer(playerid);
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "O jogador não é um membro da sua organização.");

	if (PlayerInfo[userid][pAuthwep] == 0)
	{
		PlayerInfo[userid][pAuthwep] = 1;
		SendSyntaxMessage(userid, "%s %s lhe deu autoridade para emitir licenças de armas.", GetRankName(playerid), GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Você emitiu autoridade para emitir licenças de armas a %s.", GetNameEx(userid));
		return 1;
	}

	PlayerInfo[userid][pAuthwep] = 0;
	SendSyntaxMessage(userid, "%s %s retirou sua autoridade para emitir licenças de armas", GetRankName(playerid), GetNameEx(playerid));
	SendSyntaxMessage(playerid, "Você retirou a autoridade de %s para emitir licenças de armas.", GetNameEx(userid));
	return 1;
}


CMD:autorizarswat(playerid, params[])
{
	new userid;

	if (sscanf(params,"u",userid))			return SendHelpMessage(playerid, "/autorizarswat [ID/Parte do Nome]");
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!isLeader(playerid))				return SendErrorMessage(playerid, "Você não é um líder de facção.");
	if (!IsPlayerLogged(userid))			return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))						return SendNotNearPlayer(playerid);
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "O jogador não é um membro da sua organização.");

	if (PlayerInfo[userid][pSwat] == 0)
	{
		PlayerInfo[userid][pSwat] = 1;
		SendSyntaxMessage(userid, "%s %s emitiu os poderes de um empregado do grupo de captura S.W.A.T", GetRankName(playerid), GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Você autorizou %s S.W.A.T.", GetNameEx(userid));
		return 1;
	}

	PlayerInfo[userid][pSwat] = 0;
	SendSyntaxMessage(userid, "%s %s retirou os poderes de um empregado do grupo de captura S.W.A.T", GetRankName(playerid), GetNameEx(playerid));
	SendSyntaxMessage(playerid, "Você retirou os poderes de %s S.W.A.T.", GetNameEx(userid));
	return 1;
}


// IMPOUND COORDINATES (CHANGE HERE)
#define IMPOUNDX (-1209.9501)
#define IMPOUNDY (-1064.3761)
#define IMPOUNDZ (128.2656)

CMD:guinchar(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionPolice(faction))				return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (!PlayerInfo[playerid][pOnDuty])			return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	if (!IsATowTruck(vehicle))					return SendErrorMessage(playerid, "Você deve estar em um guincho.");
    if (!IsTrailerAttachedToVehicle(vehicle))	return SendErrorMessage(playerid, "Nada está atrelado ao guincho.");
	
	new trailer = GetVehicleTrailer(vehicle);

	if (!VehicleInfo[trailer][carOwnerID])								return SendErrorMessage(playerid, "Apenas veículos pessoais podem ser enviados para o depósito.");
	if (!PlayerToPoint(10.0, playerid, IMPOUNDX, IMPOUNDY, IMPOUNDZ))	return SendErrorMessage(playerid, "Você está muito longe do estacionamento de multas.");

	DetachTrailerFromVehicle(vehicle);
	GetVehiclePos(trailer, VehicleInfo[trailer][carParkX], VehicleInfo[trailer][carParkY], VehicleInfo[trailer][carParkZ]);
    GetVehicleZAngle(trailer, VehicleInfo[trailer][carParkA]);

	SendSyntaxMessage(playerid, "%s enviado para o depósito.", VehicleNames[GetVehicleModel(trailer)-400]);
	VehicleEngine(trailer, false);
	
	VehicleInfo[trailer][Impounded] = 1;

	Save_Car(trailer);
	
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || PlayerInfo[i][pCarKey] != trailer) continue;

		SendSyntaxMessage(i, "Seu %s foi atrelado e enviado para o depósito por %s %s.", VehicleNames[GetVehicleModel(trailer)-400], GetRankName(playerid), GetNameEx(playerid));
		break;
	}
	return 1;
}

CMD:rapel(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionPolice(faction))			return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	if (!PlayerInfo[playerid][pSwatDuty])	return SendErrorMessage(playerid, "Esta função está disponível para S.W.A.T.");
	if (GetPVarInt(playerid, #SWATROPE))	return 1;
	if (IsPlayerInAnyVehicle(playerid))		return SendErrorMessage(playerid,"Você deve estar em uma helicoptero.");
	if (IsAHelicopter(vehicle))				return SendErrorMessage(playerid,"Você deve estar em uma helicoptero.");
	if (!GetPlayerVehicleSeat(playerid))	return SendErrorMessage(playerid, "Você deve estar no assento do passageiro.");

	OnPlayerCallRappelling(playerid);
	return 1;
}

CMD:rebocar(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionPolice(faction))			return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	if (!IsATowTruck(vehicle))				return SendErrorMessage(playerid, "Você deve estar no guincho.");
    if (IsTrailerAttachedToVehicle(vehicle))
	{
		DetachTrailerFromVehicle(vehicle);
		return SendErrorMessage(playerid, "O veículo previamente rebocado foi desengatado.");
	}

	new vehid;

    if (sscanf(params,"i", vehid))														return SendHelpMessage(playerid, "/rebocar [ID do veículo]");
	if (vehid < 1 || vehid >= MAX_VEHICLES)												return 1;
    if (GetNearVehicle(playerid, 14.0, vehid) == 0)										return SendErrorMessage(playerid, "Este veículo deve estar próximo do seu veículo.");
    if (IsABoat(vehid) || IsAPlane(vehid) || IsAHelicopter(vehid) || IsABiker(vehid))	return SendErrorMessage(playerid, "Este veículo não pode ser rebocado.");
		
    AttachTrailerToVehicle(vehid, vehicle);
	SendSyntaxMessage(playerid, "Você rebocou %s. Use /rebocar para desatrelar.", VehicleNames[GetVehicleModel(vehid)-400]);
	return 1;
}


CMD:apb(playerid, params[])
{
	new action[24], secoption[153], tmp[2][128], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))		return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");

    if (sscanf(params, "s[24]S()[153]", action, secoption))
	{
		SendHelpMessage(playerid, "/apb [opção]");
		SendSyntaxMessage(playerid, "criar / limpar / mostrar / editar / listar");
		return 1;
	}

	if (CompareStrings(action, "lista"))
	{
	    new count;
	   
		for(new i = 0; i != MAX_APBS; i++)
	    {
	        if (ApbInfo[i][apbOn] == 0) continue;

	        if (strlen(ApbInfo[i][apbCharge]) > 64)	SendClientMessageEx(playerid, COLOR_LIGHTRED, "APB %i:{FFFFFF} %.64s ... (%s)", i, ApbInfo[i][apbCharge], ApbInfo[i][apbSuspect]);
	        else									SendClientMessageEx(playerid, COLOR_LIGHTRED, "APB %i:{FFFFFF} %s (%s)", i, ApbInfo[i][apbCharge], ApbInfo[i][apbSuspect]);

			count++;
	    }

	    if (!count)	return SendErrorMessage(playerid,"Notas de APB não encontradas.");

		return 1;
	}
	else if (CompareStrings(action, "ver"))
	{
		new number;

		if (sscanf(secoption,"i", number))	return SendHelpMessage(playerid, "/apb ver [número do APB]");
		if (!ApbInfo[number][apbOn])			return SendErrorMessage(playerid, "Nota de APB não encontrada.");

		SCM(playerid, -1, "____________________________________");
		SendClientMessageEx(playerid, COLOR_LIGHTRED, "Suspeito:{FFFFFF} %s", ApbInfo[number][apbSuspect]);
		
		if (strlen(ApbInfo[number][apbCharge]) > 64)
		{
		    SendClientMessageEx(playerid, COLOR_LIGHTRED, "Descrição:{FFFFFF} %.64s ...", ApbInfo[number][apbCharge]);
		    SendClientMessageEx(playerid, -1, "... %s", ApbInfo[number][apbCharge][64]);
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_LIGHTRED, "Descrição:{FFFFFF} %s", ApbInfo[number][apbCharge]);
		}

        SendClientMessageEx(playerid, COLOR_LIGHTRED, "Adicionado por:{FFFFFF} %s, %s.", ApbInfo[number][apbOfficer], Factions[ApbInfo[number][apbFaction]][fShortName]);
		SCM(playerid, -1, "____________________________________");
	}
	else if (CompareStrings(action, "limpar"))
	{
		new number;

		if (sscanf(secoption,"i", number))	return SendHelpMessage(playerid, "/apb limpar [número do APB]");
		if (!ApbInfo[number][apbOn])		return SendErrorMessage(playerid, "Nota de APB não encontrada.");

		SendSyntaxMessage(playerid, "Você limpou a nota de APB número %i.", number);
		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s deletou a nota de APB número %i.", GetRankName(playerid), GetNameEx(playerid), number);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `apbs` WHERE `id` = %i", ApbInfo[number][apbID]);
		mysql_tquery(dbHandle, query);

		ApbInfo[number][apbOn] =
		ApbInfo[number][apbID] = 0;
	}
	else if (CompareStrings(action, "editar"))
	{
		new number, info[153];

		if (sscanf(secoption,"is[153]", number, info))	return SendHelpMessage(playerid, "/apb editar [número do APB] [suspeito / descrição]");
		if (!ApbInfo[number][apbOn])					return SendErrorMessage(playerid, "Nota de APB não encontrada.");
        if (strfind(info, "/", true) == -1)				return SendErrorMessage(playerid, "Nenhum símbolo encontrado \"/\".");
		
		SendSyntaxMessage(playerid, "Você modificou a nota de APB [NUM: %i].", number);
		strexplode(tmp, info, "/");
		format(ApbInfo[number][apbSuspect], MAX_PLAYER_NAME, "%s", tmp[0]);
		format(ApbInfo[number][apbCharge], 128, "%s", tmp[1]);
		ApbInfo[number][apbFaction] = faction;

		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s modificou o APB número %i.", GetRankName(playerid), GetNameEx(playerid), number);
		
		new query[128 + 128];
		mysql_format(dbHandle,query, sizeof(query), "UPDATE `apbs` SET `charge` = '%s', `suspect` = '%e' WHERE `id` = %i", tmp[1], tmp[0], ApbInfo[number][apbID]);
		mysql_tquery(dbHandle, query);
		return 1;
	}
	else if (CompareStrings(action, "criar"))
	{
		new info[153];

		if (sscanf(secoption,"s[153]", info))	return SendHelpMessage(playerid, "/apb criar [suspeito / descrição]");
        if (strfind(info, "/", true) == -1)		return SendErrorMessage(playerid, "Nenhum símbolo encontrado \"/\".");

		new apb = -1;

		for(new i = 0; i != MAX_APBS; i++)
		{
			if (ApbInfo[i][apbOn] != 0) continue;

			apb = i;
			break;
		}

		if (apb == -1)	return SendErrorMessage(playerid, "Não há espaço para uma nova nota de APB.");

		SendSyntaxMessage(playerid, "Você criou uma nota de APB [NUM: %i].", apb);
		strexplode(tmp, info, "/");
		format(ApbInfo[apb][apbOfficer], MAX_PLAYER_NAME, "%s", GetNameEx(playerid));
		format(ApbInfo[apb][apbSuspect], MAX_PLAYER_NAME, "%s", tmp[0]);
		format(ApbInfo[apb][apbCharge], 128, "%s", tmp[1]);
		format(ApbInfo[apb][apbDate], 32, "%s", GetFullDate());

		ApbInfo[apb][apbFaction] = faction;

		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s adicionou uma nova nota de APB, número %i.", GetRankName(playerid), GetNameEx(playerid), apb);
		
        new query[128 + 128];
		mysql_format(dbHandle,query, sizeof(query), "INSERT INTO `apbs` (faction, charge, suspect, officer, date) VALUES (%i, '%s', '%s', '%s', '%s')", faction, tmp[1], tmp[0], GetNameEx(playerid), GetFullDate());
		mysql_tquery(dbHandle, query, "ApbCreate", "i", apb);
		return 1;
	}
	return 1;
}

CMD:pedagio(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
		
	if (isnull(params))
	{
	    SendHelpMessage(playerid, "/pedagio [opções]");
	    SCM(playerid, COLOR_GRAD2, "close/abrir -Fecha / Abre todas as barreiras. ");
	    SCM(playerid, COLOR_GRAD2, "0 - Abre / fecha as barreiras nas autoestradas de Las Venturas. ");
	    SCM(playerid, COLOR_GRAD2, "1 - Blueberry (ponte direita) - abre / fecha barreiras na ponte direita em Blueberry. ");
	    SCM(playerid, COLOR_GRAD2, "2 - Blueberry (ponte esquerda) - abre / fecha barreiras na ponte esquerda em Blueberry. ");
		return 1;
	}
	
	if (L_a_RequestAllowedCop > gettime())	return SendErrorMessage(playerid, "Você tem que esperar %i segundos.", L_a_RequestAllowedCop - gettime());
	    
    if (CompareStrings(params, "abrir"))
	{
	    if (PlayerInfo[playerid][pRank] < 10)	return SendErrorMessage(playerid, "Esta função está disponível a partir da patente 10.");
		
	    for(new i; i != MAX_TOLLS; i++)
		{
			aTolls[i][E_tLocked] = 0;
		}
		
        SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s abriu todas as barreiras para entrada e saída. **", GetRankName(playerid), GetNameEx(playerid));
		L_a_RequestAllowedCop = gettime() + TOLL_DELAY_COP;
		return 1;
	}
	else if (CompareStrings(params, "fechar"))
	{
	    if (PlayerInfo[playerid][pRank] < 10)	return SendErrorMessage(playerid, "Esta função está disponível a partir da patente 10.");
			
		for(new i; i != MAX_TOLLS; i++)
		{
		    aTolls[i][E_tLocked] = 1;
		    Toll_CloseToll(i);
		}
		
		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s fechou todas as barreiras para entrada e saída. **", GetRankName(playerid), GetNameEx(playerid));
        L_a_RequestAllowedCop = gettime() + TOLL_DELAY_COP;
		return 1;
	}
	
	new id = strval(params);
	
	if (id < 0 || id > 4)				return SendHelpMessage(playerid, "/pedagio [número da barreira]");
    if (PlayerInfo[playerid][pRank] < 4)	return SendErrorMessage(playerid, "Esta função está disponível a partir da patente 2.");
		
    if (!IsPlayerInRangeOfPoint(playerid, TOLL_OPEN_DISTANCE, toll_gate_pos[id][0], toll_gate_pos[id][1], toll_gate_pos[id][2]) && !IsPlayerInRangeOfPoint(playerid, TOLL_OPEN_DISTANCE, toll_gate_pos[id+1][0], toll_gate_pos[id+1][1], toll_gate_pos[id+1][2]))
	{
		return SendErrorMessage(playerid, "Você deve estar perto destas barreiras.");
	}
	        
	new toll_name[3][35] = {"autoestradas de Las Venturas", "na ponte direita de Blueberry", "na ponte esquerda de Blueberry"};

	if (aTolls[id][E_tLocked] == 0)
	{
		aTolls[id][E_tLocked] = 1;
		Toll_CloseToll(id);
		aTolls[id+1][E_tLocked] = 1;
		Toll_CloseToll(id+1);
		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s fechou as barreiras para entrada e saída %s. **", GetRankName(playerid), GetNameEx(playerid), toll_name[id]);
	}
	else
	{
		aTolls[id][E_tLocked] = 0;
		aTolls[id+1][E_tLocked] = 0;
		SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s fechou as barreiras para entrada e saída %s. **", GetRankName(playerid), GetNameEx(playerid), toll_name[id]);
	}

	L_a_RequestAllowedCop = gettime() + TOLL_DELAY_COP;
	return 1;
}


ALTX:government("/gov");
CMD:government(playerid, params[])
{
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Seu chat local está bloqueado pelo sistema.");
	if (isnull(params))							return SendHelpMessage(playerid, "(/gov)ernment [mensagem]");

	new faction = PlayerInfo[playerid][pFaction], rank = PlayerInfo[playerid][pRank];

    if (!faction || IsFactionLegal(faction))						return SendErrorMessage(playerid, "Você não é um membro da estrutura de poder.");
	if (!isLeader(playerid) && rank != Factions[faction][fRanks]-1)	return SendErrorMessage(playerid, "Disponível apenas para o líder.");
	if (PlayerInfo[playerid][pSettings] & togGov)					return SendErrorMessage(playerid, "Comece mostrando as notícias do governo primeiro.");

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || (PlayerInfo[i][pSettings] & togGov))	continue;

		if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;

			if (pos < MAXLEN-1) pos = MAXLEN;

			SendClientMessageEx(i, COLOR_LSPD, "[Anúncio do Governo] %.*s ...", pos, params);
			SendClientMessageEx(i, COLOR_LSPD, "[Anúncio do Governo] ... %s", params[pos]);
		}
		else
		{
			SendClientMessageEx(i, COLOR_LSPD, "[Anúncio do Governo] %s", params);
		}
	}
	return 1;
}

CMD:duty(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!faction || !IsFactionLegal(faction))								return 1;
    if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))	return SendErrorMessage(playerid, "Você não está no vestiário / no veículo oficial.");

    Dialog_Show(playerid, DutyMenu, DIALOG_STYLE_LIST, "{FFFFFF}Duty", "- Duty\n- Skins\n- Equipamentos\n- Atualizar armadura\n- Atualizar saúde", "Selecionar", "Cancelar");
	return 1;
}

CMD:cornome(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial.");
    if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	if (PlayerInfo[playerid][pRank] < 5)	return SendErrorMessage(playerid, "Esta função não está disponível para você.");

    if (GetPlayerColor(playerid) != RemoveAlpha(Factions[faction][fColor]))
	{
		SetPlayerColor(playerid, RemoveAlpha(Factions[faction][fColor]));
        SendSyntaxMessage(playerid, "Você alterou a cor do seu nickname.");
	}
	else
	{
		SetPlayerColor(playerid, TEAM_HIT_COLOR);
        SendSyntaxMessage(playerid, "Você alterou a cor do seu nickname.");
	}
	return 1;
}


/*
CMD:lift(playerid, params[])
{
	if (!IsPlayerInRangeOfPoint(playerid, 2.0, 2268.0640,2448.2449,3.5313) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2781.0559,-1056.2823,1028.4969) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2874.6665,-940.6707,1035.9259) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2872.8813,-1072.9934,1037.1782)) return 1;

	Dialog_Show(playerid, LSPDElevator, DIALOG_STYLE_LIST, "{FFFFFF}Elevador", "[0] Garagem\n[1] Primeiro andar\n[2] Segundo andar\n[3] Sala de esportes", "Selecionar", "Cancelar");
	return 1;
}
*/

CMD:aceitarchamado(playerid, params[])
{
    new
		vehicle = GetPlayerVehicleID(playerid), 
		userid
	;

	if (!IsPlayerFactionPolice(playerid))												
		return SendErrorMessage(playerid, "Você deve ser um policial.");
	
	new id = PlayerInfo[playerid][pFaction];

	if (VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && !PlayerToPoint(30.0, playerid, Factions[id][fPosX], Factions[id][fPosY], Factions[id][fPosZ]))	
		return SendErrorMessage(playerid, "Disponível apenas na delegacia ou em veículos da LSPD / SASD.");
	if (sscanf(params, "i", userid))																										
		return SendHelpMessage(playerid, "/aceitarchamado [número da chamada]");
	if (!IsPlayerLogged(userid) || pTemp[userid][pCallOn] != 1)																											
		return SendErrorMessage(playerid, "Uma chamada com esse número não foi encontrada. ");

	if (pTemp[playerid][pPatrol])	SendFactionMessageF(FACTION_POLICE, COLOR_LSPD, "* %s %s [%s] aceitou uma chamada na linha direta. #%i.", GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], userid);
	else							SendFactionMessageF(FACTION_POLICE, COLOR_LSPD, "* %s %s aceitou uma chamada na linha direta. #%i.", GetRankName(playerid), GetNameEx(playerid), userid);	

	Waypoint_Set(playerid, pTemp[userid][pCallPosX], pTemp[userid][pCallPosY], pTemp[userid][pCallPosZ], 3.0);
	pTemp[userid][pCallOn] = 0;

	pTemp[playerid][pPoliceCalls]++;
	return 1;
}


ALTX:spike("/sp");
CMD:spike(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial.");
	if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
	
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/spike [criar / remover / removertodos]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		if (GetWorld(playerid) != 0 || GetInterior(playerid) != 0)	return SendErrorMessage(playerid, "Você deve estar ao ar livre.");
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)		return SendErrorMessage(playerid, "Você não deve estar em um veículo.");

		new Float:x, Float:y, Float:z, Float:a, location[MAX_ZONE_NAME];

		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, a);

		if (CreateStrip(x, y, z, a) == -1)	return SendErrorMessage(playerid, "O número máximo de espinhos foi atingido.");

		GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);

		SendFactionMessageF(FACTION_POLICE, COLOR_LSPD, "* HQ: %s %s colocou espinhos na estrada %s.", GetRankName(playerid), GetNameEx(playerid), location);
		return 1;
	}
	if (CompareStrings(option, "remover"))
	{
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)	return SendErrorMessage(playerid, "Você não deve estar em um veículo.");

		for(new i = 0; i < MAX_SPIKE; i++)
		{
			if (!SpikeInfo[i][sCreated] || IsPlayerInRangeOfPoint(playerid, 4.0, SpikeInfo[i][sX], SpikeInfo[i][sY], SpikeInfo[i][sZ])) continue;

			SpikeInfo[i][sCreated] = 0;
			SpikeInfo[i][sX] = 
			SpikeInfo[i][sY] = 
			SpikeInfo[i][sZ] = 0.0;
			
			if (IsValidDynamicObject(SpikeInfo[i][sObject]))
			{
				DestroyDynamicObject(SpikeInfo[i][sObject]);
				SpikeInfo[i][sObject] = INVALID_OBJECT_ID;	
			}

			SendSyntaxMessage(playerid, "Você removeu os espinhos [ID: %i].", i);
			break;
		}
		return 1;
	}

	else if (CompareStrings(option, "removertodos"))
	{
		for(new i = 0; i < MAX_SPIKE; i++)
		{
			if (!SpikeInfo[i][sCreated]) continue;

			SpikeInfo[i][sCreated] = 0;
			SpikeInfo[i][sX] = 
			SpikeInfo[i][sY] = 
			SpikeInfo[i][sZ] = 0.0;

			if (IsValidDynamicObject(SpikeInfo[i][sObject]))
			{
				DestroyDynamicObject(SpikeInfo[i][sObject]);
				SpikeInfo[i][sObject] = INVALID_OBJECT_ID;
			}
		}

		SendFactionMessageF(FACTION_POLICE, COLOR_LSPD, "* HQ: Todos os espinhos nas estradas foram removidos por %s %s", GetRankName(playerid), GetNameEx(playerid));
		return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "/spike [criar / remover / removertodos]");
	}
	return 1;
}


CMD:prisioneiros(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))	return SendErrorMessage(playerid,"Você deve ser um policial / agente prisional.");
	
	if (IsFactionPolice(faction))
	{
		SendClientMessage(playerid, COLOR_GREEN, "Lista de prisioneiros na delegacia:");

		foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || PlayerInfo[i][pJailed] != PALOMINO_JAIL) continue; 

			SendClientMessageEx(playerid, COLOR_GRAD2, "%s | Tempo de confinamento: %s seg.", GetNameEx(i), FormatNumber(PlayerInfo[i][pJailTime]));
		}
		return 1;
	}
	return 1;
}

ALTX:repaircam("/rcam","/repararcamera");
CMD:repaircam(playerid)
{
	new faction = PlayerInfo[playerid][pFaction]; 

	if (!IsFactionPolice(faction)) 				return SendErrorMessage(playerid, "Você deve ser um policial."); 
	if (!IsPlayerHasItem(playerid, INV_TOOLS)) 	return SendErrorMessage(playerid, "Você não tem ferramentas.");

	new camid = GetNearestCamera(playerid, 10.0); 

	if (camid == -1) 				return SendErrorMessage(playerid, "Você deve estar perto da câmera de CCTV.");
	if (!CCTVInfo[camid][tvBreak]) 	return SendErrorMessage(playerid, "Esta câmera não precisa de conserto.");

	SetCameraBreakState(camid, 0); 
	SendSyntaxMessage(playerid, "Você consertou a câmera de CCTV [ID: %i | %s].", camid, CCTVInfo[camid][tvName]);
	return 1; 
}
