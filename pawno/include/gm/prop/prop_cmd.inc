ALTX:lock("/trancar");
CMD:lock(playerid, params[]) //îòêðûòü è çàêðûòü
{
	if (PlayerInfo[playerid][pInjured] || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return SendErrorMessage(playerid, "Você não pode usar esta funcionalidade agora.");

	new bizid;
	if ((bizid = GetNearOutsideBiz(playerid, 1.5)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid != -1)
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID] && PlayerInfo[playerid][pWorkOn] != BizInfo[bizid][bizID]) return GameTextForPlayer(playerid, "~r~Você não tem uma chave!", 5000, 6);
	    if (BizInfo[bizid][bizDDoor] > 0) return SendErrorMessage(playerid, "A porta está quebrada.");

	    if (BizInfo[bizid][bizLocked])
	    {
	        BizInfo[bizid][bizLocked] = 0;
	        GameTextForPlayer(playerid, "~w~EMPRESA ~g~ABERTA", 5000, 6);
	        PlaySound(playerid, 1145);
	        return 1;
	    }
	    else
	    {
	        BizInfo[bizid][bizLocked] = 1;
	        GameTextForPlayer(playerid, "~w~EMPRESA ~r~FECHADA", 5000, 6);
	        PlaySound(playerid, 1145);
	        return 1;
	    }
	}

	new id = GetNearOutsideGarage(playerid, 1.5);
	if (id == -1) id = GetNearInsideGarage(playerid);
	if (id != -1)
	{
	    if (GarageInfo[id][garageOwner] != PlayerInfo[playerid][pID]
	        || (GarageInfo[id][garageHouse] && HouseInfo[GarageInfo[id][garageHouse]][hID] != PlayerInfo[playerid][pRentKey])
	        || (GarageInfo[id][garageBiz] && BizInfo[GarageInfo[id][garageBiz]][bizID] != PlayerInfo[playerid][pWorkOn]))
	        return GameTextForPlayer(playerid, "~r~Você não tem uma chave!", 5000, 6);

	    GarageInfo[id][garageLocked] = GarageInfo[id][garageLocked] ? 0 : 1;

	    if (GarageInfo[id][garageLocked]) GameTextForPlayer(playerid, "~w~GARAGEM ~r~FECHADA", 3000, 4);
	    else GameTextForPlayer(playerid, "~w~GARAGEM ~g~ABERTA", 3000, 4);

	    new query[60];
	    mysql_format(dbHandle, query, sizeof(query), "UPDATE `garage` SET `locked`=%i WHERE `id`=%i", 1, GarageInfo[id][garageLocked], GarageInfo[id][garageID]);
	    mysql_tquery(dbHandle, query);
	    return 1;
	}

	for (new i = 0; i != MAX_ENTRANCE; i++)
	{
	    if (!EntranceInfo[i][eFaction] || !EntranceInfo[i][entranceOn] || !(IsPlayerEntranceOutside(playerid, i) && IsPlayerEntranceInside(playerid, i))) continue;

	    if (PlayerInfo[playerid][pFaction] != EntranceInfo[i][eFaction]) return GameTextForPlayer(playerid, "~r~Você não tem uma chave!", 5000, 6);

	    EntranceInfo[i][eLocked] = (EntranceInfo[i][eLocked]) ? (0) : (1);
	    PlaySound(playerid, 1145);

	    if (EntranceInfo[i][eLocked]) GameTextForPlayer(playerid, "~w~Porta ~r~fechada", 5000, 6);
	    else GameTextForPlayer(playerid, "~w~Porta ~g~aberta", 5000, 6);

	    break;
	}

	
	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) != -1 && (HouseInfo[houseid][hOwnerID] == PlayerInfo[playerid][pID] || HouseInfo[houseid][hID] == PlayerInfo[playerid][pRentKey]))
	{
	    for(new i = 0; i < MAX_FURNITURE; i++)
		{
			if (!IsPlayerInRangeOfPoint(playerid, 2.0, HFurn[houseid][i][fPosX], HFurn[houseid][i][fPosY], HFurn[houseid][i][fPosZ]) || !HFurn[houseid][i][fOn] || !isHouseDoor(HFurn[houseid][i][fModel]))
   			{
				continue;
			}

			if (HFurn[houseid][i][fOpened]) return SendWarningMessage(playerid, "Feche a porta primeiro.");

			if (!HFurn[houseid][i][fLocked])
			{
			    InfoMSG(playerid, "Porta ~r~trancada", 3);
			    HFurn[houseid][i][fLocked] = 1;
			    return 1;
			}
			else
			{
			    InfoMSG(playerid, "Porta ~g~destrancada", 3);
			    HFurn[houseid][i][fLocked] = 0;
			    return 1;
			}
		}
	}

	new biz_id;
	if ((biz_id = GetNearInsideBiz(playerid)) != -1 && (BizInfo[biz_id][bizOwnerID] == PlayerInfo[playerid][pID] || BizInfo[biz_id][bizID] == PlayerInfo[playerid][pWorkOn]))
	{
	    for(new i = 0; i < MAX_FURNITURE; i++)
		{
			if (!IsPlayerInRangeOfPoint(playerid, 2.0, BFurn[biz_id][i][fPosX], BFurn[biz_id][i][fPosY], BFurn[biz_id][i][fPosZ]) || !BFurn[biz_id][i][fOn] || !BFurn[biz_id][i][fModel])
   			{
				continue;
			}

			if (BFurn[biz_id][i][fOpened]) return SendWarningMessage(playerid, "Feche a porta primeiro.");

			if (!BFurn[biz_id][i][fLocked])
			{
			    InfoMSG(playerid, "Porta ~r~trancada", 3);
			    BFurn[biz_id][i][fLocked] = 1;
			    return 1;
			}
			else
			{
			    InfoMSG(playerid, "Porta ~g~destrancada", 3);
			    BFurn[biz_id][i][fLocked] = 0;
			    return 1;
			}
		}
	}

	new garage = GetNearInsideGarage(playerid);
	if (garage != -1) {
		if (GarageInfo[garage][garageOwner] == PlayerInfo[playerid][pID]
		|| (GarageInfo[garage][garageHouse] && HouseInfo[GarageInfo[garage][garageHouse]][hID] == PlayerInfo[playerid][pRentKey])
		|| (GarageInfo[garage][garageBiz] && BizInfo[GarageInfo[garage][garageBiz]][bizID] == PlayerInfo[playerid][pWorkOn]))
		{
			for(new i = 0; i < MAX_FURNITURE; i++)
			{
				if (!IsPlayerInRangeOfPoint(playerid, 2.0, GFurn[garage][i][fPosX], GFurn[garage][i][fPosY], GFurn[garage][i][fPosZ]) || !GFurn[garage][i][fOn] || !GFurn[garage][i][fModel])
				{
					continue;
				}

				if (GFurn[garage][i][fOpened]) return SendWarningMessage(playerid, "Feche a porta primeiro.");

				if (!GFurn[garage][i][fLocked])
				{
				    InfoMSG(playerid, "Porta ~r~trancada", 3);
				    GFurn[garage][i][fLocked] = 1;
				    return 1;
				}
				else
				{
				    InfoMSG(playerid, "Porta ~g~destrancada", 3);
				    GFurn[garage][i][fLocked] = 0;
				    return 1;
				}
			}
		}
	}
	
	if ((houseid = GetNearHouse(playerid, 30.0, 1.2)) != -1)
	{
		if (HouseInfo[houseid][hDDoor] > 0) return SendWarningMessage(playerid, "A porta está quebrada.");

		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID] && HouseInfo[houseid][hID] != PlayerInfo[playerid][pRentKey])
		    return GameTextForPlayer(playerid, "~r~Você não tem a chave!", 5000, 6);

		if (HouseInfo[houseid][hLocked] == 1)
		{
		    HouseInfo[houseid][hLocked] = 0;
		    GameTextForPlayer(playerid, "~w~Porta ~g~aberta", 5000, 3);
		    PlaySound(playerid, 1145);
		    return 1;
		}
		if (HouseInfo[houseid][hLocked] == 0)
		{
		    HouseInfo[houseid][hLocked] = 1;
		    GameTextForPlayer(playerid, "~w~Porta ~r~fechada", 5000, 3);
		    PlaySound(playerid, 1145);
		    return 1;
		}
	}

	if (GetNearVehicle(playerid, 4.0) != 0) cmd::vehicle(playerid, "lock");
	
	return 1;
}

ALTX:property("/pr","/propriedade");
CMD:property(playerid, params[])
{
	return ShowProperty(playerid, playerid);
}
ALTX:clothes("/roupas");
CMD:clothes(playerid, params[])
{
	if (PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Você deve estar fora de serviço.");

	new id, used;

	if ((id = GetNearInsideHouse(playerid)) != -1)
	{
		if (HouseInfo[id][hOwnerID] != PlayerInfo[playerid][pID] && PlayerInfo[playerid][pRentKey] != HouseInfo[id][hID])	return SendErrorMessage(playerid, "Esta casa não pertence a você ou você não a alugou.");
		if (!HouseInfo[id][hCupboard])																						return SendErrorMessage(playerid, "Não há armário de armazenamento na casa.");

		used = 1;
	}

	if (!used && (id = GetNearInsideBiz(playerid)) != -1)
	{
		if (BizInfo[id][bizType] != BIZ_CLOTHES)	return SendErrorMessage(playerid, "Você não está em uma loja de roupas.");

		used = 1;
	}

	if (!used) return 1;

	new mes[128];

	for(new i; i != 3; i++)
	{
	 	if (!PlayerInfo[playerid][pTog][i])	strcat(mes, "\n[Colocar]");
	  	else								format(mes, sizeof(mes), "%s\n[Selecionar pele #%i]", mes, PlayerInfo[playerid][pTog][i]);
	}

	Dialog_Show(playerid, EditarRoupas, DIALOG_STYLE_LIST, "Selecionar slot...", mes, "Selecionar", "Cancelar");
	return 1;
}

CMD:tocar(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] == 2)	return SendErrorMessage(playerid, "Você não pode bater na porta no modo de morte.");

	for(new i = 0; i < MAX_HOUSES; i++)
	{
	    if (!HouseInfo[i][hID] || !IsPlayerOutsideHouse(playerid, i, 2.0)) continue;

		foreach(new p : Player)
		{
		    if (!IsPlayerInsideHouse(p, i)) continue;

  			SendClientMessageEx(p, COLOR_PURPLE, "* [Porta] Toc! Toc! Toc! (( %s ))", GetNameWithMask(playerid));
		}

		cmd::me(playerid, "bate na porta da casa.");
		return 1;
	}

	for(new i = 0; i < MAX_BIZ; i++)
	{
	    if (!BizInfo[i][bizID] || !IsPlayerBizOutside(playerid, i, 2.0)) continue;

		foreach(new p : Player)
		{
		    if (!IsPlayerBizInside(p, i)) continue;

			SendClientMessageEx(p, COLOR_PURPLE, "* [Porta] Toc! Toc! Toc! (( %s ))", GetNameWithMask(playerid));
		}

		cmd::me(playerid, "bate na porta do estabelecimento.");
		return 1;
	}
	return 1;
}
