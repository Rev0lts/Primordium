CMD:licenseexam(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!IsADMVCar(vehicle) && !IsADMVHeli(vehicle) && !IsADMVTaxi(vehicle)) return SendErrorMessage(playerid, "Você deve estar em um veículo do DMV.");
	if (!IsDriver(playerid))												 return SendErrorMessage(playerid, "Você deve estar dirigindo um veículo.");
    if (pTemp[playerid][pLicenseExam] != 0)									 return SendErrorMessage(playerid, "Você já está fazendo um dos exames.");
	        
	if (IsADMVCar(vehicle))
	{
        if (PlayerInfo[playerid][pCarLic] != 0)	return SendErrorMessage(playerid, "Você já possui uma carteira de motorista.");
		if (GetCash(playerid) < CARLIC_PRICE)	return SendNotCash(playerid);

		VehicleEngine(vehicle, true); //ligar o motor
		SendSyntaxMessage(playerid, "Você iniciou o exame para a carteira de motorista.");
		SendSyntaxMessage(playerid, "Você tem %i minutos para completar, caso contrário, você será reprovado no exame.", EXAM_TIME / 60);
		
		pTemp[playerid][pLicenseExam] = 1;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, license_exam_cp[0], true);
		GiveCash(playerid, -CARLIC_PRICE);

		new bizid = LicenseOffice_Nearest(playerid);
		if (bizid != -1) GiveBizCash(bizid, CARLIC_PRICE);
	
		if (!IsPlayerInAnyVehicle(playerid)) {
			pTemp[playerid][pLicenseExam] = 0;
			DeletePVar(playerid, #exam_second);
			SendSyntaxMessage(playerid, "Você falhou no exame por ter saído do veículo.");
			TogglePlayerAllDynamicRaceCPs(playerid, false);
			RespawnVehicle(vehicle);
			return 1;
		}
	}
	else if (IsADMVHeli(vehicle))
	{
	    if (PlayerInfo[playerid][pFlyLic] != 0)	return SendErrorMessage(playerid, "Você já tem uma licença para operar veículo aéreo.");
		if (PlayerInfo[playerid][pFlyLic] == 2)	return SendErrorMessage(playerid, "Você está na lista negra como portador da licença de piloto.");
		if (GetCash(playerid) < FLYLIC_PRICE)	return SendNotCash(playerid);

		VehicleEngine(vehicle, true);
		SendSyntaxMessage(playerid, "Você iniciou o exame de licença de tráfego aéreo.");
		SendSyntaxMessage(playerid, "[1]: Tente não colidir com nada durante o voo.");
		SendSyntaxMessage(playerid, "[2]: Conduza a técnica com muito cuidado e sem pressa.");
		
		pTemp[playerid][pLicenseExam] = 2;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, fly_exam_cp[0], true);
		GiveCash(playerid, -FLYLIC_PRICE);
	}
	else if (IsADMVTaxi(vehicle))
	{
	    if (PlayerInfo[playerid][pCarLic] == 0)		return SendErrorMessage(playerid, "Para obter a licença de taxista, você deve ter uma carteira de motorista.");
        if (PlayerInfo[playerid][pTaxiLic] != 0)	return SendErrorMessage(playerid, "Você já tem uma licença de taxista.");
		if (PlayerInfo[playerid][pTaxiLic] == 2)	return SendErrorMessage(playerid, "Você está na lista negra como motorista de táxi.");
		if (GetCash(playerid) < TAXILIC_PRICE)		return SendNotCash(playerid);

		VehicleEngine(vehicle, true);
		SendSyntaxMessage(playerid, "Você iniciou o exame para a licença de motorista de táxi.");
		SendSyntaxMessage(playerid, "Você tem %i minutos para completar, caso contrário, você será reprovado no exame.", EXAM_TIME / 60);

		pTemp[playerid][pLicenseExam] = 3;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, license_exam_cp[0], true);
		GiveCash(playerid, -TAXILIC_PRICE);

		new bizid = LicenseOffice_Nearest(playerid);

		if (bizid != -1)	GiveBizCash(bizid, TAXILIC_PRICE);
	}
	return 1;
}
