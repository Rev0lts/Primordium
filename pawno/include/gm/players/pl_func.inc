this::KickPlayer(playerid)
{
    if (GetPVarInt(playerid,"kicked") != 1)	return 1;
	
    DeletePVar(playerid,"kicked");
	Kick(playerid);
	return 1;
}

this::Float:GetPlayerDistanceFromPlayer(playerid, targetid)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(targetid, x, y, z);
	return GetPlayerDistanceFromPoint(playerid, x, y, z);
}

/*PlayerEnterFreeze(playerid)
{
	FreezePlayer(playerid);
	SetTimer("PlayerEnterUnFreeze", 2000, false);
	return 1;
}*/

this::PlayerEnterUnFreeze(playerid)
{
	UnFreezePlayer(playerid);
	return 1;
}

UnFreezePlayer(playerid)
{
	DeletePVar(playerid, "Freeze");
	TogglePlayerControllable(playerid, 1);
	return 1;
}

FreezePlayer(playerid)
{
	SetPVarInt(playerid, "Freeze", 1);
	TogglePlayerControllable(playerid, 0);
	return 1;
}

IsPlayerFreeze(playerid)
{
	if (GetPVarInt(playerid, "Freeze")) return 1;

	return 0;
}

SetPlayerSkinEx(playerid, skinid)
{
	PlayerInfo[playerid][pSkin] = skinid;
	SetSpawnInfoEx(playerid, 1642.2006, -2334.4751, -2.6797);
	return SetPlayerSkin(playerid, skinid);
}

Float:GetPlayerHealthEx(playerid)
{
	new Float:hp;
	AntiCheatGetHealth(playerid, hp);
	return hp;
}

SetPlayerSkill(playerid, type = 1)
{
	if (type == 1) { // ��
		SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 899);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 1);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 1);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 1);
				
		SetPlayerSkillLevel(playerid, WEAPONSKILL_AK47, 999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_DESERT_EAGLE, 999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SHOTGUN, 999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_M4, 999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MP5, 999);
	} else if (type == 2) { // +-
		SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 200);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
				
		SetPlayerSkillLevel(playerid, WEAPONSKILL_AK47, 200);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_DESERT_EAGLE, 200);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SHOTGUN, 200);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_M4, 200);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MP5, 200);
	} else if (type == 3) { // �����
		SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 100);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
				
		SetPlayerSkillLevel(playerid, WEAPONSKILL_AK47, 50);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_DESERT_EAGLE, 50);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SHOTGUN, 100);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_M4, 50);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MP5, 50);
	}	
	return 1;
}

IsPlayerAFK(playerid)
{
	return (pTemp[playerid][pDarkAFK] > 5)?(1):(0);
}

IsPlayerInCube(playerid, Float:min_x, Float:min_y, Float:max_x, Float:max_y)
{
    new Float: x, Float: y, Float: z;

    GetPlayerPos(playerid, x, y, z);
    return ((x <= max_x && x >= min_x) && (y <= max_y && y >= min_y))?(1):(0);
}

IsValidPlayerName(const str[])
{
	if (!str[0] || str[0] == '\1') return 0;

	for (new i = 0, l = strlen(str); i != l; i ++) {
	    if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))	continue;
		if (str[i] == '_' || str[i] == '$' || str[i] == '@' || str[i] == '[' || str[i] == ']')	continue;
		else return 0;
	}
	return 1;
}

SendPlayerToPlayer(playerid, targetid)
{
	new Float:x, Float:y, Float:z;
	
	GetPlayerPos(targetid, x, y, z);

    if (GetInterior(targetid) || GetWorld(targetid))
	{
   		PlayerInfo[playerid][pLocationPos][0] = PlayerInfo[targetid][pLocationPos][0];
	    PlayerInfo[playerid][pLocationPos][1] = PlayerInfo[targetid][pLocationPos][0];
		PlayerInfo[playerid][pLocationPos][2] = PlayerInfo[targetid][pLocationPos][0];
	}

	SetPlayerPosEx(playerid, x + 1, y, z);
	SetInterior(playerid, GetInterior(targetid));
	SetWorld(playerid, GetWorld(targetid));
	pTemp[playerid][pHouseEntered] = pTemp[targetid][pHouseEntered];
	pTemp[playerid][pApartEntered] = pTemp[targetid][pApartEntered];
	pTemp[playerid][pBizEntered] = pTemp[targetid][pBizEntered];
	return 1;
}

ShowSettings(playerid)
{
    new text[128], string[1024];
        
    switch(PlayerInfo[playerid][pWalk])//0
    {
        case 0: format(text, sizeof(text), "Estilo de caminhada (/walk)\t[Desativado]\n");
        default: format(text, sizeof(text), "Estilo de caminhada (/walk)\t[%i]\n", PlayerInfo[playerid][pWalk]);
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pTalk])//1
    {
        case 0: format(text, sizeof(text), "Estilo de conversação\t[Desativado]\n");
        default: format(text, sizeof(text), "Estilo de conversação\t[%i]\n", PlayerInfo[playerid][pTalk]);
    }
    strcat(string, text);
    
    format(text, sizeof(text), "Estilo de combate\t[%s]\n", GetFightStyleName(playerid));//2
    strcat(string, text);
    
    format(text, sizeof(text), "Estilo do velocímetro\t[%i]\n", PlayerInfo[playerid][pHud]);//3
    strcat(string, text);

    switch(PlayerInfo[playerid][pSpawnType])//4
    {
        case 0: format(text, sizeof(text), "Local de surgimento\t[Padrão]\n");
        case 1: format(text, sizeof(text), "Local de surgimento\t[Casa #%i]\n", PlayerInfo[playerid][pSpawnPoint]);
        case 2: format(text, sizeof(text), "Local de surgimento\t[Fação]\n");
        case 3: format(text, sizeof(text), "Local de surgimento\t[Última saída]\n");
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togOOC)//5
    {
        case 0: text = "Chat OOC\t[Ativo]\n";
        default: text = "Chat OOC\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togName)//6
    {
        case 0: text = "Apelidos dos jogadores\t[Ativo]\n";
        default: text = "Apelidos dos jogadores\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togAdv)//7
    {
        case 0: text = "Publicidade no chat\t[Ativo]\n";
        default: text = "Publicidade no chat\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togJoin)//8
    {
        case 0: text = "Informações de login / logout dos jogadores\t[Ativo]\n";
        default: text = "Informações de login / logout dos jogadores\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togGov)//9
    {
        case 0: text = "Anúncios do governo (/gov)\t[Ativo]\n";
        default: text = "Anúncios do governo (/gov)\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togPM)//10
    {
        case 0: text = "Mensagens privadas\t[Ativo]\n";
        default: text = "Mensagens privadas\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togAdmCmds)//11
    {
        case 0: text = "Alertas de Admin\t[Ativo]\n";
        default: text = "Alertas de Admin\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togNews)//12
    {
        case 0: text = "Notícias no chat\t[Ativo]\n";
        default: text = "Notícias no chat\t[Desativado]\n";
    }
    strcat(string, text);

    switch(PlayerInfo[playerid][pSettings] & togAFK)//13
    {
        case 0: text = "Mostrar tempo gasto em AFK\t[Ativo]\n";
        default: text = "Mostrar tempo gasto em AFK\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togHUD)//14
    {
        case 0: text = "Mostrar HUD\t[Ativo]\n";
        default: text = "Mostrar HUD\t[Desativado]\n";
    }
    strcat(string, text);

    switch(PlayerInfo[playerid][pSettings] & togFaction)//15
    {
        case 0: text = "Chat da Fação\t[Ativo]\n";
        default: text = "Chat da Fação\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(PlayerInfo[playerid][pSettings] & togEnter)//16
    {
        case 0: text = "Entrar / sair com a tecla F\t[Ativo]\n";
        default: text = "Entrar / sair com a tecla F\t[Desativado]\n";
    }
    strcat(string, text);
    
    switch(CompareStrings(PlayerInfo[playerid][pGoogle], "!"))//17
    {
        case 0: text = "Google Authenticator\t[Ativo]\n";
        default: text = "Google Authenticator\t[Desativado]\n";
    }
    strcat(string, text);
    
    strcat(string, "Alterar senha");//18
    
    Dialog_Show(playerid, MainSettings, DIALOG_STYLE_TABLIST, "Configurações", string, "Selecionar", "Retornar");
    return 1;
}

PlayerNearPlayer(Float:radi, playerid, targetid)
{
	new Float:fX, Float:fY, Float:fZ;
	GetPlayerPos(targetid, fX, fY, fZ);
	return (GetInterior(playerid) == GetInterior(targetid) && GetWorld(playerid) == GetWorld(targetid)) && IsPlayerInRangeOfPoint(playerid, radi, fX, fY, fZ);
}

SavePlayerPos(playerid)
{
	GetPlayerPos(playerid, PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
	GetPlayerFacingAngle(playerid, PlayerInfo[playerid][pPosA]);
	PlayerInfo[playerid][pWorld] = GetWorld(playerid);
	PlayerInfo[playerid][pInt] = GetInterior(playerid);
	SetSpawnInfoEx(playerid, PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
	return 1;
}

GiveCash(playerid, cash)
{
 	PlayerInfo[playerid][pCash] += cash;
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, PlayerInfo[playerid][pCash]);

	new __cash = cash;

	if (cash < 0)	cash = -cash;

    new mes[19];
	format(mes, sizeof(mes), "%s$%i", (__cash < 0)?("~r~-"):("~g~+"), cash);
	GameTextForPlayer(playerid, mes, 1000, 1);
	return 1;
}

SetCash(playerid, cash)
{
	PlayerInfo[playerid][pCash] = cash;
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, cash);
	return 1;
}

GetCash(playerid)
{
	return PlayerInfo[playerid][pCash];
}

SendClientMessageEx(playerid, color, const str[], {Float,_}:...)
{
	static args, start, end, string[144];
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 12)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 12); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string
		#emit PUSH.C args
		#emit SYSREQ.C format

		SendClientMessage(playerid, color, string);

		#emit LCTRL 5
		#emit SCTRL 4
		#emit RETN
	}
	return SendClientMessage(playerid, color, str);
}

stock SendClientMessageToAllEx(color, const str[], {Float,_}:...)
{
	static
	    args,
	    start,
	    end,
	    string[144]
	;
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

		#emit LOAD.pri args
		#emit ADD.C 4
		#emit PUSH.pri
		#emit SYSREQ.C format

        #emit LCTRL 5
		#emit SCTRL 4

		foreach (new i : Player) {
			SendClientMessage(i, color, string);
		}
		return 1;
	}
	return SendClientMessageToAll(color, str);
} 

SendNearbyMessage(Float:radius, playerid, color, const str[], {Float,_}:...)
{
	static args, start, end, string[144];
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 16)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 16); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit CONST.alt 4
		#emit SUB
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach (new i : Player)
		{
		    if (0 == IsPlayerLogged(i) || 0 == PlayerNearPlayer(radius, i, playerid)) continue;
			SendClientMessage(i, color, string);
		}
		return 1;
	}

	foreach (new i : Player)
	{
	    if (0 == IsPlayerLogged(i) || 0 == PlayerNearPlayer(radius, i, playerid)) continue;
		SendClientMessage(i, color, str);
	}
	return 1;
}

SendLocalMessage(Float:radius, playerid, col1, col2, col3, col4, col5, const str[], {Float,_}:...)
{
	static args, start, end, string[144];
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 16)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 16); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit CONST.alt 4
		#emit SUB
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach (new i : Player)
		{
		    if (0 == IsPlayerLogged(i)) 							continue;
			if (PlayerNearPlayer(radius/16, i, playerid))		SendClientMessage(i, col1, string);
            else if (PlayerNearPlayer(radius/8, i, playerid))	SendClientMessage(i, col2, string);
            else if (PlayerNearPlayer(radius/4, i, playerid))	SendClientMessage(i, col3, string);
            else if (PlayerNearPlayer(radius/2, i, playerid))	SendClientMessage(i, col4, string);
            else if (PlayerNearPlayer(radius, i, playerid))		SendClientMessage(i, col5, string);
			//SetPlayerChatBubble(i, string, col5, 5, 5000);
		}
		return 1;
	}

	foreach (new i : Player)
	{
	    if (0 == IsPlayerLogged(i))							continue;
	    if (PlayerNearPlayer(radius/16, i, playerid))		SendClientMessage(i, col1, string);
		else if (PlayerNearPlayer(radius/8, i, playerid))	SendClientMessage(i, col2, string);
		else if (PlayerNearPlayer(radius/4, i, playerid))	SendClientMessage(i, col3, string);
		else if (PlayerNearPlayer(radius/2, i, playerid))	SendClientMessage(i, col4, string);
		else if (PlayerNearPlayer(radius, i, playerid))		SendClientMessage(i, col5, string);
		//SetPlayerChatBubble(i, string, col5, 5, 5000);
	}
	return 1;
}

SendPMToPlayer(playerid, targetid, text[])
{
    new pos = MAXLEN;
    if (pos < MAXLEN-1) pos = MAXLEN;

    if (strlen(text) > MAXLEN)
    {
        if (IsAdminDuty(playerid)) SendClientMessageEx(targetid, COLOR_PMIN, "(( MP de {FF9A00}%s{FFDB18} (ID: %i): %.*s ... ))", GetName(playerid), playerid, pos, (text));
        else                        SendClientMessageEx(targetid, COLOR_PMIN, "(( MP de %s (ID: %i): %.*s ... ))", GetName(playerid), playerid, pos, (text));
        SendClientMessageEx(targetid, COLOR_PMIN, "(( ... %s ))", (text)[pos]);
        
        if (IsAdminDuty(targetid)) SendClientMessageEx(playerid, COLOR_PMOUT, "(( MP para {FF9A00}%s{FFDB18} (ID: %i): %.*s ... ))", GetName(targetid), targetid, pos, (text));
        else                        SendClientMessageEx(playerid, COLOR_PMOUT, "(( MP para %s (ID: %i): %.*s ... ))", GetName(targetid), targetid, pos, (text));
        SendClientMessageEx(playerid, COLOR_PMOUT, "(( ... %s ))", (text)[pos]);
    }
    else
    {
        if (IsAdminDuty(playerid))  SendClientMessageEx(targetid, COLOR_PMIN, "(( MP de {FF9A00}%s{FFDB18} (ID: %i): %s ))", GetName(playerid), playerid, (text));
        else                        SendClientMessageEx(targetid, COLOR_PMIN, "(( MP de %s (ID: %i): %s ))", GetName(playerid), playerid, (text));
        if (IsAdminDuty(targetid))  SendClientMessageEx(playerid, COLOR_PMOUT, "(( MP para {FF9A00}%s{FFDB18} (ID: %i): %s ))", GetName(targetid), targetid, (text));
        else                        SendClientMessageEx(playerid, COLOR_PMOUT, "(( MP para %s (ID: %i): %s ))", GetName(targetid), targetid, (text));
    }

    foreach(new i : Player)
    {
        if (!IsPlayerLogged(i) || !IsAdminDuty(i) || (pTemp[i][pPMPlayer] != playerid && GetPVarInt(i, #spypm_all) != 1)) continue;

        if (strlen(text) > MAXLEN)
        {
            SendClientMessageEx(i, COLOR_PMIN, "(( Espionagem de MP %s para %s (ID: %i): %.*s ... ))", GetName(playerid), GetName(targetid), targetid, pos, (text));
            SendClientMessageEx(i, COLOR_PMIN, "(( ... %s ))", (text)[pos]);
        }
        else
        {
            SendClientMessageEx(i, COLOR_PMIN, "(( Espionagem de MP %s para %s (ID: %i): %s ))", GetName(playerid), GetName(targetid), targetid, text);
        }
    }
    return 1;
}


GetPlayer2DZone(playerid, zone[], len)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (GetWorld(playerid) || GetInterior(playerid))
	{
		x = PlayerInfo[playerid][pLocationPos][0];
		y = PlayerInfo[playerid][pLocationPos][1];
		z = PlayerInfo[playerid][pLocationPos][2];
	}

	for(new i = 0; i != sizeof(gSAZones); i++)
	{
		if (x >= gSAZones[i][SAZONE_AREA][0] && x <= gSAZones[i][SAZONE_AREA][3] && y >= gSAZones[i][SAZONE_AREA][1] && y <= gSAZones[i][SAZONE_AREA][4]) return format(zone, len, gSAZones[i][SAZONE_NAME], 0);
	}
	return 0;
}

GetZone(Float:x, Float:y, zone[], len)
{
	for(new i = 0; i != sizeof(gSAZones); i++)
	{
		if (x >= gSAZones[i][SAZONE_AREA][0] && x <= gSAZones[i][SAZONE_AREA][3] && y >= gSAZones[i][SAZONE_AREA][1] && y <= gSAZones[i][SAZONE_AREA][4]) return format(zone, len, gSAZones[i][SAZONE_NAME], 0);
	}
	return format(zone, len, "Unknown");
}

ResetPlayerVariables(playerid)
{
    ResetDamages(playerid);
	for(new i = 0; i != MAX_CHARACTERS; i++)
	{
	    login_char_id[playerid][i] = 0;
	    format(login_char_name[playerid][i], MAX_PLAYER_NAME, "Slot vazio");
	}

	maskOn{playerid} = false;

	JackingEngine[playerid] =
	JackingLock[playerid] = 
	BreakingHouse[playerid] = 
	JackingVehicle[playerid] = 0;

	OnAnim{playerid} = false;

	Job_OnPlayerDisconnect(playerid);

	for(new i; i != MAX_PLAYERS; i++)
		people_blocking[playerid][i] = 0;

	for(new i = 0; i < MAX_PLAYER_CONTACTS; i++)
		PlayerContacts[playerid][i][contactOn] = 0;

	for(new i = 0; i < MAX_PLAYER_NOTES; i++)
		PlayerNotes[playerid][i][noteOn] = 0;

	for(new i; i != 30; i++)
		pTemp[playerid][pWeaponListed][i] = 0;

    for(new i; i != 5; i++)
		pTemp[playerid][pListed][i] = 0;
		
	for(new i = 0; i < MAX_INV_SLOTS; i++)
	{
		Inventory[playerid][i][pInvItem] = 0;
		Inventory[playerid][i][pInvItem] = 0;
		Inventory[playerid][i][pInvEx] = 0;
		Inventory[playerid][i][pInvQFloat] = 0;
		Inventory[playerid][i][pInvQ] = 0;
		Inventory[playerid][i][pInvName] = EOS;
	}

	pTemp[playerid][pChangeNick] = 
	pTemp[playerid][pCallName] =
	pTemp[playerid][pGoogle] = EOS;

	pTemp[playerid][pPayphone] = -1;
	
	//pTemp[playerid][JackingVehID] = 
	pTemp[playerid][pUnscrambleID] =
	//pTemp[playerid][pPunctePremium] =  
	pTemp[playerid][pScrambleFailed] = 
	pTemp[playerid][pScrambleSuccess] = 
	//pTemp[playerid][JackingCP] = 
	pTemp[playerid][JackingType] = 
	pTemp[playerid][JackingWarnings] = 
	pTemp[playerid][pPizzaBoy] = 
    pTemp[playerid][pDemolitionTime] = 
	pTemp[playerid][pDrugUse] = 
	pTemp[playerid][pDrugTime] = 
	pTemp[playerid][pDrugChgWeather] = 
	pTemp[playerid][tempItem] = 
	pTemp[playerid][tempEx] = 
	pTemp[playerid][tempQ] = 	
	pTemp[playerid][pLicenseExam] = 
	pTemp[playerid][pTesterDuty] = 
    pTemp[playerid][pAdminDuty] = 
	pTemp[playerid][pServeType] = 
	pTemp[playerid][pGreetType] = 
	pTemp[playerid][pFoodStand] = 
	pTemp[playerid][pDMLog] = 
	pTemp[playerid][pNews] = 
	pTemp[playerid][pDarkAFK] = 
	pTemp[playerid][pCuffed] = 
	pTemp[playerid][pFireTime] =
	pTemp[playerid][pPlantTime] =
	pTemp[playerid][pPlantType] =
    pTemp[playerid][pFishTime] =
	pTemp[playerid][pPatrol] =
    pTemp[playerid][pReportTime] =
	pTemp[playerid][pLogged] = 
	pTemp[playerid][pLoggedTime] =
	pTemp[playerid][pLoggedTry] = 
	pTemp[playerid][pJob] = 
	pTemp[playerid][pJobTime] = 
	pTemp[playerid][pRentPrice] =
	pTemp[playerid][pHousePrice] = 
	pTemp[playerid][pCarPrice] = 
	pTemp[playerid][pTaser] = 
	pTemp[playerid][pTaserTime] = 
	pTemp[playerid][pFreezeTime] =
	pTemp[playerid][pEditBort] = 
    pTemp[playerid][pCCTV] = 
	pTemp[playerid][pZond] = 
	pTemp[playerid][pDamagesCount] = 
	pTemp[playerid][pInjuredTime] = 
	pTemp[playerid][pInjuredLegTime] = 
	pTemp[playerid][pHelpUp3dText] = 
	pTemp[playerid][pFlashTime] =
	pTemp[playerid][pHealTime] = 
	pTemp[playerid][pBuildHouse] = 
	pTemp[playerid][pRamType] = 
    pTemp[playerid][pRamTime] = 
	pTemp[playerid][pCallStatus] = 
	pTemp[playerid][pCallIncoming] = 
	pTemp[playerid][pCallTime] = 
	pTemp[playerid][pWepPrice] = 
	PlayerInfo[playerid][pGanjaTime] = 
	PlayerInfo[playerid][pPrescribe] = 
	PlayerInfo[playerid][pDependence] =
	PlayerInfo[playerid][pDemolition] = 
	PlayerInfo[playerid][pID] = 
	PlayerInfo[playerid][pAccID] = 
	pTemp[playerid][pInteriorHouse] = 
	pTemp[playerid][pInteriorBiz] =
	pTemp[playerid][pInteriorID] = 
	pTemp[playerid][pCargoID] = 
	pTemp[playerid][pCallOn] = 
	pTemp[playerid][pEmergency] = 
	pTemp[playerid][pPoliceCalls] = 
	pTemp[playerid][pPoliceJails] = 
	pTemp[playerid][pPoliceFines] = 
	pTemp[playerid][UsingBort] = 
	pTemp[playerid][UsingCorpse] =
	pTemp[playerid][tempStatus] =
	pTemp[playerid][pTraceStage] = 0;

	pTemp[playerid][pBoombox_house] = 
	pTemp[playerid][pBoombox_biz] = 
	pTemp[playerid][pBizEntered] = 
	pTemp[playerid][pApartEntered] = 
	pTemp[playerid][pHouseEntered] = 
	pTemp[playerid][pHouseHouse] = 
	pTemp[playerid][pFurniture] = 
	pTemp[playerid][pFurnitureB] = 
	pTemp[playerid][p3DMenu] = 
    pTemp[playerid][pRamHouse] = 
	pTemp[playerid][pRamVeh] = 
    pTemp[playerid][pRamGarage] = 
    pTemp[playerid][pRamBiz] = 
	pTemp[playerid][pTraceGZ] = -1;

	pTemp[playerid][pBizPrice] = 0;
	pTemp[playerid][pBizID] = INVALID_PLAYER_ID;

	pTemp[playerid][pBoombox] = 
	pTemp[playerid][pServePlayer] = 
	pTemp[playerid][pGreetPlayer] = 
	pTemp[playerid][pMDCPlayer] = 
	pTemp[playerid][pWepPlayer] = 
	pTemp[playerid][pRentPlayer] = 
	pTemp[playerid][pPMPlayer] = 
	pTemp[playerid][pHousePlayer] = 
	pTemp[playerid][pInterview] = 
	pTemp[playerid][pBizPlayer] = 
	pTemp[playerid][pInterviewPlayer] = 
	pTemp[playerid][pHealPlayer] = 
	pTemp[playerid][pFactionPlayer] = 
	pTemp[playerid][pFriskPlayer] = 
	pTemp[playerid][pSpecPlayer] = 
	pTemp[playerid][pHirePlayer] = 
	pTemp[playerid][pCarPlayer] = 
	pTemp[playerid][pCallLine] = 
	pTemp[playerid][pTracePlayer] = INVALID_PLAYER_ID;

	pTemp[playerid][pDriver] = INVALID_VEHICLE_ID;
	pTemp[playerid][pZondActor] = INVALID_ACTOR_ID;
    pTemp[playerid][pRamObj] = INVALID_OBJECT_ID;

	for(new c = 0; c < 5; c++)
	{
		pTemp[playerid][MyCarSpawn][c] = 0;
		pTemp[playerid][MyCarSpawn][c] = 0;
	}

	for(new i = 0; i < MAX_WEAP-1; i++)	PlayerInfo[playerid][pHideWeap][i] = 0;

	PlayerInfo[playerid][pGoogle] = 
	PlayerInfo[playerid][pAccname] = 
	PlayerInfo[playerid][pBirthdate] = 
	PlayerInfo[playerid][pAdminCode] = EOS;

	PlayerInfo[playerid][pFishTime] = 
	PlayerInfo[playerid][pCarKey] =
	PlayerInfo[playerid][pTog][0] =
	PlayerInfo[playerid][pTog][1] = 
	PlayerInfo[playerid][pTog][2] = 
	PlayerInfo[playerid][pSwatDuty] = 
	PlayerInfo[playerid][pOnDuty] = 
	PlayerInfo[playerid][pCash] = 
	PlayerInfo[playerid][pBank] = 
	PlayerInfo[playerid][pSkin] = 
	PlayerInfo[playerid][pSex] = 
	PlayerInfo[playerid][pLevel] =
	//PlayerInfo[playerid][pExp] = 
	PlayerInfo[playerid][pHours] = 
	PlayerInfo[playerid][pAdmin] = 
	PlayerInfo[playerid][pAdminHide] = 
	PlayerInfo[playerid][pHelper] = 
	PlayerInfo[playerid][pInt] = 
	PlayerInfo[playerid][pWorld] =
	PlayerInfo[playerid][pJailed] = 
	PlayerInfo[playerid][pIsolateTime] = 
	PlayerInfo[playerid][pJailTime] = 
	PlayerInfo[playerid][pWarns] = 
	PlayerInfo[playerid][pPremium] = 
	PlayerInfo[playerid][pPremiumTime] = 
	PlayerInfo[playerid][pNameChanges] = 
	PlayerInfo[playerid][pNumberChanges] = 
	PlayerInfo[playerid][pCarLic] = 
	PlayerInfo[playerid][pWepLic] = 
	PlayerInfo[playerid][pFlyLic] = 
	PlayerInfo[playerid][pTaxiLic] = 
	PlayerInfo[playerid][pInjured] = 
	PlayerInfo[playerid][pDeathReason] = 
	PlayerInfo[playerid][pFightStyle] = 
	PlayerInfo[playerid][pPayCheckMoney] = 
	PlayerInfo[playerid][pPayDayTime] = 
	PlayerInfo[playerid][pNumber] = 
	PlayerInfo[playerid][pSavings] = 
	PlayerInfo[playerid][pRentKey] = 
	PlayerInfo[playerid][pRentPrice] = 
	PlayerInfo[playerid][pChannel] = 
	PlayerInfo[playerid][pOwnedChannel] = 
	PlayerInfo[playerid][pAuthed] = 
	PlayerInfo[playerid][pSpareKey] =
	PlayerInfo[playerid][pWorkOn] = 
	PlayerInfo[playerid][pWalk] = 
	PlayerInfo[playerid][pFaction] = 
	PlayerInfo[playerid][pJob] = 
	PlayerInfo[playerid][pCareer] = 
	PlayerInfo[playerid][pRank] = 
	PlayerInfo[playerid][pBadgeNumber] = 
	PlayerInfo[playerid][pDriveWarns] = 
	PlayerInfo[playerid][pWepWarns] = 
	PlayerInfo[playerid][pWepCount] = 
	PlayerInfo[playerid][pArrested] = 
	PlayerInfo[playerid][pTalk] = 
	PlayerInfo[playerid][pSwat] = 
	PlayerInfo[playerid][pAuthwep] = 
	PlayerInfo[playerid][pSpawnType] = 
	PlayerInfo[playerid][pSpawnPoint] = 
	PlayerInfo[playerid][pPrisoned] = 
	PlayerInfo[playerid][pHud] = 0;

	pTemp[playerid][pInvulnerable] = 
	PlayerInfo[playerid][pPosX] =
	PlayerInfo[playerid][pPosY] = 
	PlayerInfo[playerid][pPosZ] = 
	PlayerInfo[playerid][pPosA] = 
	PlayerInfo[playerid][pArmour] = 0.0;

	PlayerInfo[playerid][pSlot] = 1;

	//Tog
	PlayerInfo[playerid][pSettings] &= ~togService;
	PlayerInfo[playerid][pSettings] &= ~togName;
	PlayerInfo[playerid][pSettings] &= ~togOOC;
	PlayerInfo[playerid][pSettings] &= ~togJoin;
	PlayerInfo[playerid][pSettings] &= ~togAdv;
	PlayerInfo[playerid][pSettings] &= ~togGov;
	PlayerInfo[playerid][pSettings] &= ~togPM;
    PlayerInfo[playerid][pSettings] &= ~togAdmCmds;
    PlayerInfo[playerid][pSettings] &= ~togNews;
    PlayerInfo[playerid][pSettings] &= ~togAFK;
    PlayerInfo[playerid][pSettings] &= ~togHUD;
    PlayerInfo[playerid][pSettings] &= ~togFaction;
    PlayerInfo[playerid][pSettings] &= ~togTester;
    PlayerInfo[playerid][pSettings] &= ~togEnter;

	for(new i = 0; i < MAX_PLAYER_MARKS; i++)
	{
		PlayerInfo[playerid][pMark][i] = -1;
	}

	for(new i = 0; i < MAX_CCTV; i++)
	{
		PCamera[playerid][i][pc_used] =
		PCamera[playerid][i][pc_time] = 0;
	}
    
	ResetPlayerWeaponsEx(playerid);
	Fuel_ResetPlayer(playerid);
	return 1;
}

RemovePlayerMask(playerid)
{
	if (IsValidDynamic3DTextLabel(cNametag[playerid])) DestroyDynamic3DTextLabel(cNametag[playerid]);
	maskOn{playerid} = false;
	ShowPlayerName(playerid, true);
	format(PlayerInfo[playerid][pMaskname], 32, PlayerInfo[playerid][pUsername_]);
	
	return 1;
}

IsPlayerLogged(playerid)
{
	return (playerid != INVALID_PLAYER_ID && pTemp[playerid][pLogged] == 2 && IsPlayerConnected(playerid)) ? (1) : (0);
}

GetAge(playerid)
{
	new aget[5];
	strmid(aget, PlayerInfo[playerid][pBirthdate], 6, 10);
	return 2020 - strval(aget);
}

ShowPlayerName(playerid, bool:show)
{
	if (show)
	{
		foreach(new i : Player)
		{
			ShowPlayerNameTagForPlayer(i, playerid, 1);
		}
	}
	else
	{
		foreach(new i : Player)
		{
			ShowPlayerNameTagForPlayer(i, playerid, 0);
		}
	}
	return 1;
}

SetPlayerBuyInterior(playerid, step, houseid = 0)
{
    if (!step)
    {
        if (pTemp[playerid][pInteriorBiz] == 0) return 1;
        
        TogglePlayerSpectating(playerid, 0);
        DestroyPlayerObject(playerid, pTemp[playerid][pInteriorObj]);
        HideMenuForPlayer(buy_interior, playerid);
        return 1;
    }

    new bizid = -1;

    if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_BUILD) return SendErrorMessage(playerid, "Você deve estar em uma empresa de construção.");
        
    new start = -1;

    for(new i = 0; i < MAX_INTERIORS; i++)
    {
        if (!AdminTeles[i][telesOn] || HouseInfo[houseid][hClass] != AdminTeles[i][telesType]) continue;

        start = i;
        break;
    }
    
    if (start == -1) return SendErrorMessage(playerid, "Nenhum interior encontrado para a sua casa.");
        
    SavePlayerPos(playerid);
    GameTextForPlayer(playerid, "_", 1, 4);
    TogglePlayerSpectating(playerid, 1);
    pTemp[playerid][pInteriorBiz] = bizid+1;
    pTemp[playerid][pInteriorHouse] = houseid+1;
    pTemp[playerid][pInteriorID] = start;
    pTemp[playerid][pInteriorObj] = CreatePlayerObject(playerid, 19300, AdminTeles[start][telesPosX], AdminTeles[start][telesPosY], AdminTeles[start][telesPosZ], 0.0, 0.0, 0.0);
    AttachCameraToPlayerObject(playerid, pTemp[playerid][pInteriorObj]);
    SetInterior(playerid, AdminTeles[start][telesInt]);
    SetWorld(playerid, 5000 + playerid);
    ShowMenuForPlayer(buy_interior, playerid);
    return 1;
}


IsPlayerAtCarfix(playerid)
{
    if (PlayerInfo[playerid][pFaction] == 0) return 0;
    
	new i = PlayerInfo[playerid][pFaction];
	
	if (GetWorld(playerid) != Factions[i][Carfix][fixVW] || GetInterior(playerid) != Factions[i][Carfix][fixInt]) return 0;

	if (PlayerToPoint(15.0, playerid, Factions[i][Carfix][fixX], Factions[i][Carfix][fixY], Factions[i][Carfix][fixZ]) != 0) return 1;
	
	return 0;
}

PlaySound(playerid, soundid)
{
	PlayerPlaySound(playerid, soundid, 0.0, 0.0, 0.0);
}

PlayerPlaySoundEx(playerid, sound)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	foreach (new i : Player)
	{
	    if (!IsPlayerLogged(i) || IsPlayerInRangeOfPoint(i, 20.0, x, y, z) || GetWorld(i) != GetWorld(playerid) || GetInterior(playerid) != GetInterior(i)) continue;

	    PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
	}
	return 1;
}

q_GetPlayerIP(playerid)
{
	new IP[24];

	GetPlayerIp(playerid, IP, sizeof(IP));
	return IP;
}

Save_User(playerid)
{	
	if (!IsPlayerLogged(playerid))	return 1;

    if (GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
    {
		SavePlayerPos(playerid);
		AntiCheatGetArmour(playerid, PlayerInfo[playerid][pArmour]);
	}

	new Float:health;
	if (pTemp[playerid][pInvulnerable] != 0.0) 	health = pTemp[playerid][pInvulnerable];
	else										health = GetPlayerHealthEx(playerid);
	
	new query[1024];
	format(query, sizeof(query), "UPDATE `users` SET `armour`=%f,`cash`=%i,`bank`=%i,`skin`=%i,`sex`=%i,`level`=%i,`hours`=%i,`admin_hide`=%i,`int`=%i,`world`=%i,`channel`=%i,`owned_channel`=%i,`authed`=%i,`paydaytime`=%i,`number`=%i, `puncte`=%i WHERE `id`=%i",
		PlayerInfo[playerid][pArmour], PlayerInfo[playerid][pCash], PlayerInfo[playerid][pBank], 
		PlayerInfo[playerid][pSkin], PlayerInfo[playerid][pSex], PlayerInfo[playerid][pLevel],
		PlayerInfo[playerid][pHours], PlayerInfo[playerid][pAdminHide], PlayerInfo[playerid][pInt], 
		PlayerInfo[playerid][pWorld], PlayerInfo[playerid][pChannel], PlayerInfo[playerid][pOwnedChannel], 
		PlayerInfo[playerid][pAuthed], PlayerInfo[playerid][pPayDayTime], PlayerInfo[playerid][pNumber], PlayerInfo[playerid][pPunctePremium], PlayerInfo[playerid][pID]
	);
	mysql_tquery(dbHandle, query);

	format(query, sizeof(query), "UPDATE `users` SET `ajailed`=%i,`ajailtime`=%i,`jailed`=%i,`isolatetime`=%i,`warns`=%i,`vip`=%i,`carlic`=%i,`weplic`=%i,`flylic`=%i,`savings`=%i,`rent_key`=%i,`rent_price`=%i,`taxilic`=%i,`dependence`=%i,`demolition`=%i WHERE `id`=%i",
		PlayerInfo[playerid][pAJailed], PlayerInfo[playerid][pAJailTime], PlayerInfo[playerid][pJailed], 
		PlayerInfo[playerid][pIsolateTime], PlayerInfo[playerid][pWarns], PlayerInfo[playerid][pPremium], 
		PlayerInfo[playerid][pCarLic], PlayerInfo[playerid][pWepLic], PlayerInfo[playerid][pFlyLic],
		PlayerInfo[playerid][pSavings], PlayerInfo[playerid][pRentKey], PlayerInfo[playerid][pRentPrice],
		PlayerInfo[playerid][pTaxiLic], PlayerInfo[playerid][pDependence], PlayerInfo[playerid][pDemolition],PlayerInfo[playerid][pID]
	);
	mysql_tquery(dbHandle, query);

	format(query, sizeof(query), "UPDATE `users` SET `arrested`=%i,`chat`=%i,`authwep`=%i,`swat`=%i,`swat_duty`=%i,`job`=%i,`career`=%i,`fightstyle`=%i,`hud`=%i,`mutetime`=%i,`phone_off`=%i,`phone_airplane`=%i,`paycheck`=%i,`fishcount`=%i,`fish_time`=%i WHERE `id`=%i",
		PlayerInfo[playerid][pArrested], PlayerInfo[playerid][pTalk], PlayerInfo[playerid][pAuthwep], 
		PlayerInfo[playerid][pSwat], PlayerInfo[playerid][pSwatDuty], PlayerInfo[playerid][pJob], 
		PlayerInfo[playerid][pCareer], PlayerInfo[playerid][pFightStyle], PlayerInfo[playerid][pHud], 
		PlayerInfo[playerid][pMutedTime], PlayerInfo[playerid][pPhoneOff], PlayerInfo[playerid][pPhoneAirplane],
		PlayerInfo[playerid][pPayCheckMoney], PlayerInfo[playerid][pFishCount], PlayerInfo[playerid][pFishTime],PlayerInfo[playerid][pID]
	);
	mysql_tquery(dbHandle, query);

	format(query, sizeof(query), "UPDATE `users` SET `walk`=%i,`faction`=%i,`rank`=%i,`phone_sound`=%i,`phone_model`=%i,`phone_ringtone`=%i,`badgenumber`=%i,`drivewarns`=%i,`wepwarns`=%i,`wep_count`=%i,`jailtime`=%i,`slot`=%i,`injured`=%i,`deathreason`=%i WHERE `id`=%i",
		PlayerInfo[playerid][pWalk], PlayerInfo[playerid][pFaction], PlayerInfo[playerid][pRank],
		PlayerInfo[playerid][pPhoneSound], PlayerInfo[playerid][pPhoneModel], PlayerInfo[playerid][pPhoneRingtone],
		PlayerInfo[playerid][pBadgeNumber], PlayerInfo[playerid][pDriveWarns], PlayerInfo[playerid][pWepWarns], 
		PlayerInfo[playerid][pWepCount],PlayerInfo[playerid][pJailTime],PlayerInfo[playerid][pSlot], 
		PlayerInfo[playerid][pInjured], PlayerInfo[playerid][pDeathReason],PlayerInfo[playerid][pID]
	);
	mysql_tquery(dbHandle, query);

	format(query, sizeof(query), "UPDATE `users` SET `sidejob`=%i,`prescribe`=%i,`ganja_time`=%i,`phone_style`=%i,`sparekey`=%i,`workon`=%i,`spawntype`=%i,`spawnpoint`=%i,`health`=%f,`pos_X`=%f,`pos_Y`=%f,`pos_Z`=%f,`pos_A`=%f,`prisoned`=%i WHERE `id`=%i",
    	PlayerInfo[playerid][pSideJob], PlayerInfo[playerid][pPrescribe], PlayerInfo[playerid][pGanjaTime], 
		PlayerInfo[playerid][pPhoneStyle], PlayerInfo[playerid][pSpareKey], PlayerInfo[playerid][pWorkOn],
		PlayerInfo[playerid][pSpawnType], PlayerInfo[playerid][pSpawnPoint], health,
		PlayerInfo[playerid][pPosX], PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ], 
		PlayerInfo[playerid][pPosA], PlayerInfo[playerid][pPrisoned],PlayerInfo[playerid][pID]
	);
	mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "UPDATE `users` SET `weaphide0`=%i,`weaphide1`=%i,`weaphide2`=%i,`duty`=%i,`char`=%i,`drugaccess`=%i,`tog_0`=%i,`tog_1`=%i,`tog_2`=%i WHERE `id`=%i",
		PlayerInfo[playerid][pHideWeap][0], PlayerInfo[playerid][pHideWeap][1], PlayerInfo[playerid][pHideWeap][2],
		PlayerInfo[playerid][pOnDuty], PlayerInfo[playerid][pChar], PlayerInfo[playerid][pDrugLabAccess],
		PlayerInfo[playerid][pTog][0], PlayerInfo[playerid][pTog][1], PlayerInfo[playerid][pTog][2], PlayerInfo[playerid][pID]
	);
	mysql_tquery(dbHandle, query);

   	format(query, sizeof(query), "UPDATE `users` SET `PlayerWeapon`=%i,`PlayerAmmo`=%i,`AmmoType`=%i,`WepSerial`=%i,`PlayerSerial`=%i,`LastPrim`=%i,`LastSec`=%i,`LastMelee`=%i,`Forbid`=%i WHERE `id`=%i",
		PlayerInfo[playerid][pPlayerWeapon], PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], 
		PlayerInfo[playerid][pWepSerial], PlayerInfo[playerid][pPlayerSerial], PlayerInfo[playerid][pLastPrim], 
		PlayerInfo[playerid][pLastSec], PlayerInfo[playerid][pLastMelee], PlayerInfo[playerid][pForbid], PlayerInfo[playerid][pID]
	);
	mysql_tquery(dbHandle, query);

	//printf(playerid, "%s", query);
	
	for(new i = 0; i < MAX_COP_SLOTS; i++)
	{
		//if (!PlayerInfo[playerid][pCopItem][i])	continue;

		format(query, sizeof(query), "UPDATE `users` SET `CopItem_%i`=%i,`CopAmmo_%i`=%i,`CopEx_%i`=%i,`CopS_%i`=%i WHERE `id`=%i",
			i+1, PlayerInfo[playerid][pCopItem][i], i+1, PlayerInfo[playerid][pCopQ][i],
			i+1, PlayerInfo[playerid][pCopEx][i], i+1, PlayerInfo[playerid][pCopS][i], PlayerInfo[playerid][pID]
		);
		mysql_tquery(dbHandle, query);
	}	

	SaveUser_Inventory(playerid);
	return 1;
}

SaveUser_Inventory(playerid) 
{
	new query[256];

	for(new i = 0; i < MAX_INV_SLOTS; i++)
	{
		if (strlen(Inventory[playerid][i][pInvName]) < 2) format(Inventory[playerid][i][pInvName], 16, "None");

	    format(query, sizeof(query), "UPDATE `inventory` SET `id_%i`=%i,`a_%i`=%i,`f_%i`=%.1f,`ex_%i`=%i,`s_%i`=%i,`n_%i`='%s' WHERE `userid`=%i",
			i+1, Inventory[playerid][i][pInvItem], i+1, Inventory[playerid][i][pInvQ], 
			i+1, Inventory[playerid][i][pInvQFloat], i+1, Inventory[playerid][i][pInvEx], 
			i+1, Inventory[playerid][i][pInvS], i+1, Inventory[playerid][i][pInvName], PlayerInfo[playerid][pID]
		);
		mysql_tquery(dbHandle, query);
	}
	
	return 1;
}

stock Fuel_ResetPlayer(playerid) 
{
    if (RefuelTimer[playerid] != -1)
    {
        KillTimer(RefuelTimer[playerid]);
        RefuelTimer[playerid] = -1;
//        PlayerTextDrawHide(playerid, FuelText[playerid]);
    }
 
    DeletePVar(playerid, #UsePump);
    DeletePVar(playerid, #FuelBought);
    return 1;
}

ResetVariables(playerid)
{
    pTemp[playerid][pPizzaBoy] = 
    pTemp[playerid][pDemolitionTime] =
	pTemp[playerid][pDrugUse] =
	pTemp[playerid][pDrugTime] =
	pTemp[playerid][pDrugChgWeather] =
    pTemp[playerid][pLicenseExam] =
	pTemp[playerid][pWaypoint] = 
    pTemp[playerid][pWaypoint] = 
	pTemp[playerid][pWepPrice] =
	pTemp[playerid][pRentPrice] = 
	pTemp[playerid][pHousePrice] =
    pTemp[playerid][pRentveh] = 
	pTemp[playerid][pServeType] = 
	pTemp[playerid][pGreetType] =
    pTemp[playerid][pNews] = 
	pTemp[playerid][pCallOn] =
	pTemp[playerid][pFishTime] =
    pTemp[playerid][pEditBort] = 
	pTemp[playerid][pCarPrice] = 
	pTemp[playerid][tempItem] = 
	pTemp[playerid][tempEx] = 
	pTemp[playerid][tempQ] = 	
    pTemp[playerid][pHealTime] = 
    pTemp[playerid][pRamType] =
	pTemp[playerid][UsingBort] = 
	pTemp[playerid][UsingCorpse] =
	pTemp[playerid][tempStatus] =
    pTemp[playerid][pRamTime] = 0;

	pTemp[playerid][pHouseHouse] = -1;

	pTemp[playerid][pBizPrice] = 0;
	pTemp[playerid][pBizID] = INVALID_PLAYER_ID;

    pTemp[playerid][pInterview] = 
	pTemp[playerid][pInterviewPlayer] =
	pTemp[playerid][pHealPlayer] = 
    pTemp[playerid][pFactionPlayer] = 
	pTemp[playerid][pBizPlayer] = 
    pTemp[playerid][pFriskPlayer] = 
	pTemp[playerid][pSpecPlayer] = 
	pTemp[playerid][pHirePlayer] = 
	pTemp[playerid][pCarPlayer] = 
	pTemp[playerid][pWepPlayer] = 
	pTemp[playerid][pRentPlayer] = 
	pTemp[playerid][pHousePlayer] =
	pTemp[playerid][pServePlayer] = 
	pTemp[playerid][pGreetPlayer] = INVALID_PLAYER_ID;

	for(new c = 0; c < 5; c++)
	{
		pTemp[playerid][MyCarSpawn][c] = 0;
		pTemp[playerid][MyCarSpawn][c] = 0;
	}

	SetPVarInt(playerid, #attach_item, 0);

	if (IsPlayerPhoneCreated(playerid)) 	Phone_Hide(playerid);
	if (IsPlayerUseMDC(playerid))		MDC_Hide(playerid);

    if (IsValidDynamicObject(pTemp[playerid][pRamObj]))
	{
		DestroyDynamicObject(pTemp[playerid][pRamObj]);
		pTemp[playerid][pRamObj] = INVALID_OBJECT_ID;
	}

	PlayerInfo[playerid][pArmour] = 0.0;

	if (pTemp[playerid][pFlashTime]) pTemp[playerid][pFlashTime] = 1;

	if (pTemp[playerid][p3DMenu] != -1)
	{
    	Destroy3DMenu(pTemp[playerid][p3DMenu]);
		pTemp[playerid][p3DMenu] = -1;
	}

	if (IsPlayerPhoneCreated(playerid)) Phone_Hide(playerid);

	if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID)
	{
	    SendPhoneMessage(pTemp[playerid][pCallLine], "A conexão com o assinante foi interrompida...");
	    cmd::hangup(playerid, "");
		
	}
	pTemp[playerid][pEmergency] = 0;

	if (GetPVarInt(playerid, #selfie)) DeletePVar(playerid, #selfie);

    if (pTemp[playerid][pTracePlayer] != INVALID_PLAYER_ID)
	{
	    pTemp[playerid][pTracePlayer] = INVALID_PLAYER_ID;
		pTemp[playerid][pTraceStage] = 0;

		if (pTemp[playerid][pTraceGZ] != -1) GangZoneDestroy(pTemp[playerid][pTraceGZ]);

		pTemp[playerid][pTraceGZ] = -1;
	}

	if (pTemp[playerid][pCargoID]) pTemp[playerid][pCargoID] = 0;
	
	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	TogglePlayerAllDynamicRaceCPs(playerid, false);
	TogglePlayerAllDynamicCPs(playerid, false);
	EndJob(playerid);
	Fuel_ResetPlayer(playerid);
	return 1;
}

KickEx(playerid)
{
	if (GetPVarInt(playerid,"kicked")) return 1;

    SetPVarInt(playerid,"kicked", 1);
    SetTimerEx("KickPlayer", 200, false, "d", playerid);
    return 1;
}

ShowStats(playerid, id)
{
    new nums[32], works[64], donate[50], fname[50], rname[50];
    new year, month, day, hour, minute, second;

    if (PlayerInfo[id][pNumber] == 0)  nums = "N/D";
    else                                format(nums, sizeof(nums), "%i", PlayerInfo[id][pNumber]);

    if (PlayerInfo[id][pWorkOn] == 0)  works = "N/D";
    else                                format(works, sizeof(works), "%i", PlayerInfo[id][pWorkOn]);

    if (PlayerInfo[id][pFaction] == 0) fname = "N/D";
    else                                format(fname, 50, "%s", Factions[PlayerInfo[id][pFaction]][fName]);

    if (PlayerInfo[id][pRank] == 0)    rname = "N/D";
    else                                format(rname, 50, "%s", GetRankName(id));

    switch(PlayerInfo[id][pPremium])
    {
        case 1: donate = "BRONZE";
        case 2: donate = "PRATA";
        case 3: donate = "OURO";
        default: donate = "Não";
    }

    getdate(year, month, day);
    gettime(hour, minute, second);

    SendClientMessageEx(playerid, -1,"{6D00FF} %s [%02d %s %i, %02d:%02d:%02d]", GetNameEx(id), day, GetMonthName(month), year, hour, minute, second);
    SendClientMessageEx(playerid, -1, "{6D00FF} Características {FFFFFF} UCP:{6D00FF}[%s] {FFFFFF}Facção:{6D00FF}[%s]{FFFFFF} Posto:{6D00FF}[%s]{FFFFFF} Telefone:{6D00FF}[%s]", GetAccName(id), fname, rname, nums);
    SendClientMessageEx(playerid, -1, "{6D00FF} Nível {FFFFFF} Nível:{6D00FF}[%i] {FFFFFF}| Premium:{6D00FF}[%s]", PlayerInfo[id][pLevel], donate);
    SendClientMessageEx(playerid, -1, "{6D00FF} Drogas {FFFFFF} Dependência:{6D00FF}[%i] {FFFFFF}| Acesso para criar laboratório:{6D00FF}[%s]", PlayerInfo[id][pDependence], (PlayerInfo[id][pDrugLabAccess] != 0)?("Sim"):("Não"));
    SendClientMessageEx(playerid, -1,"{6D00FF} Habilidades {FFFFFF} Horas de Jogo:{6D00FF}[%i] {FFFFFF} Estilo de combate:{6D00FF}[%s]", PlayerInfo[id][pHours], GetFightStyleName(id));
    SendClientMessageEx(playerid, -1,"{6D00FF} Dinheiro {FFFFFF} Dinheiro vivo:{6D00FF}[$%i] {FFFFFF} No banco:{6D00FF}[$%i] {FFFFFF} Cheque:{6D00FF}[$%i] {FFFFFF} conta poupança:{6D00FF}[$%i]", GetCash(id), PlayerInfo[id][pBank], PlayerInfo[id][pPayCheckMoney], PlayerInfo[id][pSavings]);
    SendClientMessageEx(playerid, -1,"{6D00FF} Trabalho {FFFFFF} Emprego no negócio:{6D00FF}[%s] {FFFFFF}Trabalho principal:{6D00FF}[%s] {FFFFFF} Segundo emprego:{6D00FF}[%s]", works, GetJobName(PlayerInfo[id][pJob]), GetJobName(PlayerInfo[id][pSideJob]));
    SendClientMessageEx(playerid, -1,"{6D00FF} OOC {FFFFFF} Advertências:{6D00FF}[%i] {FFFFFF}IP:{6D00FF}[%s] {FFFFFF}Administrador:{6D00FF}[%s] {FFFFFF} Tester:{6D00FF}[%s]", PlayerInfo[id][pWarns], q_GetPlayerIP(id), (PlayerInfo[id][pAdmin] != 0)?("Sim"):("Não"), (PlayerInfo[id][pHelper] != 0)?("Sim"):("Não"));
    if (PlayerInfo[id][pPlayerWeapon]) SendClientMessageEx(playerid, -1, "{6D00FF} Arma {FFFFFF} %s (%i) / {6D00FF}em mãos", inventory_items[PlayerInfo[id][pPlayerWeapon]][itemName], PlayerInfo[id][pPlayerAmmo]);

    if (CheckAdmin(playerid, ADMIN_LEVEL_1))
    {
        if (PlayerInfo[id][pJailed] != 0)     SendClientMessageEx(playerid, COLOR_LIGHTRED, "| Punição IC | Está na cadeia por mais [%i minutos].", PlayerInfo[id][pJailTime]);
        if (PlayerInfo[id][pMutedTime] > 0)   SendClientMessageEx(playerid, COLOR_LIGHTRED, "| Punição OOC | Silenciado por %i minutos.", PlayerInfo[id][pMutedTime]);
        if (PlayerInfo[id][pForbid] != 0)     SendClientMessageEx(playerid, COLOR_LIGHTRED, "| Punição OOC | Proibido de usar arma por %ih.", PlayerInfo[id][pForbid]);
        if (id != playerid)                   SendHelpMessage(playerid,"Para visualizar inventário {ffc46a}/check %i", id);
    }

    SendClientMessageEx(playerid, -1,"{6D00FF} %s [%02d %s %i, %02d:%02d:%02d]", GetNameEx(id), day, GetMonthName(month), year, hour, minute, second);
    return 1;
}


SetSpawnInfoEx(playerid, Float:x, Float:y, Float:z, Float:Angle = 0.0)
{
    if (PlayerInfo[playerid][pChar] > 0)	SetSpawnInfo(playerid, 0, PlayerInfo[playerid][pChar], x, y, z, Angle, 0, 0, 0, 0, 0, 0);
	else									SetSpawnInfo(playerid, 0, PlayerInfo[playerid][pSkin], x, y, z, Angle, 0, 0, 0, 0, 0, 0);
	return 1;
}

GetNameForPlayer(playerid)
{
    GetPlayerName(playerid, PlayerInfo[playerid][pUsername], MAX_PLAYER_NAME+1);
    GetPlayerName(playerid, PlayerInfo[playerid][pUsername_], MAX_PLAYER_NAME+1);
    strreplace(PlayerInfo[playerid][pUsername_], '_', ' ');
    format(PlayerInfo[playerid][pMaskname], 32, PlayerInfo[playerid][pUsername_]);
    return 1;
}

GetIDByID(databaseid)
{
	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i)) 												continue;
		if (PlayerInfo[i][pID] == databaseid)								return i;
	}
	return INVALID_PLAYER_ID;
}

GetNameFromBD(id)
{
	new query[64], name[MAX_PLAYER_NAME];

	mysql_format(dbHandle, query, sizeof(query), "SELECT `name` FROM `users` WHERE `id` = %i", id);
	new Cache:cache = mysql_query(dbHandle, query);
	
	cache_get_field_content(0, "name", name, dbHandle, 24);
	strreplace(name, '_', ' ');

	cache_delete(cache);

	return name;
}

CheckPMBlockList(playerid)
{
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || people_blocking[i][playerid] != 1)			continue;

        SendSyntaxMessage(i, "%s desconectou-se do servidor e foi removido da sua lista negra de MP.", GetNameEx(playerid), playerid);
		people_blocking[i][playerid] = 0;
	}
	return 1;
}

stock GetXYInFrontOfPlayer(playerid, &Float:X, &Float:Y, &Float:Z, Float:distance)
{
	new Float:A;
	GetPlayerPos(playerid, X, Y, Z);

	if (GetPlayerVehicleID(playerid))	GetVehicleZAngle(GetPlayerVehicleID(playerid), A);
	else								GetPlayerFacingAngle(playerid, A);

	X += (distance * floatsin(-A, degrees));
	Y += (distance * floatcos(-A, degrees));
}

#define GetVectorSpeed(%0,%1,%2) floatround(VectorSize(%0, %1, %2) * 179.28625)
SetPlayerPosEx(playerid, Float:x, Float:y, Float:z, freeze = 1)
{
	SetPlayerPos(playerid, x, y, z + 0.2);
 	SetCameraBehindPlayer(playerid);
	if (freeze != 0)
	{
 		FreezePlayer(playerid);
 		pTemp[playerid][pFreezeTime] = 2;
	}
	return 1;
}

SetPlayerCurrentTimeAndWeather(playerid)
{
	SetPlayerTime(playerid, server_hour, 0);
	SetPlayerWeather(playerid, server_weather);
	return 1;
}

GetFightStyleName(playerid)
{
	new style[9];
	switch(PlayerInfo[playerid][pFightStyle])
	{
		case FIGHT_STYLE_BOXING: format(style, sizeof(style), "Boxe");
		case FIGHT_STYLE_KUNGFU: format(style, sizeof(style), "Kung Fu");
		case FIGHT_STYLE_GRABKICK: format(style, sizeof(style), "Chute agarrado");
		case FIGHT_STYLE_KNEEHEAD: format(style, sizeof(style), "Joelhada");
		case FIGHT_STYLE_NORMAL: format(style, sizeof(style), "Padrão");
	}
	return style;
}

Dialog:MODEL_SEL_BUYCLOTHES(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    new modelid;
    if (PlayerInfo[playerid][pSex] == 1) modelid = male_skins[listitem];
    else                                modelid = female_skins[listitem];

    if (modelid < 1 || modelid > 311) return 1;

    if (GetCash(playerid) < 100) return SendNotCash(playerid);
    if (GetPlayerSkin(playerid) == modelid || PlayerInfo[playerid][pTog][0] == modelid || PlayerInfo[playerid][pTog][1] == modelid || PlayerInfo[playerid][pTog][2] == modelid) 
    {
        return SendErrorMessage(playerid, "Você já adquiriu esta skin.");
    }

    new mes[128];

    for(new i; i != 3; i++)
    {
        if (PlayerInfo[playerid][pTog][i] == 0) strcat(mes, "\n[Vazio]");
        else                                    format(mes, sizeof(mes), "%s\n[Skin #%i]", mes, PlayerInfo[playerid][pTog][i]);
    }

    Dialog_Show(playerid, BuyClothes, DIALOG_STYLE_LIST, "Selecione um slot...", mes, "Mudar", "X");
    SetPVarInt(playerid, "clothes_model", modelid);
    return 1;
}

CreateDynamicExplosion(Float:x,Float:y,Float:z,type,Float:radius,worldid = -1,interiorid = -1,playerid = -1,Float:distance = 200.0){
	if (playerid == -1){
		foreach(new i : Player){
			new Float:px, Float:py, Float:pz;
			if (IsPlayerInAnyVehicle(i)){
				GetVehiclePos(GetPlayerVehicleID(i),px,py,pz);
			} else {
				GetPlayerPos(i,px,py,pz);
			}
			if ((GetPlayerInterior(i) == interiorid || interiorid == -1) && (GetPlayerVirtualWorld(i) == worldid || worldid == -1) && (VectorSize(px-x,py-y,pz-z) <= distance)){
				CreateExplosionForPlayer(i,x,y,z,type,radius);
			}
		}
	} else {
		if (IsPlayerConnected(playerid)){
			new Float:px, Float:py, Float:pz;
			if (IsPlayerInAnyVehicle(playerid)){
				GetVehiclePos(GetPlayerVehicleID(playerid),px,py,pz);
			} else {
				GetPlayerPos(playerid,px,py,pz);
			}
			if ((GetPlayerInterior(playerid) == interiorid || interiorid == -1) && (GetPlayerVirtualWorld(playerid) == worldid || worldid == -1) && (VectorSize(px-x,py-y,pz-z) <= distance)){
				CreateExplosionForPlayer(playerid,x,y,z,type,radius);
			}
		}
	}
} 

GetLocationZone(Float:x, Float:y, Float:z)
{
	new string[128], zone[28];

	GetZone(x, y, zone, sizeof(zone));
	
	if (IsPointInAnyDynamicArea(x, y, z)) {
		new areas[10], street_area[2], j = GetNumberDynamicAreasForPoint(x,y,z);
		GetDynamicAreasForPoint(x, y, z, areas, sizeof(areas));

		if (GetStreet(areas, j, street_area)) {
			if (street_area[1] == -1) format(string, sizeof(string), "%s, %s", zone, street_names[street_area[0]]); 
			else					 format(string, sizeof(string), "%s, no cruzamento entre %s e %s", zone, street_names[street_area[0]], street_names[street_area[1]]);

		}
	}
	format(string, sizeof(string), "%s", zone);

	return string;
}

stock bool:IsPlayerCrouching(playerid)
{
    if (GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_DUCK) return false;
	return true;
}

CheckHanditem(playerid, model) {
	new result = 0;
	for(new i = 0; i < MAX_PLAYER_ATTACH; i++)
	{
		if (!AttachInfo[playerid][i][aUsed]) 										   continue;
		if (AttachInfo[playerid][i][aModel] != model)								   continue;
		if (AttachInfo[playerid][i][aBone] != 5 && AttachInfo[playerid][i][aBone] != 6) continue;
		return 1;
	}
	
	return result;
}

stock PlaySoundInArea(soundid,Float:x,Float:y,Float:z,Float:radi)
{
    foreach(new i : Player)
	{
        if (!IsPlayerLogged(i)) continue;
	    if (IsPlayerInRangeOfPoint(i,radi,x,y,z)) PlayerPlaySound(i, soundid, x, y, z);
	}
	return 1;
}

Float:XB_GetDistanceBetweenTPoints(Float:x,Float:y,Float:z,Float:tx,Float:ty,Float:tz)
{
	new Float:temp1, Float:temp2 , Float:temp3;
	temp1 = x-tx;temp2 = y-ty;
	temp3 = z-tz;
	return floatsqroot(temp1*temp1+temp2*temp2+temp3*temp3);
}

CheckNumberAvailable(number)
{
	new
		query[64]
	;

 	mysql_format(dbHandle, query, sizeof(query), "SELECT COUNT(id) as total FROM `users` WHERE `number`=%i", number);
	new Cache:cache = mysql_query(dbHandle, query),
		count = cache_get_field_content_int(0, "total");
	
	cache_delete(cache);
	return count;
}

SetPlayerNumber(playerid) {	
	mysql_tquery(dbHandle, "SELECT FLOOR(RAND()*(999999-100100)+100100) as randNumber \
							FROM `users` \
							HAVING (SELECT COUNT(*) FROM `users` WHERE number = randNumber) < 1 \
							LIMIT 1", 
				"GetPlayerNumber", "i", playerid
	);
	return 1;
}

this::GetPlayerNumber(playerid) {
	if (!cache_num_rows()) return SetPlayerNumber(playerid);

	PlayerInfo[playerid][pNumber] = cache_get_field_content_int(0, "randNumber", dbHandle);
	SQL_SetInteger("users", "number", PlayerInfo[playerid][pNumber], PlayerInfo[playerid][pID]);

	SendSyntaxMessage(playerid, "Você recebeu um novo número de celular: %i. ((/celular))", PlayerInfo[playerid][pNumber]);
	return 1;
}

CreatePlayersFire(playerid, type, building)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (GetDistanceBetweenPoints(x,y,z,PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]) > 1.5 
		&& PlayerInfo[playerid][pWorld] != GetWorld(playerid) 
		&& PlayerInfo[playerid][pInt] != GetInterior(playerid)
	) {
		StopCreatingPlayersFire(playerid);
		return SendErrorMessage(playerid, "Falha ao acender o fogo, tente novamente!");
	}
	
	new 
		location[128], 
		where[12], 
		Float:ExpPos[3]
	;

	switch(type) {
		case 0: {
			CreateFireInHouse(building);

			if (HouseInfo[building][hApart]) {
				ExpPos[0] = ApartInfo[HouseInfo[building][hApart]][aPosX];
				ExpPos[1] = ApartInfo[HouseInfo[building][hApart]][aPosY];
				ExpPos[2] = ApartInfo[HouseInfo[building][hApart]][aPosZ];

				where = "apartamento";
			} else {
				ExpPos[0] = HouseInfo[building][hPosX]; 
				ExpPos[1] = HouseInfo[building][hPosY];
				ExpPos[2] = HouseInfo[building][hPosZ];

				where = "casa";
			}
		}
		case 1: {
			CreateFireInBiz(building);

			ExpPos[0] = BizInfo[building][bizPosX]; 
			ExpPos[1] = BizInfo[building][bizPosY];
			ExpPos[2] = BizInfo[building][bizPosZ];

			where = "empresa";
		} 
		case 2: {
			CreateFireInGarage(building);

			ExpPos[0] = GarageInfo[building][garageVeh_Enter][0]; 
			ExpPos[1] = GarageInfo[building][garageVeh_Enter][1];
			ExpPos[2] = GarageInfo[building][garageVeh_Enter][2];

			where = "garagem";
		}
	}

	format(location, sizeof(location), "%s", GetLocationZone(x,y,z));

	for(new i = FACTION_POLICE; i <= FACTION_FIRE; i++)
	{
	    SendFactionMessage(i, COLOR_LIGHTBLUE, "|__________Chamado de Emergência__________|");
	    SendFactionMessage(i, COLOR_LIGHTBLUE, "Chamado de: Telefone Público");
	    SendFactionMessage(i, COLOR_LIGHTBLUE, "Solicitação: Polícia, Ambulância / Bombeiros");
	    SendFactionMessage(i, COLOR_LIGHTBLUE, "Localização: %s %i", location, building);
	    SendFactionMessage(i, COLOR_LIGHTBLUE, "Situação: Houve um incêndio no(a) %s!", where);         
	}

	for(new i = 0; i != MAX_CALLS_AND_ARREST; i++)
	{
	    if (CallInfo[i][callExists] != 0) continue;

	    CallInfo[i][callPosX] = ExpPos[0];
	    CallInfo[i][callPosY] = ExpPos[1];
	    CallInfo[i][callPosZ] = ExpPos[2];

	    CallInfo[i][callType] = 3;
	    CallInfo[i][callPhone] = 0;

	    format(CallInfo[i][callText], 128, "Houve um incêndio no(a) %s!", where);
	    format(CallInfo[i][callName], MAX_PLAYER_NAME, "Telefone Público");
	    format(CallInfo[i][callDate], 32, "%s", GetFullDate());
	    format(CallInfo[i][callLocation], MAX_ZONE_NAME, "%s %i", location, building);
	    
	    CallInfo[i][callDelete] = gettime() + 1200;
	    CallInfo[i][callExists] = 1;
	    break;
	}


	return 1;
}

StopCreatingPlayersFire(playerid)
{
	UnFreezePlayer(playerid);
	StopPlayerAnims(playerid);
	pTemp[playerid][pFireTime] = 0;
	DeletePVar(playerid, #firetype);
	DeletePVar(playerid, #firebuildID);
	return 1;
}

GetHealthDots(playerid)
{
    new
        dots[64], Float: HP;
 
    AntiCheatGetHealth(playerid, HP);
 
	if (HP >= 100)
        dots = "IIIIIIIIII";
    else if (HP >= 90)
        dots = "IIIIIIIII{660000}I";
    else if (HP >= 80)
        dots = "IIIIIIII{660000}II";
    else if (HP >= 70)
        dots = "IIIIIII{660000}III";
    else if (HP >= 60)
        dots = "IIIIII{660000}IIII";
    else if (HP >= 50)
        dots = "IIIII{660000}IIIII";
    else if (HP >= 40)
        dots = "IIII{660000}IIIIII";
    else if (HP >= 30)
        dots = "III{660000}IIIIIII";
    else if (HP >= 20)
        dots = "II{660000}IIIIIIII";
    else if (HP >= 10)
        dots = "I{660000}IIIIIIIII";
    else if (HP >= 0)
        dots = "{660000}IIIIIIIIII";

    /*if (HP >= 100)
        dots = "����������";
    else if (HP >= 90)
        dots = "���������{660000}�";
    else if (HP >= 80)
        dots = "��������{660000}��";
    else if (HP >= 70)
        dots = "�������{660000}���";
    else if (HP >= 60)
        dots = "������{660000}����";
    else if (HP >= 50)
        dots = "�����{660000}�����";
    else if (HP >= 40)
        dots = "����{660000}������";
    else if (HP >= 30)
        dots = "���{660000}�������";
    else if (HP >= 20)
        dots = "��{660000}��������";
    else if (HP >= 10)
        dots = "�{660000}���������";
    else if (HP >= 0)
        dots = "{660000}����������";*/
 
    return dots;
}
 
GetArmorDots(playerid)
{
    new
        dots[64], Float: AR;
 
    AntiCheatGetArmour(playerid, AR);
	
	if (AR >= 100)
        dots = "IIIIIIIIII";
    else if (AR >= 90)
        dots = "IIIIIIIII{666666}I";
    else if (AR >= 80)
        dots = "IIIIIIII{666666}II";
    else if (AR >= 70)
        dots = "IIIIIII{666666}III";
    else if (AR >= 60)
        dots = "IIIIII{666666}IIII";
    else if (AR >= 50)
        dots = "IIIII{666666}IIIII";
    else if (AR >= 40)
        dots = "IIII{666666}IIIIII";
    else if (AR >= 30)
        dots = "III{666666}IIIIIII";
    else if (AR >= 20)
        dots = "II{666666}IIIIIIII";
    else if (AR >= 10)
        dots = "I{666666}IIIIIIIII";
    else if (AR >= 0)
        dots = "{666666}IIIIIIIIII";

    /*if (AR >= 100)
        dots = "����������";
    else if (AR >= 90)
        dots = "���������{666666}�";
    else if (AR >= 80)
        dots = "��������{666666}��";
    else if (AR >= 70)
        dots = "�������{666666}���";
    else if (AR >= 60)
        dots = "������{666666}����";
    else if (AR >= 50)
        dots = "�����{666666}�����";
    else if (AR >= 40)
        dots = "����{666666}������";
    else if (AR >= 30)
        dots = "���{666666}�������";
    else if (AR >= 20)
        dots = "��{666666}��������";
    else if (AR >= 10)
        dots = "�{666666}���������";
    else if (AR >= 0)
        dots = "{666666}����������";*/
 
    return dots;
}

this::Invulnerable(playerid)
{
	if (pTemp[playerid][pInvulnerable] == 0.0) {
		pTemp[playerid][pInvulnerable] = 30.0;
	}

	SetPlayerHealth(playerid, pTemp[playerid][pInvulnerable]);
	pTemp[playerid][pInvulnerable] = 0.0;
	return 1;
}

stock to_date( timestamp, _form=0 )
{
    /*
        ~ convert a Timestamp to a Date.
        ~ 10.07.2009

        date( 1247182451 )  will print >> 09.07.2009-23:34:11
        date( 1247182451, 1) will print >> 09/07/2009, 23:34
        date( 1247182451, 2) will print >> July 09, 2009, 23:34:11
        date( 1247182451, 3) will print >> 9 Jul 2009, 23:34
    */
    new year=1970, day=0, month=0, hour=0, mins=0, sec=0;

    new days_of_month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    new names_of_month[12][10] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
    new returnstring[32];

    while(timestamp>31622400){
        timestamp -= 31536000;
        if ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) ) timestamp -= 86400;
        year++;
    }

    if ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) )
        days_of_month[1] = 29;
    else
        days_of_month[1] = 28;


    while(timestamp>86400){
        timestamp -= 86400, day++;
        if (day==days_of_month[month]) day=0, month++;
    }

    while(timestamp>60){
        timestamp -= 60, mins++;
        if ( mins == 60) mins=0, hour++;
    }

    sec=timestamp;

    switch( _form ){
        case 1: format(returnstring, 31, "%02d/%02d/%d %02d:%02d", day+1, month+1, year, hour, mins);
        case 2: format(returnstring, 31, "%s %02d, %d, %02d:%02d:%02d", names_of_month[month],day+1,year, hour, mins, sec);
        case 3: format(returnstring, 31, "%d %c%c%c %d, %02d:%02d", day+1,names_of_month[month][0],names_of_month[month][1],names_of_month[month][2], year,hour,mins);

        default: format(returnstring, 31, "%02d.%02d.%d-%02d:%02d:%02d", day+1, month+1, year, hour, mins, sec);
    }

    return returnstring;
}