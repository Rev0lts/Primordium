CMD:taxi(playerid, params[])
{
	new option[11], secoption, vehicle = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][pJob] != JOB_TAXI)	return SendErrorMessage(playerid, "Você não é um motorista de táxi.");
	if (sscanf(params,"s[11]D(-1)", option, secoption))
	{
		SendHelpMessage(playerid, "/taxi [opção]"); // disponível
		SendSyntaxMessage(playerid, "duty / taxa / aceitar");
		return 1;
	}

	if (!IsDriver(playerid))	return SendErrorMessage(playerid, "Você deve estar em um Taxi ou Cabbie como motorista.");
	if (!IsATaxi(vehicle))	return SendErrorMessage(playerid, "Você deve estar em um Taxi ou Cabbie.");

	if (CompareStrings(option, "duty"))
	{
		if (TaxiDuty[playerid])
		{
			SCM(playerid, COLOR_YELLOWG, "[TAXI] Você saiu do serviço e chamadas não estão mais disponíveis para você.");
			SendClientMessageEx(playerid, COLOR_YELLOWG, "[TAXI] Foi creditado a você o valor de $%i.", TaxiMade[playerid]);
			GiveCash(playerid, TaxiMade[playerid]);
			SetPlayerColor(playerid, TEAM_HIT_COLOR);
			TaxiDuty[playerid] = false;
			TaxiMade[playerid] = 0;
			TaxiMoney[playerid] = 0;
			TaxiPassenger[playerid] = INVALID_PLAYER_ID;
			return 1;
		}

		SendClientMessageEx(playerid, COLOR_YELLOWG, "[TAXI] Você está em serviço e pode receber chamadas. Tarifa: $%i/5 segundos.", TaxiFare[playerid]);
		SetPlayerColor(playerid, TAXI_DUTY);
		TaxiDuty[playerid] = true;
		TaxiMoney[playerid] = 0;
		TaxiMade[playerid] = 0;
		TaxiPassenger[playerid] = INVALID_PLAYER_ID;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !IsPlayerInVehicle(i, vehicle) || GetPlayerState(i) != PLAYER_STATE_PASSENGER)	continue;

			TaxiPassenger[playerid] = i;
			SendClientMessageEx(i, COLOR_YELLOWG, "[TAXI] Você pegou um táxi para %s. Tarifa: $%i/5 segundos.", GetNameEx(playerid), TaxiFare[playerid]);
			break;
		}

		return 1;
	}

	else if (CompareStrings(option, "taxa"))
	{
		new fare;

		if (sscanf(params,"{s[11]}d",fare))			return SendHelpMessage(playerid, "/taxi fare [custo]");
		if (fare < 0 || fare > 25)					return SendErrorMessage(playerid, "A tarifa não pode ser menos de $0 e não mais de $25.");
		if (IsPlayerLogged(TaxiPassenger[playerid]))	return SendErrorMessage(playerid, "Dirija o cliente antes de mudar a tarifa.");
		    
		SendClientMessageEx(playerid,COLOR_YELLOWG,"[TAXI] Você definiu a tarifa para $%i/5 segundos.", fare);
		TaxiFare[playerid] = fare;
	}

	else if (CompareStrings(option, "aceitar"))
	{
		new id, Float:x, Float:y, Float:z;

		if (sscanf(params,"{s[11]}u",id))	return SendHelpMessage(playerid, "/taxi accept [ID/Part Of Name]");
		if (!TaxiDuty[playerid])				return SendErrorMessage(playerid, "Para aceitar a chamada, você precisa assumir o serviço (/taxi duty).");
		if (!IsPlayerLogged(id))				return SendNotConnected(playerid);
		if (GetPVarInt(id, "NeedTaxi") == 0)	return SendErrorMessage(playerid, "Este jogador não chamou um táxi, ou alguém já aceitou a chamada.");

		SendSyntaxMessage(id, "%s aceitou seu chamado. Tarifa: $%i/5 segundos.", GetNameEx(playerid), TaxiFare[playerid]);
		SendSyntaxMessage(playerid, "Você aceitou o chamado de %s, tel.%i", GetNameEx(playerid), PlayerInfo[id][pNumber]);
		GetPlayerPos(id, x, y, z);
		Waypoint_Set(playerid, x, y, z, 3.0);
		DeletePVar(id, "NeedTaxi");
	}
	return 1;
}
