Veh_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    new Float:oldX, Float:oldY, Float:oldZ,
	Float:oldRotX, Float:oldRotY, Float:oldRotZ;

    GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
	GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

    if (GetPVarInt(playerid, #veh_editor) != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
	{
	    new vehicle = GetPVarInt(playerid, #veh_editor), slot = GetPVarInt(playerid, #veh_slot), attobj = GetPVarInt(playerid, #veh_obj);

		DeletePVar(playerid, #veh_editor);
		DeletePVar(playerid, #veh_slot);
		DeletePVar(playerid, #veh_obj);

		new Float:ofx, Float:ofy, Float:ofz, Float:ofaz, Float:finalx, Float:finaly, Float:px, Float:py, Float:pz, Float:roz;
			
		GetVehiclePos(vehicle, px, py, pz);
		GetVehicleZAngle(vehicle, roz);
			
		ofx = x-px, ofy = y-py, ofz = z-pz, ofaz = rz-roz;
		finalx = ofx*floatcos(roz, degrees)+ofy*floatsin(roz, degrees);
		finaly = -ofx*floatsin(roz, degrees)+ofy*floatcos(roz, degrees);
		AttachDynamicObjectToVehicle(objectid, vehicle, finalx, finaly, ofz, rx, ry, ofaz);
		VehicleInfo[vehicle][carAttachObj][slot] = attobj;
		VehicleInfo[vehicle][carAttachPosX][slot] = finalx;
		VehicleInfo[vehicle][carAttachPosY][slot] = finaly;
		VehicleInfo[vehicle][carAttachPosZ][slot] = ofz;
		VehicleInfo[vehicle][carAttachRotX][slot] = rx;
		VehicleInfo[vehicle][carAttachRotY][slot] = ry;
		VehicleInfo[vehicle][carAttachRotZ][slot] = ofaz;

	    if (!GetPVarInt(playerid, "Vehicle:AttEdit")) 
		{
			GiveCash(playerid, -250);
			InfoMSG(playerid, "~w~Voce anexou ~y~objeto~w~ por ~g~$250~w~,~n~~y~~w~/attmenu - gerenciando anexos do seu veículo.", 5);
		}
		else InfoMSG(playerid, "~w~Você mudou a posição do ~y~objeto~w~ com sucesso!", 5);

		Save_CarAttach(vehicle, slot);
	}
	return 1;
}
 
this::Cars_OnGameModeInit()
{
	return mysql_tquery(dbHandle, "SELECT * FROM `cars` WHERE `owner` = '0'", "LoadDynamicCars", "");
}

this::Vehicle_OnGameModeInit()
{
	for(new v = 1; v < MAX_VEHICLES; v++)
	{
		SetVehicleVirtualWorldEx(v, 0);
		SetVehicleInteriorEx(v, 0);
    	VehicleInfo[v] = default_VehicleInfo;
	    VehicleInfo[v][carOwnerPlayer] = INVALID_PLAYER_ID;
	    VehicleInfo[v][carDriver] = INVALID_PLAYER_ID;
		VehicleInfo[v][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(v));
	}
	return 1;
}

Vehicle_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_DRIVER)
	{
		new vehicle = GetPlayerVehicleID(playerid), Float: vhealth;
 		
		AntiCheatGetVehicleHealth(vehicle, vhealth);

		if (!GetPlayerHasRightDrive(playerid, vehicle))  return 1;
	    if (IsABicycle(vehicle)) 						VehicleEngine(vehicle, true);
		
	    VehicleInfo[vehicle][carDriver] = playerid;
	    pTemp[playerid][pDriver] = vehicle;

		if (IsRentCar(vehicle) && VehicleInfo[vehicle][carRent] == 0)
		{
		    SendSyntaxMessage(playerid, "This vehicle is rented for $ 300.");
		    SendHelpMessage(playerid, "Type /rentcar to rent a vehicle.");
		}

		if (	!VehicleInfo[vehicle][carRent] &&
			((IsPizzaCar(vehicle) && PlayerInfo[playerid][pSideJob] == JOB_PIZZA) ||
			(IsTaxiCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TAXI) ||
			(IsMechanicCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_MECHANIC) ||
			(IsTrashMasterCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TRASHMASTER) ||
			(IsTruckCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TRUCKER && GetAvailableTruck(playerid, GetVehicleModel(vehicle))))
		) {
			SendSyntaxMessage(playerid, "You got into a work vehicle.");
		    SendHelpMessage(playerid, "Type /rentcarjob(/rcj), to take this vehicle. ");
		}
		
		if (VehicleInfo[vehicle][carOwnerID] == PlayerInfo[playerid][pID]) 
			SM2(playerid, "Welcome to your %s.",  VehicleNames[GetVehicleModel(vehicle) - 400]);
		
		if (!VehicleInfo[vehicle][carEngineOn] && !IsADMVCar(vehicle) && !IsADMVTaxi(vehicle) && !IsADMVHeli(vehicle) && !IsABicycle(vehicle))
			//SCM(playerid, COLOR_GREEN,"-> Engine not started - press '2' or type /(en)gine.");

		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
			SetPlayerCellphoneAction(playerid, 1);

//		TextDrawShowForPlayer(playerid, TD_vehicle_info[0]);
//    	TextDrawShowForPlayer(playerid, TD_vehicle_info[1]);
//	    PlayerTextDrawSetString(playerid, TD_vehicle_name[playerid], VehicleNames[GetVehicleModel(vehicle)-400]);
//		PlayerTextDrawShow(playerid, TD_vehicle_name[playerid]);
//		SetPVarInt(playerid, #vehicle_info, 5);
	}

	if (oldstate == PLAYER_STATE_DRIVER)
	{
     	new veh = pTemp[playerid][pDriver];
     	
		pTemp[playerid][pDriver] = INVALID_VEHICLE_ID;

		if (veh != INVALID_VEHICLE_ID && VehicleInfo[veh][carDriver] == playerid)	VehicleInfo[veh][carDriver] = INVALID_PLAYER_ID;
		if (JackingEngine[playerid]) 												StopJackingEngine(playerid);
		if (GetPVarInt(playerid, #UsePump))											Fuel_ResetPlayer(playerid);
		for(new i; i < 24; i++) PlayerTextDrawHide(playerid, speedTD[playerid][i]);
	}
	return 1;
}

Vehicle_OnPlayerExitVehicle(playerid, vehicleid)
{
	if (JackingEngine[playerid]) 			StopJackingEngine(playerid);
	if (PlayerInfo[playerid][InMission])	LeaveMission(playerid);
	if (IsPlayerUseMDC(playerid))			MDC_Hide(playerid);
	if (VehicleInfo[vehicleid][carRadioOn])	StopAudioStreamForPlayer(playerid);
	for(new i; i < 24; i++) PlayerTextDrawHide(playerid, speedTD[playerid][i]);
	return 1;
}

V_OnVehicleDamageStatusUpdate(vehicleid)
{
	new panels, doorsd, lightsf, tires;

	GetVehicleDamageStatus(vehicleid, panels, doorsd, lightsf, tires);

	if (!VehicleInfo[vehicleid][carOwnerID]) return 1;

	if (!panels && !doorsd && !lightsf && !tires) return 1;

	new Float:health;

	AntiCheatGetVehicleHealth(vehicleid, health);

	if (health > 750.0)	UpdateVehicleDamageStatus(vehicleid, 0, 0, 0, 0);
	
	if (health < 350.0)
	{
	   	if (VehicleInfo[vehicleid][carEngine] > 0.0)	VehicleInfo[vehicleid][carEngine] -= 1.0;
		if (VehicleInfo[vehicleid][carEngine] < 0.0)
		{
	    	if (VehicleInfo[vehicleid][carEngineOn])	VehicleEngine(vehicleid, false);
			
			VehicleInfo[vehicleid][carEngine] = 0.0;
		}
	}
	return 1;
}