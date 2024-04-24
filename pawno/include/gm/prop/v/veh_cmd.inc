CMD:rentcar(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!IsDriver(playerid))			return 1;
    if (!IsRentCar(vehicle))			return SendErrorMessage(playerid, "This vehicle cannot be rented");
	if (VehicleInfo[vehicle][carRent])	return SendErrorMessage(playerid,"This vehicle has already been rented.");
	if (pTemp[playerid][pRentveh])		return SendErrorMessage(playerid, "You are already renting a vehicle. Enter /unrentcar to end the rental. ");
	if (GetCash(playerid) < 300)		return SendNotCash(playerid);
	    
	GiveCash(playerid, -300);
	pTemp[playerid][pRentveh] = vehicle;
    VehicleInfo[vehicle][carRent] = PlayerInfo[playerid][pID];
	SendSyntaxMessage(playerid, "You rented %s for $300. Available commands: /unrentcar, /v lock.", VehicleNames[GetVehicleModel(vehicle)-400]);
	SendSyntaxMessage(playerid, "Attention! If you do not use the vehicle for 10 minutes, it will disappear. ");
	return 1;
}

CMD:unrentcar(playerid, params[])
{
    if (!pTemp[playerid][pRentveh]) return 1;
		
	new vehicle = pTemp[playerid][pRentveh];

    SendSyntaxMessage(playerid, "You have terminated the lease %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
	pTemp[playerid][pRentveh] = 0;
    VehicleInfo[vehicle][carRent] = 0;
    RespawnVehicle(vehicle);
	return 1;
}

CMD:unimpound(playerid, params[])
{
    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Your vehicle is not spawned.");
	if (!vehicle)														return SendNotInVehicle(playerid);
	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
	if (!VehicleInfo[vehicle][Impounded])								return SendErrorMessage(playerid, "Your vehicle has not been confiscated. ");
	if (GetCash(playerid) < IMPOUND_FEE) 								return SendErrorMessage(playerid, "Insufficient funds to complete this operation ($%i).", IMPOUND_FEE);
	
	GiveCash(playerid, -IMPOUND_FEE);
	VehicleInfo[vehicle][Impounded] = 0;
	Save_Car(vehicle);

    SendSyntaxMessage(playerid, "You took your %s from the parking lot for $%i.", VehicleNames[GetVehicleModel(vehicle)-400], IMPOUND_FEE);
	SendHelpMessage(playerid, "Do not forget to park your vehicle from the car park!(/park)");
	return 1;
}

ALTX:vehicle("/v");
CMD:vehicle(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "You cannot use this feature right now.");

	new parametrs[13], arg[128], vehicle = GetPlayerVehicleID(playerid), model = GetVehicleModel(vehicle);

	if (sscanf(params, "s[13]S()[128]", parametrs, arg))
	{
		SendHelpMessage(playerid, "/(v)ehicle [options]");
		SendSyntaxMessage(playerid, "list | get | park | find | lock | hidenum | fakenum");
		SendSyntaxMessage(playerid, "corpse | tow | stats | fines | sell | sellto");
		SendClientMessageEx(playerid, COLOR_GRAD2, "CMD:{FFFFFF} /vmenu /mycars(/myv) /vfa /(en)gine /lights /trunk /hood /setstation(/ss) /(wi)ndow /unimpound");
		return 1;
	}

	else if (!strcmp(parametrs, "hidenum", true))
	{
		vehicle = GetNearVehicle(playerid, 4.0);

		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Your vehicle is not spawned.");
		if (!vehicle)														return SendErrorMessage(playerid, "You are not near the vehicle.");
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
		if (VehicleInfo[vehicle][carFakePlate])								return SendErrorMessage(playerid, "First, you need to remove the fake numbers. ");
		if (!IsPlayerHasItem(playerid, INV_TOOLS)) 							return SendErrorMessage(playerid, "You have no tools.");
		if (VehicleInfo[vehicle][carPlate] == '_') 							return SendErrorMessage(playerid, "Your car has no license plates.");

		if (VehicleInfo[vehicle][carHidedPlate])
		{
			VehicleInfo[vehicle][carHidedPlate] = 0;
			SQL_SetInteger("cars", "hidednum", VehicleInfo[vehicle][carHidedPlate], VehicleInfo[vehicle][carID]);
			SetVehicleNumberPlate(vehicle, VehicleInfo[vehicle][carPlate]);
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "You have installed numbers on the vehicle. ");
		}
		else 
		{
			VehicleInfo[vehicle][carHidedPlate] = 1;
			SQL_SetInteger("cars", "hidednum", VehicleInfo[vehicle][carHidedPlate], VehicleInfo[vehicle][carID]);
			SetVehicleNumberPlate(vehicle, "_");
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "You have removed the license plate from the vehicle. ");
		}
		return 1;
	}

	else if (!strcmp(parametrs, "fakenum", true))
	{
		vehicle = GetNearVehicle(playerid, 4.0);

		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Your vehicle is not spawned.");
		if (!vehicle)														return SendErrorMessage(playerid, "You are not near the vehicle.");
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
		if (!IsPlayerHasItem(playerid, INV_FAKENUM)) 						return SendWarningMessage(playerid, "You have no fake numbers.");
		if (VehicleInfo[vehicle][carHidedPlate])							return SendErrorMessage(playerid, "Your numbers have been removed. (/v hidenum)");								
		
		if (VehicleInfo[vehicle][carFakePlate])
		{
			VehicleInfo[vehicle][carFakePlate] = 0;
			SQL_SetInteger("cars", "fakenum", VehicleInfo[vehicle][carFakePlate], VehicleInfo[vehicle][carID]);
			SetVehicleNumberPlate(vehicle, VehicleInfo[vehicle][carPlate]);
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "You have removed false license plates from the vehicle.");
		}
		else 
		{
			VehicleInfo[vehicle][carFakePlate] = 1;
			SQL_SetInteger("cars", "fakenum", VehicleInfo[vehicle][carFakePlate], VehicleInfo[vehicle][carID]);
			
			if (isempty(VehicleInfo[vehicle][carFakePlateS]))
			{
				format(VehicleInfo[vehicle][carFakePlateS], 24, "%s", GetVehicleNumber());
				SQL_SetString("cars", "fakenum_str", VehicleInfo[vehicle][carFakePlateS], VehicleInfo[vehicle][carID]);
			}

			SetVehicleNumberPlate(vehicle, VehicleInfo[vehicle][carFakePlateS]);
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "You have installed false numbers on the vehicle. ");
		}
		return 1;
	}

	else if (!strcmp(parametrs, "menu", true)) return cmd::vmenu(playerid, "");
	    
	else if (!strcmp(parametrs, "fines", true))
	{
		cmd::vtickets(playerid, "");
	}

	else if (!strcmp(parametrs, "get", true))
	{
		if (IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "You already have a spawned vehicle.");

		if (CountPlayerVehicles(playerid) == 0)
			return SendErrorMessage(playerid, "You don't have any vehicle.");
			
		return ShowVehicleList(playerid); 
	}

	else if (!strcmp(parametrs, "park", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Your vehicle is not spawned.");
		if (vehicle == 0)													return SendNotInVehicle(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
		if (VehicleInfo[vehicle][Impounded])									return SendErrorMessage(playerid, "While your vehicle is in the parking lot, you cannot use this feature. (/unimpound)");

		new Float:hp;
		AntiCheatGetVehicleHealth(vehicle, hp);

		if (hp < 250.0)									return SendErrorMessage(playerid, "You cannot park this vehicle as it is badly damaged.");
		if (VehicleInfo[vehicle][carAttach3DtextUse])	return SendErrorMessage(playerid, "Wait, the operation is already in progress.");
        
		GetVehiclePos(vehicle, VehicleInfo[vehicle][carParkX], VehicleInfo[vehicle][carParkY], VehicleInfo[vehicle][carParkZ]);
		GetVehicleZAngle(vehicle, VehicleInfo[vehicle][carParkA]);
		
		SendSyntaxMessage(playerid, "Vehicle \"%s\" parked.", VehicleNames[model-400]);
		ResetVehicleStuff(vehicle, 1);
		DestroyVehicle(vehicle);
		PlayerInfo[playerid][pCarKey] = 0;
		VehicleInfo[vehicle][carOwnerPlayer] = INVALID_PLAYER_ID;
		return 1;
	}

	else if (!strcmp(parametrs, "sellto", true))
	{
		new userid, price;

		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))									return SendErrorMessage(playerid, "Your vehicle is not spawned.");
		if (!vehicle)																		return SendNotInVehicle(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])					return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
		if (sscanf(arg, "ui", userid, price))												return SendHelpMessage(playerid, "/v sell [ID/Part Of Name] [price]");
        if (!IsPlayerLogged(userid))															return SendNotConnected(playerid);
		if (IsADonateModel(GetVehicleModel(vehicle)))										return SendErrorMessage(playerid,"You cannot sell donated vehicles to the player.");
        if (PlayerInfo[userid][pPremium] < 2 && VehicleInfo[vehicle][carComponent][5] != 0)	return SendErrorMessage(playerid, "This player does not have access to nitro. ");
		if (userid == playerid)																return 1;
		if (!IsABicycle(vehicle) && !VehicleInfo[vehicle][carRegistered])					return SendErrorMessage(playerid, "This vehicle is not registered.");
		if (!PlayerNearPlayer(3.0, playerid, userid))										return SendNotNearPlayer(playerid);
		if (IsValidVehicle(PlayerInfo[userid][pCarKey]))										return SendErrorMessage(playerid, "This player has a spawned vehicle.");
		if (price < 0 || price > 10000000)													return SendErrorMessage(playerid, "Price from $ 0 to $ 10,000,000.");

		SendSyntaxMessage(userid, "%s wants to sell you %s for $%i. (Click {33AA33}Y{FFFFFF} for purchase.)", GetNameEx(playerid), VehicleNames[GetVehicleModel(vehicle)-400], price);
		if (IsABicycle(vehicle))
		{
			SendSyntaxMessage(userid, "Safety: Lock[%i]", VehicleInfo[vehicle][carLock]);
			SendSyntaxMessage(userid, "Other: Colour[{%06x}#%i{FFFFFF}], Colour[{%06x}#%i{FFFFFF}]", VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2]);
		}
		else
		{
			SendSyntaxMessage(userid, "Vehicle status: Engine[%.2f], Battery[%.2f], Mileage[%.2f]",VehicleInfo[vehicle][carEngine], VehicleInfo[vehicle][carBattery], VehicleInfo[vehicle][carMilliage]);
			SendSyntaxMessage(userid, "Safety: Lock[%i], Signaling[%i], Immobilizer[%i], Insurance[%i]", VehicleInfo[vehicle][carLock], VehicleInfo[vehicle][carAlarm], VehicleInfo[vehicle][carImmob], VehicleInfo[vehicle][carInsurance]);
			
			if (VehicleInfo[vehicle][carArmour] > 0.0)	SendSyntaxMessage(userid, "Other: Colour[{%06x}#%i{FFFFFF}], Colour[{%06x}#%i{FFFFFF}], Plates[%s], Armor[%.2f], Radio cassette[%i]", VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2], VehicleInfo[vehicle][carPlate], VehicleInfo[vehicle][carArmour], VehicleInfo[vehicle][carRadio]);
			else										SendSyntaxMessage(userid, "Other: Colour[{%06x}#%i{FFFFFF}], Colour[{%06x}#%i{FFFFFF}], Plates[%s], Radio cassette[%i]", VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2], VehicleInfo[vehicle][carPlate], VehicleInfo[vehicle][carRadio]);
		}

		SendSyntaxMessage(playerid, "You offer to buy your \"%s\" %s for $%i.", VehicleNames[GetVehicleModel(vehicle)-400], GetNameWithMask(userid), price);

		pTemp[userid][pCarPlayer] = playerid;
		pTemp[userid][pCarPrice] = price;
		SetPVarInt(userid, #accept_time, gettime() + 30);
		return 1;
	}

	else if (!strcmp(parametrs, "lock", true))
	{
	    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])	return SendErrorMessage(playerid, "You cannot use this feature right now.");
		
	    vehicle = 0;
		if ((vehicle = GetNearVehicle(playerid, 4.0)) != 0)
		{
		    if ((VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction])
			 || pTemp[playerid][pRentveh] == vehicle
			 || PlayerInfo[playerid][pCarKey] == vehicle
			 || (PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehicle][carID] && PlayerInfo[playerid][pSpareKey] != 0)
			 || IsWindowOpened(vehicle) && !VehicleInfo[vehicle][carFaction])
			{
			    PlaySound(playerid, 1145);
			    
				new mes[128];

			    if (VehicleInfo[vehicle][carLocked])		format(mes, sizeof(mes), "~w~%s ~g~OPEN", VehicleNames[GetVehicleModel(vehicle)-400]);
				else									format(mes, sizeof(mes), "~w~%s ~r~CLOSED", VehicleNames[GetVehicleModel(vehicle)-400]);
				GameTextForPlayer(playerid, mes, 2500, 4);

			    if (VehicleInfo[vehicle][carLocked])	format(mes, sizeof(mes), "opened the central lock %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
				else								format(mes, sizeof(mes), "closed the central lock %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
				SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

			    ToggleVehicleLock(vehicle, (VehicleInfo[vehicle][carLocked]) ? (false): (true));
				return 1;
			}
			else {
				SendErrorMessage(playerid, "You don't have a key to the vehicle!");
				SendHelpMessage(playerid, "To open the car, enter /vbreaklock (/vbl).");
			}
		}
	    return 1;
	}

	else if (!strcmp(parametrs, "find", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Your vehicle is not spawned.");

		vehicle = PlayerInfo[playerid][pCarKey];

		if (!VehicleInfo[vehicle][carGps])			return SendErrorMessage(playerid, "Your vehicle does not have GPS installed. ");
        if (IsPlayerInVehicle(playerid, vehicle))	return SendErrorMessage(playerid, "You are using your vehicle. ");
		if (!VehicleEmpty(vehicle))					return SendErrorMessage(playerid, "Someone is using your vehicle now.");
		if (GetVehicleInteriorEx(vehicle))			return SendErrorMessage(playerid, "Unable to find your vehicle. (In the interior)");

		new Float:x, Float:y, Float:z;
		GetVehiclePos(vehicle, x, y, z);

		SendSyntaxMessage(playerid, "Your vehicle \"%s\" located in the area \"%s\".", VehicleNames[GetVehicleModel(vehicle)-400], GetVehicleZone(vehicle));
		SendSyntaxMessage(playerid, "The location of the vehicle is marked on the map. (/removecp)");
		Waypoint_Set(playerid, x, y, z);
		return 1;
	}

	else if (!strcmp(parametrs, "tow", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Your vehicle is not spawned.");

		vehicle = PlayerInfo[playerid][pCarKey];

		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
		if (VehicleInfo[vehicle][carAttach3DtextUse])						return SendErrorMessage(playerid, "Wait, the operation is already in progress.");
		if (IsPlayerInVehicle(playerid, vehicle))							return SendErrorMessage(playerid, "You are using your vehicle.");
		if (!VehicleEmpty(vehicle))											return SendErrorMessage(playerid, "Someone is using your vehicle now.");
		if (!IsABicycle(vehicle) && !VehicleInfo[vehicle][carRegistered])	return SendErrorMessage(playerid, "This vehicle is not registered.");
		if (VehicleInfo[vehicle][Impounded])								return SendErrorMessage(playerid, "While your vehicle is in the parking lot, you cannot use this function.. (/unimpound)");
		if (GetCash(playerid) < 200)										return SendNotCash(playerid);

		SendSyntaxMessage(playerid, "Your vehicle will be towed to the parking place in 10 seconds.");
        StartVehicleUpgrade(playerid, vehicle, 1, 200);
		UpdateDynamic3DTextLabelText(status_update[vehicle], 0x33AA33FF, "(( ---------- ))\nTOWING VEHICLE");
		return 1;
	}

	else if (!strcmp(parametrs, "stats", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Your vehicle is not spawned.");

  		vehicle = PlayerInfo[playerid][pCarKey];
		
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");

		new title[24], diatxt[512];
		format(title, sizeof(title), "%s", VehicleNames[GetVehicleModel(vehicle)-400]);

		if (IsABicycle(vehicle))
		{
			format(diatxt, sizeof(diatxt), "Safety:\n- Lock\t[%i]\n", VehicleInfo[vehicle][carLock]);
			format(diatxt, sizeof(diatxt), "%sOther:\n- Color\t[{%06x}#%i{FFFFFF}]\n- Color\t[{%06x}#%i{FFFFFF}]", diatxt, VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2]);
			Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, title, diatxt, ">>", "");
			return 1;
		}

		format(diatxt, sizeof(diatxt), "{ecc317}Registration:{FFFFFF}\n");

		if (VehicleInfo[vehicle][carRegistered]) format(diatxt, sizeof(diatxt), "%s{FFFFFF}Plate - %s\n", diatxt, VehicleInfo[vehicle][carPlate]);
		else 									format(diatxt, sizeof(diatxt), "%s{FF6347}NO PLATES{FFFFFF}\n", diatxt, VehicleInfo[vehicle][carPlate]);
		
		if (VehicleInfo[vehicle][carHidedPlate]) format(diatxt, sizeof(diatxt), "%s{FF6347}VEHICLES NUMBERS REMOVED{FFFFFF}\n", diatxt, VehicleInfo[vehicle][carPlate]);
		if (VehicleInfo[vehicle][carFakePlate])	format(diatxt, sizeof(diatxt), "%s{FF6347}FAKE NUMBERS ON VEHICLE\n\t\t[%s]\n", diatxt, VehicleInfo[vehicle][carFakePlateS]);
																						  	
		format(diatxt, sizeof(diatxt), "%s\n{ecc317}Main info:{FFFFFF}\n- Engine\t\t[%.2f]\n- Battery\t\t[%.2f]\n- Fuel\t\t[%.1f / %.0f]\n- Mileage\t\t[%.2f]\n\n", diatxt, VehicleInfo[vehicle][carEngine], VehicleInfo[vehicle][carBattery], VehicleInfo[vehicle][carFuel], GetVehicleFuelCapacity(GetVehicleModel(vehicle)), VehicleInfo[vehicle][carMilliage]);
		format(diatxt, sizeof(diatxt), "%s{ecc317}Safety:{FFFFFF}\n- Lock\t\t\t[%i]\n- Alarm\t[%i]\n- Immobilizer\t[%i]\n- Insurance\t\t[%i]\n", diatxt, VehicleInfo[vehicle][carLock], VehicleInfo[vehicle][carAlarm], VehicleInfo[vehicle][carImmob], VehicleInfo[vehicle][carInsurance]);
		
		if (VehicleInfo[vehicle][carArmour] > 0.0) format(diatxt, sizeof(diatxt), "%sArmor\t[%.2f]\n", diatxt, VehicleInfo[vehicle][carArmour]);
		
		format(diatxt, sizeof(diatxt), "%s\n{ecc317}Other:{FFFFFF}\n- XM Radio\t\t%s\n- Color1\t\t\t[{%06x}#%i{FFFFFF}]\n- Color2\t\t\t[{%06x}#%i{FFFFFF}]", diatxt, (VehicleInfo[vehicle][carRadio])?("{33AA33}Есть{FFFFFF}"):("{FF6347}Нет{FFFFFF}"), VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2]);
		
		Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, title, diatxt, ">>", "");
		return 1;
	}

	else if (!strcmp(parametrs, "list", true))
	{
	    new query[128];
	    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `owner` = %i", PlayerInfo[playerid][pID]);
		mysql_tquery(dbHandle, query, "ShowCarsForPlayer", "ii", playerid, playerid);
	    return 1;
	}
	else if (!strcmp(parametrs, "corpse", true))
	{	
		vehicle = GetNearVehicle(playerid, 4.0);
        if (vehicle == -1)							return SendErrorMessage(playerid, "You are not near vehicle.");
 		if (!IsNearTrunk(vehicle, playerid, 2.0)) 	return SendErrorMessage(playerid, "You are not near the trunk! ");
		if (!VehicleInfo[vehicle][carTrunkLock])	return SendErrorMessage(playerid, "The trunk must be open.");
		if (!VehicleInfo[vehicle][vCorp] && !CorpInfo[VehicleInfo[vehicle][vCorp]][cUsed]) return SendErrorMessage(playerid, "There is no corpse in the trunk. ");
        
		new Float:pos[3];
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

		DestroyDynamic3DTextLabel(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cText]);
		
	    CorpInfo[VehicleInfo[vehicle][vCorp]-1][cText]=CreateDynamic3DTextLabel("(( DEAD BODY ))\npress '~k~~GROUP_CONTROL_BWD~'", COLOR_LIGHTRED, pos[0]+0.75, pos[1], pos[2]-0.4, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 50.0);
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cX] = pos[0]+0.75;
	    CorpInfo[VehicleInfo[vehicle][vCorp]-1][cY] = pos[1];
	    CorpInfo[VehicleInfo[vehicle][vCorp]-1][cZ] = pos[2];
	    CorpInfo[VehicleInfo[vehicle][vCorp]-1][cVeh] = 0;
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cTime] = gettime();
		if (CorpInfo[VehicleInfo[vehicle][vCorp]-1][cType] == 0) {
			CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody]=CreateActor(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cSkin], pos[0]+0.75, pos[1], pos[2], 0.0);
			SetActorInvulnerable(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody], true);
			ApplyActorAnimation(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody], "PED", "KO_shot_stom", 4.0, 0, 0, 0, 1, 0);
		} else {
			CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody]=CreateDynamicObject(19944, pos[0]+0.75, pos[1], pos[2]-0.5, 0.0, 0.0, 0.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
		}
		
	    VehicleInfo[vehicle][vCorp]=0;    
		SM2(playerid, "You pulled a corpse out of the trunk.");
		return 1;
	}
	else if (!strcmp(parametrs, "sell", true))
	{
		new veh = PlayerInfo[playerid][pCarKey], lockuse[200], alarmuse[300], immobuse[300], insuranceuse[350], xmradiouse[50], gpsuse[50], string1[1250];

		if (!IsValidVehicle(veh))											return SendErrorMessage(playerid, "Your vehicle is not spawned.");
		if (!IsPlayerInAnyVehicle(playerid))								return SendNotInVehicle(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");

		if (VehicleInfo[veh][carLock] == 1) 	 lockuse = "{a9c4e4}Lock: \t{bab9ba}1 level\t{e2bd63}$3.500\n{4af085}+{e2bd63}120 seconds of breaking vehicle.\n\n";
		else if (VehicleInfo[veh][carLock] == 2) lockuse = "{a9c4e4}Lock: \t{bab9ba}2 level{e2bd63}$5.500\n{4af085}+{e2bd63}180 wait time protection against prying method.\n\n";
		else if (VehicleInfo[veh][carLock] == 3) lockuse = "{a9c4e4}Lock: \t{bab9ba}3 level\t{e2bd63}$7.500\n{4af085}+{e2bd63}240 wait time protection against prying method.\n\n";
		else if (VehicleInfo[veh][carLock] == 4) lockuse = "{a9c4e4}Lock: \t{bab9ba}4 level\t{e2bd63}$10.500\n{4af085}+{e2bd63}300 wait time protection against prying method.\n\n";
		else if (VehicleInfo[veh][carLock] == 5) lockuse = "{a9c4e4}Lock: \t{bab9ba}5 level\t{e2bd63}$13.500\n{4af085}+{e2bd63}360 wait time protection against prying method.\n\n";

		if (VehicleInfo[veh][carAlarm] == 1) 		alarmuse = "{a9c4e4}Alarm: \t{bab9ba}1 level\t{e2bd63}$2.600\n{f0634a}+{e2bd63}Loud vehicle alarm.\n";
		else if (VehicleInfo[veh][carAlarm] == 2) 	alarmuse = "{a9c4e4}Alarm: \t{bab9ba}2 level\t{e2bd63}$5.500\n{f0634a}+{e2bd63}Loud vehicle alarm.\n{f0634a}+{e2bd63}Vehicle alerts the local police department of a possible breach.\n\n";
		else if (VehicleInfo[veh][carAlarm] == 3) 	alarmuse = "{a9c4e4}Alarm: \t{bab9ba}3 level\t{e2bd63}$8.000\n{f0634a}+{e2bd63}Loud vehicle alarm.\n{f0634a}+{e2bd63}Vehicle alerts the local police department of a possible breach.\n{f0634a}+{e2bd63}SMS notification\n\n";
		else if (VehicleInfo[veh][carAlarm] == 4) 	alarmuse = "{a9c4e4}Alarm: \t{bab9ba}4 level\t{e2bd63}$10.500\n{f0634a}+{e2bd63}Loud vehicle alarm.\n{f0634a}+{e2bd63}Vehicle alerts the local police department of a possible breach.\n{f0634a}+{e2bd63}SMS notification\n{f0634a}+{e2bd63}Vehicle blip will appear on the radar.\n\n";

		if (VehicleInfo[veh][carImmob] == 1) 		immobuse = "{a9c4e4}Immobilizer: \t{bab9ba}1 level\t{e2bd63}$5.600\n{feff00}+{e2bd63}25 second limit while hotwiring.\n\n";
		else if (VehicleInfo[veh][carImmob] == 2) 	immobuse = "{a9c4e4}Immobilizer: \t{bab9ba}2 level\t{e2bd63}$11.300\n{feff00}+{e2bd63}50 second limit while hotwiring.\n\n";
		else if (VehicleInfo[veh][carImmob] == 3) 	immobuse = "{a9c4e4}Immobilizer: \t{bab9ba}3 level\t{e2bd63}$17.000\n{feff00}+{e2bd63}75 second limit while hotwiring.\n\n";
		else if (VehicleInfo[veh][carImmob] == 4) 	immobuse = "{a9c4e4}Immobilizer: \t{bab9ba}4 level\t{e2bd63}$22.600\n{feff00}+{e2bd63}100 second limit while hotwiring.\n\n";

		if (VehicleInfo[veh][carInsurance] == 1) 		insuranceuse = "{a9c4e4}Insurance: \t{bab9ba}1 level\t{e2bd63}$5.600\n{4a90f0}+{e2bd63}Vehicle will always respawn with its max health.\n\n";
		else if (VehicleInfo[veh][carInsurance] == 2) 	insuranceuse = "{a9c4e4}Insurance: \t{bab9ba}2 level\t{e2bd63}$11.300\n{4a90f0}+{e2bd63}Vehicle will always respawn with its max health.\n{4a90f0}+{e2bd63}Protection against cosmetic vehicle damage. Vehicle will re-spawn good as new.\n\n";
		else if (VehicleInfo[veh][carInsurance] == 3)	insuranceuse = "{a9c4e4}Insurance: \t{bab9ba}3 level\t{e2bd63}$17.000\n{4a90f0}+{e2bd63}Vehicle will always respawn with its max health.\n{4a90f0}+{e2bd63}Protection against cosmetic vehicle damage. Vehicle will re-spawn good as new.\n{4a90f0}+{e2bd63}Vehicle modification coverage. We got your fancy layout and XM tunes covered.\n\n";

		if (VehicleInfo[veh][carRadio] == 1) xmradiouse = "{a9c4e4}XM Radio:\t{e2bd63}$7.500\n";
		if (VehicleInfo[veh][carGps] == 1) gpsuse = "{a9c4e4}GPS:\t{e2bd63}$3.500\n";

		new mes[56];

		if (IsABicycle(veh))	format(mes, sizeof(mes), "{33AA33}%s", VehicleNames[VehicleInfo[veh][carModel]-400]);
		else
		{
			if (VehicleInfo[veh][carRegistered])	format(mes, sizeof(mes), "{FFFFFF}%s [%s]", VehicleNames[VehicleInfo[veh][carModel]-400], VehicleInfo[veh][carPlate]);
			else								format(mes, sizeof(mes), "{FFFFFF}%s", VehicleNames[VehicleInfo[veh][carModel]-400]);
		}

		format(string1, sizeof(string1), "{ffffff}Sale price:\t\t{e2bd63}$%i\n{ffffff}Maximum health:\t{e2bd63}%.2f\n\n%s%s%s%s%s%s", GetVehiclePrice(VehicleInfo[veh][carModel])/2, GetVehicleMaxHealth(GetVehicleModel(veh)), lockuse, alarmuse, immobuse, insuranceuse, xmradiouse, gpsuse);
		Dialog_Show(playerid, ScrapSell, DIALOG_STYLE_MSGBOX, mes, string1, "Accept", "Cancel");
		return 1;
	}
	else
	{
		SendHelpMessage(playerid, "/(v)ehicle [options]");
		SendSyntaxMessage(playerid, "list | get | park | find | lock | hidenum | fakenum | corpse");
		SendSyntaxMessage(playerid, "tow | stats | fines | sell | sellto | menu");
		SendClientMessageEx(playerid, COLOR_GRAD2, "CMD:{FFFFFF} /(en)gine /lights /trunk /hood /ss /(wi)ndow /unimpound");
	}
	return 1;
}

CMD:register(playerid, params[])
{
    if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Your vehicle is not spawned.");

	new vehicle = PlayerInfo[playerid][pCarKey];

	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
	if (IsABicycle(vehicle))												return SendErrorMessage(playerid, "You cannot register a bicycle.");
	if (VehicleInfo[vehicle][carEngineOn])								return SendErrorMessage(playerid, "Stop the engine before modifying your vehicle. ");
	if (VehicleInfo[vehicle][carAttach3DtextUse] != 0)					return SendErrorMessage(playerid, "Wait, the operation is already in progress.");
	if (VehicleInfo[vehicle][carRegistered])								return SendErrorMessage(playerid, "This vehicle is already registered.");
	if (GetCash(playerid) < 300)											return SendNotCash(playerid);

	if (!IsPlayerInRangeOfPoint(playerid, 5.0,2228.1577,-110.8875,26.6406))	return SendErrorMessage(playerid, "You must be inside the police station in Palomino.");
    if (!IsVehicleInRangeOfPoint(vehicle, 15.0,697.6670,1624.8490,785.5494))	return SendErrorMessage(playerid, "Your vehicle should be near the police station in Palomino.");
		
	StartVehicleUpgrade(playerid, vehicle, 7, 300);
	SendSyntaxMessage(playerid, "Постановка на учет начата, это займет некоторое время.");
	return 1;
}

CMD:unregister(playerid, params[])
{
    if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Your vehicle is not spawned.");

	new vehicle = PlayerInfo[playerid][pCarKey];

	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");
	if (IsABicycle(vehicle))												return SendErrorMessage(playerid, "The bike cannot be deregistered. ");
	if (VehicleInfo[vehicle][carEngineOn])								return SendErrorMessage(playerid, "Stop the engine before you start modifying your vehicle.");
	if (VehicleInfo[vehicle][carAttach3DtextUse] != 0)					return SendErrorMessage(playerid, "Wait, the operation is already in progress.");
	if (!VehicleInfo[vehicle][carRegistered])							return SendErrorMessage(playerid, "This vehicle is not registered.");
	if (GetCash(playerid) < 300)											return SendNotCash(playerid);

	if (!IsPlayerInRangeOfPoint(playerid, 5.0,2228.1577,-110.8875,26.6406))	return SendErrorMessage(playerid, "You must be inside the police station in Palomino. ");
    if (!IsVehicleInRangeOfPoint(vehicle, 15.0,697.6670,1624.8490,785.5494))	return SendErrorMessage(playerid, "Your vehicle should be near the police station in Palomino.");
		
	StartVehicleUpgrade(playerid, vehicle, 8, 300);
	SendSyntaxMessage(playerid, "Deregistration started, it will take some time. ");
	return 1;
}


CMD:lights(playerid, params[])
{
	if (!IsDriver(playerid))	return SendNotInVehicle(playerid);

	new vehicleid = GetPlayerVehicleID(playerid), mes[128];
	new engine,lights,alarm,doors,bonnet,boot,objective;

    GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);

	if (lights)
	{
		format(mes, sizeof(mes), "turned off the headlights %s.", VehicleNames[GetVehicleModel(vehicleid)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		format(mes, sizeof(mes), "~w~Headlights %s ~r~OFF", VehicleNames[GetVehicleModel(vehicleid)-400]);
		GameTextForPlayer(playerid, mes, 2500, 4);

		VehicleLight(vehicleid, false);
		return 1;
	}

	format(mes, sizeof(mes), "turned on the headlights %s.", VehicleNames[GetVehicleModel(vehicleid)-400]);
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

	format(mes, sizeof(mes), "~w~Headlights %s ~g~ON", VehicleNames[GetVehicleModel(vehicleid)-400]);
	GameTextForPlayer(playerid, mes, 2500, 4);

	VehicleLight(vehicleid, true);
	return 1;
}

CMD:alarmoff(playerid, params[])
{
    if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))						return SendErrorMessage(playerid, "Your vehicle is not spawned.");
	if (!GetNearVehicle(playerid, 10.0, PlayerInfo[playerid][pCarKey]))		return SendErrorMessage(playerid, "You must be close to your vehicle.");

    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(PlayerInfo[playerid][pCarKey],engine,lights,alarm,doors,bonnet,boot,objective);

	if (alarm == VEHICLE_PARAMS_OFF)	return SendErrorMessage(playerid, "The alarm does not work.");

	AlarmOff(PlayerInfo[playerid][pCarKey]);
	SendWarningMessage(playerid, "You turn off the alarm. ");
	return 1;
}

ALTX:window("/wi", "/ww");
CMD:window(playerid, params[])
{
    if (!IsPlayerInAnyVehicle(playerid))				return SendNotInVehicle(playerid);

	new vehicle = GetPlayerVehicleID(playerid), seat = AntiCheatGetVehicleSeat(playerid), model = GetVehicleModel(vehicle), wi;

    if (IsADontWindow(vehicle))						return SendErrorMessage(playerid, "This vehicle has no windows. ");

	if (sscanf(params, "i", wi))
	{
		SendHelpMessage(playerid, "/(wi)ndow [window]");
		SendSyntaxMessage(playerid, "Window: 1 - Driver's | 2 - Front passenger | 3 - Rear left | 4 - Rear right ");
		return 1;
	}
	
	new seat_name[4][32] = {"driver", "front passenger", "rear left", "rear right"};

	if (wi > 4 || wi < 1)						return SendErrorMessage(playerid, "1 - Driving | 2 - Front passenger | 3 - Rear left | 4 - Rear right ");
	if (!IsDriver(playerid) && seat+1 != wi)		return SendErrorMessage(playerid, "You are not a driver and you can only open / close %s window.", seat_name[seat]);

	if ((model == 401 || model == 402 || model == 403 || model == 406 || model == 408 ||model == 410 || model == 411 || model == 412 || model == 413 || model == 414 ||model == 415 || model == 416 || model == 417 || model == 419 || 
	model == 422 || model == 425 || model == 427 || model == 428 || model == 429 || model == 431 || model == 433 || model == 434 || model == 436 || model == 437 || model == 439 || model == 440 || model == 443 || model == 444 || 
	model == 447 || model == 451 || model == 455 || model == 456 || model == 459 || model == 474 || model == 475 || model == 477 || model == 478 || model == 480 || model == 482 || model == 489 || model == 491 || model == 494 || 
	model == 495 || model == 496 || model == 498 || model == 499 || model == 500 || model == 502 || model == 503 || model == 505 || model == 506 || model == 508 || model == 514 || model == 515 || model == 517 || model == 518 || 
	model == 524 || model == 525 || model == 526 || model == 527 || model == 528 || model == 533 || model == 534 || model == 535 || model == 536 || model == 541 || model == 542 || model == 543 || model == 544 || model == 545 || 
	model == 549 || model == 552 || model == 554 || model == 555 || model == 556 || model == 557 || model == 558 || model == 559 || model == 562 || model == 565 || model == 573 || model == 574 || model == 575 || model == 576 || 
	model == 578 || model == 582 || model == 583 || model == 587 || model == 589 || model == 593 || model == 599 || model == 600 || model == 602) && wi > 2)
		return SendErrorMessage(playerid, "This vehicle has no rear windows.");

	new wi_state[4], mes[128];

	GetVehicleParamsCarWindows(vehicle, wi_state[0], wi_state[1], wi_state[2], wi_state[3]);
	wi_state[wi-1] = !wi_state[wi-1] ? (1):(0);

    if (wi_state[wi-1] == 0)
	{
		format(mes, sizeof(mes), "opened the %s's window (%s).", seat_name[wi-1], VehicleNames[model-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		GameTextForPlayer(playerid, "~w~window ~g~open", 2500, 4);
	}
	else
	{
	    format(mes, sizeof(mes), "closed the %s's window (%s).", seat_name[wi-1], VehicleNames[model-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		GameTextForPlayer(playerid, "~w~window ~r~closed", 2500, 4);
	}

	SetVehicleParamsCarWindows(vehicle, wi_state[0], wi_state[1], wi_state[2], wi_state[3]);
	return 1;
}

ALTX:mycars("/myvehicles", "/myv", "/myveh", "/myvehs");
CMD:mycars(playerid, params[])
{
	new query[64];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `owner`=%i", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query, "ShowListVeh", "i", playerid);

	return 1;
}

ALTX:vfa("/vehiclefastaccess");
CMD:vfa(playerid, params[])
{
	new vehicle;
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "You must be driving a vehicle!");
	if (!(vehicle = GetPlayerVehicleID(playerid))) 		 return SendErrorMessage(playerid, "You must be driving a vehicle!");
	
	new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehicle,engine,lights,alarm,doors,bonnet,boot,objective);

	static mestxt[] = "{FFFFFF}• %s the engine\t{C7C7C7}/(en)gine\n\
					{FFFFFF}• %s vehicle's doors\t{C7C7C7}/lock\n\
					{FFFFFF}• %s lights\t{C7C7C7}/lights\n\
					{FFFFFF}• %s trunk\t{C7C7C7}/trunk\n\
					{FFFFFF}• %s hood\t{C7C7C7}/hood\n\
					{FFFFFF}• Driver's window\t{C7C7C7}/(wi)ndow\n\
					{FFFFFF}• XM Radio\t{C7C7C7}/setstation(/ss)%s";

	new mes[sizeof(mestxt) + 90];

	format(mes, sizeof(mes), mestxt,
		(!engine)?("Start"):("Stop"),
		(doors)?("Unlock"):("Lock"),
		(!lights)?("Turn on"):("Turn off"),
		(!boot)?("Open"):("Close"),
		(!bonnet)?("Open"):("Close"),
		(PlayerInfo[playerid][pCarKey] == vehicle)?("\n	\n{FF6347}> Vehicle management <"):("")
	);

	new title[39];
	format(title, sizeof(title), "{FFFFFF}%s", VehicleNames[GetVehicleModel(vehicle)-400]);

	Dialog_Show(playerid, VehFastAccess, DIALOG_STYLE_TABLIST, title, mes, "Select", "Cancel");
	return 1;
}

CMD:vmenu(playerid, params[])
{
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid,"Your vehicle is not spawned.");
	
	new vehicle = PlayerInfo[playerid][pCarKey];
	if (!vehicle) return 1;

	static mestxt[] = "{FFFFFF}• Vehicle statistics\n\
					• Unpaid fines\n\
					• Park vehicle\n\
					• Find vehicle by GPS\n\
					• Turn off the alarm [ %s{FFFFFF} ]\n\
					• Evacuate vehicle\n\
					• Sell vehicle to the state\n\
					• Sell vehicle to player";
	new mes[sizeof(mestxt)+12],engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(PlayerInfo[playerid][pCarKey],engine,lights,alarm,doors,bonnet,boot,objective);

	format(mes, sizeof(mes), mestxt, (alarm)?("{00E100}ON"):("{FF6347}OFF"));	

	new title[39];
	format(title, sizeof(title), "{FFFFFF}%s", VehicleNames[GetVehicleModel(vehicle)-400]);
	Dialog_Show(playerid, VehMenu, DIALOG_STYLE_LIST, title, mes, "Select", "Cancel");
	return 1;
}	

CMD:fill(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!GetPVarInt(playerid, #UsePump)) {
		new price, Float: f_price, bizid;
		if (!IsDriver(playerid))					return SendNotInVehicle(playerid);
		if ((bizid = AZC_Nearest(playerid)) == -1)	return SendErrorMessage(playerid, "There is no gas station near you. ");
		if (BizInfo[bizid][bizLocked])				return GameTextForPlayer(playerid, "~r~CLOSED!", 2500, 1);
		if (VehicleInfo[vehicle][carEngineOn]) 		return SendErrorMessage(playerid, "Stop the engine.");

		new modelid = GetVehicleModel(vehicle);

		if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'P') 	  f_price = float(BizInfo[bizid][bizEntrance]) * 1.2;
		else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'E') f_price = float(BizInfo[bizid][bizEntrance]) * 0.8;
		else f_price = float(BizInfo[bizid][bizEntrance]);

		price = floatround(f_price, floatround_ceil);
		if (GetCash(playerid) < price) return SendNotCash(playerid);

		new str[40];
		format(str, sizeof(str), "Do you want to start refueling for $%i/л?", price);
		ConfirmDialog(playerid, "Refueling of vehicle", str, "ConfirmFilling");
	} else {
		Fuel_ResetPlayer(playerid);
		ToggleVehicleEngine(vehicle, playerid);
	}
	return 1;
}

ALTX:engine("/en");
CMD:engine(playerid, params[])
{
	if (!IsDriver(playerid)) return 1;

	new vehicle = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][MissionTime])											 return 1;
	if (pTemp[playerid][pCuffed])     												 return SendCuffedMessage(playerid);
	if (IsADMVCar(vehicle) || IsADMVHeli(vehicle) || IsABicycle(vehicle))			 return SendErrorMessage(playerid, "It is impossible to start the engine of this vehicle.");
	if (VehicleInfo[vehicle][carFuel] == 0.0)										 return SendErrorMessage(playerid, "There is no gasoline in the vehicle.");
	if (GetPVarInt(playerid, #UsePump))												 return cmd::fill(playerid, "");
    if (VehicleInfo[vehicle][carOwnerID] && VehicleInfo[vehicle][carEngine] <= 0.0)	 return SendErrorMessage(playerid, "The vehicle's engine is not working.");
    if (VehicleInfo[vehicle][carOwnerID] && VehicleInfo[vehicle][carBattery] <= 0.0) return SendErrorMessage(playerid, "The vehicle battery is not working.");
	if (IsRentCar(vehicle) && pTemp[playerid][pRentveh] != vehicle) 				 return SendErrorMessage(playerid, "vehicle must be rented.");
	if (VehicleInfo[vehicle][Impounded])											 return SendErrorMessage(playerid, "While the vehicle is in the parking lot, you cannot use this function. (/ Unimpound)");
	
	if (VehicleInfo[vehicle][carFaction] && 
		VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])			return 1;

	if (	VehicleInfo[vehicle][carType] >= RENT_CAR && 
		VehicleInfo[vehicle][carType] <= MINER_CAR && 
		pTemp[playerid][pRentveh] != vehicle)	return SendErrorMessage(playerid, "First enter /rentcarjob(/rcj).");


	if (VehicleInfo[vehicle][carOwnerID] && (PlayerInfo[playerid][pCarKey] != vehicle && PlayerInfo[playerid][pSpareKey] != VehicleInfo[vehicle][carID]))
	{
		if (!VehicleInfo[vehicle][carEngineOn])
		{
			if (PlayerInfo[playerid][pJob] != JOB_CARJACKER)	return SendErrorMessage(playerid, "You don't have the keys to the vehicle!");
			
			if (!IsPlayerHasItem(playerid, INV_TOOLS)) 	 return SendErrorMessage(playerid, "You don't have the tools to hack the engine.");

			if (!PoliceOnline())						 return SendErrorMessage(playerid, "The game must have at least %i police officers.", NEED_COPS_TO_CRIME);
			if (VehicleInfo[vehicle][carOwnerID] == 0)   return SendErrorMessage(playerid, "Only personal vehicles can be hacked.");
			if (JackingEngine[playerid] != 0)            return SendErrorMessage(playerid, "You are already trying to start the engine!");

			static const str_size[] = "{"COL_GREY"}Do you want to start breaking the engine %s?";
			new mes[sizeof(str_size) - 4 + 1 + 31 + 7];
			format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(vehicle)-400]); //(VehicleInfo[vehicle][carImmob]+1) * 20
			Dialog_Show(playerid, StartJackingEngine, DIALOG_STYLE_MSGBOX, "{FFFFFF}Breaking the engine", mes, ">>>", "Cancel");
			return 1;
		}		
	}
	
	ToggleVehicleEngine(vehicle, playerid);
	return 1;
}

CMD:dropkey(playerid, params[])
{
	if (!PlayerInfo[playerid][pSpareKey])	return SendErrorMessage(playerid, "You have no keys.");

	PlayerInfo[playerid][pSpareKey] = 0;
	SendSyntaxMessage(playerid, "You threw the keys to the ground.");
	return 1;
}

CMD:carsign(playerid,params[])
{
	new vehicle = GetPlayerVehicleID(playerid), string[128];

	if (!IsDriver(playerid))	return SendNotInVehicle(playerid);
	if (isnull(params))			return SendHelpMessage(playerid, "/carsign [text]");
	if (IsABicycle(vehicle))	return SendErrorMessage(playerid,"You cannot install a car sign on this vehicle!");
	if (strlen(params) > 84)	return SendErrorMessage(playerid, "You cannot enter more than 84 characters.");

	if (strlen(params) > 28)
	{
		new pos = 28, string1[30], string2[30], string3[30];

		if (pos < 28-1) pos = 28;

		if (strlen(params[pos]) > 28)
		{
			format(string1, sizeof(string1), "%.*s", pos, params);
			format(string2, sizeof(string2), "%.*s", pos, params[pos]);
			format(string3, sizeof(string3), "%.*s", pos, params[pos+pos]);
			format(string, sizeof(string), "%s\n%s\n%s", string1, string2, string3);
		}
		else
		{
			format(string1, sizeof(string1), "%.*s", pos, params);
			format(string2, sizeof(string2), "%s", params[pos]);
			format(string, sizeof(string), "%s\n%s", string1, string2);
		}
	}
	else
	{
		format(string, sizeof(string), "%s",params);
	}

	if (PlayerInfo[playerid][pCarKey] == vehicle)
	{
		if (!IsPlayerHasItem(playerid, INV_SIGN))		return SendErrorMessage(playerid, "You don't have a car sign.");
		if (PlayerInfo[playerid][pCarKey] != vehicle)	return SendErrorMessage(playerid, "The car sign can only be installed on your personal vehicle.");

		SendHelpMessage(playerid, "/rcarsign - remove the car sign. ");
		if (!IsValidDynamic3DTextLabel(veicolo_callsign_testo[vehicle]))
		{
		    new Float: x, Float: y, Float: z;
        	GetCarsignPosition(vehicle, x, y, z);
			veicolo_callsign_testo[vehicle] = CreateDynamic3DTextLabel(string,0x33AA33FF, -x, y, z, 20.0, INVALID_PLAYER_ID, vehicle, 1, -1, 0);
		}
		else	UpdateDynamic3DTextLabelText(veicolo_callsign_testo[vehicle], 0x33AA33FF, string);

		return 1;
	}

	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionLegal(faction))
		return SCM(playerid, COLOR_LIGHTRED, "You must be a police officer / paramedic, or be in your car.");

	if (!PlayerInfo[playerid][pOnDuty])
		return SCM(playerid, COLOR_LIGHTRED, "You must be on duty (/duty).");

	if (VehicleInfo[vehicle][carFaction] != faction)
	    return SendErrorMessage(playerid, "This vehicle does not belong to your organization.");

	SendHelpMessage(playerid, "/rcarsign  - remove the car sign. ");
	if (!IsValidDynamic3DTextLabel(veicolo_callsign_testo[vehicle]))
	{
	    new Float: x,
	        Float: y,
	        Float: z;
        GetCarsignPosition(vehicle, x, y, z);
		veicolo_callsign_testo[vehicle] = CreateDynamic3DTextLabel(string,0xFFFFFFFF, -x, y, z, 40.0, INVALID_PLAYER_ID, vehicle, 1, -1, 0);
	}
	else
		UpdateDynamic3DTextLabelText(veicolo_callsign_testo[vehicle], 0xFFFFFFFF, string);
	return 1;
}

CMD:rcarsign(playerid,params[])
{
    if (!IsDriver(playerid))	return SendNotInVehicle(playerid);

	new vehicle = GetPlayerVehicleID(playerid);

    if (IsValidDynamic3DTextLabel(veicolo_callsign_testo[vehicle]))
	{
		DestroyDynamic3DTextLabel(veicolo_callsign_testo[vehicle]);
        veicolo_callsign_testo[vehicle] = Text3D:INVALID_3DTEXT_ID;
	}
	return 1;
}

CMD:stop(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured]) return 1;

	/*if (JackingEngine[playerid] != 0)
	{
		JackingEngine[playerid] = 0;
		UnFreezePlayer(playerid);
		SendSyntaxMessage(playerid, "Вы остановor взлом двигателя.");
	}
	
	if (JackingLock[playerid] != 0)
	{
		JackingLock[playerid] = 0;
		JackingVehicle[playerid] = 0;
		UnFreezePlayer(playerid);
		SendSyntaxMessage(playerid, "Вы остановor взлом дверей.");
	}*/
	
	if (pTemp[playerid][pPlantTime] != 0)
	{
		UnFreezePlayer(playerid);
		StopPlayerAnims(playerid);
		pTemp[playerid][pPlantTime] = 0;
		pTemp[playerid][pPlantType] = 0;
		SendSyntaxMessage(playerid, "You have stopped the landing. ");
	}

	if (pTemp[playerid][pFireTime] != 0)
	{
		StopCreatingPlayersFire(playerid);
		SendSyntaxMessage(playerid, "You have stopped lighting the fire.");
	}
	return 1;
}

CMD:trunk(playerid, params[])
{
	if (IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    new vehicle = GetPlayerVehicleID(playerid);

	    if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))	return SendErrorMessage(playerid, "This vehicle does not have a trunk.");
        if (VehicleInfo[vehicle][carOwnerID] && PlayerInfo[playerid][pCarKey] != vehicle)															return SendErrorMessage(playerid, "This vehicle does not belong to you. ");
        if (VehicleInfo[vehicle][carFaction] != 0 && PlayerInfo[playerid][pFaction] != VehicleInfo[vehicle][carFaction])							return SendErrorMessage(playerid, "This vehicle belongs to another faction.");
		if (VehicleInfo[vehicle][carLocked] || (IsRentCar(vehicle) && !VehicleInfo[vehicle][carRent]))												return SendErrorMessage(playerid, "The vehicle lock is closed. ");

		if (VehicleInfo[vehicle][carTrunkLock] == 1)
		{
		    new mes[56];
			format(mes, sizeof(mes), "~w~TRUNK %s ~r~CLOSED", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleTrunk(vehicle, false);
			SendSyntaxMessage(playerid, "You have closed the trunk of the vehicle.");
			return 1;
		}
		else
		{
			new mes[56];
	        format(mes, sizeof(mes), "~w~TRUNK %s ~g~OPEN", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleTrunk(vehicle, true);
			SendSyntaxMessage(playerid, "You opened the trunk of the vehicle.");
			SendHelpMessage(playerid, "To interact with the trunk, press N next to it.");
		}
		return 1;
	}

	new vehicle = GetNearVehicle(playerid, 4.0);
	if (vehicle == -1)							return SendErrorMessage(playerid, "You are not near vehicle.");
    if (!IsNearTrunk(vehicle, playerid, 2.0)) 	return SendErrorMessage(playerid, "You are not near the trunk! ");

	if (VehicleInfo[vehicle][carOwnerID] && PlayerInfo[playerid][pCarKey] != vehicle)							return SendErrorMessage(playerid, "This vehicle does not belong to you.");
    if (VehicleInfo[vehicle][carFaction] && PlayerInfo[playerid][pFaction] != VehicleInfo[vehicle][carFaction])	return SendErrorMessage(playerid, "This vehicle belongs to another faction.");
    if (VehicleInfo[vehicle][carLocked] || (IsRentCar(vehicle) && VehicleInfo[vehicle][carRent]))				return SendErrorMessage(playerid, "The vehicle lock is closed.");

    if (VehicleInfo[vehicle][carTrunkLock] == 1)
	{
	    new mes[56];
		format(mes, sizeof(mes), "~w~TRUNK %s ~r~CLOSED", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleTrunk(vehicle, false);
		SendSyntaxMessage(playerid, "You have closed the trunk of the vehicle.");
		return 1;
	}
	else
	{
		new mes[56];
        format(mes, sizeof(mes), "~w~TRUNK %s ~g~OPEN", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleTrunk(vehicle, true);
		SendSyntaxMessage(playerid, "You opened the trunk of the vehicle.");
	}
	return 1;
}

CMD:hood(playerid, params[])
{
	if (IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    new vehicle = GetPlayerVehicleID(playerid);
	    
		if (IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))	return SendErrorMessage(playerid, "У этого транспорта нет капота.");
		if (VehicleInfo[vehicle][carLocked])																				return SendErrorMessage(playerid, "Замок транспорта закрыт.");

		if (VehicleInfo[vehicle][carHoodLock] == 1)
		{
		    new mes[56];
			format(mes, sizeof(mes), "~w~%s hood ~r~closed", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleHood(vehicle, false);
			SendSyntaxMessage(playerid, "You have closed the hood of the vehicle.");
			return 1;
		}
		else
		{
			new mes[56];
	        format(mes, sizeof(mes), "~w~%s hood ~g~open", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleHood(vehicle, true);
			SendSyntaxMessage(playerid, "You have opened the hood of the vehicle.");
		}
		return 1;
	}

	new vehicle = 0;

	foreach(new i : Vehicle)
	{
	    if (IsAPlane(i) || IsABiker(i) || IsAHelicopter(i) || IsABoat(i) || IsABicycle(i) || !IsOnHoodVehicle(playerid, i)) continue;

	    vehicle = i;
		break;
	}

	if (!vehicle)							return SendErrorMessage(playerid, "There is no vehicle with a hood near you. ");
    if (VehicleInfo[vehicle][carLocked])	return SendErrorMessage(playerid, "The vehicle lock is closed.");

    if (VehicleInfo[vehicle][carHoodLock] == 1)
	{
	    new mes[56];
		format(mes, sizeof(mes), "~w~%s Hood ~r~Close", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleHood(vehicle, false);
		SendSyntaxMessage(playerid, "You have closed the hood of the vehicle. ");
		return 1;
	}
	else
	{
		new mes[56];
        format(mes, sizeof(mes), "~w~%s Hood ~g~Open", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleHood(vehicle, true);
		SendSyntaxMessage(playerid, "You have opened the hood of the vehicle.");
	}
	return 1;
}

ALTX:vbreaklock("/vbl");
CMD:vbreaklock(playerid, params[])
{
	if (!PoliceOnline())	return SendErrorMessage(playerid, "The game must have at least %i police officers.", NEED_COPS_TO_CRIME);

	new vehid = GetNearVehicle(playerid, 3.0);
	if (!vehid)	return SendErrorMessage(playerid, "There is no vehicle near you.");

	if (VehicleInfo[vehid][carOwnerID] == PlayerInfo[playerid][pID] || PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehid][carOwnerID])
		return SendErrorMessage(playerid, "You cannot hack your vehicle.");
	
	if (VehicleInfo[vehid][carType])			return SendErrorMessage(playerid, "Only personal vehicles can be hacked.");
	if (VehicleInfo[vehid][carOwnerID] == 0)    return SendErrorMessage(playerid, "Only personal vehicles can be hacked.");
	if (VehicleInfo[vehid][carLocked] == 0)     return SendErrorMessage(playerid, "vehicle doors are open.");
	if (JackingLock[playerid] != 0)             return SendErrorMessage(playerid, "You are already breaking the doors.");
	if (!IsPlayerInFrontVDoor(playerid, vehid)) return SendErrorMessage(playerid, "You should be near the driver's door.");

	foreach(new i : Player) {
		if (JackingVehicle[i] == vehid) return SendErrorMessage(playerid, "Someone is already breaking open the doors of this vehicle.");
	}

	new param[10];
	if (sscanf(params, "s[10]", param)) {
		SendHelpMessage(playerid, "/vbl [type] or /vbreaklock [type]");
		SendSyntaxMessage(playerid, "(q)uite - quietly breaking the vehicle door with a crowbar.");
		SendSyntaxMessage(playerid, "(n)oisy - loud breaking of the vehicle, activates the vehicle alarm.");
		return 1;
	}	

	if (CompareStrings(param, "q") || CompareStrings(param, "quite"))
	{
		//if (!IsPlayerHasItem(playerid, INV_PICKLOCK)) return SendErrorMessage(playerid, "У вас должна быть отмычка, чтобы взламывать замок транспорта.");
		if (!CheckHanditem(playerid,18634))		 return SendErrorMessage(playerid, "You should have a crowbar in your hands. (/clothing)");
		if (IsPlayerCrouching(playerid) == false) return SendErrorMessage(playerid, "You must sit down.");

		SetPVarInt(playerid, #JackingVehicle, vehid);
		pTemp[playerid][JackingType] = 1;

		static const str_size[] = "{"COL_GREY"}Do you want to start breaking doors %s? You need %i seconds.";
		new mes[sizeof(str_size) - 4 + 1 + 31 + 7];
		format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(vehid)-400], 250+VehicleInfo[vehid][carLock]*250);
        Dialog_Show(playerid, StartJackingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Breaking doors", mes, ">>>", "Cancel");
	}
	else if (CompareStrings(param, "n") || CompareStrings(param, "noisy"))
	{
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must get up.");

		SetPVarInt(playerid, #JackingVehicle, vehid);
		pTemp[playerid][JackingType] = 2;

		static const str_size[] = "{"COL_GREY"}Do you want to start breaking doors %s?";
		new mes[sizeof(str_size) - 4 + 1 + 31 + 7];
		format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(vehid)-400]);
        Dialog_Show(playerid, StartJackingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Breaking doors", mes, ">>>", "Cancel");
	}
	else {
		SendHelpMessage(playerid, "/vbl [type] or /vbreaklock [type]");
		SendSyntaxMessage(playerid, "(q)uite - quietly breaking the vehicle door with a crowbar.");
		SendSyntaxMessage(playerid, "(n)oisy - loud breaking of the vehicle, activates the vehicle alarm.");
		return 1;
	}

	

	return 1;
}

ALTX:unscramble("/uns");
CMD:unscramble(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid) || !JackingEngine[playerid]) return SendErrorMessage(playerid, "Nu poti folosi aceasta comanda now! ");

	new unword[64];
	if (sscanf(params, "s[64]", unword))	return SendHelpMessage(playerid, "/(uns)cramble [decoded word]");

	new vehid = GetPlayerVehicleID(playerid);
	if (!isnull(unword) && CompareStrings(unword, UnscrambleInfo[pTemp[playerid][pUnscrambleID]][eUnscrambledWord]))
	{
		pTemp[playerid][pScrambleSuccess]++; 
		PlaySound(playerid, 1052);

		switch(VehicleInfo[vehid][carImmob])
		{
			case 0: if (pTemp[playerid][pScrambleSuccess] < 3) return NewScrambleWord(playerid);
			case 1: if (pTemp[playerid][pScrambleSuccess] < 4) return NewScrambleWord(playerid);
			case 2: if (pTemp[playerid][pScrambleSuccess] < 5) return NewScrambleWord(playerid);
			case 3: if (pTemp[playerid][pScrambleSuccess] < 6) return NewScrambleWord(playerid);
			case 4: if (pTemp[playerid][pScrambleSuccess] < 7) return NewScrambleWord(playerid);
		}

		JackingEngine[playerid] = 0;
		pTemp[playerid][pUnscrambleID] = 0;
		pTemp[playerid][pScrambleFailed] = 0;
		pTemp[playerid][pScrambleSuccess] = 0;

		if (IsValidVehicle(GetPlayerVehicleID(playerid))) {
			ToggleVehicleEngine(GetPlayerVehicleID(playerid), playerid);
			PlaySound(playerid, 1145);
			AlarmOff(vehid);
			InfoMSG(playerid, "ENGINE ~g~SUCCESSFULLY ~w~BREAK!", 3);
		}
	}
	else {
		pTemp[playerid][pScrambleFailed]++; 
		PlaySound(playerid,1085);
		
		switch(VehicleInfo[vehid][carImmob])
		{
			case 0: if (pTemp[playerid][pScrambleFailed] == 3) return StopJackingEngine(playerid);
			case 1: {
				if (pTemp[playerid][pScrambleFailed] >= 2){
					PlaySound(playerid, 1138);
					AlarmOn(vehid);
				
					if (pTemp[playerid][pScrambleFailed] == 3) return StopJackingEngine(playerid);
				}
			}
			case 2: {
				if (pTemp[playerid][pScrambleFailed] >= 2){
					PlaySound(playerid, 1138);
					AlarmOn(vehid);
					return StopJackingEngine(playerid);
				}
			}
			case 3: {
				PlaySound(playerid, 1138);
				AlarmOn(vehid);

				if (pTemp[playerid][pScrambleFailed] == 2) return StopJackingEngine(playerid);
			}
			case 4: {
				PlaySound(playerid, 1138);
				AlarmOn(vehid);
				return StopJackingEngine(playerid);
			}
		}
		
		NewScrambleWord(playerid);
	}

	return 1;
}

/*
ALTX:jackingvehicle("/jackv");
CMD:jackingvehicle(playerid, params[])
{
	//new vehicle = GetPlayerVehicleID(playerid);

	if (!VehicleInfo[vehicle][carOwnerID]) 									return SendErrorMessage(playerid, "Вы не можете угнать государственный транспорт.")
	if (!VehicleInfo[vehicle][carEngineOn]) 									return SendErrorMessage(playerid, "Вы не можете угнать заглушенный транспорт.")
	if (PlayerInfo[playerid][pCarKey] == vehicle 
		|| PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehicle][carID])  return SendErrorMessage(playerid, "Вы не можете угнать свой транспорт, а также транспорт, от которого у вас есть ключи.")
	
	if (!PoliceOnline())	return SendErrorMessage(playerid, "В игре дожно быть хотя бы %i полицейских.", NEED_COPS_TO_CRIME);

	new jtime = pTemp[playerid][JackingTime]-gettime();

	if (GetPVarInt(playerid, #JACKING_PROCESS))						  return SendErrorMessage(playerid, "Вы уже угоняете транспорт.");
	if (pTemp[playerid][JackingTime] == 0) 							  return SendErrorMessage(playerid, "Вы не угоняли никакой транспорт.");
	if (jtime > 0) 													  return SendErrorMessage(playerid, "Вы не можете угнать этот транспорт ещё %i секунд.", jtime);
	if (GetPlayerVehicleID(playerid) != pTemp[playerid][JackingVehID]) return SendErrorMessage(playerid, "Вы не угоняли этот транспорт.");

	if (IsPoliceNear(playerid)) return SendErrorMessage(playerid, "Вы не можете угнать транспорт, так как рядом с вами есть полицейские.");

	pTemp[playerid][JackingTime] = 0;
	SetPVarInt(playerid, #JACKING_PROCESS, 1);
	//pTemp[playerid][JackingVehID] = 0;

	DisableWaypoint(playerid);
	if (IsValidDynamicCP(pTemp[playerid][JackingCP])) DestroyDynamicCP(pTemp[playerid][JackingCP]);
	pTemp[playerid][JackingCP] = 0;

	new randomCP = random(sizeof(Jacking_CP));
	pTemp[playerid][JackingCP] = CreateDynamicCP(Jacking_CP[randomCP][0], Jacking_CP[randomCP][1], Jacking_CP[randomCP][2], 1.5, 0, 0, playerid, 15000.0);
	TogglePlayerDynamicCP(playerid, pTemp[playerid][JackingCP], true);
    Streamer_Update(playerid, STREAMER_TYPE_CP);

	InfoMSG(playerid, "ДОСТАВЬТЕ ТРАНСПОРТ НА ~r~ЧЕКПОИНТ~w~, ЧТОБЫ ПОЛУЧИТЬ ЗА НЕГО ~g~ДЕНЬГИ~w~~n~ЕСЛИ ПО ~y~ПРИБЫТИЮ~w~ ЗА ВАМИ БУДЕТ ~y~ХВОСТ~w~, В ТАКОМ СЛУЧАЕ ~r~ЧЕКПОИНТ~w~~n~БУДЕТ ~y~СОЗДАН ЕЩЁ РАЗ!", 8);
	return 1;
}*/

ALTX:deletejackvehicles("/delveh");
CMD:deletejackvehicles(playerid, params[])
{
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM cars WHERE owner = %i AND jacking = 1", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query, "Remove_ShowVehicleList", "i", playerid);

	return 1;
}
