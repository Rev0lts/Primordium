CMD:reparar(playerid, params[])
{
    new userid, type, vehid = GetPlayerVehicleID(playerid);

    if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Voc� n�o � um mec�nico.");
    if (!IsDriver(playerid) || !IsATowTruck(vehid)) return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");

    if (sscanf(params, "ui", userid, type))
    {
        SendHelpMessage(playerid, "/reparar [ID/Parte do Nome] [tipo]");
        SendSyntaxMessage(playerid, "tipo 1: Conserto do motor ((Restabelece HP do ve�culo))");
        SendSyntaxMessage(playerid, "tipo 2: Reparo da lataria ((Trabalho visual))");
        SendSyntaxMessage(playerid, "tipo 3: Recupera��o do motor ((Motor falhou))");
        return 1;
    }

    if (type < 1 || type > 3)
    {
        SendSyntaxMessage(playerid, "tipo 1: Conserto do motor ((Restabelece HP do ve�culo))");
        SendSyntaxMessage(playerid, "tipo 2: Reparo da lataria ((Trabalho visual))");
        SendSyntaxMessage(playerid, "tipo 3: Recupera��o do motor ((Motor falhou))");
        return 1;
    }

    if (userid == playerid) return SendErrorMessage(playerid, "Voc� n�o pode realizar essas a��es em si mesmo.");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (!PlayerNearPlayer(6.0, playerid, userid)) return SendNotNearPlayer(playerid);
    if (!IsDriver(userid)) return SendErrorMessage(playerid, "O jogador deve estar no ve�culo.");

    new vehicle = GetPlayerVehicleID(userid);

    if (VehicleInfo[vehicle][carFaction] != 0 && type != 1 && type != 2) return SendErrorMessage(playerid, "Esse ve�culo pertence ao estado.");
    if (pTemp[userid][pServePlayer] == playerid && (pTemp[userid][pServeType] > 0 && pTemp[userid][pServeType] < 4)) return SendErrorMessage(playerid, "Voc� j� ofereceu seus servi�os para %s.", GetNameEx(userid));

    if (type == 1)
    {
        if (VehicleInfo[vehid][carComps] < 75) return SendErrorMessage(playerid, "Voc� precisa de pelo menos 75 componentes para reparar o motor.");

        new mes[128];
        format(mes, sizeof(mes), " %s oferece reparo no motor da(o) %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle) - 400]);
        cmd::me(playerid, mes);
        SendSyntaxMessage(userid, "%s oferece reparo no motor. (Pressione Y para aceitar a oferta)", GetNameWithMask(playerid));
    }
    else if (type == 2)
    {
        if (VehicleInfo[vehid][carComps] < 75) return SendErrorMessage(playerid, "Voc� precisa de pelo menos 75 componentes para reparar a lataria.");

        new mes[128];
        format(mes, sizeof(mes), "oferece reparo na lataria da(o) %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle) - 400]);
        cmd::me(playerid, mes);
        SendSyntaxMessage(userid, "%s oferece reparo na lataria. (Pressione Y para aceitar a oferta)", GetNameWithMask(playerid));
    }
    else if (type == 3)
    {
        if (VehicleInfo[vehid][carComps] < 75) return SendErrorMessage(playerid, "Voc� precisa de pelo menos 75 componentes para consertar um motor inoperante.");
        if (VehicleInfo[vehicle][carEngine] >= 20.0) return SendErrorMessage(playerid, "O motor deste ve�culo n�o precisa de reparos ( > 20%%).");

        new mes[128];
        format(mes, sizeof(mes), " %s oferece reparo no motor defeituoso da(o) %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle) - 400]);
        cmd::me(playerid, mes);
        SendSyntaxMessage(userid, "%s oferece reparo no motor defeituoso. (Pressione Y para aceitar a oferta)", GetNameWithMask(playerid));
    }

    pTemp[userid][pServePlayer] = playerid;
    pTemp[userid][pServeType] = type;
    SetPVarInt(userid, #accept_time, gettime() + 30);
    return 1;
}

CMD:comprarpecas(playerid, params[])
{
    new amount;
    
    if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Voc� n�o � um mec�nico.");
    if (!IsDriver(playerid)) return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
    if (!IsATowTruck(GetPlayerVehicleID(playerid))) return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
    if (!PlayerToPoint(6.0, playerid, 2430.4412,-2454.9304,13.6250))
    {
        Waypoint_Set(playerid, 2430.4412,-2454.9304,13.6250, 8.0);
        return SendSyntaxMessage(playerid, "A localiza��o para compra de pe�as est� marcada no mapa. (/removercp)");
    }

    if (sscanf(params,"d",amount))
    {
        SendHelpMessage(playerid, "/comprarpeca [quantidade]");
        return 1;
    }

    if (amount < 1 || amount > 300) return SendErrorMessage(playerid, "A quantidade de pe�as deve estar entre 1 e 300.");
    if (VehicleInfo[GetPlayerVehicleID(playerid)][carComps] >= 300) return SendErrorMessage(playerid, "O ve�culo atingiu o n�mero m�ximo de pe�as. ");
    if (VehicleInfo[GetPlayerVehicleID(playerid)][carComps] + amount > 300) return SendErrorMessage(playerid, "Voc� s� pode comprar %i pe�as.", 300 - VehicleInfo[GetPlayerVehicleID(playerid)][carComps]);
    if (GetCash(playerid) < amount) return SendErrorMessage(playerid, "Voc� precisa de $%i para comprar.", amount);

    GiveCash(playerid, -amount);
    SendSyntaxMessage(playerid, "Voc� comprou %i pe�as com sucesso por $%i.", amount, amount);
    JobMessage(playerid, "~w~VOC� COMPROU ALGUNS PRODUTOS PARA SEU CAMINH�O.", 6);
    VehicleInfo[GetPlayerVehicleID(playerid)][carComps] += amount;
    return 1;
}

CMD:minhaspecas(playerid, params[])
{
    if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Voc� n�o � um mec�nico.");
    if (!IsDriver(playerid)) return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
    if (!IsATowTruck(GetPlayerVehicleID(playerid))) return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");

    SendSyntaxMessage(playerid, "No guincho, voc� tem %i pe�as.", VehicleInfo[GetPlayerVehicleID(playerid)][carComps]);
    return 1;
}


CMD:reabastecer(playerid, params[])
{
    new userid, price;

    if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Voc� n�o � um mec�nico.");
    if (!IsDriver(playerid)) return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
    if (!IsATowTruck(GetPlayerVehicleID(playerid))) return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
    if (sscanf(params, "ud", userid, price)) return SendHelpMessage(playerid, "/reabastecer [ID/Parte do Nome] [pre�o]");
    if (userid == playerid) return SendErrorMessage(playerid, "Voc� n�o pode realizar essas a��es sozinho.");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (!PlayerNearPlayer(6.0, playerid, userid)) return SendNotNearPlayer(playerid);
    if (!IsDriver(userid)) return SendErrorMessage(playerid, "O jogador deve estar no ve�culo.");

    new vehicle = GetPlayerVehicleID(userid);

    if (VehicleInfo[vehicle][carFuel] >= GetVehicleFuelCapacity(GetVehicleModel(vehicle)))
        return SendErrorMessage(playerid, "O tanque est� cheio.");
    if (price < 1 || price > 500)
        return SendErrorMessage(playerid, "O custo do reabastecimento deve ser entre $1 e $500.");

    new mes[128];
    format(mes, sizeof(mes), "oferece-se para reabastecer o ve�culo de %s por $%i.", GetNameWithMask(userid), price);
    cmd::me(playerid, mes);

    SendClientMessageEx(userid, COLOR_INFO, "%s convida voc� para reabastecer o ve�culo por $%i. (Pressione Y para aceitar a oferta)", GetNameWithMask(playerid), price);
    RefillPrice[userid] = price;
    RefillOffer[userid] = playerid;
    return 1;
}


CMD:pintarveiculo(playerid, params[])
{
	new userid, color1, color2;

	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC)					return SendErrorMessage(playerid, "Voc� n�o � um mec�nico.");
	if (!IsDriver(playerid))											return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
	if (!IsATowTruck(GetPlayerVehicleID(playerid)))				return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
	if (sscanf(params,"udd", userid, color1, color2))				return SendHelpMessage(playerid, "/pintarveiculo [ID/Parte do nome] [Cor 1] [Cor 2](/corlista)");
	if (color1 > 255 || color2 > 255 || color1 < 0 || color2 < 0)	return SendErrorMessage(playerid, "O ID da cor pode ser de 0 a 255.");
	if (userid == playerid)											return SendErrorMessage(playerid, "Voc� n�o pode realizar essas a��es por si mesmo.");
	if (!IsPlayerLogged(userid))										return SendNotConnected(playerid);
	if (!PlayerNearPlayer(6.0, playerid, userid))					return SendNotNearPlayer(playerid);
	if (!IsDriver(userid))											return SendErrorMessage(playerid, "O jogador deve estar no ve�culo.");

	new vehicle = GetPlayerVehicleID(userid);

	if (VehicleInfo[vehicle][carFaction] != 0)										return SendErrorMessage(playerid, "Este transporte pertence � estrutura estatal. ");
	if (VehicleInfo[GetPlayerVehicleID(playerid)][carComps] < 45)					return SendErrorMessage(playerid, "Voc� precisa de no m�nimo 45 componentes para repintar. ");
    if (pTemp[userid][pServePlayer] == playerid && pTemp[userid][pServeType] == 4)	return SendErrorMessage(playerid, "Voc� j� ofereceu seus servi�os para %s.", GetNameEx(userid));

	new mes[128];
    format(mes, sizeof(mes), "oferece para %s pintar a(o) %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle)-400]);
	cmd::me(playerid, mes);

	SendClientMessageEx(userid, COLOR_INFO, "%s oferece a voc� um trabalho de pintura na carroceria. (Pressione Y para aceitar a oferta)", GetNameWithMask(playerid));

	SetPVarInt(playerid, "color1", color1);
	SetPVarInt(playerid, "color2", color2);
	pTemp[userid][pServePlayer] = playerid;
 	pTemp[userid][pServeType] = 4;
 	SetPVarInt(userid, #accept_time, gettime() + 30);
	return 1;
}

CMD:rebocarv(playerid, params[])
{
	new userid, vehicle = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC)		return SendErrorMessage(playerid, "Voc� n�o � um mec�nico.");
	if (!IsDriver(playerid) || !IsATowTruck(vehicle))	return SendErrorMessage(playerid, "Voc� deve estar no guincho como motorista.");
	if (IsTrailerAttachedToVehicle(vehicle))
	{
		DetachTrailerFromVehicle(vehicle);
		return SendErrorMessage(playerid, "O ve�culo previamente engatado foi desengatado.");
	}

	if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/rebocarv [ID/Parte do nome]");
	if (userid == playerid)							return SendErrorMessage(playerid, "Voc� n�o pode realizar essas a��es por si mesmo.");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(8.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (!IsPlayerInAnyVehicle(userid))				return SendErrorMessage(playerid, "O jogador deve estar no ve�culo.");

	if (pTemp[userid][pServePlayer] == playerid && pTemp[userid][pServeType] == 5) 	return SendErrorMessage(playerid, "Voc� j� ofereceu seus servi�os para %s.", GetNameEx(userid));

	new vehid = GetPlayerVehicleID(userid);

    if (IsABoat(vehid) || IsAPlane(vehid) || IsAHelicopter(vehid) || IsABiker(vehid))	return SCM(playerid, COLOR_GREY,"Este transporte n�o pode ser engatado.");

    new mes[128];
    format(mes, sizeof(mes), "oferece para %s rebocar %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehid)-400]);
	cmd::me(playerid, mes);

	SendClientMessageEx(userid, COLOR_INFO, "* %s oferece a voc� reboque de transporte. (Saia do carro e pressione Y para aceitar a oferta)", GetNameWithMask(playerid));
	SetPVarInt(userid, #tow_car, vehid);
	pTemp[userid][pServePlayer] = playerid;
	pTemp[userid][pServeType] = 5;
	SetPVarInt(userid, #accept_time, gettime() + 30);
	return 1;
}