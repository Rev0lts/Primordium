CMD:maca(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionMedic(faction) && !IsFactionFire(faction))	return SendErrorMessage(playerid,"Você precisa ser um paramédico ou bombeiro.");
	if (IsPlayerInAnyVehicle(playerid))						return SendErrorMessage(playerid, "Você não pode estar em um veículo.");

	if (pTemp[playerid][UsingBort]) {
		new idx = pTemp[playerid][UsingBort]-1, modelid = GetDynamicObjectModel(BortInfo[playerid][idx][bortObject]);

		DestroyDynamicObject(BortInfo[playerid][idx][bortObject]);

		new Float:x, Float:y, Float:z, Float:R;
		GetPlayerFacingAngle(playerid, R);
	  	GetXYInFrontOfPlayer(playerid, x, y, z, 1.5);
		BortInfo[playerid][idx][bortObject] = CreateDynamicObject(modelid, x, y, z - 0.5, 0.0, 0.0, 0.0, GetWorld(playerid), GetInterior(playerid));
		
		if (pTemp[playerid][UsingCorpse]) {
			new actorid = pTemp[playerid][UsingCorpse];
			SetActorPos(actorid, x, y, z + 0.60);
			SetActorFacingAngle(actorid, R);
			pTemp[playerid][UsingCorpse] = 0;
		}

		BortInfo[playerid][idx][bortUsing] = 0;
		pTemp[playerid][UsingBort] = 0;

		SendMedicMessage(playerid, "Você largou a maca.");
		return 1;
	}

    if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Primeiro você precisa estar de serviço.");

	new idx = -1;
	for(new i = 0; i < MAX_BORTES; i++)
	{
	    if (!BortInfo[playerid][i][bortOn]) continue;
		if (BortInfo[playerid][i][bortUsing] != 0) continue;

		new Float: x, Float: y, Float: z;
		GetDynamicObjectPos(BortInfo[playerid][i][bortObject], x, y, z);

		if (GetPlayerDistanceFromPoint(playerid, x, y, z) > 1.5) continue;

		new modelid = GetDynamicObjectModel(BortInfo[playerid][i][bortObject]);
		if (modelid != 1997 && modelid != 2146) continue;

		idx = i;
		break;
	}
	
	if (idx == -1) return SendErrorMessage(playerid, "Não há macas próximas! (/maca) ");

	BortInfo[playerid][idx][bortUsing] = playerid+1;
	pTemp[playerid][UsingBort] = idx+1;
	pTemp[playerid][UsingCorpse] = 0;

	AttachDynamicObjectToPlayer(BortInfo[playerid][idx][bortObject], playerid, 0.00, 1.50, -0.50, 0.0, 0.0, 0.0);

    SendMedicMessage(playerid, "Você pegou uma maca.");
	return 1;
}

CMD:terapia(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], userid;

	if (!IsFactionMedic(faction))					return SendErrorMessage(playerid, "Você deve ser um paramédico.");
	if (PlayerInfo[playerid][pRank] < 3)				return SendErrorMessage(playerid, "Disponível a partir do rank 3.");
	if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Primeiro, você precisa entrar de serviço.");
	if (sscanf(params, "u", userid))					return SendHelpMessage(playerid, "/terapia [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)							return SendErrorMessage(playerid, "ID inválido.");
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pDependence] == 0)		return SendErrorMessage(playerid, "O jogador não tem vício em drogas. ");

    PlayerInfo[userid][pDependence] -= (PlayerInfo[userid][pDependence] < 20) ? (PlayerInfo[userid][pDependence]) : (20);
    pTemp[userid][pDemolitionTime] = 0;
	SetPlayerCurrentTimeAndWeather(userid);
	
	SendMedicMessage(playerid, "%s %s reduziu o seu vício em 20 pontos.", GetRankName(playerid), GetNameWithMask(playerid));
    SendMedicMessage(playerid, "Você reduziu o vício de %s em 20 pontos.", GetNameWithMask(userid));

	return 1;
}

CMD:receitar(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], userid, days;

	if (!IsFactionMedic(faction))					return SendErrorMessage(playerid, "Você deve ser um paramédico.");
	if (PlayerInfo[playerid][pRank] < 3)				return SendErrorMessage(playerid, "Disponível a partir do rank 3.");
	if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Primeiro, você precisa entrar de serviço. ");	
	if (sscanf(params,"uI(0)",userid, days))			return SendHelpMessage(playerid, "/receitar [ID/Parte do Nome] [número de dias 1-93]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)							return SendErrorMessage(playerid, "ID inválido.");
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);	
	if (PlayerInfo[userid][pPrescribe] < gettime())
	{
	    if (days < 1 || days > 93)					return SendErrorMessage(playerid, "Data de validade da receita inválida. ");
	        
	    PlayerInfo[playerid][pPrescribe] = gettime() + days * 86400;
	    SendMedicMessage(userid, "%s %s lhe deu uma receita para maconha medicinal.", GetRankName(playerid), GetNameWithMask(playerid));
		SendHelpMessage(userid, "Use /mostrareceita para mostrar a receita.");
        SendMedicMessage(playerid, "Você emitiu uma receita para maconha medicinal para %s. ", GetNameWithMask(userid));
	}
	else
	{
	    PlayerInfo[userid][pPrescribe] = 0;
	    SendMedicMessage(userid, "%s %s retirou a sua receita para maconha medicinal.", GetRankName(playerid), GetNameWithMask(playerid));
        SendMedicMessage(playerid, "Você retirou de %s uma receita para maconha medicinal.", GetNameWithMask(userid));
	}
	return 1;
}


CMD:operacao(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], userid;

	if (!IsFactionMedic(faction) && !IsFactionFire(faction))	return SendErrorMessage(playerid, "Você deve ser um paramédico ou um bombeiro.");
	if (!PlayerInfo[playerid][pOnDuty])					return SendErrorMessage(playerid, "Primeiro, você precisa entrar de serviço. ");
	if (sscanf(params, "u", userid))					return SendHelpMessage(playerid, "/operacao [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)								return SendErrorMessage(playerid, "É impossível se curar. ");
	if (!PlayerNearPlayer(3.0, playerid, userid))		return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pInjured] != 1)				return SendErrorMessage(playerid, "O jogador não está ferido. ");
	if (pTemp[userid][pInvulnerable] != 0.0)			return SendErrorMessage(playerid, "Tente mais tarde!");
    ResetDamages(userid);
    SetPlayerHealth(userid, 100.0);
	StopPlayerAnims(userid);
	UnFreezePlayer(userid);

	SendMedicMessage(playerid, "Você tirou %s da fase de ferimento.", GetNameWithMask(userid));
	SendMedicMessage(userid, "%s %s te tirou da fase de ferimento.", GetRankName(playerid), GetNameWithMask(playerid));
	return 1;
}

CMD:informacaodepaciente(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], id, vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionMedic(faction) && !IsFactionFire(faction))	return SendErrorMessage(playerid, "Você deve ser um paramédico ou um bombeiro.");
	if (sscanf(params,"u",id))							return SendHelpMessage(playerid, "/informacaodepaciente [ID/Parte do Nome]");
	if (!IsPlayerLogged(id))								return SendNotConnected(playerid);
	if (VehicleInfo[vehicle][carFaction] != faction)		return SendErrorMessage(playerid, "Você deve estar no veículo de sua organização.");

	SendMedicMessage(playerid, "Informação - %s", GetFullDate());
	SendMedicMessage(playerid, "Nome completo: %s, Data de Nascimento: %s, Telefone: %i.", GetNameEx(id), PlayerInfo[id][pBirthdate], PlayerInfo[id][pNumber]);

	new count;
    for(new i; i < MAX_HOUSES; i++)
	{
	    if (HouseInfo[i][hID] == 0 || HouseInfo[i][hOwnerID] != PlayerInfo[id][pID])	continue;

		SendMedicMessage(playerid, "Endereço de registro: %s.", (!HouseInfo[i][hApart])?(GetHouseZone(i)):(GetApartmentZone(HouseInfo[i][hApart])));
		count++;
		if (count > 4) break;
	}

	if (!count) SendMedicMessage(playerid, "Endereço de registro desconhecido.");

	return 1;
}

CMD:apagarfogo(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionFire(faction)) return SendErrorMessage(playerid, "Você deve ser um bombeiro.");
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro você precisa entrar de serviço.");
	if (DeleteNearFire(playerid)) return SM1(playerid, "Você apagou o fogo.");

	SM2(playerid, "Não há fogo perto de você!");
	return 1;
}

CMD:uniformebombeiro(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionFire(faction)) return SendErrorMessage(playerid, "Você deve ser um bombeiro.");
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro você precisa entrar de serviço.");
	if (!PlayerInfo[playerid][pChar]) return SendErrorMessage(playerid, "Você deve estar em uniforme oficial.");

	if (GetPlayerSkin(playerid) == PlayerInfo[playerid][pChar])
	{
		if (PlayerInfo[playerid][pChar] == 277 || PlayerInfo[playerid][pChar] == 278 
		|| PlayerInfo[playerid][pChar] == 279 || PlayerInfo[playerid][pChar] == 191) return SendErrorMessage(playerid, "Você já está usando o uniforme de bombeiro!");
		Dialog_Show(playerid, MODEL_SELECTION_SHAPE, DIALOG_STYLE_PREVIEW_MODEL, "Seleção de uniforme", "277\tID: 277\n278\tID: 278\n279\tID: 279\n191\tID: 191", "Selecionar", "Fechar");
	} else {
		SetPlayerSkin(playerid, PlayerInfo[playerid][pChar]);
	}
	return 1;
}
