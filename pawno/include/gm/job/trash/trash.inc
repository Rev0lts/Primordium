this::LoadDynamicTrashes()
{
    new rows = cache_num_rows(), time = GetTickCount(), total, label[64];

	if (!rows)	return print("[Lixos] Nenhum carregado.");

	for(new i; i < rows; i++)
	{
	    TrashData[i][TrashID] = cache_get_field_content_int(i, "id", dbHandle);
		TrashData[i][TrashX] = cache_get_field_content_float(i, "x", dbHandle);
		TrashData[i][TrashY] = cache_get_field_content_float(i, "y", dbHandle);
		TrashData[i][TrashZ] = cache_get_field_content_float(i, "z", dbHandle);
		TrashData[i][TrashRZ] = cache_get_field_content_float(i, "rz", dbHandle);
		TrashData[i][TrashType] = cache_get_field_content_int(i, "type", dbHandle);

		TrashData[i][TrashLevel] = (TrashData[i][TrashType] == TYPE_BIN) ? 1 : 2;
 
        format(label, sizeof(label), "%s", (TrashData[i][TrashType] == TYPE_BIN) ? ("Lixeira") : ("Container de lixo"));
		TrashData[i][TrashLabel] = CreateDynamic3DTextLabel(label, 0x2ECC71FF, TrashData[i][TrashX], TrashData[i][TrashY], TrashData[i][TrashZ]+1.25, 15.0, .testlos = 1);
        
        new trashModel = (TrashData[i][TrashType] == TYPE_DUMPSTER) ? 1344 : 1328;
        TrashData[i][TrashObject] = CreateDynamicObject(trashModel, TrashData[i][TrashX], TrashData[i][TrashY], TrashData[i][TrashZ], 0.0, 0.0, TrashData[i][TrashRZ], 0, 0);

		total++;
	}
	printf("[Lata de lixo] Rows - %i. carregado - %i. tempo: %i ms.", rows, total, GetTickCount()-time);
	return 1;
}

this::OnTrashInsert(id)
{
	return TrashData[id][TrashID] = cache_insert_id();
}

this::LoadDynamicFactories()
{
    new rows = cache_num_rows(), time = GetTickCount(), total, label[128];

	if (!rows) return print("[Processadoras de Lixo] Nenhum registro encontrado.");
	 
	for(new i; i < rows; i++)
	{
	    FactoryData[i][FactoryID] = cache_get_field_content_int(i, "id", dbHandle);
		cache_get_field_content(0, "name", FactoryData[i][FactoryName], dbHandle, 32);
		FactoryData[i][FactoryX] = cache_get_field_content_float(i, "x", dbHandle);
		FactoryData[i][FactoryY] = cache_get_field_content_float(i, "y", dbHandle);
		FactoryData[i][FactoryZ] = cache_get_field_content_float(i, "z", dbHandle);
		
	    format(label, sizeof(label), "Central de processamento de res�duos - %s\n\n{FFFFFF}Quantidade atual de res�duos reciclados: {F39C12}0", FactoryData[i][FactoryName]);
		FactoryData[i][FactoryLabel] = CreateDynamic3DTextLabel(label, 0x2ECC71FF, FactoryData[i][FactoryX], FactoryData[i][FactoryY], FactoryData[i][FactoryZ] + 0.5, 15.0, .testlos = 1);
		FactoryData[i][FactoryCP] = CreateDynamicCP(FactoryData[i][FactoryX], FactoryData[i][FactoryY], FactoryData[i][FactoryZ], 6.0);
		
		total++;
	}
	printf("[Processadoras de Lixo] Rows - %i. carregado - %i. tempo: %i ms.", rows, total, GetTickCount()-time);
	return 1;
}

this::OnFactoryInsert(id)
{
	return FactoryData[id][FactoryID] = cache_insert_id();
}

Trash_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rz)
{
    new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;

    GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
	GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

    if (GetPVarInt(playerid, #edit_trash) != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
	{
	    new id = GetPVarInt(playerid, #edit_trash)-1;
	    
		DeletePVar(playerid, #edit_trash);
		if (id < 0 || id >= MAX_TRASHES || !TrashData[id][TrashID]) return SendErrorMessage(playerid, "Lixeira n�o encontrada.");
		if (objectid != TrashData[id][TrashObject]) return SendErrorMessage(playerid, "Erro ao editar a lixeira");

		SetDynamicObjectPos(objectid, x, y, z);
		SetDynamicObjectRot(objectid, 0.0, 0.0, rz);

        GetDynamicObjectPos(objectid, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]);
        TrashData[id][TrashRZ] = rz;

		if (IsValidDynamic3DTextLabel(TrashData[id][TrashLabel])) DestroyDynamic3DTextLabel(TrashData[id][TrashLabel]);

		new label[64];
        format(label, sizeof(label), "%s", (TrashData[id][TrashType] == TYPE_BIN) ? ("Trash can") : ("Garbage container"));
		TrashData[id][TrashLabel] = CreateDynamic3DTextLabel(label, 0x2ECC71FF, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]+1.25, 15.0, .testlos = 1);
        
		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `trashes` SET `x`=%f,`y`=%f,`z`=%f,`rz`=%f WHERE `id`=%i", x, y, z, rz, TrashData[id][TrashID]);
		mysql_tquery(dbHandle, query);

		SendAdmMessageToPlayer(playerid, "Posi��o da lixeira [ID: %i] alterada.", id);
	}
    return 1;
}

Trash_InitPlayer(playerid)
{
    HasTrash[playerid] = false;
    TrashCP[playerid] = -1;
    
    for(new i; i < sizeof(FactoryData); i++)
	{
		TogglePlayerDynamicCP(playerid, FactoryData[i][FactoryCP], 0);
		FactoryIcons[playerid][i] = -1;
	}
	
	/*CapacityText[playerid] = CreatePlayerTextDraw(playerid, 32.000000, 290.000000, "��������� �������� �� 0/10");
	PlayerTextDrawBackgroundColor(playerid, CapacityText[playerid], 255);
	PlayerTextDrawFont(playerid, CapacityText[playerid], 1);
	PlayerTextDrawLetterSize(playerid, CapacityText[playerid], 0.200000, 1.000000);
	PlayerTextDrawColor(playerid, CapacityText[playerid], -1);
	PlayerTextDrawSetOutline(playerid, CapacityText[playerid], 1);
	PlayerTextDrawSetProportional(playerid, CapacityText[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, CapacityText[playerid], 0);*/

	
	CapacityText[playerid] = CreatePlayerTextDraw(playerid, 15.416525, 171.518585, "Trashmaster__~w~IIIIIIIIII~n~FILLED 0 / 10");
	PlayerTextDrawLetterSize(playerid, CapacityText[playerid], 0.164166, 0.951852);
	PlayerTextDrawAlignment(playerid, CapacityText[playerid], 1);
	PlayerTextDrawColor(playerid, CapacityText[playerid], -1);
	PlayerTextDrawSetShadow(playerid, CapacityText[playerid], 0);
	PlayerTextDrawSetOutline(playerid, CapacityText[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, CapacityText[playerid], 255);
	PlayerTextDrawFont(playerid, CapacityText[playerid], 1);
	PlayerTextDrawSetProportional(playerid, CapacityText[playerid], 1);
	PlayerTextDrawSetShadow(playerid, CapacityText[playerid], 0);

	//CapacityBar[playerid] = CreatePlayerProgressBar(playerid, 30.000000, 304.000000, 118.500000, 6.199999, -1429936641, TRASH_LIMIT, 0);
	return 1;
}

Trash_ResetPlayer(playerid, removeUI = 0)
{
	if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACHMENT_INDEX)) RemovePlayerAttachedObject(playerid, ATTACHMENT_INDEX);
	if (IsValidDynamicCP(TrashCP[playerid])) DestroyDynamicCP(TrashCP[playerid]);
	HasTrash[playerid] = false;
	TrashCP[playerid] = -1;
	
	if (removeUI)
	{
	    PlayerTextDrawDestroy(playerid, CapacityText[playerid]);
	    //DestroyPlayerProgressBar(playerid, CapacityBar[playerid]);
	}
	
	return 1;
}

Trash_ShowCapacity(playerid)
{
    new vehicleid = GetPlayerVehicleID(playerid);

	static const capacity_string[] = "Trashmaster__~y~%s~n~FILLED ON %d / %d";
	new progress[TRASH_LIMIT+3], mes[sizeof(capacity_string)+sizeof(progress)+10];

	for (new i = 1; i <= TRASH_LIMIT; i++) {
		format(progress, sizeof(progress), "%sI", progress);
		if (LoadedTrash[vehicleid] == i) {
			format(progress, sizeof(progress), "%s~w~", progress);
		}
	}
	
	format(mes, sizeof(mes), capacity_string, progress, LoadedTrash[vehicleid], TRASH_LIMIT);

	if (LoadedTrash[vehicleid] == TRASH_LIMIT) {
		format(mes, sizeof(mes), "Trashmaster__~r~IIIIIIIIII~n~~w~FULLY FILLED!");
	}

    PlayerTextDrawSetString(playerid, CapacityText[playerid], mes);
    PlayerTextDrawShow(playerid, CapacityText[playerid]);

	//SetPlayerProgressBarValue(playerid, CapacityBar[playerid], LoadedTrash[vehicleid]);
	//ShowPlayerProgressBar(playerid, CapacityBar[playerid]);
	return 1;
}

Trash_Nearest(playerid, Float: dist = 3.0)
{
	new closest_id = -1, Float: tempdist;
    for(new i; i < sizeof(TrashData); i++)
	{
	    tempdist = GetPlayerDistanceFromPoint(playerid, TrashData[i][TrashX], TrashData[i][TrashY], TrashData[i][TrashZ]);
		if (tempdist > dist) continue;
		if (tempdist <= dist)
		{
		    dist = tempdist;
		    closest_id = i;
		}
	}
	
	return closest_id;
}

Trash_CheckFreeSlot()
{
	new id = -1;
	for(new i = 0; i < MAX_TRASHES; i++)
	{
		if (TrashData[i][TrashID]) continue;

		id = i;
		break;
	}
	return id;
}

Factory_Nearest(playerid, Float: dist = 3.0)
{
	new closest_id = -1, Float: tempdist;
    for(new i; i < MAX_FACTORIES; i++)
	{
	    tempdist = GetPlayerDistanceFromPoint(playerid, FactoryData[i][FactoryX], FactoryData[i][FactoryY], FactoryData[i][FactoryZ]);
		if (tempdist > dist) continue;
		if (tempdist <= dist)
		{
		    dist = tempdist;
		    closest_id = i;
		}
	}
	
	return closest_id;
}

Factory_CheckFreeSlot()
{
	new id = -1;
	for(new i = 0; i < MAX_FACTORIES; i++)
	{
		if (FactoryData[i][FactoryID]) continue;

		id = i;
		break;
	}
	return id;
}

EndTrashJob(playerid)
{
	for(new i; i < sizeof(FactoryData); i++)
	{
		if (IsValidDynamicMapIcon(FactoryIcons[playerid][i]))
		{
			DestroyDynamicMapIcon(FactoryIcons[playerid][i]);
			FactoryIcons[playerid][i] = -1;
		}

		TogglePlayerDynamicCP(playerid, FactoryData[i][FactoryCP], 0);
	}
			
	Trash_ResetPlayer(playerid);
	PlayerTextDrawHide(playerid, CapacityText[playerid]);
	//HidePlayerProgressBar(playerid, CapacityBar[playerid]);
	return 1;
}

Trash_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_DRIVER)
	{
	    new vehicleid = GetPlayerVehicleID(playerid);
	    if (IsTrashMasterCar(vehicleid) && pTemp[playerid][pRentveh] == vehicleid && pTemp[playerid][pJob])
	    {
		    if (LoadedTrash[vehicleid] > 0) {
				SendHelpMessage(playerid, "Este caminh�o de lixo est� cheio com {F39C12}%d {FFFFFF}sacos de lixo pelos quais voc� receber� {2ECC71}$%d.", LoadedTrash[vehicleid], LoadedTrash[vehicleid] * TRASH_BAG_VALUE);
				SendHelpMessage(playerid, "Voc� pode descarregar o caminh�o de lixo no aterro sanit�rio (marcado no mapa) e ser pago por isso.");

				for(new i; i < sizeof(FactoryData); i++)
				{
					if (!FactoryData[i][FactoryID]) continue;
				    FactoryIcons[playerid][i] = CreateDynamicMapIcon(FactoryData[i][FactoryX], FactoryData[i][FactoryY], FactoryData[i][FactoryZ], 51, 0, _, _, playerid, 8000.0, MAPICON_GLOBAL);
					TogglePlayerDynamicCP(playerid, FactoryData[i][FactoryCP], 1);
				}
		    } else {
		        SendHelpMessage(playerid, "Voc� precisa coletar lixo de latas de lixo e cont�ineres para ser pago por isso. ");
		    }
			Trash_ResetPlayer(playerid);
			Trash_ShowCapacity(playerid);
		}
	}
	
	if (oldstate == PLAYER_STATE_DRIVER)
	{
		if (PlayerInfo[playerid][pJob] == JOB_TRASHMASTER && pTemp[playerid][pJob])
		{
			EndTrashJob(playerid);
		}
	}
	return 1;
}


Trash_OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	if (checkpointid == TrashCP[playerid])
	{
	    if (!HasTrash[playerid]) return SendErrorMessage(playerid, "Voc� n�o tem um saco de lixo.");

	    new vehicleid = pTemp[playerid][pRentveh];
	    if (LoadedTrash[vehicleid] >= TRASH_LIMIT) return SendErrorMessage(playerid, "O caminh�o de lixo est� completamente cheio de lixo, descarregue no aterro sanit�rio! ");
	    
		LoadedTrash[vehicleid]++;
		ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.1, 0, 0, 0, 0, 0);
		InfoMSG(playerid, "~w~Voce ~y~colocou~w~ lixo no caminhao", 3);

		if (TRASH_LIMIT - LoadedTrash[vehicleid] == 0)
		{
			SendSyntaxMessage(playerid, "O caminh�o de lixo est� completamente cheio de lixo, descarregue no aterro sanit�rio! ");
		}
		
		new driver = GetVehicleDriver(vehicleid);
		if (IsPlayerConnected(driver)) Trash_ShowCapacity(driver);
		Trash_ResetPlayer(playerid);
		return 1;
	}

    if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		for(new i; i < sizeof(FactoryData); i++)
		{
		    if (checkpointid == FactoryData[i][FactoryCP])
		    {
		        new vehicleid = GetPlayerVehicleID(playerid), cash = LoadedTrash[vehicleid] * TRASH_BAG_VALUE;

		        SendSyntaxMessage(playerid, "Voc� descarregou {F39C12}%d {FFFFFF}sacos de lixo e ganhou {2ECC71}$%d.", LoadedTrash[vehicleid], cash);
		        GiveCash(playerid, cash);

		        FactoryData[i][FactoryCurrent] += LoadedTrash[vehicleid];
		        LoadedTrash[vehicleid] = 0;
                Trash_ShowCapacity(playerid);

				new string[128];
                format(string, sizeof(string), "Processamentos de residuos - %s\n\n{FFFFFF}Quantidade atual de res�duos reciclados: {F39C12}%d", FactoryData[i][FactoryName], FactoryData[i][FactoryCurrent]);
                UpdateDynamic3DTextLabelText(FactoryData[i][FactoryLabel], 0x2ECC71FF, string);
                
		        for(new x; x < sizeof(FactoryData); x++)
				{
				    if (IsValidDynamicMapIcon(FactoryIcons[playerid][x]))
				    {
				        DestroyDynamicMapIcon(FactoryIcons[playerid][x]);
				        FactoryIcons[playerid][x] = -1;
				    }

					TogglePlayerDynamicCP(playerid, FactoryData[x][FactoryCP], 0);
				}

		        break;
		    }
		}
	}
	
	return 1;
}


this::FillTrash(id)
{
	TrashData[id][TrashLevel]++;
	if (TrashData[id][TrashType] == TYPE_BIN && TrashData[id][TrashLevel] > 1) TrashData[id][TrashLevel] = 1;

	if (TrashData[id][TrashType] == TYPE_DUMPSTER) {
		if (TrashData[id][TrashLevel] == 1) TrashData[id][TrashTimer] = SetTimerEx("FillTrash", REFILL_TIME * 1000, false, "i", id);
		if (TrashData[id][TrashLevel] >= 2)
		{
			TrashData[id][TrashLevel] = 2;
			KillTimer(TrashData[id][TrashTimer]);
			TrashData[id][TrashTimer] = -1;
		}

		Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, TrashData[id][TrashLabel], E_STREAMER_COLOR, (TrashData[id][TrashLevel] == 1) ? 0xF39C12FF : 0x2ECC71FF);
		return 1;
	}

	Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, TrashData[id][TrashLabel], E_STREAMER_COLOR, 0x2ECC71FF);
	return 1;
}