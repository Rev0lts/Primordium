SendTaxiRequest(playerid, location[])
{
    foreach(new i : Player)
	{
	    // Verifica se o jogador está logado, se está de serviço e se é um taxista
	    if (!IsPlayerLogged(i) || !TaxiDuty[i] || PlayerInfo[i][pJob] != JOB_TAXI) continue;

        // Envia uma mensagem de chamada de táxi para o taxista
        SendClientMessage(i, COLOR_GREEN, "|___________Chamada de Táxi___________|");
		
		// Verifica se a chamada vem de um telefone público ou de um jogador
		if (pTemp[playerid][pPayphone] == -1) 
			SendClientMessageEx(i, COLOR_WHITE, "Chamada de: %s Tel: %i", GetNameEx(playerid), PlayerInfo[playerid][pNumber]);
		else
			SendClientMessageEx(i, COLOR_WHITE, "Chamada de: Telefone Público, Tel: %i", PayphoneInfo[pTemp[playerid][pPayphone]][phNumber]);
		
		// Envia a localização da chamada para o taxista
		SendClientMessageEx(i, COLOR_WHITE, "Localização: %s", location);
		
		// Instrui o taxista sobre como aceitar a chamada
		SendClientMessageEx(i, COLOR_WHITE, "*/taxi aceitar %i para aceitar a chamada **", playerid);
	}
	return 1;
}

Taxi_OnGameModeInit()
{
    // Configura um timer que é acionado a cada 5 segundos
    return SetTimer("Taxi_FiveSecTimer", 5000, false);
}
