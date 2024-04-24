ALTX:enter("/entrar");
CMD:enter(playerid, params[])
{
    if (pTemp[playerid][pZond] != 0) return 1;

	new id = -1;

	if (IsDriver(playerid) && (id = GetNearEntranceOutside(playerid, 3.0)) != -1)
	{
	    if (EntranceInfo[id][eLocked] == 1)					return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);
        if (EntranceInfo[id][eVehicle] != 1)					return 1;
	    if (!GetWorld(playerid) && !GetInterior(playerid))	GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

        SetPlayerPos(playerid, EntranceInfo[id][eExitX], EntranceInfo[id][eExitY], EntranceInfo[id][eExitZ]);
		SetInterior(playerid, EntranceInfo[id][eExitInt]);
		SetWorld(playerid, EntranceInfo[id][eExitVW]);

        new vehicleid = GetPlayerVehicleID(playerid);

        foreach (new i : Player)
		{
		    if (playerid == i || !IsPlayerInVehicle(i, vehicleid)) continue;

            SetPlayerPos(i, EntranceInfo[id][eExitX], EntranceInfo[id][eExitY], EntranceInfo[id][eExitZ]);
			SetInterior(i, EntranceInfo[id][eExitInt]);
			SetWorld(i, EntranceInfo[id][eExitVW]);
			//PlayerEnterFreeze(playerid);
			PlayerInfo[i][pLocationPos][0] = PlayerInfo[playerid][pLocationPos][0];
			PlayerInfo[i][pLocationPos][1] = PlayerInfo[playerid][pLocationPos][1];
			PlayerInfo[i][pLocationPos][2] = PlayerInfo[playerid][pLocationPos][2];
			PlayerPlaySound(i, 1, 0.0, 0.0, 0.0);
		}

        SetVehiclePos(GetPlayerVehicleID(playerid), EntranceInfo[id][eExitX], EntranceInfo[id][eExitY], EntranceInfo[id][eExitZ]);
		SetVehicleZAngle(GetPlayerVehicleID(playerid), EntranceInfo[id][eExitA] - 180.0);
		LinkVehicleToInterior(GetPlayerVehicleID(playerid), EntranceInfo[id][eExitInt]);
		SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), EntranceInfo[id][eExitVW]);

		PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
	    return 1;
	}

    if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearEntranceOutside(playerid)) != -1)
	{
	    if (EntranceInfo[id][eLocked] == 1)					return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);
	    if (!GetWorld(playerid) && !GetInterior(playerid))	GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid, EntranceInfo[id][eExitX], EntranceInfo[id][eExitY], EntranceInfo[id][eExitZ]);
		SetPlayerFacingAngle(playerid, EntranceInfo[id][eExitA] - 180.0);
		SetInterior(playerid, EntranceInfo[id][eExitInt]);
		SetWorld(playerid, EntranceInfo[id][eExitVW]);
		//PlayerEnterFreeze(playerid);

		PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
	    return 1;
	}

    if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearApartmentOutside(playerid)) != -1)
	{
	    if (!GetWorld(playerid) && !GetInterior(playerid))	GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid, ApartInfo[id][aExitX], ApartInfo[id][aExitY], ApartInfo[id][aExitZ]);
		SetWorld(playerid, ApartInfo[id][aExitVW]);
		SetInterior(playerid, ApartInfo[id][aExitInt]);
		//PlayerEnterFreeze(playerid);
		pTemp[playerid][pApartEntered] = id;

		PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
		return 1;
	}

	if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearOutsideHouse(playerid)) != -1)
	{
        if (!HouseInfo[id][hOwnerID]) return SendSyntaxMessage(playerid, "Use /checkint para verificar o interior da casa.");
	    if (HouseInfo[id][hLocked])		return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);

        if (!GetWorld(playerid) && !GetInterior(playerid))	GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid,HouseInfo[id][hExitX],HouseInfo[id][hExitY],HouseInfo[id][hExitZ]);
		SetInterior(playerid,HouseInfo[id][hExitInt]);
		SetWorld(playerid,HouseInfo[id][hExitVW]);
		//PlayerEnterFreeze(playerid);
		pTemp[playerid][pHouseEntered] = id;

		if (PlayerInfo[playerid][pRentKey] == HouseInfo[id][hID] || HouseInfo[id][hOwnerID] == PlayerInfo[playerid][pID])	GameTextForPlayer(playerid, "~w~BEM-VINDO~n~~y~A CASA", 4000, 1);

		if (HouseInfo[id][hRadioOn] == 1)
		{
		    pTemp[playerid][pBoombox_house] = id;
			PlayAudioStreamForPlayer(playerid, HouseInfo[id][hRadioURL]);
		}

		PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
		return 1;
	}

	if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearOutsideExtHouse(playerid)) != -1)
	{
		if (!HouseInfo[id][hExtX][1] && !HouseInfo[id][hExtX][1] && !HouseInfo[id][hExtX][1]) return 1;
	    if (HouseInfo[id][hLocked])		return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);

        if (!GetWorld(playerid) && !GetInterior(playerid))	GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid,HouseInfo[id][hExtX][1],HouseInfo[id][hExtY][1],HouseInfo[id][hExtZ][1]);
		SetInterior(playerid,HouseInfo[id][hExtInt][1]);
		SetWorld(playerid,HouseInfo[id][hExtVW][1]);
		//PlayerEnterFreeze(playerid);
		pTemp[playerid][pHouseEntered] = id;

		if (PlayerInfo[playerid][pRentKey] == HouseInfo[id][hID] || HouseInfo[id][hOwnerID] == PlayerInfo[playerid][pID])	GameTextForPlayer(playerid, "~w~BEM-VINDO~n~~y~A CASA", 4000, 1); //î

		if (HouseInfo[id][hRadioOn] == 1)
		{
		    pTemp[playerid][pBoombox_house] = id;
			PlayAudioStreamForPlayer(playerid, HouseInfo[id][hRadioURL]);
		}

		PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
		return 1;
	}

	for(new i = 0; i < MAX_BIZ; i++)
	{
	    if (IsPlayerBizOutside(playerid, i, 5.0) && BizInfo[i][bizType] == BIZ_TUNING && IsDriver(playerid)) return ShowModMenu(playerid);

		if (IsPlayerBizOutside(playerid, i, 5.0) && BizInfo[i][bizType] == BIZ_PAYNSPRAY)
		{
		    if (BizInfo[i][bizLocked]) return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);
		    if (!GetPlayerVehicleID(playerid)) return SendErrorMessage(playerid, "Você deve estar em um veículo.");
		    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "Você deve estar no controle de um veículo.");
		    if (VehicleInfo[GetPlayerVehicleID(playerid)][carEngineOn]) return SendErrorMessage(playerid, "Desligue o motor antes de iniciar o reparo.");
		    if (GetCash(playerid) < BizInfo[i][bizEntrance]) return SendErrorMessage(playerid, "O reparo custa $%i.", BizInfo[i][bizEntrance]);
		    if (BizInfo[i][bizOwnerID] && !BizInfo[i][bizProduct]) return SendErrorMessage(playerid, "Não há produtos suficientes no negócio.");

		    new Float: health;
		    AntiCheatGetVehicleHealth(GetPlayerVehicleID(playerid), health);

		    if (health < 230.0) return SendErrorMessage(playerid, "Este veículo não pode mais ser reparado.");

		    static const dlgstr[] = "{FFFFFF}Seu veículo está danificado em {FF6347}%i%%\n\
		                            {FFFFFF}Renovação completa: {1b8922}$%i {C5C5C5}(( Danos externos + HP ))\n\
		                            {FFFFFF}Selecione itens para reparo\n\
		                            {FFFFFF}Restaurar condição geral: {1b8922}$%i {C5C5C5}(( HP ))\n\
		                            {FFFFFF}Substituir bateria [ condição: {FF6347}%.2f%%{FFFFFF}]: {1b8922}$%i\n\
		                            {FFFFFF}Substituir motor [ condição: {FF6347}%.2f%%{FFFFFF}]: {1b8922}$%i";
		    new mes[sizeof(dlgstr) + 64];

		    new panels, doors, lights, tires;
		    GetVehicleStatus(playerid, panels, doors, lights, tires);

		    new
		        vehicle = GetPlayerVehicleID(playerid),
		        model = GetVehicleModel(vehicle),
		        common_per = floatround((100.0 - ((health/GetVehicleMaxHealth(model))*100) + (panels+doors+lights+tires)) / 5, floatround_ceil), //%
		        hp_cost = floatround((100 - ((health/GetVehicleMaxHealth(model))*100)) * (GetVehiclePrice(model)/1000), floatround_ceil)
		    ;
		    if (hp_cost < 0) hp_cost = 0;
		    new
		        cost = floatround(GetVehiclePrice(model) / 4, floatround_ceil),
		        common_cost = hp_cost +  floatround((cost * 0.5 * panels/100) + (cost * 0.25 * doors/100) + (cost * 0.1 * lights/100) + (cost * 0.15 * tires/100), floatround_ceil)
		    ;

		    format(mes, sizeof(mes), dlgstr,
		        common_per,
		        common_cost,
		        hp_cost,
		        VehicleInfo[vehicle][carBattery], floatround(GetVehiclePrice(model) / 100 * 5, floatround_ceil),
		        VehicleInfo[vehicle][carEngine], floatround(GetVehiclePrice(model) / 100 * 10, floatround_ceil)
		    );

		    Dialog_Show(playerid, Autoservice, DIALOG_STYLE_TABLIST_HEADERS, "Reparo de veículo", mes, "Selecionar", "Cancelar");
		    return 1;
		}

		if (IsPlayerBizOutside(playerid, i, 1.0))
		{
			GameTextForPlayer(playerid, "_", 1, 4);
			if (BizInfo[i][bizType] == BIZ_TUNING) 			return 1;
			else if (BizInfo[i][bizType] == BIZ_AUTOSALON) return Dialog_Show(playerid, SelectClass, DIALOG_STYLE_LIST, "Selecione uma categoria", "Veículos 2-portas e Compactos\n\
			    Lowriders\n\
			    Veículos 4-portas e Carros de Luxo\n\
			    Esportivos\n\
			    Pickups e Vans\n\
			    SUVs e Perueiros\n\
			    Caminhões e Veículos de Trabalho\n\
			    Motocicletas / Bicicletas\n\
			    Outros\n\
			    Veículos Premium ",
			    "Selecionar", "Fechar");
			else if (BizInfo[i][bizType] == BIZ_BOATSALON) return ShowBoats(playerid);
			else if (BizInfo[i][bizType] == BIZ_FLYSALON) return ShowFly(playerid);
			else if (BizInfo[i][bizType] == BIZ_GAS_STATION) return SendHelpMessage(playerid, "/abastecer - iniciar o abastecimento / parar o abastecimento.");
			else if (BizInfo[i][bizType] == BIZ_ADVERT) return SendHelpMessage(playerid, "/anunciar - anúncio, /canuncio - anúncio para a empresa. /alugaroutdoor - aluguel de um painel de anúncio.");

            if (IsPlayerInAnyVehicle(playerid))																																return 1;
			if (BizInfo[i][bizLocked] == 1)																																	return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);
			if (PlayerInfo[playerid][pID] != BizInfo[i][bizOwnerID] && PlayerInfo[playerid][pWorkOn] != BizInfo[i][bizID] && GetCash(playerid) < BizInfo[i][bizEntrance])	return GameTextForPlayer(playerid, "~r~FALTA DE DINHEIRO!", 5000, 1);
            
			if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

			SetPlayerPos(playerid,BizInfo[i][bizExitX], BizInfo[i][bizExitY], BizInfo[i][bizExitZ]);
			SetInterior(playerid,BizInfo[i][bizExitInt]);
			SetWorld(playerid,BizInfo[i][bizExitVW]);

			if (strlen(BizInfo[i][bizMes]) > 1) SCM(playerid, COLOR_WHITE, BizInfo[i][bizMes]);

			if (PlayerInfo[playerid][pID] != BizInfo[i][bizOwnerID] && PlayerInfo[playerid][pWorkOn] != BizInfo[i][bizID] && !BizInfo[i][bizEntrance])
			{
				GiveCash(playerid, -BizInfo[i][bizEntrance]);
				GiveBizCash(i, BizInfo[i][bizEntrance]);
			}

			pTemp[playerid][pBizEntered] = i;
			GameTextForPlayer(playerid, BizInfo[i][bizName], 5000, 1);
			
			if (BizInfo[i][bizRadioOn] == 1)
			{
			    pTemp[playerid][pBoombox_biz] = i;
				PlayAudioStreamForPlayer(playerid, BizInfo[i][bizRadioURL]);
			}
			return 1;
		}
	}

	if ((id = GetNearGarage(playerid)) != -1)
	{
		if (GarageInfo[id][garageOwner] == 0 && IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Esta garagem não é acessível para veículos.");
		if (GarageInfo[id][garageLocked] != 0) return SendErrorMessage(playerid, "As portas da garagem estão fechadas.");

		if (IsDriver(playerid))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
            if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

			PlayerInfo[playerid][pLocationPos][0] = GarageInfo[id][garageVeh_Enter][0];
			PlayerInfo[playerid][pLocationPos][1] = GarageInfo[id][garageVeh_Enter][1];
			PlayerInfo[playerid][pLocationPos][2] = GarageInfo[id][garageVeh_Enter][2];

			foreach (new i : Player)
			{
			    if (playerid == i || !IsPlayerInVehicle(i, vehicleid)) continue;

				SetInterior(i, GarageInfo[id][garageExitInt]);
				SetWorld(i, GarageInfo[id][garageExitVW]);
				PlayerInfo[i][pLocationPos][0] = PlayerInfo[playerid][pLocationPos][0];
				PlayerInfo[i][pLocationPos][1] = PlayerInfo[playerid][pLocationPos][1];
				PlayerInfo[i][pLocationPos][2] = PlayerInfo[playerid][pLocationPos][2];

				PlayerPlaySound(i, 1, 0.0, 0.0, 0.0);
			}
			
			SetInterior(playerid, GarageInfo[id][garageExitInt]);
			SetWorld(playerid, GarageInfo[id][garageExitVW]);

			SetVehiclePos(vehicleid, GarageInfo[id][garageVeh_Exit][0], GarageInfo[id][garageVeh_Exit][1], GarageInfo[id][garageVeh_Exit][2]);
		    SetVehicleZAngle(vehicleid, GarageInfo[id][garageVeh_Exit][3]);
		    SetVehicleVirtualWorld(vehicleid, GarageInfo[id][garageExitVW]);
			LinkVehicleToInterior(vehicleid, GarageInfo[id][garageExitInt]);

			PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
		    return 1;
		}
		else if (!IsPlayerInAnyVehicle(playerid))
		{
		    if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

			SetPlayerPos(playerid, GarageInfo[id][garagePlayer_Exit][0], GarageInfo[id][garagePlayer_Exit][1], GarageInfo[id][garagePlayer_Exit][2]);
		    SetInterior(playerid, GarageInfo[id][garageExitInt]);
			SetWorld(playerid, GarageInfo[id][garageExitVW]);

			PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
		}
	    return 1;
	}
	return 1;
}
ALTX:exit("/sair");
CMD:exit(playerid, params[])
{
    if (pTemp[playerid][pZond] != 0) return 1;

	new id = -1;

	if (IsDriver(playerid) && (id = GetNearEntranceInside(playerid, 3.0)) != -1)
	{
	    if (EntranceInfo[id][eLocked] == 1)	return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);
        if (EntranceInfo[id][eVehicle] != 1)	return 1;

	    if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

        SetPlayerPos(playerid, EntranceInfo[id][eEnterX], EntranceInfo[id][eEnterY], EntranceInfo[id][eEnterZ]);
		SetInterior(playerid, EntranceInfo[id][eEnterInt]);
		SetWorld(playerid, EntranceInfo[id][eEnterVW]);

		new vehicleid = GetPlayerVehicleID(playerid);

        foreach (new i : Player)
		{
		    if (playerid == i || !IsPlayerInVehicle(i, vehicleid)) continue;

            SetPlayerPos(i, EntranceInfo[id][eEnterX], EntranceInfo[id][eEnterY], EntranceInfo[id][eEnterZ]);
			SetInterior(i, EntranceInfo[id][eEnterVW]);
			SetWorld(i, EntranceInfo[id][eEnterVW]);
			PlayerInfo[i][pLocationPos][0] = PlayerInfo[playerid][pLocationPos][0];
			PlayerInfo[i][pLocationPos][1] = PlayerInfo[playerid][pLocationPos][1];
			PlayerInfo[i][pLocationPos][2] = PlayerInfo[playerid][pLocationPos][2];
			
			PlayerPlaySound(i, 0, 0.0, 0.0, 0.0);
		}
		
		SetVehiclePos(GetPlayerVehicleID(playerid), EntranceInfo[id][eEnterX], EntranceInfo[id][eEnterY], EntranceInfo[id][eEnterZ]);
		SetVehicleZAngle(GetPlayerVehicleID(playerid), EntranceInfo[id][eEnterA] - 180.0);
		LinkVehicleToInterior(GetPlayerVehicleID(playerid), EntranceInfo[id][eEnterInt]);
		SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), EntranceInfo[id][eEnterVW]);

		PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
	    return 1;
	}

	if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearEntranceInside(playerid)) != -1)
	{
	    if (EntranceInfo[id][eLocked] == 1)	return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);

	    if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid, EntranceInfo[id][eEnterX], EntranceInfo[id][eEnterY], EntranceInfo[id][eEnterZ]);
		SetPlayerFacingAngle(playerid, EntranceInfo[id][eEnterA] - 180.0);
		SetInterior(playerid, EntranceInfo[id][eEnterInt]);
		SetWorld(playerid, EntranceInfo[id][eEnterVW]);

		PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
	    return 1;
	}

	if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearApartmentInside(playerid, 1.0)) != -1)
	{
	    if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid, ApartInfo[id][aPosX], ApartInfo[id][aPosY], ApartInfo[id][aPosZ]);
		SetWorld(playerid, ApartInfo[id][aPosVW]);
		SetInterior(playerid, ApartInfo[id][aPosInt]);
		pTemp[playerid][pApartEntered] = -1;

		PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
		return 1;
	}

	if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearInsideHouse(playerid, 2.0)) != -1)
	{
        if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid,HouseInfo[id][hPosX],HouseInfo[id][hPosY],HouseInfo[id][hPosZ]);
		SetInterior(playerid, HouseInfo[id][hPosInt]);
		SetWorld(playerid,HouseInfo[id][hPosVW]);
        pTemp[playerid][pHouseEntered] = -1;
        DeletePVar(playerid, #check_int);

		PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
	    return 1;
	}
	else if (!IsPlayerInAnyVehicle(playerid) && (id = GetNearInsideExtHouse(playerid, 2.0)) != -1)
	{
		if (!HouseInfo[id][hExtX][0] && !HouseInfo[id][hExtX][0] && !HouseInfo[id][hExtX][0]) return 1;
		if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);
		
		SetPlayerPos(playerid,HouseInfo[id][hExtX][0],HouseInfo[id][hExtY][0],HouseInfo[id][hExtZ][0]);
		SetInterior(playerid, HouseInfo[id][hExtInt][0]);
		SetWorld(playerid,HouseInfo[id][hExtVW][0]);
        pTemp[playerid][pHouseEntered] = -1;
        DeletePVar(playerid, #check_int);

		PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
	    return 1;
	}
	else if ((id = GetNearInsideBiz(playerid, 2.0)) != -1)
	{
	    if (BizInfo[id][bizType] == BIZ_GAS_STATION || BizInfo[id][bizType] == BIZ_AUTOSALON || BizInfo[id][bizType] == BIZ_BOATSALON || BizInfo[id][bizType] == BIZ_FLYSALON || BizInfo[id][bizType] == BIZ_TUNING || BizInfo[id][bizType] == BIZ_ADVERT || BizInfo[id][bizType] == BIZ_LICENSE)
		{
			return 1;
		}

        if (!GetWorld(playerid) && !GetInterior(playerid))	GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

		SetPlayerPos(playerid,BizInfo[id][bizPosX], BizInfo[id][bizPosY], BizInfo[id][bizPosZ]);
		SetInterior(playerid,BizInfo[id][bizPosInt]);
		SetWorld(playerid,BizInfo[id][bizPosVW]);
		pTemp[playerid][pBizEntered] = -1;

		PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
		return 1;
	}
	else if ((id = GetNearGarage(playerid)) != -1)
	{
	    if (GarageInfo[id][garageLocked] != 0) return SendErrorMessage(playerid, "As portas da garagem estão fechadas.");

		if (IsDriver(playerid))
		{
		    if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);
			new vehicleid = GetPlayerVehicleID(playerid);

		    PlayerInfo[playerid][pLocationPos][0] = GarageInfo[id][garageVeh_Enter][0];
			PlayerInfo[playerid][pLocationPos][1] = GarageInfo[id][garageVeh_Enter][1];
			PlayerInfo[playerid][pLocationPos][2] = GarageInfo[id][garageVeh_Enter][2];

			foreach (new i : Player)
			{
			    if (playerid == i || !IsPlayerInVehicle(i, vehicleid)) continue;

				SetInterior(i, GarageInfo[id][garageEnterInt]);
				SetWorld(i, GarageInfo[id][garageEnterVW]);
                PlayerInfo[i][pLocationPos][0] = PlayerInfo[playerid][pLocationPos][0];
				PlayerInfo[i][pLocationPos][1] = PlayerInfo[playerid][pLocationPos][1];
				PlayerInfo[i][pLocationPos][2] = PlayerInfo[playerid][pLocationPos][2];

				PlayerPlaySound(i, 0, 0.0, 0.0, 0.0);
			}
			
			SetInterior(playerid, GarageInfo[id][garageEnterInt]);
			SetWorld(playerid, GarageInfo[id][garageEnterVW]);

			SetVehiclePos(vehicleid, GarageInfo[id][garageVeh_Enter][0], GarageInfo[id][garageVeh_Enter][1], GarageInfo[id][garageVeh_Enter][2]);
		    SetVehicleZAngle(vehicleid, GarageInfo[id][garageVeh_Enter][3]);
		    SetVehicleVirtualWorld(vehicleid, GarageInfo[id][garageEnterVW]);
			LinkVehicleToInterior(vehicleid, GarageInfo[id][garageEnterInt]);

			PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
		    return 1;
		}
		else if (!IsPlayerInAnyVehicle(playerid))
		{
		    if (!GetWorld(playerid) && !GetInterior(playerid)) GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

			SetPlayerPos(playerid, GarageInfo[id][garagePlayer_Enter][0], GarageInfo[id][garagePlayer_Enter][1], GarageInfo[id][garagePlayer_Enter][2]);
		    SetInterior(playerid, GarageInfo[id][garageEnterInt]);
			SetWorld(playerid, GarageInfo[id][garageEnterVW]);

			PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
		}
	    return 1;
	}
	return 1;
}