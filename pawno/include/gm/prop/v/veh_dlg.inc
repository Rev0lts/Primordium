Dialog:DuplicateKey(playerid, response, listitem, inputtext[])
{
	if (!response)																			return 1;
    if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))										return SendErrorMessage(playerid, "Your vehicle is not spawned.");
	if (VehicleInfo[PlayerInfo[playerid][pCarKey]][carOwnerID] != PlayerInfo[playerid][pID])	return 1;

    static const str_size[] = "{ffffff}Duplicate creation for %s. Type {C3C3C3}[ID/Part Of Name] {ffffff}who do you want to give duplicate keys to:";
	new mes[sizeof(str_size) - 2 + 1 + 32];

	format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(PlayerInfo[playerid][pCarKey])-400]);
	    
    new userid;

	if (sscanf(inputtext, "u", userid))
	{
	    Dialog_Show(playerid, DuplicateKey, DIALOG_STYLE_INPUT, "{FFFFFF}Duplicate creation", mes, "Buy", "Cancel");
	    return SendErrorMessage(playerid, "Enter your player ID.");
	}

	if (!IsPlayerLogged(userid) || userid == playerid)
	{
	    Dialog_Show(playerid, DuplicateKey, DIALOG_STYLE_INPUT, "{FFFFFF}Duplicate creation", mes, "Buy", "Cancel");
		return SendNotConnected(playerid);
	}

	if (!PlayerNearPlayer(5.0, playerid, userid))
	{
	    Dialog_Show(playerid, DuplicateKey, DIALOG_STYLE_INPUT, "{FFFFFF}Duplicate creation", mes, "Buy", "Cancel");
		return SendNotNearPlayer(playerid);
	}
	
	if (GetCash(playerid) < 150)	 return SendNotCash(playerid);
	    
	GiveCash(playerid, -150);
	PlayerInfo[userid][pSpareKey] = VehicleInfo[PlayerInfo[playerid][pCarKey]][carID];
	SendSyntaxMessage(playerid, "Duplicate keys was created by %s and transferred to the player %s.", VehicleNames[GetVehicleModel(PlayerInfo[playerid][pCarKey])-400], GetNameEx(userid));
    SendSyntaxMessage(userid, "%s bought a duplicate of keys from %s. (/dropkey - to throw away the duplicate).", GetNameEx(playerid), VehicleNames[GetVehicleModel(PlayerInfo[playerid][pCarKey])-400]);
	return 1;
}

Dialog:ScrapSell(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new veh = PlayerInfo[playerid][pCarKey], string1[128], mes[32];

	if (IsABicycle(veh)) format(mes, sizeof(mes), "%s", VehicleNames[VehicleInfo[veh][carModel]-400]);

	else
	{
		if (VehicleInfo[veh][carRegistered])	format(mes, sizeof(mes), "{FFFFFF}%s [%s]", VehicleNames[VehicleInfo[veh][carModel]-400], VehicleInfo[veh][carPlate]);
		else								format(mes, sizeof(mes), "{FFFFFF}%s", VehicleNames[VehicleInfo[veh][carModel]-400]);
	}

	format(string1, sizeof(string1), "{FFFFFF}Are you sure you want to sell this vehicle for $%i?", GetVehiclePrice(VehicleInfo[veh][carModel])/2);
	Dialog_Show(playerid, ScrapVehicle, DIALOG_STYLE_MSGBOX, mes, string1, "Yes", "No");
	return 1;
}

Dialog:ScrapVehicle(playerid, response, listitem, inputtext[])
{
	if (response)
	{
		new veh = PlayerInfo[playerid][pCarKey];

        SendSyntaxMessage(playerid, "You sold your vehicle \"%s\" for $%i.", VehicleNames[VehicleInfo[veh][carModel]-400], GetVehiclePrice(VehicleInfo[veh][carModel])/2);
		GiveCash(playerid,  GetVehiclePrice(VehicleInfo[veh][carModel])/2);

        new query[256], money = PlayerInfo[playerid][pCash];

		format(query, sizeof(query), "%s [MYSQL: %i | IP: %s | Money(WAS): %i | Money(BECOME): %i] sold the car %s(Model: %i) for -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, 
		GetCash(playerid), VehicleNames[VehicleInfo[veh][carModel]-400], VehicleInfo[veh][carModel], GetVehiclePrice(VehicleInfo[veh][carModel])/2);
	    insert_log(query, VEHICLE_LOG, 3, PlayerInfo[playerid][pAccID]);

		DeleteVehicleTickets(veh);
		
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `cars` WHERE `id` = %i", VehicleInfo[veh][carID]);
		mysql_tquery(dbHandle, query);

		PlayerInfo[playerid][pCarKey] = 0;
		VehicleInfo[veh][carOwnerPlayer] = INVALID_PLAYER_ID;
		DestroyVehicle(veh);
	}
	return 1;
}

Dialog:BuyingVehicleList(playerid, response, listitem, inputtext[])
{
	if (response)	ShowBuyingVehicleDetails(playerid);
	else
	{
	    new registerveh 	= price_reg[GetPVarInt(playerid, "BuyingVehicleRegister")], 
			alarmveh 		= price_alarm[GetPVarInt(playerid, "BuyingVehicleAlarm")],
			lockveh 		= price_lock[GetPVarInt(playerid, "BuyingVehicleLock")],
			immobveh 		= price_immob[GetPVarInt(playerid, "BuyingVehicleImmob")], 
			insuranceveh 	= GetInsurancePrice(GetPVarInt(playerid, "BuyingVehicleModel")) * GetPVarInt(playerid, "BuyingVehicleInsurance"),
			xmradioveh 		= price_radio[GetPVarInt(playerid, "BuyingVehicleRadio")], 
			gpsveh 			= price_gps[GetPVarInt(playerid, "BuyingVehicleGps")];

    	new mes[56];

		format(mes, sizeof(mes), "{FFFFFF}%s - {35B835}$%i", VehicleNames[GetPVarInt(playerid, "BuyingVehicleModel")-400], GetVehiclePrice(GetPVarInt(playerid, "BuyingVehicleModel"))+registerveh+alarmveh+lockveh+immobveh+insuranceveh+xmradioveh+gpsveh);
		Dialog_Show(playerid, BuyingVehicleDone, DIALOG_STYLE_MSGBOX, mes, "{FFFFFF}Are you sure you want to purchase this vehicle?", "Buy", "Cancel");
	}
	return 1;
}

Dialog:BuyingVehicleDone(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new query[128];
   	mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `cars` WHERE `owner` = %i", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query, "BuyVehicleForPlayer", "i", playerid);
	return 1;
}

Dialog:RansomVehicle(playerid, response, listitem, inputtext[])
{
	if (!response) {
		DeletePVar(playerid, #RansomVehModel);
		DeletePVar(playerid, #RansomVehID);
		return 1;
	}

	Dialog_Show(playerid, RansomVehicle2, DIALOG_STYLE_LIST, "Redeem options", "Redeem vehicle\nDelete my vehicle", "Select", "<<");
	return 1;
}
Dialog:RansomVehicle2(playerid, response, listitem, inputtext[])
{
	if (!response) {
		DeletePVar(playerid, #RansomVehModel);
		DeletePVar(playerid, #RansomVehID);
		return 1;
	}

	if (listitem == 0)
	{
		new model = GetPVarInt(playerid, #RansomVehModel),
			vehid = GetPVarInt(playerid, #RansomVehID);
		
		DeletePVar(playerid, #RansomVehModel);
		DeletePVar(playerid, #RansomVehID);

		new price = floatround(GetVehiclePrice(model)*0.5);
		if (GetCash(playerid) < price)	return SendNotCash(playerid);
		
		SendSyntaxMessage(playerid, "You bought your %s for $%i.", VehicleNames[model-400], price);
		SQL_SetInteger("cars", "jacking", 0, vehid);
		GiveCash(playerid, -price);
	} else {
		new mes[128];
		format(mes, sizeof(mes), "{FFFFFF}Do you really want {FF6347}delete{FFFFFF} %s forever and ever?", GetVehName(pTemp[playerid][MyCars][listitem]));
		ConfirmDialog(playerid, "Vehicle deletion confirmation", mes, "DELETE_VEHICLE");
	}
	return 1;
}

Dialog:MyCars(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	
	if (IsValidVehicle(PlayerInfo[playerid][pCarKey]) && pTemp[playerid][MyCarSpawn][listitem])
		return cmd::vmenu(playerid, "");
	else
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))
		{
			pTemp[playerid][MyCarSpawn][0] = listitem;

			new dlgmes[64];
			format(dlgmes, sizeof(dlgmes), "{FFFFFF}Do you really want to spawn %s?", GetVehName(pTemp[playerid][MyCars][listitem]));
			ConfirmDialog(playerid, "Confirmation", dlgmes, "MyCarsSpawn");
		}
		else 
		{
			SendErrorMessage(playerid, "You have already spawned the vehicle.");
			cmd::mycars(playerid, "");
		}
	}

	return 1;
}

Dialog:BuyingVehicleDetails(playerid, response, listitem, inputtext[])
{
	new registerveh = price_reg[GetPVarInt(playerid, "BuyingVehicleRegister")], 
		alarmveh = price_alarm[GetPVarInt(playerid, "BuyingVehicleAlarm")], 
		lockveh = price_lock[GetPVarInt(playerid, "BuyingVehicleLock")], 
		immobveh = price_immob[GetPVarInt(playerid, "BuyingVehicleImmob")], 
		insuranceveh = GetInsurancePrice(GetPVarInt(playerid, "BuyingVehicleModel")) * GetPVarInt(playerid, "BuyingVehicleInsurance"), 
		xmradioveh = price_radio[GetPVarInt(playerid, "BuyingVehicleRadio")], gpsveh = price_gps[GetPVarInt(playerid, "BuyingVehicleGps")];
		
	new registeruse[128], lockuse[128], alarmuse[250], immobuse[260], insuranceuse[260], xmradiouse[128], gpsuse[128];

	if (GetPVarInt(playerid, "BuyingVehicleRegister"))			format(registeruse, sizeof(registeruse), "{ffffff}Registration: \t{ecc317}$%i\n{ecc317}+ {ffffff}Vehicle registration plate.\n\n", registerveh);
	
	if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 1)			format(alarmuse, sizeof(alarmuse), "{ffffff}Alarm: \t{bab9ba}1 level \t{ecc317}$%i\n{ecc317}+ {ffffff}Loud signaling alarm.\n\n", alarmveh);
	else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 2)	format(alarmuse, sizeof(alarmuse), "{ffffff}Alarm: \t{bab9ba}2 level \t{ecc317}$%i\n{ecc317}+ {ffffff}Loud signaling alarm.\n{ecc317}+ {ffffff}Police Jacking Alert.\n\n", alarmveh);
	else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 3)	format(alarmuse, sizeof(alarmuse), "{ffffff}Alarm: \t{bab9ba}3 level \t{ecc317}$%i\n{ecc317}+ {ffffff}Loud signaling alarm.\n{ecc317}+ {ffffff}Police Jacking Alert.\n{ecc317}+ {ffffff}Hacking notification to the owner via SMS.\n\n", alarmveh);
	else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 4)	format(alarmuse, sizeof(alarmuse), "{ffffff}Alarm: \t{bab9ba}4 level \t{ecc317}$%i\n{ecc317}+ {ffffff}Loud signaling alarm.\n{ecc317}+ {ffffff}Police Jacking Alert.\n{ecc317}+ {ffffff}Hacking notification to the owner via SMS and billing.\n\n", alarmveh);
	
	if (GetPVarInt(playerid, "BuyingVehicleLock") > 0)			format(lockuse, sizeof(lockuse), "{ffffff}Lock: \t\t\t{bab9ba}%i level \t{ecc317}$%i\n{ecc317}+ {ffffff}%i seconds lock picking vehicle.\n\n", GetPVarInt(playerid, "BuyingVehicleLock"), lockveh, 250 + (GetPVarInt(playerid, "BuyingVehicleLock") * 250));
	
	if (GetPVarInt(playerid, "BuyingVehicleImmob") > 0)			format(immobuse, sizeof(immobuse), "{ffffff}immobilizer: \t{bab9ba}%i level \t{ecc317}$%i\n{ecc317}+ {ffffff}Helps prevent the ignition switch from being broken.\n\n", GetPVarInt(playerid, "BuyingVehicleImmob"), immobveh);
 	
	if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 1)		format(insuranceuse, sizeof(insuranceuse), "{ffffff}Insurance: \t\t{bab9ba}1 level \t{ecc317}$%i\n{ecc317}+ {ffffff}Vehicles will always respawn with their \n maximum health.\n\n", insuranceveh);
	else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 2)	format(insuranceuse, sizeof(insuranceuse), "{ffffff}Insurance: \t\t{bab9ba}2 level \t{ecc317}$%i\n{ecc317}+ {ffffff}Vehicles will always respawn with their \n maximum health.\n{ecc317}+ {ffffff}Vehicle will respawn without damage.\n\n", insuranceveh);
	else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 3)	format(insuranceuse, sizeof(insuranceuse), "{ffffff}Insurance: \t\t{bab9ba}3 level \t{ecc317}$%i\n{ecc317}+ {ffffff}Vehicles will always respawn with their \n maximum health.\n{ecc317}+ {ffffff}Vehicle will respawn without damage.\n{ecc317}+ {ffffff}Saving tuning and radio.\n\n", insuranceveh);


	if (GetPVarInt(playerid, "BuyingVehicleRadio") == 1)	format(xmradiouse, sizeof(xmradiouse), "{ffffff}XM Radio: \t{ecc317}$%i\n{ecc317}+ {ffffff}Installation of the radio and other components of the audio system.\n\n", xmradioveh);

	if (GetPVarInt(playerid, "BuyingVehicleGps") == 1) 	format(gpsuse, sizeof(gpsuse), "{ffffff}GPS: \t\t{ecc317}$%i\n{ecc317}+ {ffffff}Installing a GPS navigator in a vehicle.\n\n", gpsveh);

	new mes[56];

	format(mes, sizeof(mes), "{ffffff}%s - {35B835}$%i", VehicleNames[GetPVarInt(playerid, "BuyingVehicleModel")-400], GetVehiclePrice(GetPVarInt(playerid, "BuyingVehicleModel"))+registerveh+alarmveh+lockveh+immobveh+insuranceveh+xmradioveh+gpsveh);
	if (!response)
	{
		new vehFuel[32], 
			vehDrive[32],
			modelid = GetPVarInt(playerid, "BuyingVehicleModel"),
			Float:vehMass = GetVehicleModelInfoAsFloat(modelid, "fMass"); 
						
        if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nDriveType") == 'F')
	    	vehDrive = "Front-wheel drive";
	    else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nDriveType") == 'R')
	    	vehDrive = "Rear drive";
	    else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nDriveType") == '4')
	    	vehDrive = "Four-wheel drive";
											
	    if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'P')
	    	vehFuel = "Gasoline";
	    else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'D')
	    	vehFuel = "Diesel";
	    else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'E')
	   		vehFuel = "Electricity";
					
	    static const str_size[] = "{ecc317}Price: \t\t			{ffffff}$%s\n\
                                    {ecc317}Max speed: \t		{ffffff}%.1f km/h\n\
                                    {ecc317}Max state: \t		{ffffff}%.1f\n\
                                    {ecc317}Weight: \t\t		{ffffff}%.1f kf\n\n\
                                    {ecc317}Drive unit: \t		{ffffff}%s\n\
                                    {ecc317}Fuel \t\t			{ffffff}%s\n\
                                    {ecc317}Consumption: \t		{ffffff}%.1f l/100 km\n\
                                    {ecc317}Tank capacity: \t	{ffffff}%.1f l\n\n";

		new string[sizeof(str_size) + 1228];

		format(string, sizeof(string), str_size,
            FormatNumber(GetVehiclePrice(modelid)), 
            GetVehicleMaxSpeed[modelid-400], 
            GetVehicleMaxHealth(modelid), 
            vehMass, 
            vehDrive, 
            vehFuel,
            GetVehicleFuelUsage(modelid),
            GetVehicleFuelCapacity(modelid),
			VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color1)] >>> 8, GetPVarInt(playerid, #veh_color1), 
			VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color2)] >>> 8, GetPVarInt(playerid, #veh_color2)
		);
		
		format(string, sizeof(string), "%s{ecc317}Color 1{ffffff} - [{%06x}#%i{ffffff}]\n{ecc317}Color 2{ffffff} - [{%06x}#%i{ffffff}]\n\n", string, VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color1)] >>> 8, GetPVarInt(playerid, #veh_color1), VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color2)] >>> 8, GetPVarInt(playerid, #veh_color2));
		format(string, sizeof(string), "%s%s%s%s%s%s%s%s\n{ffffff}Click the button '{ecc317}Pay{ffffff}' to purchase vehicle.", string, lockuse, alarmuse, immobuse, insuranceuse, registeruse, xmradiouse, gpsuse);
		
		Dialog_Show(playerid, BuyingVehicleList, DIALOG_STYLE_MSGBOX, mes, string, "Edit", "Pay");
	    return 1;
	}

	new string[301];
	switch(listitem)
	{
		case 0:
		{
				if (GetPVarInt(playerid, "BuyingVehicleRegister") == 0)		format(string, sizeof(string), "{eaf133}>> {ffffff}Not installed\nRegistration - $%i", price_reg[1]);
				else if (GetPVarInt(playerid, "BuyingVehicleRegister") == 1)	format(string, sizeof(string), "{ffffff}Not installed\n{eaf133}>> {ffffff}Registration - $%i", price_reg[1]);
					
				Dialog_Show(playerid, BuyVehDetReg, DIALOG_STYLE_LIST, mes, string, "Put", "<<");
		}
		case 1:
		{
				if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 0)			format(string, sizeof(string), "{ecc317}» {ffffff}Not installed\n1 level alarm- $%i\n2 level alarm- $%i\n3 level alarm- $%i\n4 level alarm- $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 1)	format(string, sizeof(string), "Not installed\n{ecc317}» {ffffff}1 level alarm- $%i\n2 level alarm- $%i\n3 level alarm- $%i\n4 level alarm- $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 2)	format(string, sizeof(string), "Not installed\n1 level alarm- $%i\n{ecc317}» {ffffff}2 level alarm- $%i\n3 level alarm- $%i\n4 level alarm- $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 3)	format(string, sizeof(string), "Not installed\n1 level alarm- $%i\n2 level alarm- $%i\n{ecc317}» {ffffff}3 level alarm- $%i\n4 level alarm- $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 4)	format(string, sizeof(string), "Not installed\n1 level alarm- $%i\n2 level alarm- $%i\n3 level alarm- $%i\n{ecc317}» {ffffff}4 level alarm- $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);

				Dialog_Show(playerid, BuyVehDetAlarm, DIALOG_STYLE_LIST, mes, string, "Put", "<<");
		}
		case 2:
		{
				if (GetPVarInt(playerid, "BuyingVehicleLock") == 0)		format(string, sizeof(string), "{ecc317}» {ffffff}Not installed\n1 level lock - $%i\n2 level lock - $%i\n3 level lock - $%i\n4 level lock - $%i\n5 level lock - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
				else if (GetPVarInt(playerid, "BuyingVehicleLock") == 1)	format(string, sizeof(string), "Not installed\n{ecc317}» {ffffff}1 level lock - $%i\n2 level lock - $%i\n3 level lock - $%i\n4 level lock - $%i\n5 level lock - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
				else if (GetPVarInt(playerid, "BuyingVehicleLock") == 2)	format(string, sizeof(string), "Not installed\n1 level lock - $%i\n{ecc317}» {ffffff}2 level lock - $%i\n3 level lock - $%i\n4 level lock - $%i\n5 level lock - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
				else if (GetPVarInt(playerid, "BuyingVehicleLock") == 3)	format(string, sizeof(string), "Not installed\n1 level lock - $%i\n2 level lock - $%i\n{ecc317}» {ffffff}3 level lock - $%i\n4 level lock - $%i\n5 level lock - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
				else if (GetPVarInt(playerid, "BuyingVehicleLock") == 4)	format(string, sizeof(string), "Not installed\n1 level lock - $%i\n2 level lock - $%i\n3 level lock - $%i\n{ecc317}» {ffffff}4 level lock - $%i\n5 level lock - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
				else if (GetPVarInt(playerid, "BuyingVehicleLock") == 5)	format(string, sizeof(string), "Not installed\n1 level lock - $%i\n2 level lock - $%i\n3 level lock - $%i\n4 level lock - $%i\n{ecc317}» {ffffff}5 level lock - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);

				Dialog_Show(playerid, BuyVehDetLock, DIALOG_STYLE_LIST, mes, string, "Put", "<<");
		}
		case 3:
		{
				if (GetPVarInt(playerid, "BuyingVehicleImmob") == 0)			format(string, sizeof(string), "{ecc317}» {ffffff}Not installed\n1 level immobilizer - $%i\n2 level immobilizer - $%i\n3 level immobilizer - $%i\n4 level immobilizer - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 1)	format(string, sizeof(string), "Not installed\n{ecc317}» {ffffff}1 level immobilizer - $%i\n2 level immobilizer - $%i\n3 level immobilizer - $%i\n4 level immobilizer - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 2)	format(string, sizeof(string), "Not installed\n1 level immobilizer - $%i\n{ecc317}» {ffffff}2 level immobilizer - $%i\n3 level immobilizer - $%i\n4 level immobilizer - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 3)	format(string, sizeof(string), "Not installed\n1 level immobilizer - $%i\n2 level immobilizer - $%i\n{ecc317}» {ffffff}3 level immobilizer - $%i\n4 level immobilizer - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
				else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 4)	format(string, sizeof(string), "Not installed\n1 level immobilizer - $%i\n2 level immobilizer - $%i\n3 level immobilizer - $%i\n{ecc317}» {ffffff}4 level immobilizer - $%i");

				Dialog_Show(playerid, BuyVehDetImmob, DIALOG_STYLE_LIST, mes, string, "Put", "<<");
		}
		case 4:
		{
			    new amount = GetInsurancePrice(GetPVarInt(playerid, "BuyingVehicleModel"));

				if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 0)			format(string, sizeof(string), "{ecc317}» {ffffff}Not installed\n1 level insurance - $%i\n2 level insurance - $%i\n3 level insurance - $%i", amount * 1, amount * 2, amount * 3);
				else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 1)	format(string, sizeof(string), "Not installed\n{ecc317}» {ffffff}1 level insurance - $%i\n2 level insurance - $%i\n3 level insurance - $%i", amount * 1, amount * 2, amount * 3);
				else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 2)	format(string, sizeof(string), "Not installed\n1 level insurance - $%i\n{ecc317}» {ffffff}2 level insurance - $%i\n3 level insurance - $%i", amount * 1, amount * 2, amount * 3);
				else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 3)	format(string, sizeof(string), "Not installed\n1 level insurance - $%i\n2 level insurance - $%i\n{ecc317}» {ffffff}3 level insurance - $%i", amount * 1, amount * 2, amount * 3);

				Dialog_Show(playerid, BuyVehDetInsur, DIALOG_STYLE_LIST, mes, string, "Put", "<<");
		}
		case 5:
		{
				if (GetPVarInt(playerid, "BuyingVehicleRadio") == 0)			format(string, sizeof(string), "{ecc317}» {ffffff}Not installed\nXM Radio - $%i", price_radio[1]);
				else if (GetPVarInt(playerid, "BuyingVehicleRadio") == 1)	format(string, sizeof(string), "Not installed\n{ecc317}» {ffffff}XM Radio - $%i", price_radio[1]);

				Dialog_Show(playerid, BuyVehDetRadio, DIALOG_STYLE_LIST, mes, string, "Put", "<<");
		}
		case 6:
		{
				if (GetPVarInt(playerid, "BuyingVehicleGps") == 0)			format(string, sizeof(string), "{ecc317}» {ffffff}Not installed\nGPS - $%i", price_gps[1]);
				else if (GetPVarInt(playerid, "BuyingVehicleGps") == 1)		format(string, sizeof(string), "Not installed\n{ecc317}» {ffffff}GPS - $%i", price_gps[1]);

				Dialog_Show(playerid, BuyVehDetGPS, DIALOG_STYLE_LIST, mes, string, "Put", "<<");
		}
		case 7:
		{
			SendSyntaxMessage(playerid, "Please select {ffc46a}first{FFFFFF} Colour.");

			new colors[256];

			for(new i; i != 256; i++)
			{
				colors[i] = i;
			}
			SetPVarInt(playerid, #veh_color_type, 1);
			ShowColorSelectionMenu(playerid, MODEL_BUING_COLOR, colors, sizeof(colors));
		}
	}
	return 1;
}

Dialog:BuyVehDetGPS(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleGps", listitem);
		
	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetRadio(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleRadio", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetInsur(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleInsurance", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetImmob(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleImmob", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetLock(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleLock", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetAlarm(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleAlarm", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetReg(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleRegister", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:VehFastAccess(playerid, response, listitem, inputtext[]) 
{
	if (!response) 										return 1;
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "You must be driving a vehicle!");

	switch(listitem)
	{
		case 0: cmd::engine(playerid, "");
		case 1: cmd::vehicle(playerid, "lock");
		case 2: cmd::lights(playerid, "");
		case 3: cmd::trunk(playerid, "");
		case 4: cmd::hood(playerid, "");
		case 5: cmd::window(playerid, "1");
		case 6: cmd::setstation(playerid, "");
		case 7:	cmd::vfa(playerid, "");
		case 8: cmd::vmenu(playerid, "");
	}

	return 1;
}

Dialog:VehMenu(playerid, response, listitem, inputtext[])
{
	if (!response) 										return 1;
    if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Your vehicle is not spawned.");

	new vehicle = PlayerInfo[playerid][pCarKey];
	if (!vehicle || VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID]) return 1;
	
	switch(listitem)
	{
	    case 0: cmd::vehicle(playerid, "stats");
		case 1: cmd::vehicle(playerid, "fines");
	    case 2: cmd::vehicle(playerid, "park");
		case 3:	cmd::vehicle(playerid, "find");
		case 4: cmd::alarmoff(playerid, "");
		case 5: cmd::vehicle(playerid, "tow");
		case 6: cmd::vehicle(playerid, "sell");
		case 7: Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "Vehicle sales", "{ffffff}Vehicle sales. Enter 2 integers: {C3C3C3}[ID/Part Of Name] [sale value]\nExample: 1 35000", "Sell", "Cancel");
	}
	return 1;
}

Dialog:SellVehTo(playerid, response, listitem, inputtext[])
{
	if (!response)										return cmd::vmenu(playerid, "");
    if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Your vehicle is not spawned.");

	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!vehicle)														return 1;
	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You are not the owner of this vehicle.");

    new userid, price;

	if (sscanf(inputtext, "ui", userid, price))
	{
	    Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vehicle sales", "{ffffff}Vehicle sales. Enter 2 integers: {C3C3C3}[ID/Part Of Name] [sale value]\nExample: 1 35000", "Sell", "Cancel");
	    return SendErrorMessage(playerid, "Invalid format.");
	}

	if (!IsPlayerLogged(userid))
	{
	    Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vehicle sales", "{ffffff}Vehicle sales. Enter 2 integers: {C3C3C3}[ID/Part Of Name] [sale value]\nExample: 1 35000", "Sell", "Cancel");
		return SendNotConnected(playerid);
	}

	if (!PlayerNearPlayer(3.0, playerid, userid))
	{
	    Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vehicle sales", "{ffffff}Vehicle sales. Enter 2 integers: {C3C3C3}[ID/Part Of Name] [sale value]\nExample: 1 35000", "Sell", "Cancel");
		return SendNotNearPlayer(playerid);
	}

	if (price < 0 || price > 100000000)
	{
	    Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vehicle sales", "{ffffff}Vehicle sales. Enter 2 integers: {C3C3C3}[ID/Part Of Name] [sale value]\nExample: 1 35000", "Sell", "Cancel");
	    return SendErrorMessage(playerid, "Enter the correct selling price.");
	}

	new str[32];
	format(str, sizeof(str), "sellto %i %i", userid, price);
	cmd::vehicle(playerid, str);
	return 1;
}

Dialog:SelectClass(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem)
	{
		case 0: ShowBclass(playerid);
		case 1: ShowLowriders(playerid);
		case 2: ShowAclass(playerid);
		case 3: ShowSport(playerid);
		case 4: ShowPickups(playerid);
		case 5: ShowOffRoad(playerid);
		case 6: ShowIndustrial(playerid);
		case 7: ShowMotoBike(playerid);
		case 8: ShowOther(playerid);
		case 9: ShowDonate(playerid);
	}
	return 1;
}

Dialog:Autoservice(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	if (!IsDriver(playerid))	return SendNotInVehicle(playerid);

	new Float: health;
	AntiCheatGetVehicleHealth(GetPlayerVehicleID(playerid), health);

	new panels, doors, lights, tires;
	GetVehicleStatus(playerid, panels, doors, lights, tires);

	new 
		vehicle = GetPlayerVehicleID(playerid),
		model = GetVehicleModel(vehicle),
		hp_cost = floatround((100 - ((health/GetVehicleMaxHealth(model))*100)) * (GetVehiclePrice(model)/1000), floatround_ceil)
	;
	if (hp_cost < 0) hp_cost = 0;
	new 
		cost = floatround(GetVehiclePrice(model) / 4, floatround_ceil),
		common_cost = hp_cost +  floatround((cost * 0.5 * panels/100) + (cost * 0.25 * doors/100) + (cost * 0.1 * lights/100) + (cost * 0.15 * tires/100), floatround_ceil)
	;
	
	switch(listitem) {
		case 0: { // Ïî÷èíèòü âñ¸
			if (common_cost <= 0)	return cmd::enter(playerid, "");
			static const dlgstr[] = "{FFFFFF}Are you sure you want to completely repair your vehicle for {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, common_cost);

			ConfirmDialog(playerid, "Confirmation", mes, "RepairAll");
		}
		case 1: { // Âûáðàòü äëÿ ýëåìåíòû äëÿ ïî÷èíêè
			new panel_cost = floatround((cost * 0.5 * panels/100), floatround_ceil),
				door_cost = floatround((cost * 0.25 * doors/100), floatround_ceil),
				light_cost = floatround((cost * 0.1 * lights/100), floatround_ceil),
				tire_cost = floatround((cost * 0.15 * tires/100), floatround_ceil);

			if (panel_cost+door_cost+light_cost+tire_cost<=0)	return cmd::enter(playerid, "");

			static const dlgstr[] = "Select the items you want to fix:\n\
									{C5C5C5}[ - ]{FFFFFF} Repair the body: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5}[ - ]{FFFFFF} Repair the doors: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5}[ - ]{FFFFFF} Repair the headlights: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5}[ - ]{FFFFFF} Repair tires: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5} >> REPAIR >>";
			new mes[526+8*4];

			format(mes, sizeof(mes), dlgstr, 
				panel_cost, panels,
				door_cost, doors,
				light_cost, lights,
				tire_cost, tires
			);

			for(new i; i < 4; i++) pTemp[playerid][pListed][i] = 0;
			Dialog_Show(playerid, SELECT_Autoservice, DIALOG_STYLE_TABLIST_HEADERS, "Vehicle Repair - Select Items", mes, "Select", "Cancel");
		}
		case 2: { // Âîññòàíîâèòü îáùåå ñîñòîÿíèå: 
			if (hp_cost == 0)	return cmd::enter(playerid, "");
			static const dlgstr[] = "{FFFFFF}Are you sure you want to restore the general condition of the vehicle for {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, hp_cost);

			ConfirmDialog(playerid, "Confirmation", mes, "RepairHP");
		}
		case 3: { // Çàìåíèòü àêêóìóëÿòîð
			if (VehicleInfo[vehicle][carBattery] >= 20.0)	{
				cmd::enter(playerid, "");
				return SendErrorMessage(playerid, "The battery of this vehicle does not need repair ( > 20%%).");
			}
			static const dlgstr[] = "{FFFFFF}Are you sure you want to replace the vehicle battery within {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, floatround(GetVehiclePrice(model) / 100 * 5, floatround_ceil));

			ConfirmDialog(playerid, "Confirmation", mes, "ChangeBattery");
		}
		case 4: { // Çàìåíèòü äâèãàòåëü
			if (VehicleInfo[vehicle][carEngine] >= 20.0)	{
				cmd::enter(playerid, "");
				return SendErrorMessage(playerid, "The engine of this vehicle does not need repair ( > 20%%).");
			}
			static const dlgstr[] = "{FFFFFF}Are you sure you want to replace the vehicle engine for {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, floatround(GetVehiclePrice(model) / 100 * 10, floatround_ceil));

			ConfirmDialog(playerid, "Confirmation", mes, "ChangeEngine");
		}
	}
	return 1;
}


Dialog:SELECT_Autoservice(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::enter(playerid, "");

	new panels, doors, lights, tires;
	GetVehicleStatus(playerid, panels, doors, lights, tires);

	new vehicle = GetPlayerVehicleID(playerid),
		model = GetVehicleModel(vehicle),
		cost = floatround(GetVehiclePrice(model) / 4, floatround_ceil), 
		panel_cost = floatround((cost * 0.5 * panels/100), floatround_ceil), 
		door_cost = floatround((cost * 0.25 * doors/100), floatround_ceil),
		light_cost = floatround((cost * 0.1 * lights/100), floatround_ceil),
		tire_cost = floatround((cost * 0.15 * tires/100), floatround_ceil);

	if (listitem == 4) {
		new count;
		for(new i; i < 4; i++) if (pTemp[playerid][pListed][i]) count++;

		if (count) {
			count = 0;

			if (pTemp[playerid][pListed][0]) count += panel_cost;
			if (pTemp[playerid][pListed][1]) count += door_cost;
			if (pTemp[playerid][pListed][2]) count += light_cost;
			if (pTemp[playerid][pListed][3]) count += tire_cost;

			if (count) {
				static const dlgstr[] = "{FFFFFF}Are you sure you want to have your vehicle repaired for {1b8922}$%i{FFFFFF}?";
				new mes[sizeof(dlgstr)+6];

				SetPVarInt(playerid, #auto_cost, count);

				format(mes, sizeof(mes), dlgstr, count);
				return ConfirmDialog(playerid, "Confirmation", mes, "RepairList");
			}
		}
	}

	if (!pTemp[playerid][pListed][listitem]) 	pTemp[playerid][pListed][listitem] = 1;
	else 										pTemp[playerid][pListed][listitem] = 0;

	if (listitem == 0 && !panel_cost)		pTemp[playerid][pListed][listitem] = 0;
	else if (listitem == 1 && !door_cost)	pTemp[playerid][pListed][listitem] = 0;
	else if (listitem == 2 && !light_cost)	pTemp[playerid][pListed][listitem] = 0;
	else if (listitem == 3 && !tire_cost)	pTemp[playerid][pListed][listitem] = 0;

	static const select[] = "{FFFF00}[ X ]{FFFFFF}";
	static const unselect[] = "{C5C5C5}[ - ]{FFFFFF}";

	new mes[526 + 8*4] = "Select the items you want to Repair:\n";

	if (pTemp[playerid][pListed][0]) format(mes, sizeof(mes), "%s%s Repair the body: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, select, panel_cost, panels);
	else							format(mes, sizeof(mes), "%s%s Repair the body: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, unselect, panel_cost, panels);		

	if (pTemp[playerid][pListed][1]) format(mes, sizeof(mes), "%s%s Repair the doors: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, select, door_cost, doors);
	else							format(mes, sizeof(mes), "%s%s Repair the doors: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, unselect, door_cost, doors);			

	if (pTemp[playerid][pListed][2]) format(mes, sizeof(mes), "%s%s Repair the headlights: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, select, light_cost, lights);
	else							format(mes, sizeof(mes), "%s%s Repair the headlights: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, unselect, light_cost, lights);			

	if (pTemp[playerid][pListed][3]) format(mes, sizeof(mes), "%s%s Repair tires: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, select, tire_cost, tires);
	else							format(mes, sizeof(mes), "%s%s Repair tires: {1b8922}$%i {c5c5c5}(Damage: {FF6347}%i%%{c5c5c5})\n", mes, unselect, tire_cost, tires);			

	format(mes, sizeof(mes), "%s{C5C5C5} >> REPAIR >>", mes);
	Dialog_Show(playerid, SELECT_Autoservice, DIALOG_STYLE_TABLIST_HEADERS, "Vehicle Repair - Select Items", mes, "Select", "Cancel");
	return 1;
}


Dialog:ModDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)										return 1;

	if (listitem == 0)
	{
	    if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle))	return SendErrorMessage(playerid, "This function is not available for bicycles, motorcycles and taxis.");
			
	    if (IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1008) != 1 && IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1009) != 1 && IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1010) != 1)
		{
		    ShowModMenu(playerid);
			return SendErrorMessage(playerid, "You cannot install nitro on this vehicle.");
		}

        Dialog_Show(playerid, NitroDialog, DIALOG_STYLE_TABLIST, "{33AA33}Vehicle tuning", "Nitro x2\t$4.500\nNitro x5\t$9.000\nNitro x10\t$15000", "Select", "Return");
		return 1;
	}

	else if (listitem == 1)
	{
	    if (!IsVehiclePaintJob(GetVehicleModel(vehicle)))	return SendErrorMessage(playerid, "Painting works are not available for this vehicle.");

		new mes[128];

		for(new i; i != IsVehiclePaintJob(GetVehicleModel(vehicle)); i++)
		{
			format(mes, sizeof(mes), "%s\nPainting work %i", mes, i+1);
	    }

	    Dialog_Show(playerid, PaintjobDialog, DIALOG_STYLE_LIST, "{33AA33}Price: $3.000", mes, "Select", "Return");
	    return 1;
	}

	else if (listitem == 2)
	{
	    new colors[256];

     	for(new i; i != 256; i++)
		{
      		colors[i] = i;
		}

		ShowColorSelectionMenu(playerid, MODEL_SELECTION_COLOR, colors, sizeof(colors));
	    SetPVarInt(playerid, #veh_color1, -1);
	    SetPVarInt(playerid, #veh_color_type, 2);
	    return 1;
	}

	else if (listitem == 3)
	{
	    if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle))	return SendErrorMessage(playerid, "This function is not available for bicycles, motorcycles and taxis. ");
			
	    if (VehicleInfo[vehicle][carComponent][CARMODTYPE_HYDRAULICS])
		{
		    ShowModMenu(playerid);
			return SendErrorMessage(playerid, "You already have hydraulics installed. ");
		}
	    Dialog_Show(playerid, HydralicConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Vehicle tuning", "{FFFFFF}You definitely want to install hydraulics for $8.000?", "Yes", "Return");
		return 1;
	}

	else if (listitem == 4)
	{
	    if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle))	return SendErrorMessage(playerid, "This function is not available for bicycles, motorcycles and taxis.");
			
		if (VehicleInfo[vehicle][carRadio])
		{
		    ShowModMenu(playerid);
			return SendErrorMessage(playerid, "You already have a radio tape recorder.");
		}

		Dialog_Show(playerid, RadioConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Vehicle tuning", "{FFFFFF}You definitely want to install the radio tape recorder for $400?", "Yes", "Return");
	    return 1;
	}
	else if (listitem == 5)
	{
		if (VehicleInfo[vehicle][carGps])
		{
		    ShowModMenu(playerid);
			return SendErrorMessage(playerid, "You already have a GPS.");
		}

		Dialog_Show(playerid, GPSConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Vehicle tuning", "{FFFFFF}You definitely want to install GPS for $1.500?", "Yes", "Return");
	    return 1;
	}
	else if (listitem == 6)
	{
		new cost = GetVehiclePrice(GetVehicleModel(vehicle)) / 100 * 10;

		if (cost < 1) 					return 1;
		if (GetCash(playerid) < cost)	return SendErrorMessage(playerid, "The cost of replacing the engine is $%i.", cost);

		StartVehicleUpgrade(playerid, vehicle, 9, cost);
		return 1;
	}
	else if (listitem == 7)
	{
		new cost = GetVehiclePrice(GetVehicleModel(vehicle)) / 100 * 5;

		if (cost < 1)					return 1;
		if (GetCash(playerid) < cost)	return SendErrorMessage(playerid, "The cost of replacing the battery is $%i.", cost);

		StartVehicleUpgrade(playerid, vehicle, 11, cost);
		return 1;
	}
	else if (listitem == 8)
	{
		Dialog_Show(playerid, ChangeLock, DIALOG_STYLE_MSGBOX, "{33AA33}Changing the lock", "{FFFFFF}You definitely want to replace the lock for $300?\nAll issued duplicate keys from your vehicle will become invalid.", "Yes", "Return");
	    return 1;
	}
	else if (listitem == 9)
	{
	    static const str_size[] = "1. Lock\t[%i level]\n2. Alarm\t[%i level]\n3. Immobilizer\t[%i level]\n4. Insurance\t[%i level]\n5. Armor\t[%.0f HP]";
		new mes[sizeof(str_size) - 18 + 1 + 48];
  		
		format(mes, sizeof(mes), str_size, VehicleInfo[vehicle][carLock], VehicleInfo[vehicle][carAlarm], VehicleInfo[vehicle][carImmob], VehicleInfo[vehicle][carInsurance], VehicleInfo[vehicle][carArmour]);
  		Dialog_Show(playerid, VehUpgrade, DIALOG_STYLE_TABLIST, "{FFFFFF}Improvements", mes, "Select", "Return");
		return 1;
	}
	else if (listitem == 10)
	{
	    if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle)) return SendErrorMessage(playerid, "This function is not available for bicycles, motorcycles and taxis. ");
			
		new mes[512] = "{AFAFAF}Delete{FFFFFF} nitro\n{AFAFAF}Delete{FFFFFF} painting works\n{AFAFAF}Delete{FFFFFF} hydraulics\n{AFAFAF}Delete{FFFFFF} radio tape recorder";

	    Dialog_Show(playerid, DeleteTuning, DIALOG_STYLE_LIST, "{33AA33}Delete tuning", mes, "Select", "Return");
	    return 1;
	}

	ShowComponentMenu(playerid, vehicle, listitem);
	return 1;
}

Dialog:ModDialogBike(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;
  		
	if (listitem == 0)
	{
	    new colors[256];

     	for(new i; i != 256; i++)
		{
      		colors[i] = i;
		}

		ShowColorSelectionMenu(playerid, MODEL_SELECTION_COLOR, colors, sizeof(colors));
	    SetPVarInt(playerid, #veh_color1, -1);
	    SetPVarInt(playerid, #veh_color_type, 1);
	    return 1;
	}
	return 1;
}

Dialog:BuyVehArmour(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);
	    
    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;

	new amount = strval(inputtext);

    if (amount < 10 || amount > 250)
	{
	    Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installing armor", "{ffffff}Âenter the amount of armor 10 - 250 you want to buy [1 unit costs $10]:", "Buy", "Return");
	    return SendErrorMessage(playerid, "{FFFFFF}amount of armor from 10 to 250. ");
	}

    if (VehicleInfo[vehicle][carArmour] + amount > 250.0)
	{
	    Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installing armor", "{ffffff}Âenter the amount of armor 10 - 250 you want to buy [1 unit costs $10]:", "Buy", "Return");
	    return SendErrorMessage(playerid, "You can only purchase %.0f armor.", 250.0 - VehicleInfo[vehicle][carArmour]);
	}
	
    if (VehicleInfo[vehicle][carAttach3DtextUse]) return SendErrorMessage(playerid, "Wait, the operation is already in progress.");

	if (GetCash(playerid) < amount * 10)
	{
	    Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installing armor", "{ffffff}Âenter the amount of armor 10 - 250 you want to buy [1 unit costs $10]:", "Buy", "Return");
 		return SendErrorMessage(playerid, "The price of the armor is $%i.", amount * 10);
	}

	StartVehicleUpgrade(playerid, vehicle, 6, 10 * amount, amount);
	FreezePlayer(playerid);
	return 1;
}

Dialog:BuyVehInsurance(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);
	    
    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)										return 1;
  		
    new cost = GetInsurancePrice(GetVehicleModel(vehicle));

    if (!cost)										return SendErrorMessage(playerid, "Insurance is not available for this vehicle.");
    if (!VehicleInfo[vehicle][carRegistered])		return SendErrorMessage(playerid, "This vehicle is not registered.");
	if (VehicleInfo[vehicle][carAttach3DtextUse])	return SendErrorMessage(playerid, "Wait, the operation is already in progress.");

	new level = strval(inputtext);

	if (level < 1 || level > 3)						return SendErrorMessage(playerid, "insurance level from 1 to 3. ");
	if (VehicleInfo[vehicle][carInsurance] >= level)	return SendErrorMessage(playerid, "You already have %i insurance level.", VehicleInfo[vehicle][carInsurance]);
	if (GetCash(playerid) < cost * level)			return SendNotCash(playerid);

	StartVehicleUpgrade(playerid, vehicle, 5, cost * level, level);
	return 1;
}

Dialog:VehUpgrade(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);
	    
    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;

    if (listitem == 3)
	{
	    new cost = GetInsurancePrice(GetVehicleModel(vehicle));

	    if (!cost)	return SendErrorMessage(playerid, "Insurance is not available for this vehicle.");
	    
	    new mes[590] = "{ffffff}1 level \t- $%i\n{ecc317}+ {ffffff}Vehicle will always spawn with its maximum health.\n\n{ffffff}2 level \t- $%i\n\
		{ecc317}+ {ffffff}Vehicle will always spawn with its maximum health.\n{ecc317}+ {ffffff}Vehicle will respawn without damage.\n\n{ffffff}3 level \t- $%i\n";
		
	    strcat(mes, "{ecc317}+ {ffffff}Vehicle will always spawn with its maximum health.\n{ecc317}+ {ffffff}Vehicle will respawn without damage.\n{ecc317}+ {ffffff}Preservation of tuning and radio.\n\nEnter the level \tof insurance you want to purchase in the box below:");
	    format(mes, sizeof(mes), mes, cost * 1, cost * 2, cost * 3);
	    Dialog_Show(playerid, BuyVehInsurance, DIALOG_STYLE_INPUT, "{FFFFFF}Buying insurance", mes, "Buy", "Return");
	    return 1;
	}

	else if (listitem == 4)
	{
	    return Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installing armor", "{ffffff}Âenter the amount of armor 10 - 250 you want to buy [1 unit costs $10]:", "Buy", "Return");
	}

	new alarm_mes[][] =
	{
	    {"{ecc317}+ {ffffff}Loud signaling alarm."}, {"{ecc317}+ {ffffff}Loud signaling alarm.\n{ecc317}+ {ffffff}Police Jacking Alert."}, 
		{"{ecc317}+ {ffffff}Loud signaling alarm.\n{ecc317}+ {ffffff}Police Jacking Alert.\n{ecc317}+ {ffffff}Notification of hacking to the owner by SMS. "},
		{"{ecc317}+ {ffffff}Loud signaling alarm.\n{ecc317}+ {ffffff}Police Jacking Alert.\n{ecc317}+ {ffffff}Notification of hacking to the owner via SMS + billing. "}
	};

	new mes[256];

	switch(listitem)
	{
	    case 0:
		{
		    if (VehicleInfo[vehicle][carLock] >= sizeof(price_mod_lock))		return SendErrorMessage(playerid, "Your maximum level castle.");

			format(mes, sizeof(mes), "{ffffff}Upgrade lock to %i level for $%i?\n{ecc317}+ {ffffff}%i seconds of breaking the vehicle lock.", VehicleInfo[vehicle][carLock]+1, price_mod_lock[VehicleInfo[vehicle][carLock]], 250 + (GetPVarInt(playerid, "BuyingVehicleLock") * 250));
		}
		case 1:
		{
		    if (VehicleInfo[vehicle][carAlarm] >= sizeof(price_mod_alarm))	return SendErrorMessage(playerid, "Your maximum level alarm.");

			format(mes, sizeof(mes), "{ffffff}Update alarm to %i level for $%i?\n%s", VehicleInfo[vehicle][carAlarm]+1, price_mod_alarm[VehicleInfo[vehicle][carAlarm]], alarm_mes[VehicleInfo[vehicle][carAlarm]]);
		}
		case 2:
		{
		    if (VehicleInfo[vehicle][carImmob] >= sizeof(price_mod_immob))	return SendErrorMessage(playerid, "Your immobilizer is of the maximum level.");

			format(mes, sizeof(mes), "{ffffff}Update immobilizer to %i level for $%i?\n{ecc317}+ {ffffff}Helps prevent the ignition switch from being broken.", VehicleInfo[vehicle][carImmob]+1, price_mod_immob[VehicleInfo[vehicle][carImmob]]);
		}
	}

	SetPVarInt(playerid, #upgrade_veh, listitem);
	Dialog_Show(playerid, BuyVehUpgrade2, DIALOG_STYLE_MSGBOX, "{FFFFFF}Buying upgrades", mes, "Buy", "Return");
 	return 1;
}

Dialog:BuyVehUpgrade2(playerid, response, listitem, inputtext[])
{
    if (!response)	return ShowModMenu(playerid);

    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)										return 1;
  		
    listitem = GetPVarInt(playerid, #upgrade_veh);

    switch(listitem)
    {
        case 0: StartVehicleUpgrade(playerid, vehicle, 2, price_mod_lock[VehicleInfo[vehicle][carLock]], VehicleInfo[vehicle][carLock]+1);
        case 1: StartVehicleUpgrade(playerid, vehicle, 3, price_mod_alarm[VehicleInfo[vehicle][carAlarm]], VehicleInfo[vehicle][carAlarm]+1);
        case 2: StartVehicleUpgrade(playerid, vehicle, 4, price_mod_immob[VehicleInfo[vehicle][carImmob]], VehicleInfo[vehicle][carImmob]+1);
    }
	return 1;
}

Dialog:SelectModDialog(playerid, response, listitem, inputtext[])
{
    new vehicle = GetPlayerVehicleID(playerid), comptype = GetPVarInt(playerid, #component_type), compid = -1;

    if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
    if (!response)																return ShowModMenu(playerid);
   	if (ModShop_Nearest(playerid) == -1)										return 1;

    for(new i; i != max_vehicle_components[comptype]; i++)
	{
		if (!vehicle_component[comptype][i][compID] || IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), vehicle_component[comptype][i][compID]) != 1) continue;

		compid++;
		if (listitem == compid)
		{
			compid = i;
			break;
		}
   	}

   	new modelid = vehicle_component[comptype][compid][compID];

   	if (GetVehicleComponentType(modelid) == -1 || IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), modelid) != 1)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "This part does not fit your vehicle.");
	}

	if (comptype == -1 || compid == -1)	return SendErrorMessage(playerid, "Details ID error, please contact administration. ");

    new mes[128];
    format(mes, sizeof(mes), "{FFFFFF}You definitely want to install %s for $%i?", GetComponentName(modelid), vehicle_component[comptype][compid][compPrice]);

    Dialog_Show(playerid, ModConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Vehicle tuning", mes, "Yes", "Return");
    SetPVarInt(playerid, #component_model, modelid);
    SetPVarInt(playerid, #component_type, comptype);
    AddVehicleComponent(vehicle, modelid);
	return 1;
}

Dialog:DeleteTuning(playerid, response, listitem, inputtext)
{
    if (!response)	return ShowModMenu(playerid);

    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;
	if (listitem == 9 && VehicleInfo[vehicle][carPaintJob] == 3)
	{
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "You have no painting work installed.");
	}
    if (listitem == 11 && VehicleInfo[vehicle][carRadio] == 0)
    {
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "You do not have a radio tape recorder installed.");
	}

  	new comp_type[11] = {CARMODTYPE_WHEELS, CARMODTYPE_SPOILER, CARMODTYPE_HOOD, CARMODTYPE_ROOF, CARMODTYPE_SIDESKIRT, CARMODTYPE_EXHAUST, CARMODTYPE_FRONT_BUMPER, CARMODTYPE_REAR_BUMPER, CARMODTYPE_NITRO, 0, CARMODTYPE_HYDRAULICS};

	if (listitem != 9 && listitem != 11 && VehicleInfo[vehicle][carComponent][comp_type[listitem]] == 0)
	{
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "You have not installed this part.");
	}

	new comp_name[12][32] = {"wheels", "spoiler", "air intake","the roof","side skirts "," exhaust pipes "," front bumper "," rear bumper "," nitro "," paint work "," hydraulics "," radio "};

	new mes[128];
	format(mes, sizeof(mes), "{FFFFFF}You definitely want to delete %s?", comp_name[listitem]);
	Dialog_Show(playerid, DeleteTuningConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Delete tuning", mes, "Yes", "Return");
    SetPVarInt(playerid, #select_deltun, listitem);
	return 1;
}

Dialog:DeleteTuningConfrim(playerid, response, listitem, inputtext)
{
    if (!response)	return ShowModMenu(playerid);

    listitem = GetPVarInt(playerid, #select_deltun);

    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;
	if (listitem == 9 && VehicleInfo[vehicle][carPaintJob] == 3)
	{
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "You have no painting work installed.");
	}

    if (listitem == 11 && VehicleInfo[vehicle][carRadio] == 0)
    {
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "You do not have a radio tape recorder installed.");
	}

  	new comp_type[11] = {CARMODTYPE_WHEELS, CARMODTYPE_SPOILER, CARMODTYPE_HOOD, CARMODTYPE_ROOF, CARMODTYPE_SIDESKIRT, CARMODTYPE_EXHAUST, CARMODTYPE_FRONT_BUMPER, CARMODTYPE_REAR_BUMPER, CARMODTYPE_NITRO, 0, CARMODTYPE_HYDRAULICS};

	if (listitem != 9 && listitem != 11 && VehicleInfo[vehicle][carComponent][comp_type[listitem]] == 0)
	{
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "You have not installed this part.");
	}
    if (listitem == 9)
	{
	    VehicleInfo[vehicle][carPaintJob] = 3;
	    ChangeVehiclePaintjob(vehicle, 3);
	}
    else if (listitem == 11)
	{
        VehicleInfo[vehicle][carRadio] = 0;
	}
	else
	{
	    RemoveVehicleComponent(vehicle, VehicleInfo[vehicle][carComponent][comp_type[listitem]]);
	    VehicleInfo[vehicle][carComponent][comp_type[listitem]] = 0;
	}

	new comp_name[12][32] = {"wheels", "spoiler", "air intake", "roof", "side skirts", "exhaust pipes", "front bumper", "rear bumper", "nitro", "paint work", "hydraulics", " radio "};

	SendSyntaxMessage(playerid, "You have successfully removed %s from the vehicle.", comp_name[listitem]);
	return 1;
}

Dialog:HydralicConfrim(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if ((modshop = ModShop_Nearest(playerid)) == -1)							return 1;
	if (GetCash(playerid) < 8000)
	{
	   	ShowModMenu(playerid);
		return SendNotCash(playerid);
	}
	if (IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1087) != 1)
	{
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "It is impossible to install hydraulics on this vehicle.");
	}

    if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct])	return SendErrorMessage(playerid, "There are not enough parts in the tuning studio to carry out the work.");
    
	GiveCash(playerid, -8000);
    SetVehicleComponent(vehicle, 1087, GetVehicleComponentType(1087));
	SendSyntaxMessage(playerid, "You purchased hydraulics for $8,000.");
	
	if (BizInfo[modshop][bizOwnerID])
	{
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, 8000 / 100 * 80);
	}

	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:RadioConfrim(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if ((modshop = ModShop_Nearest(playerid)) == -1)								return 1;

	if (VehicleInfo[vehicle][carRadio] == 1)
	{
	    ShowModMenu(playerid);
		return SCM(playerid, COLOR_LIGHTRED, "You already have a radio tape recorder.");
	}
	if (GetCash(playerid) < 400)
	{
	    ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	GiveCash(playerid, -400);
	VehicleInfo[vehicle][carRadio] = 1;
	SendSyntaxMessage(playerid, "You bought your radio for $400. Use /setstation to tune in a radio station.");
    GiveBizCash(modshop, 400);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:GPSConfrim(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if ((modshop = ModShop_Nearest(playerid)) == -1)								return 1;
	if (VehicleInfo[vehicle][carGps] == 1)
	{
	    ShowModMenu(playerid);
		return SendErrorMessage(playerid, "You already have a GPS.");
	}

	if (GetCash(playerid) < 1500)
	{
	    ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	GiveCash(playerid, -1500);
	VehicleInfo[vehicle][carGps] = 1;
	SendSyntaxMessage(playerid, "You purchased a GPS for $1,500. Use /v find to find your vehicle.");
    GiveBizCash(modshop, 1500);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:ChangeLock(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "You must be in a personal vehicle.");
   	if ((modshop = ModShop_Nearest(playerid)) == -1)														return 1;

	if (VehicleInfo[vehicle][carGps] == 1)
	{
	    ShowModMenu(playerid);
		return SendErrorMessage(playerid, "You already have a GPS.");
	}

	if (GetCash(playerid) < 300)
	{
	    ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	GiveCash(playerid, -300);
	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pSpareKey] != VehicleInfo[vehicle][carID]) continue;
			
        PlayerInfo[i][pSpareKey] = 0;
	}
	
	new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `sparekey`='0' WHERE `sparekey`=%i", VehicleInfo[vehicle][carID]);
	mysql_tquery(dbHandle, query);
	
	SendSyntaxMessage(playerid, "You changed the lock for $300.");
    GiveBizCash(modshop, 300);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:ModConfrim(playerid, response, listitem, inputtext[])
{
   	new modshop = -1, vehicle = GetPlayerVehicleID(playerid), modelid = GetPVarInt(playerid, #component_model);
    
	RemoveVehicleComponent(vehicle, modelid);
    LoadComp(vehicle);
    
   	if (!response)																return ShowComponentMenu(playerid, vehicle, GetPVarInt(playerid, #component_type));
	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
	if ((modshop = ModShop_Nearest(playerid)) == -1)							return 1;
	if (GetVehicleComponentType(modelid) == -1 || IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), modelid) != 1)
	{
 		ShowModMenu(playerid);
   		return SendErrorMessage(playerid, "This part does not fit your vehicle.");
	}

	new compid = -1, comptype = -1;

	for(new i; i != sizeof(vehicle_component); i++)
	{
	    for(new a; a != max_vehicle_components[i]; a++)
		{
	    	if (vehicle_component[i][a][compID] != modelid) continue;

			comptype = i;
			compid = a;
			break;
		}

		if (comptype != -1 && compid != -1) break;
	}

	if (comptype == -1 || compid == -1)	return SendErrorMessage(playerid,"Part ID error, please contact administration.");

	if (GetCash(playerid) < vehicle_component[comptype][compid][compPrice])
	{
	    ShowModMenu(playerid);
	    return SendErrorMessage(playerid, "You don't have enough money. The cost of this part $%i.", vehicle_component[comptype][compid][compPrice]);
	}

	if (HasComponent(vehicle, modelid))
 	{
  		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "You already have this part.");
	}

    if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct])	return SendErrorMessage(playerid, "There are not enough parts in the tuning studio to carry out the work.");
        
	GiveCash(playerid, -vehicle_component[comptype][compid][compPrice]);
	
	if (BizInfo[modshop][bizOwnerID] != 0)
	{
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, vehicle_component[comptype][compid][compPrice] / 100 * 80);
	}
	
	SendSyntaxMessage(playerid, "You have acquired %s for $%i.", GetComponentName(modelid), vehicle_component[comptype][compid][compPrice]);
	SetVehicleComponent(vehicle, modelid, GetVehicleComponentType(modelid));
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:PaintjobDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
    if (IsVehiclePaintJob(GetVehicleModel(vehicle)) < listitem+1)				return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;

    if (VehicleInfo[vehicle][carPaintJob] == listitem)
    {
        ShowModMenu(playerid);
		return SendErrorMessage(playerid, "You already have this paint job installed.");
	}

	new mes[128];

	format(mes, sizeof(mes), "{FFFFFF}You definitely want to install the paint job %i for $3.000?", listitem+1);
	Dialog_Show(playerid, PaintjobConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Vehicle tuning", mes, "Yes", "Return");
    SetPVarInt(playerid, #select_paintjob, listitem);

	ChangeVehiclePaintjob(vehicle, listitem);
	return 1;
}

Dialog:PaintjobConfrim(playerid, response, listitem, inputtext[])
{
    listitem = GetPVarInt(playerid, #select_paintjob);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

    ChangeVehiclePaintjob(vehicle, VehicleInfo[vehicle][carPaintJob]);
    
    if (!response)																return ShowModMenu(playerid);
	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if ((modshop = ModShop_Nearest(playerid)) == -1)							return 1;
	if (!IsVehiclePaintJob(GetVehicleModel(vehicle)))							return SendErrorMessage(playerid, "Painting works are not available for this vehicle.");
	if (VehicleInfo[vehicle][carPaintJob] == listitem)
    {
        ShowModMenu(playerid);
		return SendErrorMessage(playerid, "You already have this paint job installed.");
	}
	if (GetCash(playerid) < 3000)
	{
	    ShowModMenu(playerid);
		return SendNotCash(playerid);
	}
	if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct])	return SendErrorMessage(playerid,"There are not enough parts in the tuning studio to carry out the work.");
	
	GiveCash(playerid, -3000);
	VehicleInfo[vehicle][carPaintJob] = listitem;
	ChangeVehiclePaintjob(vehicle, listitem);
	Save_Car(vehicle);
	SendSyntaxMessage(playerid, "You have purchased a paint job %i for $3.000.", listitem+1);

    if (BizInfo[modshop][bizOwnerID] != 0)
    {
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, 3000 / 100 * 80);
	}
	PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:NitroDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)										return 1;

	new nitro_id[3] = {1009, 1008, 1010}, nitro_price[3] = {4500, 9000, 15000};

    if (HasComponent(vehicle, nitro_id[listitem]))
    {
        ShowModMenu(playerid);
		return SendErrorMessage(playerid, "You already have this nitro installed.");
	}

	new mes[128];

	format(mes, sizeof(mes), "{FFFFFF}You definitely want to install nitro %i for $%i?", (listitem == 0)?(2):((listitem == 1)?(5):(10)), nitro_price[listitem]);
	Dialog_Show(playerid, NitroConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Vehicle tuning", mes, "Yes", "Return");
    SetPVarInt(playerid, #select_nitro, listitem);
	return 1;
}

Dialog:NitroConfrim(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

    listitem = GetPVarInt(playerid, #select_nitro);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if ((modshop = ModShop_Nearest(playerid)) == -1)								return 1;

	new nitro_id[3] = {1009, 1008, 1010}, nitro_price[3] = {15000, 15500, 16000}, nitro_level[3] = {2, 5, 10};

    if (HasComponent(vehicle, nitro_id[listitem]))
    {
        ShowModMenu(playerid);
		return SendErrorMessage(playerid, "You already have this nitro installed.");
	}

    if (IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), nitro_id[listitem]) != 1)
    {
        ShowModMenu(playerid);
    	return SendErrorMessage(playerid, "You cannot install nitro on this vehicle.");
	}

	if (GetCash(playerid) < nitro_price[listitem])
	{
	    ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	if (PlayerInfo[playerid][pPremium] < 2)								return SendErrorMessage(playerid, "Only a player with a premium account 'Silver' or higher can purchase nitro.");
    if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct])	return SendErrorMessage(playerid, "There are not enough parts in the tuning studio to carry out the work.");
        
	GiveCash(playerid, -nitro_price[listitem]);
	SetVehicleComponent(vehicle, nitro_id[listitem], CARMODTYPE_NITRO);
	SendSyntaxMessage(playerid, "You have purchased nitro %i for $%i.", nitro_level[listitem], nitro_price[listitem]);

    if (BizInfo[modshop][bizOwnerID] != 0)
    {
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, nitro_price[listitem] / 100 * 80);
	}
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:StartJackingLock(playerid, response, listitem, inputtext[])
{	
	if (!response)											 return 1;

	new vehicle = GetPVarInt(playerid, #JackingVehicle);
	if (!vehicle || !GetNearVehicle(playerid, 3.0, vehicle)) return SendErrorMessage(playerid, "You must be near the vehicle.");
	if (!IsPlayerInFrontVDoor(playerid, vehicle)) 			 return SendErrorMessage(playerid, "You should be near the driver's door.");
	if (!VehicleInfo[vehicle][carLocked])					 return SendErrorMessage(playerid, "Vehicle doors are open.");

	if (pTemp[playerid][JackingType] == 2) {
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must get up.");
		switch(VehicleInfo[vehicle][carLock])
		{
			case 0: VehicleInfo[vehicle][eDoorHealth] = 25, VehicleInfo[vehicle][eDoorEffect] = BLOCK_NONE;
			case 1: VehicleInfo[vehicle][eDoorHealth] = 50, VehicleInfo[vehicle][eDoorEffect] = LESS_DAMAGE_FIST;
			case 2: VehicleInfo[vehicle][eDoorHealth] = 75, VehicleInfo[vehicle][eDoorEffect] = BLOCK_FIST;
			case 3: VehicleInfo[vehicle][eDoorHealth] = 150, VehicleInfo[vehicle][eDoorEffect] = LESS_DAMAGE_MELEE;
			case 4: VehicleInfo[vehicle][eDoorHealth] = 200, VehicleInfo[vehicle][eDoorEffect] = BLOCK_PHYSICAL;
			case 5: VehicleInfo[vehicle][eDoorHealth] = 250, VehicleInfo[vehicle][eDoorEffect] = BLOCK_PHYSICAL;
		}
				
		PlaySound(playerid, 1138);
		AlarmOn(vehicle);
		SendHelpMessage(playerid, "You have to start smashing the door to open the vehicle!");
		VehicleInfo[vehicle][ePhysicalAttack] = true;

		new doorhealth[12];
		format(doorhealth, 12, "%d%%", VehicleInfo[vehicle][eDoorHealth]);
		VehicleInfo[vehicle][eVehicleLabel] = CreateDynamic3DTextLabel(doorhealth, COLOR_WHITE, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, vehicle, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), playerid);

	} else if (pTemp[playerid][JackingType] == 1) {
		if (IsPlayerCrouching(playerid) == false) return SendErrorMessage(playerid, "You must sit down.");
		if (!CheckHanditem(playerid,18634)) 	  return SendErrorMessage(playerid, "You should have a crowbar in your hands.");
		
		JackingLock[playerid] = 250 + VehicleInfo[vehicle][carLock] * 250;
		
		new mes[128];
		format(mes, sizeof(mes), "takes out something and starts fiddling with the lock %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
		cmd::me(playerid, mes);

	} else return 1;
	
	JackingVehicle[playerid] = vehicle;
	return 1;
}

Dialog:StartJackingEngine(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

    new vehicle = GetPlayerVehicleID(playerid);

    if (!vehicle || !IsDriver(playerid))					return SendErrorMessage(playerid, "You must sit behind the driver's seat.");
	if (!VehicleInfo[vehicle][carOwnerID])					return SendErrorMessage(playerid, "This vehicle cannot be opened.");
	if (JackingEngine[playerid])							return SendErrorMessage(playerid, "You are already trying to start the engine. ");
	if (VehicleInfo[vehicle][carEngineOn])					return SendErrorMessage(playerid, "The engine has already started.");
    if (VehicleInfo[vehicle][carFuel] == 0.0)				return SendErrorMessage(playerid, "There is no fuel in the vehicle.");
	if (GetPVarInt(playerid, #vehicle_upgrade_timer) > 0)	return SendErrorMessage(playerid, "It is impossible to start the engine at the moment.");
	if (GetPVarInt(playerid, #UsePump))						return SendErrorMessage(playerid, "It is impossible to start the engine at the moment.");
    if (!IsPlayerHasItem(playerid, INV_TOOLS))				return SendErrorMessage(playerid, "You have nothing to hack into the vehicle's engine. ");

	new mes[128];
	format(mes, sizeof(mes), "picking the ignition %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
	cmd::me(playerid, mes);
		
	pTemp[playerid][pUnscrambleID] = random(sizeof(UnscrambleInfo));
		
	switch(VehicleInfo[vehicle][carImmob])
	{
		case 0: JackingEngine[playerid] = 125;
		case 1: JackingEngine[playerid] = 100;
		case 2: JackingEngine[playerid] = 75;
		case 3: JackingEngine[playerid] = 50;
		case 4: JackingEngine[playerid] = 25;
	}
	
	return 1;
}