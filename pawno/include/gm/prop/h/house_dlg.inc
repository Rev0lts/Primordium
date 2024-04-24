Dialog:HouseMenu(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new houseid;
	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Você não está perto de casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	switch(listitem)
	{
	    case 0: cmd::lock(playerid, ""); // fechadura da porta
		case 1: { // Melhorias da casa
			static dlgtxt[] = "{FFFFFF}Consertar porta [%s] {33CC33}[$300]{FFFFFF}\n\
								Comprar rádio {33CC33}[$500]{FFFFFF}\n\
								Melhorar a porta\n\
								Melhorar fechadura\n\
								Melhorar alarme\n\
								Comprar armário {33CC33}[$600]{FFFFFF}\n\
								Comprar kit de primeiros socorros";

			new diatxt[sizeof(dlgtxt) + 30], door[30];	

			if (!HouseInfo[houseid][hDDoor]) format(door, sizeof(door), "{9ACD32}NÃO NECESSÁRIO{FFFFFF}");
			else 							format(door, sizeof(door), "{FF6347}NECESSÁRIO{FFFFFF}");

			format(diatxt, sizeof(diatxt), dlgtxt, door);

			Dialog_Show(playerid, HouseUpgrade, DIALOG_STYLE_LIST, "Melhorias da casa", diatxt, "Selecionar", "Voltar");
		}
		case 2: cmd::furniture(playerid, ""); // Gerenciamento de móveis
		case 3: // Inquilinos
		{
		    new query[128];
			mysql_format(dbHandle, query, sizeof(query), "SELECT `id`, `name` FROM `users` WHERE `rent_key` = %i LIMIT %i", HouseInfo[houseid][hID], MAX_TENAT_SPACE);
			mysql_tquery(dbHandle, query, "ShowTenantsAmount", "i", playerid);
		}
		case 4: // Inventário da casa
		{
		    if (!IsPlayerInsideHouse(playerid, houseid))		return SendErrorMessage(playerid, "Você deve estar dentro da casa.");
		        
			PrintHouseInv(playerid, houseid);
		}
		case 5: // Cofre com dinheiro
		{
			static dlgtxt[] = "No cofre: $%i\n\
								{FF6347}>{FFFFFF} Colocar\n\
								{FF6347}>{FFFFFF} Retirar";

			new diatxt[sizeof(dlgtxt) + 11];		
			format(diatxt, sizeof(diatxt), dlgtxt, HouseInfo[houseid][hCash]);

			Dialog_Show(playerid, HouseSafeChoose, DIALOG_STYLE_TABLIST, "Cofre com dinheiro", diatxt, "Selecionar", "Voltar");
			return 1;
		}
		case 6: cmd::clothes(playerid, ""); // Armário com roupas
		case 7: Dialog_Show(playerid, SellHouseChoose, DIALOG_STYLE_LIST, "{FFFFFF}Casa à venda", "Vender uma casa para um jogador\nVender uma casa para o estado", "Selecionar", "Voltar");
	}
	return 1;
}


Dialog:HouseUpgrade(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	switch(listitem)
	{
		case 0: { // Consertar porta
			if (!HouseInfo[houseid][hDDoor])	return SendErrorMessage(playerid, "Sua porta da frente não está danificada.");
			if (GetCash(playerid) < 300)		return SendErrorMessage(playerid, "O custo do conserto da porta é $300. ");

			HouseInfo[houseid][hDDoor] = 0;
			SQL_SetInteger("houses", "ddoor", HouseInfo[houseid][hDDoor], HouseInfo[houseid][hID]);
			UpdateHouseDoor(houseid);
			GiveCash(playerid, -300);
			SendSyntaxMessage(playerid, "Você consertou a porta da frente por $300. ");
			return 1;
		}
		case 1: { // Comprar rádio
			if (HouseInfo[houseid][hRadio])	return SendErrorMessage(playerid, "Um rádio já foi instalado nesta casa. ");
			if (GetCash(playerid) < 500)		return SendErrorMessage(playerid, "Você não tem dinheiro suficiente. Você precisa de $500.");

			HouseInfo[houseid][hRadio] = 1;
			SQL_SetInteger("houses", "radio", HouseInfo[houseid][hRadio], HouseInfo[houseid][hID]);
			GiveCash(playerid, -500);
			SendSyntaxMessage(playerid, "Você comprou um rádio por $500. Use /setestacao.");
			return 1;
		}
		case 2: { // Melhorar porta
			static dlgtxt[] = "Nível\tTipo de porta\tCaracterísticas\tPreço\n\
								{C3C3C3} Nível 1 {FFFFFF}\t Madeira\t + 5% proteção contra arrombamento\t {33CC33} $750\n\
								{C3C3C3} Nível 2 {FFFFFF}\tFerro\t + 15% proteção contra arrombamento\t {33CC33} $1500\n ";

			Dialog_Show(playerid, HouseUpgradeDoor, DIALOG_STYLE_TABLIST_HEADERS, "Melhorar porta", dlgtxt, "Comprar", "Voltar");
		}
		case 3: { // Melhorar fechadura
			static dlgtxt[] = "Nível\tCaracterística\tPreço\n\
							{C3C3C3} Nível 1 {FFFFFF}\t + 5% proteção contra arrombamento\t {33CC33} $ 250\n\
							{C3C3C3} Nível 2 {FFFFFF}\t + 10% proteção contra arrombamento\t {33CC33} $ 500\n\
							{C3C3C3} Nível 3 {FFFFFF}\t + 15% proteção contra arrombamento\t {33CC33} $ 750\n\
							{C3C3C3} Nível 4 {FFFFFF}\t + 20% proteção contra arrombamento\t {33CC33} $ 1000\n\
							{C3C3C3} Nível 5 {FFFFFF}\t + 25% proteção contra arrombamento\t {33CC33} $ 1250\n ";

			Dialog_Show(playerid, HouseUpgradeLock, DIALOG_STYLE_TABLIST_HEADERS, "Melhorar fechadura", dlgtxt, "Comprar", "Voltar");
		}
		case 4: { // Melhorar alarme
			static dlgtxt[] = "Nível\tCaracterística\tPreço\n\
							{C3C3C3} Nível 1 {FFFFFF}\tPadrão\t {33CC33} $500\n\
							{C3C3C3} Nível 2 {FFFFFF}\tMédio\t {33CC33} $1000\n\
							{C3C3C3} Nível 3 {FFFFFF}\tBom\t {33CC33} $1500\n ";
			Dialog_Show(playerid, HouseUpgradeAlarm, DIALOG_STYLE_TABLIST_HEADERS, "Melhorar alarme", dlgtxt, "Comprar", "Voltar");
		}
		case 5: { // Comprar armário
			if (HouseInfo[houseid][hCupboard])	return SendErrorMessage(playerid, "Você já tem um armário. ");
			if (GetCash(playerid) < 600)		return SendErrorMessage(playerid, "O preço do armário é $600.");

			HouseInfo[houseid][hCupboard] = 1;
			SQL_SetInteger("houses", "cupboard", HouseInfo[houseid][hCupboard], HouseInfo[houseid][hID]);
			GiveCash(playerid, -600);
			SendSyntaxMessage(playerid, "Você comprou um armário por $600.");
			SendSyntaxMessage(playerid, "Digite /roupas para usar. ");
			return 1;
		}
		case 6: { // Comprar kit de primeiros socorros
			static dlgtxt[] = "Nível\tSaúde\tRecuperação\tPreço\n\
							{C3C3C3} Nível 1 {FFFFFF}\t + 20 HP\t + 60 segundos\t {33CC33} $400\n\
							{C3C3C3} Nível 2 {FFFFFF}\t + 40 HP\t + 80 segundos\t {33CC33} $800\n\
							{C3C3C3} Nível 3 {FFFFFF}\t + 60 HP\t + 100 segundos\t {33CC33} $1200\n\
							{C3C3C3} Nível 4 {FFFFFF}\t + 80 HP\t + 120 segundos\t {33CC33} $1600\n\
							{C3C3C3} Nível 5 {FFFFFF}\t + 100 HP\t + 140 segundos\t {33CC33} $2000\n";

			Dialog_Show(playerid, HouseUpgradeHeal, DIALOG_STYLE_TABLIST_HEADERS, "Comprar kit de primeiros socorros", dlgtxt, "Comprar", "Voltar");	
		}
	}

	return 1;
}


Dialog:HouseUpgradeDoor(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	listitem += 1;

	if (HouseInfo[houseid][hDoor] >= listitem)	return SendErrorMessage(playerid, "Você já instalou uma porta de nível %i.", HouseInfo[houseid][hDoor]);
	if (GetCash(playerid) < 750 * listitem)		return SendErrorMessage(playerid, "Você não tem $%i.", listitem * 750);

	HouseInfo[houseid][hDoor] = listitem;
	SQL_SetInteger("houses", "door", HouseInfo[houseid][hDoor], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 750);
	SendSyntaxMessage(playerid, "Você adquiriu uma porta de nível %i por $%i.", HouseInfo[houseid][hDoor], listitem * 750);
	return 1;
}

Dialog:HouseUpgradeLock(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	listitem += 1;

	if (HouseInfo[houseid][hLock] >= listitem)	return SendErrorMessage(playerid, "Você já tem uma fechadura de nível %i.", HouseInfo[houseid][hLock]);
	if (GetCash(playerid) < 250 * listitem)		return SendErrorMessage(playerid, "Você não tem $%i.", listitem * 250);
	
	HouseInfo[houseid][hLock] = listitem;
	SQL_SetInteger("houses", "lock", HouseInfo[houseid][hLock], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 250);
	SendSyntaxMessage(playerid, "Você comprou uma fechadura de nível %i por $%i.", HouseInfo[houseid][hLock], listitem * 250);
	return 1;
}

Dialog:HouseUpgradeAlarm(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	listitem += 1;

	if (HouseInfo[houseid][hAlarm] >= listitem)	return SendErrorMessage(playerid, "Você já instalou um alarme de nível %i.", HouseInfo[houseid][hAlarm]);
	if (GetCash(playerid) < 500 * listitem)		return SendErrorMessage(playerid, "Você não tem $%i.", listitem * 500);

	HouseInfo[houseid][hAlarm] = listitem;
	SQL_SetInteger("houses", "alarm", HouseInfo[houseid][hAlarm], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 500);
	SendSyntaxMessage(playerid, "Você comprou um alarme de nível %i por $%i.", HouseInfo[houseid][hAlarm], listitem * 500);
	return 1;
}

Dialog:HouseUpgradeHeal(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	listitem += 1;
	
	if (HouseInfo[houseid][hHeal] >= listitem)	return SendErrorMessage(playerid, "A casa já tem um kit de primeiros socorros.");
	if (GetCash(playerid) < listitem * 400)		return SendErrorMessage(playerid, "Você não tem $%i.", listitem * 400);

	HouseInfo[houseid][hHeal] = listitem;
	HouseInfo[houseid][hHealAmount] = 3;
	SQL_SetInteger("houses", "heal", HouseInfo[houseid][hHeal], HouseInfo[houseid][hID]);
	SQL_SetInteger("houses", "heal_amount", HouseInfo[houseid][hHealAmount], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 400);
	SendSyntaxMessage(playerid, "Você comprou um kit de primeiros socorros de nível %i por $%i.", HouseInfo[houseid][hHeal], listitem * 400);
	return 1;
}

Dialog:HouseSafeChoose(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::hmenu(playerid, "");
	if (!listitem)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	if (listitem == 1)
	{
		Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Coloque seu dinheiro no cofre", "Digite o valor que você deseja colocar no cofre:\n\t\t(até $25.000.000)", "Selecionar", "Cancelar");
	}
	
	if (listitem == 2)
	{
		static dlgtxt[] = "Digite o valor que você deseja retirar do cofre:\n\t\t(até $%s)";

		new diatxt[sizeof(dlgtxt) + 10];

		format(diatxt, sizeof(diatxt), dlgtxt, FormatNumber(HouseInfo[houseid][hCash]));
		Dialog_Show(playerid, HouseWithDraw, DIALOG_STYLE_INPUT, "Retirar dinheiro do cofre", diatxt, "Selecionar", "Cancelar");
	}
	
	return 1;
}

Dialog:HouseDeposit(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	new amount;
	if (sscanf(inputtext, "i", amount))			 return Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Colocar dinheiro no cofre", "Digite o valor que você deseja colocar no cofre:\n\t\t(até $25.000.000)", "Selecionar", "Cancelar");
	if (amount > GetCash(playerid) || amount < 1) return Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Colocar dinheiro no cofre", "VOCÊ NÃO TEM TANTO DINHEIRO!\n\nDigite o valor que você deseja colocar no cofre:\n\t\t(até $25.000.000)", "Selecionar", "Cancelar");

	GiveCash(playerid, -amount);
	HouseInfo[houseid][hCash] += amount;
	SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
	SendSyntaxMessage(playerid, "Você colocou $%i no cofre. Saldo: $%i.", amount, HouseInfo[houseid][hCash]);
	return 1;
}

Dialog:HouseWithDraw(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	new amount;
	if (sscanf(inputtext, "i", amount))			 		 return Dialog_Show(playerid, HouseWithDraw, DIALOG_STYLE_INPUT, "Retirar dinheiro do cofre", "Digite o valor que você deseja retirar do cofre:\n\t\t(até $25.000.000)", "Selecionar", "Cancelar");
	if (amount > HouseInfo[houseid][hCash] || amount < 1) return Dialog_Show(playerid, HouseWithDraw, DIALOG_STYLE_INPUT, "Retirar dinheiro do cofre", "NO COFRE NÃO HÁ TANTO DINHEIRO!\n\nDigite o valor que você deseja retirar do cofre:\n\t\t(até $25.000.000)", "Selecionar", "Cancelar");

	HouseInfo[houseid][hCash] -= amount;
	SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
	GiveCash(playerid, amount);
	SendSyntaxMessage(playerid, "Você retirou $%i do cofre. Restante: $%i.", amount, HouseInfo[houseid][hCash]);

	return 1;
}


Dialog:SellHouseChoose(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Você não está perto de casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	switch(listitem)
	{
		case 0:
		{
		    Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Casa à venda", "{ffffff}Casa à venda. Digite {C3C3C3}[ID/Parte do Nome] [valor de venda]", "Vender", "Cancelar");
		}
		case 1:
		{
		    if (!IsPlayerOutsideHouse(playerid, houseid))	return SendErrorMessage(playerid, "Você deve estar fora da casa.");
		        
		    static const str_size[] = "{FFFFFF}Você deseja vender sua casa por $%i. {FFFF00}(50% do custo)? ";
			new mes[sizeof(str_size) - 2 + 12];

		 	format(mes, sizeof(mes), str_size, HouseInfo[houseid][hPrice]/2/*+GetFurniturePrice(0, houseid)*/, '%');
			Dialog_Show(playerid, SellHouse, DIALOG_STYLE_MSGBOX, "Casa à venda", mes, "Vender", "Cancelar");
		}
	}

	return 1;
}

Dialog:BuyInterior(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	    
	new houseid = strval(inputtext);

    if (houseid < 0 || houseid >= MAX_HOUSES || !HouseInfo[houseid][hID])
	{
	    Dialog_Show(playerid, BuyInterior, DIALOG_STYLE_INPUT, "{FFFFFF}Pedido de interior", "{FFFFFF}Digite o ID da casa que você quer mudar o interior:\nEsta casa deve pertencer a você!", ">>>", "Fechar");
		return SendErrorMessage(playerid, "Casa não encontrada.");
	}
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])
	{
	    Dialog_Show(playerid, BuyInterior, DIALOG_STYLE_INPUT, "{FFFFFF}Pedido de interior", "{FFFFFF}Digite o ID da casa que você quer mudar o interior:\nEsta casa deve pertencer a você!", ">>>", "Fechar");
		return SendErrorMessage(playerid, "Esta casa não pertence a você.");
	}
	
	SetPlayerBuyInterior(playerid, 1, houseid);
	return 1;
}

Dialog:BuyInteriorAccept(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowMenuForPlayer(buy_interior, playerid);

	new houseid = pTemp[playerid][pInteriorHouse]-1;

    if (houseid < 0 || houseid >= MAX_HOUSES || !HouseInfo[houseid][hID])	return SetPlayerBuyInterior(playerid, 0);
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])
	{
	    SetPlayerBuyInterior(playerid, 0);
		return SendErrorMessage(playerid, "Esta casa não pertence a você.");
	}
	if (GetCash(playerid) < 750)
	{
	    ShowMenuForPlayer(buy_interior, playerid);
	    return SendNotCash(playerid);
	}
	
	GiveCash(playerid, -750);
	SendSyntaxMessage(playerid, "Você mudou o interior de sua casa [ID: %i] por $750.", houseid);
    SetHouseInterior(houseid, pTemp[playerid][pInteriorID]);
	SetPlayerBuyInterior(playerid, 0);
	
	new paycheck;

	for(new i; i < MAX_FURNITURE; i++)
	{
	    if (!HFurn[houseid][i][fID] || !HFurn[houseid][i][fOn]) continue;
	    paycheck += (HFurn[houseid][i][fMarketPrice] - (HFurn[houseid][i][fMarketPrice] & 2))  / 2;
	    HFurn[houseid][i][fID] =
	    HFurn[houseid][i][fOn] = 0;
	    DestroyDynamicObject(HFurn[houseid][i][fObject]);
	}

    HouseInfo[houseid][hFurniture] =
    HouseInfo[houseid][hOutsideFurniture] = 0;
    GiveCash(playerid, paycheck);

   	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `furnitures` WHERE `building` = %i AND `bType` = 0", HouseInfo[houseid][hID]);
	mysql_tquery(dbHandle, query);
	return 1;
}


Dialog:SellHouseTo(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    new houseid;

    if ((houseid = GetNearHouse(playerid)) == -1)
        return SendErrorMessage(playerid, "Você não está perto de uma casa.");
    if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])
        return SendErrorMessage(playerid, "Esta casa não pertence a você.");

    if (houseid < 0 || houseid >= MAX_HOUSES || HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return 1;

    new userid, price;

    if (sscanf(inputtext, "ui", userid, price))
    {
        Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Casa à venda", "{ffffff}Casa à venda. Digite {C3C3C3}[ID/Parte do Nome] [preço]", "Vender", "Cancelar");
        return SendErrorMessage(playerid, "Formato inválido.");
    }
    if (userid == playerid)
    {
        Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Casa à venda", "{ffffff}Casa à venda. Digite {C3C3C3}[ID/Parte do Nome] [preço]", "Vender", "Cancelar");
        return SendErrorMessage(playerid, "Você não pode vender a casa para si mesmo.");
    }
    if (!IsPlayerLogged(userid))
    {
        Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Casa à venda", "{ffffff}Casa à venda. Digite {C3C3C3}[ID/Parte do Nome] [preço]", "Vender", "Cancelar");
        return SendNotConnected(playerid);
    }

    if (!PlayerNearPlayer(3.0, playerid, userid))
    {
        Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Casa à venda", "{ffffff}Casa à venda. Digite {C3C3C3}[ID/Parte do Nome] [preço]", "Vender", "Cancelar");
        return SendNotNearPlayer(playerid);
    }

    if (price < 0 || price > 100000000)
    {
        Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Casa à venda", "{ffffff}Casa à venda. Digite {C3C3C3}[ID/Parte do Nome] [preço]", "Vender", "Cancelar");
        return SendErrorMessage(playerid, "Preço de venda de $0 a $100.000.000.");
    }

    new mes[64];
    format(mes, sizeof(mes), "venderpara %s", inputtext);
    cmd::house(playerid, mes);
    return 1;
}

Dialog:SellHouse(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    new houseid;

	if ((houseid = GetNearOutsideHouse(playerid)) == -1)			return SendErrorMessage(playerid, "Não há uma casa próxima a você.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

    if (PlayerInfo[playerid][pSpawnPoint] == HouseInfo[houseid][hID]) {
		PlayerInfo[playerid][pSpawnType] = 0;
		PlayerInfo[playerid][pSpawnPoint] = 1;
		SendHelpMessage(playerid, "Sua localização de spawn foi automaticamente alterada para a padrão em Palomino Creek. (/config)");
	}

    new money = PlayerInfo[playerid][pCash], cash;

	GiveCash(playerid, HouseInfo[houseid][hPrice]/2/*+GetFurniturePrice(0, houseid)*/);
	PlaySound(playerid, 1052);
	SendSyntaxMessage(playerid, "Você vendeu com sucesso sua casa por $%i.", HouseInfo[houseid][hPrice]/2/*+GetFurniturePrice(0, houseid)*/);

	if (HouseInfo[houseid][hCash] > 0) 
	{
		SendSyntaxMessage(playerid, "Havia dinheiro no cofre após a venda da casa: $%i, que foi depositado com sucesso.", HouseInfo[houseid][hCash]);
		GiveCash(playerid, HouseInfo[houseid][hCash]);
		cash = HouseInfo[houseid][hCash];
	}

    new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(DEPOIS): %i] vendeu a casa [H_MYSQL: %i] por -- $%i + $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), 
	HouseInfo[houseid][hID], HouseInfo[houseid][hPrice]/2, cash);
    insert_log(mes, PROPERTY_LOG, 1, PlayerInfo[playerid][pAccID]);

	Sell_House(houseid);
	return 1;
}


Dialog:ChangeHouseSpawn(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new houseid = strval(inputtext);

	if (houseid < 0 || houseid >= MAX_HOUSES)																													return SendErrorMessage(playerid, "Casa não encontrada.");
	if (!HouseInfo[houseid][hID] || (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID] && HouseInfo[houseid][hID] != PlayerInfo[playerid][pRentKey]))	return SendErrorMessage(playerid, "Você não mora nesta casa.");

    SendSyntaxMessage(playerid, "Agora você vai spawnar em sua casa: %s.", (!HouseInfo[houseid][hApart])?(GetHouseZone(houseid)):(GetApartmentZone(HouseInfo[houseid][hApart])));
	PlayerInfo[playerid][pSpawnType] = 1;
	PlayerInfo[playerid][pSpawnPoint] = HouseInfo[houseid][hID];
	
	SQL_SetInteger("users", "spawntype", PlayerInfo[playerid][pSpawnType], PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "spawnpoint", PlayerInfo[playerid][pSpawnPoint], PlayerInfo[playerid][pID]);
	return 1;
}

Dialog:ChangeHouseInterior(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new index, houseid = GetPVarInt(playerid, "house_change");

    if (houseid < 0 || houseid >= MAX_HOUSES || !HouseInfo[houseid][hID])	return SendErrorMessage(playerid, "Casa não encontrada.");

	for(new i = 0; i < MAX_INTERIORS; i++)
	{
		if (!AdminTeles[i][telesOn] || HouseInfo[houseid][hClass] != AdminTeles[i][telesType]) continue;

		if (index == listitem)
		{
		    index = i;
		    break;
		}
		index++;
	}

	SetHouseInterior(houseid, index);
	SendAdmMessageToPlayer(playerid, "Casa [ID: %i | INT: %s].", houseid, AdminTeles[index][telesMapName]);
	return 1;
}


Dialog:EvictTenant(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new 
		dlgmes[70],
		name[MAX_PLAYER_NAME]
	;

	if (!listitem) Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Novo inquilino", "{FFFFFF}Digite o ID / nome do jogador que você deseja despejar de sua casa", ">>>", "Retornar");
	else 		  listitem--;

	SetPVarInt(playerid, #Temp, listitem);

	format(name, sizeof(name), "%s", GetNameFromBD(pTemp[playerid][pListed][listitem]));		
	format(dlgmes, sizeof(dlgmes), "{FFFFFF}Você realmente deseja despejar %s?", name);
	
	ConfirmDialog(playerid, "Confirmação", dlgmes, "EvictTenantConfirm");
	return 1;
}


Dialog:HouseRequestRent(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");
	
	new userid;
	if (sscanf(inputtext,"u", userid)) 								return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Novo inquilino", "{FFFFFF}Digite o ID / nome do jogador que você deseja acomodar em sua casa", ">>>", "Retornar");
    if (userid == playerid) 			  							return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Novo inquilino", "{FF6347}ID DE JOGADOR INVÁLIDO!\n{FFFFFF}Digite o ID / nome do jogador que você deseja acomodar em sua casa", ">>>", "Retornar");
	if (!IsPlayerLogged(userid))		  							return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Novo inquilino", "{FF6347}ID DE JOGADOR INVÁLIDO!\n{FFFFFF}Digite o ID / nome do jogador que você deseja acomodar em sua casa", ">>>", "Retornar");
    if (!PlayerNearPlayer(3.0, playerid, userid))					return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Novo inquilino", "{FF6347}O JOGADOR DEVE ESTAR PRÓXIMO!\n{FFFFFF}Digite o ID / nome do jogador que você deseja acomodar em sua casa", ">>>", "Retornar");
    if (GetPlayerHouses(userid) || PlayerInfo[userid][pRentKey])	return SendErrorMessage(playerid, "O jogador já comprou / alugou uma casa.");

	SetPVarInt(playerid, #RentUserid, userid);
	Dialog_Show(playerid, HouseRequestRent2, DIALOG_STYLE_INPUT, "Novo inquilino", "{FFFFFF}Digite o custo do aluguel:", ">>>", "Retornar");
	return 1;
}


Dialog:HouseRequestRent2(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;
	
	new price, userid = GetPVarInt(playerid, #RentUserid);
	if (sscanf(inputtext,"i", price)) 	return Dialog_Show(playerid, HouseRequestRent2, DIALOG_STYLE_INPUT, "Novo inquilino", "{FFFFFF}Digite o custo do aluguel:", ">>>", "Retornar");
	if (price < 0 || price > 500) 		return Dialog_Show(playerid, HouseRequestRent2, DIALOG_STYLE_INPUT, "Novo inquilino", "{FF6347}Custo de acomodação de $0 a $500!\n{FFFFFF}Digite o custo do aluguel:", ">>>", "Retornar");
	if (GetCash(userid) < price)		return SendErrorMessage(playerid, "O jogador não tem dinheiro suficiente.");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `users` WHERE `rent_key`=%i", HouseInfo[houseid][hID]);
	mysql_tquery(dbHandle, query, "OfferRentRoom", "iii", playerid, userid, price);

	return 1;
}

Dialog:StartBreakingLock(playerid, response, listitem, inputtext[])
{
	new houseid = BreakingHouse[playerid];
    
	BreakingHouse[playerid] = 0;
	
	if (!response)									  return 1;
	if (!IsPlayerOutsideHouse(playerid, houseid, 3.0)) return SendErrorMessage(playerid, "Você deve estar perto da entrada da casa.");
	if (HouseInfo[houseid][hOwnerID] == 0)   		  return SendErrorMessage(playerid, "Apenas casas compradas podem ser hackeadas.");
	if (HouseInfo[houseid][hLocked] == 0)   			  return SendErrorMessage(playerid, "As portas da casa estão abertas.");
	if (IsPlayerCrouching(playerid) == false)		  return SendErrorMessage(playerid, "Você deve se agachar.");
 
	if (pTemp[playerid][JackingType] == 3) {
		if (!IsPlayerHasItem(playerid, INV_PICKLOCK)) return SendErrorMessage(playerid, "Você deve ter uma chave de destrancamento para pegar a chave do veículo. ");
		
		new htime;
		
		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = 250+HouseInfo[houseid][hLock]*250;
			case 1: htime = 250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.05);
			case 2: htime = 250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.15);
		}

		JackingLock[playerid] = htime;	
		RemovePlayerItem(playerid, INV_PICKLOCK, 1);

	} else if (pTemp[playerid][JackingType] == 4) {
		if (!CheckHanditem(playerid,18634)) return SendErrorMessage(playerid, "Você deve ter um pé-de-cabra em suas mãos.");
		PlaySound(playerid, 1138);
		HouseAlarm(houseid, true);

		new htime;
		
		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = 250+HouseInfo[houseid][hLock]*250;
			case 1: htime = 250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.05);
			case 2: htime = 250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.15);
		}

		JackingLock[playerid] = htime;	

	} else return 1;
	
	BreakingHouse[playerid] = houseid;
	cmd::me(playerid, "tira algo e começa a mexer na fechadura da porta da frente. ");
	return 1;
}
