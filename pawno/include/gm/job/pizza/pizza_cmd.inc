CMD:pegarpizza(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar esse recurso no momento.");

	if (!pTemp[playerid][pPizzaBoy])								return SendErrorMessage(playerid, "Você não está cumprindo o pedido.");
    if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))		return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");
        
	new vehicle = GetPlayerVehicleID(playerid);

	if (vehicle != 0)																		return SendErrorMessage(playerid, "Você não deve estar no veículo.");
    if ((vehicle = GetNearVehicle(playerid, 3.0)) == 0 || GetVehicleModel(vehicle) != 448)	return SendErrorMessage(playerid, "Você deve estar perto do ciclomotor de pizza.");
    
    SetPlayerAttachedObject(playerid, ATTACH_HANDS, 19571, 6, -0.157999, -0.002000, -0.169000, -21.999990, 87.000022, 0.000000, 1.186000, 1.000000, 1.170000);
  	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
  	InfoMSG(playerid, "Pressione \" ~r~ENTER ~w~\" para largar a pizza.", 5); //mensagem
	return 1;
}

CMD:entregarpizza(playerid, params[])
{
	if (pTemp[playerid][pPizzaBoy])
	{
	    pTemp[playerid][pPizzaBoy] = 0;
	    SendSyntaxMessage(playerid, "Você recusou entregar o seu pedido. O novo pedido estará disponível em dois minutos.");
	    DisableWaypoint(playerid);
	    SetPVarInt(playerid, #pizza_time, gettime() + 120);
	    return 1;
	}
	
	if (GetPVarInt(playerid, #pizza_time) > gettime())									return SendErrorMessage(playerid, "[Rádio]: Sem pedidos, por favor tente mais tarde.");
	if (!IsDriver(playerid) || GetVehicleModel(GetPlayerVehicleID(playerid)) != 448)	return SendErrorMessage(playerid, "Você deve estar dirigindo um ciclomotor. ");
		
	new i, houseid = random(MAX_HOUSES);
		
	while(i == 0)
	{
	    if (!HouseInfo[houseid][hID] || HouseInfo[houseid][hPosInt] || HouseInfo[houseid][hPosVW])
		{
			houseid = random(MAX_HOUSES);
		}
		else
		{
		    i = 1;
		    pTemp[playerid][pPizzaBoy] = houseid+1;
		    SendClientMessageEx(playerid, COLOR_GREEN, "[Rádio]: Novo pedido. Leve a pizza para o endereço: %s", (!HouseInfo[houseid][hApart])?(GetHouseZone(houseid)):(GetApartmentZone(HouseInfo[houseid][hApart])));
            SendHelpMessage(playerid, "Use /pegarpizza - para pegar a pizza na mão.");

			if (!HouseInfo[houseid][hApart]) 
				Waypoint_Set(playerid, HouseInfo[houseid][hPosX], HouseInfo[houseid][hPosY], HouseInfo[houseid][hPosZ]);
			else 
				Waypoint_Set(playerid, ApartInfo[houseid][aPosX], ApartInfo[houseid][aPosY], ApartInfo[houseid][aPosZ]);

		    return 1;
		}
	}
	return 1;
}
