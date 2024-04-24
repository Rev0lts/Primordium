Dialog:DutyMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	    
    new faction = PlayerInfo[playerid][pFaction];
	if (!IsFactionLegal(faction)) return 1;
        
	switch(listitem)
	{
	    case 0://duty
	    {
			if (CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendErrorMessage(playerid, "Primeiro, você precisa sair do modo administrador.");
			if (pTemp[playerid][pTaser]) return SendErrorMessage(playerid, "Primeiro, você precisa remover o taser.");
			if (pTemp[playerid][pPatrol]) return SendErrorMessage(playerid, "Primeiro, você precisa sair do patrulhamento (/callsign).");
			if (!IsPlayerAtUniform(playerid)) return SendErrorMessage(playerid, "Você não está no vestiário.");
    	
	        if (!PlayerInfo[playerid][pOnDuty])
			{
				if (PlayerInfo[playerid][pPlayerWeapon]) 
				{
					if (CheckInv(playerid))	cmd::putgun(playerid, "");
					else return SendErrorMessage(playerid, "Primeiro coloque a arma no inventário. ");

				}
				
				SetPVarInt(playerid, #pDelay, 1);

				new count;
			    for(new i; i < MAX_INV_SLOTS; i++)
				{
					if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] != ITEM_TYPE_WEAPON && 
						inventory_items[Inventory[playerid][i][pInvItem]][itemType] != ITEM_TYPE_AMMO) continue;

					if (count >= MAX_COP_SLOTS)
					{
						for(new j; j < MAX_INV_SLOTS; j++)
						{
							PlayerInfo[playerid][pCopItem][j] = 0;
							PlayerInfo[playerid][pCopQ][j] = 0;
							PlayerInfo[playerid][pCopEx][j] = 0;
							PlayerInfo[playerid][pCopS][j] = 0;
						}

						SetPVarInt(playerid, #pDelay, 0);
						SendAdmMessage("%s tem um grande número de armas em seu inventário. [AVISO]", GetNameEx(playerid));
						return SendErrorMessage(playerid, "Você tem muitas armas / munições! (Você só pode colocar na caixa: 5 itens)");
					}

				    PlayerInfo[playerid][pCopItem][count] = Inventory[playerid][i][pInvItem];
				    PlayerInfo[playerid][pCopQ][count] = Inventory[playerid][i][pInvQ];
					PlayerInfo[playerid][pCopEx][count] = Inventory[playerid][i][pInvEx];
					PlayerInfo[playerid][pCopS][count] = Inventory[playerid][i][pInvItem];

					count++;
				}

				ResetPlayerWeaponsEx(playerid);

				if (IsFactionPolice(faction))
				{
					SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "%s %s pegou um distintivo do seu armário.", GetRankName(playerid), GetNameWithMask(playerid));
				}

				PlayerInfo[playerid][pOnDuty] = 1;
				pTemp[playerid][tempStatus] = 1;
		 		SetPlayerColor(playerid, RemoveAlpha(Factions[faction][fColor]));
		        SendFactionMessage(faction, IsFactionPolice(faction) ? COLOR_POLICECHAT : COLOR_LIGHTBLUE, "** HQ: %s %s entrou em serviço. **", GetRankName(playerid), GetNameEx(playerid));
                GameTextForPlayer(playerid, "~g~EM SERVICO", 5000, 3);
				
				cmd::duty(playerid, "");
				Save_User(playerid);

				SetPVarInt(playerid, #pDelay, 0);
				return 1;
			}

			ResetPlayerWeaponsEx(playerid);
			
			if (IsPlayerAttachedObjectSlotUsed(playerid, 5))	RemovePlayerAttachedObject(playerid, 5);
			if (IsPlayerAttachedObjectSlotUsed(playerid, 6))	RemovePlayerAttachedObject(playerid, 6);
    		if (IsPlayerAttachedObjectSlotUsed(playerid, 7))	RemovePlayerAttachedObject(playerid, 7);

			new copsitems = 0, playersitems = 0;
			for(new i = 0; i < MAX_INV_SLOTS; i++)
			{
				if (i < MAX_COP_SLOTS) if (PlayerInfo[playerid][pCopItem][i]) copsitems++;
				if (!Inventory[playerid][i][pInvItem]) playersitems++;
			}

			if (playersitems - copsitems >= 0)
			{
				for(new i = 0; i < MAX_COP_SLOTS; i++)
				{					
					if (PlayerInfo[playerid][pCopItem][i])
					{
						new slotid = GetInvNextSlot(playerid);
						if (slotid == -1)	continue;

						Inventory[playerid][slotid][pInvItem] = PlayerInfo[playerid][pCopItem][i];
						Inventory[playerid][slotid][pInvQ] = PlayerInfo[playerid][pCopQ][i];
						Inventory[playerid][slotid][pInvEx] = PlayerInfo[playerid][pCopEx][i];
						Inventory[playerid][slotid][pInvS] = PlayerInfo[playerid][pCopS][i];
					}
					
					PlayerInfo[playerid][pCopItem][i] = 0;
					PlayerInfo[playerid][pCopQ][i] = 0;
					PlayerInfo[playerid][pCopEx][i] = 0;
					PlayerInfo[playerid][pCopS][i] = 0;				
				}
			}
			else return SendErrorMessage(playerid, "Você não pode pegar itens da sua caixa. Seu inventário está cheio.");

			if (IsFactionPolice(faction))
			{
				SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "%s %s colocou um distintivo no seu armário.", GetRankName(playerid), GetNameWithMask(playerid));
			}

			SendFactionMessage(faction, IsFactionPolice(faction) ? COLOR_POLICECHAT : COLOR_LIGHTBLUE, "** HQ: %s %s saiu de serviço. **", GetRankName(playerid), GetNameEx(playerid));
			GameTextForPlayer(playerid, "~r~FORA DE SERVICO", 5000, 3);
			
			DestroyAllBort(playerid);
		    PlayerInfo[playerid][pChar] = 0;
			PlayerInfo[playerid][pSwatDuty] = 0;
			PlayerInfo[playerid][pOnDuty] = 0;
			pTemp[playerid][tempStatus] = 0;
			foreach(new p : Player) { if(pTemp[p][pSpecPlayer] == playerid) TogglePlayerSpectating(p, 0); }

			CallRemoteFunction("LoadHolsters","i",playerid);
			SetPlayerColor(playerid, TEAM_HIT_COLOR);
			SetPlayerArmour(playerid, 0);
			SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
			cmd::duty(playerid, "");

			return Save_User(playerid);
	    }
	    case 1:
		{
		    if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid,"Primeiro você precisa entrar em serviço.");
            if (!IsPlayerAtUniform(playerid))		return SendErrorMessage(playerid, "Você não está no vestiário.");
            if (PlayerInfo[playerid][pSwatDuty])	return SendErrorMessage(playerid, "Deixe o equipamento S.W.A.T. antes de mudar seu uniforme.");
                
			if (IsFactionPolice(faction))
			{
				new submes[16],
					mes[sizeof(police_skins)*sizeof(submes)];
				
				for(new i = 0; i < sizeof(police_skins); i++)
				{
					format(submes, sizeof(submes), "%i\tID: %i\n", police_skins[i], i);
					strcat(mes, submes);
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Seleção de farda de serviço", mes, "Selecionar", "Fechar");		
			}
			else if (IsFactionMedic(faction) || IsFactionFire(faction))
		    {
				new submes[16], mes[sizeof(fd_skins)*sizeof(submes)];

				for(new i = 0; i < sizeof(fd_skins); i++)
				{
					format(submes, sizeof(submes), "%i\tID: %i\n", fd_skins[i], i);
					strcat(mes, submes);
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Seleção de farda de serviço", mes, "Selecionar", "Fechar");
			}
			else if (IsFactionDocs(faction))
		    {
				new mes[sizeof(docs_skins)*128];
				for(new i = 0; i < sizeof(docs_skins); i++)
				{
					format(mes, sizeof(mes), "%s%i\tID: %i\n", mes, docs_skins[i], docs_skins[i]); 
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Seleção de farda de serviço", mes, "Selecionar", "Fechar");
			}
			else if (IsFactionCity(faction))
			{
				new submes[128], mes[sizeof(city_skins)*sizeof(submes)];

				for(new i = 0; i < sizeof(city_skins); i++)
				{
					format(submes, sizeof(submes), "%i\tID: %i\n", city_skins[i], i);
					strcat(mes, submes);
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Seleção de farda de serviço", mes, "Selecionar", "Fechar");
			}

			else SendErrorMessage(playerid, "Esta função não está disponível para a sua facção.");
		}
		case 2:
	    {
	        if (!PlayerInfo[playerid][pOnDuty])										return SendErrorMessage(playerid, "Primeiro você precisa entrar em serviço.");
            if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))	return SendErrorMessage(playerid, "Você não está no vestiário.");
    			
			if (IsFactionPolice(faction))
			{
				return Dialog_Show(playerid, LSPDArmoury, DIALOG_STYLE_LIST, "Equipamento & Arma", " Walkie-talkie\n Arma\n Equipamento S.W.A.T", "Selecionar", "Retornar");
			}
			else if (IsFactionDocs(faction))
			{	
				return Dialog_Show(playerid, DOCSGUN, DIALOG_STYLE_LIST, "Arma", " Walkie-talkie\n Cacetete\n Spray de Pimenta\n Desert Eagle (.50AE .ext)\n\t{ffc46a} - Pegar magazines extras (.50AE .ext)\n{FFFFFF}  MP5 (9x19mm .ext)\n\t{ffc46a} - Pegar magazines extras (9x19mm .ext)\n{FFFFFF}  Shotgun (12-G BIRDSHOT)\n\t{ffc46a} - Pegar magazines extras (12-G BIRDSHOT)\n\t{ffc46a} - Pegar magazines não letais (12-G BEANBAG)\n{FFFFFF}  4 (5.56x45mm (drum))\n\t{ffc46a} - Pegar magazines extras (5.56x45mm (drum))\n\t{ffc46a} - Pegar magazines não letais (5.56x45mm (borracha))\n{FFFFFF}  Sniper Rifle (7.62x51mm)\n\t{ffc46a} - Pegar magazines extras (7.62x51mm)\n{FFFFFF}  Granada de Gás\n Câmera\n{FF6347}  Entregar todas as armas", "Selecionar", "Fechar");
			}
			else if (IsFactionCity(faction))
			{
			    return Dialog_Show(playerid, CITYGUN, DIALOG_STYLE_LIST, "Arma", " Walkie-talkie\n Desert Eagle ( .50AE .ext )\n\t{ffc46a} - Pegar magazines extras (.50AE .ext)\n{FFFFFF}  MP5 (9x19mm .ext)\n\t{ffc46a} - Pegar magazines extras (9x19mm .ext)\n{FFFFFF}  4 (5.56x45mm (drum))\n\t{ffc46a} - Pegar munição extra (5.56x45mm (drum))\n\t{ffc46a} - Pegar munição não letal (5.56x45mm (borracha))\n{FF6347}  Entregar todas as armas", "Selecionar", "Fechar");
			}
			else if (IsFactionFire(faction) || IsFactionMedic(faction))
			{
		        return Dialog_Show(playerid, FDArmoury, DIALOG_STYLE_LIST, "Equipamento", " Walkie-talkie\n Motosserra\n Extintor\n{FF6347} Entregar todo o equipamento", "Selecionar", "Retornar"); 
			}
	    }
	    case 3:
	    {
	        if (!PlayerInfo[playerid][pOnDuty])										return SendErrorMessage(playerid, "Primeiro você precisa entrar em serviço.");
	        if (!IsFactionLegal(faction))											return SendErrorMessage(playerid, "Você deve ser um policial/paramédico.");
            if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))	return SendErrorMessage(playerid, "Você não está no vestiário.");
    			
		    if (IsFactionMedic(faction) || IsFactionFire(faction)) SetPlayerArmour(playerid, 50.0);
			else				SetPlayerArmour(playerid, 100.0);

			SendSyntaxMessage(playerid, "Você está vestindo um colete à prova de balas.");
            cmd::duty(playerid, "");
	    }
	    case 4:
	    {
	        if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Você deve estar em serviço (/duty).");
            if (!IsPlayerAtUniform(playerid))	return SendErrorMessage(playerid, "Você não está no vestiário.");

		    SetPlayerHealth(playerid, 100.0);
		    
			SendSyntaxMessage(playerid, "Você reabasteceu sua saúde.");
		    cmd::duty(playerid, "");
	    }
	}
	return 1;
}

Dialog:ScrapVehicleFaction(playerid, response, listitem, inputtext[])
{
	if (!response)											return 1;
    if (PlayerInfo[playerid][pFaction] == 0)				return SendErrorMessage(playerid, "Você não é membro de uma facção.");
	if (!IsFactionLegal(PlayerInfo[playerid][pFaction]))	return SendErrorMessage(playerid, "Você deve estar na facção governamental.");

	new veh = PlayerInfo[playerid][pCarKey];

	if (!IsValidVehicle(veh))					return SendErrorMessage(playerid, "Seu veículo não está gerado.");
	if (!IsPlayerInVehicle(playerid, veh))		return SendErrorMessage(playerid, "Você não é o proprietário deste veículo.");
	if (IsADonateModel(GetVehicleModel(veh)))	return SendErrorMessage(playerid, "Você não pode transferir este veículo.");
	if (VehicleInfo[veh][carComponent][5] != 0)	return SendErrorMessage(playerid, "Você não pode transferir este veículo enquanto ele estiver com nitro.");
	    
    VehicleInfo[veh][carOwnerID] = 0;
    VehicleInfo[veh][carFaction] = PlayerInfo[playerid][pFaction];
    StopCarBoomBox(veh);
    
	new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `cars` SET `owner_id`=0, `faction`=%i WHERE `id`=%i", PlayerInfo[playerid][pFaction], VehicleInfo[veh][carID]);
	mysql_tquery(dbHandle, query);
    PlayerInfo[playerid][pCarKey] = 0;
    VehicleInfo[veh][carOwnerPlayer] = INVALID_PLAYER_ID;
    DeleteVehicleTickets(veh);
	SendSyntaxMessage(playerid, "Você transferiu seu veículo %s para a facção %s.", VehicleNames[VehicleInfo[veh][carModel]-400], Factions[PlayerInfo[playerid][pFaction]][fName]);
	return 1;
}
Dialog:BortListChoose(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::bortlist(playerid, "");

    new bort = GetPVarInt(playerid, #select_bort)-1;

    DeletePVar(playerid, #select_bort);
	if (bort < 0)	return SendErrorMessage(playerid, "Objeto não encontrado. ");

	if (BortInfo[playerid][bort][bortUsing]) return SendErrorMessage(playerid, "O objeto está atualmente em uso! ");

	if (listitem == 0)
	{
	    if (BortInfo[playerid][bort][bortOn] == 0)	return SendErrorMessage(playerid, "Objeto não encontrado. ");

		new Float: x, Float: y, Float: z;
		GetDynamicObjectPos(BortInfo[playerid][bort][bortObject], x, y, z);


		if (GetPlayerDistanceFromPoint(playerid, x, y, z) > 10.0)	return SendErrorMessage(playerid, "O objeto está muito longe de você.");
		if (IsPlayerInAnyVehicle(playerid))							return SendErrorMessage(playerid, "Você não deve estar em um veículo.");
    	if (!PlayerInfo[playerid][pOnDuty])							return SendErrorMessage(playerid, "Primeiro você precisa entrar em serviço. ");

		EditDynamicObject(playerid, BortInfo[playerid][bort][bortObject]);
		pTemp[playerid][pEditBort] = bort+1;
	    return 1;
	}

    if (!BortInfo[playerid][bort][bortOn]) 	return SendErrorMessage(playerid, "Objeto não encontrado. ");

	BortInfo[playerid][bort][bortOn] = 0;
	BortInfo[playerid][bort][bortUsing] = 0;

	if (IsValidDynamicObject(BortInfo[playerid][bort][bortObject]))
	{
		DestroyDynamicObject(BortInfo[playerid][bort][bortObject]);
 		BortInfo[playerid][bort][bortObject] = INVALID_OBJECT_ID;
	}

	SendSyntaxMessage(playerid, "Um objeto '%s' foi deletado.", BortInfo[playerid][bort][bortName]);

	for (new i = 0; i != MAX_BORTES; i ++)
	{
		if (BortInfo[playerid][i][bortOn] == 0) continue;
		cmd::bortlist(playerid, "");
		break;
	}

	return 1;
}

Dialog:BortList(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (listitem == 0)
	{
		SendSyntaxMessage(playerid, "Todos os objetos foram deletados.");
	    DestroyAllBort(playerid);
	    return 1;
	}
	
	new count;

	for (new i = 0; i != MAX_BORTES; i ++)
	{
		if (BortInfo[playerid][i][bortOn] == 0) continue;
		
  		if (count == listitem-1)
      	{
       		Dialog_Show(playerid, BortListChoose, DIALOG_STYLE_LIST, BortInfo[playerid][i][bortName], "Editar objeto\nDeletar objeto", "Selecionar", "Voltar");
         	SetPVarInt(playerid, #select_bort, i+1);
			return 1;
   		}
    	count++;
	}
	SendErrorMessage(playerid, "Objeto não encontrado. ");
	return 1;
}


Dialog:FactionSettings(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	return ShowRightsForRank(playerid, listitem);
}

Dialog:FactionSettings_Set(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new faction = PlayerInfo[playerid][pFaction], rank = GetPVarInt(playerid, "Faction:RankEdit")-1;

	switch(listitem)
	{
		case 0:	FRank[faction][rank][rInvite] = !FRank[faction][rank][rInvite];
		case 1: FRank[faction][rank][rUninvite] = !FRank[faction][rank][rUninvite];
		case 2: FRank[faction][rank][rRank] = !FRank[faction][rank][rRank];
		case 3: FRank[faction][rank][r_eRank] = !FRank[faction][rank][r_eRank];
		case 4: FRank[faction][rank][rSpawn] = !FRank[faction][rank][rSpawn];
		case 5: FRank[faction][rank][rChat] = !FRank[faction][rank][rChat];
		case 6: FRank[faction][rank][rTow] = !FRank[faction][rank][rTow];
		case 7: FRank[faction][rank][rBodyCam] = !FRank[faction][rank][rBodyCam];
		case 8: FRank[faction][rank][r_eRights] = !FRank[faction][rank][r_eRights];
	}

	UpdateFactionRank(faction, rank+1);
	ShowRightsForRank(playerid, rank);
	DeletePVar(playerid, "Faction:EditRank");
	return 1;
}