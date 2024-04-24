forward LoadDynamicBanks();
forward LoadDynamicATMS();
forward OnATMInsert(AtmID);

public LoadDynamicATMS()
{
    new rows = cache_num_rows(), time = GetTickCount(), total;

	if (!rows)	return print("[ATM] Nenhum carregado.");
	 

	for(new i; i < rows; i++)
	{
	    ATMInfo[i][atmID] = cache_get_field_content_int(i, "id", dbHandle);
		ATMInfo[i][atmPosX] = cache_get_field_content_float(i, "posx", dbHandle);
		ATMInfo[i][atmPosY] = cache_get_field_content_float(i, "posy", dbHandle);
		ATMInfo[i][atmPosZ] = cache_get_field_content_float(i, "posz", dbHandle);
		ATMInfo[i][atmPosRZ] = cache_get_field_content_float(i, "posrz", dbHandle);
		ATMInfo[i][atmInt] = cache_get_field_content_int(i, "interior", dbHandle);
		ATMInfo[i][atmVW] = cache_get_field_content_int(i, "vw", dbHandle);
		ATMInfo[i][atmObject] = CreateDynamicObject(2942, ATMInfo[i][atmPosX], ATMInfo[i][atmPosY], ATMInfo[i][atmPosZ], 0.0, 0.0, ATMInfo[i][atmPosRZ], ATMInfo[i][atmVW], ATMInfo[i][atmInt]);
		total++;
	}
	printf("[ATM] Rows - %i. carregado - %i. tempo: %i ms.", rows, total, GetTickCount()-time);
	return 1;
}

public OnATMInsert(AtmID)
{
	return ATMInfo[AtmID][atmID] = cache_insert_id();
}

GiveBankCash(playerid, cash)
{
	return PlayerInfo[playerid][pBank] = PlayerInfo[playerid][pBank] + cash;
}

GetBankCash(playerid)
{
	return PlayerInfo[playerid][pBank];
}


BankTransfer(playerid, userid, amount)
{
    if (GetPVarInt(playerid, #just_transfer) > gettime())	return SendErrorMessage(playerid, "Aguarde 25 segundos.");
    if (userid == playerid)									return 1;
	if (!IsPlayerLogged(userid))							return SendNotConnected(playerid);
	if (GetBankCash(playerid) < amount || amount < 1)		return SendNotCash(playerid);

    new money1 = PlayerInfo[playerid][pBank], money2 = PlayerInfo[userid][pBank];

	GiveBankCash(playerid, -amount);
	GiveBankCash(userid, amount);
	SendClientMessageEx(playerid, COLOR_YELLOWG, "ATM: {FFFFFF}Você transferiu $%i para a conta de %s.", amount, GetNameEx(userid));
	SendClientMessageEx(userid, COLOR_YELLOWG, "ATM: {FFFFFF}%s transferiu $%i para a sua conta bancária.", GetNameEx(playerid), amount);

	new mes[256];
    format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Banco(ANTES): %i | Banco(ATUAL): %i] transferiu dinheiro do banco para %s [MYSQL: %i | IP: %s | Banco(ANTES): %i | Banco(ATUAL): %i] -- $%i",
	GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money1, PlayerInfo[playerid][pBank], GetName(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid), money2, PlayerInfo[userid][pBank], amount);
    insert_log(mes, MONEY_LOG, 2, PlayerInfo[playerid][pAccID], PlayerInfo[userid][pAccID]);
    
    SetPVarInt(playerid, #just_transfer, gettime() + 25);
    SendAdmMessage("%s [ID: %i] transferiu para a conta bancária de %s [ID: %i] - $%i", GetNameEx(playerid), playerid, GetNameEx(userid), userid, amount);
	return 1;
}


BankDeposit(playerid, amount)
{
    if (amount > GetCash(playerid) || amount < 1) return 1;
		
    new money = PlayerInfo[playerid][pCash], bank = PlayerInfo[playerid][pBank];

	GiveCash(playerid, -amount);
	SendClientMessageEx(playerid, COLOR_YELLOWG, "ATM: {FFFFFF}Você depositou $%i na conta bancária. Novo saldo: $%i.", amount, GetBankCash(playerid));

    new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(ATUAL): %i | Banco(ANTES): %i | Banco(ATUAL): %i] depósito -- $%i",
	GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), bank, PlayerInfo[playerid][pBank], amount);
    insert_log(mes, MONEY_LOG, 3, PlayerInfo[playerid][pAccID], 0);
	return 1;
}


BankWithdraw(playerid, amount)
{
    if (amount > GetBankCash(playerid) || amount < 1) return 1;
		
    new money = PlayerInfo[playerid][pCash], bank = PlayerInfo[playerid][pBank];
	
	GiveCash(playerid, amount);
	GiveBankCash(playerid, -amount);
	SendClientMessageEx(playerid, COLOR_YELLOWG, "ATM: {FFFFFF}Você retirou $%i da sua conta bancária. Novo saldo: $%i.", amount, GetBankCash(playerid));

    new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(ATUAL): %i | Banco(ANTES): %i | Banco(ATUAL): %i] retirou -- $%i",
	GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), bank, PlayerInfo[playerid][pBank], amount);
    insert_log(mes, MONEY_LOG, 4, PlayerInfo[playerid][pAccID], 0);
	return 1;
}


Bank_Nearest(playerid)
{
 	if (!IsPlayerLogged(playerid)) return 0;

	for(new i = 0; i < MAX_BIZ; i++)
	{
	    if (BizInfo[i][bizID] == 0 || BizInfo[i][bizType] != BIZ_BANK)	continue;

		if (PlayerToPoint(25.0,playerid,BizInfo[i][bizExitX], BizInfo[i][bizExitY], BizInfo[i][bizExitZ]) && GetWorld(playerid) == BizInfo[i][bizExitVW] && GetInterior(playerid) == BizInfo[i][bizExitInt])	return 1;
	}
	return 0;
}

AddATMToFile(atmid, Float:x, Float:y, Float:z, Float: rz, interior, vw)
{
    new query[128 + 128];
	mysql_format(dbHandle,query, sizeof(query), "INSERT INTO `atms` (posx, posy, posz, posrz, interior, vw) VALUES (%f, %f, %f, %f, %i, %i)", x, y, z, rz, interior, vw);
	mysql_tquery(dbHandle, query, "OnATMInsert", "d", atmid);
	return 1;
}

ATM_Nearest(playerid)
{
	for(new i; i != MAX_ATMS; i++)
	{
	    if (ATMInfo[i][atmID] == 0) continue;

        if (IsPlayerInRangeOfPoint(playerid, 2.0, ATMInfo[i][atmPosX], ATMInfo[i][atmPosY], ATMInfo[i][atmPosZ]) && ATMInfo[i][atmInt] == GetInterior(playerid) && ATMInfo[i][atmVW] == GetWorld(playerid)) return i;
	}
	return -1;
} //î