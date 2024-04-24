ALTX:inventory("/inv","/inventario");
CMD:inventory(playerid, params[])
{
    DeletePVar(playerid, #inventory_house);
    DeletePVar(playerid, #inventory_vehicle);
    
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])    return SendErrorMessage(playerid, "Você não pode usar o inventário agora.");
    
    if (GetPVarInt(playerid, #pDelay)) return 1;

    new    
        count,
        diatxt[64] = "Inventário do Personagem"
    ;
    
    new houseid;
    if ((houseid = GetNearHouse(playerid)) != -1 && HouseInfo[houseid][hOwnerID] == PlayerInfo[playerid][pID] && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
    {
        format(diatxt, sizeof(diatxt), "%s\nInventário da Casa", diatxt);
        count++;
    }

    new vehicle = GetNearVehicle(playerid, 4.0);
    if (vehicle != 0 && (!IsADontTrunk(vehicle) || !IsAPlane(vehicle) || !IsABiker(vehicle) || !IsAHelicopter(vehicle) || !IsABoat(vehicle) || !IsABicycle(vehicle)))
    {
        if ((VehicleInfo[vehicle][carTrunkLock] && !IsPlayerInAnyVehicle(playerid)) || (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction]))
        {
            if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction]) format(diatxt, sizeof(diatxt), "%s\nInventário da Porta-luvas", diatxt);
            else                                                                                                        format(diatxt, sizeof(diatxt), "%s\nInventário do Porta-malas", diatxt);
            count++;
        }
    }
    
    vehicle = GetPlayerVehicleID(playerid);
    if (IsPlayerInAnyVehicle(playerid) && vehicle != 0 && GetPlayerHasRightDrive(playerid, vehicle))
    {
        format(diatxt, sizeof(diatxt), "%s\nInventário do Porta-luvas", diatxt);
        count++;
    }

    if (count)    Dialog_Show(playerid, InventoryMenu, DIALOG_STYLE_LIST, "Seleção de inventário:", diatxt, "Selecionar", "Cancelar");
    else        PrintInv(playerid);

    return 1;
}
ALTX:put("/colocar");
CMD:put(playerid, params[])
{
    if (!IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))    return SendWarningMessage(playerid, "Você não tem nada em suas mãos. ");
    if (!GetPVarInt(playerid, #attach_item))                        return SendErrorMessage(playerid, "Com este comando, você só pode remover certos itens do inventário! ");

    RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    SetPVarInt(playerid, #attach_item, 0);

    if (GiveInvItem(playerid, pTemp[playerid][tempItem], pTemp[playerid][tempQ], pTemp[playerid][tempEx]) == -1) return SendErrorMessage(playerid, "Inventário está cheio.");

    pTemp[playerid][tempItem] = 0;
    pTemp[playerid][tempEx] = 0;
    pTemp[playerid][tempQ] = 0;
            
    SendSyntaxMessage(playerid, "Você removeu um objeto de suas mãos. ");
    return 1;
}

PrintInv(playerid)
{
    new result[(MAX_INV_SLOTS+5) * 128] = "#\tItem\tQuantidade"; //{ffffff}
    if (GetPVarInt(playerid, #pDelay)) return PrintInv(playerid);

    FixInv(playerid);
    for(new i = 0; i < MAX_INV_SLOTS; i++) {
        switch(Inventory[playerid][i][pInvItem])
        {
            case 0:
            {
                format(result, sizeof(result), "%s\n{ffffff}%i.\t{C3C3C3}Slot vazio", result, i+1);
            }
            case 22 .. 34:
            {
                if (Inventory[playerid][i][pInvQ] == 0) format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]); 
                else                                    format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i rodada(s)]{FFFFFF}\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]); 
            }
            
            case 78, 79:
            {
                if (Inventory[playerid][i][pInvEx] == 0) format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]); 
                else                                     format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %f]{FFFFFF}\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQFloat]); 
            }
            
            case INV_GANJA, INV_MED_GANJA, 103 .. 118:
            {
                format(result, sizeof(result), "%s\n{ffffff}%i.\t%s{FFFFFF}\t%.1fg", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQFloat]);
            }
            case INV_DRUG:
            {
                format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[OAS: %i%%]{FFFFFF}\t%.1fg", result, i+1, Inventory[playerid][i][pInvName], Inventory[playerid][i][pInvEx], Inventory[playerid][i][pInvQFloat]);
            }
            case INV_GASCAN: 
            {
                format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
            }
            
            default:
            {
                format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQ]);
            }
        }
    }

    if (PlayerInfo[playerid][pPlayerWeapon] >= 1) 
    {
        format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}REMOVER ARMA", result, MAX_INV_SLOTS+1);
        format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}DESCARTAR ARMA", result, MAX_INV_SLOTS+2);
        
        if (PlayerInfo[playerid][pPlayerWeapon] >= 22 && PlayerInfo[playerid][pPlayerWeapon] <= 34)
        {
            if (PlayerInfo[playerid][pPlayerAmmo] > 0) 
                format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}REMOVER O CARREGADOR", result, MAX_INV_SLOTS+3);
        }
        if (PlayerInfo[playerid][pPlayerWeapon] == 23)
        {
            format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}REMOVER O SILENCIADOR", result, MAX_INV_SLOTS+4);
        }
    }

    if (GetPVarInt(playerid, #attach_item))
    {
        format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}TIRAR DA MÃO\n{ffffff}%i.\t{FF6347}> {FFFFFF}JOGAR FORA", result, MAX_INV_SLOTS+1, MAX_INV_SLOTS+2);
    }

    Dialog_Show(playerid, Inventory, DIALOG_STYLE_TABLIST_HEADERS, "Inventário", result, "Selecionar", "Cancelar");
    return true;
}

PrintInvEx(playerid, userid)
{
	new result[(MAX_INV_SLOTS+1) * 64] = "#\tItem\tQuantidade"; //{ffffff}
	if (GetPVarInt(playerid, #pDelay)) return PrintInvEx(playerid, userid);
	
	format(result, sizeof(result), "-\t{33AA33}Dinheiro\t$%i{FFFFFF}\n", PlayerInfo[playerid][pCash]);

    for(new i = 0; i < MAX_INV_SLOTS; i++) 
	{
		switch(Inventory[playerid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Slot vazio", result, i+1);
			}
			case 22 .. 34:
			{
				if (Inventory[playerid][i][pInvQ] == 0) format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]); 
				else 									format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i rodada(s)]{FFFFFF}\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]); 
			}

			case 78, 79:
			{
				if (Inventory[playerid][i][pInvEx] == 0) format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]); 
				else 									format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]); 
			}
			case INV_GASCAN: 
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQ]);
			}
	    }
	}
	new title[34];

	if (!IsAdminDuty(userid))
	{
		new mes[64];
		format(mes,sizeof(mes),"revista %s.", GetNameWithMask(playerid));
		cmd::me(playerid, mes);

		title = "Revistar %s";
	}
	else title = "Inventário %s";

	format(title, sizeof(title), title, GetNameWithMask(playerid));
	Dialog_Show(userid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	return true;
}


PrintVehInv(playerid, vehid, frisk = 0)
{
	new result[(MAX_TRUNK_INVENTORY+1) * 64] = "#\tItem\tQuantidade"; //{ffffff}
	if (GetPVarInt(playerid, #pDelay)) return PrintVehInv(playerid, vehid, frisk);

    for(new i = 0; i < MAX_TRUNK_INVENTORY; i++)
	{
		switch(TrunkInventory[vehid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Slot vazio", result, i+1);
			}
			case 22 .. 34:
			{
				if (TrunkInventory[vehid][i][pInvQ] == 0) 	format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName]); 
				else 										format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i rodada(s)]{FFFFFF}\t1", result, i+1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName], inventory_items[TrunkInventory[vehid][i][pInvEx]][itemName], TrunkInventory[vehid][i][pInvQ]); 
			}

			case 78, 79:
			{
				if (TrunkInventory[vehid][i][pInvEx] == 0) 	format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName]); 
				else 										format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i+1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName], inventory_items[TrunkInventory[vehid][i][pInvEx]][itemName], TrunkInventory[vehid][i][pInvQ]); 
			}
			case INV_GASCAN: 
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			default: format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i+1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName], TrunkInventory[vehid][i][pInvQ]);
	    }
	}

	new title[64];

	if (!frisk)
	{
		if (VehicleInfo[vehid][carFaction] && VehicleInfo[vehid][carFaction] == PlayerInfo[playerid][pFaction])  title = "Porta-luvas %s";
		else																									title = "Porta-malas %s";

		format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400]);
		Dialog_Show(playerid, InventoryOther, DIALOG_STYLE_TABLIST_HEADERS, title, result, "Selecionar", "Cancelar");
	}
	else 
	{
		if (!IsAdminDuty(playerid))
		{
			new mes[64];
			format(mes,sizeof(mes),"revista o porta-malas %s.", VehicleNames[GetVehicleModel(vehid)-400]);
			cmd::me(playerid, mes);

			title = "Revista do porta-malas %s";
			format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400]);
		} 
		else 
		{
			title = "Porta-malas %s | Dono: %s";
			if (VehicleInfo[vehid][carOwnerID]) 	format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400], VehicleInfo[vehid][carOwner]);
			else   								format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400], GetInitials(Factions[VehicleInfo[vehid][carFaction]][fName])); 
		}
		
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	}

	if (VehicleInfo[vehid][vCorp] > 0 && CorpInfo[VehicleInfo[vehid][vCorp]][cUsed] == 1) SendWarningMessage(playerid,"Há um cadáver no porta-malas do veículo.");

	return 1;
}

PrintGBInv(playerid, vehid, frisk = 0)
{
	new result[(MAX_CAR_INVENTORY+1) * 64] = "#\tItem\tQuantidade"; //{ffffff}
	if (GetPVarInt(playerid, #pDelay)) return PrintGBInv(playerid, vehid, frisk);

    for(new i = 0; i < MAX_CAR_INVENTORY; i++)
	{
		switch(CarInventory[vehid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Slot vazio", result, i+1);
			}
			case 22 .. 34:
			{
				if (CarInventory[vehid][i][pInvQ] == 0) 	format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName]); 
				else 										format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i rodada(s)]{FFFFFF}\t1", result, i+1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName], inventory_items[CarInventory[vehid][i][pInvEx]][itemName], CarInventory[vehid][i][pInvQ]); 
			}

			case 78, 79:
			{
				if (CarInventory[vehid][i][pInvEx] == 0) 	format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName]); 
				else 										format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i+1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName], inventory_items[CarInventory[vehid][i][pInvEx]][itemName], CarInventory[vehid][i][pInvQ]); 
			}
			
			case INV_GASCAN: 
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}

			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i+1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName], CarInventory[vehid][i][pInvQ]);
			}
	    }
	}

	new title[64];

	if (!frisk)
	{
		title = "Porta-luvas %s";
		format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400]);
		Dialog_Show(playerid, InventoryOther, DIALOG_STYLE_TABLIST_HEADERS, title, result, "Selecionar", "Cancelar");
	}
	else 
	{
		if (!IsAdminDuty(playerid))
		{
			new mes[64];
			format(mes,sizeof(mes),"revista o porta-luvas %s.", VehicleNames[GetVehicleModel(vehid)-400]);
			cmd::me(playerid, mes);

			title = "Revistando o porta-luvas %s";
			format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400]);
		}
		else 
		{
			title = "Porta-luvas %s | Dono: %s";
			if (VehicleInfo[vehid][carOwnerID]) 	format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400], VehicleInfo[vehid][carOwner]);
			else   								format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid)-400], GetInitials(Factions[VehicleInfo[vehid][carFaction]][fName])); 
		}

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	}	

	return 1;
}

PrintHouseInv(playerid, houseid, frisk = 0)
{
	new result[(MAX_HOUSE_INVENTORY+1) * 64] = "#\tItem\tQuantidade"; //{ffffff}
	if (GetPVarInt(playerid, #pDelay)) return PrintHouseInv(playerid, houseid, frisk);

    for(new i = 0; i < MAX_HOUSE_INVENTORY; i++)
	{
		switch(HouseInventory[houseid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Slot vazio", result, i+1);
			}
			case 22 .. 34:
			{
				if (HouseInventory[houseid][i][pInvQ] == 0) 	format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName]); 
				else 											format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i rodada(s)]{FFFFFF}\t1", result, i+1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName], inventory_items[HouseInventory[houseid][i][pInvEx]][itemName], HouseInventory[houseid][i][pInvQ]); 
			}

			case 78, 79:
			{
				if (HouseInventory[houseid][i][pInvEx] == 0) 	format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", result, i+1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName]); 
				else 											format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i+1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName], inventory_items[HouseInventory[houseid][i][pInvEx]][itemName], HouseInventory[houseid][i][pInvQ]); 
			}
			
			case INV_GASCAN: 
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			
			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i+1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName], HouseInventory[houseid][i][pInvQ]);
			}
	    }
	}

	if (!frisk) Dialog_Show(playerid, InventoryOther, DIALOG_STYLE_TABLIST_HEADERS, "Esconderijo", result, "Selecionar", "Cancelar");
	else 
	{	
		new title[42];
		if (!IsAdminDuty(playerid))
		{
			cmd::me(playerid, "revista a casa.");
			title = "Busca na casa %s";
		}
		else title = "Casa %s";

		format(title, sizeof(title), title, HouseInfo[houseid][hOwner]);
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	}
	return 1;
}
