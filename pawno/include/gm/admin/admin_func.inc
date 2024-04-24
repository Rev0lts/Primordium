this::OfflineBlockPlayer(playerid, account[], reason[])
{
    if (cache_num_rows() == 0)
        return SendErrorMessage(playerid, "Personagem \"%s\" não foi encontrado no banco de dados.", account);

    new status = cache_get_field_content_int(0, "status");

    if (status == 3) return SendErrorMessage(playerid, "O personagem \"%s\" já está bloqueado.", account);
    if (status != 1) return SendErrorMessage(playerid, "O personagem \"%s\" deve ser ativado.", account);

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `block_name`='%e', `block_reason`='%e', `status`='3' WHERE `name`='%e'", GetAccName(playerid), reason, account);
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "AdmCmd: O personagem %s (offline) foi bloqueado pelo administrador %s. Motivo: %s", account, GetAccName(playerid), reason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, query);
    return 1;
}


this::UnBlockPlayer(playerid, account[])
{
    if (cache_num_rows() == 0) return SendErrorMessage(playerid, "O personagem \"%s\" não foi encontrado no banco de dados.", account);
    if (cache_get_field_content_int(0, "status") != 3) return SendErrorMessage(playerid, "O personagem \"%s\" não está bloqueado.", account);

    SendAdmMessage("%s foi desbloqueado pelo administrador %s.", account, GetAccName(playerid));

    new query[128];

    mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `status`='1' WHERE `name`='%e'", account);
    mysql_tquery(dbHandle, query);
    return 1;
}


this::OfflineBanPlayer(playerid, account[], time, reason[])
{
    if (cache_num_rows() == 0) return SendErrorMessage(playerid, "O personagem \"%s\" não foi encontrado no banco de dados.", account);
    if (cache_get_field_content_int(0, "banid") != 0) return SendErrorMessage(playerid, "A conta \"%s\" já está banida.", account);

    new admin = cache_get_field_content_int(0, "admin");
    new accountid = cache_get_field_content_int(0, "accountid");

    if (admin > PlayerInfo[playerid][pAdmin]) return SendAdmMessage("%s [ID: %i] tentou banir (offline) %s com o motivo: %s.", GetAccName(playerid), account, reason);
    new query[256];
    mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (accountid,name,admin,reason,date) VALUES (%i,'%e','%s','%e','%s')", accountid, account, GetAccName(playerid), reason, GetFullDate());
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "[/offban] %s foi banido por %i dias pelo administrador %s. O motivo: %s", account, time, GetAccName(playerid), reason);
    insert_log(query, ADMIN_LOG, 1, accountid, PlayerInfo[playerid][pAccID]);

    format(query, sizeof(query), "AdmCmd: %s (offline) foi bloqueado por %i dias pelo administrador %s. Motivo: %s", time, account, GetAccName(playerid), reason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, query);
    return 1;
}


this::UnBanPlayer(playerid, account[])
{
    if (!cache_num_rows()) return SendErrorMessage(playerid, "A conta \"%s\" não está bloqueada.", account);
    SendAdmMessage("%s foi desbloqueada pelo administrador %s.", account, GetAccName(playerid));

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `bans` WHERE `name` = '%e'", account);
    mysql_tquery(dbHandle, query);

    mysql_format(dbHandle, query, sizeof(query), "SELECT accountid FROM `bans` WHERE `name` = '%e' LIMIT 1", account);
    new Cache:cache = mysql_query(dbHandle, query),
    accountid = cache_get_field_content_int(0, "accountid", dbHandle);

    cache_delete(cache);

    format(query, sizeof(query), "[/unban] %s foi desbloqueada pelo administrador %s", account, GetAccName(playerid));
    insert_log(query, ADMIN_LOG, 12, accountid, PlayerInfo[playerid][pAccID]);
    return 1;
}


this::UnBanAnIP(playerid, ip[])
{
    if (!cache_num_rows()) return SendErrorMessage(playerid, "IP \"%s\" não está bloqueado.", ip);

    SendAdmMessage("%s desbloqueou o IP %s.", GetAccName(playerid), ip);

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `bans` WHERE `ip` = '%e'", ip);
    mysql_tquery(dbHandle, query);

    mysql_format(dbHandle, query, sizeof(query), "SELECT accountid FROM `bans` WHERE `ip` = '%e' LIMIT 1", ip);
    new Cache:cache = mysql_query(dbHandle, query),
    accountid = cache_get_field_content_int(0, "accountid", dbHandle);

    cache_delete(cache);

    new mes[128];
    format(mes, sizeof(mes), "[/unbanip] IP: %s foi desbloqueado pelo administrador %s", ip, GetAccName(playerid));
    insert_log(query, ADMIN_LOG, 12, accountid, PlayerInfo[playerid][pAccID]);
    return 1;
}

this::OnChangeName(playerid, userid, newname[])
{
    if (cache_num_rows()) return SendErrorMessage(playerid, "O nome do personagem \"%s\" já foi utilizado.", newname);

    foreach(new i : Player)
    {
        if (!IsPlayerLogged(i) || !CompareStrings(GetName(i), newname)) continue;

        SendErrorMessage(playerid, "O nome do personagem \"%s\" já foi utilizado.", newname);
        return 1;
    }

    if (!PlayerInfo[userid][pNameChanges]) return SendErrorMessage(playerid, "O jogador não possui privilégios.");

    PlayerInfo[userid][pNameChanges]--;
    SQL_SetString("accounts", "name_changes", PlayerInfo[userid][pNameChanges], PlayerInfo[userid][pAccID]);

    new oldname[MAX_PLAYER_NAME];
    GetPlayerName(userid, oldname, MAX_PLAYER_NAME);

    ChangeName(userid, newname);
    SendAdmMessage("%s confirmou a mudança de nome do jogador de %s para %s.", GetAccName(playerid), oldname, newname);
    SendSyntaxMessage(userid, "Seu nome foi alterado de %s para %s pelo administrador %s.", oldname, newname, GetAccName(playerid));
    return 1;
}

this::OnAdminChangeName(playerid, userid, newname[])
{
    if (cache_num_rows()) return SendErrorMessage(playerid, "O nome do personagem \"%s\" já foi utilizado.", newname);

    foreach(new i : Player)
    {
        if (!IsPlayerLogged(i) || !CompareStrings(GetName(i), newname)) continue;
        SendErrorMessage(playerid, "O nome do personagem \"%s\" já foi utilizado.", newname);
        return 1;
    }

    new oldname[MAX_PLAYER_NAME];
    GetPlayerName(userid, oldname, MAX_PLAYER_NAME);

    ChangeName(userid, newname);
    SendAdmMessage("%s alterou o nome do jogador de %s para %s.", GetAccName(playerid), oldname, newname);
    SendSyntaxMessage(userid, "Seu nome foi alterado de %s para %s pelo administrador %s.", oldname, newname, GetAccName(playerid));
    return 1;
}


ChangeName(userid, newname[])
{
    new oldname[MAX_PLAYER_NAME];
	GetPlayerName(userid, oldname, MAX_PLAYER_NAME);

	format(PlayerInfo[userid][pUsername], MAX_PLAYER_NAME+1, newname);
    format(PlayerInfo[userid][pUsername_], MAX_PLAYER_NAME+1, newname);
    strreplace(PlayerInfo[userid][pUsername_], '_', ' ');

    if (!maskOn{userid}) 
	{
	    SetPlayerName(userid, newname);
    	format(PlayerInfo[userid][pMaskname], 32, PlayerInfo[userid][pUsername_]);
	}
	
	for(new i; i != MAX_HOUSES; i++)
	{
	    if (!HouseInfo[i][hID] || HouseInfo[i][hOwnerID] != PlayerInfo[userid][pID]) continue;

		format(HouseInfo[i][hOwner], MAX_PLAYER_NAME, GetNameEx(userid));
	}

	for(new i; i != MAX_BIZ; i++)
	{
	    if (!BizInfo[i][bizID] || BizInfo[i][bizOwnerID] != PlayerInfo[userid][pID]) continue;

		format(BizInfo[i][bizOwner], MAX_PLAYER_NAME, GetName(userid));
	}
	
	foreach(new i:Vehicle)
	{
		if (VehicleInfo[i][carOwnerID] != PlayerInfo[userid][pID]) continue;

		format(VehicleInfo[i][carOwner], MAX_PLAYER_NAME, GetNameEx(userid));
	}
	
	SQL_SetString("users", "name", newname, PlayerInfo[userid][pID]);
	
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `tickets` WHERE `userid`=%i", PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query);
	
	DeletePlayerTickets(userid);
	return 1;
}

this::AdminSetPlate(playerid, vehicleid, plate[])
{
    if (cache_num_rows()) return SendErrorMessage(playerid, "A placa de licença já está em uso por outro veículo.");

    format(VehicleInfo[vehicleid][carPlate], 24, "%s", plate);
    SetVehicleNumberPlate(vehicleid, VehicleInfo[vehicleid][carPlate]);
    RespawnVehicle(vehicleid, 1);
    Save_Car(vehicleid);

    SendSyntaxMessage(playerid, "Você alterou a placa de número de %s. Novo número: %s.", VehicleNames[GetVehicleModel(vehicleid)-400], plate);

    SAMALL("%s alterou a placa de licença do veículo (%s) ID %i para: %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(vehicleid)-400], vehicleid, plate);

    new mes[128];
    format(mes, sizeof(mes), "[/asetplate] O administrador %s alterou a placa de licença do veículo (%s) para: %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(vehicleid)-400], plate);
    insert_log(mes, ADMIN_LOG, 6, 0, PlayerInfo[playerid][pAccID]);
    return 1;
}


this::OnPlayerOfflineJail(playerid, name[32], time, reason[32])
{
    new rows, fields;

    cache_get_data(rows, fields, dbHandle);

    if (cache_affected_rows(dbHandle))
    {
        new prefix[16] = "test";
        if (PlayerInfo[playerid][pAdmin] > 0) prefix = "administrador";

        new query[128];
        mysql_format(dbHandle, query, sizeof(query), "SELECT accountid FROM `bans` WHERE `name` = '%e' LIMIT 1", name);
        new Cache:cache = mysql_query(dbHandle, query),
            accountid = cache_get_field_content_int(0, "accountid", dbHandle);

        cache_delete(cache);

        new mes[128];
        format(mes, sizeof(mes), "AdmCmd: %s enviou para a prisão de administrador %s %s por %i minuto(s), motivo: %s", name, prefix, GetAccName(playerid), time, reason);
        SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

        format(mes, sizeof(mes), "[/offjail] %s enviado para a prisão de administrador %s %s por %i minuto(s), motivo: %s", name, prefix, GetAccName(playerid), time, reason);
        insert_log(mes, ADMIN_LOG, 51, accountid, PlayerInfo[playerid][pAccID]);
    }
    else
    {
        SendErrorMessage(playerid, "Você falhou em enviar o jogador %s para a prisão de administrador.", name);
    }
    return 1;
}


this::BanAnIP(playerid, ip[], reason[])
{
    if (cache_num_rows()) return SendErrorMessage(playerid, "IP \"%s\" já está bloqueado.", ip);

    SendAdmMessage("%s bloqueou o endereço de IP: %s.", GetAccName(playerid), ip);

    new pIP[24];
    foreach(new i : Player)
    {
        GetPlayerIp(i, pIP, sizeof(pIP));

        if (!IsPlayerLogged(i) || !CompareStrings(ip, pIP)) continue;

        SendErrorMessage(i, "Seu IP %s foi bloqueado neste servidor.");
        KickEx(i);
    }

    new query[256];
    mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (admin, reason, date, ip) VALUES ('%s', '%e', '%s', '%e')", GetAccName(playerid), reason, GetFullDate(), ip);
    mysql_tquery(dbHandle, query);
    return 1;
}

ClearKillFeed(playerid = INVALID_PLAYER_ID)
{
	if ((playerid != INVALID_PLAYER_ID) && (0 == IsPlayerConnected(playerid))) return 0;

	goto L_start;
	{
		new dummy[16/(cellbits/charbits)];
		#emit const.pri dummy
	}

	#if __Pawn < 0x030A
		SendDeathMessage(0, 0, 0),
		SendDeathMessageToPlayer(0, 0, 0, 0);
	#endif

	L_start:

	const SDM_MAGIC_ID = INVALID_PLAYER_ID-1;
	new i = 5;

	#emit    push.c    SDM_MAGIC_ID
	#emit    push.c    SDM_MAGIC_ID

	if (playerid == INVALID_PLAYER_ID)
	{
		#emit    push.c    12
		do
		{
			#emit    sysreq.c    SendDeathMessage
		}
		while(--i != 0);

		#emit    stack    12
	}
	else
	{
		#emit    push.s    playerid
		#emit    push.c    16
		do
		{	
			#emit    sysreq.c    SendDeathMessageToPlayer
		}
		while(--i != 0);

		#emit    stack    16
	}
	return 1;
}

SendClientMessageByAdmin(color, string[])
{
    foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || (PlayerInfo[i][pSettings] & togAdmCmds)) continue;

		SendClientMessage(i, color, string);
	}
	return 1;
}


AMSG(color, const str[], {Float,_}:...)
{
    static args, start, end, string[144];
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 8)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 8); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit ADD.C 4
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1)) continue;

			SendClientMessage(i, color, string);
		}
		return 1;
	}

	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1)) continue;

		SendClientMessage(i, color, str);
	}
	return 1;
}

StaffMSG(color, const str[], {Float,_}:...)
{
    static args, start, end, string[144];

	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 8)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 8); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}

		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit ADD.C 4
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i))) continue;
		    SendClientMessage(i, color, string);
		}
		return 1;
	}
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i))) continue;
	    SendClientMessage(i, color, str);
	}
	return 1;
}

CheckAdmin(playerid, level)
{
	return PlayerInfo[playerid][pAdmin] >= level && IsAdminDuty(playerid) ? 1 : 0;
}

IsTester(playerid)
{
	return PlayerInfo[playerid][pHelper] >= 1 && IsTesterDuty(playerid) ? 1 : 0;
}

IsAdminDuty(playerid)
{
	return pTemp[playerid][pAdminDuty] == 1 ? 1 : 0;
}

IsTesterDuty(playerid)
{
	return pTemp[playerid][pTesterDuty] == 1 ? 1 : 0;
}

SendStaffMessage(color, string[])
{
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || (!IsTester(i) && !CheckAdmin(i, ADMIN_LEVEL_1)) || (PlayerInfo[i][pSettings] & togTester)) continue;

        SCM(i, color, string);
	}
	return 1;
}

SetAdminCode(playerid, adm_code[])
{
    new hashed_code[65];

    SHA256_PassHash(adm_code, ADMIN_HASH, hashed_code, MAX_HASH_LENGTH);
    SQL_SetString("accounts", "admin_code", hashed_code, PlayerInfo[playerid][pAccID]);

    SendAdmMessageToPlayer(playerid, "Sua senha do Painel de Administração foi alterada. Nova Senha: %s.", adm_code);
    SendHelpMessage(playerid, "Lembre-se dela ou anote-a!");

    format(PlayerInfo[playerid][pAdminCode], MAX_HASH_LENGTH, "%s", hashed_code);
    return 1;
}


ResetAdminCode(playerid)
{
	DeletePVar(playerid, "Admin:CodeInputted");
	format(PlayerInfo[playerid][pAdminCode], 65, "");
	SQL_SetString("accounts", "admin_code", "", PlayerInfo[playerid][pAccID]);
	SendSyntaxMessage(playerid, "Sua senha administrativa foi redefinida.");
	return 1;
}

VerifyAdminCode(playerid, adm_code[])
{
	new hashed_password[65];

	SHA256_PassHash(adm_code, ADMIN_HASH, hashed_password, MAX_HASH_LENGTH);

	if (CompareStrings(hashed_password, PlayerInfo[playerid][pAdminCode]))
	{
		return 1;
	}
	return 0;
}