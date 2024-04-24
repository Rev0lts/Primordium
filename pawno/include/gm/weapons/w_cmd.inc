CMD:recarregar(playerid, params[])
{
	if (PlayerInfo[playerid][pPlayerWeapon] && PlayerInfo[playerid][pPlayerAmmo])	return SendErrorMessage(playerid, "Your weapon is already loaded. ");
    if (!PlayerInfo[playerid][pPlayerWeapon]) 										return SendErrorMessage(playerid, "You have no weapon in your hands. ");
	
	new slotid = -1;
	for(new i = 0; i < MAX_INV_SLOTS; i++)
	{
		if (CompatAmmo(PlayerInfo[playerid][pPlayerWeapon], Inventory[playerid][i][pInvItem]))
		{
			slotid = i;
			break;
		}
	}
	if (slotid == -1) return SendErrorMessage(playerid, "You do not have a suitable cartridge for this weapon.");

	PlayerInfo[playerid][pAmmoType] = Inventory[playerid][slotid][pInvItem];
	PlayerInfo[playerid][pPlayerAmmo] = Inventory[playerid][slotid][pInvQ];

	GivePlayerWeaponEx(playerid, PlayerInfo[playerid][pPlayerWeapon], PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);
            
	SM1(playerid, "You reloaded %s - %s (%i round(s)).", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);

	new mes[64];
	format(mes, sizeof(mes), "* %s recharges %s", GetNameEx(playerid), inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName]);
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 30.0, 10000);
            
	RemoveInvItem(playerid, Inventory[playerid][slotid][pInvItem], 0, slotid);
            
	ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);

	return 1;
}

CMD:arma(playerid, params[])
{
	new option[24], secoption, thirdoption, fourthoption;

	if (sscanf(params, "s[24]D(-1)D(-1)D(-1)", option, secoption, thirdoption, fourthoption)) 
	{
		SendHelpMessage(playerid, "/arma [editar / corpo / esconder / resetar]");
		SendSyntaxMessage(playerid, "Use {ffc46a}/weaponhelp{FFFFFF} for more details.");
		return 1;
	}

	if (!strcmp(option, "bone", true))
	{
		new slot, bone, type;
		if (sscanf(params, "{s[7]}ddD(-1)", slot, bone, type))
		{
			SendHelpMessage(playerid, "/weapon bone [slot] [bone]");
			SendSyntaxMessage(playerid, "Slot  - [ 1 - Hand weapon | 2 - Secondary weapon | 3 - Primary weapon ]");
			SendSyntaxMessage(playerid, "Bone - [ 1 - Spine | 3 - Left arm shoulder | 4 - Right shoulder | 5 - Left hand | 6 - Right hand ]");
			SendSyntaxMessage(playerid, "Bone - [ 7 - Right thigh | 8 - Left thigh | 9 - Left leg | 10 - Right leg ]");
			return 1;
		}

		if (slot > 3 || slot < 1)						return SendErrorMessage(playerid, "Slot 1 to 3.");
		if (IsPlayerHoldingWeapon(playerid))				return SendErrorMessage(playerid, "Put the weapon in inventory.");
		if (bone < 1 || bone == 2 || bone > 10)  		return SendErrorMessage(playerid, "ID bones from 1 to 10 (except 2).");
		if (GetPVarInt(playerid, #edit_weapon) != 0)		return SendErrorMessage(playerid, "You are already editing the position of the weapon.");

		new weaponid = -1;
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (GetWeaponType(Inventory[playerid][i][pInvItem]) == slot-1)
			{
				weaponid = Inventory[playerid][i][pInvItem];
				break;
			}
		}
		if (weaponid == -1)	return SendErrorMessage(playerid, "You don't have a weapon suitable for the slot in your inventory.");

        SetPVarInt(playerid, #edit_weapon, slot);
		SetPlayerAttachedObject(playerid, 4 + slot, inventory_items[weaponid][itemModel], PlayerInfo[playerid][pWeaponBone][slot-1]);
		EditAttachedObject(playerid, 4 + slot);
		
		return 1;
	}//о
	else if (!strcmp(option, "edit", true))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))		 return SendHelpMessage(playerid, "/weapon adjust [Slot (1 - Hand weapon | 2 - Secondary weapon | 3 - Primary weapon)]");
		if (slot > 3 || slot < 1)					 return SendErrorMessage(playerid, "Slot 1 to 3.");
		if (IsPlayerHoldingWeapon(playerid))		 return SendErrorMessage(playerid, "Put the weapon in inventory.");
		if (GetPVarInt(playerid, #edit_weapon) != 0) return SendErrorMessage(playerid, "You are already editing the position of the weapon.");
		
		new weaponid = -1;
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (GetWeaponType(Inventory[playerid][i][pInvItem]) == slot-1)
			{
				weaponid = Inventory[playerid][i][pInvItem];
				break;
			}
		}
		if (weaponid == -1)	return SendErrorMessage(playerid, "You don't have a weapon suitable for the slot in your inventory.");

		SetPVarInt(playerid, #edit_weapon, slot);
		EditAttachedObject(playerid, 4 + slot);
		return 1;
	}
	else if (!strcmp(option, "reset", true))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))	return SendHelpMessage(playerid, "/weapon reset [Slot (1 - Hand weapon | 2 - Secondary weapon | 3 - Primary weapon)]");
		if (slot > 3 || slot < 1)			return SendErrorMessage(playerid, "Slot 1 to 3.");

		if (slot == 1)
		{
			PlayerInfo[playerid][pWeaponBone][0] = 1;
			PlayerInfo[playerid][pWeaponX][0] = 0.323999;
			PlayerInfo[playerid][pWeaponY][0] = -0.144999;
			PlayerInfo[playerid][pWeaponZ][0] = -0.166;
			PlayerInfo[playerid][pWeaponRX][0] = 0.0;
			PlayerInfo[playerid][pWeaponRY][0] = -72.0;
			PlayerInfo[playerid][pWeaponRZ][0] = 0.0;
			RemovePlayerAttachedObject(playerid, 5);
			Save_AttachWeapon(playerid, 0);
			SendSyntaxMessage(playerid, "You have returned the original handgun position.");
		}
		else if (slot == 2)
		{
			PlayerInfo[playerid][pWeaponBone][1] = 8;
			PlayerInfo[playerid][pWeaponX][1] = -0.239;
			PlayerInfo[playerid][pWeaponY][1] = -0.062999;
			PlayerInfo[playerid][pWeaponZ][1] = 0.079999;
			PlayerInfo[playerid][pWeaponRX][1] = -97.2;
			PlayerInfo[playerid][pWeaponRY][1] = -8.1;
			PlayerInfo[playerid][pWeaponRZ][1] = -1.7;
			RemovePlayerAttachedObject(playerid, 6);
			Save_AttachWeapon(playerid, 1);
			SendSyntaxMessage(playerid, "You have returned the original position of the main weapon.");
		}
		else if (slot == 3)
		{
			PlayerInfo[playerid][pWeaponBone][2] = 1;
			PlayerInfo[playerid][pWeaponX][2] = 0.106999;
			PlayerInfo[playerid][pWeaponY][2] = 0.207;
			PlayerInfo[playerid][pWeaponZ][2] = -0.079;
			PlayerInfo[playerid][pWeaponRX][2] = 179.1;
			PlayerInfo[playerid][pWeaponRY][2] = 143.1;
			PlayerInfo[playerid][pWeaponRZ][2] = -0.099999;
			RemovePlayerAttachedObject(playerid, 7);
			Save_AttachWeapon(playerid, 2);
			SendSyntaxMessage(playerid, "You have reset the secondary weapon to its original position.");
		}
		CallRemoteFunction("LoadHolsters","i",playerid);
		return 1;
	}
	else if (!strcmp(option, "hide", true))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))		 return SendHelpMessage(playerid, "/weapon hide [Slot (1 - Hand weapon | 2 - Secondary weapon | 3 - Primary weapon)]");
		if (slot > 3 || slot < 1)					 return SendErrorMessage(playerid, "Slot 1 to 3.");
		if (GetPVarInt(playerid, #edit_weapon) != 0) return SendErrorMessage(playerid, "You are already editing the position of the weapon.");
		
		new weaponid = -1;
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (GetWeaponType(Inventory[playerid][i][pInvItem]) == slot-1)
			{
				weaponid = Inventory[playerid][i][pInvItem];
				break;
			}
		}
		if (weaponid == -1)				return SendErrorMessage(playerid, "You do not have a suitable weapon in your inventory.");
		if (IsWeaponDontHide(weaponid))	return SendErrorMessage(playerid, "You cannot hide this weapon.");

		PlayerInfo[playerid][pHideWeap][slot-1] = (PlayerInfo[playerid][pHideWeap][slot-1]) ? (0) : (1);

		if (!PlayerInfo[playerid][pHideWeap][slot-1])	SendSyntaxMessage(playerid, "You drew your weapon. / slot: %i /", slot);
		else
		{
		    SendSyntaxMessage(playerid, "You hid a weapon. / slot: %i /", slot);
            RemovePlayerAttachedObject(playerid, 4 + slot);
		}

		CallRemoteFunction("LoadHolsters","i",playerid);
		return 1;
	}
	else
	{
		SendHelpMessage(playerid, "/weapon [edit / bone / hide / reset]");
	}
	return 1;
}

CMD:optica(playerid, params[])
{
    if (PlayerHoldingWeapon(playerid, 33))
 	{
		if (!IsPlayerHasItem(playerid, INV_OPTICS))	return SendErrorMessage(playerid, "You don't have a telescopic sight.");

        RemovePlayerItem(playerid, INV_OPTICS, 1);

		if (PlayerInfo[playerid][pPlayerAmmo])	ResetPlayerWeapons(playerid);
		else 									RemovePlayerAttachedObject(playerid, 9);
		
  		GivePlayerWeaponEx(playerid, 34, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);
    	ApplyAnimation(playerid, "RIFLE", "rifle_load", 4.1, 0, 0, 0, 0, 0, 1);

		SendSyntaxMessage(playerid, "You are wearing a telescopic sight.");
		return 1;
	}
 	else if (PlayerHoldingWeapon(playerid, 34))
  	{
		if (GiveInvItem(playerid, INV_OPTICS, 1))	return SendErrorMessage(playerid, "There is no room in your inventory for a telescopic sight.");

		if (PlayerInfo[playerid][pPlayerAmmo])	ResetPlayerWeapons(playerid);
		else 									RemovePlayerAttachedObject(playerid, 9);

  		GivePlayerWeaponEx(playerid, 33, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);
    	ApplyAnimation(playerid, "RIFLE", "rifle_load", 4.1, 0, 0, 0, 0, 0, 1);

    	SendSyntaxMessage(playerid, "You have removed the telescopic sight.");
     	return 1;
    }
    
    SendErrorMessage(playerid, "You should have a gun with a scope or a regular gun in your hands.");
	return 1;
}

CMD:silenciador(playerid, params[])
{
    if (pTemp[playerid][pTaser])	return SendErrorMessage(playerid, "Remove the shocker.");

	if (PlayerInfo[playerid][pPlayerWeapon] == 22)
 	{
		//if (PlayerInfo[playerid][pPlayerWeapon] == 22)	return SendErrorMessage(playerid, "У вас в руках должен быть пистолет с глушителем или обычный пистолет.");
		if (!IsPlayerHasItem(playerid, INV_SILENCER))	return SendErrorMessage(playerid, "You don't have a muffler.");

		SendSyntaxMessage(playerid, "You have put on a muffler.");

		if (PlayerInfo[playerid][pPlayerAmmo])	ResetPlayerWeapons(playerid);
		else 									RemovePlayerAttachedObject(playerid, 9);

  		RemovePlayerItem(playerid, INV_SILENCER, 1);
		GivePlayerWeaponEx(playerid, 23, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);

    	if (!IsPlayerInAnyVehicle(playerid)) ApplyAnimation(playerid, "COLT45", "colt45_reload", 4.1, 0, 0, 0, 0, 0, 1);
		return 1;
	}
 	else if (PlayerInfo[playerid][pPlayerWeapon] == 23)
  	{
		if (GiveInvItem(playerid, INV_SILENCER, 1) == -1)	return SendErrorMessage(playerid, "There is no room for a silencer in your inventory.");

		SendSyntaxMessage(playerid, "You have removed the muffler.");

  		if (PlayerInfo[playerid][pPlayerAmmo])	ResetPlayerWeapons(playerid);
		else 									RemovePlayerAttachedObject(playerid, 9);

		GivePlayerWeaponEx(playerid, 22, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);
    	if (!IsPlayerInAnyVehicle(playerid)) ApplyAnimation(playerid, "COLT45", "colt45_reload", 4.1, 0, 0, 0, 0, 0, 1);
     	return 1;
    }

	SendErrorMessage(playerid, "You should have a silenced pistol or a regular pistol in your hands.");
	return 1;
}

ALTX:takeprime("/tap","/armaprimaria");
CMD:takeprime(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])  
		return SendErrorMessage(playerid, "You cannot use this feature right now.");

	if (PlayerInfo[playerid][pPlayerWeapon]) return SendErrorMessage(playerid, "You already have a weapon in your hands! ");

	new foundid = -1, 
		str[64];

	if (PlayerInfo[playerid][pLastPrim] != -1) 
	{
		if (GetWeaponType(Inventory[playerid][PlayerInfo[playerid][pLastPrim]][pInvItem]) == WEAPON_TYPE_PRIMARY) 
		{
			foundid = PlayerInfo[playerid][pLastPrim];
		} 
		else 
		{
			PlayerInfo[playerid][pLastPrim] = -1;
			SQL_SetInteger("users", "LastPrim", -1, PlayerInfo[playerid][pID]);
		}
	}
	if (foundid == -1) 
	{
		for(new i = 0; i < MAX_INV_SLOTS; i++) 
		{
			if (Inventory[playerid][i][pInvItem] >= 25 && Inventory[playerid][i][pInvItem] <= 34) 
			{
				foundid = i;
				break;
			}
		}
	}
	if (foundid == -1) return SendErrorMessage(playerid, "You don't have large-caliber weapons!");

    GivePlayerWeaponEx(playerid, Inventory[playerid][foundid][pInvItem], Inventory[playerid][foundid][pInvQ], Inventory[playerid][foundid][pInvEx], Inventory[playerid][foundid][pInvS]);
	ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);
	
   	SM2(playerid, "You got %s from inventory.", inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);

    format(str, sizeof(str), "* %s took out %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);
   	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);
	
	RemoveInvItem(playerid, Inventory[playerid][foundid][pInvItem], 0, foundid);
	return 1;
}

ALTX:takesecond("/ts","/armasecundaria");
CMD:takesecond(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])  
		return SendErrorMessage(playerid, "You cannot use this feature right now.");

	if (PlayerInfo[playerid][pPlayerWeapon]) return SendErrorMessage(playerid, "You already have a weapon in your hands!");
	    
	new 
		foundid = -1, 
		str[64]
	;

	if (PlayerInfo[playerid][pLastSec] != -1) 
	{
		if (GetWeaponType(Inventory[playerid][PlayerInfo[playerid][pLastSec]][pInvItem]) == WEAPON_TYPE_SECONDARY) 
		{
			foundid = PlayerInfo[playerid][pLastSec];
		} 
		else 
		{
			PlayerInfo[playerid][pLastSec] = -1;
			SQL_SetInteger("users", "LastSec", -1, PlayerInfo[playerid][pID]);
		}
	}
	if (foundid == -1)
	{
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (Inventory[playerid][i][pInvItem] >= 22 && Inventory[playerid][i][pInvItem] <= 24)
			{
				foundid = i;
				break;
			}
		}
	}
	if (foundid == -1) return SendErrorMessage(playerid, "You do not have small-caliber weapons!");

    GivePlayerWeaponEx(playerid, Inventory[playerid][foundid][pInvItem], Inventory[playerid][foundid][pInvQ], Inventory[playerid][foundid][pInvEx], Inventory[playerid][foundid][pInvS]);
	ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);
	
	SM2(playerid, "You got %s from inventory.", inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);

    format(str, sizeof(str), "* %s took out %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);
   	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);
	
	RemoveInvItem(playerid, Inventory[playerid][foundid][pInvItem], 0, foundid);
	return 1;
}//о

ALTX:takemelee("/tm");
CMD:takemelee(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])  
		return SendErrorMessage(playerid, "You cannot use this feature right now.");

	if (PlayerInfo[playerid][pPlayerWeapon]) return SendErrorMessage(playerid, "You already have a weapon in your hands!");
	    
	new 
		foundid = -1, 
		str[64]
	;

	if (PlayerInfo[playerid][pLastMelee] != -1) 
	{
		if (GetWeaponType(Inventory[playerid][PlayerInfo[playerid][pLastMelee]][pInvItem]) == WEAPON_TYPE_MELEE) 
		{
			foundid = PlayerInfo[playerid][pLastMelee];
		} 
		else 
		{
			PlayerInfo[playerid][pLastMelee] = -1;
			SQL_SetInteger("users", "LastMelee", -1, PlayerInfo[playerid][pID]);
		}
	}
	if (foundid == -1) 
	{
		for(new i = 0; i < MAX_INV_SLOTS; i++) 
		{
			if (Inventory[playerid][i][pInvItem] >= 1 && Inventory[playerid][i][pInvItem] <= 15) 
			{
				foundid = i;
				break;
			}
		}
	}
	if (foundid == -1) return SendErrorMessage(playerid, "You have no hand weapons.");

	GivePlayerWeaponEx(playerid, Inventory[playerid][foundid][pInvItem], Inventory[playerid][foundid][pInvQ], Inventory[playerid][foundid][pInvEx], Inventory[playerid][foundid][pInvS]);
	
	SM2(playerid, "You got %s from inventory.", inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);

    format(str, sizeof(str), "* %s took out %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);
   	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);
	
	RemoveInvItem(playerid, Inventory[playerid][foundid][pInvItem], 0, foundid);
	return 1;
}

ALTX:putgun("/pg");
CMD:putgun(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "You cannot use this feature right now.");

	if (!PlayerInfo[playerid][pPlayerWeapon]) 	return SendErrorMessage(playerid, "You have no weapons in your hands.");
	if (!CheckInv(playerid)) 					return SendErrorMessage(playerid, "Your inventory is completely full.");

    new 
		str[64],
		wep = CheckPlayerWeaponEx(playerid)
	;
	
	if (!wep) return SendAdmMessage("Player %s[ID:%i] suspected of using cheats on weapons!", GetNameEx(playerid), playerid);
	
	if (wep >= 22 && wep <= 34) {
		new am = CheckAmmo(PlayerInfo[playerid][pAmmoType]);
		if (PlayerInfo[playerid][pPlayerAmmo] > am) PlayerInfo[playerid][pPlayerAmmo]=am;
	} 

	new ammo = PlayerInfo[playerid][pPlayerAmmo], 
		ammotype = PlayerInfo[playerid][pAmmoType], 
		serial = PlayerInfo[playerid][pWepSerial];

	ResetPlayerWeapons(playerid);
	PlayerInfo[playerid][pPlayerWeapon] = 
	PlayerInfo[playerid][pPlayerAmmo] = 
	PlayerInfo[playerid][pAmmoType] = 
	PlayerInfo[playerid][pWepSerial] = 0;

	new slot = GiveInvItem(playerid, wep, ammo, ammotype, serial);
	if (slot == -1)	return SendErrorMessage(playerid, "Make a screenshot, post it with / timestamp on the forum in Bug Tracker!");
	
	switch(GetWeaponType(wep)) 
	{
		case WEAPON_TYPE_PRIMARY: 
		{
			PlayerInfo[playerid][pLastPrim] = slot;
			SQL_SetInteger("users", "LastPrim", slot, PlayerInfo[playerid][pID]);	
		}
		case WEAPON_TYPE_SECONDARY: 
		{
			PlayerInfo[playerid][pLastSec] = slot;
			SQL_SetInteger("users", "LastSec", slot, PlayerInfo[playerid][pID]);
		}
		case WEAPON_TYPE_MELEE: 
		{
			PlayerInfo[playerid][pLastMelee] = slot;
			SQL_SetInteger("users", "LastMelee", slot, PlayerInfo[playerid][pID]);
		}
	}

    SM2(playerid, "You removed %s in inventory.", inventory_items[wep][itemName]);

    format(str, sizeof(str), "* %s removed the weapon.", GetNameWithMask(playerid));
	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

    RemovePlayerAttachedObject(playerid, 9);
    CallRemoteFunction("LoadHolsters","i",playerid);

	return 1;
}