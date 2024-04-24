Dialog:VehicleFines(playerid, response, listitem, inputtext[])
{
	if (!response)						return 1;

	new vehicle;
	if (IsPlayerFactionPolice(playerid) && PlayerInfo[playerid][pOnDuty] && GetPVarInt(playerid, #ticketsuse))
	{
		vehicle = GetPVarInt(playerid, #tickets_show); 
	}
	else 
	{
		if (!IsPlayerInAnyVehicle(playerid))	
			return SendNotInVehicle(playerid);

		vehicle = GetPlayerVehicleID(playerid);

		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])
			return SendErrorMessage(playerid, "Você não é o proprietário deste veículo.");
	}
    
	new query[255];
	if (listitem == 0 && pTemp[playerid][pDialogList])
	{
		mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i AND `paid`=0 AND `id` > %i LIMIT %i", VehicleInfo[vehicle][carID], Tickets[playerid][0][tID], MAX_TICKETS);
		mysql_tquery(dbHandle, query, "ShowCarsTickets", "ii", playerid, 1);
		pTemp[playerid][pDialogList]--;
		return 1;
	}
	
	else if ((listitem == MAX_TICKETS + 1 && pTemp[playerid][pDialogList]) || (listitem == MAX_TICKETS && !pTemp[playerid][pDialogList]))
	{		
		mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i AND `paid`=0 AND `id` < %i ORDER BY `id` DESC LIMIT %i", VehicleInfo[vehicle][carID], Tickets[playerid][MAX_TICKETS-1][tID], MAX_TICKETS);
		mysql_tquery(dbHandle, query, "ShowCarsTickets", "ii", playerid, 0);
		pTemp[playerid][pDialogList]++;
		return 1;
	}

	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i AND `id`=%i LIMIT 1", 
		VehicleInfo[vehicle][carID], 
		(!pTemp[playerid][pDialogList])?(Tickets[playerid][listitem][tID]):(Tickets[playerid][listitem-1][tID]));
	mysql_tquery(dbHandle, query, "ShowCarTicket", "i", playerid);

	return 1;
}

Dialog:VehicleFinesTake(playerid, response, listitem, inputtext[])
{
	if (!response)	
	{
		if (IsPlayerFactionPolice(playerid) && PlayerInfo[playerid][pOnDuty] && GetPVarInt(playerid, #ticketsuse))
			return cmd::checkvfines(playerid, ""); 
		else return cmd::vtickets(playerid, "");
	}

    new vehicle = GetPlayerVehicleID(playerid);

	if (vehicle == 0)													return SendNotInVehicle(playerid);
	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Você não é o proprietário deste veículo.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `tickets` SET `userid`=%i,`vehicle`=0 WHERE `id`=%i", PlayerInfo[playerid][pID], Ticket[playerid][tID]);
	mysql_tquery(dbHandle, query, "VehicleFineTook", "i", playerid);

    for(new i = 0; i < MAX_TICKETS; i++) Tickets[playerid][i][tID] = 0;
	return 1;
}

this::VehicleFineTook(playerid)
{
	SM1(playerid, "Você recebeu a multa do seu veículo.");
	cmd::server_ame(playerid, "pegou uma multa do seu veículo.");
	return 1;
}

Dialog:ShowPlayerFinesShow(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new userid;
	if (IsPlayerFactionPolice(playerid) && PlayerInfo[playerid][pOnDuty] && GetPVarInt(playerid, #ticketsuse))
	{
		userid = GetPVarInt(playerid, #tickets_show); 
	}
	else 
	{
		userid = playerid;
	}
    
	new query[255];
	if (listitem == 0 && pTemp[playerid][pDialogList])
	{
		mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `userid`=%i AND `paid`=0 AND `id` > %i LIMIT %i", PlayerInfo[userid][pID], Tickets[playerid][0][tID], MAX_TICKETS);
		mysql_tquery(dbHandle, query, "ShowPlayersTickets", "ii", playerid, 1);
		pTemp[playerid][pDialogList]--;
		return 1;
	}

	else if ((listitem == MAX_TICKETS + 1 && pTemp[playerid][pDialogList]) || (listitem == MAX_TICKETS && !pTemp[playerid][pDialogList]))
	{	
		mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `userid`=%i AND `paid`=0 AND `id` < %i ORDER BY `id` DESC LIMIT %i", PlayerInfo[userid][pID], Tickets[playerid][MAX_TICKETS-1][tID], MAX_TICKETS);
		mysql_tquery(dbHandle, query, "ShowPlayersTickets", "ii", playerid, 0);
		pTemp[playerid][pDialogList]++;
		return 1;
	}
	
	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =tickets.userid) as ownername FROM `tickets` WHERE `userid`=%i AND `id`=%i LIMIT 1", 
		PlayerInfo[userid][pID], 
		(!pTemp[playerid][pDialogList])?(Tickets[playerid][listitem][tID]):(Tickets[playerid][listitem-1][tID]));
	mysql_tquery(dbHandle, query, "ShowPlayerTicket", "i", playerid);
	
	return 1;
}


Dialog:PlayerFinesPay(playerid, response, listitem, inputtext[])
{
	if (!response)	
	{
		if (IsPlayerFactionPolice(playerid) && PlayerInfo[playerid][pOnDuty] && GetPVarInt(playerid, #ticketsuse))
			return cmd::checkfines(playerid, ""); 
		else return cmd::tickets(playerid, "");
	}
	
    if (!Bank_Nearest(playerid))	return SendErrorMessage(playerid, "Para pagar a multa, você deve estar no banco.");

	if (GetCash(playerid) < Tickets[playerid][0][tAmount])	return SendErrorMessage(playerid, "Você não possui fundos suficientes para pagar está multa $%i.", Ticket[playerid][tAmount]);

	SM1(playerid, "You paid the fine: \"%.20s...\", price $%i.", Ticket[playerid][tReason], Ticket[playerid][tAmount]);
    SendHelpMessage(playerid, "Você ainda tem multas não pagas - %i.", CountPlayerTickets(playerid)-1);
	GiveCash(playerid, -Ticket[playerid][tAmount]);
	
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `tickets` SET `paid`= 0 WHERE `id`=%i", Ticket[playerid][tID]);
	mysql_tquery(dbHandle, query);

	return 1;
} //î