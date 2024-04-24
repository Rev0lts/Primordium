Dialog:Inventory(playerid, response, listitem, inputtext[])
{
	if (!response)										return 1;
	if (GetPVarInt(playerid, "pDelay"))					return cmd::inventory(playerid, "");

	new title[56], mes[256];

	if (listitem == MAX_INV_SLOTS) // STORE WEAPON SECTION !
	{
		if (GetPVarInt(playerid, #attach_item)) 	return cmd::put(playerid, "");
	    else 									return cmd::putgun(playerid, "");
	}
	else if (listitem == MAX_INV_SLOTS + 1) // DROP WEAPON SECTION !
	{
		if (GetPVarInt(playerid, #attach_item))	return cmd::drop(playerid, "");
	    else 									return cmd::dropgun(playerid, "");
	}
	else if (listitem == MAX_INV_SLOTS + 2) // STORE AMMO SECTION !
	{
		if (PlayerInfo[playerid][pPlayerAmmo] < 1) return cmd::silenciador(playerid, "");
		
	    if (!CheckInv(playerid))					return SendErrorMessage(playerid, "Seu inventário está completamente cheio.");
		if (!PlayerInfo[playerid][pPlayerAmmo])	return SendErrorMessage(playerid, "Não há pente na arma.");

		new wep = CheckPlayerWeaponEx(playerid);
		if (!wep) return SendAdmMessage("%s[ID:%i] suspeito de usar cheats em armas!", GetNameEx(playerid), playerid);

		if (wep >= 22 && wep <= 34) {
			new am = CheckAmmo(PlayerInfo[playerid][pAmmoType]);
			if (PlayerInfo[playerid][pPlayerAmmo] > am) PlayerInfo[playerid][pPlayerAmmo]=am;
		} 

		if (GiveInvItem(playerid, PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pPlayerAmmo]) == -1)
			return SendErrorMessage(playerid, "Inventário está cheio.");
			
		
		SendWarningMessage(playerid, "Você descarregou %s - %s, %i rodada(s).", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);

		new msg[64];
		format(msg, sizeof(msg), "* %s retira o pente da arma.", GetNameWithMask(playerid));
		SetPlayerChatBubble(playerid, msg, COLOR_PURPLE, 20.0, 4000);

		PlayerInfo[playerid][pPlayerAmmo]=0;
		PlayerInfo[playerid][pAmmoType]=0;
		
		ResetPlayerWeapons(playerid);	
		
		return true;
	}

	else if (listitem == MAX_INV_SLOTS + 3) // ÃËÓØÈÒÅËÜ
	{
		return cmd::silenciador(playerid, "");
	}

	if (Inventory[playerid][listitem][pInvItem] == 0)	return cmd::inventory(playerid, "");

	if (inventory_items[Inventory[playerid][listitem][pInvItem]][itemSplit])
		if ((inventory_items[Inventory[playerid][listitem][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][listitem][pInvItem]][itemType] == ITEM_TYPE_SEEDS) && (IsPlayerHasItem(playerid, 78) || IsPlayerHasItem(playerid, 79)))
			mes = "Usar item\nTransferir item\nDescartar item\nDestruir item\nDividir\nCombinar\nColocar em bolsa / caixa";
		else
			mes = "Usar item\nTransferir item\nDescartar item\nDestruir item\nDividir\nCombinar";
	else
	{
		if (inventory_items[Inventory[playerid][listitem][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
		{
			if (Inventory[playerid][listitem][pInvEx]) 
				format(mes, sizeof(mes), "Alcançar %s(%f)\nTransferir o item\nDescartar o item\nDestruir o item", inventory_items[Inventory[playerid][listitem][pInvEx]][itemName], Inventory[playerid][listitem][pInvQFloat]);
			else mes = "Colocar um item\nTransferir um item\nDescartar um item\nDestruir um item";

			if (!GetPVarInt(playerid, #attach_item)) format(mes, sizeof(mes), "%s\nPegar", mes);
		}
		else
			mes = "Usar item\nTransferir item\nDescartar item\nDestruir item";
	}

    new houseid = -1;
	if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
	{
	    SetPVarInt(playerid, #inventory_house, houseid+1);
 		format(mes, sizeof(mes), "%s\n{ffff00}Colocar na casa (ID: %i)", mes, houseid);
	}

	new vehicle = GetNearVehicle(playerid, 4.0);
	if (vehicle != 0 && (!IsADontTrunk(vehicle) || !IsAPlane(vehicle) || !IsABiker(vehicle) || !IsAHelicopter(vehicle) || !IsABoat(vehicle) || !IsABicycle(vehicle)))
	{
		if ((VehicleInfo[vehicle][carTrunkLock] && !IsPlayerInAnyVehicle(playerid)) || (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction]))
		{
			SetPVarInt(playerid, #inventory_vehicle, vehicle);
			format(mes, sizeof(mes), "%s\n{ffff00}Colocar em %s (ID: %i)", mes, VehicleNames[GetVehicleModel(vehicle)-400], vehicle);
		}
	}

	if (GetPVarInt(playerid, #inventory_vehicle) == 0 && inventory_items[Inventory[playerid][listitem][pInvItem]][itemSize] == ITEM_SIZE_SMALL)
	{
		vehicle = GetPlayerVehicleID(playerid);
		if (IsPlayerInAnyVehicle(playerid) && vehicle != 0 && VehicleInfo[vehicle][carOwnerID] == PlayerInfo[playerid][pID])
		{
			format(mes, sizeof(mes), "%s\n{ffff00}Colocar no porta-luvas %s (ID: %i)", mes, VehicleNames[GetVehicleModel(vehicle)-400], vehicle);	
		}
	}

	SetPVarInt(playerid, #inventory_choose, listitem);
	format(title, sizeof(title), "{FFFFFF}Inventário > %s", inventory_items[Inventory[playerid][listitem][pInvItem]][itemName]);
    Dialog_Show(playerid, InventoryChoose, DIALOG_STYLE_LIST, title, mes, "Selecionar", "Retornar");
	return 1;
}

Dialog:InventoryChoose(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::inventory(playerid, "");

	new slot = GetPVarInt(playerid, #inventory_choose);

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || Inventory[playerid][slot][pInvItem] == 0)	return cmd::inventory(playerid, "");
	
	if (listitem > 3 || (listitem == 0 && inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT))
	{
		new listadd;
		if (inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit])
			if ((inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_SEEDS) && (IsPlayerHasItem(playerid, 78) || IsPlayerHasItem(playerid, 79)))
				listadd = 3;
			else
				listadd = 2;
		else 
		{
			if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
			{
				if (listitem == 4)
				{
					if (!GetPVarInt(playerid, #attach_item))	listadd = 6; //Âçÿòü â ðóêè								
				}
				else
				{
					if (Inventory[playerid][slot][pInvEx]) 	listadd = 4; // Äîñòàòü
					else 									listadd = 5; // Ïîëîæèòü
				}											
			}
			else listadd = 0;
		}

		switch(listitem)
		{
			case 0:
			{
				switch(listadd)
				{
					case 4: listitem = 8; // Pegar
					case 5: listitem = 9; // Colocar
				}
			}
			case 4:
			{
				switch(listadd)
				{
					case 0: listitem = 4; // BAGAGEIRO
					case 2: listitem = 5; // DIVIDIR
					case 3: listitem = 5; // DIVIDIR
					case 6: listitem = 10; // PEGAR NAS MÃOS
				}
			}
			case 5:
			{
				switch(listadd)
				{
					case 2: listitem = 6; // COMBINAR
					case 3: listitem = 6; // COMBINAR
				}
			}
			case 6:
			{
				switch(listadd)
				{
					case 2: listitem = 4; // BAGAGEIRO
					case 3: listitem = 7; // COLOCAR EM
				}
			}
			case 7:
			{
				listitem = 4; // BAGAGEIRO
			}
		}
	}

	switch(listitem)
	{
	    case 0: CallLocalFunction("OnPlayerUseItem", "ii", playerid, slot);
		case 1: Dialog_Show(playerid, GiveItem, DIALOG_STYLE_INPUT, "{FFFFFF}Transferir item", "{ffffff}Digite o nome ou ID do jogador para quem você deseja transferir o item:", ">>>", "Retornar");
		case 2:
		{
			if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_AMMO || inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
			{
				if (CheckAdmin(playerid, ADMIN_LEVEL_1) || IsTester(playerid))	return SendErrorMessage(playerid, "Primeiro saia do modo admin.");
				if (PlayerInfo[playerid][pOnDuty])								return SendErrorMessage(playerid, "Esta função não está disponível em serviço.");
			}

			if (Inventory[playerid][slot][pInvQ])	
			{
				if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
				{
					if (ItemDrop_Drop(playerid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)	return SendErrorMessage(playerid, "O número máximo de itens largados foi atingido.");
					switch(inventory_items[Inventory[playerid][slot][pInvItem]][itemType]) {
						case 22..34: SendSyntaxMessage(playerid, "Você largou %s [%s / %i rodada(s)]. Digite {ffc46a}/pitem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], inventory_items[Inventory[playerid][slot][pInvEx]][itemName], inventory_items[Inventory[playerid][slot][pInvQ]][itemName]);
						default: 	 SendSyntaxMessage(playerid, "Você largou %s. Digite {ffc46a}/pitem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
					}
				}
				else if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_REACT)
				{
					
				}
				else 
				{
					if (ItemDrop_Drop(playerid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)	return SendErrorMessage(playerid, "O número máximo de itens largados foi atingido.");
					SendSyntaxMessage(playerid, "Você largou %s [%i øò]. Digite {ffc46a}/pitem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
				}
			}
			else
			{
				if (ItemDrop_Drop(playerid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)	return SendErrorMessage(playerid, "O número máximo de itens largados foi atingido.");
				SendSyntaxMessage(playerid, "Você largou %s. Digite {ffc46a}/pitem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);			
			}
			RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
		}
		case 3:
		{
			new mes[128];
			format(mes, sizeof(mes), "{ffffff}Você realmente quer destruir o item - %s?", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
   			Dialog_Show(playerid, DeleteItem, DIALOG_STYLE_MSGBOX, "{FFFFFF}Destruir o item", mes, "Sim", "Cancelar");
		}
		case 4:
		{
		    new mes[128];

		    if (GetPVarInt(playerid, #inventory_house) != 0)
			{
				if (Inventory[playerid][slot][pInvQ] > 1 && inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit]) 
				{
					format(mes, sizeof(mes), "{ffffff}Item: %s (Quantidade: %i)\n\nDigite a quantidade que você gostaria de guardar no esconderijo em casa: ",
					inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
				}
				else
				{
					new 
						houseid = -1,
						i = GetNextHouseTrunkSlot(houseid),
						item = Inventory[playerid][slot][pInvItem]
					;

					if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
					{
						if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Você não pode guardar uma arma no esconderijo enquanto está em serviço.");

						if (i == -1)		return SendErrorMessage(playerid, "Não há espaço na casa.");
						if (item == 0) 		return cmd::inventory(playerid, "");
						
						SM2(playerid, "Você guardou %s no esconderijo secreto da casa.", inventory_items[item][itemName]);

						HouseInventory[houseid][i][pInvItem] 	= Inventory[playerid][slot][pInvItem];
						HouseInventory[houseid][i][pInvQ]		= Inventory[playerid][slot][pInvQ];
						HouseInventory[houseid][i][pInvEx]		= Inventory[playerid][slot][pInvEx];
						HouseInventory[houseid][i][pInvS]		= Inventory[playerid][slot][pInvS];
						HouseInventory[houseid][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
						format(HouseInventory[houseid][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
						
						RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
						
						format(mes, sizeof(mes), "guarda %s no esconderijo secreto da casa.", inventory_items[item][itemName]);
						SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

						Save_HouseInventory(houseid);
						return cmd::inventory(playerid, "");
					}
				}
			}
			else if (GetPVarInt(playerid, #inventory_vehicle) != 0)
			{
			    if (Inventory[playerid][slot][pInvQ] > 1 && inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit]) 
			    {
			        format(mes, sizeof(mes), "{ffffff}Item: %s (Quantidade: %i)\n\nDigite a quantidade que você deseja colocar no porta-malas: ",
			        inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
			    }
			    else 
			    {
			        new vehicle = GetPVarInt(playerid, #inventory_vehicle);

			        new item = Inventory[playerid][slot][pInvItem];
			        if (item == 0) return cmd::inventory(playerid, "");

			        if (vehicle > 0 && GetNearVehicle(playerid, 5.0, vehicle) != 0)
			        {
			            if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle)) return SendErrorMessage(playerid, "Esse veículo não possui um porta-malas.");
			            if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você não pode acessar o porta-malas do seu veículo oficial.");
			            if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0) return SendErrorMessage(playerid,"Você não pode colocar uma arma no seu veículo pessoal enquanto está de serviço.");
			            if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você deve estar no porta-malas.");
			            if (!IsPlayerInAnyVehicle(playerid) && !VehicleInfo[vehicle][carTrunkLock]) return SendErrorMessage(playerid, "O porta-malas deste veículo está fechado.");

			            new i = GetNextVehicleTrunkSlot(vehicle);
			            if (i == -1) return SendErrorMessage(playerid, "Não há espaço no veículo. ");

			            TrunkInventory[vehicle][i][pInvItem]     = Inventory[playerid][slot][pInvItem];
			            TrunkInventory[vehicle][i][pInvQ]        = Inventory[playerid][slot][pInvQ];
			            TrunkInventory[vehicle][i][pInvEx]       = Inventory[playerid][slot][pInvEx];
			            TrunkInventory[vehicle][i][pInvS]        = Inventory[playerid][slot][pInvS];
			            TrunkInventory[vehicle][i][pInvQFloat]   = Inventory[playerid][slot][pInvQFloat];
			            format(TrunkInventory[vehicle][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
			            
			            RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			            
			            SM2(playerid, "Você colocou %s no %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);

			            format(mes, sizeof(mes), "coloca %s no %s %s.", inventory_items[item][itemName], (VehicleInfo[vehicle][carFaction])?("interior"):("porta-malas"), VehicleNames[GetVehicleModel(vehicle)-400]);
			            SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

			            Save_CarInventory(vehicle);
			            return cmd::inventory(playerid, "");
			        }
			    }
			}
			else if (GetPVarInt(playerid, #inventory_vehicle) == 0 && IsPlayerInAnyVehicle(playerid))
			{
			    new vehicle = GetPlayerVehicleID(playerid);
			    if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID] && vehicle == 0) return cmd::inventory(playerid, "");

			    if (Inventory[playerid][slot][pInvQ] > 1 && inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit]) 
			    {
			        format(mes, sizeof(mes), "{ffffff}Item: %s (Quantidade: %i)\n\nDigite a quantidade que você deseja colocar no porta-luvas:", 
			        inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
			    }
			    else 
			    {
			        new 
			            i = GetNextVehicleGBSlot(vehicle),
			            item = Inventory[playerid][slot][pInvItem]
			        ;

			        if (vehicle == 0) return cmd::inventory(playerid, "");
			        if (item == 0) return cmd::inventory(playerid, "");
			        if (i == -1) return SendErrorMessage(playerid, "Não há espaço no veículo. ");

			        if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "O porta-luvas deste veículo não está disponível para você.");
			        if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0)
			            return SendErrorMessage(playerid, "Você não pode colocar uma arma no seu veículo pessoal enquanto está de serviço. ");
			        
			        CarInventory[vehicle][i][pInvItem]     = Inventory[playerid][slot][pInvItem];
			        CarInventory[vehicle][i][pInvQ]        = Inventory[playerid][slot][pInvQ];
			        CarInventory[vehicle][i][pInvEx]       = Inventory[playerid][slot][pInvEx];
			        CarInventory[vehicle][i][pInvS]        = Inventory[playerid][slot][pInvS];
			        CarInventory[vehicle][i][pInvQFloat]   = Inventory[playerid][slot][pInvQFloat];
			        format(CarInventory[vehicle][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
			            
			        RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			            
			        SM2(playerid, "Você colocou %s no porta-luvas %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);

			        format(mes, sizeof(mes), "coloca o porta-luvas em %s %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);
			        SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

			        Save_CarInventory(vehicle);
			        return cmd::inventory(playerid, "");
			    }
			}
			else return SendErrorMessage(playerid, "Algo deu errado!");

			Dialog_Show(playerid, PutItemInProperty, DIALOG_STYLE_INPUT, "{FFFFFF}Colocar item", mes, "Colocar", "Cancelar");
			return 1;
		}
		case 5: // Dividir 
		{
		    if (!inventory_items[Inventory[playerid][slot][pInvItem]][itemFloat]) {
		        if (Inventory[playerid][slot][pInvQ] < 2)    
		            return SendErrorMessage(playerid, "Para divisão, a quantidade de itens deve ser maior que 1.");
		                        
		        new divide_string[128];
		        format(divide_string, sizeof(divide_string), "{FFFFFF}Quantos você deseja separar de %s {ffc46a}(%i){FFFFFF}?", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
		        Dialog_Show(playerid, DivideInvInt, DIALOG_STYLE_INPUT, "Dividindo item", divide_string, ">>>","Voltar");
		    } else {
		        if (Inventory[playerid][slot][pInvQFloat] < 0.2)    
		            return SendErrorMessage(playerid, "Para divisão, a quantidade de itens deve ser maior que 0,2.");
		                        
		        new divide_string[128];
		        format(divide_string, sizeof(divide_string), "{FFFFFF}Quantos gramas você deseja separar de %s {ffc46a}(%.1fg){FFFFFF}?", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQFloat]);
		        Dialog_Show(playerid, DivideInvFloat, DIALOG_STYLE_INPUT, "Dividindo item", divide_string, ">>>","Voltar");
		    }
		    
		    return 1;
		}
		case 6: // Combinar
		{
		    FixInv(playerid);

		    SendHelpMessage(playerid, "Selecione o objeto com o qual você gostaria de combinar %s (%i).", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);

		    new dlgstr[20 * 128] = "#\tItem\tQuantidade";

		    new ccount;
		    for(new i = 0; i < MAX_INV_SLOTS; i++) 
		    {
		        switch(Inventory[playerid][i][pInvItem])
		        {
		            case 0: format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i\t{C3C3C3}Slot vazio", dlgstr, i+1);
		            case 22 .. 34:
		            {
		                if (Inventory[playerid][i][pInvQ] == 0) format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s {C3C3C3}[vazio]\t1", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]); 
		                else format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i rodada(s)]{FFFFFF}\t1", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]); 
		            }
		            case INV_GANJA, INV_MED_GANJA, 103 .. 118:
		            {
		                format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s{FFFFFF}\t%.1fg", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQFloat]);
		            }
		            case INV_DRUG:
		            {
		                format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s {C3C3C3}[OAS: %i%%]{FFFFFF}\t%.1fg", dlgstr, i+1, Inventory[playerid][i][pInvName], Inventory[playerid][i][pInvEx], Inventory[playerid][i][pInvQFloat]);
		            }
		            default: format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s\t%i", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQ]);    
		        }
		        ccount++;
		    }
		    if (!ccount) return SendErrorMessage(playerid, "Nenhum objeto correspondente!");

		    Dialog_Show(playerid, CombInv, DIALOG_STYLE_TABLIST_HEADERS, "Inventário > Combinar", dlgstr, "Selecionar", "Cancelar");
		    return 1;
		}
		case 7: // Colocar em
		{
		    new dlgstr[32*MAX_INV_SLOTS];

		    format(dlgstr, sizeof(dlgstr), "Escolha onde você deseja colocar '%s':{FFFFFF}\n", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

		    new ccount;
		    for(new i = 0; i < MAX_INV_SLOTS; i++) 
		    {
		        if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
		        {
		            if (!Inventory[playerid][i][pInvEx])
		            {
		                ccount++;
		                format(dlgstr, sizeof(dlgstr), "%s%i. %s\n", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
		            }
		        }
		    }
		    if (!ccount) return SendErrorMessage(playerid, "Nenhum objeto correspondente!");

		    Dialog_Show(playerid, InventoryPut, DIALOG_STYLE_TABLIST_HEADERS, "Inventário", dlgstr, "Selecionar", "Cancelar");
		    return 1;
		}
		case 8: // Retirar
		{
		    if (!Inventory[playerid][slot][pInvEx]) return 1;

		    if (!CheckInv(playerid))
		    {
		        cmd::inventory(playerid, "");
		        return SendErrorMessage(playerid, "O inventário está cheio.");
		    }

		    if (GiveInvItem(playerid, Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvQ]) == -1) return SendErrorMessage(playerid, "O inventário está cheio.");
		    SendSyntaxMessage(playerid, "Você obteve %s (%i) de %s.", inventory_items[Inventory[playerid][slot][pInvEx]][itemName], Inventory[playerid][slot][pInvQ], inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

		    Inventory[playerid][slot][pInvEx] = 0;
		    Inventory[playerid][slot][pInvQ] = 1;

		    return 1;    
		}
		case 9: // Remover
		{
		    new dlgstr[32*MAX_INV_SLOTS];

		    format(dlgstr, sizeof(dlgstr), "Selecione o item que você deseja remover em '%s':{FFFFFF}\n", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

		    new ccount;
		    for(new i = 0; i < MAX_INV_SLOTS; i++) 
		    {
		        if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_SEEDS)
		        {
		            ccount++;
		            if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_DRUGS){
		                format(dlgstr, sizeof(dlgstr), "%s%i. %s\n", dlgstr, i+1, Inventory[playerid][i][pInvName]);
		            } else {
		                format(dlgstr, sizeof(dlgstr), "%s%i. %s\n", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
		            }
		            
		        }
		    }

		    if (!ccount) return SendErrorMessage(playerid, "Nenhum objeto correspondente!");

		    Dialog_Show(playerid, InventoryPut2, DIALOG_STYLE_TABLIST_HEADERS, "Inventário", dlgstr, "Selecionar", "Cancelar");
		    return 1;
		}
		case 10:
		{
		    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		        return SendErrorMessage(playerid, "Você não pode usar esta função agora.");

		    if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");
		    if (GetPlayerVehicleID(playerid) != 0) return SendErrorMessage(playerid, "Você precisa estar fora do veículo.");

		    if (Inventory[playerid][slot][pInvItem] == 78)
		    {
		        SetPlayerAttachedObject(playerid, ATTACH_HANDS, 19571, 6, -0.157999, -0.002000, -0.169000, -21.999990, 87.000022, 0.000000, 1.186000, 1.000000, 1.170000);
		        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
		    }
		    else if (Inventory[playerid][slot][pInvItem] == 79)
		    {
		        SetPlayerAttachedObject(playerid, ATTACH_HANDS, inventory_items[Inventory[playerid][slot][pInvItem]][itemModel], 6, 0.204, 0.012, 0.026, 0.0, -101.3, -10.5, 0.838, 0.5, 0.597);
		    }

		    SetPVarInt(playerid, #attach_item, 1);

		    pTemp[playerid][tempItem] = Inventory[playerid][slot][pInvItem];
		    pTemp[playerid][tempEx] = Inventory[playerid][slot][pInvEx];
		    pTemp[playerid][tempQ] = Inventory[playerid][slot][pInvQ];

		    InfoMSG(playerid, "Você pode remover ou descartar este item das suas mãos~n~~y~No inventário~w~ digitando o comando ~y~/PUT~w~ ou ~y~/DROP~w~", 5);
		    SendSyntaxMessage(playerid, "Você pegou %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

		    RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
		    return 1;
		}
	}
	return 1;
}

Dialog:InventoryPut2(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new count,
		slot = GetPVarInt(playerid, #inventory_choose);

	for(new i = 0; i < MAX_INV_SLOTS; i++) 
	{
		if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_SEEDS)
		{
			if (listitem == count)	
			{
				count = i;
				break;
			}
			else count++;
		}
	}

	Inventory[playerid][slot][pInvEx] = Inventory[playerid][count][pInvItem];
	Inventory[playerid][slot][pInvQ] = Inventory[playerid][count][pInvQ];

	SendSyntaxMessage(playerid, "Você removeu com sucesso %s (%i) de %s.", inventory_items[Inventory[playerid][slot][pInvEx]][itemName], Inventory[playerid][slot][pInvQ], inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

	RemoveInvItem(playerid, Inventory[playerid][count][pInvItem], 0, count);
	return 1;
}


Dialog:InventoryPut(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new count,
		slot = GetPVarInt(playerid, #inventory_choose);

	for(new i = 0; i < MAX_INV_SLOTS; i++) 
	{
		if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
		{
			if (!Inventory[playerid][i][pInvEx])
			{
				if (listitem == count)	
				{
					count = i;
					break;
				}
				else count++;
			}
		}
	}

	Inventory[playerid][count][pInvEx] = Inventory[playerid][slot][pInvItem];
	Inventory[playerid][count][pInvQ] = Inventory[playerid][slot][pInvQ];

	SendSyntaxMessage(playerid, "Você removeu com sucesso %s (%i) de %s.", inventory_items[Inventory[playerid][count][pInvEx]][itemName], Inventory[playerid][count][pInvQ], inventory_items[Inventory[playerid][count][pInvItem]][itemName]);

	RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
	return 1;
}

Dialog:CombInv(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot1 = GetPVarInt(playerid, #inventory_choose),
		slot2 = listitem;

	if (Inventory[playerid][slot1][pInvItem] != Inventory[playerid][slot2][pInvItem])
	{
		SendErrorMessage(playerid, "Você só pode combinar os mesmos itens.");
		return cmd::inventory(playerid, "");
	}

	if (slot1 == slot2)
	{
		SendErrorMessage(playerid, "Você não pode combinar um item consigo mesmo.");
		return cmd::inventory(playerid, "");
	}
	
	switch(Inventory[playerid][slot1][pInvItem])
	{
		case INV_GANJA, INV_MED_GANJA, 103 .. 118:
		{
			if (Inventory[playerid][slot1][pInvQFloat] + Inventory[playerid][slot2][pInvQFloat] > 999.0) {
				SendErrorMessage(playerid, "O peso do número de ambos os itens, quando combinados, não deve exceder 999g.");
				return cmd::inventory(playerid, "");
			}

			Inventory[playerid][slot2][pInvQFloat] += Inventory[playerid][slot1][pInvQFloat];
			SendSyntaxMessage(playerid, "Você mesclou com sucesso %s (%.1fg).", inventory_items[Inventory[playerid][slot2][pInvItem]][itemName], Inventory[playerid][slot2][pInvQFloat]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot1][pInvItem], 0.0, slot1);
		}
		case INV_DRUG:
		{
			if (!CompareStrings(Inventory[playerid][slot2][pInvName], Inventory[playerid][slot2][pInvName])) {
				SendErrorMessage(playerid, "Você não pode combinar duas drogas diferentes. (Nomes diferentes!)");
				return cmd::inventory(playerid, "");
			}
			if (Inventory[playerid][slot1][pInvEx] != Inventory[playerid][slot2][pInvEx]) {
				SendErrorMessage(playerid, "Você não pode combinar duas drogas diferentes. (Pureza diferente!)");
				return cmd::inventory(playerid, "");
			}
			if (Inventory[playerid][slot1][pInvQFloat] + Inventory[playerid][slot2][pInvQFloat] > 999.0) {
				SendErrorMessage(playerid, "O peso do número de ambos os itens, quando combinados, não deve exceder 999g.");
				return cmd::inventory(playerid, "");
			}

			Inventory[playerid][slot2][pInvQFloat] += Inventory[playerid][slot1][pInvQFloat];
			SendSyntaxMessage(playerid, "Você mesclou com sucesso %s {C3C3C3}[OAS: %i%%]{FFFFFF} (%.1fg).", Inventory[playerid][slot2][pInvName], Inventory[playerid][slot2][pInvEx], Inventory[playerid][slot2][pInvQFloat]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot1][pInvItem], 0.0, slot1);
		}
		default: {
			if ((Inventory[playerid][slot1][pInvQ] + Inventory[playerid][slot2][pInvQ] > 100) && (inventory_items[Inventory[playerid][slot1][pInvQ]][itemType] == ITEM_TYPE_SEEDS || inventory_items[Inventory[playerid][slot1][pInvQ]][itemType] == ITEM_TYPE_DRUGS))
			{
				SendErrorMessage(playerid, "A soma do número de ambos os itens ao combiná-los não deve exceder 100.");
				return cmd::inventory(playerid, "");
			} 
			else if (Inventory[playerid][slot1][pInvQ] + Inventory[playerid][slot2][pInvQ] > 10)
			{
				SendErrorMessage(playerid, "A soma do número de ambos os itens ao combiná-los não deve exceder 10.");
				return cmd::inventory(playerid, "");
			}

			Inventory[playerid][slot2][pInvQ] += Inventory[playerid][slot1][pInvQ];
			SendSyntaxMessage(playerid, "Você mesclou com sucesso %s (%i).", inventory_items[Inventory[playerid][slot2][pInvItem]][itemName], Inventory[playerid][slot2][pInvQ]);

			RemoveInvItem(playerid, Inventory[playerid][slot1][pInvItem], 0, slot1);
		}
	}

	return 1;
}

Dialog:DivideInvInt(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot1 = GetPVarInt(playerid, #inventory_choose);
	
	if (CheckInv(playerid) == 0) 
		return SendErrorMessage(playerid, "Seu inventário está cheio.");
	if (strval(inputtext) <= 0 || strval(inputtext) >= Inventory[playerid][slot1][pInvQ]) 
		return SendErrorMessage(playerid, "Valor incorreto.");

	SendSyntaxMessage(playerid, "Você dividiu com sucesso {ffc46a}%s{ffffff} em %i e %i.", inventory_items[Inventory[playerid][slot1][pInvItem]][itemName], strval(inputtext), Inventory[playerid][slot1][pInvQ]-strval(inputtext));
	
	Inventory[playerid][slot1][pInvQ] -= strval(inputtext);

	new str[1] = EOS;
	GiveInvItem(playerid, Inventory[playerid][slot1][pInvItem], strval(inputtext), 0, 0, str, 1);
	return 1;
}

Dialog:DivideInvFloat(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot1 = GetPVarInt(playerid, #inventory_choose);
	
	if (CheckInv(playerid) == 0)	return SendErrorMessage(playerid, "Seu inventário está cheio.");

	if (floatstr(inputtext) < 0.1 || floatstr(inputtext) >= Inventory[playerid][slot1][pInvQFloat])  
		return SendErrorMessage(playerid, "Valor incorreto.");

	SendSyntaxMessage(playerid, "Você dividiu com sucesso {ffc46a}%s{ffffff} em %.1fg e %.1fg.", inventory_items[Inventory[playerid][slot1][pInvItem]][itemName], floatstr(inputtext), Inventory[playerid][slot1][pInvQFloat]-floatstr(inputtext));
	
	Inventory[playerid][slot1][pInvQFloat] -= floatstr(inputtext);
	
	new str[1] = EOS;
	GiveFloatInvItem(playerid, Inventory[playerid][slot1][pInvItem], floatstr(inputtext), 0, 0, str, 1);
	return 1;
}

Dialog:PutItemInProperty(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

    new 
		slot = GetPVarInt(playerid, #inventory_choose), 
		houseid = GetPVarInt(playerid, #inventory_house)-1, 
		vehicle = GetPVarInt(playerid, #inventory_vehicle),
		item = Inventory[playerid][slot][pInvItem]
	;

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || item == 0)	
		return cmd::inventory(playerid, "");
    if (strval(inputtext) < 1 || Inventory[playerid][slot][pInvQ] < strval(inputtext))	
		return SendErrorMessage(playerid, "Você não tem tantos. ");

	if (GetPVarInt(playerid, #inventory_vehicle) != 0)
	{
		if (vehicle > 0 && GetNearVehicle(playerid, 5.0, vehicle) != 0)
		{
			if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))	
				return SendErrorMessage(playerid, "Este veículo não possui um porta-malas.");
			if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])										
				return SendErrorMessage(playerid, "O porta-malas deste veículo não está disponível para você. ");
			if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0) 									
				return SendErrorMessage(playerid, "Você não pode colocar uma arma em seu veículo pessoal durante o expediente. ");
			if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) 
				return SendErrorMessage(playerid, "Você tem que estar perto do porta-malas. ");
			if (!IsPlayerInAnyVehicle(playerid) && !VehicleInfo[vehicle][carTrunkLock]) 																	
				return SendErrorMessage(playerid, "O porta-malas deste veículo está fechado. ");
			
			new i = GetNextVehicleTrunkSlot(vehicle);
			if (i == -1)	return SendErrorMessage(playerid, "Não há espaço no veículo.");

			TrunkInventory[vehicle][i][pInvItem] 	= Inventory[playerid][slot][pInvItem];
			TrunkInventory[vehicle][i][pInvQ] 		= Inventory[playerid][slot][pInvQ];
			TrunkInventory[vehicle][i][pInvEx] 		= Inventory[playerid][slot][pInvEx];
			TrunkInventory[vehicle][i][pInvS] 		= Inventory[playerid][slot][pInvS];
			TrunkInventory[vehicle][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
			format(TrunkInventory[vehicle][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
						
			Inventory[playerid][slot][pInvQ] -= strval(inputtext);
						
			SM2(playerid, "Você colocou %s em %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);

			new mes[128];
			format(mes, sizeof(mes), "coloca %s (%i) em %s %s.", inventory_items[item][itemName], strval(inputtext), (VehicleInfo[vehicle][carFaction])?("apertar"):("porta-malas"), VehicleNames[GetVehicleModel(vehicle)-400]);
			SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

			Save_CarInventory(vehicle);
			return cmd::inventory(playerid, "");
		}
	}

    else if (GetPVarInt(playerid, #inventory_vehicle) == 0 && IsPlayerInAnyVehicle(playerid))
	{
	    vehicle = GetPlayerVehicleID(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID] && vehicle == 0) return cmd::inventory(playerid, "");
			
		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0) 
			return SendErrorMessage(playerid, "Você não pode colocar uma arma em seu veículo pessoal durante o expediente.");

		new i = GetNextVehicleGBSlot(vehicle);
		if (i == -1)	return SendErrorMessage(playerid, "Não há espaço no veículo.");

		CarInventory[vehicle][i][pInvItem]		= Inventory[playerid][slot][pInvItem];
		CarInventory[vehicle][i][pInvQ]			= Inventory[playerid][slot][pInvQ];
		CarInventory[vehicle][i][pInvEx] 		= Inventory[playerid][slot][pInvEx];
		CarInventory[vehicle][i][pInvS]			= Inventory[playerid][slot][pInvS];
		CarInventory[vehicle][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
		format(CarInventory[vehicle][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
						
		Inventory[playerid][slot][pInvQ] -= strval(inputtext);
						
		SM2(playerid, "Você colocou %s (%i) no porta-luvas de %s.", inventory_items[item][itemName], strval(inputtext), VehicleNames[GetVehicleModel(vehicle)-400]);
		
		new mes[128];
		format(mes, sizeof(mes), "coloca o porta-luvas em %s %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		Save_CarInventory(vehicle);
		return cmd::inventory(playerid, "");
	}

	else if (GetPVarInt(playerid, #inventory_house) != 0)
	{
		new i = GetNextHouseTrunkSlot(houseid);

		if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
		{
			if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty]) 
				return SendErrorMessage(playerid, "Você não pode colocar uma arma em um esconderijo durante o expediente. ");

			if (i == -1)	return SendErrorMessage(playerid,"Não há espaço na casa.");
			SM2(playerid, "Você colocou %s no esconderijo secreto.", inventory_items[item][itemName]);

			HouseInventory[houseid][i][pInvItem] 	= Inventory[playerid][slot][pInvItem];
			HouseInventory[houseid][i][pInvQ]		= Inventory[playerid][slot][pInvQ];
			HouseInventory[houseid][i][pInvEx]		= Inventory[playerid][slot][pInvEx];
			HouseInventory[houseid][i][pInvS]		= Inventory[playerid][slot][pInvS];
			HouseInventory[houseid][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
			format(HouseInventory[houseid][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
						
			Inventory[playerid][slot][pInvQ] -= strval(inputtext);
						
			new mes[128];
			format(mes, sizeof(mes), "coloca %s (%i) no esconderijo secreto.", inventory_items[item][itemName], strval(inputtext));
			SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

			Save_HouseInventory(houseid);
			return cmd::inventory(playerid, "");
		}
	}

	else 
	{
		SendErrorMessage(playerid, "Ops .. Algo deu errado. Tente a operação novamente.");
		return cmd::inventory(playerid, "");
	}
	
	return 1;
}


Dialog:InventoryOther(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new houseid = -1;

	if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES && PlayerToPoint(1, playerid, HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
	{
        if (!HouseInventory[houseid][listitem][pInvItem]) return 1;

        new item = HouseInventory[houseid][listitem][pInvItem];
		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty])	
			return SendErrorMessage(playerid, "Você não pode pegar uma arma na casa durante o expediente. ");
		if (GivePlayerInvItem(playerid, item, HouseInventory[houseid][listitem][pInvQ], HouseInventory[houseid][listitem][pInvQFloat], HouseInventory[houseid][listitem][pInvEx], HouseInventory[houseid][listitem][pInvS], HouseInventory[houseid][listitem][pInvName]) == -1) 
			return SendErrorMessage(playerid, "Inventário está cheio.");

        SendSyntaxMessage(playerid, "Você pegou %s do esconderijo da casa.", inventory_items[item][itemName]);
		
		HouseInventory[houseid][listitem][pInvName] = EOS;
		HouseInventory[houseid][listitem][pInvQFloat] = 0.0;
        HouseInventory[houseid][listitem][pInvItem] = 
		HouseInventory[houseid][listitem][pInvQ] =
		HouseInventory[houseid][listitem][pInvEx] =
		HouseInventory[houseid][listitem][pInvS] = 0;

		Save_HouseInventory(houseid);

		new mes[128];
		format(mes, sizeof(mes), "pega %s do esconderijo da casa.", inventory_items[item][itemName]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
		return 1;
	}
	
	new vehicle = GetPlayerVehicleID(playerid);

	if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carOwnerID] == PlayerInfo[playerid][pID])
	{
	    if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])	
			return SendErrorMessage(playerid, "O porta-luvas deste veículo não está disponível para você.");
        if (!CarInventory[vehicle][listitem][pInvItem])																
			return 1;

	    new item = CarInventory[vehicle][listitem][pInvItem];

		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && PlayerInfo[playerid][pOnDuty]) 	
			return SendErrorMessage(playerid, "Você não pode pegar armas de veículos de facções diferentes durante o expediente.");
		if (GivePlayerInvItem(playerid, item, CarInventory[vehicle][listitem][pInvQ], CarInventory[vehicle][listitem][pInvQFloat], CarInventory[vehicle][listitem][pInvEx], CarInventory[vehicle][listitem][pInvS], CarInventory[vehicle][listitem][pInvName]) == -1) 
			return SendErrorMessage(playerid, "Inventário está cheio.");
		//if (IsValidDynamicObject(VehicleInfo[vehicle][carAttach][listitem])) DestroyDynamicObject(VehicleInfo[vehicle][carAttach][listitem]);

		CarInventory[vehicle][listitem][pInvName] = EOS;
		CarInventory[vehicle][listitem][pInvQFloat] = 0.0;
        CarInventory[vehicle][listitem][pInvItem] =
		CarInventory[vehicle][listitem][pInvS] =
		CarInventory[vehicle][listitem][pInvEx] =
		CarInventory[vehicle][listitem][pInvQ] = 0;
		
		Save_CarInventory(vehicle);

		new mes[128];
		format(mes, sizeof(mes), "tira o porta-luvas %s %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
	    return 1;
	}

	if ((vehicle = GetNearVehicle(playerid, 5.0)) != 0)
	{
		new item = TrunkInventory[vehicle][listitem][pInvItem];

	    if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))									return SendErrorMessage(playerid,"Este veículo não tem um porta-malas.");
		if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])																	return SendErrorMessage(playerid, "O porta-malas deste veículo não está disponível para você.");
		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0) 																return SendErrorMessage(playerid, "Você não pode colocar uma arma em seu veículo pessoal durante o expediente. ");
		if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) 								return SendErrorMessage(playerid, "Você precisa estar perto do porta-malas. ");
		if (!IsPlayerInAnyVehicle(playerid) && !VehicleInfo[vehicle][carTrunkLock]) 																									return SendErrorMessage(playerid, "O porta-malas deste veículo está trancado.");
        if (!TrunkInventory[vehicle][listitem][pInvItem])																																return cmd::inventory(playerid, "");
		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && PlayerInfo[playerid][pOnDuty]) 	return SendErrorMessage(playerid, "Você não pode pegar armas de veículos de facções diferentes durante o expediente. ");
		
		if (GivePlayerInvItem(playerid, item, TrunkInventory[vehicle][listitem][pInvQ], TrunkInventory[vehicle][listitem][pInvQFloat], TrunkInventory[vehicle][listitem][pInvEx], TrunkInventory[vehicle][listitem][pInvS], TrunkInventory[vehicle][listitem][pInvName]) == -1) 
			return SendErrorMessage(playerid, "Inventário está cheio.");
		if (IsValidDynamicObject(VehicleInfo[vehicle][carAttach][listitem])) 													
			DestroyDynamicObject(VehicleInfo[vehicle][carAttach][listitem]);

		TrunkInventory[vehicle][listitem][pInvName] = EOS;
		TrunkInventory[vehicle][listitem][pInvQFloat] = 0.0;
        TrunkInventory[vehicle][listitem][pInvItem] =
		TrunkInventory[vehicle][listitem][pInvS] =
		TrunkInventory[vehicle][listitem][pInvEx] =
		TrunkInventory[vehicle][listitem][pInvQ] = 0;

		Save_CarInventory(vehicle);

		new mes[128];
		format(mes, sizeof(mes), "retira %s de %s %s.", inventory_items[item][itemName], (VehicleInfo[vehicle][carFaction])?("fixações"):("porta-malas"), VehicleNames[GetVehicleModel(vehicle)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
	    return 1;
	}
	
	cmd::inventory(playerid, "");
	return 1;
}

Dialog:DeleteItem(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::inventory(playerid, "");

    new slot = GetPVarInt(playerid, #inventory_choose);

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || Inventory[playerid][slot][pInvItem] == 0)	return cmd::inventory(playerid, "");
	if (Inventory[playerid][slot][pInvItem] == 21 && maskOn{playerid}) cmd::mascara(playerid, ""); 

	SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "** (( %s removeu o item %s de seu inventário. ))", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

	RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
	return 1;
}

Dialog:GiveItem(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::inventory(playerid, "");

    new slot = GetPVarInt(playerid, #inventory_choose);

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || Inventory[playerid][slot][pInvItem] == 0)	return cmd::inventory(playerid, "");

	new userid;

	if (sscanf(inputtext, "u", userid))	return Dialog_Show(playerid, GiveItem, DIALOG_STYLE_INPUT, "{FFFFFF}Transferir item", "{ffffff}Digite o nome ou ID do jogador para quem você deseja transferir o item:", ">>>", "Retornar");

	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (playerid == userid)							return SendErrorMessage(playerid, "Você não pode transferir um item para si mesmo.");

	if (inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit] == 0 || Inventory[playerid][slot][pInvQFloat] == 0.1)
	{
		switch(Inventory[playerid][slot][pInvItem])
		{
			case 22..34: // ARMA
			{
			    if (GiveInvItem(userid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)
			        return SendErrorMessage(playerid, "Inventário está cheio.");

			    new str[128];
			    format(str, sizeof(str), "pega %s e passa para %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			    SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			    SM1(userid, "%s passou para você %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
			    SM1(playerid, "Você passou %s %s.", GetNameWithMask(userid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

			    RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			}
			case INV_GANJA, INV_MED_GANJA, 103 .. 118: // MACONHA e MEDICAMENTOS
			{
			    if (GiveFloatInvItem(userid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQFloat]) == -1)
			        return SendErrorMessage(playerid, "Inventário está cheio!");

			    new str[128];
			    format(str, sizeof(str), "pega %s e passa para %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			    SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			    SM1(userid, "%s passou para você %s (%.1fg).", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQFloat]);
			    SM1(playerid, "Você passou %s %s (%.1fg).", GetNameWithMask(userid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQFloat]);

			    RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			}
			case INV_DRUG: // DROGA
			{
			    if (GiveDrugInvItem(userid, Inventory[playerid][slot][pInvName], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)
			        return SendErrorMessage(playerid, "Inventário está cheio!");

			    new str[128];
			    format(str, sizeof(str), "pega %s e passa para %s.", Inventory[playerid][slot][pInvName], GetNameWithMask(userid));
			    SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			    SM1(userid, "%s passou para você %s (%.1fg).", GetNameWithMask(playerid), Inventory[playerid][slot][pInvName], Inventory[playerid][slot][pInvQFloat]);
			    SM1(playerid, "Você passou %s %s (%.1fg).", GetNameWithMask(userid), Inventory[playerid][slot][pInvName], Inventory[playerid][slot][pInvQFloat]);

			    RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			}
			default:
			{
			    if (GiveInvItem(userid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ]) == -1)
			        return SendErrorMessage(playerid, "Inventário está cheio!");
			    RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);

			    new str[128];
			    format(str, sizeof(str), "pega %s e passa para %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			    SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			    SM1(userid, "%s passou para você %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
			    SM1(playerid, "Você passou %s %s.", GetNameWithMask(userid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
			}
		}
	
	    return 1;
	}

	SetPVarInt(playerid, #give_item, userid);

	new mes[256];
	format(mes, sizeof(mes), "{ffffff}Item: %s (quantidade: %i)\n\nDigite a quantidade que você deseja transferir para o jogador %s:", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));
	Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Liberar item", mes, "Liberar", "Cancelar");
	return 1;
}

Dialog:GiveQuantity(playerid, response, listitem, inputtext[])
{
	if (!response)									return cmd::inventory(playerid, "");

	new slot = GetPVarInt(playerid, #inventory_choose),	userid = GetPVarInt(playerid, #give_item);
	
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
    if (playerid == userid)							return SendErrorMessage(playerid, "Você não pode transferir itens para si mesmo.");

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || Inventory[playerid][slot][pInvItem] == 0)	return cmd::inventory(playerid, "");

	switch(Inventory[playerid][slot][pInvItem])
	{
		case 22..34: // ARMA
		{
			return 1;
		}
		case INV_GANJA, INV_MED_GANJA, 103 .. 118: // Maconha e medicamentos
		{
			format(inputtext, 5, "%.1f", floatstr(inputtext));
			new Float:amount = floatstr(inputtext);
	
			if (amount < 0.1 || Inventory[playerid][slot][pInvQFloat] < amount)
			{
				SendErrorMessage(playerid, "Você não tem essa quantidade.");

				new mes[256];
				format(mes, sizeof(mes), "{ffffff}Item: %s (quantidade: %i)\n\nDigite a quantidade que você deseja transferir para o jogador %s:", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));
				Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Liberar item", mes, "Liberar", "Cancelar");
				return 1;
			}

			if (GiveFloatInvItem(userid, Inventory[playerid][slot][pInvItem], amount) == -1)	return SendErrorMessage(playerid, "Inventário está cheio!");

			new str[128];
			format(str, sizeof(str), "pega %s e passa para %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			SM1(userid, "%s passou para você %s (%.1fg).", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], amount);
			SM1(playerid, "Você passou %s %s (%.1fg).", GetNameWithMask(userid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], amount);
			
			new query[256];
			format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] transferiu %s (quantidade: %i) para o jogador %s [MYSQL: %i | IP: %s]",
				GetNameEx(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], amount, GetNameEx(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid));
			insert_log(query, ITEM_LOG, 3, PlayerInfo[playerid][pAccID], PlayerInfo[userid][pAccID]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], amount, slot);
		}
		case INV_DRUG: // Drogas
		{
			format(inputtext, 5, "%.1f", floatstr(inputtext));
			new Float:amount = floatstr(inputtext);

			if (amount < 0.1 || Inventory[playerid][slot][pInvQFloat] < amount)
			{
				SendErrorMessage(playerid, "Você não tem essa quantidade.");

				new mes[256];
				format(mes, sizeof(mes), "{ffffff}Item: %s (quantidade: %i)\n\nDigite a quantidade que você deseja transferir para o jogador %s:", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));
				Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Liberar item", mes, "Liberar", "Cancelar");
				return 1;
			}
			
			if (GiveDrugInvItem(userid, Inventory[playerid][slot][pInvName], amount, Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)	return SendErrorMessage(playerid, "Inventário está cheio!");

			new str[128];
			format(str, sizeof(str), "pega %s e passa para %s.", Inventory[playerid][slot][pInvName], GetNameWithMask(userid));
			SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			SM1(userid, "%s passou para você %s (%.1fg).", GetNameWithMask(playerid), Inventory[playerid][slot][pInvName], amount);
			SM1(playerid, "Você passou %s %s (%.1fg).", GetNameWithMask(userid), Inventory[playerid][slot][pInvName], amount);
			
			new query[256];
			format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] transferiu %s [droga] (quantidade: %i / OAS: %i%%) para o jogador %s [MYSQL: %i | IP: %s]",
				GetNameEx(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), Inventory[playerid][slot][pInvName], amount, Inventory[playerid][slot][pInvEx], GetNameEx(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid));
			insert_log(query, ITEM_LOG, 3, PlayerInfo[playerid][pAccID], PlayerInfo[userid][pAccID]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], amount, slot);
		}
		default: // Outros itens
		{
			if (strval(inputtext) < 1 || Inventory[playerid][slot][pInvQ] < strval(inputtext))
			{
				SendErrorMessage(playerid, "Você não tem essa quantidade.");

				new mes[256];
				format(mes, sizeof(mes), "{ffffff}Item: %s (quantidade: %i)\n\nDigite a quantidade que você deseja transferir para o jogador %s:", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));
				Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Liberar item", mes, "Liberar", "Cancelar");
				return 1;
			}
		
			if (GiveInvItem(userid, Inventory[playerid][slot][pInvItem], strval(inputtext)) == -1)	return SendErrorMessage(playerid, "Inventário está cheio!");
			new str[128];
			format(str, sizeof(str), "pega %s e passa para %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			SM1(userid, "%s passou para você %s [quantidade: %i].", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], strval(inputtext));
			SM1(playerid, "Você passou %s %s [quantidade: %i].", GetNameWithMask(userid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], strval(inputtext));
			
			new query[128];
			format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] transferiu %s (quantidade: %i) para o jogador %s [MYSQL: %i | IP: %s]",
				GetNameEx(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], strval(inputtext), GetNameEx(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid));
			insert_log(query, ITEM_LOG, 3, PlayerInfo[playerid][pAccID], PlayerInfo[userid][pAccID]);

			RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], strval(inputtext), slot);
		}
	}

	return 1;
}

Dialog:InventoryMenu(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;
	
	switch(listitem)
	{
		case 0: return PrintInv(playerid);
		case 1:
		{
			new houseid;
			if ((houseid = GetNearHouse(playerid)) != -1 && HouseInfo[houseid][hOwnerID] == PlayerInfo[playerid][pID] && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
			{
				return PrintHouseInv(playerid, houseid);
			}

			new vehicle = GetNearVehicle(playerid, 4.0);
			if (vehicle != 0 && (!IsADontTrunk(vehicle) || !IsAPlane(vehicle) || !IsABiker(vehicle) || !IsAHelicopter(vehicle) || !IsABoat(vehicle) || !IsABicycle(vehicle)))
			{
				if ((VehicleInfo[vehicle][carTrunkLock] && !IsPlayerInAnyVehicle(playerid)) || (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction]))
				{
					return PrintVehInv(playerid, vehicle);
				}
			}
			
			vehicle = GetPlayerVehicleID(playerid);
			if (IsPlayerInAnyVehicle(playerid) && vehicle != 0 && VehicleInfo[vehicle][carOwnerID] == PlayerInfo[playerid][pID])
			{
				return PrintGBInv(playerid, vehicle);
			}
		}
	}

	SendErrorMessage(playerid, "Oops... Algo deu errado. Tente a operação novamente.");
	return 1;
}