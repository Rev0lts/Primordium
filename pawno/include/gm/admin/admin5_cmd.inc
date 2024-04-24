CMD:atrash(playerid, params[])
{
	if (!CheckAdmin(playerid, DEVELOPER_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/atrash [criar / remover / editar / ir]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new type;

		if (sscanf(secoption, "i", type))    return SendHelpMessage(playerid, "/atrash criar [1 - lata de lixo | 2 - Container de lixo]");
		if (type < 1 || type > 2)		    return SendErrorMessage(playerid, "Selecione uma opção entre 1 e 2.");

        if (GetPlayerInterior(playerid) != 0 ||
            GetPlayerVirtualWorld(playerid) != 0) return SendErrorMessage(playerid, "Você deve estar fora de um interior.");

        new id = Trash_CheckFreeSlot();
		if (id == -1) return SendErrorMessage(playerid, "O servidor atingiu o limte máximo de latas de lixo que podem ser criadas.");

		GetPlayerPos(playerid, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]);
		TrashData[id][TrashX] += 0.5;

        TrashData[id][TrashType] = (type == 1) ? (TYPE_BIN) : (TYPE_DUMPSTER);
       	TrashData[id][TrashLevel] = (TrashData[id][TrashType] == TYPE_BIN) ? 1 : 2;

		new label[64];
        format(label, sizeof(label), "%s", (TrashData[id][TrashType] == TYPE_BIN) ? ("Lixeira") : ("Container de resíduos"));
		TrashData[id][TrashLabel] = CreateDynamic3DTextLabel(label, 0x2ECC71FF, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]+1.25, 15.0, .testlos = 1);
        
        new trashModel = (TrashData[id][TrashType] == TYPE_DUMPSTER) ? 1344 : 1328;
        TrashData[id][TrashObject] = CreateDynamicObject(trashModel, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ], 0.0, 0.0, TrashData[id][TrashRZ], 0, 0);

		new query[256];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `trashes` (type,x,y,z,rz) VALUES (%i,%f,%f,%f,%f)", 
            TrashData[id][TrashType],
            TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ], TrashData[id][TrashRZ]
        );
		mysql_tquery(dbHandle, query, "OnTrashInsert", "d", id);

        SetPVarInt(playerid, #edit_trash, id + 1);
		EditDynamicObject(playerid, TrashData[id][TrashObject]);

		SendSyntaxMessage(playerid, "Você criou uma lixeira do TIPO %s [ ID: %i ].", (TrashData[id][TrashType] == TYPE_BIN)?("normal"):("container"), id);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new id;
		
		if (sscanf(secoption, "i", id))						        return SendHelpMessage(playerid, "/atrash remover [Lixo ID]");
		if (id < 0 || id >= MAX_TRASHES || !TrashData[id][TrashID])	return SendErrorMessage(playerid, "Lixera não encontrada.");

		if (IsValidDynamicObject(TrashData[id][TrashObject]))
		{
			DestroyDynamicObject(TrashData[id][TrashObject]);
			TrashData[id][TrashObject] = INVALID_OBJECT_ID;
		}
        
        if (IsValidDynamic3DTextLabel(TrashData[id][TrashLabel]))
		{
			DestroyDynamic3DTextLabel(TrashData[id][TrashLabel]);
			TrashData[id][TrashLabel] = Text3D:INVALID_3DTEXT_ID;
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `trashes` WHERE `id` = %i", TrashData[id][TrashID]);
		mysql_tquery(dbHandle, query);

		TrashData[id][TrashID] = 0;
		SendAdmMessageToPlayer(playerid, "Você removeu a lixeira [ID: %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id;

		if (sscanf(secoption, "d", id))						        return SendHelpMessage(playerid, "/atrash editar [Lixo ID]");
		if (id < 0 || id > MAX_TRASHES || !TrashData[id][TrashID])	return SendErrorMessage(playerid, "Lixera não encontrada.");
		if (Trash_Nearest(playerid) != id)						    return SendErrorMessage(playerid, "Você deve estar proximo a uma lixeira.");

		SetPVarInt(playerid, #edit_trash, id + 1);
		EditDynamicObject(playerid, TrashData[id][TrashObject]);
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		new id;

		if (sscanf(secoption, "i", id))						        return SendHelpMessage(playerid, "/atrash ir [Lixo ID]");
		if (id < 0 || id > MAX_TRASHES || !TrashData[id][TrashID])	return SendErrorMessage(playerid, "Lixera não encontrada.");

		SetPlayerPosEx(playerid, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]);
		SetWorld(playerid, 0);
		SetInterior(playerid, 0);
		return 1;
	}
	
	else
	{
		SendHelpMessage(playerid, "/atrash [criar / remover / editar / ir]");
	}
	return 1;
}

CMD:serverinfo(playerid, params[])
{
	new developer[64];
	if (!CheckAdmin(playerid, DEVELOPER_LEVEL))	return SendNotAdmin(playerid);
	format(developer, sizeof(developer), "~r~~h~%i~g~~h~ 0~b~~h~~h~~h~ 65535", GetServerTickRate());
	TextDrawSetString(Developer, developer); 
	TextDrawShowForPlayer(playerid, Developer);
	return 1;
}


CMD:afactory(playerid, params[])
{
	if (!CheckAdmin(playerid, DEVELOPER_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/afactory [criar / remover / ir]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new name[32];

		if (sscanf(secoption, "S()[32]", name))     return SendHelpMessage(playerid, "/afactory criar [nome]");
		if (strlen(name) < 4 || strlen(name) > 32)	return SendErrorMessage(playerid, "O nome deve ter entre 4 à 32 caracteres.");

        if (GetPlayerInterior(playerid) != 0 ||
            GetPlayerVirtualWorld(playerid) != 0) return SendErrorMessage(playerid, "Você deve estar fora de um interior.");

        new id = Factory_CheckFreeSlot();
		if (id == -1) return SendErrorMessage(playerid, "O servidor atingiu o limte máximo de processadoras de lixo que podem ser criadas.");

		GetPlayerPos(playerid, FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ]);
		format(FactoryData[id][FactoryName], 32, "%s", name);

		new label[128];
        format(label, sizeof(label), "Tratamento de resíduos - %s\n\n{FFFFFF}Quantidade atual de resíduos reciclados: {F39C12}0", FactoryData[id][FactoryName]);
		FactoryData[id][FactoryLabel] = CreateDynamic3DTextLabel(label, 0x2ECC71FF, FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ] + 0.5, 15.0, .testlos = 1);
		FactoryData[id][FactoryCP] = CreateDynamicCP(FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ], 6.0);

		new query[256];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `factories` (name,x,y,z) VALUES ('%e',%f,%f,%f)", 
            FactoryData[id][FactoryName],
            FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ]
        );
		mysql_tquery(dbHandle, query, "OnFactoryInsert", "d", id);

		SendSyntaxMessage(playerid, "Você criou uma processadora de residuos '%s' [ ID: %i ].", FactoryData[id][FactoryName], id);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new id;
		
		if (sscanf(secoption, "i", id))						                return SendHelpMessage(playerid, "/afactory remover [ID]");
		if (id < 0 || id >= MAX_FACTORIES || !FactoryData[id][FactoryID])	return SendErrorMessage(playerid, "ID não encontrado.");

		if (IsValidDynamicCP(FactoryData[id][FactoryCP]))
		{
			DestroyDynamicCP(FactoryData[id][FactoryCP]);
			FactoryData[id][FactoryCP] = INVALID_OBJECT_ID;
		}
        
        if (IsValidDynamic3DTextLabel(FactoryData[id][FactoryLabel]))
		{
			DestroyDynamic3DTextLabel(FactoryData[id][FactoryLabel]);
			FactoryData[id][FactoryLabel] = Text3D:INVALID_3DTEXT_ID;
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `factories` WHERE `id` = %i", TrashData[id][TrashID]);
		mysql_tquery(dbHandle, query);

        FactoryData[id][FactoryCurrent] = 0;
        FactoryData[id][FactoryID] = 0;
		
        SendAdmMessageToPlayer(playerid, "Excluiu a usina de reciclagem de resíduos [ID: %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		new id;

		if (sscanf(secoption, "i", id))						                return SendHelpMessage(playerid, "/afactory ir [ID]");
		if (id < 0 || id >= MAX_FACTORIES || !FactoryData[id][FactoryID])	return SendErrorMessage(playerid, "ID não encontrado.");

		SetPlayerPosEx(playerid, FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ]);
		SetWorld(playerid, 0);
		SetInterior(playerid, 0);
		return 1;
	}
	
	else
	{
		SendHelpMessage(playerid, "/afactory [criar / remover / ir]");
	}
	return 1;
}

CMD:aindustry(playerid, params[])
{
	if (!CheckAdmin(playerid, DEVELOPER_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/aindustry [criar / remover / editar / ir / lista]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new type, name[64];

		if (sscanf(secoption, "is[64]", type, name)) return SendHelpMessage(playerid, "/aindustry create [0 - matérias-primas | 1 - produção | 2 - navio | 3 - requisito de produção] [nome da indústria]");
		if (type < 0 || type > 3)					 return SendErrorMessage(playerid, "Tipo de produção de 0 a 3. ");
		if (strlen(name) < 4 || strlen(name) > 63)	 return SendErrorMessage(playerid, "O número máximo de caracteres é 63. ");
		
		new cargoid = -1;

		for(new i = 0; i < MAX_TRUCK_CARGO; i++)
		{
			if (!TruckCargo[i][tOn]) continue;

			cargoid = i;
			break;
		}

		if (cargoid == -1) return SendErrorMessage(playerid, "O número máximo de industrias foi atingido.");

		GetPlayerPos(playerid, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ]);

		TruckCargo[cargoid][tType] = type;
		TruckCargo[cargoid][tStorageSize] = 200;
		TruckCargo[cargoid][tProductID] =
		TruckCargo[cargoid][tProductAmount] = 
		TruckCargo[cargoid][tPrice] = 
		TruckCargo[cargoid][tStorage] = 
		TruckCargo[cargoid][tPack] = 
		TruckCargo[cargoid][tGps] =
		TruckCargo[cargoid][tLocked] = 0;
		format(TruckCargo[cargoid][tName], 64, "%s", name);

		new mes[128];

		format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nCapacidade %i/%i\nPreço: $%i/unidade.", truckcargo_product[TruckCargo[cargoid][tProductID]], TruckCargo[cargoid][tStorage], TruckCargo[cargoid][tStorageSize], TruckCargo[cargoid][tPrice]);
		TruckCargo[cargoid][tLabel] = CreateDynamic3DTextLabel(mes, 0xFFFFFFFF, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ]+0.8, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1, 100.0);
		TruckCargo[cargoid][tPickup] = CreateDynamicPickup(1318, 1, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ], 0, 0, -1, 100.0);
		TruckCargo[cargoid][tOn] = 1;

		new query[256];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `truck_cargo` (x, y, z, type, name, storage_size) VALUES (%f, %f, %f, %i, '%e', 200)", TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ], 
		type, name);
		mysql_tquery(dbHandle, query, "OnTruckCargoInsert", "d", cargoid);

		SendSyntaxMessage(playerid, "Você criou uma indústria [ Nome: %s | ID: %i | Tipo: %s ].", TruckCargo[cargoid][tName], cargoid, TruckCargo[cargoid][tPack]);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new cargoid, query[128];

		if (sscanf(secoption, "i", cargoid))										return SendHelpMessage(playerid, "/aindustry remove [ID da indústria]");
		if (cargoid < 0 || cargoid >= MAX_TRUCK_CARGO || !TruckCargo[cargoid][tOn])	return SendErrorMessage(playerid, "A indústria não foi encontrada.");
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `truck_cargo` WHERE `id` = %i", TruckCargo[cargoid][tID]);
		mysql_tquery(dbHandle, query);

		if (IsValidDynamic3DTextLabel(TruckCargo[cargoid][tLabel]))
		{
			DestroyDynamic3DTextLabel(TruckCargo[cargoid][tLabel]);
			TruckCargo[cargoid][tLabel] = Text3D:INVALID_3DTEXT_ID;
		}
		if (IsValidDynamicPickup(TruckCargo[cargoid][tPickup]))	DestroyDynamicPickup(TruckCargo[cargoid][tPickup]);

		TruckCargo[cargoid][tOn] = 
		TruckCargo[cargoid][tID] = 0;
		SendSyntaxMessage(playerid, "Industria [ID: %i] apagada.", cargoid);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id, parameters[16], arg[112];

		if (sscanf(secoption, "is[12]S()[112]", id, parameters, arg))
		{
			SendHelpMessage(playerid, "/aindustry editar [ID da indústria] [capacidade / tramanho / preco / unidade / grupo / trancar / gps]");
			return 1;
		}

		if (id < 0 || id >= MAX_TRUCK_CARGO || !TruckCargo[id][tOn])	return SendErrorMessage(playerid, "A indústria não foi encontrada.");

		new mes[128];

		if (CompareStrings(parameters, "capacidade"))
		{
			new storage;

			if (sscanf(arg, "i", storage))								return SendHelpMessage(playerid, "/aindustry editar [ID da indústria] capacidade [numero de produtos em estoque] ");
			if (storage < 0 || storage > TruckCargo[id][tStorageSize])	return SendErrorMessage(playerid, "A quantidade deve ser de 0 à %i.", TruckCargo[id][tStorageSize]);

			TruckCargo[id][tStorage] = storage;
			SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nCapacidade %i/%i\nPreço: $%i/unidade.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
			UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
			SendSyntaxMessage(playerid, "Você alterou a capacidade de protudos no estoque da industria [ID: %i] para %i.", id, TruckCargo[id][tStorage]);
			return 1;
		}

		else if (CompareStrings(parameters, "locked"))
		{
			new locked;

			if (sscanf(arg, "i", locked))	return SendHelpMessage(playerid, "/aindustry editar [ID da indústria] trancar [0 - aberto | 1 - fechado] ");
			if (locked < 0 || locked > 1)	return SendErrorMessage (playerid, "O valor deve ser definido entre 0 e 1.");

			TruckCargo[id][tLocked] = locked;
			SQL_SetInteger("truck_cargo", "locked", TruckCargo[id][tLocked], TruckCargo[id][tID]);
			SendSyntaxMessage(playerid, "Você %s a indústria [ID: %i].", (!locked) ? ("abriu") : ("trancou"), id);
			return 1;
		}

		else if (CompareStrings(parameters, "tipo"))
		{
			new type;

			if (sscanf(arg, "i", type))	return SendHelpMessage(playerid, "/aindustry editar [ID da indústria] tipo [0 - matérias-primas | 1 - produção | 2 - navio | 3 - requisito de produção] ");
			if (type < 0 || type > 3)	return SendErrorMessage(playerid, "Escolha um tipo entre 0 e 3.");

			TruckCargo[id][tType] = type;
			SQL_SetInteger("truck_cargo", "type", TruckCargo[id][tType], TruckCargo[id][tID]);
			SendSyntaxMessage(playerid, "Você alterou o tipo da indústria [ID: %i] para %i.", id, type);
			return 1;
		}
		
		else if (CompareStrings(parameters, "gps"))
		{
			new locked;

			if (sscanf(arg, "i", locked))	return SendHelpMessage(playerid, "/aindustry editar [ID da indústria] gps [0-ocultar/1-mostrar]");
			if (locked < 0 || locked > 1)	return SendErrorMessage(playerid, "Escolha um valor entre 0 e 1.");

			TruckCargo[id][tGps] = locked;
			SQL_SetInteger("truck_cargo", "gps", TruckCargo[id][tGps], TruckCargo[id][tID]);
			SendSyntaxMessage(playerid, "Você %s a indústria [ID: %i] do /tpda.", (!locked) ? ("mostrou") : ("ocultou"), id);
			return 1;
		}

		else if (CompareStrings(parameters, "tamanho"))
		{
			new size;

			if (sscanf(arg, "i", size))		return SendHelpMessage(playerid, "/aindustry editar [ID da indústria] tamanho [1-10000]");
			if (size <= 0 || size > 10000)	return SendErrorMessage(playerid,"Escolha um valor entre 1 e 10000.");

			TruckCargo[id][tStorageSize] = size;
			SQL_SetInteger("truck_cargo", "storage_size", TruckCargo[id][tStorageSize], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nCapacidade %i/%i\nPreço: $%i/unidade.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
        	UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
			SendSyntaxMessage(playerid, "Você alterou a quantidade de armazenamento da indústria [ID: %i] para %i.", id, TruckCargo[id][tStorageSize]);
			return 1;
		}

		else if (CompareStrings(parameters, "preco"))
		{
			new price;

			if (sscanf(arg, "i", price))		return SendHelpMessage(playerid, "/aindustry editar [ID da indústria] preco [cost]");
			if (price < 1 || price > 10000)	return SendErrorMessage(playerid, "O valor deve ser entre $1 e $10000.");
			
			TruckCargo[id][tPrice] = price;
			SQL_SetInteger("truck_cargo", "price", TruckCargo[id][tPrice], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nCapacidade %i/%i\nPreço: $%i/unidade.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
        	UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
			SendSyntaxMessage(playerid, "Você alterou o valor unitário da indústria [ID: %i] para $%i.", id, TruckCargo[id][tPrice]);
			return 1;
		}

		else if (CompareStrings(parameters, "unidade"))
		{
			new unit;

			if (sscanf(arg, "i", unit))		return SendHelpMessage(playerid, "/aindustry editar [ID da indústria] unidade [unidade por hora]");
			if (unit < -100 || unit > 100)	return SendErrorMessage(playerid, "Selecione um valor entre -100 e 100.");

			TruckCargo[id][tProductAmount] = unit;
			SQL_SetInteger("truck_cargo", "product_amount", TruckCargo[id][tProductAmount], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nCapacidade %i/%i\nPreço: $%i/unidade.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
        	UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
			SendSyntaxMessage(playerid, "Você alterou a adição de mercadorias por hora na indústria [ID: %i] para %i.", id, TruckCargo[id][tProductAmount]);
			return 1;
		}

		else if (CompareStrings(parameters, "grupo"))
		{
			new pack;

			if (sscanf(arg, "i", pack))	
			{
				SendHelpMessage(playerid, "/aindustry editar [ID da indústria] grupo [grupo]");
				SendSyntaxMessage(playerid, "Grupo atual da indústria %i.", TruckCargo[id][tPack]);
				return 1;
			}	
			if (pack < 0 || pack > MAX_TRUCK_PACK)	return SendErrorMessage(playerid, "O grupo da indústria deve ser de 0 à %i.", MAX_TRUCK_PACK);

			TruckCargo[id][tPack] = pack;
			SQL_SetInteger("truck_cargo", "pack", TruckCargo[id][tPack], TruckCargo[id][tID]);
			SendSyntaxMessage(playerid, "Você alterou o grupo da indústria [ID: %i] para %i.", id, TruckCargo[id][tPack]);
			return 1;
		}
		
		else 
		{
			SendHelpMessage(playerid, "/aindustry editar [ID da indústria] [capacidade / tamanho / preco / unidade / grupo / trancar / gps]");
			return 1;
		}
	}
	
	else if (CompareStrings(option, "ir"))
	{
		new cargoid;

		if (sscanf(secoption, "i", cargoid))										return SendHelpMessage(playerid, "/aindustry ir [ID da indústria]");
		if (cargoid < 0 || cargoid >= MAX_TRUCK_CARGO || !TruckCargo[cargoid][tOn])	return SendErrorMessage(playerid, "Indústria não encontrada.");

		SetPlayerPosEx(playerid, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ]);
		SetInterior(playerid, 0);
		SetWorld(playerid, 0);
		SendSpecialMessage(playerid, "Você se teleportou para indústria %s [ID: %i].", TruckCargo[cargoid][tName], cargoid);
		return 1;
	}

	else if (CompareStrings(option, "lista"))
	{
		pTemp[playerid][pDialogList] = 0;
		DisplayCargoGroups(playerid);
	}
	return 1;
}

CMD:payday(playerid, params[])
{
	new userid;
	if (!CheckAdmin(playerid, DEVELOPER_LEVEL))		return SendNotAdmin(playerid);
	if (sscanf(params, "u", userid))		return SendHelpMessage(playerid, "/payday [ID/Parte do nome]");
	if (userid == INVALID_PLAYER_ID)		return SendErrorMessage(playerid, "Jogador não encontrado.");
	
	PayDay(userid);
	
	SendAdmMessageToPlayer(userid, "O administrador %s concedeu seu pagamento.", GetAccName(playerid));
	SendAdmMessageToPlayer(playerid, "Você emitiu o pagamento para %s.", GetNameEx(userid));
	return 1;
}

CMD:paydayall(playerid, params[])
{
	if (!CheckAdmin(playerid, DEVELOPER_LEVEL))		return SendNotAdmin(playerid);

	for(new i = 0; i < MAX_PLAYERS; i++)
		PayDay(i);
	
	SendAdmMessageToAll("O administrador %s emitiu o pagamento para todos os jogadores.", GetAccName(playerid));
	return 1;
}

CMD:testmsgs(playerid, params[]) 
{
	if (!CheckAdmin(playerid, DEVELOPER_LEVEL))		return SendNotAdmin(playerid);

	new type, str[64];
	if (sscanf(params, "is[64]", type, str))		return SendHelpMessage(playerid, "/testmsgs [tipo] [txt]");
	if (type < 0 || type > 1)					return 1;

	switch(type)
	{
		case 0: JobMessage(playerid, str, 10);
		case 1: InfoMSG(playerid, str, 10);
	}
	
	return 1;
}