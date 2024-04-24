
GetPlayerAcceptAction(playerid)
{
	if (GetPVarInt(playerid, #accept_time) < gettime()) return 0;

	if (pTemp[playerid][pGreetPlayer] != INVALID_PLAYER_ID)
	{
	    new userid = pTemp[playerid][pGreetPlayer], type = pTemp[playerid][pGreetType];

	    pTemp[playerid][pGreetPlayer] = INVALID_PLAYER_ID;
		pTemp[playerid][pGreetType] = 0;

		if (IsPlayerLogged(userid) == 0) return SendErrorMessage(playerid, "O jogador que lhe ofereceu uma saudação não está online.");
		if (!PlayerNearPlayer(1.0, playerid, userid))	return SendNotNearPlayer(playerid);

		StopPlayerAnims(playerid);
		StopPlayerAnims(userid);
		SetPlayerFacePlayer(playerid, userid);
		SetPlayerFacePlayer(userid, playerid);
		
		switch(type)
		{
			case 1:
			{
				ApplyAnimation(playerid, "KISSING", "Playa_Kiss_02", 2.0, 0, 0, 1, 0, 0, 1);
				ApplyAnimation(userid, "KISSING", "Playa_Kiss_02", 2.0, 0, 0, 1, 0, 0, 1);
			}
			case 2:
			{
				ApplyAnimation(playerid, "GANGS", "hndshkfa_swt", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "hndshkfa_swt", 2.0, 0, 0, 0, 0, 0, 1);
			}
			case 3:
			{
				ApplyAnimation(playerid, "GANGS", "hndshkba", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "hndshkba", 2.0, 0, 0, 0, 0, 0, 1);
			}
			case 4:
			{
				ApplyAnimation(playerid, "GANGS", "hndshkca", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "hndshkca", 2.0, 0, 0, 0, 0, 0, 1);
			}
			case 5:
			{
				ApplyAnimation(playerid, "GANGS", "hndshkcb", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "hndshkcb", 2.0, 0, 0, 0, 0, 0, 1);
			}
			case 6:
			{
				ApplyAnimation(playerid, "GANGS", "hndshkda", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "hndshkda", 2.0, 0, 0, 0, 0, 0, 1);
			}
			case 7:
			{
				ApplyAnimation(playerid, "GANGS", "hndshkda", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "hndshkda", 2.0, 0, 0, 0, 0, 0, 1);
			}
			case 8:
			{
				ApplyAnimation(playerid, "GANGS", "hndshkaa", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "hndshkaa", 2.0, 0, 0, 0, 0, 0, 1);
			}
			case 9:
			{
				ApplyAnimation(playerid, "GANGS", "prtial_hndshk_biz_01", 2.0, 0, 0, 0, 0, 0, 1);
				ApplyAnimation(userid, "GANGS", "prtial_hndshk_biz_01", 2.0, 0, 0, 0, 0, 0, 1);
			}
		}
		return 1;
	}

    else if (pTemp[playerid][pCarPlayer] != INVALID_PLAYER_ID)
	{
  		new query[128];
    	mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `cars` WHERE `owner` = %i", PlayerInfo[playerid][pID]);
		mysql_tquery(dbHandle, query, "BuyVehicleAtPlayer", "i", playerid);
   		return 1;
	}

	else if (pTemp[playerid][pHealPlayer] != INVALID_PLAYER_ID)
	{
	    new userid = pTemp[playerid][pHealPlayer];
	    pTemp[playerid][pHealPlayer] = INVALID_PLAYER_ID;

	    if (!IsPlayerLogged(userid))                return SendErrorMessage(playerid, "O jogador que lhe ofereceu o tratamento não está online.");
	    if (PlayerInfo[playerid][pInjured] != 0)    return SendErrorMessage(playerid, "Você está na fase de ferimento/morte.");

	    SendSyntaxMessage(playerid, "Você foi curado por %s.", GetNameWithMask(userid));
	    SendSyntaxMessage(userid, "Você curou %s.", GetNameWithMask(playerid));
	    SetPlayerHealth(playerid, 100.0);
	    return 1;
	}
	else if (pTemp[playerid][pWepPlayer] != INVALID_PLAYER_ID)
	{
	    new userid = pTemp[playerid][pWepPlayer],
	        price = pTemp[playerid][pWepPrice];

	    pTemp[playerid][pWepPlayer] = INVALID_PLAYER_ID;

	    if (!IsPlayerLogged(userid))        return SendErrorMessage(playerid, "O jogador que lhe ofereceu uma licença de arma não está online.");
	    if (GetCash(playerid) < price)    return SendNotCash(playerid);

	    GiveCash(playerid, -price);
	    PlayerInfo[playerid][pWepLic] = 1;
	    PlayerInfo[playerid][pWepWarns] = 0;
	    PlayerInfo[playerid][pWepCount] = 3;

	    new query[168];
	    mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `wep_count`='3',`weplic`='1',`wepwarns`='0',`wep_officer`='%s',`wep_data`='%s',`wep_price`=%i WHERE `id` = %i", GetNameEx(userid), GetFullDate(), price, PlayerInfo[playerid][pID]);
	    mysql_tquery(dbHandle, query);

	    SendSyntaxMessage(userid, "%s comprou uma licença de arma de você por $%i.", GetNameEx(playerid), price);
	    SendSyntaxMessage(playerid, "Você comprou uma licença de arma de %s por $%i.", GetNameEx(userid), price);
	    return 1;
	}

	else if (pTemp[playerid][pHirePlayer] != INVALID_PLAYER_ID)
	{
	    new userid = pTemp[playerid][pHirePlayer];

	    pTemp[playerid][pHirePlayer] = INVALID_PLAYER_ID;

	    if (!IsPlayerLogged(userid))                    return SendErrorMessage(playerid, "O jogador que lhe ofereceu um emprego no negócio não está online.");
	    if (!PlayerNearPlayer(3.0, playerid, userid))    return SendNotNearPlayer(playerid);

	    new bizid;

	    if ((bizid = GetNearOutsideBiz(playerid)) == -1)            return SendErrorMessage(playerid, "Não há nenhum negócio perto de você.");
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[userid][pID])    return SendErrorMessage(playerid, "Você deve estar perto do negócio do empregador.");

	    PlayerInfo[playerid][pWorkOn] = BizInfo[bizid][bizID];
	    SQL_SetInteger("users", "workon", BizInfo[bizid][bizID], PlayerInfo[playerid][pID]);

	    new j = WorkSpace(bizid);
	    BizWorkers[bizid][j][bizJobOn] = PlayerInfo[playerid][pID];
	    format(BizWorkers[bizid][j][bizWorker], MAX_PLAYER_NAME, "%s", GetAccName(playerid));
	    format(BizWorkers[bizid][j][bizPosition], MAX_PLAYER_NAME, "Sem posição");

	    SendSyntaxMessage(playerid, "Você aceitou uma oferta de emprego.");
	    SendSyntaxMessage(userid, "%s aceitou sua oferta de emprego.", GetNameEx(playerid));
	    return 1;
	}

	else if (pTemp[playerid][pFactionPlayer] != INVALID_PLAYER_ID)
	{
	    new targetid = pTemp[playerid][pFactionPlayer];

	    pTemp[playerid][pFactionPlayer] = INVALID_PLAYER_ID;

	    if (!IsPlayerLogged(targetid))                                return SendErrorMessage(playerid, "O jogador que o convidou para se juntar à facção não está online.");
	    if (!isLeader(targetid) || !GetInviteRight(targetid))        return 1;
	    if (!Factions[PlayerInfo[targetid][pFaction]][fJoinRank])    return SendErrorMessage(playerid, "Entrada proibida. Os ranks e salários na estrutura não estão configurados.");

	    PlayerInfo[playerid][pFaction] = PlayerInfo[targetid][pFaction];
	    PlayerInfo[playerid][pRank] = Factions[PlayerInfo[targetid][pFaction]][fJoinRank];
	    PlayerInfo[playerid][pBadgeNumber] = randomEx(23465, 99999);

	    SendSyntaxMessage(playerid, "Você se juntou a %s.", Factions[PlayerInfo[targetid][pFaction]][fName]);
	    SendFactionMessageF(PlayerInfo[targetid][pFaction], COLOR_LIGHTBLUE, "[Facção] %s aceitou o convite de %s e entrou na facção.", GetNameEx(playerid), GetNameEx(targetid));
	    return 1;
	}

	else if (RefillOffer[playerid] != INVALID_PLAYER_ID)
	{
	    new userid = RefillOffer[playerid], price = RefillPrice[playerid], vehicle = GetPlayerVehicleID(playerid);

	    if (!IsPlayerLogged(userid)) return SendErrorMessage(playerid, "O jogador que ofereceu um posto de gasolina para você não está online.");

	    RefillPrice[playerid] = 0;
	    RefillOffer[playerid] = INVALID_PLAYER_ID;

	    if (!IsDriver(playerid)) return SendErrorMessage(playerid, "Você deve estar no veículo como motorista.");
	    if (!PlayerNearPlayer(6.0, playerid, userid)) return SendNotNearPlayer(playerid);
	    if (GetCash(playerid) < price) return SendNotCash(playerid);

	    GiveCash(playerid, -price);
	    GiveCash(userid, price);
	    VehicleInfo[vehicle][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(vehicle));

	    new mes[128];
	    format(mes, sizeof(mes), "aceita um pedido para abastecer veículos de %s por $%i.", GetNameWithMask(userid), price);
	    cmd::me(playerid, mes);

	    return 1;
	}
	else if (pTemp[playerid][pServePlayer] != INVALID_PLAYER_ID && pTemp[playerid][pServeType] == 5) // Reboque
	{
	    new mechanicid = pTemp[playerid][pServePlayer], vehicle = GetPVarInt(playerid, #tow_car);

	    pTemp[playerid][pServePlayer] = INVALID_PLAYER_ID;
	    pTemp[playerid][pServeType] = 0;

	    if (!IsPlayerLogged(mechanicid)) return SendErrorMessage(playerid, "O jogador que ofereceu o serviço de reboque não está online.");
	    if (PlayerInfo[mechanicid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Este jogador não é um mecânico.");
	    if (!IsDriver(mechanicid) || !IsATowTruck(GetPlayerVehicleID(mechanicid))) return SendErrorMessage(playerid, "O mecânico deve estar no caminhão de reboque como motorista.");
	    if (!PlayerNearPlayer(7.0, playerid, mechanicid)) return SendNotNearPlayer(playerid);
	    if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você não deve estar em nenhum veículo.");
	    if (GetNearVehicle(mechanicid, 7.0, vehicle) == 0) return SendErrorMessage(playerid, "Seu veículo deve estar próximo para ser rebocado.");
	    if (IsABoat(vehicle) || IsAPlane(vehicle) || IsAHelicopter(vehicle) || IsABiker(vehicle)) return SendErrorMessage(playerid, "Este veículo não pode ser rebocado.");

	    new mes[128];
	    format(mes, sizeof(mes), "tomou ação de %s e liberou o gancho de reboque em %s.", GetNameWithMask(mechanicid), VehicleNames[GetVehicleModel(vehicle) - 400]);
	    cmd::me(playerid, mes);

	    AttachTrailerToVehicle(vehicle, GetPlayerVehicleID(mechanicid));
	    return 1;
	}

	else if (pTemp[playerid][pServePlayer] != INVALID_PLAYER_ID && (pTemp[playerid][pServeType] > 0 && pTemp[playerid][pServeType] < 5))
	{
	    new vehicle = GetPlayerVehicleID(playerid), mechanicid = pTemp[playerid][pServePlayer], type = pTemp[playerid][pServeType], vehid = GetPlayerVehicleID(mechanicid);

	    pTemp[playerid][pServePlayer] = INVALID_PLAYER_ID;
	    pTemp[playerid][pServeType] = 0;

	    if (!IsPlayerLogged(mechanicid)) return SendErrorMessage(playerid, "O jogador que ofereceu os serviços de um mecânico não está online.");
	    if (PlayerInfo[mechanicid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Este jogador não é um mecânico.");
	    if (!IsDriver(mechanicid) || !IsATowTruck(vehid)) return SendErrorMessage(playerid, "O mecânico deve estar no caminhão de reboque como motorista.");
	    if (!IsDriver(playerid)) return SendErrorMessage(playerid, "Você deve estar no banco do motorista.");
	    if (!PlayerNearPlayer(6.0, playerid, mechanicid)) return SendNotNearPlayer(playerid);
	    if (VehicleInfo[vehicle][carFaction] != 0 && type != 1 && type != 2) return SendErrorMessage(playerid, "Este veículo pertence à estrutura estatal.");

	    new mes[128];

	    switch (type)
	    {
	        case 1: format(mes, sizeof(mes), "aceita um pedido para trabalhar no motor de %s.", VehicleNames[GetVehicleModel(vehicle) - 400]);
	        case 2: format(mes, sizeof(mes), "aceita um pedido para fazer reparos na carroceria de %s.", VehicleNames[GetVehicleModel(vehicle) - 400]);
	        case 3: format(mes, sizeof(mes), "aceita um pedido para reparar danos em %s.", VehicleNames[GetVehicleModel(vehicle) - 400]);
	        case 4: format(mes, sizeof(mes), "aceita um pedido para pintar %s.", VehicleNames[GetVehicleModel(vehicle) - 400]);
	    }
	    cmd::me(playerid, mes);

	    RepairOffer[mechanicid] = playerid;
	    RepairToVehicle[mechanicid] = vehicle;
	    RepairService[mechanicid] = type;
	    RepairTowTruck[mechanicid] = vehid;

	    ShowJobMessage(mechanicid, "~h~~p~TIRE SEU SPRAYCAN.");
	    return 1;
	}
	else if (pTemp[playerid][pHousePlayer] != INVALID_PLAYER_ID)
	{
	    new userid = pTemp[playerid][pHousePlayer], price = pTemp[playerid][pHousePrice], houseid = pTemp[playerid][pHouseHouse];

	    pTemp[playerid][pHouseHouse] = -1;
	    pTemp[playerid][pHousePrice] = 0;
	    pTemp[playerid][pHousePlayer] = INVALID_PLAYER_ID;

	    if (!IsPlayerLogged(userid)) return SendErrorMessage(playerid, "O jogador que ofereceu a compra de uma casa não está online.");
	    if (houseid == -1) return SendErrorMessage(playerid, "Ninguém ofereceu a compra de uma casa para você.");
	    if (price < 0 || price > 100000000) return SendErrorMessage(playerid, "Valor de venda de $0 a $100000000.");
	    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
	    if (IsPlayerOutsideHouse(playerid, houseid) == 0) return SendErrorMessage(playerid, "Você deve estar próximo da casa que deseja comprar.");
	    if (HouseInfo[houseid][hOwnerID] != PlayerInfo[userid][pID]) return SendErrorMessage(playerid, "Esta casa não pertence a %s.", GetNameEx(userid));
	    if (GetCash(playerid) < price) return SendNotCash(playerid);
	    if (GetPlayerHouses(playerid) >= GetMaxPlayerHouse(playerid)) return SendErrorMessage(playerid, "Você não pode comprar mais casas.");

	    HouseInfo[houseid][hOwnerID] = PlayerInfo[playerid][pID];
	    format(HouseInfo[houseid][hOwner], MAX_PLAYER_NAME, GetNameEx(playerid));
	    GiveCash(playerid, -price);
	    GiveCash(userid, price);

	    for (new i = 0; i != MAX_GARAGES; i++)
	    {
	        if (!GarageInfo[i][garageID] || GarageInfo[i][garageHouse] != HouseInfo[houseid][hID]) continue;

	        SendSyntaxMessage(playerid, "Junto com a casa, você obteve uma garagem [ID: %i].", i);

	        GarageInfo[i][garageOwner] = PlayerInfo[playerid][pID];
	        format(GarageInfo[i][garageOwnerName], 24, "%s", GetNameEx(playerid));

	        SQL_SetInteger("garage", "owner", GarageInfo[i][garageOwner], GarageInfo[i][garageID]);
	        SQL_SetString("users", "name", GarageInfo[i][garageOwnerName], GarageInfo[i][garageOwner]);
	    }

	    Save_House(houseid);

	    SendSyntaxMessage(playerid, "Você comprou uma casa do jogador %s por $%i.", GetNameEx(userid), price);
	    SendSyntaxMessage(userid, "%s comprou uma casa sua por $%i.", GetNameEx(playerid), price);
	    return 1;
	}


	else if (pTemp[playerid][pBizPlayer] != INVALID_PLAYER_ID)
	{
	    new userid = pTemp[playerid][pBizPlayer], price = pTemp[playerid][pBizPrice], bizid = pTemp[playerid][pBizID];

	    pTemp[playerid][pBizPlayer] = INVALID_PLAYER_ID;
	    pTemp[playerid][pBizPrice] = 0;
	    pTemp[playerid][pBizID] = INVALID_PLAYER_ID;

	    if (!IsPlayerLogged(userid)) return SendErrorMessage(playerid, "O jogador que ofereceu a compra do negócio não está online.");
	    if (bizid == -1) return SendErrorMessage(playerid, "Ninguém ofereceu a compra de um negócio para você.");
	    if (price < 0 || price > 100000000) return SendErrorMessage(playerid, "Valor de venda de $0 a $100000000.");
	    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[userid][pID]) return SendErrorMessage(playerid, "Este negócio não pertence a %s.", GetNameEx(userid));
	    if (GetCash(playerid) < price) return SendNotCash(playerid);
	    if (GetPlayerBizes(playerid) >= GetMaxPlayerBiz(playerid)) return SendErrorMessage(playerid, "Você não pode comprar mais negócios.");

	    if (IsPlayerBizOutside(playerid, bizid) == 0 && IsPlayerBizInside(playerid, bizid) == 0) return SendErrorMessage(playerid, "Você deve estar próximo do negócio que deseja comprar.");

	    BizInfo[bizid][bizOwnerID] = PlayerInfo[playerid][pID];
	    format(BizInfo[bizid][bizOwner], MAX_PLAYER_NAME, "%s", GetAccName(playerid));

	    GiveCash(playerid, -price);
	    GiveCash(userid, price);

	    for (new i = 0; i != MAX_GARAGES; i++)
	    {
	        if (!GarageInfo[i][garageID] || GarageInfo[i][garageBiz] != BizInfo[bizid][bizID]) continue;

	        GarageInfo[i][garageOwner] = PlayerInfo[playerid][pID];
	        SendSyntaxMessage(playerid, "Junto com o negócio, você obteve uma garagem [ID: %i].", i);

	        new query[128];
	        mysql_format(dbHandle, query, sizeof(query), "UPDATE `garage` SET `owner`=%i WHERE `id`=%i", PlayerInfo[playerid][pID], GarageInfo[i][garageID]);
	        mysql_tquery(dbHandle, query);
	    }

	    RemoveAllWorkers(bizid);
	    Save_Business(bizid);

	    SendSyntaxMessage(playerid, "Você comprou um negócio do jogador %s por $%i.", GetNameEx(userid), price);
	    SendSyntaxMessage(userid, "%s adquiriu um negócio seu por $%i.", GetNameEx(playerid), price);
	    return 1;
	}


	else if (pTemp[playerid][pRentPlayer] != INVALID_PLAYER_ID)
	{
	    new userid = pTemp[playerid][pRentPlayer], price = pTemp[playerid][pRentPrice];

	    pTemp[playerid][pRentPlayer] = INVALID_PLAYER_ID;
	    pTemp[playerid][pRentPrice] = 0;

	    if (!IsPlayerLogged(userid)) return SendErrorMessage(playerid, "O jogador que ofereceu a locação da casa não está online.");
	    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);

	    new houseid;

	    if ((houseid = GetNearInsideHouse(userid)) == -1) return SendErrorMessage(userid, "Você deve estar dentro de sua casa.");
	    if (HouseInfo[houseid][hOwnerID] != PlayerInfo[userid][pID]) return SendErrorMessage(userid, "Esta casa não pertence a você.");
	    if (GetCash(playerid) < price) return SendErrorMessage(playerid, "Você não tem $%i para o pagamento adiantado.", price);

	    PlayerInfo[playerid][pRentKey] = HouseInfo[houseid][hID];
	    PlayerInfo[playerid][pRentPrice] = price;
	    SendSyntaxMessage(playerid, "Você alugou uma casa de %s. Você pagará $%i a cada salário.", GetNameWithMask(userid), price);
	    SendSyntaxMessage(playerid, "Você pode sair da casa a qualquer momento usando o comando: /unrent.");
	    SendSyntaxMessage(userid, "%s alugou sua casa. Ele pagará $%i a cada salário.", GetNameWithMask(playerid), price);

	    new query[128];
	    format(query, sizeof(query), "UPDATE `users` SET `rent_key`=%i,`rent_price`=%i WHERE `id`=%i", PlayerInfo[playerid][pRentKey], PlayerInfo[playerid][pRentPrice], PlayerInfo[playerid][pID]);
	    mysql_tquery(dbHandle, query);

	    return 1;
	}

	else if (pTemp[playerid][pInterviewPlayer] != INVALID_PLAYER_ID)
	{
	    if (PlayerInfo[playerid][pSettings] & togNews) return SendErrorMessage(playerid, "Primeiro habilite a exibição de mensagens do SAN (/tog news).");

	    new userid = pTemp[playerid][pInterviewPlayer];

	    pTemp[playerid][pInterviewPlayer] = INVALID_PLAYER_ID;

	    if (!IsPlayerLogged(userid)) return SendErrorMessage(playerid, "O jogador que ofereceu a entrevista não está online.");
	    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
	    if (!IsPlayerFactionNews(playerid)) return SendErrorMessage(playerid, "O jogador deve ser um funcionário do SAN.");

	    pTemp[playerid][pInterview] = userid;
	    SendSyntaxMessage(playerid, "Você aceitou a oferta de %s para realizar uma entrevista.", GetNameWithMask(userid));
	    SendSyntaxMessage(playerid, "Use o chat local para transmitir. /exitinterview - encerrar a entrevista.");
	    SendSyntaxMessage(userid, "%s aceitou sua oferta de conduzir uma entrevista.", GetNameWithMask(playerid));

	    return 1;
	}

	
	return 0;
}

GetPlayerDeniedAction(playerid) {
	if (GetPVarInt(playerid, #accept_time) < gettime()) return 0;
	DeletePVar(playerid, #accept_time);
	return 1;
}