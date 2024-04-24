this::CheckAccount(playerid)
{
    if (!IsPlayerConnected(playerid))	return 1;

    SetPlayerColor(playerid, TEAM_HIT_COLOR);
	InterpolateCameraPos(playerid, -138.643066, 479.371765, 12.819633, -798.732910, -2049.936035, 138.499679, 10000000);
	InterpolateCameraLookAt(playerid, -139.824829, 474.517059, 13.007596, -797.734008, -2046.228637, 135.296936, 10000000);
	
	new name[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, name, sizeof(name));

	new query[256 - 4 + (MAX_PLAYER_NAME * 2) + 1];
	
	mysql_format(dbHandle, query, sizeof(query), "SELECT accounts.login AS login, accounts.id AS id, accounts.last_game_ip AS last_game_ip , accounts.google_auth AS google_auth FROM `accounts`, `users` WHERE accounts.login = '%e' OR (users.name = '%e' AND accounts.id = users.accountid) LIMIT 1", name, name); 
	mysql_tquery(dbHandle, query, "OnAccountCheck", "d", playerid);
	return 1;
}

this::LoadCharacter(playerid)
{
	if (cache_num_rows())
	{
	    new status = cache_get_field_content_int(0, "status");

		switch(status)
		{
		    case 0:
		    {
		        SendErrorMessage(playerid, "Seu personagem está em consideração. Mais detalhes na sua conta: %s", WeburlName);
		        ShowCharacterMenu(playerid);
		        return 1;
		    }
		    case 2:
		    {
		        SendErrorMessage(playerid, "Seu personagem foi REJEITADO. Mais detalhes na sua conta: %s", WeburlName);
		        ShowCharacterMenu(playerid);
		        return 1;
		    }
		    case 3:
		    {
		        new admin_name[MAX_PLAYER_NAME + 1];
		        cache_get_field_content(0, "block_name", admin_name);
		        
		        new reason[65];
		        cache_get_field_content(0, "block_reason", reason);
		        
		        static const str_size[] = "Seu personagem foi bloqueado pelo administrador %s.\nMotivo: %s";
		        new str[sizeof(str_size) - 4 + MAX_PLAYER_NAME + 64];
		        format(str, sizeof(str), str_size, admin_name, reason);
		        Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Bloqueio", str, "Fechar", "");
		        return KickEx(playerid);
		    }
		    case 4: 
		        return Kick(playerid);
		}


	    PlayerInfo[playerid][pID] = cache_get_field_content_int(0, "id");
		
	    foreach (new i : Player)
		{
			if (!IsPlayerLogged(i) || PlayerInfo[playerid][pAccID] != PlayerInfo[i][pAccID]) continue;

			SendErrorMessage(playerid, "This account is already in use.");
			KickEx(playerid);
			return 1;
		}

		new name[MAX_PLAYER_NAME+1];

		cache_get_field_content(0, "name", name);

  		if (!CompareStrings(PlayerInfo[playerid][pAccname], name) && SetPlayerName(playerid, name) != 1)
		{
		    SendErrorMessage(playerid,"This account is already in use.");    
		    KickEx(playerid);
		    return 1;
		}
	    GetNameForPlayer(playerid);

		SQL_SetInteger("users", "online", 1, PlayerInfo[playerid][pID]);

		//Load database
		PlayerInfo[playerid][pCash] = cache_get_field_content_int(0, "cash");
		PlayerInfo[playerid][pBank] = cache_get_field_content_int(0, "bank");
		PlayerInfo[playerid][pSkin] = cache_get_field_content_int(0, "skin");
		PlayerInfo[playerid][pSex] = cache_get_field_content_int(0, "sex");
	 	cache_get_field_content(0, "birthdate", PlayerInfo[playerid][pBirthdate], dbHandle, 24);
		PlayerInfo[playerid][pLevel] = cache_get_field_content_int(0, "level");
		PlayerInfo[playerid][pHours] = cache_get_field_content_int(0, "hours");
		PlayerInfo[playerid][pAdminHide] = cache_get_field_content_int(0, "admin_hide");
		
		cache_get_field_content(0, "admin_code", PlayerInfo[playerid][pAdminCode], dbHandle, 65);

		PlayerInfo[playerid][pAdmin] = cache_get_field_content_int(0, "admin");
		PlayerInfo[playerid][pHelper] = cache_get_field_content_int(0, "helper");
		PlayerInfo[playerid][pDonate] = cache_get_field_content_int(0, "donate");
        PlayerInfo[playerid][pNameChanges] = cache_get_field_content_int(0, "name_changes");
        PlayerInfo[playerid][pNumberChanges] = cache_get_field_content_int(0, "number_changes");
		
		PlayerInfo[playerid][pInt] = cache_get_field_content_int(0, "int");
		PlayerInfo[playerid][pWorld] = cache_get_field_content_int(0, "world");
		PlayerInfo[playerid][pAJailed] = cache_get_field_content_int(0, "ajailed");
		PlayerInfo[playerid][pAJailTime] = cache_get_field_content_int(0, "ajailtime");
		PlayerInfo[playerid][pJailed] = cache_get_field_content_int(0, "jailed");
		PlayerInfo[playerid][pIsolateTime] = cache_get_field_content_int(0, "isolatetime");
		PlayerInfo[playerid][pPremium] = cache_get_field_content_int(0, "vip");
		PlayerInfo[playerid][pPremiumTime] = cache_get_field_content_int(0, "vip_time");
		PlayerInfo[playerid][pCarLic] = cache_get_field_content_int(0, "carlic");
		PlayerInfo[playerid][pWepLic] = cache_get_field_content_int(0, "weplic");
		PlayerInfo[playerid][pFlyLic] = cache_get_field_content_int(0, "flylic");
		PlayerInfo[playerid][pTaxiLic] = cache_get_field_content_int(0, "taxilic");
		PlayerInfo[playerid][pInjured] = cache_get_field_content_int(0, "injured");
		PlayerInfo[playerid][pDeathReason] = cache_get_field_content_int(0, "deathreason");
		PlayerInfo[playerid][pPayCheckMoney] = cache_get_field_content_int(0, "paycheck");
		PlayerInfo[playerid][pPayDayTime] = cache_get_field_content_int(0, "paydaytime");
		PlayerInfo[playerid][pNumber] = cache_get_field_content_int(0, "number");
		PlayerInfo[playerid][pSavings] = cache_get_field_content_int(0, "savings");
		PlayerInfo[playerid][pMutedTime] = cache_get_field_content_int(0, "mutetime");
		PlayerInfo[playerid][pRentKey] = cache_get_field_content_int(0, "rent_key");
		PlayerInfo[playerid][pRentPrice] = cache_get_field_content_int(0, "rent_price");

		PlayerInfo[playerid][pHideWeap][0] = cache_get_field_content_int(0, "weaphide0");
		PlayerInfo[playerid][pHideWeap][1] = cache_get_field_content_int(0, "weaphide1");
		PlayerInfo[playerid][pHideWeap][2] = cache_get_field_content_int(0, "weaphide2");

		PlayerInfo[playerid][pChannel] = cache_get_field_content_int(0, "channel");
		PlayerInfo[playerid][pOwnedChannel] = cache_get_field_content_int(0, "owned_channel");
		PlayerInfo[playerid][pAuthed] = cache_get_field_content_int(0, "authed");
		PlayerInfo[playerid][pSpareKey] = cache_get_field_content_int(0, "sparekey");
		PlayerInfo[playerid][pWorkOn] = cache_get_field_content_int(0, "workon");
		PlayerInfo[playerid][pWalk] = cache_get_field_content_int(0, "walk");
		PlayerInfo[playerid][pArmour] = cache_get_field_content_float(0, "armour");
		PlayerInfo[playerid][pFaction] = cache_get_field_content_int(0, "faction");
		PlayerInfo[playerid][pRank] = cache_get_field_content_int(0, "rank");
		PlayerInfo[playerid][pBadgeNumber] = cache_get_field_content_int(0, "badgenumber");
		PlayerInfo[playerid][pDriveWarns] = cache_get_field_content_int(0, "drivewarns");
		PlayerInfo[playerid][pWepWarns] = cache_get_field_content_int(0, "wepwarns");
		PlayerInfo[playerid][pWepCount] = cache_get_field_content_int(0, "wep_count");
		PlayerInfo[playerid][pJailTime] = cache_get_field_content_int(0, "jailtime");
		PlayerInfo[playerid][pArrested] = cache_get_field_content_int(0, "arrested");
		PlayerInfo[playerid][pTalk] = cache_get_field_content_int(0, "chat");
		PlayerInfo[playerid][pAuthwep] = cache_get_field_content_int(0, "authwep");
  		PlayerInfo[playerid][pSwat] = cache_get_field_content_int(0, "swat");
		PlayerInfo[playerid][pSwatDuty] = cache_get_field_content_int(0, "swat_duty");
		PlayerInfo[playerid][pFishCount] = cache_get_field_content_int(0, "fishcount");
		PlayerInfo[playerid][pSlot] = cache_get_field_content_int(0, "slot");
		PlayerInfo[playerid][pJob] = cache_get_field_content_int(0, "job");
		PlayerInfo[playerid][pSideJob] = cache_get_field_content_int(0, "sidejob");
		PlayerInfo[playerid][pPrescribe] = cache_get_field_content_int(0, "prescribe");
		PlayerInfo[playerid][pGanjaTime] = cache_get_field_content_int(0, "ganja_time");
		PlayerInfo[playerid][pCareer] = cache_get_field_content_int(0, "career");
		PlayerInfo[playerid][pSpawnType] = cache_get_field_content_int(0, "spawntype");
		PlayerInfo[playerid][pSpawnPoint] = cache_get_field_content_int(0, "spawnpoint");
		PlayerInfo[playerid][pPosX] = cache_get_field_content_float(0, "pos_X");
		PlayerInfo[playerid][pPosY] = cache_get_field_content_float(0, "pos_Y");
		PlayerInfo[playerid][pPosZ] = cache_get_field_content_float(0, "pos_Z");
		PlayerInfo[playerid][pPosA] = cache_get_field_content_float(0, "pos_A");
		PlayerInfo[playerid][pPrisoned] = cache_get_field_content_int(0, "prisoned");
		PlayerInfo[playerid][pOnDuty] = cache_get_field_content_int(0, "duty");
		PlayerInfo[playerid][pChar] = cache_get_field_content_int(0, "char");
		PlayerInfo[playerid][pHud] = cache_get_field_content_int(0, "hud");
		
		PlayerInfo[playerid][pDrugLabAccess] = cache_get_field_content_int(0, "drugaccess");

		PlayerInfo[playerid][pWeaponBone][0] = cache_get_field_content_int(0, "weap_0_bone");
		PlayerInfo[playerid][pWeaponX][0] = cache_get_field_content_float(0, "weap_0_x");
		PlayerInfo[playerid][pWeaponY][0] = cache_get_field_content_float(0, "weap_0_y");
		PlayerInfo[playerid][pWeaponZ][0] = cache_get_field_content_float(0, "weap_0_z");
		PlayerInfo[playerid][pWeaponRX][0] = cache_get_field_content_float(0, "weap_0_rx");
		PlayerInfo[playerid][pWeaponRY][0] = cache_get_field_content_float(0, "weap_0_ry");
		PlayerInfo[playerid][pWeaponRZ][0] = cache_get_field_content_float(0, "weap_0_rz");

		PlayerInfo[playerid][pWeaponBone][1] = cache_get_field_content_int(0, "weap_1_bone");
		PlayerInfo[playerid][pWeaponX][1] = cache_get_field_content_float(0, "weap_1_x");
		PlayerInfo[playerid][pWeaponY][1] = cache_get_field_content_float(0, "weap_1_y");
		PlayerInfo[playerid][pWeaponZ][1] = cache_get_field_content_float(0, "weap_1_z");
		PlayerInfo[playerid][pWeaponRX][1] = cache_get_field_content_float(0, "weap_1_rx");
		PlayerInfo[playerid][pWeaponRY][1] = cache_get_field_content_float(0, "weap_1_ry");
		PlayerInfo[playerid][pWeaponRZ][1] = cache_get_field_content_float(0, "weap_1_rz");

		PlayerInfo[playerid][pWeaponBone][2] = cache_get_field_content_int(0, "weap_2_bone");
		PlayerInfo[playerid][pWeaponX][2] = cache_get_field_content_float(0, "weap_2_x");
		PlayerInfo[playerid][pWeaponY][2] = cache_get_field_content_float(0, "weap_2_y");
		PlayerInfo[playerid][pWeaponZ][2] = cache_get_field_content_float(0, "weap_2_z");
		PlayerInfo[playerid][pWeaponRX][2] = cache_get_field_content_float(0, "weap_2_rx");
		PlayerInfo[playerid][pWeaponRY][2] = cache_get_field_content_float(0, "weap_2_ry");
		PlayerInfo[playerid][pWeaponRZ][2] = cache_get_field_content_float(0, "weap_2_rz");

		PlayerInfo[playerid][pFightStyle] = cache_get_field_content_int(0, "fightstyle");

		PlayerInfo[playerid][pTog][0] = cache_get_field_content_int(0, "tog_0");
		PlayerInfo[playerid][pTog][1] = cache_get_field_content_int(0, "tog_1");
		PlayerInfo[playerid][pTog][2] = cache_get_field_content_int(0, "tog_2");

		PlayerInfo[playerid][pPhoneOff] = cache_get_field_content_int(0, "phone_off");
		PlayerInfo[playerid][pPhoneAirplane] = cache_get_field_content_int(0, "phone_airplane");
		PlayerInfo[playerid][pPhoneSound] = cache_get_field_content_int(0, "phone_sound");
		PlayerInfo[playerid][pPhoneModel] = cache_get_field_content_int(0, "phone_model");
		PlayerInfo[playerid][pPhoneRingtone] = cache_get_field_content_int(0, "phone_ringtone");
        PlayerInfo[playerid][pPhoneStyle] = cache_get_field_content_int(0, "phone_style");
        PlayerInfo[playerid][pSettings] = cache_get_field_content_int(0, "settings");

		PlayerInfo[playerid][pWarns] = cache_get_field_content_int(0, "warns");

		PlayerInfo[playerid][pPlayerWeapon] = cache_get_field_content_int(0, "PlayerWeapon");
		PlayerInfo[playerid][pPlayerAmmo] = cache_get_field_content_int(0, "PlayerAmmo");
		PlayerInfo[playerid][pAmmoType] = cache_get_field_content_int(0, "AmmoType");
		PlayerInfo[playerid][pWepSerial] = cache_get_field_content_int(0, "WepSerial");

		PlayerInfo[playerid][pPlayerSerial] = cache_get_field_content_int(0, "PlayerSerial");
		if (!PlayerInfo[playerid][pPlayerSerial]) PlayerInfo[playerid][pPlayerSerial] = PlayerInfo[playerid][pID]*100 + 99999 + random(99);

		PlayerInfo[playerid][pLastPrim] = cache_get_field_content_int(0, "LastPrim");
		PlayerInfo[playerid][pLastSec] = cache_get_field_content_int(0, "LastSec");
		PlayerInfo[playerid][pLastMelee] = cache_get_field_content_int(0, "LastMelee");

		PlayerInfo[playerid][pForbid] = cache_get_field_content_int(0, "Forbid");

		cache_get_field_content(0, "Desc1", PlayerInfo[playerid][pDescribe], dbHandle, PAME_MAX_LEN);
		cache_get_field_content(0, "Desc2", PlayerInfo[playerid][pDescribe2], dbHandle, PAME_MAX_LEN);

		if (gettime() - cache_get_field_content_int(0, "last_login") < 25*60) {
			SetPVarInt(playerid, #last_login, 1);
			SetPlayerHealth(playerid, cache_get_field_content_float(0, "health"));
		} else {
			SetPlayerHealth(playerid, cache_get_field_content_float(0, "spawnhealth"));
		}

		for(new w = 0; w < MAX_COP_SLOTS; w++)
		{
			new q_str[16];
			
			format(q_str, sizeof(q_str), "CopItem_%i", w+1);
			PlayerInfo[playerid][pCopItem][w] = cache_get_field_content_int(0, q_str);

			format(q_str, sizeof(q_str), "CopAmmo_%i", w+1);
			PlayerInfo[playerid][pCopQ][w] = cache_get_field_content_int(0, q_str);

			format(q_str, sizeof(q_str), "CopEx_%i", w+1);
			PlayerInfo[playerid][pCopEx][w] = cache_get_field_content_int(0, q_str);

			format(q_str, sizeof(q_str), "CopS_%i", w+1);
			PlayerInfo[playerid][pCopS][w] = cache_get_field_content_int(0, q_str);
		}

		pTemp[playerid][pPayphone] = -1;
		pTemp[playerid][pPayphoneNumStr] = EOS;
		
		pTemp[playerid][pCallLine] = INVALID_PLAYER_ID;
		pTemp[playerid][pCallStatus] = 0;
		pTemp[playerid][pCallIncoming] = 0;
		pTemp[playerid][pCallTime] = 0;
		format(pTemp[playerid][pCallName], 25, "");

		for(new i; i<6; i++) pTemp[playerid][editPos][i] = 0.0;
		
		PhoneInfo[playerid][current_page] = PAGE_NONE;

		LoadPlayerAttach(playerid);
		LoadPlayerMusic(playerid);
        LoadPlayerInventory(playerid);
		LoadPlayerNotes(playerid);
		LoadPlayerContacts(playerid);
		LoadPlayerMarks(playerid);
		
		LoadPayPhoneBtnTD(playerid);
		LoadNumPayPhoneTD(playerid);
		CreateVehicleMenu(playerid);

		
        PlayerInfo[playerid][pDependence] = cache_get_field_content_int(0, "dependence");
        PlayerInfo[playerid][pDemolition] = cache_get_field_content_int(0, "demolition");
        PlayerInfo[playerid][pFishTime] = cache_get_field_content_int(0, "fish_time");

		foreach(new i:Vehicle)
		{
		    if (VehicleInfo[i][carOwnerID] == PlayerInfo[playerid][pID])
			{
            	PlayerInfo[playerid][pCarKey] = i;
            	VehicleInfo[i][carOwnerPlayer] = playerid;
            }
			else if (VehicleInfo[i][carRent] == PlayerInfo[playerid][pID])
			{
				if (VehicleInfo[i][carType] > 5 && VehicleInfo[i][carType] < 10) {
					pTemp[playerid][pJob] = 1;
					pTemp[playerid][pJobTime] = gettime();
				}

			    pTemp[playerid][pRentveh] = i;
			    VehicleInfo[i][carTerminate] = 0;
			}
		}
		
        ClearChatbox(playerid, 50);
		new gametext[48];

		format(gametext, sizeof(gametext), "~w~Bem-vindo de volta ~n~~h~~y~   %s", GetNameEx(playerid));
		GameTextForPlayer(playerid, gametext, 2500, 1);

		SendServerMessage(playerid, "Bem-vindo ao servidor, {ffc46a}%s!", GetNameEx(playerid));
		SendServerMessage(playerid, "Não se esqueça dos comandos: {ffc46a}/askq, /help, /mm");

		new premium_info[4][32] = {"Usual", "{CC6633}Bronze{FFFFFF}", "{C0C0C0}Prata{FFFFFF}", "{FFD700}Ouro{FFFFFF}"};

		if (PlayerInfo[playerid][pPremium] && PlayerInfo[playerid][pPremiumTime] > gettime())
		{
		    new Float:days = float(PlayerInfo[playerid][pPremiumTime] - gettime()) / 86400.0;

		    SendServerMessage(playerid, "Você tem conta %s. %i dias restantes.", premium_info[PlayerInfo[playerid][pPremium]], floatround(days));
		}
		else if (PlayerInfo[playerid][pPremium] && PlayerInfo[playerid][pPremiumTime] < gettime())
		{
		    SendServerMessage(playerid, "Conta Premium (%s) expirada.", premium_info[PlayerInfo[playerid][pPremium]]);

		    PlayerInfo[playerid][pPremium] = 0;
		    PlayerInfo[playerid][pPremiumTime] = 0;
		    PlayerInfo[playerid][pHud] = 0;
		    PlayerInfo[playerid][pFightStyle] = 4;
		    SQL_SetFloat("users", "spawnhealth", 75.0, PlayerInfo[playerid][pID]);
		    Save_User(playerid);			
		}

        if (!(PlayerInfo[playerid][pSettings] & togHUD)) ShowHudTextDraw(playerid);

		if (PlayerInfo[playerid][pInjured])
		{
			if (PlayerInfo[playerid][pInjured] == 3) pTemp[playerid][pInjuredTime] = 120;
			else									pTemp[playerid][pInjuredTime] = 60;
		}

		if (PlayerInfo[playerid][pAdmin] > 0)		SendServerMessage(playerid, "Ai fost logat cu administrator nivel %i. (/ah)", PlayerInfo[playerid][pAdmin]);
		else if (PlayerInfo[playerid][pHelper] == 1)	SendServerMessage(playerid, "Ai fost logat cu functia de tester. (/testerhelp)");	
		
		foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) && i != playerid)	SendClientMessageEx(i, COLOR_GRAD2, "* %s entrou no servidor.", GetNameEx(playerid));
			else {
				if (!(PlayerInfo[i][pSettings] & togJoin))
				{
					if (CheckAdmin(i, ADMIN_LEVEL_1)) 
					{
						SendClientMessageEx(i, COLOR_GRAD2, "*** %s (ID: %i) entrou no servidor (IP: %s, dinheiro: $%s, banco: $%s)", GetNameEx(playerid),playerid, q_GetPlayerIP(playerid), FormatNumber(PlayerInfo[playerid][pCash]), FormatNumber(PlayerInfo[playerid][pBank]));
					}
				}
			}
		}


		if (PlayerInfo[playerid][pOnDuty] && PlayerInfo[playerid][pFaction] && IsFactionLegal(PlayerInfo[playerid][pFaction]))
		{
		    SetPlayerColor(playerid, RemoveAlpha(Factions[PlayerInfo[playerid][pFaction]][fColor]));
		    SendClientMessageEx(playerid, COLOR_GRAD2, "{%06x}[%s]{FFFFFF} Você voltou ao serviço.%s", Factions[PlayerInfo[playerid][pFaction]][fColor] >>> 8, Factions[PlayerInfo[playerid][pFaction]][fShortName], IsFactionPolice(PlayerInfo[playerid][pFaction]) ? " (Status: 10-7 EM SERVIÇO)" : "");
			if(IsFactionPolice(PlayerInfo[playerid][pFaction])) pTemp[playerid][tempStatus] = 1;
		}

		new count = CountPlayerTickets(playerid);
		if (count) SendSyntaxMessage(playerid, "Você tem multas não pagas - %i.", count);

		if (!PlayerInfo[playerid][pNumber]) {
		    SetPlayerNumber(playerid);
		}

		SQL_SetInteger("users", "last_login", gettime(), PlayerInfo[playerid][pID]);
		SQL_SetString("users", "last_ip", q_GetPlayerIP(playerid), PlayerInfo[playerid][pID]);

		if (PlayerInfo[playerid][pHours] < 8) SendHelpMessage(playerid, "Quando você atingir 8 horas jogadas no servidor, poderá receber um bônus de $%i.", SERVER_BONUS);

		SetCash(playerid, PlayerInfo[playerid][pCash]);

		if (PlayerInfo[playerid][pSpawnPoint] == 0)
		{
		    SendHelpMessage(playerid, "Você sempre pode editar o local de spawn através do /mm.");
		    Dialog_Show(playerid, FirstSpawn, DIALOG_STYLE_LIST, "{8D8DFF}Escolha um local de spawn permanente", "Palomino Creek\nMontgomery\nDillimore\nFort Carson", "Selecionar", "");
		} else {
		    pTemp[playerid][pLogged] = 2;
		    
		    SetSpawnInfoEx(playerid, PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
		    TogglePlayerSpectating(playerid, 0);
		    SpawnPlayer(playerid);
		}
	}
	else
	{
	    SendErrorMessage(playerid, "Personagem não encontrado.");
	    KickEx(playerid);
	}
	return 1;
}

this::OnAccountCheck(playerid)
{
    if (IsPlayerNPC(playerid))	return 1;

    if (!cache_num_rows())
    {
        #if REGISTRATION == 0
            SendErrorMessage(playerid, "Infelizmente, você não está registrado neste servidor.");
            SendServerMessage(playerid, "O registro no servidor é feito através do Painel de Controle do Usuário do servidor.");
            SendServerMessage(playerid, "Você pode se registrar em "WeburlName"");
            KickEx(playerid);
        #else
            ClearChatbox(playerid, 50);
            SendServerMessage(playerid, "Bem-vindo ao {FFC46A}"WeburlName"{FFFFFF}.");
            SendServerMessage(playerid, "Agora você precisa registrar uma conta única, depois disso você poderá começar a criar personagens.");

            Dialog_Show(playerid, AccountRegister, DIALOG_STYLE_INPUT, "Registro de Conta", "Digite o apelido desejado (24 caracteres):\n{C7C7C7}Exemplo: {FFFFFF}danny", "Registrar", "");
        #endif

        return 1;
    }

    new last_ip[24];

    PlayerInfo[playerid][pAccID] = cache_get_field_content_int(0, "id");
    cache_get_field_content(0, "login", PlayerInfo[playerid][pAccname], dbHandle, MAX_PLAYER_NAME+1);
    cache_get_field_content(0, "last_game_ip", last_ip);
    cache_get_field_content(0, "google_auth", PlayerInfo[playerid][pGoogle], dbHandle, 17);

    if (PlayerInfo[playerid][pAccID] < 1)
    {
        SendErrorMessage(playerid, "Erro de conta, por favor entre novamente no servidor.");
        KickEx(playerid);
        return 1;
    }

    new name[MAX_PLAYER_NAME];

    GetPlayerName(playerid, name, sizeof(name));

    if (!CompareStrings(PlayerInfo[playerid][pAccname], name) && SetPlayerName(playerid, PlayerInfo[playerid][pAccname]) != 1)
    {
        SendErrorMessage(playerid, "Esta conta já está em uso.");
        KickEx(playerid);
        return 1;
    }

    new query[170], sub[8];

    GetPlayerIp(playerid, sub, sizeof(sub));

    new pos;

    for(new i; i < 2; i++)
    {
        pos = strfind(sub, ".", true, pos+1);
    }

    if (pos != -1) strdel(sub, pos, 24);

    new pip[24];

    GetPlayerIp(playerid, pip, sizeof(pip));

    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `bans` WHERE `name` = '%e' OR `ip`='%s' OR `ip`='%s'", PlayerInfo[playerid][pAccname], sub, pip);
    mysql_tquery(dbHandle, query, "OnPlayerAttemptLogin", "is", playerid, last_ip);
    return 1;
} 

this::LoginPlayer(playerid)
{
    if (cache_num_rows() == 0)
    {
        if (pTemp[playerid][pLoggedTry]++ > 3) return KickEx(playerid);
        
        static const str_size[] = "{c7c3c3}SERVIDOR:\t{FFFFFF}Você tem 5 minutos para logar\n\
                                   {c7c3c3}DICA:\t\t{FFFFFF}Você pode recuperar sua senha\n\
                                   \t\t no endereço: {c7c3c3}"WeburlName"\n\
                                   \t\t{FFFFFF}acessando a área do ucp\n\n\
                                   Digite a senha:"; // original message placeholder
        new mes[sizeof(str_size)+103];

        format(mes, sizeof(mes), "{FF6347}[ ! ] {FFFFFF}Você digitou uma senha incorreta!\n{FF6347}[ ! ] {FFFFFF}Tentativas restantes: {FF6347}%d\n\n%s", 3-pTemp[playerid][pLoggedTry], str_size);   
        Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Bem-vindo ao "ProjectName"", mes, "Login", "Cancelar");
        SendWarningMessage(playerid, "Você digitou uma senha incorreta.");
        return 1;
    }

    pTemp[playerid][pLoggedTime] = 0;
    
    new query[128];

    mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `loginlog` (accountid, ip, method, date) VALUES (%i, '%e', 'Login', '%e')", PlayerInfo[playerid][pAccID], q_GetPlayerIP(playerid), GetFullDate());
    mysql_tquery(dbHandle, query);
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `last_game_ip` = '%e', `last_game_login` = '%e' WHERE `id` = %i", q_GetPlayerIP(playerid), GetFullDate(), PlayerInfo[playerid][pAccID]);
    mysql_tquery(dbHandle, query);

    ShowCharacterMenu(playerid);
    return 1;
}

this::SearchCharacters(playerid)
{
    new list;
    #if REGISTRATION == 0
        if (!cache_num_rows())
        {
            SendErrorMessage(playerid, "Não existem personagens ativos nesta conta.");
            KickEx(playerid);
            return 1;
        }
        list = cache_num_rows();
        if (list > MAX_CHARACTERS) list = MAX_CHARACTERS;
    #else
        for(new i = 0; i != MAX_CHARACTERS; i++)
        {
            login_char_id[playerid][i] = 0;
            format(login_char_name[playerid][i], MAX_PLAYER_NAME, "Espaço vazio");
        }
        list = MAX_CHARACTERS;
    #endif

    new status[MAX_CHARACTERS], status_names[][] = {"{DABB3E}Pendente", "{33AA33}Aprovado", "{FF6347}Rejeitado", "{FF6347}Banido"}, mes[43 + ((MAX_PLAYER_NAME + 64) * 3)], date[24];

    format(mes, sizeof(mes), "Slot\tNome\tÚltimo login\tStatus");

    for(new i = 0; i < list; i++)
    {	   
        if (i < cache_num_rows()) {
            if(cache_get_field_content_int(i, "accountid") == 0) {
                format(mes, sizeof(mes), "%s\n%i\t{C7C7C7}%s", mes, i+1, login_char_name[playerid][i]);
                continue;
            }

            if (cache_get_field_content_int(i, "accountid") != PlayerInfo[playerid][pAccID])
            {
                SendErrorMessage(playerid, "Erro de conta. Por favor, faça login novamente!");
                KickEx(playerid);
                return 1;
            }

            status[i] = cache_get_field_content_int(i, "status");
            login_char_id[playerid][i] = cache_get_field_content_int(i, "id");
            cache_get_field_content(i, "name", login_char_name[playerid][i], dbHandle, MAX_PLAYER_NAME+1);
            strreplace(login_char_name[playerid][i], '_', ' ');
            
            format(date, sizeof(date), "%s", to_date(cache_get_field_content_int(i, "last_login"), 1));
        }
        
        if (login_char_id[playerid][i])	format(mes, sizeof(mes), "%s\n%i\t%s\t%s\t%s", mes, i+1, login_char_name[playerid][i], date, status_names[status[i]]);
        else							format(mes, sizeof(mes), "%s\n%i\t%s", mes, i+1, login_char_name[playerid][i]);
    }

    Dialog_Show(playerid, SelectChar, DIALOG_STYLE_TABLIST_HEADERS, "Escolha um personagem:", mes, "Selecionar", "Cancelar");
    return 1;
}

this::OnPlayerAttemptLogin(playerid, ultimo_ip[])
{
    if (cache_num_rows() != 0) {
        new motivo[64], ip[24], data[24], nome[24], administrador[MAX_PLAYER_NAME];
        
        cache_get_field_content(0, "motivo", motivo);
        cache_get_field_content(0, "data", data);
        cache_get_field_content(0, "ip", ip);
        cache_get_field_content(0, "administrador", administrador);
        cache_get_field_content(0, "nome", nome);
        
        new tempo = cache_get_field_content_int(0, "tempo");

        if (gettime() < tempo) 
        {
            new mensagem[512];
            if (CompareStrings(ip, "-") != 0)
            {
                format(mensagem, sizeof(mensagem), "{FFFFFF}Sua conta foi bloqueada pelo administrador {B5B5B5}%s.\n{FFFFFF}Data do bloqueio: {B5B5B5}%s\n{FFFFFF}Motivo: {B5B5B5}%s\n\nVocê pode ir ao fórum para contestar o bloqueio.\n\
                Para sair pressione F6 e digite /q", administrador, data, motivo);
            }
            else
            {
                format(mensagem, sizeof(mensagem), "{FFFFFF}Seu IP (%s) foi bloqueado pelo administrador {B5B5B5}%s.\n{FFFFFF}Data do bloqueio: {B5B5B5}%s\n{FFFFFF}Motivo: {B5B5B5}%s\n\nVocê pode ir ao fórum para contestar o bloqueio.\n\
                Para sair pressione F6 e digite /q", ip, administrador, data, motivo);
            }
            
            Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Bloqueio", mensagem, "/q", "");
            KickEx(playerid);
            return 1;
        } 
        else 
        {
            new consulta[128];
            mysql_format(dbHandle, consulta, sizeof(consulta), "DELETE FROM `bans` WHERE `nome` = '%e'", nome);
            mysql_tquery(dbHandle, consulta);
        }
    }

    new ip[24];

    GetPlayerIp(playerid, ip, sizeof(ip));

    pTemp[playerid][pLogged] = 1;
    ClearChatbox(playerid, 50);
    SendServerMessage(playerid,"Versão do modo de jogo: {FFC46A}"GameModeVersion".");
    SendServerMessage(playerid,"Última atualização:{FFC46A} "GameModeUpdate".");

    if (CompareStrings(ip, ultimo_ip) || CompareStrings(PlayerInfo[playerid][pGoogle], "!"))
    {
        static const tamanho_str[] = "{c7c3c3}SERVIDOR:\t{FFFFFF}Você tem 5 minutos para se logar\n\
                                       {c7c3c3}DICA:\t\t{FFFFFF}Você pode recuperar sua senha\n\
                                       \t\t no endereço: {c7c3c3}"WeburlName"\n\
                                       \t\t{FFFFFF}acessando a área do ucp\n\n\
                                       Insira a senha:"; // Espaço para personalização

        new mensagem[sizeof(tamanho_str)];

        format(mensagem, sizeof(mensagem), tamanho_str);
        Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Bem-vindo ao "ProjectName"", mensagem, "Login", "Cancelar");
    } else {
        Dialog_Show(playerid, ConfirmWord, DIALOG_STYLE_PASSWORD, "{c7c3c3}Google Authenticator", "{FFFFFF}Você está se logando de um endereço IP não verificado.\nInsira o código do aplicativo Google Authenticator para confirmar que você é o dono da conta.", ">>>", "");
    }
    return 1;
}

ShowCharacterMenu(playerid)
{
    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `accountid` = %i and status<4", PlayerInfo[playerid][pAccID]);
	mysql_tquery(dbHandle, query, "SearchCharacters", "i", playerid);
	return 1;
}

SetPlayerSpawn(playerid)
{
	if (IsPlayerNPC(playerid)) return 1;

	if (pTemp[playerid][pSpecPlayer] != INVALID_PLAYER_ID)
	{
	    pTemp[playerid][pSpecPlayer] = INVALID_PLAYER_ID;
		SetInterior(playerid,PlayerInfo[playerid][pInt]);
		SetWorld(playerid, PlayerInfo[playerid][pWorld]);
		SetPlayerPos(playerid,PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
		PlayerTextDrawHide(playerid, spectate_TD[playerid][0]);
		PlayerTextDrawHide(playerid, spectate_TD[playerid][1]);
		return 1;
	}

    else if (pTemp[playerid][pCCTV])
	{
	    pTemp[playerid][pCCTV] = 0;
        DeletePVar(playerid, "CCTV:ID");

		SetInterior(playerid,PlayerInfo[playerid][pInt]);
		SetWorld(playerid, PlayerInfo[playerid][pWorld]);
		SetPlayerPos(playerid,PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
	    return 1;
	}

	else if (pTemp[playerid][pInteriorBiz])
	{
	    new bizid = pTemp[playerid][pInteriorBiz]-1;

		SetPlayerPos(playerid,BizInfo[bizid][bizExitX], BizInfo[bizid][bizExitY], BizInfo[bizid][bizExitZ]);
		SetInterior(playerid,BizInfo[bizid][bizExitInt]);
		SetWorld(playerid,BizInfo[bizid][bizExitVW]);

        pTemp[playerid][pInteriorBiz] = 0;
        pTemp[playerid][pInteriorID] = 0;
        pTemp[playerid][pInteriorHouse] = 0;
	    return 1;
	}
	
	else if (pTemp[playerid][pZond])
	{
	    pTemp[playerid][pZond] = 0;
		SetInterior(playerid,PlayerInfo[playerid][pInt]);
		SetWorld(playerid, PlayerInfo[playerid][pWorld]);
		SetPlayerPos(playerid,PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
	    return 1;
	}

	if (PlayerInfo[playerid][pAJailed] != 0)
	{
		SetInterior(playerid, 18);
		SetWorld(playerid, playerid + 100);
		SetPlayerPos(playerid, -219.4580,1410.6747,27.7734);
		SendSyntaxMessage(playerid, "Você foi retornado para a prisão administrativa. Tempo restante: %i minutos.", PlayerInfo[playerid][pAJailTime]/60);
		return 1;
	}
	else if (PlayerInfo[playerid][pJailed]) return PutPlayerInPrison(playerid);

	// TIPOS DE SPAWN
	if (PlayerInfo[playerid][pInjured] || GetPVarInt(playerid, #ultimo_login) || PlayerInfo[playerid][pSpawnType] == 3)
	{
		SetPlayerPos(playerid, PlayerInfo[playerid][pPosX], PlayerInfo[playerid][pPosY], PlayerInfo[playerid][pPosZ]);
		SetPlayerFacingAngle(playerid, PlayerInfo[playerid][pPosA]);
		SetInterior(playerid, PlayerInfo[playerid][pInt]);
		SetWorld(playerid, PlayerInfo[playerid][pWorld]);
		
		DeletePVar(playerid, #ultimo_login);
		return 1;
	}

	else if (PlayerInfo[playerid][pSpawnType] == 1)
	{
		new houseid = GetHouseIDByMysql(PlayerInfo[playerid][pSpawnPoint]);
		if (houseid == -1 || (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID] && PlayerInfo[playerid][pRentKey] != PlayerInfo[playerid][pSpawnPoint]))
		{
			SendSyntaxMessage(playerid, "Você não tem mais uma casa, ou você não a aluga mais.");
			PlayerInfo[playerid][pSpawnType] = 0;
			PlayerInfo[playerid][pSpawnPoint] = 1;
			SendHelpMessage(playerid, "Seu local de spawn foi automaticamente alterado para Palomino Creek. (/config)");
		}
		else
		{
			SetPlayerPos(playerid, HouseInfo[houseid][hExitX], HouseInfo[houseid][hExitY], HouseInfo[houseid][hExitZ]);
			SetInterior(playerid, HouseInfo[houseid][hExitInt]);
			SetWorld(playerid, HouseInfo[houseid][hExitVW]);
			pTemp[playerid][pHouseEntered] = houseid;
			return 1;
		}
	}

	else if (PlayerInfo[playerid][pSpawnType] == 2)
	{
		if (PlayerInfo[playerid][pFaction] == 0)
		{
			SendSyntaxMessage(playerid, "Você não faz mais parte de uma facção.");
			PlayerInfo[playerid][pSpawnType] = 0;
			PlayerInfo[playerid][pSpawnPoint] = 1;
			SendHelpMessage(playerid, "Seu local de spawn foi automaticamente alterado para Palomino Creek. (/config)");
		}
		else
		{
			SetPlayerPos(playerid, Factions[PlayerInfo[playerid][pFaction]][fPosX], Factions[PlayerInfo[playerid][pFaction]][fPosY], Factions[PlayerInfo[playerid][pFaction]][fPosZ]);
			SetInterior(playerid, Factions[PlayerInfo[playerid][pFaction]][fSpawnInt]);
			SetWorld(playerid, Factions[PlayerInfo[playerid][pFaction]][fSpawnVW]);
			return 1;
		}
	}


	else if (PlayerInfo[playerid][pSpawnType] == 0)
	{		        
	   	SetPlayerPos(playerid, first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][0], first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][1], first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][2]);
	    SetPlayerFacingAngle(playerid, first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][3]);
		SetInterior(playerid,0);
		SetWorld(playerid, 0);
		return 1;
	}

	return 1;
}

stock RPnamecheck(pname[])
{
	if(strfind(pname,"[",true) != (-1)) return 0;
    else if(strfind(pname,"]",true) != (-1)) return 0;
    else if(strfind(pname,"$",true) != (-1)) return 0;
    else if(strfind(pname,"(",true) != (-1)) return 0;
    else if(strfind(pname,")",true) != (-1)) return 0;
    else if(strfind(pname,"=",true) != (-1)) return 0;
    else if(strfind(pname,"@",true) != (-1)) return 0;
    else if(strfind(pname,"1",true) != (-1)) return 0;
    else if(strfind(pname,"2",true) != (-1)) return 0;
    else if(strfind(pname,"3",true) != (-1)) return 0;
    else if(strfind(pname,"4",true) != (-1)) return 0;
    else if(strfind(pname,"5",true) != (-1)) return 0;
    else if(strfind(pname,"6",true) != (-1)) return 0;
    else if(strfind(pname,"7",true) != (-1)) return 0;
    else if(strfind(pname,"8",true) != (-1)) return 0;
    else if(strfind(pname,"9",true) != (-1)) return 0;
    else if(strfind(pname,"fuck",true) != (-1)) return 0;
    else if(strfind(pname,"FUCK",true) != (-1)) return 0;
    else if(strfind(pname,"Boobies",true) != (-1)) return 0;
    else if(strfind(pname,"Tupac_Shakur",true) != (-1)) return 0;
    else if(strfind(pname,"Pussy",true) != (-1)) return 0;
    else if(strfind(pname,"Rape",true) != (-1)) return 0;
    else if(strfind(pname,"kill",true) != (-1)) return 0;
    else if(strfind(pname,"shit",true) != (-1)) return 0;
    else if(strfind(pname,"ass",true) != (-1)) return 0;
    else if(strfind(pname,"Jack_Black",true) != (-1)) return 0;
    else if(strfind(pname,"Max_Kenton",true) != (-1)) return 0;
    else if(strfind(pname,"Will_Smith",true) != (-1)) return 0;
    else if(strfind(pname,"Jaden_Smith",true) != (-1)) return 0;
    else if(strfind(pname,"Megan_Fox",true) != (-1)) return 0;
    else if(strfind(pname,"Charlie_Kenton",true) != (-1)) return 0;
    else if(strfind(pname,"Hugh_Hefner",true) != (-1)) return 0;
    else if(strfind(pname,"Paris_Hilton",true) != (-1)) return 0;
    else if(strfind(pname,"Marshall_Mathers",true) != (-1)) return 0;
    else if(strfind(pname,"Sheldon_Cooper",true) != (-1)) return 0;
    else if(strfind(pname,"Jet_Lee",true) != (-1)) return 0;
    else if(strfind(pname,"Jackie_Chan",true) != (-1)) return 0;
    else if(strfind(pname,"Chuck_Norris",true) != (-1)) return 0;
    else if(strfind(pname,"Peter_Parker",true) != (-1)) return 0;
    else if(strfind(pname,"Spider_Man",true) != (-1)) return 0;
    else if(strfind(pname,"Bat_Man",true) != (-1)) return 0;
    else if(strfind(pname,"Emma_Stone",true) != (-1)) return 0;
    else if(strfind(pname,"whore",true) != (-1)) return 0;
    else if(strfind(pname,"Hugh_Jackman",true) != (-1)) return 0;
    else if(strfind(pname,"Charles_Kenton",true) != (-1)) return 0;
    else if(strfind(pname,"Harry_Potter",true) != (-1)) return 0;
    else if(strfind(pname,"Chris_Hemsworth",true) != (-1)) return 0;
    else if(strfind(pname,"Penis",true) != (-1)) return 0;
    else if(strfind(pname,"_Dick",true) != (-1)) return 0;
    else if(strfind(pname,"Vagina",true) != (-1)) return 0;
    else if(strfind(pname,"Cock",true) != (-1)) return 0;
    else if(strfind(pname,"Rectum",true) != (-1)) return 0;
    else if(strfind(pname,"Sperm",true) != (-1)) return 0;
    else if(strfind(pname,"Rektum",true) != (-1)) return 0;
    else if(strfind(pname,"Pistol",true) != (-1)) return 0;
    else if(strfind(pname,"AK47",true) != (-1)) return 0;
    else if(strfind(pname,"Shotgun",true) != (-1)) return 0;
    else if(strfind(pname,"Cum",true) != (-1)) return 0;
    else if(strfind(pname,"Hitler",true) != (-1)) return 0;
    else if(strfind(pname,"Jesus",true) != (-1)) return 0;
    else if(strfind(pname,"God",true) != (-1)) return 0;
    else if(strfind(pname,"Shotgun",true) != (-1)) return 0;
    else if(strfind(pname,"Desert_Eagle",true) != (-1)) return 0;
    else if(strfind(pname,"fucker",true) != (-1)) return 0;
    else if(strfind(pname,"Retard",true) != (-1)) return 0;
    else if(strfind(pname,"Tarded",true) != (-1)) return 0;
    else if(strfind(pname,"fanny",true) != (-1)) return 0;
    else if(strfind(pname,"Daniel_Hardy",true) != (-1)) return 0;
    else if(strfind(pname,"abcdefghijklmnopqrstuvwxyz",true) != (-1)) return 0;
   
    return 1;
}