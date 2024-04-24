CMD:licenseexam(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!IsADMVCar(vehicle) && !IsADMVHeli(vehicle) && !IsADMVTaxi(vehicle)) return SendErrorMessage(playerid, "Voc� deve estar em um ve�culo do DMV.");
	if (!IsDriver(playerid))												 return SendErrorMessage(playerid, "Voc� deve estar dirigindo um ve�culo.");
    if (pTemp[playerid][pLicenseExam] != 0)									 return SendErrorMessage(playerid, "Voc� j� est� fazendo um dos exames.");
	        
	if (IsADMVCar(vehicle))
	{
        if (PlayerInfo[playerid][pCarLic] != 0)	return SendErrorMessage(playerid, "Voc� j� possui uma carteira de motorista.");
		if (GetCash(playerid) < CARLIC_PRICE)	return SendNotCash(playerid);

		VehicleEngine(vehicle, true); //ligar o motor
		SendSyntaxMessage(playerid, "Voc� iniciou o exame para a carteira de motorista.");
		SendSyntaxMessage(playerid, "Voc� tem %i minutos para completar, caso contr�rio, voc� ser� reprovado no exame.", EXAM_TIME / 60);
		
		pTemp[playerid][pLicenseExam] = 1;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, license_exam_cp[0], true);
		GiveCash(playerid, -CARLIC_PRICE);

		new bizid = LicenseOffice_Nearest(playerid);
		if (bizid != -1) GiveBizCash(bizid, CARLIC_PRICE);
	
		if (!IsPlayerInAnyVehicle(playerid)) {
			pTemp[playerid][pLicenseExam] = 0;
			DeletePVar(playerid, #exam_second);
			SendSyntaxMessage(playerid, "Voc� falhou no exame por ter sa�do do ve�culo.");
			TogglePlayerAllDynamicRaceCPs(playerid, false);
			RespawnVehicle(vehicle);
			return 1;
		}
	}
	else if (IsADMVHeli(vehicle))
	{
	    if (PlayerInfo[playerid][pFlyLic] != 0)	return SendErrorMessage(playerid, "Voc� j� tem uma licen�a para operar ve�culo a�reo.");
		if (PlayerInfo[playerid][pFlyLic] == 2)	return SendErrorMessage(playerid, "Voc� est� na lista negra como portador da licen�a de piloto.");
		if (GetCash(playerid) < FLYLIC_PRICE)	return SendNotCash(playerid);

		VehicleEngine(vehicle, true);
		SendSyntaxMessage(playerid, "Voc� iniciou o exame de licen�a de tr�fego a�reo.");
		SendSyntaxMessage(playerid, "[1]: Tente n�o colidir com nada durante o voo.");
		SendSyntaxMessage(playerid, "[2]: Conduza a t�cnica com muito cuidado e sem pressa.");
		
		pTemp[playerid][pLicenseExam] = 2;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, fly_exam_cp[0], true);
		GiveCash(playerid, -FLYLIC_PRICE);
	}
	else if (IsADMVTaxi(vehicle))
	{
	    if (PlayerInfo[playerid][pCarLic] == 0)		return SendErrorMessage(playerid, "Para obter a licen�a de taxista, voc� deve ter uma carteira de motorista.");
        if (PlayerInfo[playerid][pTaxiLic] != 0)	return SendErrorMessage(playerid, "Voc� j� tem uma licen�a de taxista.");
		if (PlayerInfo[playerid][pTaxiLic] == 2)	return SendErrorMessage(playerid, "Voc� est� na lista negra como motorista de t�xi.");
		if (GetCash(playerid) < TAXILIC_PRICE)		return SendNotCash(playerid);

		VehicleEngine(vehicle, true);
		SendSyntaxMessage(playerid, "Voc� iniciou o exame para a licen�a de motorista de t�xi.");
		SendSyntaxMessage(playerid, "Voc� tem %i minutos para completar, caso contr�rio, voc� ser� reprovado no exame.", EXAM_TIME / 60);

		pTemp[playerid][pLicenseExam] = 3;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, license_exam_cp[0], true);
		GiveCash(playerid, -TAXILIC_PRICE);

		new bizid = LicenseOffice_Nearest(playerid);

		if (bizid != -1)	GiveBizCash(bizid, TAXILIC_PRICE);
	}
	return 1;
}