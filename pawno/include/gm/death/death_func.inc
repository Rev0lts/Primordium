ResetDamages(playerid)
{
	for(new i = 0; i != MAX_DAMAGES; i++)
	{
	    if (DamageInfo[playerid][i][damageOn] == 0) continue;

		DamageInfo[playerid][i][damageBy] = EOS;
        DamageInfo[playerid][i][damageOn] = 0;
        DamageInfo[playerid][i][damageWeapon] = 0;
        DamageInfo[playerid][i][damageAmount] = 0;
        DamageInfo[playerid][i][damageArmour] = 0;
		DamageInfo[playerid][i][damageTime] = 0;
	}

	pTemp[playerid][pDamagesCount] = 0;
	pTemp[playerid][pInjuredTime] = 0;
    pTemp[playerid][pInjuredLegTime] = 0;
	PlayerInfo[playerid][pInjured] = 0;
	PlayerInfo[playerid][pDeathReason] = 0;

	SQL_SetInteger("users", "injured", 0, PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "deathreason", 0, PlayerInfo[playerid][pID]);
	return 1;
}

ResetDamage(playerid, i)
{
	if (!DamageInfo[playerid][i][damageOn])	return 1;

	DamageInfo[playerid][i][damageBy] = EOS;
	DamageInfo[playerid][i][damageOn] = 0;
	DamageInfo[playerid][i][damageWeapon] = 0;
	DamageInfo[playerid][i][damageAmount] = 0;
	DamageInfo[playerid][i][damageArmour] = 0;
	DamageInfo[playerid][i][damageTime] = 0;
	
	return 1;
}

GetBodyPartName(bodypart)
{
    new part[15];
    switch(bodypart)
    {
        case BODY_PART_TORSO:       part = "TORSO";         
        case BODY_PART_GROIN:       part = "PÉLVIS";         
        case BODY_PART_LEFT_ARM:    part = "BRAÇO ESQUERDO";  
        case BODY_PART_RIGHT_ARM:   part = "BRAÇO DIREITO";  
        case BODY_PART_LEFT_LEG:    part = "PERNA ESQUERDA";  
        case BODY_PART_RIGHT_LEG:   part = "PERNA DIREITA";  
        case BODY_PART_HEAD:        part = "CABEÇA";        
        default:                    part = "NENHUM"; 
    }
    return part;
}


DisplayDamages(toplayer, playerid)
{
    new maxpages = floatround(MAX_DAMAGES/MAX_DIALOG_LIST, floatround_ceil);
    if (pTemp[toplayer][pDialogList] < 0 || (maxpages < pTemp[toplayer][pDialogList]+1)) return 1;

    new 
        title[45], 
        start_itter = MAX_DIALOG_LIST * pTemp[playerid][pDialogList],
        end_itter = (maxpages == pTemp[playerid][pDialogList]+1) ? (MAX_DAMAGES) : (start_itter+MAX_DIALOG_LIST+1),
        count, 
        str[(MAX_PLAYER_NAME + 65) * MAX_DIALOG_LIST + 71 + 24] = "Lista de todos os danos:\n",
        weaponname[32]
    ;

    format(title, sizeof(title), "{FFFFFF}%s", GetNameEx(playerid));

    if (pTemp[toplayer][pDialogList]) format(str, sizeof(str), "%s{bccd32}Página anterior{FFFFFF}\n", str);

    for(new i = start_itter; i < end_itter; i++)
    {
        if (DamageInfo[playerid][i][damageOn] != 1) continue;

        if (gettime()-DamageInfo[playerid][i][damageTime] > 3600) ResetDamage(playerid, i);
        else
        {
            if (count == MAX_DIALOG_LIST) {
                strcat(str, "{bccd32}Próxima página");
            } else {
                new weaponid = DamageInfo[playerid][i][damageWeapon];

                if (weaponid == 0) format(weaponname, sizeof(weaponname), "punho");
                else format(weaponname, sizeof(weaponname), "%s", inventory_items[weaponid][itemName]);
                
                new stradmin[3 + MAX_PLAYER_NAME] = EOS;
                if (CheckAdmin(toplayer, 1))  format(stradmin, sizeof(stradmin), " [%s]", DamageInfo[playerid][i][damageBy]);

                if (DamageInfo[playerid][i][damageWeapon] == 51)    format(str, sizeof(str), "%s{FFFFFF}%.0f HP de queimadura %d segundos atrás%s\n", str, DamageInfo[playerid][i][damageAmount], gettime()-DamageInfo[playerid][i][damageTime], stradmin);
                else if (!DamageInfo[playerid][i][damageArmour])    format(str, sizeof(str), "%s{FFFFFF}%.0f HP em %s no(a) %s %d segundos atrás%s\n", str, DamageInfo[playerid][i][damageAmount], weaponname, GetBodyPartName(DamageInfo[playerid][i][damageBodypart]), gettime()-DamageInfo[playerid][i][damageTime], stradmin);
                else                                               format(str, sizeof(str), "%s{FFFFFF}%.0f de armadura em %s no(a) %s %d segundos atrás%s\n", str, DamageInfo[playerid][i][damageAmount], weaponname, GetBodyPartName(DamageInfo[playerid][i][damageBodypart]), gettime()-DamageInfo[playerid][i][damageTime], stradmin);
                
                count++;
            }
        }
    }

    if (!count) return Dialog_Show(toplayer, None, DIALOG_STYLE_LIST, title, "Não há danos para exibir...", ">>>", "");

    SetPVarInt(toplayer, #userid, playerid);
    Dialog_Show(toplayer, DamageDialog, DIALOG_STYLE_TABLIST_HEADERS, title, str, ">>>", "");
    return 1;
}


Dialog:DamageDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (pTemp[playerid][pDialogList]) {			//if not first page
		if (listitem == 0) {
			pTemp[playerid][pDialogList]--;
			return DisplayDamages(playerid, GetPVarInt(playerid, #userid));
		} else if (listitem == MAX_DIALOG_LIST+1) {
			pTemp[playerid][pDialogList]++;
			return DisplayDamages(playerid, GetPVarInt(playerid, #userid));
		}
	} else { 									//if it's a first page
		if (listitem == MAX_DIALOG_LIST) {
			pTemp[playerid][pDialogList]++;
			return DisplayDamages(playerid, GetPVarInt(playerid, #userid));
		}
	}

	return 1;
}

CountDamageRecord(playerid)
{
	new count = 0;
	for(new i = 0; i != MAX_DAMAGES; i++)
	{
		if (DamageInfo[playerid][i][damageOn] == 1)  count++;
	}
	return count;
}

RecordDamage(userid, playerid, weaponid, bodypart, Float: amount, armour)
{
	for(new i = 0; i != MAX_DAMAGES; i++)
	{
		if (DamageInfo[playerid][i][damageOn] == 1 && gettime()-DamageInfo[playerid][i][damageTime] > 3600) ResetDamage(playerid, i);

	    if (DamageInfo[playerid][i][damageOn] == 1)  continue;

        DamageInfo[playerid][i][damageOn] = 1;
		
		if (userid != -1) format(DamageInfo[playerid][i][damageBy], MAX_PLAYER_NAME, "%s",  GetNameEx(userid));
		else 			 format(DamageInfo[playerid][i][damageBy], MAX_PLAYER_NAME, "fogo");

		DamageInfo[playerid][i][damageWeapon] = weaponid;
		DamageInfo[playerid][i][damageBodypart] = bodypart;
		DamageInfo[playerid][i][damageAmount] = amount;
		DamageInfo[playerid][i][damageArmour] = armour;
		DamageInfo[playerid][i][damageTime] = gettime();
		pTemp[playerid][pDamagesCount]++;
		return 1;
    }
	
    return 1;
}

Death_OnPlayerExitVehicle(playerid, vehicleid)
{
	if (PlayerInfo[playerid][pInjured]) {
		UnFreezePlayer(playerid);

		new Float:ccX, Float:ccY, Float:ccZ;
		new Float:dX, Float:dY, Float:dZ;
		
		GetVehicleModelInfo(VehicleInfo[vehicleid][carModel], VEHICLE_MODEL_INFO_FRONTSEAT, ccX, ccY, ccZ);
		GetVehicleRelativePos(vehicleid, dX, dY, dZ, -ccX - 1, ccY, ccZ);
		
		SetPlayerPos(playerid, dX, dY, dZ);
		//SetPlayerPosFindZ(playerid, 1234.5, 1234.5, 1000.0);
		
		ApplyAnimation(playerid, "PED", "KO_SHOT_STOM", 4.0, 1, 0, 0, 0, 0, 1);

		SavePlayerPos(playerid);
	}
	return 1;
}

this::InjuredPlayer(playerid, killerid, weaponid)
{
	if (!IsPlayerLogged(playerid)) return 1;
	if (IsValidVehicle(JackingVehicle[playerid])) StopJackingLock(playerid);

	ResetVariables(playerid);
	RemovePlayerMask(playerid);
	
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || pTemp[i][pDMLog] != 1) continue;
		SendDeathMessageToPlayer(i, killerid, playerid, weaponid);
	}

	SavePlayerPos(playerid);
	PlayerInfo[playerid][pDeathReason] = weaponid;
	SQL_SetInteger("users", "deathreason", weaponid, PlayerInfo[playerid][pID]);

	if (Check_KO(playerid)) {
	    GameTextForPlayer(playerid, "~r~INCONSCIENTE", 5000, 3);

	    ApplyAnimation(playerid,"SWEET", "Sweet_injuredloop", 4.0, 0, 1, 1, 1, 0);

	    pTemp[playerid][pInvulnerable] = 15.0;
	    PlayerInfo[playerid][pInjured] = 3;
	    pTemp[playerid][pInjuredTime] = 60;

	    SendClientMessageEx(playerid, COLOR_LIGHTRED,"[ ! ] >> {FFFFFF}Você está inconsciente. Após 60 segundos, você pode se levantar ou alguém pode te ajudar.");
	    SendClientMessageEx(playerid, COLOR_LIGHTRED,"[ ! ] >> {FFFFFF}Você foi ferido %i vez(es), use /dmg %i para informações mais detalhadas.", pTemp[playerid][pDamagesCount], playerid);
	} else {
	    GameTextForPlayer(playerid, "~b~GRAVEMENTE FERIDO", 5000, 3);

	    new vehicle = GetPlayerVehicleID(playerid);
	    if (IsPlayerInAnyVehicle(playerid) && !IsAPlane(vehicle) && !IsABiker(vehicle) && !IsAHelicopter(vehicle) && !IsABoat(vehicle) && !IsABicycle(vehicle))
	    {
	        if (VehicleInfo[GetPlayerVehicleID(playerid)][carEngineOn]) ToggleVehicleEngine(GetPlayerVehicleID(playerid), playerid);
	        ApplyAnimation(playerid, "PED", "CAR_DEAD_LHS", 4.0, 1, 0, 0, 0, 0, 1);
	        FreezePlayer(playerid);
	    }
	    else ApplyAnimation(playerid, "PED", "KO_shot_stom", 4.1, 0, 1, 1, 1, 0);

		pTemp[playerid][pInvulnerable] = 100.0;
		PlayerInfo[playerid][pInjured] = 1;
		pTemp[playerid][pInjuredTime] = 60;

	    SendClientMessageEx(playerid, COLOR_LIGHTRED,"[ ! ] >> {FFFFFF}Você está gravemente ferido. Você pode esperar por uma ambulância ou digitar \"/aceitarmorte\".");
	    SendClientMessageEx(playerid, COLOR_LIGHTRED,"[ ! ] >> {FFFFFF}Você foi ferido %i vez(es), use /dmg %i para detalhes adicionais.", pTemp[playerid][pDamagesCount], playerid);
	}

	
    pTemp[playerid][pInjuredLegTime] = 0;
	
	SetPlayerHealth(playerid, Float:0x7F800000);
	SetTimerEx("Invulnerable", 4000, false, "i", playerid);
	return 1;
}

Check_KO(playerid)
{
	if (PlayerInfo[playerid][pInjured])	return 0;
	if (IsPlayerInAnyVehicle(playerid))	return 0;

	new Float:m_damage, Float:a_damage;

	for(new i = CountDamageRecord(playerid)-1; i >= 0; i--)
	{
		if (DamageInfo[playerid][i][damageOn] != 1) continue;
		if (gettime()-DamageInfo[playerid][i][damageTime] > 5*60) continue;
		
		switch(DamageInfo[playerid][i][damageWeapon]) {
			case 0 .. 9: 	m_damage += DamageInfo[playerid][i][damageAmount];
			default:		a_damage += DamageInfo[playerid][i][damageAmount];
		}
	}

	if (m_damage > 20.0 && m_damage > a_damage) return 1;
	return 0;
}