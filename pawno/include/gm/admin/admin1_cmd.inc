ALTX:ah("/ahelp");
CMD:ah(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] == 0) return SendNotAdmin(playerid);

	if (CheckAdmin(playerid, ADMIN_LEVEL_1))
	{
		SCM(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/aduty /adminchat /an /aooc /announce /aame /setinterior(/setint) /setworld(/vw) /masked");
		SCM(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/kick /skick /ajail /afree /warn /offban /ban /unban /banip /unbanip /mute /unmute /checkcars");
		SCM(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/clearchat /atp /tp /gethere /goto /freeze /slap /getip /getstats(/astats) /getproperty ");
		SCM(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/removevehicle /gotocar /getcar /spec /specoff /disarm /togservice /del(ete)ad(vertisment)");
		SCM(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/plantinfo /removeplant /aprison /dmlog /near /removebox /check /fixveh /d(elete)pame");
		SCM(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/delaction /healplayer(/aheal) /forbid /unforbid (/agr)eegraffiti /gotopc(-ls/-fc/-d/-m)");
	}
	if (CheckAdmin(playerid, ADMIN_LEVEL_2))
	{
		SCM(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/up /policer /down /getincar /afire /aengine /fillveh /fillallveh /rtc /rtcall");
		SCM(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/setskin /sethp /setarmour /showpm /showpmoff /showpms /nopms /telecmds /aveh /mark");
		SCM(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/aunimpound /delplayertickets /delvehtickets /gotomark /settime /weather /weatherall");
		SCM(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/go /govw /goint /x /y /z /noooc ");
	}
	if (CheckAdmin(playerid, ADMIN_LEVEL_3))
	{
		SCM(playerid, COLOR_WHITE, "[Level 3]: {C3C3C3}/itemlist /alock /giveitem /givegunex /getgun /changename /acceptname /chopshop /apayphone(/app)");
		SCM(playerid, COLOR_WHITE, "[Level 3]: {C3C3C3}/afaction /avehicle /agarage /ahouse /abiz /aapart /aparkmeter(/aparkm) /adoor /abillboard(/abb)");
	}
	if (CheckAdmin(playerid, HIGH_ADMIN_LEVEL))
	{
		SCM(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/admin /makepremium /tester /changefight /copstocrime /firetocrime");
		SCM(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/asetplate /givemoney /adminhide /setstat /astreet");
		SCM(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/aobject /afood(stand) /alabel /as(peed)c(am) /resetcode");
		SCM(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/aatm /atower /acctv /agate /ajob /gmx /reloadmap /streamer");
	}
	if (CheckAdmin(playerid, DEVELOPER_LEVEL))
	{
		SCM(playerid, COLOR_WHITE, "[DEVELOPER CMD]: {C3C3C3}/payday /paydayall /aindustry /anticheat_settings");
	}
	return 1;
}

CMD:aduty(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] < 1) return SendNotAdmin(playerid);

	if (IsAdminDuty(playerid))
	{
		for(new i = 0; i<52; i++) EnableAntiCheatForPlayer(playerid, i, 1); 

		SendAdmMessage("%s saiu de serviço /aduty.", GetAccName(playerid));
		TextDrawHideForPlayer(playerid, Admin);
		SetPlayerHealth(playerid, pTemp[playerid][pInvulnerable]);
		pTemp[playerid][pInvulnerable] = 0.0;
		pTemp[playerid][pAdminDuty] = 0;
		SetPlayerColor(playerid, TEAM_HIT_COLOR);
		
		if (pTemp[playerid][pDMLog] == 1)
		{
			ClearKillFeed(playerid);
			pTemp[playerid][pDMLog] = 0;
		}
		server_admins--;
		return 1;
	}

	else
	{
		if (!GetPVarInt(playerid, "Admin:CodeInputted"))
		{
			new acode[32];

			if (sscanf(params, "s[24]", acode)) return SendHelpMessage(playerid, "/aduty [senha]");
			if (isempty(PlayerInfo[playerid][pAdminCode])) return SendErrorMessage(playerid, "Você deve definir uma senha para entrar em modo de administração. Use o comando /setcode.");
			if (strlen(acode) < 4 || strlen(acode) > 16) return SendErrorMessage(playerid, "A senha deve ter entre 4 e 16 caracteres.");
			if (!VerifyAdminCode(playerid, acode)) return SendErrorMessage(playerid, "Senha inserida incorretamente.");

		}
		
		SetPVarInt(playerid, "Admin:CodeInputted", 1);
		pTemp[playerid][pAdminDuty] = 1;
		pTemp[playerid][pDMLog] = 1;
		SendAdmMessage("%s entrou em serviço /aduty.", GetAccName(playerid));
		TextDrawShowForPlayer(playerid, Admin);
		pTemp[playerid][pInvulnerable] = GetPlayerHealthEx(playerid);
		SetPlayerHealth(playerid, Float:0x7F800000);

		SetPlayerColor(playerid,TEAM_ADMIN_COLOR);
		server_admins++;

		for(new i = 0; i<52; i++) EnableAntiCheatForPlayer(playerid, i, 1); 
	}
	return 1;
}

CMD:setcode(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] < 1)	return SendNotAdmin(playerid);

	new new_acode[32], old_acode[32];

	if (isempty(PlayerInfo[playerid][pAdminCode]))
	{
		if (sscanf(params, "s[24]", new_acode)) return SendHelpMessage(playerid, "/setcode [Nova Senha]");
		if (strlen(new_acode) < 4 || strlen(new_acode) > 16) return SendErrorMessage(playerid, "A senha deve conter entre 4 e 16 caracteres.");


		SetAdminCode(playerid, new_acode);
		return 1;
	}
	else
	{
		if (sscanf(params, "s[24]s[24]", old_acode, new_acode))	return SendSyntaxMessage(playerid, "/setcode [senha] [senha]");
		if (strlen(old_acode) < 4 || strlen(old_acode) > 16)    return SendErrorMessage(playerid, "A senha deve conter entre 4 e 16 caracteres.");
		if (strlen(new_acode) < 4 || strlen(new_acode) > 16)    return SendErrorMessage(playerid, "A senha deve conter entre 4 e 16 caracteres.");
		if (!VerifyAdminCode(playerid, old_acode))    return SendErrorMessage(playerid, "A senha antiga foi inserida incorretamente. Entre em contato com o administrador sênior.");

		SetAdminCode(playerid, new_acode);				
	}
	return 1;
}

ALTX:adminchat("/a");
CMD:adminchat(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (isnull(params)) 							return SendSyntaxMessage(playerid, "(/a)dminchat [texto]");

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;

		foreach(new i : Player)
		{
		    if (IsPlayerLogged(i) == 0 || !CheckAdmin(i, ADMIN_LEVEL_1)) continue;

			SendClientMessageEx(i, COLOR_YELLOWG, "* [A%d] %s [ID: %d]: %.*s ...", PlayerInfo[playerid][pAdmin], GetAccName(playerid), playerid, pos, params);
			SendClientMessageEx(i, COLOR_YELLOWG, "... %s", params[pos]);
		}
	}
	else
	{
		AMSG(COLOR_YELLOWG, "* [A%d] %s [ID: %d]: %s", PlayerInfo[playerid][pAdmin], GetAccName(playerid), playerid, params);
	}
	return 1;
}

ALTX:an("/ans", "/answer");
CMD:an(playerid,params[])
{
	new userid, text[128];

	if (sscanf(params, "us[128]", userid, text))                 return SendSyntaxMessage(playerid, "/an [ID/Parte do Nome] [resposta]");
	if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (!IsPlayerLogged(userid))                                   return SendNotConnected(playerid);
	if (userid == playerid)                                         return SendErrorMessage(playerid, "Você não pode responder a si mesmo.");

    new mes[256];

	if (strlen(text) > MAXLEN)
	{
	    new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		format(mes, sizeof(mes), "%s responde a você: %s", GetAccName(playerid), text);
		SendClientMessageEx(userid, 0x00CC99AA, "%.*s", pos, mes);
		SendClientMessageEx(userid, 0x00CC99AA, "... %s", mes[pos]);

		format(mes, sizeof(mes), "[RESPOSTA]: %s respondeu a %s [ID: %i]: %s", GetAccName(playerid), GetNameEx(userid), userid, text);

		StaffMSG(0x00CC99AA, "%.*s", pos, mes);
		StaffMSG(0x00CC99AA, "... %s", mes[pos]);
	}
	else
	{
		format(mes, sizeof(mes), "%s responde a você: %s", GetAccName(playerid), text);
		SendClientMessage(userid, 0x00CC99AA, mes);
		StaffMSG(0x00CC99AA, "[RESPOSTA]: %s respondeu a %s [ID: %i]: %s", GetAccName(playerid), GetNameEx(userid), userid, text);

	}
	return 1;
}

ALTX:aooc("/ao");
CMD:aooc(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (isnull(params))							return SendSyntaxMessage(playerid, "(/ao)oc [texto]");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		
		if (pos < MAXLEN-1) pos = MAXLEN;

		format(mes, sizeof(mes), "[AOOC] %s: %.*s ...", GetAccName(playerid), pos, params);
		SCMALL(0xF97804FF,mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SCMALL(0xF97804FF,mes);
	}
	else
	{
		format(mes, sizeof(mes), "[AOOC] %s: %s", GetAccName(playerid), params);
		SCMALL(0xF97804FF, mes);
	}
	return 1;
}

CMD:announce(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (isnull(params))							return SendSyntaxMessage(playerid, "/announce [texto]");

    new mes[128];
    format(mes, sizeof(mes), "~y~%s: ~w~%s", GetNameEx(playerid), params);

    foreach (new i : Player)
	{
	    if (IsPlayerLogged(i) == 0 || GetWorld(playerid) != GetWorld(i)) continue;

		GameTextForPlayer(i, mes, 5000, 4);
	}
	return 1;
}

ALTX:setinterior("/setint");
CMD:setinterior(playerid, params[])
{
	new userid, int;
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 	return SendNotAdmin(playerid);
	if (sscanf(params, "ud", userid, int))    return SendSyntaxMessage(playerid, "/setinterior [ID/Parte do Nome] [ID interior (0-255)]");
	if (!IsPlayerLogged(userid))    return SendNotConnected(playerid);
	if (int < 0 || int > 255)    return SendErrorMessage(playerid, "ID interior de 0 a 255.");

	SetInterior(userid, int);
	SendAdmMessageToPlayer(userid, "O administrador %s [ID: %i] alterou o seu interior [ID: %i].", GetAccName(playerid), playerid, int);
	SendAdmMessageToPlayer(playerid, "Você alterou o interior de %s [ID: %i] para [ID: %i].", GetNameEx(userid), userid, int);
	return 1;
}

CMD:setworld(playerid, params[])
{
	new userid, wid;
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 	return SendNotAdmin(playerid);
	if (sscanf(params, "ud", userid, wid))    return SendSyntaxMessage(playerid, "/setworld [ID/Parte do Nome] [ID mundo virtual (65535)]");
	if (!IsPlayerLogged(userid))    return SendNotConnected(playerid);
	if (wid < 0 || wid > 65535)    return SendErrorMessage(playerid, "ID do mundo virtual deve estar entre 0 e 65535.");

	
	SetWorld(userid, wid);
	SendAdmMessageToPlayer(userid, "O administrador %s [ID: %i] alterou o seu mundo virtual [ID: %i].", GetAccName(playerid), playerid, wid);
	SendAdmMessageToPlayer(playerid, "Você alterou o mundo virtual de %s [ID: %i] para [ID: %i].", GetNameEx(userid), userid, wid);
	return 1;
}

CMD:getworld(playerid, params[])
{
	new userid;
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))    return SendSyntaxMessage(playerid, "/getworld [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))    return SendNotConnected(playerid);

	SendAdmMessageToPlayer(playerid, "O mundo virtual do jogador %s [ID: %d] = %i.", GetNameEx(userid), GetWorld(userid));
	return 1;
}

CMD:masked(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	new mask_string[1024];
	mask_string[0] = EOS;

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || !maskOn{i}) continue;
		format(mask_string, sizeof(mask_string), "{FFFFFF}%s%s - %s [ID: %i]\n", mask_string, GetNameWithMask(i), GetNameEx(i), i); 
	}

	Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Jogadores com máscara", mask_string, ">>>", "");
	return 1;
}

CMD:gotols(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 1529.6, -1691.2, 13.3);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotopc(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 2269.4795,-81.2131,26.5414);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotom(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 1358.6038,251.7677,19.5669);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotod(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 207.7609,-64.7452,1.5781);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotofc(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, -204.1443,1119.1970,19.7422);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:kick(playerid, params[])
{
	new userid, reason[64];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))		return SendNotAdmin(playerid);
	if (sscanf(params, "us[64]", userid, reason)) 	return SendSyntaxMessage(playerid, "/kick [ID/Parte do Nome] [razão]");
	if (!IsPlayerLogged(userid)) 					return SendNotConnected(playerid);


	if (PlayerInfo[userid][pAdmin] < PlayerInfo[playerid][pAdmin]) 
	{
		new mes[128];

		format(mes, sizeof(mes), "[/kick] %s foi expulso pelo administrador %s. Motivo: %s", GetNameEx(userid), GetAccName(playerid), reason);
		insert_log(mes, ADMIN_LOG, 2, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

		format(mes, sizeof(mes), "AdmCmd: %s foi expulso pelo administrador %s. Motivo: %s", GetNameEx(userid), GetAccName(playerid), reason);
		SendClientMessageByAdmin(COLOR_LIGHTRED, mes);
		
		KickEx(userid);
	}
	return 1;
}

CMD:skick(playerid, params[])
{
	new userid, reason[128];
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 		return SendNotAdmin(playerid);
	if (sscanf(params, "us[128]", userid, reason)) 	return SendSyntaxMessage(playerid, "/skick [ID/Parte do Nome] [razão]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);

	if (PlayerInfo[userid][pAdmin] < PlayerInfo[playerid][pAdmin])
	{
		new mes[128];
		format(mes, sizeof(mes), "AdmCmd: %s expulsou %s.", GetAccName(playerid), GetNameEx(userid));
		SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

		format(mes, sizeof(mes), "[/skick] %s foi expulso pelo administrador %s. Motivo: %s", GetNameEx(userid), GetAccName(playerid), reason);
		insert_log(mes, ADMIN_LOG, 2, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

		KickEx(userid);
	}
	return 1;
}

CMD:offjail(playerid, params[])
{
	new username[MAX_PLAYER_NAME + 1], reason[32], time;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 					return SendNotAdmin(playerid);
	if (sscanf(params, "s[32]ds[32]", username, time, reason)) return SendSyntaxMessage(playerid, "/offjail [nome do personagem] [tempo em minutos (1-2880)] [razão]");
	if (!IsValidPlayerName(username)) return SendErrorMessage(playerid, "Nome do personagem inválido.");
	if (time <= 0 || time > 2880) return SendErrorMessage(playerid, "O tempo em minutos deve estar entre 1 e 2880.");


    foreach (new i : Player)
	{
	    if (IsPlayerLogged(i) == 0 || !CompareStrings(username, GetNameEx(i))) continue;
	    SendErrorMessage(playerid, "Este jogador está online: %s [ID: %i]", GetNameEx(i), i);
		return 1;
	}

	new query[128];
	mysql_format(dbHandle, query,sizeof(query),"UPDATE `users` SET `ajailed` = '1', `ajailtime` = '%i' WHERE `name` = '%e'", time * 60, username);
	mysql_tquery(dbHandle, query, "OnPlayerOfflineJail", "dsds", playerid, username, time, reason);
	return 1;
}

CMD:ajail(playerid, params[])
{
	new userid, time, reason[128];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))				return SendNotAdmin(playerid);
	if (sscanf(params, "uds[128]", userid, time, reason)) return SendSyntaxMessage(playerid, "/ajail [ID/Parte do Nome] [tempo em minutos (1-2880)] [razão]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (time <= 0 || time > 2880) return SendErrorMessage(playerid, "O tempo em minutos deve estar entre 1 e 2880.");

	PlayerInfo[userid][pAJailed] = 1;
	PlayerInfo[userid][pAJailTime] = time*60;
	SetInterior(userid, 18);
	SetWorld(userid, userid + 100);
	SetPlayerPosEx(userid, -219.4580,1410.6747,27.7734);
	SetSpawnInfoEx(userid, -219.4580,1410.6747,27.7734);
	ResetPlayerWeaponsEx(userid);

	new mes[128];

	format(mes, sizeof(mes), "AdmCmd: %s foi enviado para a prisão administrativa por %s por %i minutos. Motivo: %s", GetNameEx(userid), GetAccName(playerid), time, reason);
	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	format(mes, sizeof(mes), "AdmCmd: %s foi enviado para a prisão administrativa por %s por %i minutos. Motivo: %s", GetNameEx(userid), GetAccName(playerid), time, reason);
	insert_log(mes, ADMIN_LOG, 5, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);
	return 1;
}

CMD:afree(playerid, params[])
{
	new userid;
	
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 									return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid)) return SendSyntaxMessage(playerid, "/afree [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (PlayerInfo[userid][pJailed] == 0 && PlayerInfo[userid][pAJailed] == 0) return SendErrorMessage(playerid, "O jogador não está na prisão.");


	PlayerInfo[userid][pJailed] = 
	PlayerInfo[userid][pIsolateTime] =
	PlayerInfo[userid][pJailTime] =
	PlayerInfo[userid][pAJailed] = 
	PlayerInfo[userid][pAJailTime] = 0;

	if (IsValidDynamic3DTextLabel(PrisonLabel[userid]))
	{
		DestroyDynamic3DTextLabel(PrisonLabel[userid]);
  		PrisonLabel[userid] = Text3D:INVALID_3DTEXT_ID;
	}

	SetInterior(userid, 0);
	SetWorld(userid, 0);
	SetPlayerPosEx(userid, 2230.5518,-109.0134,26.6406);
 	SetSpawnInfoEx(userid, 2230.5518,-109.0134,26.6406, 3.9650);

	SendAdmMessageToPlayer(userid, "Você foi libertado da prisão por um administrador %s [ID: %i].", GetAccName(playerid), playerid);
	SendAdmMessageToPlayer(playerid, "Você libertou %s da prisão [ID: %i].", GetAccName(userid), userid);
	return 1;
}

CMD:delaction(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid)) return SendSyntaxMessage(playerid, "/delaction [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (!GetPVarInt(userid, "3DLabel")) return SendErrorMessage(playerid, "O jogador não criou um texto 3D.");

	DestroyDynamic3DTextLabel(Text3D:GetPVarInt(playerid, "3DLabel"));
	DeletePVar(userid, "3DLabel");

	SendAdmMessageToPlayer(userid, "Sua descrição do ambiente (/action) foi removida pelo administrador %s.", GetAccName(playerid));
	AMSG(COLOR_LIGHTRED, "AdmCmd: %s removeu a descrição do ambiente do jogador %s [ID:%i].", GetAccName(playerid), GetNameEx(userid), userid);
    return 1;
}

CMD:warn(playerid, params[])
{
	new userid, reason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 		return SendNotAdmin(playerid);
	if (sscanf(params, "us[125]", userid, reason)) return SendSyntaxMessage(playerid, "/warn [ID/Parte do Nome] [razão]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (strlen(reason) > 63) return SendErrorMessage(playerid, "Número de caracteres não pode exceder 63.");

    new mes[128], query[256];

	PlayerInfo[userid][pWarns]++;
	SQL_SetInteger("users", "warn", PlayerInfo[userid][pWarns], PlayerInfo[userid][pID]);
	
	if (PlayerInfo[userid][pWarns] >= 3)
	{
		format(mes, sizeof(mes), "AdmCmd: %s banido pelo administrador %s. Motivo: %s (3 avisos)", GetNameEx(userid), GetAccName(playerid), reason, PlayerInfo[userid][pWarns]);
		SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

		format(query, sizeof(query), "[/ban] %s foi banido pelo administrador %s. Motivo: %s (3 avisos)", GetAccName(userid), GetAccName(playerid), reason);

		insert_log(query, ADMIN_LOG, 1, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

		new ip[24];
		GetPlayerIp(userid, ip, sizeof(ip));

		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (accountid, ip, name, admin, reason, date) VALUES (%i, '%e', '%e', '%e', '%e', '%e')", PlayerInfo[playerid][pAccID], ip, GetAccName(userid), GetAccName(playerid), reason, GetFullDate());
		mysql_tquery(dbHandle, query);

		KickEx(userid);
		return 1;
	}

	SendAdmMessageToPlayer(userid, "Você recebeu um aviso de %s [%i/3].", GetAccName(playerid), PlayerInfo[userid][pWarns]);

	format(query, sizeof(query), "[/warn] %s recebeu um aviso de %s. Motivo: %s", GetAccName(userid), GetAccName(playerid), reason);
	insert_log(query, ADMIN_LOG, 4, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

	format(mes, sizeof(mes), "AdmCmd: %s recebeu um aviso do admin %s. Motivo: %s [%i/3]", GetNameEx(userid), GetAccName(playerid), reason, PlayerInfo[userid][pWarns]);

	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);
	return 1;
}

CMD:block(playerid, params[])
{
	new userid, reason[64];
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (sscanf(params, "us[64]", userid, reason)) return SendSyntaxMessage(playerid, "/block [ID/Parte do Nome] [razão]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (strlen(reason) > 63) return SendErrorMessage(playerid, "Número de caracteres não pode exceder 63.");

	new query[256];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `block_name`='%e', `block_reason` ='%e', `status` = '3' WHERE `id` = '%i'", GetAccName(playerid), reason, PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query);

	new mes[128];
	format(mes, sizeof(mes), "AdmCmd: O personagem %s foi bloqueado pelo administrador %s. Motivo: %s.", GetNameEx(userid), GetAccName(playerid), reason);
	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	KickEx(userid);
	return 1;
}

CMD:offblock(playerid, params[])
{
	new name[MAX_PLAYER_NAME], reason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (sscanf(params, "s[24]s[64]", name, reason)) return SendSyntaxMessage(playerid, "/offblock [nome do personagem] [razão]");
	if (!IsValidPlayerName(name)) return SendErrorMessage(playerid, "Nome de personagem inválido.");
	if (strlen(reason) > 63) return SendErrorMessage(playerid, "O número de caracteres da razão não pode exceder 63.");

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || !CompareStrings(GetNameEx(i), name)) continue;

		SendErrorMessage(playerid, "O personagem está online - %s [ID: %i]", GetNameEx(i), i);
		return 1;
	}

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", name);
	mysql_tquery(dbHandle, query, "OfflineBlockPlayer", "iss", playerid, name, reason);
	return 1;
}

CMD:unblock(playerid, params[])
{
    new name[MAX_PLAYER_NAME];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "s[24]", name)) return SendSyntaxMessage(playerid, "/unblock [nome do personagem]");

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", name);
    mysql_tquery(dbHandle, query, "UnBlockPlayer", "is", playerid, name);
    return 1;
}

CMD:offban(playerid, params[])
{
    new name[MAX_PLAYER_NAME], reason[64];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "s[24]s[64]", name, reason)) return SendSyntaxMessage(playerid, "/offban [conta] [razão]");
    if (!IsValidPlayerName(name)) return SendErrorMessage(playerid, "Nome de usuário inválido.");
    if (strlen(reason) > 63) return SendErrorMessage(playerid, "Número de caracteres não pode exceder 63.");

    foreach (new i : Player)
    {
        if (!IsPlayerLogged(i) || !CompareStrings(GetAccName(i), name)) continue;

        SendErrorMessage(playerid, "Conta \"%s\" online - personagem %s [ID: %i]", name, GetNameEx(i), i);
        return 1;
    }

    new query[156];
    mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `id` FROM `bans` WHERE `name` = '%e') as banid FROM `accounts` WHERE `login` = '%e'", name, name);
    mysql_tquery(dbHandle, query, "OfflineBanPlayer", "is[24]s[64]", playerid, name, reason);
    return 1;
}

CMD:ban(playerid, params[])
{
    new userid, reason[64];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "us[64]", userid, reason)) return SendSyntaxMessage(playerid, "/ban [ID/Parte do Nome] [razão]");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (strlen(reason) > 63) return SendErrorMessage(playerid, "Número de caracteres não pode exceder 63.");

    new mes[128];
    format(mes, sizeof(mes), "AdmCmd: %s [%s] foi banido pelo administrador %s. Motivo: %s.", GetNameEx(userid), GetAccName(userid), GetAccName(playerid), reason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

    new ip[24], query[512];
    GetPlayerIp(userid, ip, sizeof(ip));

    mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (accountid, ip, name, admin, reason, date) VALUES (%i, '%e', '%e', '%e', '%e', '%e')", PlayerInfo[playerid][pAccID], ip, GetAccName(userid), GetAccName(playerid), reason, GetFullDate());
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "[/ban] %s foi banido pelo administrador %s. Motivo: %s", GetAccName(userid), GetAccName(playerid), reason);
    insert_log(query, ADMIN_LOG, 1, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

    KickEx(userid);
    return 1;
}

CMD:unban(playerid, params[])
{
	new name[MAX_PLAYER_NAME];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "s[24]", name))			return SendSyntaxMessage(playerid, "/unban [conta]");

    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `bans` WHERE `name` = '%e'", name);
	mysql_tquery(dbHandle, query, "UnBanPlayer", "is", playerid, name);
	return 1;
}

CMD:banip(playerid, params[])
{
    new ip[24], reason[64];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "s[24]s[64]", ip)) return SendSyntaxMessage(playerid, "/banip [IP] [razão]");
    if (!IsAnIP(ip)) return SendErrorMessage(playerid, "Formato de IP incorreto.");
    if (strlen(reason) > 63) return SendErrorMessage(playerid, "Número de caracteres não pode exceder 63.");

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `bans` WHERE `ip` = '%e'", ip);
    mysql_tquery(dbHandle, query, "BanirUmIP", "iss", playerid, ip, reason);
    return 1;
}

CMD:unbanip(playerid, params[])
{
    new ip[24];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "s[24]", ip)) return SendSyntaxMessage(playerid, "/unbanip [IP]");
    if (!IsAnIP(ip)) return SendErrorMessage(playerid, "Formato de IP incorreto.");

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `bans` WHERE `ip` = '%e'", ip);
    mysql_tquery(dbHandle, query, "DesbanirUmIP", "is", playerid, ip);

    return 1;
}

CMD:mute(playerid, params[])
{
    new userid, time, mreason[64];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "uds[128]", userid, time, mreason)) return SendSyntaxMessage(playerid, "/mute [ID/Parte do Nome] [tempo (1-1440)] [razão]");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (time < 1 || time > 1440) return SendErrorMessage(playerid, "Tempo de 1 a 1440 minutos.");
    if (strlen(mreason) > 63) return SendErrorMessage(playerid, "Texto com até 63 caracteres.");

    PlayerInfo[userid][pMutedTime] = time * 60;

    new mes[128];
    format(mes, sizeof(mes), "AdmCmd: %s foi silenciado pelo administrador %s por %i minutos. Razão: %s", GetNameEx(userid), GetAccName(playerid), time, mreason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, mes);
    return 1;
}

CMD:unmute(playerid, params[])
{
    new userid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "u", userid)) return SendSyntaxMessage(playerid, "/unmute [ID/Parte do Nome]");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (PlayerInfo[userid][pMutedTime] == 0) return SendErrorMessage(playerid, "O chat do jogador não está bloqueado.");

    PlayerInfo[userid][pMutedTime] = 0;

    new mes[128];
    format(mes, sizeof(mes), "AdmCmd: %s foi desbloqueado pelo administrador %s.", GetNameEx(userid), GetAccName(playerid));
    SendClientMessageByAdmin(COLOR_LIGHTRED, mes);
    return 1;
}

ALTX:clearchat("/cc");
CMD:clearchat(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);

	ClearChatboxForAll(50);
	SAMALL("O administrador %s limpou o chat.", GetAccName(playerid)); // AdmCmd
	return 1;
}

CMD:testmsg(playerid, params[])
{
	new Float:hp;
	AntiCheatGetHealth(playerid, hp);

	SM1(playerid, "%f", hp);
	return 1;
}


CMD:test(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))			return SendSyntaxMessage(playerid, "/gethere [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))				return SendNotConnected(playerid);

	SetPlayerMarkerForPlayer(playerid, userid, 0xA52A2AFF);
	return 1;
}

CMD:tp(playerid, params[])
{
	new telestring[3072], part[40];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);

	for(new OrT = 0; OrT < sizeof(OrTeleports); OrT++)
	{
		format(part, sizeof(part), "%s\n", OrTeleports[OrT][tele_name]);
		strcat(telestring, part, sizeof(telestring));
	}

	Dialog_Show(playerid, Teles, DIALOG_STYLE_LIST, "{FFFFFF}Escolha um local para teleportar", telestring, ">>>", "Fechar");
	return 1;
}

CMD:gethere(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))				return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))						return SendSyntaxMessage(playerid, "/gethere [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))							return SendNotConnected(playerid);
	if (GetPlayerState(userid) == PLAYER_STATE_SPECTATING)	return SendErrorMessage(playerid, "O jogador está em modo de observação.");
        
    SendPlayerToPlayer(userid, playerid);
	SendAdmMessageToPlayer(userid, "O administrador %s teleportou você para o local dele.", GetAccName(playerid));
	return 1;
}

CMD:goto(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))				return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))						return SendSyntaxMessage(playerid, "/goto [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))							return SendNotConnected(playerid);
    if (GetPlayerState(userid) == PLAYER_STATE_SPECTATING)	return SendErrorMessage(playerid, "O jogador está em modo de observação.");
    
    SendPlayerToPlayer(playerid, userid);
	return 1;
}

CMD:freeze(playerid, params[])
{
	new userid;

	if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params,"u", userid))										return SendSyntaxMessage(playerid, "/freeze [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))										return SendNotConnected(playerid);

	if (!GetPVarInt(userid, "Freeze"))					
	{
		SendAdmMessageToPlayer(userid, "Você foi congelado pelo administrador %s [ID: %i].", GetAccName(playerid), playerid);
		SendAdmMessageToPlayer(playerid, "Você congelou %s [ID: %i].", GetNameEx(userid), userid);
		FreezePlayer(playerid);
		
	}
	else
	{
		SendAdmMessageToPlayer(userid, "Você foi descongelado pelo administrador %s [ID: %i].", GetAccName(playerid), playerid);
		SendAdmMessageToPlayer(playerid, "Você descongelou %s [ID: %i].", GetNameEx(userid), userid);
		UnFreezePlayer(playerid);
	}
	return 1;
}

CMD:slap(playerid, params[])
{
	new userid, Float:slx, Float:sly, Float:slz;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))				return SendSyntaxMessage(playerid, "/slap [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);

	AntiCheatGetPos(userid, slx, sly, slz);
	SetPlayerPosEx(userid, slx, sly, slz+10, 0);
	PlayerPlaySound(userid, 1130, slx, sly, slz+10);

	SendAdmMessageToPlayer(userid, "%s [ID: %i] te deu um tapa.", GetAccName(playerid), playerid);
	SendAdmMessageToPlayer(playerid, "Você deu um tapa em %s [ID: %i].", GetNameEx(userid), userid);
	return 1;
}

CMD:getip(playerid, params[])
{
	new ip[24], userid;
	
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))				return SendSyntaxMessage(playerid, "/getip [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);

	GetPlayerIp(userid, ip, sizeof(ip));
	SendAdmMessageToPlayer(playerid, "Endereço IP de %s: %s", GetNameEx(userid), ip);
	return 1;
}

ALTX:getstats("/astats");
CMD:getstats(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))			return SendSyntaxMessage(playerid, "/getstats [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))				return SendNotConnected(playerid);

	ShowStats(playerid, userid);
	return 1;
}

CMD:getproperty(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))			return SendSyntaxMessage(playerid, "/getproperty [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))				return SendNotConnected(playerid);

	ShowProperty(playerid, userid);
	return 1;
}

ALTX:healplayer("/aheal");
CMD:healplayer(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))			return SendHelpMessage(playerid, "/healplayer [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))				return SendNotConnected(playerid);
	if (pTemp[userid][pInvulnerable] != 0.0)	return SendErrorMessage(playerid, "Tente mais tarde!");

	SetPlayerHealth(userid, 100.0);
	ResetDamages(userid);
	StopPlayerAnims(userid);
	UnFreezePlayer(userid);
	SetPlayerSkill(playerid);

	SendAdmMessageToPlayer(userid, "O administrador %s te curou.", GetAccName(playerid));
	SendAdmMessageToPlayer(playerid, "Você curou %s.", GetNameEx(userid));
	return 1;
}

CMD:gotocar(playerid, params[])
{
	new vehicle, Float:x, Float:y, Float:z;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "d", vehicle))			return SendSyntaxMessage(playerid, "/gotocar [ID do Veículo]");
	if (!IsValidVehicle(vehicle))				return SendErrorMessage(playerid, "Veículo não encontrado.");

	GetVehiclePos(vehicle, x, y, z);
	SetPlayerPosEx(playerid, x+1, y+1, z);
	SetWorld(playerid, GetVehicleVirtualWorldEx(vehicle));
	SetInterior(playerid, GetVehicleInteriorEx(vehicle));
	return 1;
}

CMD:getcar(playerid, params[])
{
	new vehicle, Float:x, Float:y, Float:z, interior = GetInterior(playerid), vw = GetWorld(playerid);

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 	return SendNotAdmin(playerid);
	if (sscanf(params, "d", vehicle)) 			return SendSyntaxMessage(playerid, "/getcar [ID do Veículo]");
	if (!IsValidVehicle(vehicle))				return SendErrorMessage(playerid, "Veículo não encontrado.");
	if (IsPlayerInVehicle(playerid, vehicle))	return SendErrorMessage(playerid, "Você já está neste veículo.");
	    
    GetXYInFrontOfPlayer(playerid, x, y, z, 5.0);
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || !IsPlayerInVehicle(i, vehicle))
	        continue;
	        
		SetPlayerPosEx(i, x, y, z);
		SetInterior(i, interior);
		SetWorld(i, vw);
	}
	
	SetVehiclePos(vehicle, x, y, z);
	SetVehicleVirtualWorld(vehicle, vw);
	LinkVehicleToInterior(vehicle, interior);
	return 1;
}

CMD:fixveh(playerid, params[])
{
	new vehicle;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "d", vehicle))			return SendSyntaxMessage(playerid, "/fixveh [ID do Veículo]");
	if (!IsValidVehicle(vehicle))				return SendErrorMessage(playerid, "Veículo não encontrado.");
		
	FixVehicle(vehicle);
	return 1;
}

ALTX:spectate("/spec");
CMD:spectate(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))			return SendSyntaxMessage(playerid, "(/spec)tate [ID/Parte do Nome]");
	if (userid == playerid)						return SendErrorMessage(playerid, "Você não pode fazer spectate em si mesmo.");
	if (!IsPlayerLogged(userid))				return SendNotConnected(playerid);
	
	if (pTemp[playerid][pSpecPlayer] == INVALID_PLAYER_ID) SavePlayerPos(playerid);	
	pTemp[playerid][pSpecPlayer] = userid;
    TogglePlayerSpectating(playerid, 1);

	if (IsPlayerInAnyVehicle(userid)) 	PlayerSpectateVehicle(playerid, GetPlayerVehicleID(userid));
	else 								PlayerSpectatePlayer(playerid, userid);
	
	SetInterior(playerid, GetInterior(userid));
	SetWorld(playerid, GetWorld(userid));
	return 1;
}

CMD:specoff(playerid, params[])
{
	//if (!CheckAdmin(playerid, ADMIN_LEVEL_1))				return SendNotAdmin(playerid);
	if (pTemp[playerid][pSpecPlayer] == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Você não está em modo de spectate.");

	TogglePlayerSpectating(playerid, 0);

	if(GetPVarInt(playerid, #BODYCAM))	MDC_Show(playerid, PAGE_MEMBERS);
	return 1;
}


CMD:aplant(playerid, params[])
{
	new option[16], plant_string[16];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	if (sscanf(params, "s[16]S(-1)[127]", option, plant_string))
	{
		SendSyntaxMessage(playerid, "/aplant [info / remove]");
		return 1;
	}
	
	if (CompareStrings(option, "info"))
	{
		new plant = -1;

		if ((plant = Plant_Nearest(playerid)) == -1)		return SendErrorMessage(playerid, "Não há plantas perto de você.");

		new message[1024];

		format(message, sizeof(message), "ID da planta\t%d\nPlantada por\t%s\nData de plantio\t%s\n",
		plant, CropsInfo[plant][pPlanter], CropsInfo[plant][pPlantDate]);
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST, "{FFFFFF}Informações da planta", message, "Fechar", "");	
	}
	else if (CompareStrings(option, "remove"))
	{
		new plant_id = -1;
		
		if (sscanf(plant_string, "d", plant_id)) 										return SendSyntaxMessage(playerid, "/aplant remove [ID da Planta]");
		if (plant_id < 0 || plant_id > MAX_PLANTS-1 || !CropsInfo[plant_id][pPlanted])	return SendErrorMessage(playerid, "Uma planta com este ID não foi encontrada.");

		DestroyCrop(plant_id);
		SendAdmMessageToPlayer(playerid, "Você removeu a planta [ID: %d].", plant_id);
	}
	return 1;
}


CMD:aprison(playerid, params[])
{
	//return SendErrorMessage(playerid, "A prisão ainda está desativada!");
	new userid, time, reason[128];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 			 return SendNotAdmin(playerid);
	if (sscanf(params,"uds[128]", userid, time, reason)) return SendSyntaxMessage(playerid, "/aprison [ID/Parte do Nome] [tempo em minutos] [razão]");
	if (!IsPlayerLogged(userid))						 return SendNotConnected(playerid);
	if (time < 1 || time > 99999)						 return SendErrorMessage(playerid, "O tempo não pode ser inferior a 1 minuto nem superior a 99999 minutos.");

	new mes[128];
	format(mes, sizeof(mes), "AdmCmd: %s foi preso pelo administrador %s por %s minutos. Motivo: %s", GetNameEx(userid), GetAccName(playerid), FormatNumber(time), reason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));
	PlayerInfo[userid][pJailed] = PRISON_JAIL;
	PlayerInfo[userid][pJailTime] = time*60;

	SendSyntaxMessage(userid, "Você foi preso pelo administrador %s por %s minutos. Motivo: %s", GetAccName(playerid), FormatNumber(time), reason);
	SendSyntaxMessage(userid, "Seu número de prisioneiro é - A%i (/myprisonnumber)", PlayerInfo[playerid][pPrisoned]+100);

	SpawnPlayer(userid);
	return 1;
}

CMD:check(playerid, params[])
{
	new option[10], secoption[5];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);

	if (sscanf(params, "s[10]S()[5]", option, secoption))
	{
		SendHelpMessage(playerid, "/check [inv / gb / trunk / prop] [ID]");
		return 1;
	}
	if (CompareStrings(option, "inv"))
	{
		new userid;
		if (sscanf(secoption,"u", userid))			return SendHelpMessage(playerid, "/check inv [ID/Parte do Nome]");
		if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);

		PrintInvEx(userid, playerid);
	}
	else if (CompareStrings(option, "trunk"))
	{
		new vehid;
		if (sscanf(secoption,"i", vehid))		return SendHelpMessage(playerid, "/check trunk [ID do Veículo]");
		if (vehid < 1 || vehid >= MAX_VEHICLES)	return SendErrorMessage(playerid, "ID do veículo incorreto!");

		PrintVehInv(playerid, vehid, 1);
	}
	else if (CompareStrings(option, "gb"))
	{
		new vehid;
		if (sscanf(secoption,"i", vehid))		return SendHelpMessage(playerid, "/check gb [ID do Veículo]");
		if (vehid < 1 || vehid >= MAX_VEHICLES)	return SendErrorMessage(playerid, "ID do veículo incorreto!");

		PrintGBInv(playerid, vehid, 1);
	}
	else if (CompareStrings(option, "prop"))
	{
		new houseid;
		if (sscanf(secoption,"i", houseid))			return SendHelpMessage(playerid, "/check prop [ID da Casa]");
		if (houseid < 1 || houseid >= MAX_HOUSES)	return SendErrorMessage(playerid, "ID da casa incorreto!");
		if (!HouseInfo[houseid][hOwnerID])			return SendErrorMessage(playerid, "Esta casa não tem proprietário!");

		PrintHouseInv(playerid, houseid, 1);
	}
	else
	{
		SendHelpMessage(playerid, "/check [inv / gb / trunk / prop] [ID]");
	}
	return 1;
}

CMD:dmlog(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	
	if (pTemp[playerid][pDMLog] == 0)
	{
		SendAdmMessageToPlayer(playerid, "Você ativou o registro de DM.");
		pTemp[playerid][pDMLog] = 1;
	}
	else
	{
		SendAdmMessageToPlayer(playerid, "Você desativou o registro de DM.");
		ClearKillFeed(playerid);
		pTemp[playerid][pDMLog] = 0;
	}
	return 1;
}

ALTX:removevehicle("/rveh", "/dcar", "/removecar", "/dveh", "/rcar");
CMD:removevehicle(playerid, params[])
{
	new vehid = GetPlayerVehicleID(playerid);

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))				return SendNotAdmin(playerid);
	if (!IsValidVehicle(vehid) && sscanf(params, "d", vehid))	return SendSyntaxMessage(playerid, "/removevehicle [ID do Veículo]");
	if (!IsValidVehicle(vehid))								return SendErrorMessage(playerid, "Veículo não encontrado.");
	if (VehicleInfo[vehid][carOwnerID] != 0)					return SendErrorMessage(playerid, "Este veículo não pode ser removido.");

	if (VehicleInfo[vehid][carAdmin] == 0)
	{
		if (!CheckAdmin(playerid, ADMIN_LEVEL_3))				return SendErrorMessage(playerid, "Este veículo não pode ser removido.");

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `cars` WHERE `id` = %i", VehicleInfo[vehid][carID]);
		mysql_tquery(dbHandle, query);
	}

	DestroyVehicle(vehid);
	return 1;
}

ALTX:removebox("/rbox");
CMD:removebox(playerid, params[])
{
	new id = -1;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);

	for(new i; i != MAX_CARGO_OBJ; i++)
	{
		if (CargoObject[i][oOn] == 0 || !IsPlayerInRangeOfPoint(playerid, 2.0, CargoObject[i][oX], CargoObject[i][oY], CargoObject[i][oZ]) || GetWorld(playerid) != CargoObject[i][oVW] || CargoObject[i][oInt] != GetInterior(playerid)) continue;

		id = i;
		break;
	}

	if (id == -1) return SendErrorMessage(playerid, "Não há caixa de coleta próxima a você.");

	CargoObject[id][oOn] = 0;
	if (IsValidDynamicObject(CargoObject[id][oObj])) DestroyDynamicObject(CargoObject[id][oObj]);
	if (IsValidDynamic3DTextLabel(CargoObject[id][oLabel]))
	{
		DestroyDynamic3DTextLabel(CargoObject[id][oLabel]);
		CargoObject[id][oLabel] = Text3D:INVALID_3DTEXT_ID;
	}
	SendAdmMessageToPlayer(playerid, "Caixa [ID: %i] removida.", id);
	return 1;
}


CMD:near(playerid, params[])
{
	new id = -1;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);

	if ((id = Plant_Nearest(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma planta [ID: %i].", id);
	if ((id = Gate_Nearest(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um portão [ID: %i | Modelo: %i].", id, Gates[id][gateModel]);
	if ((id = GetNearHouse(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma casa [ID: %i].", id);
	if ((id = ATM_Nearest(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um caixa eletrônico [ID: %i].", id);
	if ((id = GetNearGarage(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma garagem [ID: %i].", id);
	if ((id = GetNearApartment(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um apartamento [ID: %i].", id);
	if ((id = GetNearBiz(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um negócio [ID: %i].", id);
	if ((id = TruckCargo_Nearest(playerid, 2.0)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma indústria [ID: %i].", id);
	if ((id = Object_Nearest(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um objeto [ID: %i | Modelo: %i].", id, ObjectsInfo[id][oModel]);
	if ((id = Job_Nearest(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um trabalho [ID: %i].", id);
	if ((id = GetNearestCamera(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma câmera de velocidade [ID: %i].", id);
	if ((id = Payphone_Nearest(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um orelhão [ID: %i].", id);
	if ((id = ParkMeter_Nearest(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de um parquímetro [ID: %i].", id);
	if ((id = GetStreetID(playerid)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma placa de rua [ID: %i].", id);
	if ((id = GetClosestBillBoard(playerid, 9.0)) != -1) SendSyntaxMessage(playerid, "Você está perto de um painel publicitário [ID: %i].", id);
	if ((id = Trash_Nearest(playerid, 9.0)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma lixeira [ID: %i].", id);
	if ((id = Factory_Nearest(playerid, 9.0)) != -1) SendSyntaxMessage(playerid, "Você está perto de uma fábrica de reciclagem de resíduos [ID: %i].", id);

	for (new i; i != MAX_ENTRANCE; i++)
	{
		if (EntranceInfo[i][entranceOn] == 0) continue;

		if (IsPlayerEntranceOutside(playerid, i)) SendSyntaxMessage(playerid, "Você está perto de uma entrada [ID: %i].", i);
		if (IsPlayerEntranceInside(playerid, i)) SendSyntaxMessage(playerid, "Você está perto de uma saída [ID: %i].", i);
	}

	for (new i = 0; i != MAX_LABELS; i++)
	{
		if (LabelsInfo[i][tLabelOn] != 1 || !PlayerToPoint(3, playerid, LabelsInfo[i][tPosX], LabelsInfo[i][tPosY], LabelsInfo[i][tPosZ]) || LabelsInfo[i][tVW] != GetWorld(playerid) || LabelsInfo[i][tInt] != GetInterior(playerid)) continue;

		SendSyntaxMessage(playerid, "Você está perto de uma TextLabel [ID: %i].", i);
	}

	//SendSyntaxMessage(playerid, "Não há nada perto de você.");
	return 1;
}


CMD:disarm(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid)) return SendSyntaxMessage(playerid, "/disarm [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);

	ResetPlayerWeaponsEx(userid);
	SendAdmMessageToPlayer(userid, "O administrador %s desarmou você.", GetAccName(playerid));
	SendAdmMessageToPlayer(playerid, "Você desarmou %s.", GetNameEx(userid));
	return 1;
}

ALTX:teles("/atp");
CMD:teles(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);

	new id;
	if (sscanf(params, "i", id)) pTemp[playerid][pDialogList] = 0;
	else pTemp[playerid][pDialogList] = id;

	new string[570],
		min_tp = pTemp[playerid][pDialogList] * MAX_ADMIN_TELES,
		max_tp = (pTemp[playerid][pDialogList] + 1) * MAX_ADMIN_TELES;

	if (pTemp[playerid][pDialogList]) string = "{bccd32}Página anterior{FFFFFF}\n";

	for (new i = min_tp; i < max_tp; i++)
	{
		if (!AdminTeles[i][telesOn]) continue;

		format(string, sizeof(string), "%s<%i>{66FF66}%s\n", string, i, AdminTeles[i][telesMapName]);
	}

	if (AdminTeles[max_tp][telesOn]) format(string, sizeof(string), "%s{bccd32}Próxima página", string);

	Dialog_Show(playerid, AdminTeles, DIALOG_STYLE_LIST, "{FFFFFF}Teleportar", string, ">>>", "Cancelar");

	return 1;
}

CMD:checkcars(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid)) return SendSyntaxMessage(playerid, "/checkcars [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `owner` = '%i'", PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query, "ShowCarsForPlayer", "ii", playerid, userid);
	return 1;
}

CMD:forbid(playerid, params[])
{
	new userid,
		hr;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (sscanf(params, "ui", userid, hr)) return SendHelpMessage(playerid, "/forbid [ID/Parte do Nome] [horas]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (hr > 10 && hr < 1) return SendHelpMessage(playerid, "/forbid [ID/Parte do Nome] [tempo em horas (1-10)]");

	PlayerInfo[userid][pForbid] = hr;
	SQL_SetInteger("users", "Forbid", hr, PlayerInfo[userid][pID]);

	SendAdmMessageToPlayer(userid, "O administrador %s proibiu você de usar armas por %i minutos (%i horas).", GetAccName(playerid), hr * 60, hr);
	SendAdmMessageToPlayer(playerid, "Você proibiu o uso de armas para %s por %i minutos (%i horas).", GetNameEx(userid), hr * 60, hr);

	return 1;
}


CMD:unforbid(playerid, params[])
{
	new userid;
	
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/unforbid [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (!PlayerInfo[userid][pForbid]) return SendErrorMessage(playerid, "O jogador não está proibido de usar armas.");

	PlayerInfo[userid][pForbid] = 0;
	SQL_SetInteger("users", "Forbid", 0, PlayerInfo[userid][pID]);

	SendAdmMessageToPlayer(userid, "O administrador %s removeu sua proibição de usar armas.", GetAccName(playerid));
	SendAdmMessageToPlayer(playerid, "Você removeu a proibição de uso de armas do jogador %s.", GetNameEx(userid));
	return 1;
}

ALTX:deletepame("/dpame");
CMD:deletepame(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
	if (sscanf(params, "i", userid)) return SendHelpMessage(playerid, "/deletepame [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);

	format(PlayerInfo[userid][pDescribe], PAME_MAX_LEN, "NULL");
	format(PlayerInfo[userid][pDescribe2], PAME_MAX_LEN, "NULL");

	SQL_SetString("users", "Desc1", "NULL", PlayerInfo[userid][pID]);
	SQL_SetString("users", "Desc1", "NULL", PlayerInfo[userid][pID]);

	SendAdmMessageToPlayer(playerid, "Você deletou a descrição do personagem de - %s.", GetNameEx(userid));
	SendAdmMessageToPlayer(userid, "O administrador %s deletou a descrição do seu personagem!", GetAccName(playerid));

	return 1;
}

ALTX:deleteadvertisment("/delad");
CMD:deleteadvertisment(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);

	new ad_id;
	if (sscanf(params, "i", ad_id)) return SendHelpMessage(playerid, "/del(ete)ad(vertisment) [ID do Anúncio]");
	if (!advert_data[ad_id][advert_exists]) return SendErrorMessage(playerid, "Anúncio com este ID não existe!");

	SendAdmMessageToPlayer(playerid, "Você removeu o anúncio - %s [ID:%i].", GetNameEx(advert_data[ad_id][advert_placeby_id]), advert_data[ad_id][advert_placeby_id]);
	SendAdmMessageToPlayer(advert_data[ad_id][advert_placeby_id], "O administrador %s removeu o seu anúncio.", GetAccName(playerid));

	ClearAd(ad_id);
	return 1;
}
