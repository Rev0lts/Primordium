Player_OnPlayerUpdate(playerid)
{
    if (IsPlayerNPC(playerid) || !IsPlayerLogged(playerid)) 	return 1;
	
    if (IsPlayerAFK(playerid))
	{
		return 1;
		//printf("%s e afk", PlayerInfo[pUsername]);
	//	if (!(PlayerInfo[playerid][pSettings] & togAFK))	SendSyntaxMessage(playerid, "You stayed in AFK %i seconds.", pTemp[playerid][pDarkAFK]-2);
	}

	pTemp[playerid][pDarkAFK] = 0;
    return 1;
}