ALTX:damages("/dmg","/danos");
CMD:damages(playerid, params[])
{
    new userid;

    if (!IsPlayerLogged(playerid))                                                 return SendNotConnected(playerid);
	if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/danos(/dmg) [ID/Parte do Nome]");
	if (!IsPlayerConnected(userid)) return SendErrorMessage(playerid, "O jogador está offline.");
	if (!PlayerNearPlayer(3.0, playerid, userid) && !CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotNearPlayer(playerid);

    pTemp[playerid][pDialogList] = 0;

    DisplayDamages(playerid, userid);
    return 1;
}

ALTX:helpup("/ajudalevantar");
CMD:helpup(playerid, params[])
{
    new userid;

    if (!IsPlayerLogged(playerid))                 return SendNotConnected(playerid);
    if (sscanf(params, "u", userid))               return SendHelpMessage(playerid, "/ajudalevantar [ID/Parte do Nome]");
    if (userid == playerid)                        return 1;
    if (!IsPlayerConnected(userid))                return SendErrorMessage(playerid, "O jogador está offline.");
    if (!PlayerNearPlayer(2.0, playerid, userid))  return SendNotNearPlayer(playerid);
    if (pTemp[userid][pInjuredTime]-40 > 0)        return SendErrorMessage(playerid, "Você deve esperar mais %i segundos!", pTemp[userid][pInjuredTime]-40);

    StartProgress(userid, playerid, 1, 839);
    return 1;
}


CMD:aceitarmorte(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] == 0) return SendErrorMessage(playerid, "Você não está ferido.");

    new count = 0;
    foreach (new i : Player)
    {
        if (i == playerid) continue;
        if (GetPlayerDistanceFromPlayer(playerid, i) < 11.0) count++;
    }

    if (pTemp[playerid][pInjuredTime])
    {
        if (count) {
            SendWarningMessage(playerid, "Você não pode aceitar a morte imediatamente se houver jogadores por perto. Você precisa esperar um pouco mais. %i segundos.", pTemp[playerid][pInjuredTime]);
            return 1;
        }
    }

    SendSpecialMessage(playerid, "Seu personagem morreu, mas você deve esperar 60 segundos para renascer \"/renascer\".");
    PlayerInfo[playerid][pInjured] = 2;
    pTemp[playerid][pInjuredTime] = 60;
    return 1;
}


CMD:renascer(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 2) return SendErrorMessage(playerid, "Você não está morto.");
	if (pTemp[playerid][pInjuredTime]) 		return SendSpecialMessage(playerid, "Você acabou de esperar %i segundos. Você deve esperar pelo menos 60 segundos para usar /renascer.", 60 - pTemp[playerid][pInjuredTime]);

    if (PlayerInfo[playerid][pAJailed] != 0)
	{
		SetInterior(playerid, 18);
		SetWorld(playerid, playerid + 100);
		SetPlayerPos(playerid, -219.4580,1410.6747,27.7734);
		SendClientMessageEx(playerid, COLOR_WHITE,"Você foi devolvido à prisão de administração. Prazo: %i minutos.", PlayerInfo[playerid][pAJailTime]/60);
	}
	else if (PlayerInfo[playerid][pJailed])
	{
	    PutPlayerInPrison(playerid);
	}
	else
	{
		CreateCorpse(playerid, PlayerInfo[playerid][pDeathReason]);

    	SetPlayerPos(playerid, 1242.1882, 328.0487, 19.7555);
		SetPlayerFacingAngle(playerid, 270.0);
		SetInterior(playerid, 0);
		SetWorld(playerid, 0);
		
		SM2(playerid, "Você foi hospitalizado.");
		
		new 
			bill,
			tax,
			year,
			month,
			day
		;
		
	    getdate(year, month, day);
	    
	    bill = 100, tax = 15;

		SendClientMessageEx(playerid,0xFF8282AA,"**********************************************");
        SendClientMessageEx(playerid,0xFF8282AA,"			CONTA HOSPITALAR:");
        SendClientMessageEx(playerid,0xFF8282AA,"NOME: %s - DATA: %d-%d-%d.", GetNameEx(playerid), day, month, year);
        SendClientMessageEx(playerid,0xFF8282AA,"SERVIÇOS: $%d - IMPOSTO: $%d.", bill, tax);
		bill+=tax;
        SendClientMessageEx(playerid,0xFF8282AA,"TOTAL: $%d.", bill);
        SendClientMessageEx(playerid,0xFF8282AA,"**********************************************");
	}
	
	PlayerInfo[playerid][pDeathReason] = 0;
	SQL_SetInteger("users", "deathreason", 0, PlayerInfo[playerid][pID]);
	
	if (!PlayerInfo[playerid][pPremium]) SetPlayerHealth(playerid, 50.0);
	else							  	SetPlayerHealth(playerid, 100.0);

	ResetDamages(playerid);
	StopPlayerAnims(playerid);
	UnFreezePlayer(playerid);

	SetPlayerSkill(playerid);
	return 1;
}
