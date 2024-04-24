CMD:tpda(playerid, params[])
{
    if (PlayerInfo[playerid][pJob] != JOB_TRUCKER)     return 1;
    if (!IsPlayerInAnyVehicle(playerid))               return SendErrorMessage(playerid, "Voc� deve estar em um ve�culo.");

    Dialog_Show(playerid, PDAMenu, DIALOG_STYLE_LIST, "{FFFFFF}TPDA - Menu", "{FFFFFF}Mostrar empresas\nMostrar pedidos de todas as empresas\nMostrar informa��es sobre o Navio", "Selecionar", "Cancelar");
    return 1;
}


CMD:trailer(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] != JOB_TRUCKER)	return 1;
    if (!IsPlayerInAnyVehicle(playerid))				return 1;

    new vehicle = GetPlayerVehicleID(playerid), trailerid = GetVehicleTrailer(vehicle);

    if (!IsTruckCar(vehicle))						return 1;
    if (trailerid == 0)								return SendErrorMessage(playerid, "N�o h� trailer anexado ao ve�culo.");

	if (isnull(params))
	{
		SendHelpMessage(playerid, "/trailer [carga /desengatar /luzes /travar]");
		return 1;
	}

	if (CompareStrings(params, "carga") || CompareStrings(params, "lista"))
	{
	    new titulo[52], mensagem[512] = "{9ACD32}Produto\t{9ACD32}Quantidade", contador;

	    format(titulo, sizeof(titulo), "{FFFFFF}Trailer [Capacidade: %i]", GetMaxCargoVehicle(trailerid));

		for(new i; i != MAX_TRUCK_PRODUCT; i++)
		{
		    if (!VehicleInfo[trailerid][carCargoAmount][i])	continue;

			format(mensagem, sizeof(mensagem), "%s\n%s\t%i unidade(s)", mensagem, truckcargo_product[i], VehicleInfo[trailerid][carCargoAmount][i]);
			contador++;
		}

		if (!contador)	return SendErrorMessage(playerid, "N�o h� produto no reboque.");

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, titulo, mensagem, "Fechar", "");
	    return 1;
	}

	else if (CompareStrings(params, "desengatar"))
	{
	    if (GetVehicleSpeed(vehicle) > 3)	return SendErrorMessage(playerid, "Voc� deve parar.");

        DetachTrailerFromVehicle(vehicle);
        SendSyntaxMessage(playerid, "Trailer desconectado com sucesso. ");
	    return 1;
	}

	else if (CompareStrings(params, "luzes"))
	{
	    new engine,lights,alarm,doors,bonnet,boot,objective;
    	
		GetVehicleParamsEx(trailerid,engine,lights,alarm,doors,bonnet,boot,objective);

		if (lights != 0)
		{
			GameTextForPlayer(playerid, "~w~LUZES DO TRAILER ~r~DESLIGADAS", 2500, 4);
			VehicleLight(trailerid, false);
			return 1;
		}

		GameTextForPlayer(playerid, "~w~LUZES DO TRAILER ~g~LIGADAS", 2500, 4);
		VehicleLight(trailerid, true);
	    return 1;
	}

	else if (CompareStrings(params, "travar"))
	{
	    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Congelar") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])	return SendErrorMessage(playerid, "Voc� n�o pode usar este recurso agora.");
		
	    PlaySound(playerid, 1145);

		if (VehicleInfo[trailerid][carLocked])	GameTextForPlayer(playerid, "~w~TRAILER ~g~DESTRANCADO", 2500, 4);
		else									GameTextForPlayer(playerid, "~w~TRAILER ~r~TRANCADO", 2500, 4);

	    ToggleVehicleLock(trailerid, (VehicleInfo[trailerid][carLocked]) ? (false) : (true));
	    return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "/trailer [carga /desengatar /luzes /travar]");
	}
	return 1;
}


CMD:carga(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] != JOB_TRUCKER)	return 1;

	new parametrs[8],	arg[4];
	    
	if (sscanf(params, "s[8]S()[4]", parametrs, arg))
	{
		SendHelpMessage(playerid, "/cargo [lista | colocar | retirar | pegar | comprar | vender]");
		return 1;
	}

	new mes[128];

	if (CompareStrings(parametrs, "lista"))
	{
	    new vehicle = GetPlayerVehicleID(playerid);
		
		if (!vehicle && (vehicle = GetNearBootVehicle(playerid)) == 0) 	return SendErrorMessage(playerid, "Voc� deve estar no ve�culo ou perto do porta-malas.");
        if (IsTruckCar(vehicle))											return SendSyntaxMessage(playerid, "Use /trailer carga para ver uma lista de itens carregados. ");
		if (!IsTruckerJob(vehicle))										return SendErrorMessage(playerid, "Este ve�culo n�o � adequado para transportar mercadorias. ");
		if (ValidTruckForPlayer(playerid, vehicle) == 0)					return SendErrorMessage(playerid, "Sua habilidade n�o permite que voc� use este ve�culo, /ajudatrabalho para ver a lista de ve�culos dispon�veis.");
     	if (VehicleInfo[vehicle][carLocked] == 1)						return SendErrorMessage(playerid, "O bloqueio do ve�culo est� fechado.");

	    new title[52], msg[512] = "{9ACD32}Produto\t{9ACD32}Quantidade", count;

	    format(title, sizeof(title), "%s (Capacidade: %i)", VehicleNames[GetVehicleModel(vehicle)-400], GetMaxCargoVehicle(vehicle));

        for(new i; i != MAX_TRUCK_PRODUCT; i++)
		{
      		pTemp[playerid][pCargoListed][i] = 0;
		}

		for(new i; i != MAX_TRUCK_PRODUCT; i++)
		{
		    if (VehicleInfo[vehicle][carCargoAmount][i] == 0) continue;
			
 			pTemp[playerid][pCargoListed][count] = i;
			format(msg, sizeof(msg), "%s\n%s\t%i unidade(s)", msg, truckcargo_product[i], VehicleInfo[vehicle][carCargoAmount][i]);
			count++;
		}

		if (!count)	return SendErrorMessage(playerid, "N�o h� produto no ve�culo.");

		Dialog_Show(playerid, CargoList, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "Pegar" ,"Cancelar");
		SetPVarInt(playerid, #cargo_vehicle, vehicle);
	    return 1;
	}
	
	else if (CompareStrings(parametrs, "colocar"))
	{
	    if (pTemp[playerid][pCargoID] == 0) return SendErrorMessage(playerid, "Voc� n�o tem nada em suas m�os.");
        
		new vehicle;

		if ((vehicle = GetNearBootVehicle(playerid)) == 0) 					return SendErrorMessage(playerid, "Voc� deve estar perto do porta-malas do ve�culo.");
        if (IsTruckerJob(vehicle) == 0)										return SendErrorMessage(playerid, "Este ve�culo n�o � adequado para transportar mercadorias. ");
        if (ValidTruckForPlayer(playerid, vehicle) == 0)						return SendErrorMessage(playerid, "Sua habilidade n�o permite este ve�culo. Digite /ajudatrabalho para ver uma lista de ve�culos dispon�veis.");
	    if (VehicleInfo[vehicle][carLocked] == 1)							return SendErrorMessage(playerid, "O bloqueio do ve�culo est� fechado.");
        if (VehicleInfo[vehicle][carTrunkLock] == 0)							return SendErrorMessage(playerid, "O porta-malas deste ve�culo est� fechado.");   
		if (!IsValidProductVehicle(vehicle, pTemp[playerid][pCargoID]-1))	return SendErrorMessage(playerid, "Este produto n�o � adequado para este ve�culo.");

		new amount;

		for(new i; i != MAX_TRUCK_PRODUCT; i++)
		{
			amount += VehicleInfo[vehicle][carCargoAmount][i];
		}

		if (amount >= GetMaxCargoVehicle(vehicle))							return SendErrorMessage(playerid, "N�o h� espa�o no caminh�o.");

        switch(GetVehicleModel(vehicle))
        {
            case 600, 543, 605, 422, 478, 554: VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(2912, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
            case 443: VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(3593, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
		}

		switch(GetVehicleModel(vehicle))
		{
		    case 600: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, picador_attach[amount][0], picador_attach[amount][1], picador_attach[amount][2], 0.0, 0.0, 0.0);
		    case 543, 605: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, sadler_attach[amount][0], sadler_attach[amount][1], sadler_attach[amount][2], 0.0, 0.0, 0.0);
		    case 422: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, bobcat_attach[amount][0], bobcat_attach[amount][1], bobcat_attach[amount][2], 0.0, 0.0, 0.0);
            case 478: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, walton_attach[amount][0], walton_attach[amount][1], walton_attach[amount][2], 0.0, 0.0, 0.0);
            case 554: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, yosemite_attach[amount][0], yosemite_attach[amount][1], yosemite_attach[amount][2], 0.0, 0.0, 0.0);
		}
		
		Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
        cmd::putdown(playerid, "");

        if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);

        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		VehicleInfo[vehicle][carCargoAmount][pTemp[playerid][pCargoID]-1]++;
		pTemp[playerid][pCargoID] = 0;
	    return 1;
	}


	else if (CompareStrings(parametrs, "largar"))
	{
	    if (pTemp[playerid][pCargoID] == 0)	return SendErrorMessage(playerid, "Voc� n�o tem nada em suas m�os.");

		new id = -1;
		for(new i; i != MAX_CARGO_OBJ; i++)
		{
		    if (CargoObject[i][oOn] != 0)	continue;

			id = i;
			break;
		}

		if (id == -1) 						return SendErrorMessage(playerid, "O n�mero m�ximo de caixas largadas foi atingido.");

		CargoObject[id][oOn] = 1;
		CargoObject[id][oProduct] = pTemp[playerid][pCargoID];
        cmd::putdown(playerid, "");
		if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
  		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    	pTemp[playerid][pCargoID] = 0;

		new Float: x, Float: y, Float: z;

        GetXYInFrontOfPlayer(playerid, x, y, z, 1.5);
        CargoObject[id][oZ] -= 2.0;
		CargoObject[id][oObj] = CreateDynamicObject(2912, x, y, z, 0.0, 0.0, 0.0, GetWorld(playerid), GetInterior(playerid));
        CargoObject[id][oX] = x,
        CargoObject[id][oY] = y,
        CargoObject[id][oZ] = z,
        CargoObject[id][oInt] = GetInterior(playerid),
        CargoObject[id][oVW] = GetWorld(playerid);

        format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]", truckcargo_product[CargoObject[id][oProduct]-1]);

		CargoObject[id][oLabel] = CreateDynamic3DTextLabel(mes, 0xFFFFFFFF, x, y, z+0.1, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GetWorld(playerid), GetInterior(playerid), -1, 100.0);
		return 1;
	}

	else if (CompareStrings(parametrs, "pegar"))
	{
        if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) || pTemp[playerid][pCargoID] != 0) return SendErrorMessage(playerid, "Suas m�os j� est�o ocupadas com algo.");

		new id;
		if ((id = IsPlayerNearCargo(playerid)) == -1)	return SendErrorMessage(playerid, "N�o h� caixa ao seu lado que possa ser levantada.");

        CargoObject[id][oOn] = 0;
        pTemp[playerid][pCargoID] = CargoObject[id][oProduct];
        cmd::liftup(playerid, "");
        SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2912, 5, 0.102000, 0.306000, -0.228999, -1.100001, 14.499999, -1.300000, 1.000000, 1.000000, 1.000000);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

        if (IsValidDynamicObject(CargoObject[id][oObj]))	DestroyDynamicObject(CargoObject[id][oObj]);

        if (IsValidDynamic3DTextLabel(CargoObject[id][oLabel]))
		{
			DestroyDynamic3DTextLabel(CargoObject[id][oLabel]);
	        CargoObject[id][oLabel] = INVALID_3DTEXT_ID;
		}
	    return 1;
	}

	else if (CompareStrings(parametrs, "comprar"))
	{
	    if (IsPlayerInAnyVehicle(playerid))
		{
			new id = -1;
			if ((id = TruckCargo_Nearest(playerid, 10.0)) == -1)	return SendErrorMessage(playerid, "N�o h� nenhuma empresa perto de voc�.");
            if (TruckCargo[id][tLocked] == 1)					return SendErrorMessage(playerid, "A empresa est� fechada.");
		    
		    new vehicle = GetPlayerVehicleID(playerid), trailerid = GetVehicleTrailer(vehicle);

			if (GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 554 || GetVehicleModel(vehicle) == 499 || GetVehicleModel(vehicle) == 414 || GetVehicleModel(vehicle) == 456 || GetVehicleModel(vehicle) == 455)
				trailerid = vehicle;
		    
	     	if (trailerid == 0) 												return SendErrorMessage(playerid, "Acople o reboque ao ve�culo.");
            if (ValidTruckForPlayer(playerid, trailerid) == 0)					return SendErrorMessage(playerid, "Sua habilidade n�o permite este ve�culo. Digite /jobhelp para ver uma lista de ve�culos dispon�veis.");
            if (VehicleInfo[trailerid][carLocked] == 1) 						return SendErrorMessage(playerid, "O bloqueio de ve�culo est� fechado.");
            if (IsTakeProduct(TruckCargo[id][tProductID]))						return SendErrorMessage(playerid, "Este produto s� pode ser comprado fora do ve�culo.");
			if (!IsValidProductVehicle(trailerid, TruckCargo[id][tProductID]))	return SendErrorMessage(playerid, "Este produto n�o � adequado para este ve�culo.");

			new capacity = strval(arg), content = GetMaxCargoVehicle(trailerid);
			    
			if (isnull(arg))
			{
			    SendHelpMessage(playerid, "/cargo comprar [quantidade]");
			    switch(TruckCargo[id][tProductID])
			    {
			        case TRUCKER_BRICKS: SendClientMessageEx(playerid, -1, "Capacidade: %i palete(s) de tijolos.", content/6);
					case TRUCKER_WOODS: SendClientMessageEx(playerid, -1, "Capacidade: %i pacote(s) de toras.", content/18);
					default: SendClientMessageEx(playerid, -1, "Capacidade: %i unidade(s).", content);
				}
			    return 1;
			}

			if (capacity < 1 || capacity > GetMaxCargoVehicle(trailerid))	return SendErrorMessage(playerid, "Quantidade errada de mercadorias: 1 - %i.", GetMaxCargoVehicle(trailerid));
			if (TruckCargo[id][tStorage] < capacity)							return SendErrorMessage(playerid, "N�o h� %i item(ns) em estoque.", capacity);
			if (GetCash(playerid) < TruckCargo[id][tPrice] * capacity)		return SendNotCash(playerid);
		    
			new amount, prodid = -1;

			for(new i; i != MAX_TRUCK_PRODUCT; i++)
			{
			    if (VehicleInfo[trailerid][carCargoAmount][i] == 0) continue;
				    
				amount += VehicleInfo[trailerid][carCargoAmount][i];
				prodid = i;
			}

            switch(TruckCargo[id][tProductID])
			{
   				case TRUCKER_BRICKS:
   				{
   				    if (amount + capacity > content/6)	return SendErrorMessage(playerid, "N�o h� espa�o no caminh�o para %i item(ns).", capacity);
   				}
				case TRUCKER_WOODS:
				{
				    if (amount + capacity > content/18)	return SendErrorMessage(playerid, "N�o h� espa�o no caminh�o para %i item(ns).", capacity);
				}
			 	default:
				{
					if (amount + capacity > content)		return SendErrorMessage(playerid, "N�o h� espa�o no caminh�o para %i item(ns).", capacity);
				}
			}

			if (amount && prodid != TruckCargo[id][tProductID])	return SendErrorMessage(playerid, "O caminh�o j� cont�m \"%s\", voc� s� pode descarregar este produto.", truckcargo_product[prodid]);


			if (GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 554)
			{
				for(new i = 0; i != capacity; i++)
				{
			        switch(GetVehicleModel(vehicle))
			        {
			            case 443:
						{
							VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(3593, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
							AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, paker_attach[amount][0], paker_attach[amount][1], paker_attach[amount][2], paker_attach[amount][3], 0.0, 0.0);
						}
						case 578:
						{
							VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject((TruckCargo[id][tProductID] == TRUCKER_WOODS)?(18609):(1685), 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
							if (TruckCargo[id][tProductID] == TRUCKER_WOODS)
							{
								AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, dft_attach[0], dft_attach[1], dft_attach[2], 0.0, 0.0, dft_attach[3]);
							}
							else if (TruckCargo[id][tProductID] == TRUCKER_BRICKS)
							{
								AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, dft_attach_brick[amount][0], dft_attach_brick[amount][1], dft_attach_brick[amount][2], 0.0, 0.0, 0.0);
							}
						}
						case 554:
						{
							VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(1685, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
							AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, yosemite_attach_brick[0], yosemite_attach_brick[1], yosemite_attach_brick[2], 0.0, 0.0, 0.0);
						}
					}
					amount++;
				}
			}
			Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
			VehicleInfo[trailerid][carCargoAmount][TruckCargo[id][tProductID]] += capacity;
            GiveCash(playerid, -TruckCargo[id][tPrice] * capacity);
			TruckCargo[id][tStorage] -= capacity;
			SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nArmazenamento: %i /%i\nPre�o: $%i / unidade(s)", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
        	UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
			return 1;
		}

        new id = -1;
		if ((id = TruckCargo_Nearest(playerid, 1.5)) == -1)												return SendErrorMessage(playerid, "N�o h� nenhuma empresa perto de voc�.");
		if (TruckCargo[id][tLocked] == 1)																return SendErrorMessage(playerid, "A empresa est� fechada.");
		if (!IsTakeProduct(TruckCargo[id][tProductID]))													return SendErrorMessage(playerid, "Este produto s� pode ser comprado estando em ve�culo.");
		if (!TruckCargo[id][tStorage])																	return SendErrorMessage(playerid, "O armaz�m da empresa est� vazio.");
		if (GetCash(playerid) < TruckCargo[id][tPrice])													return SendNotCash(playerid);
		if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) || pTemp[playerid][pCargoID] != 0)	return SendErrorMessage(playerid, "Voc� j� est� ocupado(a) com algo nas m�os.");

		cmd::liftup(playerid, "");

		InfoMSG(playerid, "~w~Voce pode colocar a ~y~caixa ~w~no chao ou no porta-malas~n~do seu ve�culo, ~w~e tambem levanta-la do chao pressionando ~w~'~h~~r~~k~~GROUP_CONTROL_BWD~~w~'.", 10);

		SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2912, 5, 0.102000, 0.306000, -0.228999, -1.100001, 14.499999, -1.300000, 1.000000, 1.000000, 1.000000);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
		GiveCash(playerid, -TruckCargo[id][tPrice]);
		pTemp[playerid][pCargoID] = TruckCargo[id][tProductID]+1;
		TruckCargo[id][tStorage]--;
		SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
		format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nArmazenamento: %i /%i\nPre�o: $%i / unidade", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
        UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
	    return 1;
	}
	else if (CompareStrings(parametrs, "vender"))
	{
	    if (IsPlayerInAnyVehicle(playerid))
		{
            new vehicle = GetPlayerVehicleID(playerid), trailerid = GetVehicleTrailer(vehicle);

			if (GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 554 || GetVehicleModel(vehicle) == 499 || GetVehicleModel(vehicle) == 414 || GetVehicleModel(vehicle) == 456) 
			{
				trailerid = vehicle;
			}
			if (trailerid == 0)									return SendErrorMessage(playerid, "Acople o reboque ao ve�culo.");
			if (ValidTruckForPlayer(playerid, trailerid) == 0)	return SendErrorMessage(playerid, "Sua habilidade de trabalho n�o permite usar este ve�culo. Digite /jobhelp para ver a lista de ve�culos dispon�veis.");
			if (VehicleInfo[trailerid][carLocked] == 1)			return SendErrorMessage(playerid, "O cadeado do ve�culo est� fechado.");

			new capacity = strval(arg);

			if (isnull(arg))												return SendHelpMessage(playerid, "/cargo vender [quantidade de mercadorias]");
			if (capacity < 1 || capacity > GetMaxCargoVehicle(trailerid))	return SendErrorMessage(playerid, "quantidade de mercadorias de 1 a %i.", GetMaxCargoVehicle(trailerid));

			new amount, prodid = -1;

			for(new i; i != MAX_TRUCK_PRODUCT; i++)
			{
			    if (VehicleInfo[trailerid][carCargoAmount][i] == 0 || IsTakeProduct(i))	continue;

				amount += VehicleInfo[trailerid][carCargoAmount][i];
				prodid = i;
				break;
			}

			if (amount < capacity)	return SendErrorMessage(playerid, "N�o h� tal quantidade de mercadorias no ve�culo.");


            new id = -1;

			if ((id = TruckCargo_Nearest(playerid, 15.0)) != -1)
			{
			    if (TruckCargo[id][tLocked] == 1)										return SendErrorMessage(playerid, "A empresa est� fechada.");
				if (TruckCargo[id][tStorage] + capacity > TruckCargo[id][tStorageSize])	return SendErrorMessage(playerid, "O armaz�m da empresa est� cheio.");
				if (TruckCargo[id][tProductID] != prodid)								return SendErrorMessage(playerid, "Este produto n�o pode ser vendido nesta empresa.");

				if (GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 554)
				{
					for(new i = 0; i != capacity; i++)
					{
						DestroyDynamicObject(VehicleInfo[vehicle][carCargoObj][amount-1]);
						amount--;
					}
				}

				Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
				VehicleInfo[trailerid][carCargoAmount][prodid] -= capacity;
				GiveCash(playerid, TruckCargo[id][tPrice] * capacity);
				TruckCargo[id][tStorage] += capacity;
				SQL_SetInteger("truck_cargo", "armazenamento", TruckCargo[id][tStorage], TruckCargo[id][tID]);
				format(mes, sizeof(mes), "~w~+$%i", TruckCargo[id][tPrice] * capacity);
				GameTextForPlayer(playerid, mes, 1000, 1);
				format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nArmazenamento: %i /%i\nPre�o: $%i /unidades", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
				UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
				return 1;
			}

			else if ((id = GetNearOutsideBiz(playerid, 15.0)) != -1)
			{
			    if (BizInfo[id][bizProduct] + (biz_prod_point[BizInfo[id][bizType]] * capacity) > 400)	return SendErrorMessage(playerid, "N�o h� espa�o suficiente no neg�cio.");
			    if (BizInfo[id][bizPrice] == 0)																return SendErrorMessage(playerid, "Este neg�cio n�o compra mercadorias.");
			    if (biz_prod_types[BizInfo[id][bizType]] != prodid)											return SendErrorMessage(playerid, "Este produto n�o � adequado para este neg�cio.");

			    if (GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 554)
			    {
			        for(new i = 0; i != capacity; i++)
			        {
			            DestroyDynamicObject(VehicleInfo[vehicle][carCargoObj][amount-1]);
			            amount--;
			        }
			    }

			    Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
			    VehicleInfo[trailerid][carCargoAmount][prodid] -= capacity;
			    BizInfo[id][bizProduct] += (biz_prod_point[BizInfo[id][bizType]] * capacity);
			    GiveCash(playerid, BizInfo[id][bizBuyPrice] * capacity);
			    GiveBizCash(id, -BizInfo[id][bizBuyPrice] * capacity);

			    if (BizInfo[id][bizProduct] >= 400)	BizInfo[id][bizBuyPrice] = 0;

			    Save_Business(id);
			    format(mes, sizeof(mes), "~w~+$%i", BizInfo[id][bizBuyPrice] * capacity);
			    GameTextForPlayer(playerid, mes, 1000, 1);
			    return 1;
			}
			else 
			{
			    SendErrorMessage(playerid, "Voc� precisa estar perto de um neg�cio ou empresa!");
			}

			return 1;
		}

		if (pTemp[playerid][pCargoID] == 0)	return SendErrorMessage(playerid, "Voc� n�o tem nada em suas m�os.");

		new id = -1;

		if ((id = TruckCargo_Nearest(playerid, 1.5)) != -1)
		{
		    if (TruckCargo[id][tLocked] == 1)									return SendErrorMessage(playerid, "A empresa est� fechada.");
		    if (TruckCargo[id][tStorage] + 1 > TruckCargo[id][tStorageSize])	return SendErrorMessage(playerid, "O armaz�m da empresa est� cheio.");
		    if (TruckCargo[id][tProductID] != pTemp[playerid][pCargoID]-1)		return SendErrorMessage(playerid, "Este produto n�o pode ser vendido nesta empresa.");

		    cmd::putdown(playerid, "");

		    if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
		    
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
			GiveCash(playerid, TruckCargo[id][tPrice]);
			pTemp[playerid][pCargoID] = 0;
		    format(mes, sizeof(mes), "~w~+$%i", TruckCargo[id][tPrice]);
			GameTextForPlayer(playerid, mes, 1000, 1);
			TruckCargo[id][tStorage]++;
			SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nArmazenamento: %i /%i\nPre�o: $%i / unidade", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
		    UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
		}

		else if ((id = GetNearOutsideBiz(playerid)) != -1)
		{
		    if (BizInfo[id][bizProduct] + biz_prod_point[BizInfo[id][bizType]]  > 400)	return SendErrorMessage(playerid, "N�o h� espa�o suficiente no neg�cio.");
		    if (BizInfo[id][bizPrice] == 0)												return SendErrorMessage(playerid, "Este neg�cio n�o compra produtos.");
		    if (biz_prod_types[BizInfo[id][bizType]] != pTemp[playerid][pCargoID]-1)	return SendErrorMessage(playerid, "Este produto n�o pode ser vendido aqui.");

		    cmd::putdown(playerid, "");
		    
			if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
		    
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		    BizInfo[id][bizProduct] += biz_prod_point[BizInfo[id][bizType]];
		    GiveCash(playerid, BizInfo[id][bizBuyPrice]);
		    GiveBizCash(id, -BizInfo[id][bizBuyPrice]);
			
			if (BizInfo[id][bizProduct] >= 400)	BizInfo[id][bizBuyPrice] = 0;
			Save_Business(id);
		    pTemp[playerid][pCargoID] = 0;
			return 1;
		}

		else SendErrorMessage(playerid, "Voc� deve estar perto de um neg�cio ou empresa!");
	}
	return 1;
}

CMD:industria(playerid, params[])
{
    if (PlayerInfo[playerid][pJob] != JOB_TRUCKER)	return 1;

	new id = -1;

	if ((id = TruckCargo_Nearest(playerid)) == -1)	return SendErrorMessage(playerid, "N�o h� nenhuma industria pr�xima a voc�.");

    Show_Industry(playerid, id);
	return 1;
}