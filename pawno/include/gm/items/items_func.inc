this::OnPlayerUseItem(playerid, slotid) 
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) 
        return SendErrorMessage(playerid, "Você não pode usar o inventário agora.");
    if (slotid < 0 || slotid > MAX_INV_SLOTS-1) 
        return 1;

    new itemid = Inventory[playerid][slotid][pInvItem];
	
    switch(itemid)
    {
        case INV_SIGN: cmd::carsign(playerid, "");
        case INV_HEAL:
        {
            if (!PlayerInfo[playerid][pInjured]) return SendErrorMessage(playerid, "Indisponível na fase de ferimento/morte.");
            if (GetPVarInt(playerid, #heal_time) > gettime()) return SendErrorMessage(playerid, "Pode ser usado uma vez por minuto.");

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health >= 100.0) return SendErrorMessage(playerid, "Você está com saúde completa.");

            if (health + 15.0 > 100.0) SetPlayerHealth(playerid, 100.0);
            else SetPlayerHealth(playerid, health + 100.0);

            SetPVarInt(playerid, #heal_time, gettime() + 60);
            Inventory[playerid][slotid][pInvQ]--;
            if (!Inventory[playerid][slotid][pInvQ]) Inventory[playerid][slotid][pInvItem] = 0;
        }

        case INV_GANJA, INV_MED_GANJA, INV_DRUG: {
            if (pTemp[playerid][pDrugTime]) return SendErrorMessage(playerid, "Você usou drogas recentemente.");
            UseDrug(playerid, itemid, slotid);
        }


        case INV_BOOMBOX:   PlaceBoombox(playerid);
        case INV_RADIO:
        {
            new mes[550] = "/r - fale no rádio.\n";
            strcat(mes, "/rr - fale no rádio no slot.\n");
            strcat(mes, "/(rb)aixo - fale no rádio em meio sussurro.\n");
            strcat(mes, "/(rrb)aixo - fale no rádio em meio sussurro no slot.\n");
            strcat(mes, "/auth - faça login no canal de ondas de rádio.\n");
            strcat(mes, "/(ca)anal frequencia - mude o canal de rádio.\n");
            strcat(mes, "/(ca)anal alugar - mude o canal de rádio.\n");
            strcat(mes, "/setslot - mude o slot do rádio.\n");
            strcat(mes, "/(ca)anal sair - saia do canal de ondas de rádio.\n");
            strcat(mes, "/(ca)anal vender - venda o canal de ondas de rádio. (50% será devolvido)\n");
            strcat(mes, "/newauth - mude a senha do canal.\n");
            strcat(mes, "/kickoffradio - expulse do canal.\n");
            strcat(mes, "/radiomembros - membros do canal de ondas de rádio.");
            Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Ajuda do Rádio", mes, "X", "");
        }

        case INV_MASK:      cmd::mascara(playerid, "");
        case INV_LIGHTER:   SendHelpMessage(playerid, "Use o cigarro em seu inventário para acender um.");
        case INV_CIGAR:
        {
            if (!IsPlayerHasItem(playerid, INV_LIGHTER))                 return SendErrorMessage(playerid, "Você não tem um isqueiro.");
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))  return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");
            
            Inventory[playerid][slotid][pInvQ]--;
            
            if (Inventory[playerid][slotid][pInvQ] == 0) Inventory[playerid][slotid][pInvItem] = 0;
            
            cmd::server_ame(playerid, "pega um palito de cigarro da carteira acende usando um isqueiro.");
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);
            SM2(playerid, "Você acendeu um cigarro.");

            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.", 5);
        }
        case INV_DICE:
            SendNearbyMessage(Range_Normal, playerid, COLOR_WHITE, "* %s rola os dados e obtém %i.", GetNameWithMask(playerid), random(6)+1);

        case INV_PEN:       cmd::nota(playerid, "");
        case INV_SILENCER:  cmd::silenciador(playerid, "");
        case INV_OPTICS:    cmd::optica(playerid, "");
        case INV_GASCAN:
        {
            if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você deve estar fora do veículo.");

            new vehicle;

            if ((vehicle = GetNearVehicle(playerid, 2.5)) == 0) return SendErrorMessage(playerid, "Não há veículos próximos a você.");
            if (VehicleInfo[vehicle][carFuel] + 20.0 > GetVehicleFuelCapacity(GetVehicleModel(vehicle))) return SendErrorMessage(playerid, "O tanque do veículo está cheio.");

            VehicleInfo[vehicle][carFuel] += 20.0;
            SM2(playerid, "Seu veículo foi abastecido com um galão.");
            cmd::me(playerid, "reabastece o veículo com um galão.");
            Inventory[playerid][slotid][pInvQ]--;
            if (Inventory[playerid][slotid][pInvQ] == 0) Inventory[playerid][slotid][pInvItem] = 0;
        }
        case INV_BURGER:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");
            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 10 > 100.0) SetPlayerHealth(playerid, 100.0);
            else                    SetPlayerHealth(playerid, health + 10.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());
            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);
            RemovePlayerItem(playerid, INV_BURGER, 1);
            cmd::server_ame(playerid, "comendo um hambúrguer.");
        }
        case INV_PIZZA:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");
            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 15.0 > 100.0) SetPlayerHealth(playerid, 100.0);
            else                       SetPlayerHealth(playerid, health + 15.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());
            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);
            RemovePlayerItem(playerid, INV_PIZZA, 1);
            cmd::server_ame(playerid, "comendo pizza.");
        }
        case INV_CHICKEN:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 7.0 > 100.0) SetPlayerHealth(playerid, 100.0);
            else                      SetPlayerHealth(playerid, health + 7.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());
            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);
            RemovePlayerItem(playerid, INV_CHICKEN, 1);
            cmd::server_ame(playerid, "comendo frango.");
        }
        case INV_HOTDOG:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 5.0 > 100.0) SetPlayerHealth(playerid, 100.0);
            else                      SetPlayerHealth(playerid, health + 5.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());
            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);
            RemovePlayerItem(playerid, INV_HOTDOG, 1);
            cmd::server_ame(playerid, "comendo cachorro-quente.");
        }
        case INV_WATER, INV_SODA, INV_COLA:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.",5);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
            RemovePlayerItem(playerid, itemid, 1);

            if (itemid == INV_WATER)     cmd::server_ame(playerid, "bebe água.");
            else if (itemid == INV_SODA) cmd::server_ame(playerid, "bebe refrigerante.");
            else if (itemid == INV_COLA) cmd::server_ame(playerid, "bebe coca-cola.");
        }
        case INV_BEER:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.", 5);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
            RemovePlayerItem(playerid, itemid, 1);
            cmd::server_ame(playerid, "bebendo cerveja.");
        }
        case INV_VODKA, INV_WHISKEY, INV_BRANDY, INV_WINE:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

            InfoMSG(playerid, "Pressione \" ~g~LKM ~w~\" para ~y~usar ~w~o objeto~n~Pressione \" ~r~ENTER ~w~\" para ~y~remover ~w~o objeto.", 5);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
            RemovePlayerItem(playerid, itemid, 1);

            if (itemid == INV_VODKA)         cmd::server_ame(playerid, "bebe vodka.");
            else if (itemid == INV_WHISKEY)  cmd::server_ame(playerid, "bebe uísque.");
            else if (itemid == INV_BRANDY)   cmd::server_ame(playerid, "bebe conhaque.");
            else                             cmd::server_ame(playerid, "bebe vinho.");
        }
        case INV_PICKLOCK: cmd::hbreaklock(playerid, "");
        case INV_CARSOUND:
        {
            new vehicle = GetPlayerVehicleID(playerid);

            if (vehicle == 0) return SendSyntaxMessage(playerid, "Você pode instalar um rádio tocador de fitas em seu veículo ou vendê-lo em uma loja de penhores (( /vender ))");
            if (VehicleInfo[vehicle][carOwnerID] == 0)    return SendErrorMessage(playerid, "O rádio tocador de fitas só pode ser instalado em veículos pessoais.");
            if (VehicleInfo[vehicle][carRadio] == 1)      return SendErrorMessage(playerid, "Já existe um rádio tocador de fitas instalado neste veículo.");
            if (!IsPlayerHasItem(playerid, INV_CARSOUND)) return SendErrorMessage(playerid, "Você não possui um rádio tocador de fitas.");

            new mes[128];
            format(mes, sizeof(mes), "insere o rádio em %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
            cmd::me(playerid, mes);
            RemovePlayerItem(playerid, itemid, 1);
            VehicleInfo[vehicle][carRadio] = 1;
        }
        case INV_SKATE: cmd::skate(playerid, "");

        /*case 1 .. 18, 22 .. 34, 41 .. 43, 46: // WEAPONS
        {
            if (PlayerInfo[playerid][pPlayerWeapon] != 0)                        cmd::putgun(playerid, "");
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	         return SendWarningMessage(playerid, "Your hands are already taken.");
            if (PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed]) return SendErrorMessage(playerid, "You cannot use this feature right now.");

            PlayerInfo[playerid][pPlayerWeapon] = Inventory[playerid][slotid][pInvItem];
            PlayerInfo[playerid][pPlayerAmmo]   = Inventory[playerid][slotid][pInvQ];
            PlayerInfo[playerid][pAmmoType]     = Inventory[playerid][slotid][pInvEx];
            PlayerInfo[playerid][pWepSerial]    = Inventory[playerid][slotid][pInvS];

            GivePlayerWeaponEx(playerid, Inventory[playerid][slotid][pInvItem], Inventory[playerid][slotid][pInvQ], Inventory[playerid][slotid][pInvEx], Inventory[playerid][slotid][pInvS]);
            
            SM2(playerid, "You got %s from inventory.", inventory_items[Inventory[playerid][slotid][pInvItem]][itemName]);

            new mes[64];
            if (PlayerInfo[playerid][pPlayerWeapon] >= 22 && PlayerInfo[playerid][pPlayerWeapon] <= 34)  format(mes, sizeof(mes), "* %s pulls out a weapon.", GetNameEx(playerid), inventory_items[Inventory[playerid][slotid][pInvItem]][itemName]);
            else                                                                                        format(mes, sizeof(mes), "* %s gets out %s.", GetNameEx(playerid), inventory_items[Inventory[playerid][slotid][pInvItem]][itemName]);
            SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
            
            RemoveInvItem(playerid, Inventory[playerid][slotid][pInvItem], 0, slotid);
        }
        case 82 .. 99:
        {
            if (PlayerInfo[playerid][pPlayerWeapon] != 0 && PlayerInfo[playerid][pPlayerAmmo] != 0)
            {
                SendErrorMessage(playerid, "Your weapon is already loaded.");
                return PrintInv(playerid);
            }
            if (PlayerInfo[playerid][pPlayerWeapon] == 0)
            {
                SendErrorMessage(playerid, "You have no weapon in your hands ");
                return PrintInv(playerid);
            }
            if (CompatAmmo(PlayerInfo[playerid][pPlayerWeapon], itemid) == 0)
            {
                SendErrorMessage(playerid, "These cartridges do not fit your weapon ");
                return PrintInv(playerid);
            }

            PlayerInfo[playerid][pAmmoType] = itemid;
            PlayerInfo[playerid][pPlayerAmmo] = Inventory[playerid][slotid][pInvQ];

            GivePlayerWeaponEx(playerid, PlayerInfo[playerid][pPlayerWeapon], PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);
            
            SM1(playerid, "You reloaded %s - %s (%i round(s)).", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);

            new mes[64];
            format(mes, sizeof(mes), "* %s recharges %s", GetNameEx(playerid), inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName]);
            SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 30.0, 10000);
            
            RemoveInvItem(playerid, itemid, 0, slotid);
            
            ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);
        }*/
	}
	return 1;
}

IsPlayerHasItem(playerid, itemid, amount = 0)
{
	if (!IsPlayerLogged(playerid) || !itemid) return 0;
    
    new result = -1;
	for(new i = 0; i != MAX_INV_SLOTS; i++)
	{
	    if (amount > 0 && Inventory[playerid][i][pInvItem] == itemid && Inventory[playerid][i][pInvQ] >= amount) 
        { 
            result = i;
            break; 
        }  
	    else if (amount < 1 && Inventory[playerid][i][pInvItem] == itemid) 
        {
            result = i;
            break; 
        }
	}

    if (result == -1) return 0;
    else return 1;
}

RemovePlayerItem(playerid, itemid, amount = -1)
{
    for(new i; i != MAX_INV_SLOTS; i++)
	{
	    if (Inventory[playerid][i][pInvItem] != itemid) continue;

	    if (amount == -1)
	    {
	        Inventory[playerid][i][pInvItem] = 0;
	        Inventory[playerid][i][pInvQ] = 0;
			return 1;
		}

	    Inventory[playerid][i][pInvQ] -= amount;

		if (Inventory[playerid][i][pInvQ] < 1)
		{
		    Inventory[playerid][i][pInvItem] = 0;
	        Inventory[playerid][i][pInvQ] = 0;
  		}
		return 1;
	}
	return 0;
}

this::LoadHolsters(playerid)
{
	/*if (GetPVarInt(playerid, "Backpack") == 1) 
    {
		if (IsPlayerAttachedObjectSlotUsed(playerid, 7)) RemovePlayerAttachedObject(playerid, 6);
		SetPlayerAttachedObject(playerid, 6, 371, 1, 0.0, -0.14, 0.0, 0.0, 90, 359.5022, 1, 1, 1);
		return 1;
	}*/

    new count;
    for(new w = 0; w < MAX_WEAP-1; w++)
    {
        count = -1;
        for(new p = count+1; p < MAX_INV_SLOTS-1; p++)
        {
            if (inventory_items[Inventory[playerid][p][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
            {
                if (GetWeaponType(Inventory[playerid][p][pInvItem]) == w)
                {
                    count = p;
                    break;
                }
            }
        }
        if (count != -1)
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, w + 5))
                RemovePlayerAttachedObject(playerid, w + 5);

            if (0 == PlayerInfo[playerid][pHideWeap][w] && !IsPlayerAttachedObjectSlotUsed(playerid, w + 5))
            {
                SetPlayerAttachedObject(playerid, w + 5, 
                    inventory_items[Inventory[playerid][count][pInvItem]][itemModel], 
                    PlayerInfo[playerid][pWeaponBone][w], 
                    PlayerInfo[playerid][pWeaponX][w], 
                    PlayerInfo[playerid][pWeaponY][w], 
                    PlayerInfo[playerid][pWeaponZ][w], 
                    PlayerInfo[playerid][pWeaponRX][w], 
                    PlayerInfo[playerid][pWeaponRY][w], 
                    PlayerInfo[playerid][pWeaponRZ][w], 
                    1.0, 1.0, 1.0);
                //SM1(playerid, "[slot: %i]: %s [item id: %i]", w, inventory_items[PlayerInfo[playerid][pInvItem][count]][itemName], count+1);
            }
        }
        else 
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, w + 5)) RemovePlayerAttachedObject(playerid, w + 5);
        }
    }

	return 1;
}

GetWeaponType(weaponid)
{
    switch(weaponid)
    {
        case 2,3,5,6,7,8,15:    return 0; //WEAPON_TYPE_HAND
        case 22,23,24,28,29,32: return 1; //WEAPON_TYPE_SEC
        case 25,26,30,31,33,34: return 2; //WEAPON_TYPE_PRIM
    }
    return -1;
}
    
