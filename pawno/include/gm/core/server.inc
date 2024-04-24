Server_OnGameModeInit()
{
    SendRconCommand("mapname "MAP_NAME"");
	SendRconCommand("hostname "ServerName"");
    
    DisableInteriorEnterExits();
	ManualVehicleEngineAndLights();
	AllowInteriorWeapons(1);
	EnableStuntBonusForAll(0);
 	DisableNameTagLOS();
	ShowPlayerMarkers(1);
	LimitPlayerMarkerRadius(15.0);
	SetNameTagDrawDistance(15.0);
	SetGameModeText(GameModeText);

	new query[128];
	format(query, sizeof(query), "INSERT INTO `logs_exit` (`date`) VALUES ('%s')", GetFullDate());
    mysql_tquery(dbHandle, query, "OnLogsExitInsert");
	
	//Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, 800);
	
	AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);
    return 1;
}