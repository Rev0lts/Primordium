Corpse_OnPlayerEdit(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rz)
{
    if (GetPVarInt(playerid, #CorpsEdit) != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
    {
        new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;

        GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
        GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

        new id = GetPVarInt(playerid, #CorpsEdit)-1;
        DeletePVar(playerid, #CorpsEdit);

        if (id < 0 || id >= sizeof(CorpInfo) || !CorpInfo[id][cUsed]) return SendErrorMessage(playerid, "O corpo não foi encontrado.");
        if (objectid != CorpInfo[id][cBody]) return SendErrorMessage(playerid, "Erro na edição do corpo.");

        SetDynamicObjectPos(objectid, x, y, z);
        SetDynamicObjectRot(objectid, 0.0, 0.0, rz);

        GetDynamicObjectPos(objectid, CorpInfo[id][cX], CorpInfo[id][cY], CorpInfo[id][cZ]);

        if (IsValidDynamic3DTextLabel(CorpInfo[id][cText])) DestroyDynamic3DTextLabel(CorpInfo[id][cText]);
        CorpInfo[id][cText] = CreateDynamic3DTextLabel("(( CORPO MORTO ))\npressione '~k~~GROUP_CONTROL_BWD~'", COLOR_LIGHTRED, CorpInfo[id][cX], CorpInfo[id][cY], CorpInfo[id][cZ]-0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 50.0);
    }
    return 1;
}


Corpse_OnPlayerUpdate(playerid)
{
    if (pTemp[playerid][UsingBort] && pTemp[playerid][UsingCorpse]) {
        new Float:X, Float:Y, Float:Z, Float:R;
        GetPlayerPos(playerid, X, Y, Z);
        GetPlayerFacingAngle(playerid, R);
        GetXYInFrontOfPlayer(playerid, X, Y, Z, 1.8);

        new idx = pTemp[playerid][UsingCorpse];
        CorpInfo[idx][cX]=X;
        CorpInfo[idx][cY]=Y;
        CorpInfo[idx][cZ]=Z;
        SetActorPos(CorpInfo[idx][cBody], X, Y, Z + 0.60);
        SetActorFacingAngle(CorpInfo[idx][cBody], R);
    }
    return 1;
}

Corpse_OnPlayerEnterVehicle(playerid)
{
    if (pTemp[playerid][UsingBort]) {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        SetPlayerPos(playerid,x,y,z);
        SendErrorMessage(playerid, "Você não pode entrar no veículo enquanto segura a cadeira de rodas.");
    }
    return 1;
}

CreateCorpse(playerid, weaponid)
{
    if (weaponid == 53) return 1;

    new 
        found = 0, 
        foundid = 0, 
        Float:x, 
        Float:y, 
        Float:z, 
        sex[8],
        age
    ;

    GetPlayerPos(playerid, x, y, z);

	for(new o = 0; o < sizeof(CorpInfo); o++)
	{
		if (o != 0)
		{
	        if (CorpInfo[o][cUsed] == 0 && found == 0)
		    {
		        found++;
			    foundid=o;
                break;
            }
        }
    }
    if (found == 0) return 1;

    CorpInfo[foundid][cUsed]=1;
    CorpInfo[foundid][cVeh]=0;
    
    format(CorpInfo[foundid][cName], 25, "%s", GetName(playerid));
    CorpInfo[foundid][cType] = 0;
    CorpInfo[foundid][cTime] = gettime();
    
    CorpInfo[foundid][cX]=x;
    CorpInfo[foundid][cY]=y;
    CorpInfo[foundid][cZ]=z;
    
    if (weaponid == 54) CorpInfo[foundid][cX] -= 0.5;
    
	CorpInfo[foundid][cSkin]=GetPlayerSkin(playerid);
	CorpInfo[foundid][cBody]=CreateActor(GetPlayerSkin(playerid), x, y, z, 0.0);
	SetActorInvulnerable(CorpInfo[foundid][cBody], true);
	ApplyActorAnimation(CorpInfo[foundid][cBody], "PED", "KO_shot_stom", 4.0, 0, 0, 0, 1, 0);
    SetActorVirtualWorld(CorpInfo[foundid][cBody], GetPlayerVirtualWorld(playerid));

	if (PlayerInfo[playerid][pSex] == 1)	format(sex, sizeof(sex), "Masculino");
	else 			                        format(sex, sizeof(sex), "Feminino");

    age = 2020 - GetAge(playerid) + random(7) - random(5);
    
    format(CorpInfo[foundid][cNote], 200, "{FFFFFF}(( Nome: %s ))\n\nIdade aproximada:\t %d\nGênero da vítima:\t %s\n\n", GetNameEx(playerid), age, sex);

    new count;
    for(new i = 0; i != MAX_DAMAGES; i++)
    {
        if (DamageInfo[playerid][i][damageOn] != 1) continue;    
        count++;
    }

    if (count > 0) format(CorpInfo[foundid][cNote], 200, "%s a vítima foi ferida: %d vez(es)\n\n", CorpInfo[foundid][cNote], count);
    else format(CorpInfo[foundid][cNote], 200, "%s sem feridas visíveis / queimaduras no corpo.\n\n", CorpInfo[foundid][cNote]);


    switch(weaponid)
    {
        case 0: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Foi espancado.", CorpInfo[foundid][cNote]);
        case 1 .. 16: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Espancado com um objeto.", CorpInfo[foundid][cNote]);
        case 22 .. 34: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Foi baleado com %s.", CorpInfo[foundid][cNote], inventory_items[weaponid][itemName]);
        case 17, 41 .. 42: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Provavelmente sufocado.", CorpInfo[foundid][cNote]);
        case 49: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Atropelado por um veículo.", CorpInfo[foundid][cNote]);
        case 50: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Despedaçado por lâminas de aeronave.", CorpInfo[foundid][cNote]);
        case 18: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Queimaduras múltiplas.", CorpInfo[foundid][cNote]);
        case 51: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Morreu de explosão.", CorpInfo[foundid][cNote]);
        case 53: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Afogado com água.", CorpInfo[foundid][cNote]);
        case 54: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Queda de grande altura.", CorpInfo[foundid][cNote]);
        default: format(CorpInfo[foundid][cNote], 200, "%s Causa da morte: Desconhecida.", CorpInfo[foundid][cNote]);
    }

    CorpInfo[foundid][cText]=CreateDynamic3DTextLabel("(( CORPO MORTO ))\npressione '~k~~GROUP_CONTROL_BWD~'", COLOR_LIGHTRED, x, y, z-0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 50.0);
	return 1;
}

Dialog:CorpseInfo(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    new dlg[90], i = GetNearestCorpse(playerid);

    switch(CorpInfo[i][cType]) {
        case 0: { // Não identificado
            dlg = "Levar para o veículo\nColocar em uma bolsa\nEnterrar";
        }
        case 1: { // Maca
            dlg = "Remover o corpo da maca";
        }
        case 2: { // Bolsa de cadáver
            dlg = "Levar para o veículo\nTirar o corpo da bolsa";
        }
        case 3: { // Enterrado
            dlg = "Desenterrar um corpo";
        }
    }

    if (!CorpInfo[i][cType] && PlayerInfo[playerid][pOnDuty] == 1 && (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid)))
        strcat(dlg, "\nColocar na maca");

    if (CheckAdmin(playerid, ADMIN_LEVEL_1) || (PlayerInfo[playerid][pOnDuty] == 1 && (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid) || IsPlayerFactionPolice(playerid))))
        strcat(dlg, "\nRemover objeto do corpo");

    Dialog_Show(playerid, CorpseInfo2, DIALOG_STYLE_LIST, "Corpo morto", dlg, "Selecionar", "Retornar");
    return 1;
}



Dialog:CorpseInfo2(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::cadaver(playerid, "");

    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
	    return SendErrorMessage(playerid, "Você não pode usar esse recurso no momento.");

    if (!IsPlayerLogged(playerid)) return SendNotConnected(playerid);

	new i = GetNearestCorpse(playerid);

    switch(listitem) {
        case 0: {
            switch(CorpInfo[i][cType]) {
                case 0: listitem=0;
                case 1: listitem=1;
                case 2: listitem=0;
                case 3: listitem=2;
            }
        }
        case 1: {
            switch(CorpInfo[i][cType]) {
                case 0: listitem=5;
                case 1: listitem=7;
                case 2: listitem=2;
                case 3: listitem=7;
            }
        }
        case 2: {
            switch(CorpInfo[i][cType]) {
                case 0: listitem=6;
                case 2: listitem=7;
            }
        }
        case 3: listitem=4;
        case 4: listitem=7;
    }

    switch(listitem)
    {
        case 0:
        {
            new vehicle = GetNearVehicle(playerid, 4.0);
            if (vehicle == -1) return SendErrorMessage(playerid, "Você não está próximo de um veículo.");
            if (!IsNearTrunk(vehicle, playerid, 2.0)) return SendErrorMessage(playerid, "Você não está perto do porta-malas!");
            if (!VehicleInfo[vehicle][carTrunkLock]) return SendErrorMessage(playerid, "O porta-malas deve estar aberto.");

            if (VehicleInfo[vehicle][vCorp] > 0 && CorpInfo[VehicleInfo[vehicle][vCorp]][cUsed] == 1)
                return SendWarningMessage(playerid, "Já há um corpo no porta-malas!");

            new mes[128];
            format(mes, sizeof(mes), "arrasta um corpo %s no porta-malas.", CorpInfo[i][cName]);
            cmd::me(playerid, mes);

            SendSyntaxMessage(playerid, "Você colocou o corpo no porta-malas. (( /v corpo - para retirar ))");

                    
            CorpInfo[i][cTime] = gettime();
            CorpInfo[i][cVeh]=vehicle;
            VehicleInfo[vehicle][vCorp]=i+1;

            CorpInfo[i][cX] =
            CorpInfo[i][cY] =
            CorpInfo[i][cZ] = 0.0;
                    

            DestroyDynamic3DTextLabel(CorpInfo[i][cText]);
            if (!CorpInfo[i][cType] && IsValidActor(CorpInfo[i][cBody])) DestroyActor(CorpInfo[i][cBody]);
            else if (CorpInfo[i][cType] && IsValidDynamicObject(CorpInfo[i][cBody])) DestroyDynamicObject(CorpInfo[i][cBody]);
        }
        case 1: {
            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);

            CorpInfo[i][cType] = 0;
            CorpInfo[i][cX] = x+0.75;
            CorpInfo[i][cY] = y;
            CorpInfo[i][cZ] = z-0.5;
            CorpInfo[i][cTime] = gettime();

            CorpInfo[i][cText]=CreateDynamic3DTextLabel("(( CORPO MORTO ))\npressione '~k~~GROUP_CONTROL_BWD~'", COLOR_LIGHTRED, CorpInfo[i][cX], CorpInfo[i][cY], CorpInfo[i][cZ]-0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 50.0);

            pTemp[playerid][UsingCorpse] = 0;

            SetActorPos(CorpInfo[i][cBody], CorpInfo[i][cX], CorpInfo[i][cY], CorpInfo[i][cZ]);
            SendSyntaxMessage(playerid, "Você retirou o corpo da cadeira de rodas.");
        }
        case 2: {
            if (IsValidDynamicObject(CorpInfo[i][cBody]))        DestroyDynamicObject(CorpInfo[i][cBody]);
            if (IsValidDynamic3DTextLabel(CorpInfo[i][cText]))   DestroyDynamic3DTextLabel(CorpInfo[i][cText]);

            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);

            CorpInfo[i][cType] = 0;
            CorpInfo[i][cX] = x+0.75; 
            CorpInfo[i][cY] = y;
            CorpInfo[i][cZ] = z-0.5;
            CorpInfo[i][cTime] = gettime();

            CorpInfo[i][cBody]=CreateActor(CorpInfo[i][cSkin], x+0.75-0.5, y, z, 0.0);
            SetActorInvulnerable(CorpInfo[i][cBody], true);
            ApplyActorAnimation(CorpInfo[i][cBody], "PED", "KO_shot_stom", 4.0, 0, 0, 0, 1, 0);

            CorpInfo[i][cText]=CreateDynamic3DTextLabel("(( CORPO MORTO ))\npressione '~k~~GROUP_CONTROL_BWD~'", COLOR_LIGHTRED, CorpInfo[i][cX], CorpInfo[i][cY], CorpInfo[i][cZ]-0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 50.0);

            if (CorpInfo[i][cType] == 3) SendSyntaxMessage(playerid, "Você desenterrou o corpo.");
            else                        SendSyntaxMessage(playerid, "Você tirou o corpo da bolsa.");
        }
        case 4: 
        {

            if (!pTemp[playerid][UsingBort]) return SendErrorMessage(playerid, "Pegue a maca primeiro! (/maca)");
            if (pTemp[playerid][UsingCorpse]) return SendErrorMessage(playerid, "Você já tem um corpo na maca!");
            if (IsValidDynamic3DTextLabel(CorpInfo[i][cText]))   DestroyDynamic3DTextLabel(CorpInfo[i][cText]);
           
            pTemp[playerid][UsingCorpse] = i;
            CorpInfo[i][cType] = 1;   

            SendSyntaxMessage(playerid, "Você colocou o corpo na maca.");
        }
        case 5: // Colocar em uma bolsa de corpo
        {
            if (!IsPlayerHasItem(playerid, INV_BODYBAG)) return SendErrorMessage(playerid, "Você não possui uma bolsa de corpo.");

            new Float:posZ;
            GetActorFacingAngle(CorpInfo[i][cBody], posZ);

            if (IsValidActor(CorpInfo[i][cBody]))  DestroyActor(CorpInfo[i][cBody]);

            SetPVarInt(playerid, #CorpsEdit, i+1);

            CorpInfo[i][cType] = 2;
            CorpInfo[i][cBody] = CreateDynamicObject(19944, CorpInfo[i][cX], CorpInfo[i][cY], CorpInfo[i][cZ]-0.5, 0.0, 0.0, posZ, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));

            EditDynamicObject(playerid, CorpInfo[i][cBody]);

            SM1(playerid, "Você colocou o corpo em uma bolsa. [ID:%i]", i);
            cmd::me(playerid, "embala o corpo em uma bolsa.");
        }

        case 6: //çàêîïàòü
        {
            if (GetWorld(playerid) || GetInterior(playerid)) return SendErrorMessage(playerid, "Você não pode enterrar um corpo aqui!");
            //if (!IsPlayerHasItem(playerid, 6))            return SendErrorMessage(playerid, "Você precisa ter uma pá para isso!"); 
            if (PlayerInfo[playerid][pPlayerWeapon] != 6) return SendErrorMessage(playerid, "Você deve segurar a pá na mão!");

            new Float:posZ;
            GetActorFacingAngle(CorpInfo[i][cBody], posZ);

            if (IsValidActor(CorpInfo[i][cBody]))  DestroyActor(CorpInfo[i][cBody]);
            if (IsValidDynamicObject(CorpInfo[i][cBody]))  DestroyDynamicObject(CorpInfo[i][cBody]);
                        
            SetPVarInt(playerid, #CorpsEdit, i+1);

            CorpInfo[i][cType] = 3;
            CorpInfo[i][cBody]=CreateDynamicObject(19944 , CorpInfo[i][cX], CorpInfo[i][cY], CorpInfo[i][cZ]-0.5, 0.0, 0.0, posZ, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
            SetObjectMaterial(CorpInfo[i][cBody], 0, 10101, "2notherbuildsfe", "Bow_Abpave_Gen", 0x00000000);

            EditDynamicObject(playerid, CorpInfo[i][cBody]);

            SM1(playerid, "Você enterrou o corpo. [ID:%i]", i);
            cmd::me(playerid, "enterra o corpo.");

        }
        case 7: //remover
        {
            SM1(playerid, "Você removeu o objeto do corpo. [ID:%i]", i);
            RemoveCorpse(i);
            return 1;
        }
    }
	return 1;
}

GetNearestCorpse(playerid, Float:corpse_range = 2.0)
{
    if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) return -1;

	for(new i = 0; i < sizeof(CorpInfo); i++)
    {
        if (CorpInfo[i][cUsed] == 1)
        {
            if (IsPlayerInRangeOfPoint(playerid, corpse_range, CorpInfo[i][cX], CorpInfo[i][cY], CorpInfo[i][cZ]))
            {
                return i;
            }
        }
    }
    return -1;
}

RemoveCorpse(id)
{
	if (id == 0) return 1;
	if (CorpInfo[id][cUsed] == 1)
	{
	    CorpInfo[id][cUsed]=0;
        CorpInfo[id][cType]=0;
	    CorpInfo[id][cX]=0;
        CorpInfo[id][cY]=0;
        CorpInfo[id][cZ]=0;
		CorpInfo[id][cSkin]=0;

		if (IsValidActor(CorpInfo[id][cBody]))           DestroyActor(CorpInfo[id][cBody]);
        if (IsValidDynamicObject(CorpInfo[id][cBody]))   DestroyDynamicObject(CorpInfo[id][cBody]);
		
        if (CorpInfo[id][cVeh] > 0 && GetVehicleModel(CorpInfo[id][cVeh]) > 0) {
		    VehicleInfo[CorpInfo[id][cVeh]][vCorp]=0;
		} else { DestroyDynamic3DTextLabel(CorpInfo[id][cText]); }
        
        for(new i; i < GetPlayerPoolSize(); i++) {
            if (pTemp[i][UsingCorpse] == id) {
                pTemp[i][UsingCorpse] = 0;
                break;
            }
        }
	}
	return 1;
}