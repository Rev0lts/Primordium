ALTX:channel("/ch","/canal");
CMD:channel(playerid, params[])
{
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendSyntaxMessage(playerid, "/(ch)annel [set / leave / rent / sell]");
		return 1;
	}

	if (CompareStrings(option, "set"))
	{
		if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");
		
		new channelid;

		if (sscanf(secoption, "i", channelid))						return SendHelpMessage(playerid, "(/ch)annel set [channel]");
		if (!ChannelExists(channelid))								return SendErrorMessage(playerid, "Channel not found.");
		if (channelid == 911 && !IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid,"Radio channel reserved.");
		if (channelid == 912 && !IsPlayerFactionMedic(playerid))	return SendErrorMessage(playerid, "Radio channel reserved.");
		if (channelid == 998 && !IsPlayerFactionFire(playerid))		return SendErrorMessage(playerid, "Radio channel reserved.");
		if (channelid == GetPlayerChannel(playerid))				return SendErrorMessage(playerid, "You are already connected to this channel.");
		
		if (channelid == 911 && IsPlayerFactionPolice(playerid))
		{
			SendRadioMsg(playerid, "Channel changed [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		else if (channelid == 912 && IsPlayerFactionMedic(playerid))
		{
			SendRadioMsg(playerid, "Channel changed [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		else if (channelid == 998 && IsPlayerFactionFire(playerid))
		{
			SendRadioMsg(playerid, "Channel changed [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		PlayerInfo[playerid][pAuthed] = 0;
		SetRadioChannel(playerid, channelid);
		SendRadioMsg(playerid, "Channel changed [CH: %i].", channelid);
		SendHelpMessage(playerid, "Use /auth to log in.");
		return 1;
	}

	else if (CompareStrings(option, "leave"))
	{
		if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");
		if (!GetPlayerChannel(playerid))				return SendErrorMessage(playerid, "You are not connected to any channel.");

		SendRadioMsg(playerid, "You have left the channel [CH: %i].", GetPlayerChannel(playerid));
		SetRadioChannel(playerid, 0);
		PlaySound(playerid, 6400);
		return 1;
	}

	else if (CompareStrings(option, "rent"))
	{
		if (!IsPlayerInRangeOfPoint(playerid, 2.0, 2269.65, -74.306, 26.772))
		{
			Waypoint_Set(playerid, 2269.65, -74.306, 26.772);
			SendWarningMessage(playerid, "You should be near the city library.");
			return 1;
		}

		new channel, pass[33];

		if (sscanf(secoption, "iS()[33]", channel, pass))	return SendHelpMessage(playerid, "(/ch)annel rent [channel (1-9999)] [password | leave blank if you do not want to set a password]");
		if (strlen(pass) > 30)								return SendErrorMessage(playerid, "Password up to 30 characters.");
		if (channel >= MAX_RADIOS || channel < 1)			return SendErrorMessage(playerid, "Channel 1 to %i.", MAX_RADIOS - 1);
		if (PlayerInfo[playerid][pOwnedChannel])			return SendErrorMessage(playerid, "You already have a channel, first sell it.");
		if (ChannelExists(channel))							return SendErrorMessage(playerid, "Radio channel reserved. ");
		if (StringHasSymbols(pass))							return SendErrorMessage(playerid, "The password can only contain letters and numbers. ");
		
		new cost = 1500;

		if (GetCash(playerid) < cost)						return SendNotCash(playerid);

		new id = -1;

		for(new i = 0; i < MAX_RADIOS; i++)
		{
			if (RadiosInfo[i][radioOn]) continue;

			id = i;
			break;
		}

		if (id == -1)	return SendErrorMessage(playerid, "All frequencies are busy, come back later.");

		GiveCash(playerid, -cost);
		SendRadioMsg(playerid, "You rented a channel [CH: %i] for $%i.", channel, cost);
		SendHelpMessage(playerid, "Use /radiohelp for help.");
		RadiosInfo[id][radioChannel] = channel;
		RadiosInfo[id][radioOwner] = PlayerInfo[playerid][pID];
		format(RadiosInfo[id][radioPass], 32, "%s", pass);
		AddChannelToFile(playerid, channel, pass, id);
		return 1;
	}

	else if (CompareStrings(option, "sell"))
	{
		if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "You are not renting a channel.");
		
		new id = -1;

		if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)
		{
			PlayerInfo[playerid][pOwnedChannel] = 0;
			SendWarningMessage(playerid, "Channel not found.");
			return 1;
		}

		new award = 750;

		GiveCash(playerid, award);
		SendRadioMsg(playerid, "You sold a channel [CH: %i] for $%i.", RadiosInfo[id][radioChannel], award);

		foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || GetPlayerChannel(i) != RadiosInfo[id][radioChannel] || i == playerid) continue;

			SendRadioMsg(i, "The channel you are connected to has been sold.");
			SetRadioChannel(i, 0);
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `channel` = 0, `authed` = 0 WHERE `channel` = %i", RadiosInfo[id][radioChannel]);
		mysql_tquery(dbHandle, query);
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `radios` WHERE `id` = %i", RadiosInfo[id][radioID]);
		mysql_tquery(dbHandle, query);

		RadiosInfo[id][radioOn] = 
		RadiosInfo[id][radioID] = 
		RadiosInfo[id][radioChannel] = 
		RadiosInfo[id][radioOwner] = 0;
		SetRadioChannel(playerid, 0);
		PlayerInfo[playerid][pOwnedChannel] = 0;
		return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "/(ch)annel [set / leave / rent / sell]");
	}
	return 1;
}

CMD:setslot(playerid, params[])
{
    if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");

	new slot;

	if (sscanf(params,"d",slot))			return SendHelpMessage(playerid, "/setslot [slot (0-100)]");
	if (slot < 0 || slot > 100)			return SendErrorMessage(playerid, "Slot from 0 to 100. ");
	if (slot == GetPlayerSlot(playerid))	return SendErrorMessage(playerid, "You are already using this slot. ");

	SetRadioSlot(playerid, slot);
	SendRadioMsg(playerid, "Frequency replaced [SLOT: %i]", slot);
	return 1;
}

CMD:auth(playerid, params[])
{
    if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");
	if (!GetPlayerChannel(playerid))					return SendErrorMessage(playerid, "Your radio is off.");
	if (PlayerInfo[playerid][pAuthed])				return SendErrorRadioMessage(playerid, "You are already logged in to this channel. ");

	new id = -1;

	if ((id = ChannelExists(GetPlayerChannel(playerid))) == 0)	return SendErrorMessage(playerid, "Channel not found.");

	if (CompareStrings(RadiosInfo[id][radioPass], ""))
	{
	    SendRadioMsg(playerid, "You are logged in to the channel %i.", GetPlayerChannel(playerid));
		PlayerInfo[playerid][pAuthed] = 1;
		PlaySound(playerid, 6400);
	    return 1;
	}

	if (isnull(params))										return SendHelpMessage(playerid, "/auth [channel password]");
	if (CompareStrings(RadiosInfo[id][radioPass], params))	return SendErrorRadioMessage(playerid, "You entered the wrong password.");

	SendRadioMsg(playerid, "You are logged in to the channel [CH: %i].", GetPlayerChannel(playerid));
	PlayerInfo[playerid][pAuthed] = 1;
	PlaySound(playerid, 6400);
	return 1;
}

CMD:newauth(playerid, params[])
{
	if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "You are not renting a channel.");

    new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)				return SendErrorMessage(playerid, "Channel not found."); 
	if (isnull(params))																return SendHelpMessage(playerid, "/newauth [new password | if you want to remove the password, then do not enter anything]");
	if (strlen(params) < 1 || strlen(params) > 32)									return SendErrorMessage(playerid, "Characters from 1 to 32.");
	if (CompareStrings(RadiosInfo[id][radioPass], "") && CompareStrings(params, ""))	return SendErrorMessage(playerid, "Your channel has no password.");
	if (!CompareStrings(params, "") && StringHasSymbols(params))						return SendErrorMessage(playerid, "Only letters and numbers can be used in the password.");

	if (CompareStrings(params, ""))	SendRadioMsg(playerid, "Your channel now has no password. ");
	else							SendRadioMsg(playerid, "You changed your channel password to %s.", params);

	format(RadiosInfo[id][radioPass], 33, "%s", params);
	SQL_SetString("radios", "pass", RadiosInfo[id][radioPass], RadiosInfo[id][radioID]);
	return 1;
}

CMD:radiomembers(playerid, params[])
{
    if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "You are not renting a channel.");

    new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)	return SendErrorMessage(playerid, "Channel not found.");

	new dialog_header[64] = "{FFFFFF}Name\t{FFFFFF}ID\n", dialog_body[2048];

	format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || GetPlayerChannel(i) != RadiosInfo[id][radioChannel]) continue;

	    format(dialog_body, sizeof(dialog_body), "%s%s\t%i\n", GetNameEx(i), i);
	}
	
	Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}List of players in the channel", dialog_body, "Close", "");
	return 1;
}

CMD:kickoffradio(playerid, params[])
{
    if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "You are not renting a channel.");

	new userid;

	if (sscanf(params,"u", userid))	return SendHelpMessage(playerid, "/kickoffradio [ID/Part Of Name]");
	if (!IsPlayerLogged(userid))		return SendNotConnected(playerid);
	if (userid == playerid)			return SendErrorMessage(playerid, "You cannot kick yourself out of the channel.");

    new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)	return SendErrorMessage(playerid, "Channel not found.");
	if (RadiosInfo[id][radioChannel] != GetPlayerChannel(playerid))		return SendErrorMessage(playerid, "You must be in your channel [CH: %i].", RadiosInfo[id][radioChannel]);
	if (GetPlayerChannel(userid) != GetPlayerChannel(playerid))			return SendErrorMessage(playerid, "This player is not in this channel. ");

	SendRadioMsg(playerid, "You kicked %s.", GetNameEx(id));
	SendRadioMsg(userid, "You were kicked from the channel. ");
	SetRadioChannel(userid, 0);
	return 1;
}

ALTX:r("/radio");
CMD:r(playerid, params[])
{
    if (PlayerInfo[playerid][pMutedTime] > 0)		return SendErrorMessage(playerid,"Your local chat is blocked by the system.");

    new channel = GetPlayerChannel(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");
	if (channel == 0)							return SendErrorMessage(playerid, "You are not on frequency.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorMessage(playerid, "You are not logged into the frequency.");

	if (channel == 911) {
		new tag[6], arg[25];
		if (!sscanf(params, "s[6]s[24]", tag, arg)) { 
			if (CompareStrings(tag, "10-27")) {
				new userid;
				if (sscanf(arg, "u", userid)) return SendErrorMessage(playerid, "/r 10-27 [ID/Part Of Name]");
				if (!IsPlayerLogged(userid))	 return SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: No results found..");

				SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: %s, sex: %s, age: %i, unpaid fines: %s, search: %s.", GetNameEx(userid), (PlayerInfo[userid][pSex]==1)?("male"):("female"), GetAge(userid), (CountPlayerTickets(userid))?("{FF6347}%i{9189EF}"):("No"), (IsPlayerWanted(userid))?("Yes"):("No"));
				SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: Dispatcher, 10-27 %s.", GetNameWithMask(playerid), GetNameEx(userid));
				return 1;
			} else if (CompareStrings(tag, "10-28")) {
				new plates[10], vehicle;
				if (sscanf(arg, "s[10]", plates))				 return SendErrorMessage(playerid, "/r 10-28 [plates]");
				if ((vehicle = FindVehicleByPlate(plates)) == -1) return SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: No results found..");
				
				PlayCrimeReportForPlayer(playerid, playerid, 13);
				
				SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ: %s [%s], owner: %s, unpaid fines: %s, search: %s.", VehicleNames[GetVehicleModel(vehicle)-400], VehicleInfo[vehicle][carPlate], VehicleInfo[vehicle][carOwner], (CountVehicleTickets(vehicle))?("%i"):("No"), (VehicleInfo[vehicle][carWanted])?("Yes"):("No"));
				SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: Dispatcher, 10-28 %s.", GetNameWithMask(playerid), VehicleInfo[vehicle][carPlate]);
				return 1;
			}
		}
	}
	
	if (sscanf(params, "s[144]", params)) 		return SendHelpMessage(playerid, "/r [text]");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		if (pTemp[playerid][pPatrol] && channel == 911) {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s: %.*s ...", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		} else {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s: %.*s ...", channel, GetNameEx(playerid), pos, params);
		}

		SendRadioMessage(channel, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol] && channel == 911) {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s: %s", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		} else {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s: %s", channel, GetNameEx(playerid), params);
		}
			
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

CMD:rr(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rr [text]");
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Your local chat is blocked by the system.");

    new channel = GetPlayerChannel(playerid),
		slot = GetPlayerSlot(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");
	if (channel == 0)							return SendErrorRadioMessage(playerid, "You are not on frequency.");
	if (PlayerInfo[playerid][pAuthed] == 0)		return SendErrorRadioMessage(playerid, "You are not logged into the frequency.");

	new mes[128];
	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		if (pos < MAXLEN-1) pos = MAXLEN;
		if (pTemp[playerid][pPatrol])
		    format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s: %.*s ...", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		else
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s: %.*s ...", channel, slot, GetNameEx(playerid), pos, params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);

		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol])
			format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s: %s", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		else
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s: %s", channel, slot, GetNameEx(playerid), params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

ALTX:rlow("/rl","/rb");
CMD:rlow(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rlow [text]");
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Your local chat is blocked by the system.");

    new channel = GetPlayerChannel(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");
	if (!channel)								return SendErrorMessage(playerid, "You are not on frequency.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorMessage(playerid, "You are not logged into the frequency.");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s [quiet]: %.*s ...", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: 0] %s [quiet]: %.*s ...", channel, GetNameEx(playerid), pos, params);

		SendRadioMessage(channel, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s [quiet]: %s", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: 0] %s [quiet]: %s", channel, GetNameEx(playerid), params);

		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

ALTX:rrlow("/rrl","/rrb");
CMD:rrlow(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rrlow [text]");
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid,"Your local chat is blocked by the system.");

    new channel = GetPlayerChannel(playerid), slot = GetPlayerSlot(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "You don't have a walkie-talkie.");
	if (!channel)								return SendErrorRadioMessage(playerid, "You are not on frequency.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorRadioMessage(playerid, "You are not logged into the frequency.");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s [quiet]: %.*s ...", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), pos, params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: %i] %s [quiet]: %.*s ...", channel, slot, GetNameEx(playerid), pos, params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
	    if (pTemp[playerid][pPatrol])	format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s [quiet]: %s", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName], GetNameEx(playerid), params);
		else							format(mes, sizeof(mes), "**[CH: %i, S: %i] %s [quiet]: %s", channel, slot, GetNameEx(playerid), params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s spune [quiet]: %s", GetNameWithMask(playerid), params);
	}
	return 1;
} //î