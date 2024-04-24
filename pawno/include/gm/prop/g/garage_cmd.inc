ALTX:garage("/g","/garagem");
CMD:garage(playerid, params[])
{
	new id = GetNearGarage(playerid);

	if (id == -1) return SendErrorMessage(playerid, "Você deve estar perto da garagem.");

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
	    SendHelpMessage(playerid, "(/g)aragem [info / comprar / vender / consertarporta]");
	    return 1;
	}

	if (CompareStrings(option, "info"))
	{
	    new dialog_header[64] = "{FFFFFF}Informação\t{FFFFFF}Valor\n", dialog_body[1024], temp[128];

	    format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

	    format(temp, sizeof(temp), "ID\t%i\nCusto\t%i\nPortão\t%s\n", id, GarageInfo[id][garagePrice], (GarageInfo[id][garageLocked]) ? ("Fechado") : ("Aberto"));
	    strcat(dialog_body, temp);

	    if (GarageInfo[id][garageHouse])
	    {
	        format(temp, sizeof(temp), "Anexado à casa\t%i\n", GarageInfo[id][garageHouse]);
	        strcat(dialog_body, temp);
	    }

	    if (GarageInfo[id][garageBiz])
	    {
	        format(temp, sizeof(temp), "Anexado ao negócio\t%i\n", GarageInfo[id][garageBiz]);
	        strcat(dialog_body, temp);
	    }

	    if (GarageInfo[id][garageOwner])
	    {
	        format(temp, sizeof(temp), "Proprietário\t%s", GarageInfo[id][garageOwnerName]);
	        strcat(dialog_body, temp);
	    }

	    Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Informações da garagem", dialog_body, "Fechar", "");
		return 1;
	}

	else if (CompareStrings(option, "comprar"))
	{
	    if (GarageInfo[id][garageOwner]) return SendErrorMessage(playerid, "Esta garagem não está à venda.");
	    if (GarageInfo[id][garageHouse]) return SendErrorMessage(playerid, "Esta garagem está anexada à casa.");
	    if (GarageInfo[id][garageBiz]) return SendErrorMessage(playerid, "Esta garagem está anexada a um negócio.");

	    new count = 0;

	    for (new i = 0; i < MAX_GARAGES; i++)
	    {
	        if (!GarageInfo[i][garageID] || GarageInfo[i][garageHouse] || GarageInfo[i][garageBiz] || PlayerInfo[playerid][pID] != GarageInfo[i][garageOwner]) continue;

	        count++;
	        break;
	    }

	    if (count) return SendErrorMessage(playerid, "Você não pode comprar mais garagens.");
	    if (GetCash(playerid) < GarageInfo[id][garagePrice]) return SendNotCash(playerid);

	    new dinheiro = PlayerInfo[playerid][pCash];

	    GiveCash(playerid, -GarageInfo[id][garagePrice]);
	    SendSyntaxMessage(playerid, "Você comprou uma garagem [ID: %i] por $%i.", id, GarageInfo[id][garagePrice]);
	    GarageInfo[id][garageOwner] = PlayerInfo[playerid][pID];
	    format(GarageInfo[id][garageOwnerName], 24, "%s", GetNameEx(playerid));

	    SQL_SetInteger("garage", "owner", GarageInfo[id][garageOwner], GarageInfo[id][garageID]);
	    SQL_SetString("users", "name", GarageInfo[id][garageOwnerName], GarageInfo[id][garageOwner]);

	    new mes[256];
	    format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro (ANTES): %s | Dinheiro (AGORA): %s] comprou uma garagem [G_MYSQL: %i] por -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), dinheiro, GetCash(playerid),
	    GarageInfo[id][garageID], GarageInfo[id][garagePrice]);
	    insert_log(mes, PROPERTY_LOG, 5, PlayerInfo[playerid][pAccID]);
	    return 1;
	}


	else if (CompareStrings(option, "vender"))
	{
	    if (GarageInfo[id][garageOwner] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Esta garagem não pertence a você.");
	    if (GarageInfo[id][garageHouse]) return SendErrorMessage(playerid, "Esta garagem está anexada à casa.");
	    if (GarageInfo[id][garageBiz]) return SendErrorMessage(playerid, "Esta garagem está anexada a um negócio.");

	    new dinheiro = PlayerInfo[playerid][pCash];

	    GiveCash(playerid, GarageInfo[id][garagePrice]);
	    SendSyntaxMessage(playerid, "Você vendeu sua garagem [ID: %i] por $%i.", id, GarageInfo[id][garagePrice]);
	    GarageInfo[id][garageOwner] = 0;
	    GarageInfo[id][garageOwnerName][0] = EOS;
	    SQL_SetInteger("garage", "owner", 0, GarageInfo[id][garageID]);
	    //SQL_SetString("garage", "ownername", "", GarageInfo[id][garageID]);

	    new mes[256];
	    format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro (ANTES): %i | Dinheiro (AGORA): %i] vendeu a garagem [G_MYSQL: %i] por -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), dinheiro, GetCash(playerid),
	    GarageInfo[id][garageID], GarageInfo[id][garagePrice]);
	    insert_log(mes, PROPERTY_LOG, 6, PlayerInfo[playerid][pAccID]);
	}

	else if (CompareStrings(option, "repararporta"))
	{
	    if (!GarageInfo[id][garageDDoor]) return SendErrorMessage(playerid,"Sua porta da frente não está danificada.");
	    if (GetCash(playerid) < 300) return SendErrorMessage(playerid, "O preço para consertar a porta é de $ 300.");

	    GarageInfo[id][garageDDoor] = 0;
	    SQL_SetInteger("garage", "ddoor", GarageInfo[id][garageDDoor], GarageInfo[id][garageID]);
	    UpdateGarageDoor(id);
	    GiveCash(playerid, -300); // Subtrai 300 da sua quantia em dinheiro
	    SendSyntaxMessage(playerid, "Você consertou a porta da frente por $300.");
	    return 1;
	}
	return 1;
}