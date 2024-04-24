Weapon_EditAttachedObject(playerid, response, index, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
    if (GetPVarInt(playerid, #edit_weapon) != 0)
	{
	    if (response)
	    {
	    	new i = GetPVarInt(playerid, #edit_weapon) - 1;

	    	DeletePVar(playerid, #edit_weapon);
	    	PlayerInfo[playerid][pWeaponBone][i] = boneid;
			PlayerInfo[playerid][pWeaponX][i] = fOffsetX;
			PlayerInfo[playerid][pWeaponY][i] = fOffsetY;
			PlayerInfo[playerid][pWeaponZ][i] = fOffsetZ;
			PlayerInfo[playerid][pWeaponRX][i] = fRotX;
			PlayerInfo[playerid][pWeaponRY][i] = fRotY;
			PlayerInfo[playerid][pWeaponRZ][i] = fRotZ;
			RemovePlayerAttachedObject(playerid, index);
            Save_AttachWeapon(playerid, i);
			SendServerMessage(playerid, "You've edited the weapon slot ID: %i.", i+1);
		}
		else
		{
		    new i = GetPVarInt(playerid, #edit_weapon) - 1;

	    	DeletePVar(playerid, #edit_weapon);
	    	RemovePlayerAttachedObject(playerid, index);
			SendServerMessage(playerid, "You have canceled editing the weapon slot ID: %i.", i+1);
		}
	}
	CallRemoteFunction("LoadHolsters","i",playerid);
	return 1;
}

Weapon_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (IsPlayerAFK(playerid))				  return 1;
	if (pTemp[playerid][pInvulnerable] != 0.0) return 1;
	
	if (issuerid != INVALID_PLAYER_ID)
	{			
		new Float:health, Float:armour;

		AntiCheatGetHealth(playerid, health);
		AntiCheatGetArmour(playerid, armour);
		
	    if (PlayerInfo[playerid][pInjured] == 1 || PlayerInfo[playerid][pInjured] == 3)
		{
		    SetPlayerHealth(playerid, 100.0);

		    PlayerInfo[playerid][pInjured] = 2;
			PlayerInfo[issuerid][pDeathReason] = weaponid;
			
			SQL_SetInteger("users", "deathreason", weaponid, PlayerInfo[issuerid][pID]);

			SM3(playerid, "You died after being seriously injured.");
			ApplyAnimation(playerid,"PARACHUTE","FALL_skyDive_DIE",3.5,0,0,0,1,0);
			return 1;
		}

		if (PlayerInfo[playerid][pInjured] == 2)
		{
		    SetPlayerHealth(playerid, 9999.0);
		    return 1;
		}

        if (weaponid == 23 && pTemp[issuerid][pTaser])
		{
		    SetPlayerHealth(playerid, health);
            SetPlayerArmour(playerid, armour);

            if (!PlayerNearPlayer(10.0, playerid, issuerid))	return SM1(issuerid, "The player you are shooting at is far away.");

            new vehicle = GetPlayerVehicleID(playerid);

			if (vehicle && !IsABiker(vehicle) && !IsABicycle(vehicle))	return 1;
            if (pTemp[playerid][pTaserTime])							return SM1(issuerid, "This player has already been neutralized.");
            if (vehicle)												RemovePlayerFromVehicle(playerid);

            SavePlayerPos(playerid);
            ApplyAnimation(playerid, "SWEET", "Sweet_injuredloop", 4.0, 1, 0, 0, 1, 0);
            pTemp[playerid][pTaserTime] = 20;
			cmd::me(playerid, "fell (a), having received a shock from an electric shock.");
			SetPlayerDrunkLevel(playerid, 2500);
		    return 1;
		}
		else if ((PlayerInfo[issuerid][pAmmoType] == 89 || PlayerInfo[issuerid][pAmmoType] == 99) && PlayerInfo[issuerid][pPlayerAmmo])
		{
            SetPlayerHealth(playerid, health);
            SetPlayerArmour(playerid, armour);

            if (!PlayerNearPlayer(10.0, playerid, issuerid))	return SM1(issuerid, "The player you are shooting at is far away.");

			new vehicle = GetPlayerVehicleID(playerid);

			if (vehicle && !IsABiker(vehicle) && !IsABicycle(vehicle))	return 1;
            if (pTemp[playerid][pTaserTime])								return SM1(issuerid, "This player has already been neutralized.");
			if (vehicle)													RemovePlayerFromVehicle(playerid);

            SavePlayerPos(playerid);
            ApplyAnimation(playerid, "SWEET", "Sweet_injuredloop", 4.0, 1, 0, 0, 1, 0);
            pTemp[playerid][pTaserTime] = 20;

			if (PlayerInfo[playerid][pSex] == 1)	cmd::me(playerid, "was paralyzed non-lethal ammunition and fell to the ground.");

			SetPlayerDrunkLevel(playerid, 2500);
		    return 1;
		}

        if ((bodypart == 7 || bodypart == 8) && pTemp[playerid][pInjuredLegTime] == 0)
		{
		    SM3(playerid, "Your character was shot in the leg. You will not be able to run and jump fast for 30 seconds.");
    		pTemp[playerid][pInjuredLegTime] = INJURED_LEG_TIME;
		}
		
		new Float:damage;
		switch(weaponid)
		{
		    case 0: damage = float(randomEx(15, 30))/10.0; // Fist
			case 1: damage = 5.0; // Brass Knuckles
			case 2: damage = 5.0; // G�lf Club
			case 3: damage = 5.0; // Nightstick
			case 4: damage = 7.0; // Knife
			case 5: damage = 5.0; // Baseball Bat
			case 6: damage = 5.0; // Shovel
			case 7: damage = 5.0; // Pool Cue
			case 8: damage = 8.0; // Katana
			case 9: damage = 10.0; // Chainsaw
			case 14: damage = 2.0; // Flowers
			case 15: damage = 5.0; // Cane
			case 16: damage = 50.0; // Grenade
			case 18: damage = 20.0; // Molotov
			case 22: damage = float(randomEx(15, 20)); 				// M1911
			case 23, 28, 29, 32: damage = float(randomEx(17, 23)); 	// SD Pistol, UZI, MP5, Tec
			case 24: damage = float(randomEx(38, 43)); 				// Desert Eagle
			case 25, 26, 27: // Shotgun, Sawnoff Shotgun, CombatShotgun
			{
			    new Float: p_x, Float: p_y, Float: p_z;
			    GetPlayerPos(issuerid, p_x, p_y, p_z);
			    new Float: dist = GetPlayerDistanceFromPoint(playerid, p_x, p_y, p_z);

			    if (dist < 5.0)			damage = float(randomEx(50, 55));
				else if (dist < 10.0)	damage = float(randomEx(23, 35));
				else if (dist < 15.0)	damage = float(randomEx(15, 25));
				else if (dist < 20.0)	damage = float(randomEx(10, 15));
				else if (dist < 30.0)	damage = float(randomEx(5, 8));
			}
			case 30: damage = float(randomEx(20, 25)); // AK47
			case 31: damage = float(randomEx(20, 22)); // M4A1
			case 33, 34: damage = float(randomEx(70, 75)); // Country Rifle, Sniper Rifle
			case 35: damage = 0.0; // RPG
			case 36: damage = 0.0; // HS Rocket
			case 38: damage = 0.0; // Minigun
		}

        if (bodypart == BODY_PART_TORSO && armour > 0.0 && (22 <= weaponid <= 38))
		{
		    if (armour - damage <= 0.0)	SetPlayerArmour(playerid, 0.0);
	 		else						SetPlayerArmour(playerid, armour - damage);

            SetPlayerHealth(playerid, health);
			RecordDamage(issuerid, playerid, weaponid, bodypart, damage, 1);
		}
		else
		{
		    if (health - damage <= 5.0)	
			{
				SetPlayerHealth(playerid, 30);
				SetTimerEx("InjuredPlayer", 1, false, "iii", playerid, issuerid, weaponid);
			}
			else SetPlayerHealth(playerid, health - damage);
			if (armour) SetPlayerArmour(playerid, armour);

			if (health - amount <= 30.0 && health - amount >= 16.0)
			{
				SM3(playerid, "Your health is low, your shooting skills are lowered to medium.");
				SetPlayerSkill(playerid, 2);
			}
			else if (health - amount < 16.0)
			{
				SM3(playerid, "Your health is low, your shooting skills are reduced to minimal.");
				SetPlayerSkill(playerid, 3);
			}		

			RecordDamage(issuerid, playerid, weaponid, bodypart, damage, 0);
		}
	}
    return 1;
}

Weapon_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (hittype == BULLET_HIT_TYPE_PLAYER)
    {
        if ((floatcmp(floatabs(fX), 20.0) == 1) || (floatcmp(floatabs(fY), 20.0) == 1) || (floatcmp(floatabs(fZ), 20.0) == 1)) return KickEx(playerid), 0;
    }
    
	if (weaponid < 22 || weaponid > 38) return 0;
	
	if (pTemp[playerid][pTaser] == 0)
	{
		if (PlayerInfo[playerid][pPlayerAmmo] > 0) 	PlayerInfo[playerid][pPlayerAmmo]--;
		else {										PlayerInfo[playerid][pPlayerAmmo] = 0; PlayerInfo[playerid][pAmmoType] = 0; }

	    if (PlayerInfo[playerid][pPlayerAmmo] > GetPlayerAmmo(playerid)) PlayerInfo[playerid][pPlayerAmmo] = GetPlayerAmmo(playerid);
	}
    
	if (weaponid == 23 && pTemp[playerid][pTaser])
	{
	    PlayerPlaySoundEx(playerid, 6003);
	    ApplyAnimation(playerid, "COLT45", "colt45_reload", 4.1, 0, 0, 0, 0, 0, 1);

		if (GetPlayerWeaponState(playerid) == WEAPONSTATE_LAST_BULLET)
		{
			PlayerInfo[playerid][pPlayerAmmo]++;
	    	SetPlayerWeapons(playerid);
    		//pTemp[playerid][pTaser] = 0;
			//SetPlayerChatBubble(playerid, "������� ������������ �� ����������� ����.", COLOR_PURPLE, 20.0, 4000);
		}
	}

	if (PlayerInfo[playerid][pInjured] != 0)
	{
	    if (!PlayerToPoint(2.0, playerid, PlayerInfo[playerid][pPosX], PlayerInfo[playerid][pPosY], PlayerInfo[playerid][pPosZ])) SetPlayerPosEx(playerid, PlayerInfo[playerid][pPosX], PlayerInfo[playerid][pPosY], PlayerInfo[playerid][pPosZ]);
	    
		ApplyAnimation(playerid, "WUZI", "CS_Dead_Guy", 4.0, 1, 1, 1, 1, 0, 1);
	    return 0;
	}

	if (IsPlayerAFK(playerid))	return 0; //������ �� ���������� ����

	if (hittype == BULLET_HIT_TYPE_VEHICLE && hitid != INVALID_VEHICLE_ID)
	{
	    new Float: health;

	    AntiCheatGetVehicleHealth(hitid, health);
		
	    return (health < 350.0)?(0):(1);
	}

    return 1;
}

Weapon_OnPlayerStateChange(playerid, newstate)
{
    if (newstate == PLAYER_STATE_DRIVER)
    {
        if (PlayerHoldingWeapon(playerid, 24)) SetPlayerArmedWeapon(playerid, 0);
    }
    return 1;
}

Weapon_OnPlayerExitVehicle(playerid, vehicleid)
{
    if (IsAPlane(vehicleid) || IsAHelicopter(vehicleid) && PlayerInfo[playerid][pPlayerWeapon] != 46) ResetPlayerWeapons(playerid);
	
    return 1;
} 