CMD:afaction(playerid, params[])
{
	new option[16], secoption[128];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/afacção [criar / remover / editar / listar / líder]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new factionid = -1, name[32], shortname[8], type;

		if (sscanf(secoption, "is[8]S()[32]", type, shortname, name))
		{
			SendHelpMessage(playerid, "/afacção criar [um tipo] [redução] [nome]");
			SendSyntaxMessage(playerid, "Tipos: [1] Estrutura estatal | [2] Polícia | [3] Médicos | [4] Bombeiros | [5] Gangue | [6] Máfia | [7] Civil");
			return 1;
		}

		for(new i = 1; i < MAX_FACTIONS; i++)
		{
			if (Factions[i][fON] != 0) continue;

			factionid = i;
			break;
		}

		if (factionid == -1) 						return SendErrorMessage(playerid, "O número máximo de facções foi atingido.");
		if (strlen(name) < 1 || strlen(name) > 30)	return SendErrorMessage(playerid, "O nome da facção pode ter até 30 caracteres.");
		if (type < 1 || type > 9)					return SendErrorMessage(playerid, "Tipo de facção de 1 a 9.");

		format(Factions[factionid][fName], 32, "%s", name);
		format(Factions[factionid][fShortName], 8, "%s", shortname);
		Factions[factionid][fColor] = 0xFFFFFF00;
		Factions[factionid][fType] = type;
		Factions[factionid][fRanks] = 5;
		Factions[factionid][fChatON] = 1;
		Factions[factionid][fON] = 1;
		Factions[factionid][fJoinRank] = 1;
		Factions[factionid][fPointTime] = 604800 + gettime();

		if (type == FACTION_MAFIA)	Factions[factionid][fPoint] = 50;
		else 						Factions[factionid][fPoint] = 30;

		for(new i; i != 20; i++)
		{
			FactionRanks[factionid][i] = "Nenhum";
		}

		AddFactionToFile(factionid, shortname, name, Factions[factionid][fColor], type, 5, Factions[factionid][fPointTime]);
		SendAdmMessage("O administrador %s criou uma facção %s [ID: %i].", GetAccName(playerid), name, factionid);
		SendHelpMessage(playerid, "Digite '/afacção editar' para editar uma facção.");

		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new id;

		if (sscanf(secoption, "i", id))							 return SendHelpMessage(playerid, "/afaction remover [ID de facção]");
		if (id < 1 || id > MAX_FACTIONS-1 || !Factions[id][fON]) return SendErrorMessage(playerid, "Facção não encontrada.");

		if (IsValidDynamicPickup(Factions[id][fPickUp])) DestroyDynamicPickup(Factions[id][fPickUp]);

		new query[256];

		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `factions` WHERE `id` = %i", Factions[id][fID]);
		mysql_tquery(dbHandle, query);
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `char`='0',`swat`='0',`swat_duty`='0',`duty`='0',`faction`='0',`rank`='0',`sidejob`='0',`spawntype`=0,`spawnpoint`=1 WHERE `faction`=%i", id);
		mysql_tquery(dbHandle, query);

		foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || PlayerInfo[i][pFaction] != id) continue;

			RemovePlayerFaction(i);
		}

		SendAdmMessageToPlayer(playerid, "Você removeu com sucesso uma facção %s [ID: %i].", Factions[id][fName], id);
		Factions[id][fON] = 0;
		Factions[id][fID] = 0;
	}


	else if (CompareStrings(option, "editar"))
	{
		new id, parameters[12], arg[36];

		if (sscanf(secoption, "is[12]S()[36]", id, parameters, arg))
		{
			SendHelpMessage(playerid, "/afaction editar [ID de facção] [opções] [valor]");
			SendSyntaxMessage(playerid, "spawn / cor / nome / cargos / cargoinicial / nomecargo / tipo / uniforme / consertodecarro");
			return 1;
		}
		if (id < 1 || id > MAX_FACTIONS-1 || !Factions[id][fON]) return SendErrorMessage(playerid, "Facção não encontrada.");

		if (CompareStrings(parameters, "spawn"))
		{
			GetPlayerPos(playerid, Factions[id][fPosX], Factions[id][fPosY], Factions[id][fPosZ]);
			Factions[id][fSpawnInt] = GetInterior(playerid);
			Factions[id][fSpawnVW] = GetWorld(playerid);

			new query[128 + 56];

			mysql_format(dbHandle, query, sizeof(query), "UPDATE `factions` SET `spawnx` = %f, `spawny` = %f, `spawnz` = %f, `spawnint` = %i, `spawnvw` = %i WHERE `id` = %i",
			Factions[id][fPosX], Factions[id][fPosY], Factions[id][fPosZ], Factions[id][fSpawnInt], Factions[id][fSpawnVW], Factions[id][fID]);
			mysql_tquery(dbHandle, query);

			SendAdmMessageToPlayer(playerid, "Você definiu a posição de spawn da facção %s [ID: %i] para este local.", Factions[id][fName], id);
			return 1;
		}

		else if (CompareStrings(parameters, "color"))
		{
			new color;
			if (sscanf(arg, "h", color)) return SendHelpMessage(playerid, "/afacção editar [ID da facção] cor [Cor HEX]");

			Factions[id][fColor] = color;
			SQL_SetInteger("factions", "color", Factions[id][fColor], Factions[id][fID]);
			SendAdmMessageToPlayer(playerid, "Você alterou com sucesso a cor da sua facção para {%06x}%s{FF6347} [ID: %i].", color >>> 8, Factions[id][fName], Factions[id][fID]);
			return 1;
		}

		else if (CompareStrings(parameters, "nome"))
		{			
			new factioname[32], shortname[8];

			if (sscanf(arg, "s[8]S()[32]", shortname, factioname)) 	return SendHelpMessage(playerid, "/afacção editar [ID da facção] nome [nova redução] [novo nome]");
			if (isnull(factioname) || strlen(factioname) > 32) 		return SendErrorMessage(playerid, "Nome da facção, com no máximo 32 caracteres.");

			SendAdmMessageToPlayer(playerid, "Você alterou o nome da sua facção de %s [ID: %i] para %s [%s].", Factions[id][fName], id, factioname, shortname);
			
			format(Factions[id][fName], 32, "%s", factioname);
			format(Factions[id][fShortName], 8, "%s", shortname);

			SQL_SetString("factions", "name", Factions[id][fName], Factions[id][fID]);
			SQL_SetString("factions", "shortname", Factions[id][fShortName], Factions[id][fID]);
			return 1;
		}

		else if (CompareStrings(parameters, "cargo"))
		{
			if (isnull(arg))							return SendHelpMessage(playerid, "/afacção editar [ID da facção] classificações [número de classificações]");
			if (strval(arg) < 1 || strval(arg) > 20)	return SendErrorMessage(playerid, "Número de classificações de 1 a 20.");
			Factions[id][fRanks] = strval(arg);
			SQL_SetInteger("factions", "ranks", Factions[id][fRanks], Factions[id][fID]);
			SendAdmMessageToPlayer(playerid, "Você alterou o número de classificações para a facção %s [ID: %i] para %i.", Factions[id][fName], id, strval(arg));
			return 1;
		}

		else if (CompareStrings(parameters, "tipo"))
		{
			if (isnull(arg) ||  strval(arg) < 1 || strval(arg) > 7)
			{
				SendHelpMessage(playerid, "/afaction edit [ID da facção] tipo [tipo da facção]");
				SendSyntaxMessage(playerid, "Tipos: [1] Governamentais [2] Polícia | [3] Médicos | [4] Bombeiros | [5] Gangue | [6] Máfia | [7] Civil");
				return 1;
			}

			new type_fr[4][32] = {"Governamentais", "Gangue", "Máfia", "Civis"};

			Factions[id][fType] = strval(arg);
			SQL_SetInteger("factions", "type", Factions[id][fType], Factions[id][fID]);
			SendAdmMessageToPlayer(playerid, "Você alterou o tipo de facção %s [ID: %i] para %s [%i].", Factions[id][fName], id, type_fr[strval(arg)-1], strval(arg));

			if (IsFactionLegal(id))
			{
				Factions[id][fPickUp] = CreateDynamicPickup(1239, 1, Factions[id][fUniformX], Factions[id][fUniformY], Factions[id][fUniformZ], Factions[id][fUniformVW], Factions[id][fUniformInt], -1, 100.0);
			}
			else
			{
				if (IsValidDynamicPickup(Factions[id][fPickUp])) DestroyDynamicPickup(Factions[id][fPickUp]);
			}
			return 1;
		}

		else if (CompareStrings(parameters, "cargoinicial"))
		{
			if (isnull(arg))												return SendHelpMessage(playerid, "/afaction editar [ID da facção] joinrank [classificação na admissão]");
			if (strval(arg) < 1 || strval(arg) > Factions[id][fRanks]-1)	return SendErrorMessage(playerid, "Classificação de 1 a %i.", Factions[id][fRanks]-1);

			Factions[id][fJoinRank] = strval(arg);
			SQL_SetInteger("factions", "joinrank", Factions[id][fJoinRank], Factions[id][fID]);
			SendAdmMessageToPlayer(playerid, "Você alterou a classificação na admissão da facção %s [ID: %i] para %i.", Factions[id][fName], id, strval(arg));
			return 1;
		}

		else if (CompareStrings(parameters, "uniforme"))
		{
			if (!IsFactionLegal(id)) return SendErrorMessage(playerid, "O parâmetro está disponível apenas para governamentais.");

			GetPlayerPos(playerid, Factions[id][fUniformX], Factions[id][fUniformY], Factions[id][fUniformZ]);
			Factions[id][fUniformInt] = GetInterior(playerid);
			Factions[id][fUniformVW] = GetWorld(playerid);
			SQL_SetFloat("factions", "uniform_x", Factions[id][fUniformX], Factions[id][fID]);
			SQL_SetFloat("factions", "uniform_y", Factions[id][fUniformY], Factions[id][fID]);
			SQL_SetFloat("factions", "uniform_z", Factions[id][fUniformZ], Factions[id][fID]);
			SQL_SetInteger("factions", "uniform_int", Factions[id][fUniformInt], Factions[id][fID]);
			SQL_SetInteger("factions", "uniform_vw", Factions[id][fUniformVW], Factions[id][fID]);

			if (IsValidDynamicPickup(Factions[id][fPickUp]))	DestroyDynamicPickup(Factions[id][fPickUp]);
			Factions[id][fPickUp] = CreateDynamicPickup(1239, 1, Factions[id][fUniformX], Factions[id][fUniformY], Factions[id][fUniformZ], Factions[id][fUniformVW], Factions[id][fUniformInt], -1, 100.0);
			SendAdmMessageToPlayer(playerid, "Você alterou a posição da farda da facção %s [ID: %i].", Factions[id][fName], id);
			return 1;
		}
		else if (CompareStrings(parameters, "carfix"))
		{
			if (!IsFactionLegal(id)) return SendErrorMessage(playerid, "O parâmetro está disponível apenas para governamentais.");

			GetPlayerPos(playerid, Factions[id][Carfix][fixX], Factions[id][Carfix][fixY], Factions[id][Carfix][fixZ]);
			Factions[id][Carfix][fixInt] = GetInterior(playerid);
			Factions[id][Carfix][fixVW] = GetWorld(playerid);

			SQL_SetFloat("factions", "fix_X", Factions[id][Carfix][fixX], Factions[id][fID]);
			SQL_SetFloat("factions", "fix_Y", Factions[id][Carfix][fixY], Factions[id][fID]);
			SQL_SetFloat("factions", "fix_Z", Factions[id][Carfix][fixZ], Factions[id][fID]);
			SQL_SetInteger("factions", "fix_Int", Factions[id][Carfix][fixInt], Factions[id][fID]);
			SQL_SetInteger("factions", "fix_VW", Factions[id][Carfix][fixVW], Factions[id][fID]);

			if (IsValidDynamicPickup(Factions[id][Carfix][fixPickUp])) DestroyDynamicPickup(Factions[id][Carfix][fixPickUp]);

			Factions[id][Carfix][fixPickUp] = CreateDynamicPickup(1650, 1, Factions[id][Carfix][fixX], Factions[id][Carfix][fixY], Factions[id][Carfix][fixZ], Factions[id][Carfix][fixVW], Factions[id][Carfix][fixInt], -1, 20.0);
			SendAdmMessageToPlayer(playerid, "Você alterou a posição das correcões de veículo da facção %s [ID: %i].", Factions[id][fName], id);
			return 1;
		}

		else if (CompareStrings(parameters, "nomecargo"))
		{
			new rank, name[32];

			if (sscanf(arg, "is[32]", rank, name)) return SendHelpMessage(playerid, "/afaction edit [ID factions] rankname [rank] [name]");
			if (rank < 1 || rank > Factions[id][fRanks]) return SendSyntaxMessage(playerid, "Rank de 1 a %i.", Factions[id][fRanks]);
			if (strlen(name) > 30) return SendSyntaxMessage(playerid, "Nome do cargo com até 30 caracteres.");


			format(FactionRanks[id][rank-1], 32, "%s", name);
			UpdateFactionRank(id, rank);
			SendAdmMessageToPlayer(playerid, "Você alterou o cargo %i da facção %s [ID: %i] para %s.", rank, Factions[id][fName], id, name);
			return 1;
		}

		else 
		{
			SendHelpMessage(playerid, "/afaction edit [ID de facções] [spawn/cor/nome/cargos/cargoinicial/nomenocargo/tipo/uniforme/carfix] [valor]");
			return 1;
		}
	}

	else if (CompareStrings(option, "lista"))
	{
		new faction_type[9][10] = {"GOV", "POLÍCIA", "HOSPITAL", "BOMBEIROS", "MÉDICOS", "NOTÍCIAS", "GANGUE", "MÁFIA", "CIVIL"},
			faction_body[(30)+(2+2+2+32+3+8+2+(10))*MAX_FACTIONS] = "ID\tNome [Abreviação]\tTipo",
			count;

		for(new i = 1; i != MAX_FACTIONS; i++)
		{
			if (!Factions[i][fON]) continue;
			format(faction_body, sizeof(faction_body), "%s\n%i\t%s [%s]\t%s", faction_body, i, Factions[i][fName], Factions[i][fShortName], faction_type[Factions[i][fType]-1]);
			count++;
		}

		if (!count) return Dialog_Show(playerid, None, DIALOG_STYLE_LIST, "{FFFFFF}Lista de facções", "A lista de facções está vazia...", "Fechar", "");

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Lista de facções", faction_body, "Fechar", "");
		return 1;
	}

	else if (CompareStrings(option, "lider"))
	{
		new paramx[8], idx[64];

		if (sscanf(secoption, "s[8]S()[64]", paramx, idx))
		{
			SendHelpMessage(playerid, "/afaction lider [definir / remover]");
			return 1;
		}

		if (CompareStrings(paramx, "definir"))
		{
			new userid, faction;

			if (sscanf(idx, "iu", faction, userid)) return SendHelpMessage(playerid, "/afaction lider definir [ID da facção] [ID/Parte do Nome]");
			if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
			if (faction < 1 || faction > MAX_FACTIONS - 1 || !Factions[faction][fON]) return SendErrorMessage(playerid, "Facção não encontrada");


			RemovePlayerFaction(userid);

			PlayerInfo[userid][pFaction] = faction;
			SQL_SetInteger("users", "faction", PlayerInfo[userid][pFaction], PlayerInfo[userid][pID]);

			PlayerInfo[userid][pRank] = Factions[faction][fRanks];
			SQL_SetInteger("users", "rank", PlayerInfo[userid][pRank], PlayerInfo[userid][pID]);

			PlayerInfo[userid][pBadgeNumber] = randomEx(23456, 99999);
			SQL_SetInteger("users", "badgenumber", PlayerInfo[userid][pBadgeNumber], PlayerInfo[userid][pID]);

			SendAdmMessageToPlayer(playerid, "Você concedeu liderança a %s sobre %s [ID: %i].", GetNameEx(userid), Factions[faction][fName], faction);
			SendAdmMessageToPlayer(userid, "O administrador %s lhe concedeu liderança sobre %s [ID: %i].", GetAccName(playerid), Factions[faction][fName], faction);
			SendAdmMessage("%s concedeu liderança a %s sobre %s [ID: %i].", GetAccName(playerid), GetNameEx(userid), Factions[faction][fName], faction);
			return 1;

		}

		else if (CompareStrings(paramx, "remover"))
		{
			new userid;

			if (sscanf(idx, "u", userid))	return SendHelpMessage(playerid, "/afaction lider remover [ID/Parte do Nome]");
			if (!IsPlayerLogged(userid))	return SendNotConnected(playerid);
			if (!isLeader(userid))			return SendErrorMessage(playerid, "Este jogador não é o líder.");

			new faction = PlayerInfo[userid][pFaction];

			SendAdmMessageToPlayer(playerid, "Você removeu a liderança de %s de %s [ID: %i].", GetNameEx(userid), Factions[faction][fName], faction);
			SendAdmMessageToPlayer(userid, "%s removeu a sua liderança de %s [ID: %i].", GetAccName(playerid), Factions[faction][fName], faction);
			SendAdmMessage("%s removeu a liderança de %s [ID: %i] de %s [ID: %i].", GetAccName(playerid), GetNameEx(userid), Factions[faction][fName], faction);

			RemovePlayerFaction(userid);
			return 1;	
		}

		else 
		{
			SendHelpMessage(playerid, "/afaction lider [definir / remover]");
			return 1;
		}
	}

	else 
	{
		SendHelpMessage(playerid, "/afaction [criar / remover / editar / lista / lider]");
	}
	return 1;
}

CMD:avehicle(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption)) return SendHelpMessage(playerid, "/avehicle [criar / remover / estacionar / tipos]");

	if (CompareStrings(option, "criar"))
	{
		new model, type, faction, plate[24], color1, color2, Float:x, Float:y, Float:z, Float:a, vw, interior;

		if (sscanf(secoption, "iiiiI(0)S()[24]", model, type, color1, color2, faction, plate)) {
			return SendHelpMessage(playerid, "/avehicle criar [modelo] [um tipo(0 - para a facção)] [cor 1] [cor 2] {c7c7c7}[Facção] [placas]");
		}

		if (model < 400 || model > 611) return SendErrorMessage(playerid, "Modelo 400 a 611.");
		if (color1 > 255 || color1 < 0 || color2 > 255 || color2 < 0) return SendErrorMessage(playerid, "Cores de 0 a 255.");
		if (model == 520 || model == 425 || model == 432 || model == 447 || model == 476) return SendErrorMessage(playerid, "Esse tipo de técnica é proibido no servidor.");

		if (isempty(plate)) format(plate, 24, "%s", GetVehicleNumber());

		GetXYInFrontOfPlayer(playerid, x, y, z, 5.0);
		GetPlayerFacingAngle(playerid, a);
		vw = GetWorld(playerid);
		interior = GetInterior(playerid);

		if (!CreateFactionVehicle(true, model, x, y, z, a, color1, color2, type, faction, plate, vw, interior)) return SendErrorMessage(playerid, "Veículo não foi criado.");
		SendAdmMessageToPlayer(playerid, "Veículo criado [modelo %i] [placas %s] [tipo %i] [Facção %i] [vw %i] [int %i].", model, plate, type, faction, vw, interior);
	}

	else if (CompareStrings(option, "remover"))
	{
		new vehid;

		if (sscanf(secoption, "i", vehid))		 return SendHelpMessage(playerid, "/avehicle remover [ID do Veículo]");
		if (!IsValidVehicle(vehid))				 return SendErrorMessage(playerid, "Veículo não encontrado.");
		if (VehicleInfo[vehid][carOwnerID] != 0) return SendErrorMessage(playerid, "Este veículo não pode ser deletado.");

		if (VehicleInfo[vehid][carAdmin] == 0)
		{
			new query[128];
			mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `cars` WHERE `id` = %i", VehicleInfo[vehid][carID]);
			mysql_tquery(dbHandle, query);
		}

		DestroyVehicle(vehid);
	}

	else if (CompareStrings(option, "estacionar"))
	{
		new vehid;

		if (sscanf(secoption, "i", vehid)) return SendHelpMessage(playerid, "/avehicle estacionar [ID do Veículo]");
		if (!IsValidVehicle(vehid)) return SendErrorMessage(playerid, "Veículo não encontrado.");
		if (VehicleInfo[vehid][carOwnerID]) return SendErrorMessage(playerid, "Este veículo não pode ser estacionado.");

		GetVehiclePos(vehid, VehicleInfo[vehid][carParkX], VehicleInfo[vehid][carParkY], VehicleInfo[vehid][carParkZ]);
		GetVehicleZAngle(vehid, VehicleInfo[vehid][carParkA]);

		SQL_SetFloat("cars", "x", VehicleInfo[vehid][carParkX], VehicleInfo[vehid][carID]);
		SQL_SetFloat("cars", "y", VehicleInfo[vehid][carParkY], VehicleInfo[vehid][carID]);
		SQL_SetFloat("cars", "z", VehicleInfo[vehid][carParkZ], VehicleInfo[vehid][carID]);
		SQL_SetFloat("cars", "fa", VehicleInfo[vehid][carParkA], VehicleInfo[vehid][carID]);

		new vehicle = CreateFactionVehicle(false, VehicleInfo[vehid][carModel], VehicleInfo[vehid][carParkX], VehicleInfo[vehid][carParkY], VehicleInfo[vehid][carParkZ], VehicleInfo[vehid][carParkA], VehicleInfo[vehid][carColor1],
			VehicleInfo[vehid][carColor2], VehicleInfo[vehid][carType], VehicleInfo[vehid][carFaction], VehicleInfo[vehid][carPlate], VehicleInfo[vehid][carVW], VehicleInfo[vehid][carInt], VehicleInfo[vehid][carID]);

		if (!vehicle) return SendErrorMessage(playerid, "Veículo não foi criado.");

		DestroyVehicle(vehid);
		SendAdmMessageToPlayer(playerid, "Veículo [ID: %i | NID: %i] estacionado e será spawnado neste local.", vehid, vehicle);
	}

	else if (CompareStrings(option, "tipos"))
	{
		new dialog_header[64] = "{FFFFFF}Tipo\t{FFFFFF}Nome\n", dialog_body[256];
		dialog_body[0] = EOS;

		format(dialog_body, sizeof(dialog_body), "%s0\tComum\n1\tCarro DMV\n2\tHelicóptero DMV\n3\tCom sirene\n4\tTáxi DMV\n5\tAluguel\n6\tEntregador de Pizza\n7\tTaxistas\n8\tCaminhoneiros\n9\tMecânicos", dialog_header);
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Tipos de Veículos", dialog_body, "Fechar", "");
	}

	else return SendHelpMessage(playerid, "/avehicle [criar / remover / estacionar / tipos]");
	return 1;
}


ALTX:apayphone("/app");
CMD:apayphone(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3))	return SendNotAdmin(playerid);

	new option[16], secoption[128];
	
	if (sscanf(params, "s[16]S()[128]", option, secoption)) return SendHelpMessage(playerid, "/apayphone [criar / remover / editar / ir]");

	if (CompareStrings(option, "criar"))
	{
		new number, type;
		if (sscanf(secoption,"iI(0)", number, type)) return SendHelpMessage(playerid, "/apayphone criar [número] {c7c7c7}[ID do modelo]");
		
		if (!type) type = 1216;

		new Float:x, Float:y, Float:z, Float:a, vw, interior;
		GetXYInFrontOfPlayer(playerid, x, y, z, 2.0);
		GetPlayerFacingAngle(playerid, a);
		vw = GetWorld(playerid);
		interior = GetInterior(playerid);
		
		new id;
		if ((id = CreatePayphone(true, type, number, x, y, z, 0.0, 0.0, 0.0, vw, interior)) == -1) return SendErrorMessage(playerid, "O número máximo de telefones públicos foi atingido.");

		SetPVarInt(playerid, #edit_pp, id + 1);
		EditDynamicObject(playerid, PayphoneInfo[id][phObj]);

		SendAdmMessageToPlayer(playerid, "Você criou um telefone público com o número: %i [ID: %i].", number, id);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new id;
		
		if (sscanf(secoption, "i", id))									return SendHelpMessage(playerid, "/apayphone remover [ID do telefone público]");
		if (id < 0 || id >= MAX_PAYPHONES || !PayphoneInfo[id][phID])	return SendErrorMessage(playerid, "Telefone público não encontrado.");

		if (IsValidDynamicObject(PayphoneInfo[id][phObj])) 			DestroyDynamicObject(PayphoneInfo[id][phObj]);
		if (IsValidDynamic3DTextLabel(PayphoneInfo[id][phLabel])) 	DestroyDynamic3DTextLabel(PayphoneInfo[id][phLabel]);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `payphones` WHERE `id` = %i", PayphoneInfo[id][phID]);
		mysql_tquery(dbHandle, query);

		PayphoneInfo[id][phID] = 0;
		SendAdmMessageToPlayer(playerid, "Você excluiu um telefone público [ID: %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id;

		if (sscanf(secoption, "d", id))								 return SendHelpMessage(playerid, "/apayphone editar [ID do telefone público]");
		if (id < 0 || id > MAX_PAYPHONES || !PayphoneInfo[id][phID]) return SendErrorMessage(playerid, "Telefone público não encontrado.");
		if (Payphone_Nearest(playerid) != id)						 return SendErrorMessage(playerid, "Você deve estar perto do telefone público.");

		SetPVarInt(playerid, #edit_pp, id + 1);
		EditDynamicObject(playerid, PayphoneInfo[id][phObj]);
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		new id;

		if (sscanf(secoption, "i", id))								return SendHelpMessage(playerid, "/apayphone ir [ID do telefone público]");
		if (id < 0 || id > MAX_PAYPHONES || !PayphoneInfo[id][phID])	return SendErrorMessage(playerid, "Telefone público não encontrado.");

		SetPlayerPosEx(playerid, PayphoneInfo[id][phX], PayphoneInfo[id][phY], PayphoneInfo[id][phZ]);
		SetWorld(playerid, PayphoneInfo[id][phVW]);
		SetInterior(playerid, PayphoneInfo[id][phInt]);
		return 1;
	}
	
	else
	{
		SendHelpMessage(playerid, "/apayphone [criar / remover / editar / ir]");
	}
	return 1;
}

ALTX:aparkmeter("/aparkm");
CMD:aparkmeter(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3))	return SendNotAdmin(playerid);

	new option[16], secoption[128];
	
	if (sscanf(params, "s[16]S()[128]", option, secoption)) return SendHelpMessage(playerid, "/aparkm [criar / remover / editar / ir]");

	if (CompareStrings(option, "criar"))
	{
		if (ParkMeter_Nearest(playerid) != -1) return SendErrorMessage(playerid, "Muito perto do parquímetro instalado.");

		new Float:x, Float:y, Float:z, Float:a;
		GetXYInFrontOfPlayer(playerid, x, y, z, 2.0);
		GetPlayerFacingAngle(playerid, a);

    	new id;
		if ((id = InsertParkMeters(playerid, x, y, z, a)) == -1) return SendErrorMessage(playerid, "É impossível criar um parquímetro, pois o limite de ("#MAX_METERS") foi atingido.");	

		SetPVarInt(playerid, #edit_pm, id + 1);
		EditDynamicObject(playerid, pMeter[id][parkObject]);
		
		SendAdmMessageToPlayer(playerid, "Você criou um parquímetro [ID: %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new id;
		
		if (sscanf(secoption, "i", id))							return SendHelpMessage(playerid, "/aparkm remover [ID do parquímetro]");
		if (id < 0 || !pMeter[id][parkID] || id >= MAX_METERS)	return SendErrorMessage(playerid, "Parquímetro não encontrado.");

		SendAdmMessageToPlayer(playerid, "Você excluiu com sucesso o parquímetro [ID:%d].", id);	
    	EraseParkMeter(id);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id;

		if (sscanf(secoption, "d", id))							return SendHelpMessage(playerid, "/aparkm editar [ID do parquímetro]");
		if (id < 0 || !pMeter[id][parkID] || id >= MAX_METERS)	return SendErrorMessage(playerid, "Parquímetro não encontrado.");
		if (ParkMeter_Nearest(playerid) != id)					return SendErrorMessage(playerid, "Você deve estar perto do parquímetro.");
		
		SetPVarInt(playerid, #edit_pm, id + 1);
		EditDynamicObject(playerid, pMeter[id][parkObject]);
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		new id;

		if (sscanf(secoption, "i", id))							return SendHelpMessage(playerid, "/aparkm ir [ID do parquímetro]");
		if (id < 0 || !pMeter[id][parkID] || id >= MAX_METERS)	return SendErrorMessage(playerid, "Parquímetro não encontrado.");

		SetPlayerPosEx(playerid, pMeter[id][parkX], pMeter[id][parkY], pMeter[id][parkZ]);
		return 1;
	}
	
	else
	{
		SendHelpMessage(playerid, "/aparkm [criar / remover / editar / ir]");
	}
	return 1;
}

CMD:givegunex(playerid, params[])
{
	new 
		userid, 
		weapon,
		ammo,
		ammotype, 
		serial
	;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) 							return SendNotAdmin(playerid);
	if (sscanf(params,"uiiii", userid, weapon, ammotype, ammo, serial))	return SendHelpMessage(playerid, "/givegunex [ID/Parte do Nome] [ID da arma] [tipo de cartucho (/ammohelp)] [munição] [número de série (0 - 1)]");
	if (!IsPlayerLogged(userid))										return SendNotConnected(playerid);
	if (weapon >= 22 && weapon <= 34) if (!CompatAmmo(weapon, ammotype))return SendErrorMessage(playerid, "Os cartuchos não são adequados para esta arma. (/ammohelp)");
	if (serial)															serial = PlayerInfo[userid][pPlayerSerial];
	if (GiveInvItem(userid, weapon, ammo, ammotype, serial) == -1)		return SendErrorMessage(playerid, "Inventário está cheio.");

	SendSyntaxMessage(playerid, "Você emitiu a %s uma(a) %s [munição: %i])", GetNameEx(userid), inventory_items[weapon][itemName], ammo);
	SendSyntaxMessage(userid, "O administrador %s emitiu para você uma(a) %s [munição: %i].", GetAccName(playerid), inventory_items[weapon][itemName], ammo);
	SendAdmMessage("%s emitiu a %s um(a) %s [munição: %i].", GetAccName(playerid), GetNameEx(userid), inventory_items[weapon][itemName], ammo);

    new mes[128];
	format(mes, sizeof(mes), "[/givegunex] %s emitiu uma arma para %s. (%s, Munição: %i)", GetAccName(playerid), GetNameEx(userid), inventory_items[weapon][itemName], ammo);
	insert_log(mes, ADMIN_LOG, 7, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);
	return 1;
}


CMD:acceptname(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_3))	return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))				return SendHelpMessage(playerid, "/acceptname [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (!strlen(pTemp[userid][pChangeNick]))		return SendErrorMessage(playerid, "O jogador não solicitou a mudança de nome.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", pTemp[userid][pChangeNick]);
	mysql_tquery(dbHandle, query, "OnChangeName", "iis", playerid, userid, pTemp[userid][pChangeNick]);
	return 1;
}

CMD:itemlist(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);

	new dialog_header[64] = "{FFFFFF}Nome\t{FFFFFF}Item ID\n", dialog_body[3072];
	dialog_body[0] = EOS;

	format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

	for(new i = 1; i != sizeof(inventory_items); i++)
	{
		format(dialog_body, sizeof(dialog_body), "%s%s\t%i\n", dialog_body, inventory_items[i][itemName], i);
	}

	Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Lista de itens", dialog_body, "Fechar", "");
	return 1;
}

CMD:giveitem(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);

	new userid, item, amount;

	if (sscanf(params, "udd", userid, item, amount))	return SendHelpMessage(playerid, "/giveitem [ID/Parte do Nome] [ID do item] [quantidade]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (item < 1 || item >= sizeof(inventory_items)) 	return SendErrorMessage(playerid, "ID de item inválido.");
	if (amount < 1 || amount > 1000)					return SendErrorMessage(playerid, "Quantidade de 1 a 1000.");

	switch(item)
	{
		case 22..34: return SendErrorMessage(playerid, "Você pode emitir armas através dos comandos /getgun ou /givegunex.");
		case INV_DRUG: return SendErrorMessage(playerid, "Você pode dispensar drogas através do comando /givedrug.");
		case INV_GANJA, INV_MED_GANJA, 103 .. 118:
		{	
			if (GiveFloatInvItem(userid, item, float(amount)) == -1)	return SendErrorMessage(playerid, "Inventário está cheio.");

			SendAdmMessageToPlayer(userid, "Administrador %s emitiu para você %s [%ig].", GetAccName(playerid), inventory_items[item][itemName], amount);
			SendAdmMessage("%s emitiu %s %s [%ig].", GetAccName(playerid), GetNameEx(userid), inventory_items[item][itemName], amount);
		}
		default:
		{
			if (GiveInvItem(userid, item, amount) == -1) return SendErrorMessage(playerid, "Inventário está cheio.");
			
			SendAdmMessageToPlayer(userid, "Administrador %s emitiu para você %s [%i peças].", GetAccName(playerid), inventory_items[item][itemName], amount);
			SendAdmMessage("%s emitiu %s %s [%i peças].", GetAccName(playerid), GetNameEx(userid), inventory_items[item][itemName], amount);
		}
	}

	return 1;
}

CMD:givedrug(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);

	new userid, type, Float:amount, quality, drugname[16];

	if (sscanf(params, "us[16]fdd", userid, drugname, amount, quality, type)) return SendHelpMessage(playerid, "/givedrug [ID/Parte do Nome] [Nome da Droga] [Quantidade] [OAS] [tipo]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (isnull(drugname) || strlen(drugname) > 16) return SendErrorMessage(playerid, "Caracteres no nome devem ter entre 1 e 16.");
	if (amount < 1.0 || amount > 1000.0) return SendErrorMessage(playerid, "Quantidade deve estar entre 1 e 1000.");
	if (quality < 1 || quality > 100) return SendErrorMessage(playerid, "OAS deve estar entre 1 e 100.");
	if (type < 1 || type > 4) return SendErrorMessage(playerid, "Tipo deve estar entre 1 e 4.");

	if (GiveDrugInvItem(playerid, drugname, amount, quality, type) == -1) return SendErrorMessage(playerid, "Inventário está cheio.");
	SendAdmMessageToPlayer(userid, "O administrador %s emitiu para você %s [OAS:%i%%] (%.1fg) / %i do tipo.", GetAccName(playerid), drugname, quality, amount, type);
	SendAdmMessage("%s emitiu %s %s [OAS:%i%%] (%.1fg) / %i do tipo.", GetAccName(playerid), GetNameEx(userid), drugname, quality, amount, type);

	return 1;
}

CMD:getgun(playerid, params[])
{
	new weapon, ammo = 999, serial = 0;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);
	if (sscanf(params, "d", weapon)) return SendHelpMessage(playerid, "/getgun [ID da arma]");

	new ammotype = GetDefaultAmmoType(weapon);

	if (serial) serial = PlayerInfo[playerid][pPlayerSerial];
	if (GivePlayerWeaponEx(playerid, weapon, ammo, ammotype, serial) == 0) return SendErrorMessage(playerid, "Arma não encontrada.");

	SendAdmMessage("%s emitiu para si mesmo %s.", GetAccName(playerid), inventory_items[weapon][itemName]);

	new mes[128];
	format(mes, sizeof(mes), "[/getgun] %s emitiu para si mesmo %s.", GetAccName(playerid), inventory_items[weapon][itemName]);
	insert_log(mes, ADMIN_LOG, 7, PlayerInfo[playerid][pAccID]);
	return 1;
}
CMD:alock(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);

    for(new i; i != MAX_HOUSES; i++)
    {
        if (HouseInfo[i][hID] == 0 || (0 == IsPlayerInsideHouse(playerid, i) && 0 == IsPlayerOutsideHouse(playerid, i))) continue;

        HouseInfo[i][hLocked] = !HouseInfo[i][hLocked];
        SQL_SetInteger("houses", "locked", HouseInfo[i][hLocked], HouseInfo[i][hID]);

        if (HouseInfo[i][hLocked] == 0)
        {
            SendAdmMessage("%s abriu a porta da casa [ID: %i].", GetAccName(playerid), i);
            GameTextForPlayer(playerid, "~w~PORTA ~g~ABERTA", 5000, 6);
        }
        else
        {
            SendAdmMessage("%s fechou a porta da casa [ID: %i].", GetAccName(playerid), i);
            GameTextForPlayer(playerid, "~w~PORTA ~r~FECHADA", 5000, 6);
        }
        return 1;
    }

    for(new i = 0; i != MAX_BIZ; i++)
    {
        if (BizInfo[i][bizID] == 0 || (0 == IsPlayerBizOutside(playerid, i) && 0 == IsPlayerBizInside(playerid, i)))    continue;

        BizInfo[i][bizLocked] = !BizInfo[i][bizLocked];
        SQL_SetInteger("business", "locked", BizInfo[i][bizLocked], BizInfo[i][bizID]);

        if (BizInfo[i][bizLocked] == 0)
        {
            SendAdmMessage("%s abriu a porta da empresa [ID: %i].", GetAccName(playerid), i);
            GameTextForPlayer(playerid, "~w~PORTA ~g~ABERTA", 5000, 6);
        }
        else
        {
            SendAdmMessage("%s fechou a porta da empresa [ID: %i].", GetAccName(playerid), i);
            GameTextForPlayer(playerid, "~w~PORTA ~r~FECHADA", 5000, 6);
        }
        return 1;
    }

    for(new i = 0; i != MAX_ENTRANCE; i++)
    {
        if (EntranceInfo[i][eFaction] == 0 || EntranceInfo[i][entranceOn] == 0 || (IsPlayerEntranceOutside(playerid, i) == 0 && IsPlayerEntranceInside(playerid, i) == 0)) continue;

        EntranceInfo[i][eLocked] = !EntranceInfo[i][eLocked];
        PlaySound(playerid, 1145);

        if (EntranceInfo[i][eLocked])
        {
            SendAdmMessage("%s fechou a porta da facção [ID portas: %i].", GetAccName(playerid), i);
            GameTextForPlayer(playerid, "~w~PORTA ~r~FECHADA", 5000, 6);
        }
        else
        {
            SendAdmMessage("%s abriu a porta da facção [ID portas: %i].", GetAccName(playerid), i);
            GameTextForPlayer(playerid, "~w~PORTA ~g~ABERTA", 5000, 6);
        }
        break;
    }
    SendErrorMessage(playerid, "Você deve estar perto de sua casa / empresa / entrada.");
    return 1;
}

CMD:ahouse(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendNotAdmin(playerid);
	
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/ahouse [criar / remover / editar / vender / ir / portatrazeira]");
		return 1;
	}

	new houseid,
		level,
		price, 
		clases,
		classes_name[3][10] = {"Pequena", "Media", "Grande"};

	if (CompareStrings(option, "criar"))
	{
		new Float:x, Float:y, Float:z, Float:newZ;

		if (sscanf(secoption, "iiF(0.0)", clases, price, newZ)) 	return SendHelpMessage(playerid, "/ahouse criar [TIPO (1 - trailer | 2 - pequena | 3 - media | 4 - grande] [preço]");
		if (clases < 1 || clases > 4)            return SendErrorMessage(playerid, "Tipos de casas de 1 a 4.");
		if (price < 1 || price > 2000000)       return SendErrorMessage(playerid, "Custo da casa de $1 a $2000000.");
		if ((houseid = GetNextHouseID()) == -1)  return SendErrorMessage(playerid, "Limite máximo de casas atingido.");

		new vw, interior;

		if (clases > 2) 	level = 1;
		else 			level = 0;

		GetPlayerPos(playerid, x, y, z);
		vw = GetWorld(playerid);
		interior = GetInterior(playerid);

		if (vw != 0 || interior != 0) HouseInfo[houseid][hApart] = GetNearApartmentInside(playerid);
		else 						 HouseInfo[houseid][hApart] = 0;

		HouseInfo[houseid][hPosX] = x;
		HouseInfo[houseid][hPosY] = y;
		HouseInfo[houseid][hPosZ] = z+newZ;
		HouseInfo[houseid][hPosVW] = vw;
		HouseInfo[houseid][hPosInt] = interior;

		HouseInfo[houseid][hExitX] = 
		HouseInfo[houseid][hExitY] = 
		HouseInfo[houseid][hExitZ] = 0.0;
		HouseInfo[houseid][hExitVW] = 5000 + houseid;

		if (clases == 1) 		SetHouseInterior(houseid, 0);
		else if (clases == 2)	SetHouseInteriorEx(houseid, 0);
		else if (clases == 3)	SetHouseInteriorEx(houseid, 1);
		else if (clases == 4)	SetHouseInteriorEx(houseid, 2);

		switch(clases)
		{
			case 1: clases = 0;
			case 2: clases = 0;
			case 3: clases = 1;
			case 4: clases = 2;
		}

		HouseInfo[houseid][hClass] = clases;

		for(new i = 0; i < 2; i++) {
			HouseInfo[houseid][hExtX][i] = 
			HouseInfo[houseid][hExtY][i] = 
			HouseInfo[houseid][hExtZ][i] = 0.0;
			HouseInfo[houseid][hExtVW][i] = 
			HouseInfo[houseid][hExtInt][i] = 0;
		}

		HouseInfo[houseid][hCheckPosX] = 
		HouseInfo[houseid][hCheckPosY] = 
		HouseInfo[houseid][hCheckPosZ] = 0.0;
		HouseInfo[houseid][hLevelbuy] = level;
		HouseInfo[houseid][hPrice] = price;
		HouseInfo[houseid][hLocked] = 1;
		HouseInfo[houseid][hFurnitureRadius] = 7.0;
		HouseInfo[houseid][hOwnerID] = 
		HouseInfo[houseid][hCash] = 
		HouseInfo[houseid][hRadio] = 
		HouseInfo[houseid][hHeal] = 
		HouseInfo[houseid][hHealAmount] =
		HouseInfo[houseid][hCupboard] = 0;

		new door[11];
		
		switch(HouseInfo[houseid][hDoor])
		{
			case 0: door = "Normal";
			case 1: door = "Madeira";
			case 2: door = "Ferro";
		}

		HouseInfo[houseid][hCheckPoint] = CreateDynamicCP(x, y, z+newZ, 1.0, HouseInfo[houseid][hPosVW], -1, -1, 2.0);
		AddHouseToFile(houseid, HouseInfo[houseid][hClass], level, price, HouseInfo[houseid][hApart], x, y, z+newZ, 5000 + houseid, vw, interior);

		SendAdmMessageToPlayer(playerid, "Você criou uma casa [ID: %i | Nível: %i | Preço: %i | Tipo: %s [%i] ].", houseid, level, price, classes_name[clases], clases);
		return 1;
	}
	else if (CompareStrings(option, "remover"))
	{
	    if (sscanf(secoption, "i", houseid))
	        return SendHelpMessage(playerid, "/ahouse remover [ID da casa]");
	        
	    if (houseid < 0 || houseid >= MAX_HOUSES || !HouseInfo[houseid][hID])
	        return SendErrorMessage(playerid, "Casa não encontrada.");

	    DestroyDynamicCP(HouseInfo[houseid][hCheckPoint]);

	    if (IsValidDynamic3DTextLabel(HouseInfo[houseid][hLabelDDoor]))
	    {
	        DestroyDynamic3DTextLabel(HouseInfo[houseid][hLabelDDoor]);
	        HouseInfo[houseid][hLabelDDoor] = Text3D:INVALID_3DTEXT_ID;
	    }

	    new query[128];
	    mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `houses` WHERE `id` = %i", HouseInfo[houseid][hID]);
	    mysql_tquery(dbHandle, query);

	    HouseInfo[houseid][hID] = 0;
	    SendAdmMessageToPlayer(playerid, "Casa [ID: %i] deletada.", houseid);
	}


	else if (CompareStrings(option, "editar"))
	{
		new parameters[12], arg[12];
		
		if (sscanf(secoption,"is[12]S()[12]", houseid, parameters, arg))
		{
			SendHelpMessage(playerid, "/ahouse editar [ID da casa] [opções] [valor]");
			SendSyntaxMessage(playerid, "preço / nivel / tipo / raio / interior / entrada / saida / entradainterna / saidaexterna");
			return 1;
		}

		if (houseid < 0 || houseid > MAX_HOUSES || !HouseInfo[houseid][hID]) return SendErrorMessage(playerid, "Casa não encontrada.");

		if (CompareStrings(parameters, "preco"))
		{
		    if (sscanf(arg, "i", price))       return SendHelpMessage(playerid, "/ahouse editar [ID da casa] preco [preço]");
		    if (price < 1 || price > 2000000) return SendErrorMessage(playerid, "Preço de $1 a $2000000.");

		    HouseInfo[houseid][hPrice] = price;
		    SQL_SetInteger("houses", "price", HouseInfo[houseid][hPrice], HouseInfo[houseid][hID]);
		    SendAdmMessageToPlayer(playerid, "Você alterou o valor da sua casa [ID: %i] para $%i.", houseid, HouseInfo[houseid][hPrice]);
		}
		else if (CompareStrings(parameters, "nivel"))
		{
		    if (sscanf(arg, "i", level)) return SendHelpMessage(playerid, "/ahouse editar [ID da casa] nivel [nível]");
		    if (level < 0 || level > 35) return SendErrorMessage(playerid, "Nível da casa de 0 a 35.");

		    HouseInfo[houseid][hLevelbuy] = level;
		    SQL_SetInteger("houses", "levelbuy", HouseInfo[houseid][hLevelbuy], HouseInfo[houseid][hID]);
		    SendAdmMessageToPlayer(playerid, "Você alterou o nível para comprar uma casa [ID: %i] para %i.", houseid, HouseInfo[houseid][hLevelbuy]);
		}

		else if (CompareStrings(parameters, "tipo"))
		{
		    if (sscanf(arg, "i", clases)) return SendHelpMessage(playerid, "/ahouse editar [ID da casa] tipo [um tipo]");
		    if (clases < 1 || clases > 3) return SendErrorMessage(playerid, "Tipo de casa [1 - pequena | 2 - média | 3 - grande].");

		    HouseInfo[houseid][hClass] = clases - 1;
		    SQL_SetInteger("houses", "class", HouseInfo[houseid][hClass], HouseInfo[houseid][hID]);
		    SendAdmMessageToPlayer(playerid, "Você alterou o tipo de casa [ID: %i] para %s [%i].", houseid, classes_name[clases-1], HouseInfo[houseid][hClass]);
		}

		else if (CompareStrings(parameters, "raio"))
		{
		    new Float:radius;

		    if (sscanf(arg, "f", radius)) return SendHelpMessage(playerid, "/ahouse editar [ID da casa] raio [raio para colocar os móveis do lado de fora].");
		    if (radius < 0.0 || radius > 30.0) return SendErrorMessage(playerid, "Raio de 0.0 a 30.0.");

		    HouseInfo[houseid][hFurnitureRadius] = radius;
		    SQL_SetFloat("houses", "furniture_radius", HouseInfo[houseid][hFurnitureRadius], HouseInfo[houseid][hID]);
		    SendAdmMessageToPlayer(playerid, "Você alterou o raio para os móveis da sua casa [ID: %i] para %.2f", houseid, HouseInfo[houseid][hFurnitureRadius]);
		}


		else if (CompareStrings(parameters, "int"))
		{
		    new string[2048];

		    for(new i = 0; i < MAX_INTERIORS; i++)
		    {
		        if (!AdminTeles[i][telesOn] || HouseInfo[houseid][hClass] != AdminTeles[i][telesType]) continue;
		        format(string, sizeof(string), "%s\n<%i> {66FF66}%s", string, i, AdminTeles[i][telesMapName]);
		    }

		    if (isempty(string)) return SendErrorMessage(playerid, "Interiores para casas deste tipo não encontrados.");

		    SetPVarInt(playerid, "house_change", houseid);
		    Dialog_Show(playerid, ChangeHouseInterior, DIALOG_STYLE_LIST, "{FFFFFF}Interior da Casa", string, "Selecionar", "Cancelar");
		}
		
		else if (CompareStrings(parameters, "entrada"))
		{
			if (IsValidDynamicCP(HouseInfo[houseid][hCheckPoint])) DestroyDynamicCP(HouseInfo[houseid][hCheckPoint]);

			GetPlayerPos(playerid, HouseInfo[houseid][hPosX], HouseInfo[houseid][hPosY], HouseInfo[houseid][hPosZ]);
			HouseInfo[houseid][hPosVW] = GetWorld(playerid);
			HouseInfo[houseid][hPosInt] = GetInterior(playerid);

			SQL_SetFloat("houses", "posx", HouseInfo[houseid][hPosX], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "posy", HouseInfo[houseid][hPosY], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "posz", HouseInfo[houseid][hPosZ], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "posint", HouseInfo[houseid][hPosInt], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "posvw", HouseInfo[houseid][hPosVW], HouseInfo[houseid][hID]);
			
			HouseInfo[houseid][hCheckPoint] = CreateDynamicCP(HouseInfo[houseid][hPosX], HouseInfo[houseid][hPosY], HouseInfo[houseid][hPosZ], 1.0, HouseInfo[houseid][hPosVW], HouseInfo[houseid][hPosInt], -1, 2.0);
			SendAdmMessageToPlayer(playerid, "Você mudou a entrada principal da casa [ID: %i].", houseid);
		}

		else if (CompareStrings(parameters, "exit"))
		{
			GetPlayerPos(playerid, HouseInfo[houseid][hExitX], HouseInfo[houseid][hExitY], HouseInfo[houseid][hExitZ]);
			HouseInfo[houseid][hExitInt] = GetInterior(playerid);

			SQL_SetFloat("houses", "exitx", HouseInfo[houseid][hExitX], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "exity", HouseInfo[houseid][hExitY], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "exitz", HouseInfo[houseid][hExitZ], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "interior", HouseInfo[houseid][hExitInt], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "exitvw", HouseInfo[houseid][hExitVW], HouseInfo[houseid][hID]);

			SendAdmMessageToPlayer(playerid, "Você mudou sua saída principal de casa [ID: %i].", houseid);
		}

		else if (CompareStrings(parameters, "entradaadicional"))
		{
			if (IsValidDynamicCP(HouseInfo[houseid][hExtCheckPoint])) DestroyDynamicCP(HouseInfo[houseid][hExtCheckPoint]);

			GetPlayerPos(playerid, HouseInfo[houseid][hExtX][0], HouseInfo[houseid][hExtY][0], HouseInfo[houseid][hExtZ][0]);
			HouseInfo[houseid][hExtVW][0] = GetWorld(playerid);
			HouseInfo[houseid][hExtInt][0] = GetInterior(playerid);

			SQL_SetFloat("houses", "ext_ENposX", HouseInfo[houseid][hExtX][0], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "ext_ENposY", HouseInfo[houseid][hExtY][0], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "ext_ENposZ", HouseInfo[houseid][hExtZ][0], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "ext_ENint", HouseInfo[houseid][hExtInt][0], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "ext_ENvw", HouseInfo[houseid][hExtVW][0], HouseInfo[houseid][hID]);
			
			HouseInfo[houseid][hExtCheckPoint] = CreateDynamicCP(HouseInfo[houseid][hExtX][0], HouseInfo[houseid][hExtY][0], HouseInfo[houseid][hExtZ][0], 1.0, HouseInfo[houseid][hExtVW][0], HouseInfo[houseid][hExtInt][0], -1, 2.0);
			SendAdmMessageToPlayer(playerid, "Você mudou uma entrada adicional para a casa [ID: %i].", houseid);
		}

		else if (CompareStrings(parameters, "saidaadicional"))
		{
			GetPlayerPos(playerid, HouseInfo[houseid][hExtX][1], HouseInfo[houseid][hExtY][1], HouseInfo[houseid][hExtZ][1]);
			HouseInfo[houseid][hExtInt][1] = GetInterior(playerid);

			SQL_SetFloat("houses", "ext_EXposX", HouseInfo[houseid][hExtX][1], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "ext_EXposY", HouseInfo[houseid][hExtY][1], HouseInfo[houseid][hID]);
			SQL_SetFloat("houses", "ext_EXposZ", HouseInfo[houseid][hExtZ][1], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "ext_EXint", HouseInfo[houseid][hExtInt][1], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "ext_EXvw", HouseInfo[houseid][hExtVW][1], HouseInfo[houseid][hID]);

			SendAdmMessageToPlayer(playerid, "Você alterou a saida da casa [ID: %i].", houseid);
		}

		else 
		{
			SendHelpMessage(playerid, "/ahouse editar [Casa ID] [opção] [valor]");
			SendSyntaxMessage(playerid, "preco / nivel / tipo / raio / int / entrada / saida / entradaadicional / saidaadicional");
			return 1;
		}
	}
	
	else if (CompareStrings(option, "vender"))
	{
		if (sscanf(secoption, "i", houseid)) 									 return SendHelpMessage(playerid, "/ahouse sell [ID das casas]");
		if (houseid < 0 || houseid > MAX_HOUSES - 1 || !HouseInfo[houseid][hID]) return SendErrorMessage(playerid, "Casa não encontrada.");
		if (!HouseInfo[houseid][hOwnerID])										 return SendErrorMessage(playerid, "Esta casa já está à venda.");


		new userid = GetIDByID(HouseInfo[houseid][hOwnerID]);

		if (IsPlayerLogged(userid)) {
			GiveCash(playerid, HouseInfo[houseid][hPrice]/2/* + GetFurniturePrice(0, houseid)*/);
			if (HouseInfo[houseid][hCash] > 0) GiveCash(playerid, HouseInfo[houseid][hCash]);

			SendAdmMessageToPlayer(userid, "Administrador %s vendeu sua casa. Devolveu a você $%i e do cofre $%i.", GetAccName(playerid), HouseInfo[houseid][hPrice]/2/* + GetFurniturePrice(0, houseid)*/, HouseInfo[houseid][hCash]);
		} else {
			new query[128];
			mysql_format(dbHandle, query, sizeof(query), "SELECT cash FROM `users` WHERE id=%i LIMIT 1", HouseInfo[houseid][hOwnerID]);
			new Cache:cache = mysql_query(dbHandle, query),
				money = cache_get_field_content_int(0, "cash", dbHandle);

			cache_delete(cache);

			money += HouseInfo[houseid][hPrice]/2/*+ GetFurniturePrice(0, houseid)*/ + HouseInfo[houseid][hCash];

			mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `cash`=%i WHERE id=%i LIMIT 1", money, HouseInfo[houseid][hOwnerID]);		
			mysql_query(dbHandle, query);
		}

		Sell_House(houseid);
		SendAdmMessageToPlayer(playerid, "Você colocou a casa à venda [ID: %i]", houseid);

	}

	else if (CompareStrings(option, "ir"))
	{
		if (sscanf(secoption, "i", houseid)) return SendHelpMessage(playerid, "/ahouse goto [ID das casas]");
		if (houseid < 0 || houseid > MAX_HOUSES - 1 || !HouseInfo[houseid][hID]) return SendErrorMessage(playerid, "Casa não encontrada.");

		SetPlayerPosEx(playerid, HouseInfo[houseid][hPosX], HouseInfo[houseid][hPosY], HouseInfo[houseid][hPosZ]);
		SetInterior(playerid, HouseInfo[houseid][hPosInt]);
		SetWorld(playerid, HouseInfo[houseid][hPosVW]);
		SendSpecialMessage(playerid, "Você teleportou para casa [ID: %i].", houseid);
	}
	
	else if (CompareStrings(option, "portatrazeira"))
	{
	    if (sscanf(secoption, "i", houseid)) return SendHelpMessage(playerid, "/ahouse portatrazeira [ID da casa]");
	    if (houseid < 0 || houseid > MAX_HOUSES - 1 || !HouseInfo[houseid][hID]) return SendErrorMessage(playerid, "Casa não encontrada.");
	    if (HouseInfo[houseid][hApart]) return SendErrorMessage(playerid, "Não é possível criar uma entrada/saída traseira para esta casa!");

		if (pTemp[playerid][pHouseEntered] == houseid) {
			new Float: posX, Float: posY, Float: posZ;
			GetPlayerPos(playerid, posX, posY, posZ);

			HouseInfo[houseid][hExtX][0] = posX;
			HouseInfo[houseid][hExtY][0] = posY;
			HouseInfo[houseid][hExtZ][0] = posZ;

			Save_House(houseid);

			HouseInfo[houseid][hExtVW][0] = GetPlayerVirtualWorld(playerid);
			HouseInfo[houseid][hExtInt][0] = GetPlayerInterior(playerid);

			return SendAdmMessageToPlayer(playerid, "Você posicionou com sucesso uma SAÍDA adicional para casa [ID:%i]", houseid);
		} else {
			new garageid = GetNearInsideGarage(playerid);
			if (garageid != -1) {
				if (HouseInfo[houseid][hID] == GarageInfo[garageid][garageHouse]) {
					if (IsValidDynamicCP(HouseInfo[houseid][hExtCheckPoint])) DestroyDynamicCP(HouseInfo[houseid][hExtCheckPoint]);
					
					new Float: posX, Float: posY, Float: posZ;
					GetPlayerPos(playerid, posX, posY, posZ);

					HouseInfo[houseid][hExtX][0] = posX;
					HouseInfo[houseid][hExtY][0] = posY;
					HouseInfo[houseid][hExtZ][0] = posZ;

					HouseInfo[houseid][hExtVW][0] = GetPlayerVirtualWorld(playerid);
					HouseInfo[houseid][hExtInt][0] = GetPlayerInterior(playerid);

					Save_House(houseid);

					HouseInfo[houseid][hExtCheckPoint] = CreateDynamicCP(HouseInfo[houseid][hExtX][0], HouseInfo[houseid][hExtY][0], HouseInfo[houseid][hExtZ][0], 1.0, HouseInfo[houseid][hExtVW][0], HouseInfo[houseid][hExtInt][0], -1, 2.0);
					return SendAdmMessageToPlayer(playerid, "Você posicionou com sucesso uma ENTRADA adicional para casa [ID:%i]", houseid);
				}
				else return SendErrorMessage(playerid, "You cannot create a back here Entrance/exit!");
			}
			else if (GetNearOutsideHouse(playerid, 30)) {
				if (IsValidDynamicCP(HouseInfo[houseid][hExtCheckPoint])) DestroyDynamicCP(HouseInfo[houseid][hExtCheckPoint]);
				
				new Float: posX, Float: posY, Float: posZ;
				GetPlayerPos(playerid, posX, posY, posZ);

				HouseInfo[houseid][hExtX][0] = posX;
				HouseInfo[houseid][hExtY][0] = posY;
				HouseInfo[houseid][hExtZ][0] = posZ;

				HouseInfo[houseid][hExtVW][0] = GetPlayerVirtualWorld(playerid);
				HouseInfo[houseid][hExtInt][0] = GetPlayerInterior(playerid);

				Save_House(houseid);

				HouseInfo[houseid][hExtCheckPoint] = CreateDynamicCP(HouseInfo[houseid][hExtX][0], HouseInfo[houseid][hExtY][0], HouseInfo[houseid][hExtZ][0], 1.0, HouseInfo[houseid][hExtVW][0], HouseInfo[houseid][hExtInt][0], -1, 2.0);
				return SendAdmMessageToPlayer(playerid, "Você posicionou com sucesso uma ENTRADA adicional para casa [ID:%i]", houseid);
			}
			else return SendErrorMessage(playerid, "Você não pode criar uma entrada/saída aqui!");
		}
	}

	else 
	{
		SendHelpMessage(playerid, "/ahouse [opções]");
		SendSyntaxMessage(playerid, "criar / remover / editar / vender / ir / portatrazeira");	
	}

	return 1;
}

ALTX:abiz("/abusiness");
CMD:abiz(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/abiz [criar / remover / editar / ir / vender / tipos / lista]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new bizid = -1, level, price, freeze, type, name[32];

		if (sscanf(secoption, "iI(0)I(0)S()[32]", type, freeze, price, name)) 
		    return SendHelpMessage(playerid, "/abiz criar [tipo] {c7c7c7}[0 - pode comprar | 1 - não pode comprar] [preço] [nome]");
		if (freeze < 0 || freeze > 1)
		    return SendErrorMessage(playerid, "Congelamento de 0 a 1.");
		if (type > sizeof(biz_types) - 1 || type < 0)
		    return SendErrorMessage(playerid, "Tipo de negócio de 0 a %i.", sizeof(biz_types) - 1);
		if ((bizid = GetNextBizID()) == -1)
		    return SendErrorMessage(playerid, "O número máximo de empresas foi atingido.");

		
		if (!price) {
			switch(type) {
				case 0:							 price = 10000;
				case 1,2,4,5,15..19,21,22,23,24: price = 15000;
				case 3,6,7,12,20: 				 price = 25000;
				case 8,9,10,11,13: 				 price = 45000;
				case 14: 						 price = 100000;
			}
		}
		
		if (price < 2500 || price > 5000000)	return SendErrorMessage(playerid, "Preço do negócio de $2500 a $5000000");


		level = 1;

		GetPlayerPos(playerid, BizInfo[bizid][bizPosX], BizInfo[bizid][bizPosY], BizInfo[bizid][bizPosZ]);
		BizInfo[bizid][bizPosInt] = GetInterior(playerid);
		BizInfo[bizid][bizPosVW] = GetWorld(playerid);

		GetPlayerPos(playerid, BizInfo[bizid][bizExitX], BizInfo[bizid][bizExitY], BizInfo[bizid][bizExitZ]);
		BizInfo[bizid][bizExitInt] = GetInterior(playerid);
		BizInfo[bizid][bizExitVW] = 5000 + bizid;

		BizInfo[bizid][bizMes] = EOS;
		BizInfo[bizid][bizType] = type;
		BizInfo[bizid][bizLevelbuy] = level;
		BizInfo[bizid][bizPrice] = price;
		BizInfo[bizid][bizFreeze] = freeze;
		BizInfo[bizid][bizOwnerID] = 
		BizInfo[bizid][bizLocked] = 
		BizInfo[bizid][bizCash] = 
		BizInfo[bizid][bizEntrance] = 
		BizInfo[bizid][bizRadioOn] = 
		BizInfo[bizid][bizRadio] = 0;
		
		new biztype = -1;

		switch(type)
		{
			case BIZ_PIZZA:		biztype = 0;
			case BIZ_CLUCKIN:	biztype = 1;
			case BIZ_BURGER:	biztype = 2;
			case BIZ_DONUT:		biztype = 3;
			case BIZ_STORE:		biztype = 4;
			case BIZ_BAR:		biztype = 5;
			case BIZ_PHARMACY:	biztype = 6;
		}

		if (biztype != -1) {
			for (new i = 0; i < max_product[type]; i++) {
				BizInfo[bizid][bizProductPrice][i] = biz_prices[biztype][i];
			}
		}
		
		BizInfo[bizid][bizProduct] = 400;
		BizInfo[bizid][bizBuyPrice] = price;
	
		format(BizInfo[bizid][bizOwner], MAX_PLAYER_NAME, "Estado");
		
		if (isempty(name))	format(BizInfo[bizid][bizName], 32, "%s", biz_types[type]);
		else				format(BizInfo[bizid][bizName], 32, "%s", name);
		
		
		BizInfo[bizid][bizPickUp] = CreateDynamicPickup(1239, 1, BizInfo[bizid][bizPosX], BizInfo[bizid][bizPosY], BizInfo[bizid][bizPosZ], BizInfo[bizid][bizPosVW], BizInfo[bizid][bizPosInt], -1, 100.0);
		AddBizToFile(bizid, level, price, freeze, type, BizInfo[bizid][bizName], BizInfo[bizid][bizMes], BizInfo[bizid][bizPosX], BizInfo[bizid][bizPosY], BizInfo[bizid][bizPosZ], BizInfo[bizid][bizExitX], BizInfo[bizid][bizExitY],
		BizInfo[bizid][bizExitZ], BizInfo[bizid][bizPosInt], BizInfo[bizid][bizPosVW], BizInfo[bizid][bizExitInt], BizInfo[bizid][bizExitVW]);
		SendSyntaxMessage(playerid, "Você criou um negócio [ID: %i | Nível: %i | Preço: %i | Oportunidade de compra: %s | Tipo: %s].", bizid, level, price, (freeze) ? ("Não") : ("Sim"), biz_types[BizInfo[bizid][bizType]]);

		if(BizInfo[bizid][bizType] == BIZ_AUTOSALON || BizInfo[bizid][bizType] == BIZ_BOATSALON || BizInfo[bizid][bizType] == BIZ_FLYSALON)  {
			SendHelpMessage(playerid, "Você deve definir a posição do veículo ao comprar '/abiz editar %i vehspawn'", bizid);
		}

		Save_Business(bizid);
	}

	else if (CompareStrings(option, "remover"))
	{
		new bizid;

		if (sscanf(secoption, "i", bizid))							 return SendHelpMessage(playerid, "/abiz remove [ID do negócio]");
		if (bizid < 1 || bizid >= MAX_BIZ || !BizInfo[bizid][bizID]) return SendErrorMessage(playerid, "Negócio não encontrado.");

		if (IsValidDynamicPickup(BizInfo[bizid][bizPickUp]))	DestroyDynamicPickup(BizInfo[bizid][bizPickUp]);

		new ownerid = GetIDByID(BizInfo[bizid][bizOwnerID]), query[128];

		if (IsPlayerLogged(ownerid))
		{
			SendSyntaxMessage(ownerid, "O administrador %s excluiu sua empresa.", GetAccName(playerid));
			SendSyntaxMessage(ownerid, "Você foi reembolsado pela sua compra [$%i] e pelo dinheiro do caixa da empresa [$%i].", BizInfo[bizid][bizPrice], BizInfo[bizid][bizCash]);
			GiveCash(ownerid, BizInfo[bizid][bizPrice]);
			
			if (BizInfo[bizid][bizCash] > 0) GiveCash(ownerid, BizInfo[bizid][bizCash]);
		}
		else 
		{
			new biz_cost = BizInfo[bizid][bizPrice];
			
			if (BizInfo[bizid][bizCash] > 0) biz_cost += BizInfo[bizid][bizCash];

			mysql_format(dbHandle, query, sizeof(query), "SELECT `cash` FROM `users` WHERE `id` = %i", BizInfo[bizid][bizOwnerID]);
			mysql_tquery(dbHandle, query, "GivePlayerBizMoney", "ii", biz_cost, BizInfo[bizid][bizOwnerID]);
		}

		RemoveAllWorkers(bizid);

		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `business` WHERE `id` = %i", BizInfo[bizid][bizID]);
		mysql_tquery(dbHandle, query);

		BizInfo[bizid][bizID] = 
		BizInfo[bizid][bizOwnerID] = 0;
		SendSyntaxMessage(playerid, "Você excluiu o negócio [ID: %i].", bizid);

	}

	else if (CompareStrings(option, "editar"))
	{
		new id, parameters[16], arg[64];
		
		if (sscanf(secoption, "is[16]S()[64]", id, parameters, arg))
		{
			SendHelpMessage(playerid, "/abiz editar [ID do negócio] [opções] [valor]");
			SendSyntaxMessage(playerid, "preco / produtos / nivel / tipo / nome / taxa / caixa / congelar / entrada / saída / int / spawn de veículo");
			return 1;
		}
		
		if (id < 0 || id >= MAX_BIZ || !BizInfo[id][bizID]) return SendErrorMessage(playerid, "Empresa não encontrada.");

		if (CompareStrings(parameters, "preco"))
		{
			new price;
			
			if (sscanf(arg, "i", price)) 		return SendHelpMessage(playerid, "/abiz edit [ID do negócio] preço [preço]");
			if (price < 2 || price > 5000000)	return SendErrorMessage(playerid, "Preço do negócio de $2 a $5000000.");

			BizInfo[id][bizPrice] = price;
			SQL_SetInteger("business", "price", BizInfo[id][bizPrice], BizInfo[id][bizID]);
			SendSyntaxMessage(playerid, "Você alterou o preço do seu negócio [ID: %i] para $%i.", id, BizInfo[id][bizPrice]);

		}

		if (CompareStrings(parameters, "vehspawn"))
		{
			if(BizInfo[id][bizType] != BIZ_AUTOSALON || BizInfo[id][bizType] != BIZ_BOATSALON || BizInfo[id][bizType] != BIZ_FLYSALON) 
				return 1;
			
			new Float: posX, Float: posY, Float: posZ, Float: posR;
			GetPlayerPos(playerid, posX, posY, posZ);
			GetPlayerFacingAngle(playerid, posR);

			if (GetDistanceBetweenPoints(posX, posY, posZ, BizInfo[id][bizSpawn][0], BizInfo[id][bizSpawn][1], BizInfo[id][bizSpawn][2]) > 30.0) 
				return SendErrorMessage(playerid, "Muito longe da empresa!");

			GetPlayerPos(playerid, BizInfo[id][bizSpawn][0], BizInfo[id][bizSpawn][1], BizInfo[id][bizSpawn][2]);
			BizInfo[id][bizSpawnInt] = GetInterior(playerid);
			BizInfo[id][bizSpawnVW] = GetWorld(playerid);
			BizInfo[id][bizSpawn][3] = posR;

			SQL_SetFloat("business", "spawnX", BizInfo[id][bizSpawn][0], BizInfo[id][bizID]);
			SQL_SetFloat("business", "spawnY", BizInfo[id][bizSpawn][1], BizInfo[id][bizID]);
			SQL_SetFloat("business", "spawnZ", BizInfo[id][bizSpawn][2], BizInfo[id][bizID]);
			SQL_SetFloat("business", "spawnR", BizInfo[id][bizSpawn][3], BizInfo[id][bizID]);
			SQL_SetInteger("business", "spawnInt", BizInfo[id][bizSpawnInt], BizInfo[id][bizID]);
			SQL_SetInteger("business", "spawnVW", BizInfo[id][bizSpawnVW], BizInfo[id][bizID]);

			SendSyntaxMessage(playerid, "Você alterou a posição de spawn de veículo da empresa [ID: %i].", id);
		}

		if (CompareStrings(parameters, "produtos"))
		{
			new products;
			
			if (sscanf(arg, "i", products)) 	return SendHelpMessage(playerid, "/abiz editar [ID do negócio] produtos [quantidade]");
			if (products < 1 || products > 500)	return SendErrorMessage(playerid, "Os produtos do negócio podem ser configurados de 1 a 500.");

			BizInfo[id][bizProduct] = products;
			SQL_SetInteger("business", "product", BizInfo[id][bizProduct], BizInfo[id][bizID]);
			SendSyntaxMessage(playerid, "Você definiu o número de produtos do negócio [ID: %i] como %i.", id, BizInfo[id][bizProduct]);
		}

		else if (CompareStrings(parameters, "nivel"))
		{
			new level;

			if (sscanf(arg, "i", level))	return SendHelpMessage(playerid, "/abiz edit [ID do negócio] nível [nível]");
			if (level < 0 || level > 35)	return SendErrorMessage(playerid, "Nível de negócio de 0 a 35.");
						
			BizInfo[id][bizLevelbuy] = level;
			SQL_SetInteger("business", "levelbuy", BizInfo[id][bizLevelbuy], BizInfo[id][bizID]);
			SendSyntaxMessage(playerid, "Você alterou o nível de compra do seu negócio [ID: %i] para %i.", id, BizInfo[id][bizLevelbuy]);
		}

		else if (CompareStrings(parameters, "tipo"))
		{
			new type;

			if (sscanf(arg, "i", type)) 					return SendHelpMessage(playerid, "/abiz edit [ID do negócio] tipo [tipo]");
			if (type < 0 || type > sizeof(biz_types) - 1)	return SendErrorMessage(playerid, "Tipo de negócio de 0 a %i.", sizeof(biz_types) - 1);

			BizInfo[id][bizType] = type;
			SQL_SetInteger("business", "type", BizInfo[id][bizType], BizInfo[id][bizID]);
			SendSyntaxMessage(playerid, "Você alterou o tipo do negócio [ID: %i] para %s.", id, biz_types[BizInfo[id][bizType]]);
		}

		else if (CompareStrings(parameters, "nome"))
		{
			new name[32];
			
			if (sscanf(arg, "s[32]", name))	return SendHelpMessage(playerid, "/abiz editar [ID do negócio] nome [texto]");
			if (strlen(name) > 30)			return SendErrorMessage(playerid, "Nome do negócio com até 30 caracteres.");
			if (CheckTextDrawString(name))	return SendErrorMessage(playerid, "O nome do negócio não deve conter o caractere '~'.");

			format(BizInfo[id][bizName], 32, "%s", name);
			SQL_SetString("business", "name", BizInfo[id][bizName], BizInfo[id][bizID]);
			SendSyntaxMessage(playerid, "Você alterou o nome do negócio [ID: %i] para %s.", id, BizInfo[id][bizName]);
		}

		else if (CompareStrings(parameters, "taxas"))
		{
			new fee;

			if (sscanf(arg, "i", fee))		return SendHelpMessage(playerid, "/abiz editar [ID do negócio] taxas [preço]");
			if (fee < 0 || fee > 100)		return SendErrorMessage(playerid, "Taxa de entrada de $0 a $100.");

			BizInfo[id][bizEntrance] = fee;
			SQL_SetInteger("business", "entrance", BizInfo[id][bizEntrance], BizInfo[id][bizID]);
			SendSyntaxMessage(playerid, "Você alterou o preço de entrada do negócio [ID: %i] para $%i.", id, BizInfo[id][bizEntrance]);

		}

		else if (CompareStrings(parameters, "dinheiro"))
		{
			new cash;

			if (sscanf(arg, "i", cash))		return SendHelpMessage(playerid, "/abiz editar [ID do negócio] dinheiro [dinheiro]");
			if (cash < 0 || cash > 5000000)	return SendErrorMessage(playerid, "Dinheiro de $0 a $5000000.");

			BizInfo[id][bizCash] = cash;
			SQL_SetInteger("business", "cash", BizInfo[id][bizCash], BizInfo[id][bizID]);
			SendSyntaxMessage(playerid, "Você alterou o dinheiro no caixa do negócio [ID: %i] para $%i.", id, BizInfo[id][bizCash]);

		}

		else if (CompareStrings(parameters, "freeze"))
		{
			new freeze;

			if (sscanf(arg, "i", freeze)) return SendHelpMessage(playerid, "/abiz editar [ID do negócio] congelar [congelar]");
			if (freeze < 0 || freeze > 1) return SendErrorMessage(playerid, "Congelar de 0 a 1.");

			BizInfo[id][bizFreeze] = freeze;
			SQL_SetInteger("business", "freeze", BizInfo[id][bizFreeze], BizInfo[id][bizID]);

			
			new freeze_naming[4];

			if (BizInfo[id][bizFreeze]) format(freeze_naming, sizeof(freeze_naming), "Não");
			else 						format(freeze_naming, sizeof(freeze_naming), "Sim");
		
			SendSyntaxMessage(playerid, "Você alterou a capacidade de comprar um negócio [ID: %i] para - %s.", id, freeze_naming);
		}

		else if (CompareStrings(parameters, "entrada"))
		{
			if (IsValidDynamicPickup(BizInfo[id][bizPickUp])) DestroyDynamicPickup(BizInfo[id][bizPickUp]);

			GetPlayerPos(playerid, BizInfo[id][bizPosX], BizInfo[id][bizPosY], BizInfo[id][bizPosZ]);
			BizInfo[id][bizPosInt] = GetInterior(playerid);
			BizInfo[id][bizPosVW] = GetWorld(playerid);

			UpdateBusinessDoor(id);
			BizInfo[id][bizPickUp] = CreateDynamicPickup(1239, 1, BizInfo[id][bizPosX], BizInfo[id][bizPosY], BizInfo[id][bizPosZ], BizInfo[id][bizPosVW], BizInfo[id][bizPosInt], -1, 100.0);

			SQL_SetFloat("business", "posx", BizInfo[id][bizPosX], BizInfo[id][bizID]);
			SQL_SetFloat("business", "posy", BizInfo[id][bizPosY], BizInfo[id][bizID]);
			SQL_SetFloat("business", "posz", BizInfo[id][bizPosZ], BizInfo[id][bizID]);
			SQL_SetInteger("business", "posvw", BizInfo[id][bizPosVW], BizInfo[id][bizID]);
			SQL_SetInteger("business", "posint", BizInfo[id][bizPosInt], BizInfo[id][bizID]);
			
			SendSyntaxMessage(playerid, "You have moved your business entry [ID: %i].", id);
		}

		else if (CompareStrings(parameters, "saida"))
		{
			GetPlayerPos(playerid, BizInfo[id][bizExitX], BizInfo[id][bizExitY], BizInfo[id][bizExitZ]);
			BizInfo[id][bizExitInt] = GetInterior(playerid);
			BizInfo[id][bizExitVW] = GetWorld(playerid);

			SQL_SetFloat("business", "exitx", BizInfo[id][bizExitX], BizInfo[id][bizID]);
			SQL_SetFloat("business", "exity", BizInfo[id][bizExitY], BizInfo[id][bizID]);
			SQL_SetFloat("business", "exitz", BizInfo[id][bizExitZ], BizInfo[id][bizID]);
			SQL_SetInteger("business", "exitint", BizInfo[id][bizExitInt], BizInfo[id][bizID]);
			SQL_SetInteger("business", "exitvw", BizInfo[id][bizExitVW], BizInfo[id][bizID]);

			SendSyntaxMessage(playerid, "Você moveu a entrada do seu negócio [ID: %i].", id);

		}

		else if (CompareStrings(parameters, "int"))
		{
			new string[3072];

			for(new i = 0; i < MAX_INTERIORS; i++)
			{
				if (!AdminTeles[i][telesOn] || BUSSINESS_INT_TYPE != AdminTeles[i][telesType]) continue;
				format(string, sizeof(string), "%s\n<%i> {66FF66}%s", string, i, AdminTeles[i][telesMapName]);
			}

			if (isempty(string)) SendErrorMessage(playerid, "Interiores para empresas não encontrados.");

			SetPVarInt(playerid, "biz_change", id);
			Dialog_Show(playerid, ChangeBizInterior, DIALOG_STYLE_LIST, "{FFFFFF}Interior para empresas", string, "Selecionar", "Cancelar");

		}

		else
		{
			SendHelpMessage(playerid, "/abiz edit [ID business] [options] [value]");
			SendSyntaxMessage(playerid, "preco / produtos / nivel / tipo / nome / taxas / dinheiro / freeze / entrada / saida / int");
			return 1;
		}
	}

	else if (CompareStrings(option, "vender"))
	{
		new bizid;

		if (sscanf(secoption, "i", bizid)) return SendHelpMessage(playerid, "/abiz vender [ID da empresa]");
		if (bizid < 0 || bizid > MAX_BIZ - 1 || !BizInfo[bizid][bizID]) return SendErrorMessage(playerid, "Empresa não encontrada.");
		if (!BizInfo[bizid][bizOwnerID]) return SendErrorMessage(playerid, "Esta empresa já está à venda.");


		new ownerid = GetIDByID(BizInfo[bizid][bizOwnerID]);

		if (IsPlayerLogged(ownerid))
		{
			SendSyntaxMessage(ownerid, "O administrador %s vendeu a sua empresa.", GetAccName(playerid));
			SendSyntaxMessage(ownerid, "Você foi reembolsado pela sua compra [$%i] e pelo dinheiro do caixa da empresa [$%i].", BizInfo[bizid][bizPrice], BizInfo[bizid][bizCash]);
			GiveCash(ownerid, BizInfo[bizid][bizPrice]);
			if (BizInfo[bizid][bizCash] > 0) GiveCash(ownerid, BizInfo[bizid][bizCash]);
		}
		else
		{
			new biz_cost = BizInfo[bizid][bizPrice];

			if (BizInfo[bizid][bizCash] > 0) biz_cost += BizInfo[bizid][bizCash];

			new query[128];
			mysql_format(dbHandle, query, sizeof(query), "SELECT `cash` FROM `users` WHERE `id` = %i", BizInfo[bizid][bizOwnerID]);
			mysql_tquery(dbHandle, query, "GivePlayerBizMoney", "ii", biz_cost, BizInfo[bizid][bizOwnerID]);	
		}

		Sell_Business(bizid);
		SendSyntaxMessage(playerid, "Você colocou a empresa à venda [ID: %i].", bizid);
	}

	else if (CompareStrings(option, "ir"))
	{
		new bizid; 

		if (sscanf(secoption, "i", bizid))								return SendHelpMessage(playerid, "/abiz ir [ID da empresa]");
		if (bizid < 0 || bizid > MAX_BIZ - 1 || !BizInfo[bizid][bizID])	return SendErrorMessage(playerid, "Empresa não encontrada.");

		SetPlayerPosEx(playerid, BizInfo[bizid][bizPosX], BizInfo[bizid][bizPosY], BizInfo[bizid][bizPosZ]);
		SetInterior(playerid, BizInfo[bizid][bizPosInt]);
		SetWorld(playerid, BizInfo[bizid][bizPosVW]);
		SendSpecialMessage(playerid, "Você se teleportou para empresa [ID: %i].", bizid);
	}

	else if (CompareStrings(option, "tipos"))
	{
		new dialog_header[64] = "{FFFFFF}Nome\t{FFFFFF}ID\n", dialog_body[1024];

		format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

		for(new i = 0; i != sizeof(biz_types); i++)
		{
			format(dialog_body, sizeof(dialog_body), "%s%s\t%i\n", dialog_body, biz_types[i], i);
		}

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Tipos de empresa", dialog_body, "Fechar", "");
	}
	
	else if (CompareStrings(option, "lista"))
	{
		pTemp[playerid][pDialogList] = 0;
		DisplayBizList(playerid);
	}

	else
	{
		SendHelpMessage(playerid, "/abiz [criar / remover / editar / ir / vender / tipos / lista]");
	}
	return 1;
}

CMD:aapart(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/aapart [criar / remover / int / entrada / saida / ir]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new apart;

		if ((apart = GetNextApartID()) == -1)	return SendErrorMessage(playerid, "Limite máximo de complexos residenciais alcançado.");

		GetPlayerPos(playerid, ApartInfo[apart][aPosX], ApartInfo[apart][aPosY], ApartInfo[apart][aPosZ]);
		ApartInfo[apart][aPosInt] = GetInterior(playerid);
		ApartInfo[apart][aPosVW] = GetWorld(playerid);
		ApartInfo[apart][aApartOn] = 1;
		ApartInfo[apart][aExitVW] = apart + 1000;

		AddApartToFile(apart, ApartInfo[apart][aPosX], ApartInfo[apart][aPosY], ApartInfo[apart][aPosZ], ApartInfo[apart][aPosInt], ApartInfo[apart][aPosVW]);
		SetAparmentInterior(playerid, apart, 1);
		
		ApartInfo[apart][aCheckPoint] = CreateDynamicCP(ApartInfo[apart][aPosX], ApartInfo[apart][aPosY], ApartInfo[apart][aPosZ], 1.0, ApartInfo[apart][aPosVW], ApartInfo[apart][aPosInt], -1, 2.0);
	}

	else if (CompareStrings(option, "remover"))
	{
		new id; 

		if ((id = GetNearApartmentOutside(playerid)) == -1)
    		return SendErrorMessage(playerid, "Não há apartamentos próximos a você.");

		DestroyDynamicCP(ApartInfo[id][aCheckPoint]);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `apartment` WHERE `id` = %i", ApartInfo[id][aID]);
		mysql_tquery(dbHandle, query, "OnApartRemove", "i", id);

		SendSyntaxMessage(playerid, "Apartamento [ID: %i] deletado.", id);
	}

	else if (CompareStrings(option, "int"))
	{
		new id, interior;

		if ((id = GetNearApartmentOutside(playerid)) == -1)
		    return SendErrorMessage(playerid, "Não há apartamentos próximos a você.");
		if (sscanf(secoption, "i", interior))
		{
		    SendHelpMessage(playerid, "/aapart int [1-17]");
			//SendSyntaxMessage(playerid, "1: Ghetto Hotel 2: Ghetto Hotel(2) 3: Hotel 4: Hotel(2) 5: Apartment �1 (�����)");
			//SendSyntaxMessage(playerid, "6: Apartment �2 (�����) 7: Apartment �3 (�����) 8: Apartment �4 (��������� �����)");
			//SendSyntaxMessage(playerid, "9: Apartment �5 (�����) 10: Apartment �6 (����������) 11: Apartment �7 (�����) 12: Apartment �8 (�����)");
		    SendSyntaxMessage(playerid, "13: Apartamento 9 (Gueto) 14: Apartamento 10 (Elite) 15: Entrada (por Umka) 16: Motel 17: Motel (2)");
		    return 1;
		}
		if (interior < 1 || interior > 17)
    		return SendErrorMessage(playerid, "ID de interior de 1 a 17.");

		SetAparmentInterior(playerid, id, interior);
	}

	else if (CompareStrings(option, "entrada"))
	{
		new apartid;

		if (sscanf(secoption, "i", apartid))
		    return SendHelpMessage(playerid, "/aapart entrar [ID do Apartamento]");
		if (apartid < 0 || apartid > MAX_APART - 1 || !ApartInfo[apartid][aApartOn])
		    return SendErrorMessage(playerid, "Apartamento não encontrado.");


		if (IsValidDynamicCP(ApartInfo[apartid][aCheckPoint]))	DestroyDynamicCP(ApartInfo[apartid][aCheckPoint]);

		GetPlayerPos(playerid, ApartInfo[apartid][aPosX], ApartInfo[apartid][aPosY], ApartInfo[apartid][aPosZ]);
		ApartInfo[apartid][aPosInt] = GetInterior(playerid);
		ApartInfo[apartid][aPosVW] = GetWorld(playerid);

		SQL_SetFloat("apartment", "posx", ApartInfo[apartid][aPosX], ApartInfo[apartid][aID]);
		SQL_SetFloat("apartment", "posy", ApartInfo[apartid][aPosY], ApartInfo[apartid][aID]);
		SQL_SetFloat("apartment", "posz", ApartInfo[apartid][aPosZ], ApartInfo[apartid][aID]);
		SQL_SetInteger("apartment", "posint", ApartInfo[apartid][aPosInt], ApartInfo[apartid][aID]);
		SQL_SetInteger("apartment", "posvw", ApartInfo[apartid][aPosVW], ApartInfo[apartid][aID]);

		ApartInfo[apartid][aCheckPoint] = CreateDynamicCP(ApartInfo[apartid][aPosX], ApartInfo[apartid][aPosY], ApartInfo[apartid][aPosZ], 1.0, ApartInfo[apartid][aPosVW], ApartInfo[apartid][aPosInt], -1, 2.0);
		SendSyntaxMessage(playerid, "Você alterou a entrada do apartamento [ID: %i].", apartid);
	}

	else if (CompareStrings(option, "saida"))
	{
		new apartid;

		if (sscanf(secoption, "i", apartid))
		    return SendHelpMessage(playerid, "/aapart sair [ID do Apartamento]");
		if (apartid < 0 || apartid > MAX_APART - 1 || !ApartInfo[apartid][aApartOn])
		    return SendErrorMessage(playerid, "Apartamento não encontrado.");


		GetPlayerPos(playerid, ApartInfo[apartid][aExitX], ApartInfo[apartid][aExitY], ApartInfo[apartid][aExitZ]);
		ApartInfo[apartid][aExitInt] = GetInterior(playerid);
		ApartInfo[apartid][aExitVW] = GetWorld(playerid);

		SQL_SetFloat("apartment", "exitx", ApartInfo[apartid][aExitX], ApartInfo[apartid][aID]);
		SQL_SetFloat("apartment", "exity", ApartInfo[apartid][aExitY], ApartInfo[apartid][aID]);
		SQL_SetFloat("apartment", "exitz", ApartInfo[apartid][aExitZ], ApartInfo[apartid][aID]);
		SQL_SetInteger("apartment", "exitint", ApartInfo[apartid][aExitInt], ApartInfo[apartid][aID]);
		SQL_SetInteger("apartment", "exitvw", ApartInfo[apartid][aExitVW], ApartInfo[apartid][aID]);

		SendSyntaxMessage(playerid, "Você alterou a saida do apartamento [ID: %i].", apartid);
	}

	else if (CompareStrings(option, "ir"))
	{
		new apartid;

		if (sscanf(secoption, "i", apartid))
		    return SendHelpMessage(playerid, "/aapart ir [ID do Apartamento]");
		if (apartid < 0 || apartid > MAX_APART - 1 || !ApartInfo[apartid][aApartOn])
		    return SendErrorMessage(playerid, "Apartamento não encontrado.");


		SetPlayerPosEx(playerid, ApartInfo[apartid][aPosX], ApartInfo[apartid][aPosY], ApartInfo[apartid][aPosZ]);
		SetInterior(playerid, ApartInfo[apartid][aPosInt]);
		SetWorld(playerid, ApartInfo[apartid][aPosVW]);
		SendSpecialMessage(playerid, "Você se teleportou para o apartamento [ID: %i].", apartid);
	}
	
	else 
	{
		SendHelpMessage(playerid, "/aapart [criar/remover/int/entrada/saida/ir]");
	}

	return 1;
}

CMD:changename(playerid, params[])
{
	new userid, newname[MAX_PLAYER_NAME];
	 
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3)) 			 return SendNotAdmin(playerid);
	if (sscanf(params, "us[24]", userid, newname)) 	 return SendHelpMessage(playerid, "/mudarnome [ID/Parte do Nome] [novo nome]");
	if (!IsPlayerLogged(userid)) 						 return SendNotConnected(playerid);
	if (!IsValidPlayerName(newname)) 					 return SendErrorMessage(playerid, "Formato de nome inválido.");
	if (strlen(newname) < 4 || strlen(newname) > 24)	 return SendErrorMessage(playerid, "Comprimento do nome deve ser entre 4 a 24 caracteres.");
	    
	new query[128];
    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", newname);
	mysql_tquery(dbHandle, query, "OnAdminChangeName", "iis", playerid, userid, newname);
	return 1;
}

CMD:adoor(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_3))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/adoor [criar / remover / faccao / veiculo / texto / entrada / saida / ir / nomegps]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new enterid = -1;

		for(new i = 0; i < MAX_ENTRANCE; i++)
		{
			if (EntranceInfo[i][entranceOn])	continue;

			enterid = i;
			break;
		}

		if (enterid == -1)	return SendErrorMessage(playerid, "Número máximo de entradas/saídas atingido.");

		GetPlayerPos(playerid, EntranceInfo[enterid][eEnterX], EntranceInfo[enterid][eEnterY], EntranceInfo[enterid][eEnterZ]);
		GetPlayerFacingAngle(playerid, EntranceInfo[enterid][eEnterA]);

	    EntranceInfo[enterid][eEnterInt] = GetInterior(playerid);
	    EntranceInfo[enterid][eEnterVW] = GetWorld(playerid);

	    GetPlayerPos(playerid, EntranceInfo[enterid][eExitX], EntranceInfo[enterid][eExitY], EntranceInfo[enterid][eExitZ]);
		GetPlayerFacingAngle(playerid, EntranceInfo[enterid][eExitA]);

	    EntranceInfo[enterid][eExitInt] = GetInterior(playerid);
	    EntranceInfo[enterid][eExitVW] = GetWorld(playerid);
   		EntranceInfo[enterid][eEnterPickup] = CreateDynamicPickup(1239, 1, EntranceInfo[enterid][eEnterX], EntranceInfo[enterid][eEnterY], EntranceInfo[enterid][eEnterZ], EntranceInfo[enterid][eEnterVW], EntranceInfo[enterid][eEnterInt], -1, 100.0);
		EntranceInfo[enterid][eExitPickup] = CreateDynamicPickup(1239, 1, EntranceInfo[enterid][eExitX], EntranceInfo[enterid][eExitY], EntranceInfo[enterid][eExitZ], EntranceInfo[enterid][eExitVW], EntranceInfo[enterid][eExitInt], -1, 100.0);
		EntranceInfo[enterid][entranceOn] = 1;

    	new query[156];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `entrance` (`enterx`,`entery`,`enterz`,`entera`,`entervw`,`enterint`) VALUES (%f,%f,%f,%f,%i,%i)", EntranceInfo[enterid][eEnterX], EntranceInfo[enterid][eEnterY], EntranceInfo[enterid][eEnterZ], EntranceInfo[enterid][eEnterA], EntranceInfo[enterid][eEnterVW], EntranceInfo[enterid][eEnterInt]);
		mysql_tquery(dbHandle, query, "OnInsertEntrance", "i", enterid);

    	SendSyntaxMessage(playerid, "Você criou uma entrada/saída [ID: %i].", enterid);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new enterid;

		if (sscanf(secoption, "i", enterid))	return SendHelpMessage(playerid, "/adoor remover [ID Entrada/Saída]");
		if (!EntranceInfo[enterid][entranceOn])	return SendErrorMessage(playerid, "Entrada/Saída não encontrada.");


		if (IsValidDynamicPickup(EntranceInfo[enterid][eEnterPickup]))	DestroyDynamicPickup(EntranceInfo[enterid][eEnterPickup]);
		if (IsValidDynamicPickup(EntranceInfo[enterid][eExitPickup]))	DestroyDynamicPickup(EntranceInfo[enterid][eExitPickup]);

		EntranceInfo[enterid][entranceOn] = 0;

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `entrance` WHERE `id` = %i", EntranceInfo[enterid][eID]);
		mysql_tquery(dbHandle, query);

		SendSyntaxMessage(playerid, "Entrada/Saída [ID: %i] removida.", enterid);
		return 1;
	}

	else if (CompareStrings(option, "faccao"))
	{
		new enterid, faction = 0;

		if (sscanf(secoption, "iI(0)", enterid, faction)) return SendHelpMessage(playerid, "/adoor faction [ID de Entrada/Saída] {c7c7c7}[Facção | Deixe em branco ou especifique 0 para liberar a Entrada/Saída]");
		if (faction < 0 || faction >= MAX_FACTIONS || (faction != 0 && !Factions[faction][fON])) return SendErrorMessage(playerid, "Facção não encontrada.");
		if (!EntranceInfo[enterid][entranceOn]) return SendErrorMessage(playerid, "Entrada/Saída não encontrada.");


		EntranceInfo[enterid][eFaction] = faction;
		SQL_SetInteger("entrance", "faction", EntranceInfo[enterid][eFaction], EntranceInfo[enterid][eID]);
		SendSyntaxMessage(playerid, "Você alterou a facção da sua Entrada/Saída [ID: %i] para %i.", enterid, EntranceInfo[enterid][eFaction]);
		return 1;
	}

	else if (CompareStrings(option, "veiculo"))
	{
		new enterid, vehicle;

		if (sscanf(secoption, "ii", enterid, vehicle)) return SendHelpMessage(playerid, "/adoor vehicle [ID da Entrada/Porta] [acesso de veículo (0 - fechado | 1 - aberto)]");
		if (vehicle < 0 || vehicle > 1) return SendErrorMessage(playerid, "Valor de 0 a 1.");
		if (!EntranceInfo[enterid][entranceOn]) return SendErrorMessage(playerid, "Entrada/Porta não encontrada.");


		EntranceInfo[enterid][eVehicle] = vehicle;
		SQL_SetInteger("entrance", "vehicle", EntranceInfo[enterid][eVehicle], EntranceInfo[enterid][eID]);

		new entrance_state[16];

		if (EntranceInfo[enterid][eVehicle])	format(entrance_state, sizeof(entrance_state), "permitido");
		else									format(entrance_state, sizeof(entrance_state), "banido");

		SendSyntaxMessage(playerid, "Você %s o acesso de veículo para a entrada/saída [ID: %i].", entrance_state, enterid);
		return 1;
	}
	
	else if (CompareStrings(option, "texto"))
	{
		new enterid, text[128];

		if (sscanf(secoption, "is[128]", enterid, text)) return SendHelpMessage(playerid, "/adoor text [ID Entrada/Saída] [texto]");
		if (!EntranceInfo[enterid][entranceOn]) return SendErrorMessage(playerid, "Entrada/Saída não encontrada.");
		if (strlen(text) < 1 || strlen(text) > 127) return SendErrorMessage(playerid, "Comprimento do texto de 1 a 127.");

		format(EntranceInfo[enterid][eText], 128, "%s", text);
		SQL_SetString("entrance", "text", EntranceInfo[enterid][eText], EntranceInfo[enterid][eID]);
		SendSyntaxMessage(playerid, "Você alterou o texto da entrada [ID: %i] para %s.", enterid, EntranceInfo[enterid][eText]);
		return 1;
	}

	else if (CompareStrings(option, "entrada"))
	{
		new enterid;

		if (sscanf(secoption, "i", enterid))	return SendHelpMessage(playerid, "/adoor entrada [Entrada / saida ID]");
		if (!EntranceInfo[enterid][entranceOn])	return SendErrorMessage(playerid, "Entrada / Saida não encontrada.");

		GetPlayerPos(playerid, EntranceInfo[enterid][eEnterX], EntranceInfo[enterid][eEnterY], EntranceInfo[enterid][eEnterZ]);
		GetPlayerFacingAngle(playerid, EntranceInfo[enterid][eEnterA]);

		EntranceInfo[enterid][eEnterInt] = GetInterior(playerid);
		EntranceInfo[enterid][eEnterVW] = GetWorld(playerid);

		if (IsValidDynamicPickup(EntranceInfo[enterid][eEnterPickup]))	DestroyDynamicPickup(EntranceInfo[enterid][eEnterPickup]);

		EntranceInfo[enterid][eEnterPickup] = CreateDynamicPickup(1239, 1, EntranceInfo[enterid][eEnterX], EntranceInfo[enterid][eEnterY], EntranceInfo[enterid][eEnterZ], EntranceInfo[enterid][eEnterVW], EntranceInfo[enterid][eEnterInt], -1, 100.0);

		SQL_SetFloat("entrance", "enterx", EntranceInfo[enterid][eEnterX], EntranceInfo[enterid][eID]);
		SQL_SetFloat("entrance", "entery", EntranceInfo[enterid][eEnterY], EntranceInfo[enterid][eID]);
		SQL_SetFloat("entrance", "enterz", EntranceInfo[enterid][eEnterZ], EntranceInfo[enterid][eID]);
		SQL_SetFloat("entrance", "entera", EntranceInfo[enterid][eEnterA], EntranceInfo[enterid][eID]);
		SQL_SetInteger("entrance", "entervw", EntranceInfo[enterid][eEnterVW], EntranceInfo[enterid][eID]);
		SQL_SetInteger("entrance", "enterint", EntranceInfo[enterid][eEnterInt], EntranceInfo[enterid][eID]);
		
		SendSyntaxMessage(playerid, "You have changed the entry position [ID: %i].", enterid);
		return 1;
	}

	else if (CompareStrings(option, "saida"))
	{
		new enterid;

		if (sscanf(secoption, "i", enterid))	return SendHelpMessage(playerid, "/adoor saida [Entrada / saida ID]");
		if (!EntranceInfo[enterid][entranceOn])	return SendErrorMessage(playerid, "Entrada / Saida não encontrada.");

		GetPlayerPos(playerid, EntranceInfo[enterid][eExitX], EntranceInfo[enterid][eExitY], EntranceInfo[enterid][eExitZ]);
		GetPlayerFacingAngle(playerid, EntranceInfo[enterid][eExitA]);

		EntranceInfo[enterid][eExitInt] = GetInterior(playerid);
		EntranceInfo[enterid][eExitVW] = GetWorld(playerid);

		if (IsValidDynamicPickup(EntranceInfo[enterid][eExitPickup]))	DestroyDynamicPickup(EntranceInfo[enterid][eExitPickup]);

		EntranceInfo[enterid][eExitPickup] = CreateDynamicPickup(1239, 1, EntranceInfo[enterid][eExitX], EntranceInfo[enterid][eExitY], EntranceInfo[enterid][eExitZ], EntranceInfo[enterid][eExitVW], EntranceInfo[enterid][eExitInt], -1, 100.0);

		SQL_SetFloat("entrance", "exitx", EntranceInfo[enterid][eExitX], EntranceInfo[enterid][eID]);
		SQL_SetFloat("entrance", "exity", EntranceInfo[enterid][eExitY], EntranceInfo[enterid][eID]);
		SQL_SetFloat("entrance", "exitz", EntranceInfo[enterid][eExitZ], EntranceInfo[enterid][eID]);
		SQL_SetFloat("entrance", "exita", EntranceInfo[enterid][eExitA], EntranceInfo[enterid][eID]);
		SQL_SetInteger("entrance", "exitvw", EntranceInfo[enterid][eExitVW], EntranceInfo[enterid][eID]);
		SQL_SetInteger("entrance", "exitint", EntranceInfo[enterid][eExitInt], EntranceInfo[enterid][eID]);
		
		SendSyntaxMessage(playerid, "Você alterou a posição de saida [ID: %i].", enterid);
		return 1;
	}

	else if (CompareStrings(option, "goto"))
	{
		new enterid;

		if (sscanf(secoption, "i", enterid)) return SendHelpMessage(playerid, "/adoor goto [ID entrada/saída]");
		if (!EntranceInfo[enterid][entranceOn]) return SendErrorMessage(playerid, "Entrada/Saída não encontrada.");
		if (enterid >= MAX_ENTRANCE || enterid < 0) return SendErrorMessage(playerid, "De 0 a %i.", MAX_ENTRANCE);

		SetPlayerPosEx(playerid, EntranceInfo[enterid][eEnterX], EntranceInfo[enterid][eEnterY], EntranceInfo[enterid][eEnterZ]);
		SetInterior(playerid, EntranceInfo[enterid][eEnterInt]);
		SetWorld(playerid, EntranceInfo[enterid][eEnterVW]);
	}

	else if (CompareStrings(option, "nomegps"))
	{
		new enterid, text[128];

		if (sscanf(secoption, "is[128]", enterid, text)) return SendHelpMessage(playerid, "/adoor nomegps [ID Entrada/Saída] [texto]");
		if (!EntranceInfo[enterid][entranceOn]) return SendErrorMessage(playerid, "Entrada/Saída não encontrada.");
		if (strlen(text) < 1 || strlen(text) > 127) return SendErrorMessage(playerid, "Comprimento do texto de 1 a 127.");

		format(EntranceInfo[enterid][gpsText], 128, "%s", text);
		SQL_SetString("entrance", "gpsname", EntranceInfo[enterid][gpsText], EntranceInfo[enterid][eID]);
		SendSyntaxMessage(playerid, "Você alterou o nome no GPS da entrada / saida [ID: %i] para %s.", enterid, EntranceInfo[enterid][gpsText]);
		return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "/adoor [criar / remover / faccao / veiculo / texto / entrada / saida / ir / nomegps]");
	}
	return 1;
}

