CMD:entregarcarro(playerid, params[])
{
	if (IsPoliceNear(playerid)) return InfoMSG(playerid, "Há ~p~~b~oficiais de polícia ~w~por perto!", 4);
	if (!PoliceOnline()) return SendErrorMessage(playerid, "Deve haver pelo menos %i policiais no jogo.", NEED_COPS_TO_CRIME);

	new
		id = GetChopshopID(playerid),
		vehID = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][pJob] != JOB_CARJACKER) return SendErrorMessage(playerid, "Você não é um ladrão de carros.");
	if (id == -1 || !IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Não há nenhum desmanche perto de você, ou você não está dirigindo um veículo.");

	if (VehicleInfo[vehID][carOwnerID] == PlayerInfo[playerid][pID] || PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehID][carOwnerID])
		return SendErrorMessage(playerid, "Você não pode desmontar seu próprio veículo.");

	if (VehicleInfo[vehID][carOwnerID] == 0) return SendErrorMessage(playerid, "Apenas veículos pessoais podem ser desmontados.");
	if (PlayerInfo[playerid][InMission]) return SendErrorMessage(playerid, "Você já está em uma missão.");
	if (!CheckWantedModel(id, GetVehicleModel(vehID))) return SendErrorMessage(playerid, "Este veículo não é necessário.");

	new count, time[64];
	for (new i; i < 14; i++) if (VehicleInfo[vehID][carComponent][i] > 0) count++;

	if (VehicleInfo[vehID][carEngineOn]) ToggleVehicleEngine(vehID, playerid);

	PlayerInfo[playerid][InMission] = CARJACKER_DELIVER;
	PlayerInfo[playerid][MissionTime] = 15 + count;
	PlayerInfo[playerid][MissionTarget][0] = vehID;
	PlayerInfo[playerid][MissionTarget][1] = id;

	format(time, sizeof(time), "~r~DESATIVAÇÃO DAS PEÇAS DE VEÍCULOS~n~~w~RESTAM ~y~%d ~w~SEGUNDOS", PlayerInfo[playerid][MissionTime]);
	InfoMSG(playerid, time, 2);

	return 1;
}

CMD:abandonarmissao(playerid, params[])
{
	if (!PlayerInfo[playerid][InMission]) return SendErrorMessage(playerid, "Você não está em uma missão.");
	LeaveMission(playerid);
	return 1;
}

CMD:largacarro(playerid, params[])
{
	if (IsPoliceNear(playerid)) return InfoMSG(playerid, "Há ~p~~b~oficiais de polícia ~w~por perto!", 4);

	new vehID = PlayerInfo[playerid][MissionTarget][0],
		id = PlayerInfo[playerid][MissionTarget][1],
		Float: playerPos[3];

	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	if (PlayerInfo[playerid][pJob] != JOB_CARJACKER) return SendErrorMessage(playerid, "Você não é um ladrão de carros.");
	if (!PlayerInfo[playerid][InMission]) return SendErrorMessage(playerid, "Você não está em uma missão.");

	if (PlayerInfo[playerid][InMission] != CARJACKER_DROPOFF) return SendErrorMessage(playerid, "Você não está roubando veículos.");
	if (GetPlayerVehicleID(playerid) != PlayerInfo[playerid][MissionTarget][0]) return SendErrorMessage(playerid, "Este não é o veículo correto!");

	if (!IsPlayerInRangeOfPoint(playerid, 50.0, chopshop_data[id][chopshop_pos][0], chopshop_data[id][chopshop_pos][1], chopshop_data[id][chopshop_pos][2]))
	{
		new Float: distance_reward = XB_GetDistanceBetweenTPoints(playerPos[0], playerPos[1], playerPos[2], chopshop_data[id][chopshop_pos][0], chopshop_data[id][chopshop_pos][1], chopshop_data[id][chopshop_pos][2]);

		if (distance_reward > 200.0) PlayerInfo[playerid][MissionReward] += 400;
		else PlayerInfo[playerid][MissionReward] += floatround(distance_reward) * 2;

		new money[32];
		format(money, 32, "~g~MISSAO COMPLETA~n~~w~VOCE GANHOU ~y~$%s", FormatNumber(PlayerInfo[playerid][MissionReward]));
		InfoMSG(playerid, money, 5);

		GiveCash(playerid, PlayerInfo[playerid][MissionReward]);

		new userid;
		foreach (new i : Player)
		{
			if (PlayerInfo[i][pID] == VehicleInfo[vehID][carOwnerID])
			{
				userid = i;
				break;
			}
		}

		SendClientMessageEx(userid, COLOR_LIGHTRED, "[ VEÍCULO ] Seu %s foi roubado. (/meuscars)", VehicleNames[GetVehicleModel(vehID) - 400]);

		VehicleInfo[vehID][carDestroyed]++;

		SQL_SetInteger("cars", "jacking", 1, VehicleInfo[vehID][carID]);
		SQL_SetInteger("cars", "destroys", VehicleInfo[vehID][carDestroyed], VehicleInfo[vehID][carID]);

		new money_str[70];
		format(money_str, sizeof(money_str), "VOCÊ ~g~CONSEGUIU ~w~SE LIVRAR DO VEÍCULO~n~E GANHOU ~g~$%i", PlayerInfo[playerid][MissionReward]);
		InfoMSG(playerid, money_str, 5);
		RemovePlayerFromVehicle(playerid);

		ResetVehicleStuff(vehID);
		DestroyVehicle(vehID);

		PlayerInfo[playerid][MissionReward] = 0;
		PlayerInfo[playerid][InMission] = MISSION_NONE;
		PlayerInfo[playerid][MissionTime] = 0;
		PlayerInfo[playerid][MissionTarget][0] = 0;
		PlayerInfo[playerid][MissionTarget][1] = 0;
	}
	else return SendErrorMessage(playerid, "Você está muito perto da oficina!"); // Mensagem de erro se o jogador estiver muito perto da oficina.
	return 1;
}

