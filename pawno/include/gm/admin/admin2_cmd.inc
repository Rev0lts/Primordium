CMD:up(playerid, params[])
{
	new amount, Float:x, Float:y, Float:z;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", amount)) return SendHelpMessage(playerid, "/up [blocos]");

	GetPlayerPos(playerid, x, y, z);
	SetPlayerPosEx(playerid, x, y, z + amount);
	return 1;
}

CMD:down(playerid, params[])
{
	new amount, Float:x, Float:y, Float:z;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", amount)) return SendHelpMessage(playerid, "/down [blocos]");

	GetPlayerPos(playerid, x, y, z);
	SetPlayerPosEx(playerid, x, y, z - amount);
	return 1;
}

CMD:policer(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2))
		return SendErrorMessage(playerid, "Você não tem direitos suficientes para usar o comando.");

	// SCMALL(COLOR_LIGHTBLUE, "<< O ADMINISTRADOR PRODUZIU O RESPAWN DE VEÍCULOS DE POLÍCIA >>");
	AMSG(COLOR_LIGHTRED, "AdmCmd: %s respawnou os veículos de polícia.", GetAccName(playerid));

	new vehicle,
	    ocupado[MAX_VEHICLES];
	    
    foreach(new i : Player)
	{
	    if ((vehicle = GetPlayerVehicleID(i)) == 0 || !IsFactionPolice(VehicleInfo[vehicle][carFaction]))
	        continue;
	        
		ocupado[vehicle] = 1;
	}
	
	foreach(new i : Vehicle)
	{
		if (!IsFactionPolice(VehicleInfo[vehicle][carFaction]) || ocupado[i] == 1)
			continue;

		RespawnVehicle(i);
	}
	return 1;
}

CMD:getincar(playerid, params[])
{
	new vehicle;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", vehicle)) return SendHelpMessage(playerid, "/getincar [ID do Veículo]");
	if (!IsValidVehicle(vehicle)) return SendErrorMessage(playerid, "Veículo com este ID não foi encontrado.");

	PutPlayerInVehicle(playerid, vehicle, 0);
	SendAdmMessage("%s teleportado para o veículo [ID: %i].", GetAccName(playerid), vehicle);
	return 1;
}

CMD:afire(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	new option[16];

	if (sscanf(params, "s[16]", option))
	{
		SendHelpMessage(playerid, "/afire [criar / removertodos]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		if (GetPVarInt(playerid, #crt_fire) == 0)
		{
			if (IsPlayerHoldingWeapon(playerid)) return SendErrorMessage(playerid, "Guarde a arma no inventário!");

			SetPVarInt(playerid, #crt_fire, 1);

  			GivePlayerWeapon(playerid, 24, 500);

			SendSyntaxMessage(playerid, "Você entrou no modo de criação de incêndios.");
			SendSyntaxMessage(playerid, "Atire onde deseja colocar o fogo.");
			SendAdmMessage("%s entrou no modo de criação de incêndios.", GetAccName(playerid));
			return 1;
		}

		SendAdmMessageToPlayer(playerid, "Você saiu do modo de criação de incêndios. Para remover todos os incêndios, use /afire removertodos.");
		SendAdmMessage("%s saiu do modo de criação de incêndios.", GetAccName(playerid));
		
		ResetPlayerWeapons(playerid);
		DeletePVar(playerid, #crt_fire);
	 	return 1;
	}

	else if (CompareStrings(option, "removertodos"))
	{
		for(new i; i != MAX_FIRES; i++)
		{
		    if (FiresInfo[i][fireOn] == 0) continue;

			DeleteFire(i);
		}
		SendAdmMessage("%s removeu todos os incêndios do servidor.", GetAccName(playerid));
	}

	else 
	{
		SendHelpMessage(playerid, "/afire [criar/removertodos]");
	}
	return 1;
}

CMD:aengine(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (!IsPlayerInAnyVehicle(playerid)) return SendNotInVehicle(playerid);

	new vehicleid = GetPlayerVehicleID(playerid);

	ToggleVehicleEngine(vehicleid, playerid);
	return 1;
}

CMD:fillveh(playerid, params[])
{
	new vehicle;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", vehicle)) return SendHelpMessage(playerid, "/fillveh [ID do Veículo]");
	if (!IsValidVehicle(vehicle)) return SendErrorMessage(playerid, "Nenhum veículo com este ID foi encontrado.");

    VehicleInfo[vehicle][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(vehicle));
	SendAdmMessageToPlayer(playerid, "Você abasteceu o veículo ID: %i.", vehicle);   
	return 1;
}

CMD:fillallveh(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	foreach(new i : Vehicle)
	{
        VehicleInfo[i][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(i));
	}

	SendAdmMessageToAll("Todos os veículos no servidor foram abastecidos pelo administrador %s.", GetAccName(playerid));   
	return 1;
}

CMD:rtc(playerid, params[])
{
	new vehicle;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", vehicle)) return SendHelpMessage(playerid, "/rtc [ID do Veículo]");
    if (!IsValidVehicle(vehicle)) return SendErrorMessage(playerid, "Nenhum veículo com este ID foi encontrado.");
	if (!VehicleEmpty(vehicle)) return SendErrorMessage(playerid, "Este veículo está em uso.");

	if (VehicleInfo[vehicle][carOwnerID] != 0)
	{
	    new userid = GetVehicleOwner(vehicle);

	    if (userid != INVALID_PLAYER_ID)
	    {
	    	SendAdmMessageToPlayer(userid, "O administrador %s estacionou o seu %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(vehicle)-400]);
			PlayerInfo[userid][pCarKey] = 0;
			VehicleInfo[vehicle][carOwnerPlayer] = INVALID_PLAYER_ID;
		}
		ResetVehicleStuff(vehicle, 1);
		DestroyVehicle(vehicle);
	}
    else
	{
		SendAdmMessageToPlayer(playerid, "Você estacionou o veículo %s [ID: %i].", VehicleNames[GetVehicleModel(vehicle)-400], vehicle);
		RespawnVehicle(vehicle);
	}
	return 1;
}

CMD:rtcall(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	SendAdmMessage("%s estacionou todos os veículos não utilizados no servidor.", GetAccName(playerid));

    new vehicle, occupied[MAX_VEHICLES];

    foreach(new i : Player)
	{
	    if ((vehicle = GetPlayerVehicleID(i)) == 0) continue;

		occupied{vehicle} = 1;
	}

	for(new i = 1; i < MAX_VEHICLES; i++)
	{
		if (!IsValidVehicle(i) || occupied{i} == 1) continue;

		if (VehicleInfo[i][carOwnerID] != 0)
		{
		    new userid = GetVehicleOwner(i);
		    if (userid != INVALID_PLAYER_ID)
		    {
		    	SendSyntaxMessage(userid, "O administrador %s estacionou o seu %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(i)-400]);
				PlayerInfo[userid][pCarKey] = 0;
				VehicleInfo[i][carOwnerPlayer] = INVALID_PLAYER_ID;
			}
			ResetVehicleStuff(i, 1);
			DestroyVehicle(i);
		}
	    else RespawnVehicle(i);
	}
	return 1;
}

CMD:amark(playerid, params[])
{
	new option[16], string[145];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendErrorMessage(playerid, "Você não tem permissão para usar este comando.");
	if (sscanf(params, "s[16]S()[144]", option, string))
	{
		SendHelpMessage(playerid, "/amark [destroy/info/goto]");
		return 1;
	}

	if (CompareStrings(option, "destroy"))
	{
		new markid;
		if (sscanf(string, "i", markid)) return SendHelpMessage(playerid, "/amark destroy [ID de tags]");
		if (markid < 0 || markid >= MAX_MARKS || !E_MARKS[markid][mON]) return SendErrorMessage(playerid, "Marca com este ID não existe.");

		DestroyDynamicMark(markid);
		SendSyntaxMessage(playerid, "Você removeu com sucesso a etiqueta [ID: %i].", markid);
		return 1;
	}
	else if (CompareStrings(option, "info"))
	{
		new markid = Mark_Nearest(playerid);

		if (markid == -1) return SendErrorMessage(playerid, "Tags com este ID não existem.");

		new mark_info[512];
		mark_info[0] = EOS;

		format(mark_info, sizeof(mark_info), "ID da marca\t%i\nCriador\t%s\nData de Criação\t%s", markid, E_MARKS[markid][mCreator], E_MARKS[markid][mCreateDate]);
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST, "{FFFFFF}Informações da Marca", mark_info, "Fechar", "");
		return 1;
	}
	else if (CompareStrings(option, "goto"))
	{
		new markid;
		if (sscanf(string, "i", markid)) return SendHelpMessage(playerid, "/amark goto [ID da marca]");
		if (markid < 0 || markid > MAX_MARKS || !E_MARKS[markid][mON]) return SendErrorMessage(playerid, "Não existe marca com este ID.");

		SetPlayerPosEx(playerid, E_MARKS[markid][mPosX], E_MARKS[markid][mPosY], E_MARKS[markid][mPosZ]);
		SetInterior(playerid, E_MARKS[markid][mInt]);
		SetWorld(playerid, E_MARKS[markid][mVW]);
	}
	else
	{
		SendHelpMessage(playerid, "/amark [destroy/info/goto]");
	}
	return 1;
}

CMD:sethp(playerid, params[])
{
	new userid, hp;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "ud", userid, hp)) return SendHelpMessage(playerid, "/sethp [ID/Parte do Nome] [vida (1-100)]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (PlayerInfo[playerid][pInjured]) return SendErrorMessage(playerid, "O jogador está em estado de ferimento/morto. Use /aheal");
	if (hp < 1 || hp > 100) return SendErrorMessage(playerid, "Valor de vida de 1 a 100.");

	SendAdmMessageToPlayer(userid, "O administrador %s definiu %i de vida para você.", GetAccName(playerid), hp);
	SendAdmMessageToPlayer(playerid, "Você definiu %s com %i de vida.", GetNameEx(userid), hp);
	SetPlayerHealth(userid, hp);
	return 1;
}

CMD:setarmour(playerid, params[])
{
	new userid, armour;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "ud", userid, armour)) return SendHelpMessage(playerid, "/setarmour [ID/Parte do Nome] [colete (1-100)]");
	if (armour < 1 || armour > 100) return SendErrorMessage(playerid, "Colete (1-100)");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);

	SendAdmMessageToPlayer(userid, "O administrador %s definiu %i de colete para você.", GetAccName(playerid), armour);
	SendAdmMessageToPlayer(playerid, "Você definiu a %s %i de colete.", GetNameEx(userid), armour);
    SetPlayerArmour(userid, armour);
	return 1;
}

CMD:setskin(playerid, params[])
{
	new targetid, skinid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "ui", targetid, skinid)) return SendHelpMessage(playerid, "/setskin [ID/Parte do Nome] [ID da Skin (0 - 311)]");
	if (!IsPlayerLogged(targetid)) return SendNotConnected(playerid);
	if (skinid < 1 || skinid == 74 || skinid > 311) return SendErrorMessage(playerid, "ID da Skin de 1 a 311.");

	SetPlayerSkinEx(targetid, skinid);
	SendAdmMessageToPlayer(targetid, "Administrador %s alterou sua skin [ID: %i].", GetAccName(playerid), skinid);
	SendAdmMessageToPlayer(playerid, "Você alterou a skin de %s [ID: %i].", GetNameEx(targetid), skinid);
	return 1;
}

CMD:showpm(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/showpm [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);

    pTemp[playerid][pPMPlayer] = userid;
	SendSyntaxMessage(playerid, "Você está visualizando o PM de %s. Digite /showpmoff para desativar.", GetNameEx(userid));
	return 1;
}

CMD:showpmoff(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (pTemp[playerid][pPMPlayer] == INVALID_PLAYER_ID) return SendErrorMessage(playerid, "Você não está visualizando o PM.");

	SendSyntaxMessage(playerid, "Você parou de visualizar o PM de %s.", GetNameEx(pTemp[playerid][pPMPlayer]));
	pTemp[playerid][pPMPlayer] = INVALID_PLAYER_ID;
	return 1;
}

CMD:showpms(playerid, params[]) 
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	SCM(playerid, COLOR_WHITE, "Você está visualizando o PM de todo o servidor, \"/nopms\" para parar.");
	SetPVarInt(playerid, #spypm_all, 1);
	return 1;
}

CMD:nopms(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2))
		return SendNotAdmin(playerid);

	if (GetPVarInt(playerid, #spypm_all) == 0)
		return SCM(playerid, COLOR_LIGHTRED, "Você não está visualizando o PM.");

	SCM(playerid, COLOR_WHITE, "Você parou de visualizar o PM.");
	DeletePVar(playerid, #spypm_all);
	return 1;
}

CMD:telecmds(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	SCM(playerid, COLOR_WHITE, "[Nível 3]: {C3C3C3}/atp /maketele /makehouseint /removetele");
	return 1;
}

ALTX:makehouseint("/mhouseint");
CMD:makehouseint(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	new mapname[32],
		type;

	if (sscanf(params, "is[32]", type, mapname))
		return SendHelpMessage(playerid, "/makehouseint [Tipo da casa: 1-pequena, 2-média, 3-grande] [Nome do interior]");

	if (type < 1 || type > 3)
	    return SendErrorMessage(playerid, "Tipo de casa: 1-pequena, 2-média, 3-grande.");

    if (strlen(mapname) < 1 || strlen(mapname) > 32)
	    return SendErrorMessage(playerid, "Caracteres de 1 a 32.");

	new id = -1;
	for(new i = 0; i < MAX_ADMIN_TELES; i++)
	{
	    if (0 != AdminTeles[i][telesOn])
			continue;

		id = i;
		break;
	}

	if (id == -1)
	    return SendErrorMessage(playerid, "Limite de teleportes atingido.");

	GetPlayerPos(playerid, AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ]);
	AdminTeles[id][telesInt] = GetInterior(playerid);
	AdminTeles[id][telesType] = type-1;
	format(AdminTeles[id][telesMapName], 32, "%s", mapname);
    AdminTeles[id][telesOn] = 1;
    
	AddTeleToFile(playerid, id, mapname, AdminTeles[id][telesInt], AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ], type-1);
 	SendClientMessageEx(playerid,COLOR_GREEN,"Novo interior (%s) ID: %i estabelecido.", AdminTeles[id][telesMapName], id);
	return 1;
}

CMD:delplayertickets(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/delplayertickets [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);

	DeletePlayerTickets(userid);

	SendAdmMessageToPlayer(playerid, "Você limpou todas as penalidades do jogador - %s.", GetNameEx(userid));
	SendAdmMessageToPlayer(userid, "O administrador %s limpou todas as multas para você.", GetAccName(playerid));
	
	return 1;
}

CMD:delvehtickets(playerid, params[])
{
	new vehid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "i", vehid)) return SendHelpMessage(playerid, "/delvehtickets [ID do Veículo]");
	
	DeleteVehicleTickets(vehid);

	SendAdmMessageToPlayer(playerid, "Você limpou todas as multas - %s [ID:%i].", VehicleNames[VehicleInfo[vehid][carModel]-400], vehid);

	return 1;
}

CMD:aunimpound(playerid, params[])
{
	new vehicle;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "i", vehicle)) return SendHelpMessage(playerid, "/aunimpound [ID do Veículo]");

	if (!IsValidVehicle(vehicle)) return SendErrorMessage(playerid, "O veículo não está spawnado.");
	if (!VehicleInfo[vehicle][Impounded]) return SendErrorMessage(playerid, "O veículo não foi confiscado.");

	VehicleInfo[vehicle][Impounded] = 0;
	Save_Car(vehicle);

	SendAdmMessageToPlayer(playerid, "O veículo [ID %i], cujo dono é - %s, não está mais confiscado.", vehicle, GetNameEx(VehicleInfo[vehicle][carOwnerID]));
	SendAdmMessageToPlayer(VehicleInfo[vehicle][carOwnerID], "O administrador %s liberou seu %s da penalidade de confisco.", GetAccName(playerid), VehicleNames[VehicleInfo[vehicle][carModel]-400]);
	return 1;
}

ALTX:makebizint("/mbizint");
CMD:makebizint(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	new mapname[32];

	if (sscanf(params, "s[32]", mapname))
		return SendHelpMessage(playerid, "/makebizint [Nome do Interior]");

	if (strlen(mapname) < 1 || strlen(mapname) > 32)
	    return SendErrorMessage(playerid, "Caracteres de 1 a 32.");

	new id = -1;
	for(new i = 0; i < MAX_ADMIN_TELES; i++)
	{
	    if (0 != AdminTeles[i][telesOn])
			continue;

		id = i;
		break;
	}

	if (id == -1)
	    return SendErrorMessage(playerid, "Limite de teleportes atingido.");

	GetPlayerPos(playerid, AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ]);
	AdminTeles[id][telesInt] = GetInterior(playerid);
	AdminTeles[id][telesType] = BUSSINESS_INT_TYPE;
	format(AdminTeles[id][telesMapName], 32, "%s", mapname);
    AdminTeles[id][telesOn] = 1;

	AddTeleToFile(playerid, id, mapname, AdminTeles[id][telesInt], AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ], 4);
 	SendClientMessageEx(playerid,COLOR_GREEN,"Novo interior (%s) ID: %i estabelecido.", AdminTeles[id][telesMapName], id);
	return 1;
}

ALTX:maketele("/mtele");
CMD:maketele(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	new mapname[32];

	if (sscanf(params, "s[32]", mapname)) return SendHelpMessage(playerid, "/maketele [Nome do Interior]");
	if (strlen(mapname) < 1 || strlen(mapname) > 32) return SendErrorMessage(playerid, "Caracteres de 1 a 32.");

	new id = -1;

	for(new i = 0; i < MAX_INTERIORS; i++)
	{
	    if (AdminTeles[i][telesOn]) continue;

		id = i;
		break;
	}

	if (id == -1) return SendErrorMessage(playerid, "Limite de teleportes atingido.");

	format(AdminTeles[id][telesMapName], 32, "%s", mapname);
	GetPlayerPos(playerid, AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ]);
	AdminTeles[id][telesInt] = GetInterior(playerid);
	AdminTeles[id][telesType] = 3;
	AdminTeles[id][telesOn] = 1;

	AddTeleToFile(playerid, id, mapname, AdminTeles[id][telesInt], AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ], 3);
 	SendAdmMessageToPlayer(playerid, "Novo teletransporte (%s) ID: %i estabelecido.", AdminTeles[id][telesMapName], id);
	return 1;
}

ALTX:removetele("/rtele");
CMD:removetele(playerid,params[])
{
	new teleid;
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	if (sscanf(params, "d", teleid))
		return SendHelpMessage(playerid, "/removetele [ID do Teletransporte]");

	if (AdminTeles[teleid][telesOn] == 0)
	    return SendErrorMessage(playerid, "Teletransporte com este ID não existe.");

    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `ateles` WHERE `id` = %i", AdminTeles[teleid][aID]);
	mysql_tquery(dbHandle, query);

	SendAdmMessageToPlayer(playerid, "Você removeu o teletransporte ID %i.", teleid);
	
	AdminTeles[teleid][telesOn] = 0;
	AdminTeles[teleid][telesType] = 0;
	return 1;
}

CMD:aveh(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	new model, color1, color2, Float:x, Float:y, Float:z, Float:a, Float:distance = 5.0;

	if (sscanf(params, "iI(0)I(0)", model, color1, color2)) return SendHelpMessage(playerid, "/aveh [modelo] {c7c7c7}[cor1] [cor2]");
	if (model == 520 || model == 425 || model == 432 || model == 447 || model == 476) return SendErrorMessage(playerid, "Este tipo de veículo é proibido no servidor.");
	if (model < 400 || model > 611) return SendErrorMessage(playerid, "Modelo de veículo de 400 a 611.");
	if (color1 > 255 || color1 < 0 || color2 > 255 || color2 < 0) return SendErrorMessage(playerid, "Cor: de 0 a 255.");

	GetPlayerPos(playerid, x, y ,z);
	GetPlayerFacingAngle(playerid, a);
  	x += (distance * floatsin(-a, degrees));
    y += (distance * floatcos(-a, degrees));
	a += 90.0;

	if (a > 360.0) a -= 360.0;

	new vehicle = CreateVehicle(model, x, y, z, a, color1, color2, -1);

	if (vehicle == INVALID_VEHICLE_ID) return SendErrorMessage(playerid, "O veículo não foi criado.");

	SetVehicleHealth(vehicle, 1000.0);
	SetVehicleNumberPlate(vehicle, GetVehicleNumber());
	SetVehicleVirtualWorld(vehicle, GetWorld(playerid));
	LinkVehicleToInterior(vehicle, GetInterior(playerid));
    VehicleInfo[vehicle][carDriver] = INVALID_PLAYER_ID;
	VehicleInfo[vehicle][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(vehicle));
	VehicleInfo[vehicle][carAdmin] = 1;
	VehicleInfo[vehicle][carColor1] = color1;
	VehicleInfo[vehicle][carColor2] = color2;
	VehicleInfo[vehicle][carType] = 0;

	SendAdmMessageToPlayer(playerid, "Você criou um(a) %s [ID: %i | MODELO: %i].", VehicleNames[model-400], vehicle, model);
	return 1;
}

CMD:makemark(playerid, params[])
{
	new Float:x, Float:y, Float:z, interior, world;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	
	interior = GetInterior(playerid);
	world = GetWorld(playerid);
	GetPlayerPos(playerid, x, y, z);
	SetPVarInt(playerid, "PontoMarcado", 1);
	SetPVarFloat(playerid, "MarkX", x);
	SetPVarFloat(playerid, "MarkY", y);
	SetPVarFloat(playerid, "MarkZ", z);
	SetPVarInt(playerid, "MarkInterior", interior);
	SetPVarInt(playerid, "MarkWorld", world);
	SendAdmMessageToPlayer(playerid, "Ponto definido, \"/gotomark\" para teleportar.");
	return 1;
}

CMD:gotomark(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (GetPVarInt(playerid, "PontoMarcado") == 0) return SendErrorMessage(playerid, "O ponto não está definido. Use \"/makemark\".");
	
	SetPlayerPosEx(playerid, GetPVarFloat(playerid, "MarkX"), GetPVarFloat(playerid, "MarkY"), GetPVarFloat(playerid, "MarkZ"));
	SetWorld(playerid, GetPVarInt(playerid, "MarkWorld"));
	SetInterior(playerid, GetPVarInt(playerid, "MarkInterior"));
	SCM(playerid, COLOR_GREY, "* Você foi teleportado.");
	return 1;
}

CMD:settime(playerid, params[])
{
	new time;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", time)) return SendHelpMessage(playerid, "/settime [tempo (0-23)]");
	if (time < 0 || time > 23) return SendErrorMessage(playerid, "Tempo de 0 a 23.");

	SetWorldTime(time);
	SAMALL("%s definiu o tempo para %i:00.", GetAccName(playerid), time);
	return 1;
}
CMD:weather(playerid, params[])
{
	new weather;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", weather)) return SendHelpMessage(playerid, "/weather [clima]");
	if (weather < 0 || weather > 45) return SendErrorMessage(playerid, "Clima de 0 a 45.");

	SetPlayerWeather(playerid, weather);
	return 1;
}

CMD:weatherall(playerid, params[])
{
	new weather;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", weather)) return SendHelpMessage(playerid, "/weatherall [clima]");
	if (weather < 0 || weather > 45) return SendErrorMessage(playerid, "Clima de 0 a 45.");

    server_weather = weather;
	SetWeather(server_weather);

	SAMALL("%s definiu o clima para todos [ID %i].", GetAccName(playerid), weather);
	return 1;
}

CMD:govw(playerid, params[])
{
	new vw;
	
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "i", vw)) return SendHelpMessage(playerid, "/govw [mundo virtual (0-65535)]");
	if (vw < 0 || vw > 65535) return SendErrorMessage(playerid, "Mundo virtual de 0 a 65535.");

	SetWorld(playerid, vw);
	SendSyntaxMessage(playerid, "Você alterou seu mundo virtual para %i.", vw);
	return 1;
}

CMD:goint(playerid, params[])
{
	new interior;
	
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params, "i", interior)) return SendHelpMessage(playerid, "/goint [interior (0-255)]");
	if (interior < 0 || interior > 255) return SendErrorMessage(playerid, "Interior de 0 a 255.");

	SetInterior(playerid, interior);
	SendSyntaxMessage(playerid, "Você alterou seu interior para %i.", interior);
	return 1;
}

CMD:go(playerid, params[])
{
	new Float:x, Float:y, Float:z, interior, vw;
	
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params,"fffI(0)I(0)", x, y ,z, interior, vw)) return SendHelpMessage(playerid, "/go [x] [y] [z] {c7c7c7}[interior (0-255)] [vw (0-65535)]");
	if (interior < 0 || interior > 255) return SendErrorMessage(playerid, "Interior de 0 a 255.");
	if (vw < 0 || vw > 65535) return SendErrorMessage(playerid, "VW de 0 a 65535.");

	SetPlayerPosEx(playerid, x, y, z);
	SetInterior(playerid, interior);
 	SetWorld(playerid, vw);
	return 1;
}

CMD:x(playerid, params[])
{
	new Float: coord;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params,"f", coord)) return SendHelpMessage(playerid, "/x [+coordenada x]");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPosEx(playerid, x + coord, y, z);
	return 1;
}

CMD:y(playerid, params[])
{
	new Float: coord;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params,"f", coord)) return SendHelpMessage(playerid, "/y [+coordenada y]");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPosEx(playerid, x, y + coord, z);
	return 1;
}

CMD:z(playerid, params[])
{
	new Float: coord;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
	if (sscanf(params,"f", coord)) return SendHelpMessage(playerid, "/z  [+coordenada z]");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPosEx(playerid, x, y, z + coord);
	return 1;
}

CMD:noooc(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

	noooc = !noooc;
	if (noooc == false) SCMALL(COLOR_GRAD2, "O chat OOC está ativado pelo administrador.");
	else SCMALL(COLOR_GRAD2, "O chat OOC está desativado pelo administrador.");
	return 1;
}
