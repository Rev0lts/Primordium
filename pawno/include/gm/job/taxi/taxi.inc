Taxi_OnPlayerStateChange(playerid, newstate)
{
    if (newstate == PLAYER_STATE_PASSENGER)
	{
	    new vehicle = GetPlayerVehicleID(playerid), driver = VehicleInfo[vehicle][carDriver];
        if (IsPlayerLogged(driver) && TaxiDuty[driver] && TaxiPassenger[driver] == INVALID_PLAYER_ID)
        {
            if (GetCash(playerid) < TaxiFare[driver])
			{
		 		SendErrorMessage(playerid, "Você não tem dinheiro suficiente.");
		 		SendErrorMessage(driver, "O passageiro não tem dinheiro suficiente.");
		 		return 1;
			}
			TaxiPassenger[driver] = playerid;
            SendClientMessageEx(playerid, COLOR_YELLOWG, "[TAXI]Você pegou um táxi para %s. Tarifa: $%i/5 segundos.", GetNameEx(driver), TaxiFare[driver]);
		}
	}
    return 1;
}
