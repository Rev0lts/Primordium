Dialog:BuyGunMenu(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new bizid = -1;

	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO)	return 1;
	
	if (listitem == 0) { // armas
		static const str[] = "Arma\tPreço\n\
							M1911\t{33CC33}$2000\n\
							Desert Eagle\t{33CC33}$3900\n\
							Espingarda\t{33CC33}$4700\n\
							Uzi\t{33CC33}$400\n\
							MP5\t{33CC33}$4000\n\
							Tec-9\t{33CC33}$4000\n\
							Rifle de Caça\t{33CC33}$5800\n\
							Faca\t{33CC33}$200\n\
							Katana\t{33CC33}$1000";

		new dlg[sizeof(str)+12] = "";
		format(dlg, sizeof(dlg), str);

		Dialog_Show(playerid, BuyGun, DIALOG_STYLE_TABLIST_HEADERS, "Seleção de Arma", dlg, "Comprar", "Voltar");
		
	} else if (listitem == 1) {	// munições
		static const str[] = "Arma\tCartucho da Arma\tQuantidade\tPreço\n\
							M1911\t.45 ACP\t7 cartucho(s)\t{33CC33}$35\n\
							{000000}.	\t.45 ACP .ext\t14 cartucho(s)\t{33CC33}$80\n\
							Desert Eagle\t.50AE\t7 cartucho(s)\t{33CC33}$60\n\
							{000000}.	\t.50AE .ext\t14 cartucho(s)\t{33CC33}$130\n\
							Espingarda\t12-G BIRDSHOT\t6 cartucho(s)\t{33CC33}$300\n\
							{000000}.	\t12-G BUCKSHOT\t6 cartucho(s)\t{33CC33}$200\n\
							{000000}.	\t12-G SLUG\t6 cartucho(s)\t{33CC33}$150\n\
							Tec-9 / MP5 / Uzi\t9x19mm\t30 cartucho(s)\t{33CC33}$200\n\
							{000000}.	\t9x19mm .ext\t60 cartucho(s)\t{33CC33}$450\n\
							Rifle de Caça\t7.62x51mm\t4 cartucho(s)\t{33CC33}$400";

		new dlg[sizeof(str)];
		format(dlg, sizeof(dlg), str);

		Dialog_Show(playerid, BuyAmmo, DIALOG_STYLE_TABLIST_HEADERS, "Seleção de Cartucho", dlg, "Comprar", "Voltar");

	} else if (listitem == 2) {
		static const str[] = "Produto\tPreço\n\
							Mira ótica\t{33CC33}$2700\n\
							Silenciador\t{33CC33}$1000\n\
							Colete de corpo leve {c7c7c7}(60%)\t{33CC33}$1000\n\
							Colete de corpo padrão {c7c7c7}(80%)\t{33CC33}$1500\n\
							Colete de corpo pesado {c7c7c7}(100%)\t{33CC33}$2000";

		new dlg[sizeof(str)+12];
		format(dlg, sizeof(dlg), str);
		
		Dialog_Show(playerid, BuyAdditional, DIALOG_STYLE_TABLIST_HEADERS, "Adicionais", dlg, "Comprar", "Voltar");

	} else {
		cmd::ajudamunicao(playerid, "");
	}

	return 1;
}


Dialog:BuyGun(playerid, response, listitem, inputtext[])
{
	if (!response)	return BuyGunMenu(playerid);

	new bizid = -1;

	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO)	return 1;
	if (!CheckInv(playerid)) return SendErrorMessage(playerid, "Não há espaço suficiente no inventário.");
	        
	new gun_list[] = {WEAPON_COLT45, WEAPON_DEAGLE, WEAPON_SHOTGUN, WEAPON_UZI, WEAPON_MP5, WEAPON_TEC9, WEAPON_RIFLE, WEAPON_KNIFE, WEAPON_KATANA},
		prc_list[] = {2000, 3900, 4700, 4000, 4000, 4000, 5800, 200, 1000};

	if (GetCash(playerid) < prc_list[listitem])							return SendErrorMessage(playerid, "Você precisa de $%i para comprar.", prc_list[listitem]);
	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	return SendErrorMessage(playerid, "Não há munição suficiente à venda. ");
    	
	switch(listitem) {
		case 0..6: {
			if (PlayerInfo[playerid][pWepLic] != 1)	 return SendErrorMessage(playerid, "Você não tem licença de armas.");
			if (PlayerInfo[playerid][pWepCount] != 0) return SendErrorMessage(playerid, "Você pode fazer a próxima compra de armas em %i dias de pagamento.", PlayerInfo[playerid][pWepCount]);
			
			PlayerInfo[playerid][pWepCount] = 6;
			SQL_SetInteger("users", "wep_count", 6, PlayerInfo[playerid][pID]);
			SendHelpMessage(playerid, "Você pode fazer a próxima compra de armas em %i dias de pagamento.", PlayerInfo[playerid][pWepCount]);
		}
	}

	new money = GetCash(playerid),
		weapon = gun_list[listitem],
		ammotype = GetDefaultAmmoType(weapon),
		ammo = CheckAmmo(ammotype);
		
	if (BizInfo[bizid][bizOwnerID] != 0)
	{
		BizInfo[bizid][bizProduct]--;
		GiveBizCash(bizid, prc_list[listitem] / 100 * 80);
	}

	GiveCash(playerid, -prc_list[listitem]);
	GiveInvItem(playerid, weapon, ammo, ammotype, PlayerInfo[playerid][pPlayerSerial]);

	SendSyntaxMessage(playerid, "Você comprou %s por $%i.", inventory_items[weapon][itemName], prc_list[listitem]);

	new query[128];
	format(query, sizeof(query), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(DEPOIS): %i] comprou (%s) por -- $%i", 
	GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), inventory_items[weapon][itemName], prc_list[listitem]);
	insert_log(query, WEAPON_LOG, 1, PlayerInfo[playerid][pAccID]);

	BuyGunMenu(playerid);
	return 1;
}

Dialog:BuyAmmo(playerid, response, listitem, inputtext[])
{
	if (!response) return BuyGunMenu(playerid);

    new bizid = -1;
	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO) 	return 1;
	
	new gun_list[][20] = {"M1911","M1911","Desert Eagle","Desert Eagle","Shotgun","Shotgun","Shotgun","Tec-9 / MP5 / Uzi", "Tec-9 / MP5 / Uzi","Country Rifle"},
		ammo_list[] = {82, 83, 84, 85, 86, 87, 88, 90, 91, 96},
		ammo_pr[] = {35, 80, 60, 130, 300, 200, 150, 200, 450, 400};

	if (!CheckInv(playerid)) 					return SendErrorMessage(playerid, "Não há espaço suficiente no seu inventário. ");
	if (GetCash(playerid) < ammo_pr[listitem])	return SendErrorMessage(playerid, "Você precisa de $%i para comprar.", ammo_pr[listitem]);
	if (!PlayerInfo[playerid][pWepLic])			return SendErrorMessage(playerid, "Você não tem licença de armas.");
    
	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	
		return SendErrorMessage(playerid, "Não há munição suficiente à venda.");

	new money = GetCash(playerid);
	if (BizInfo[bizid][bizOwnerID] != 0)
	{
		BizInfo[bizid][bizProduct]--;
		GiveBizCash(bizid, ammo_pr[listitem]);
	}

	GiveCash(playerid, -ammo_pr[listitem]);
	GiveInvItem(playerid, ammo_list[listitem], CheckAmmo(ammo_list[listitem])); 

	SendSyntaxMessage(playerid, "Você comprou um carregador de arma %s [%i munições] para %s por $%i.", inventory_items[ammo_list[listitem]][itemName], CheckAmmo(ammo_list[listitem]), gun_list[listitem], ammo_pr[listitem]);

	new query[128];
	format(query, sizeof(query), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(DEPOIS): %i] comprou (Munições:%s) por -- $%i",GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), ammo_list[listitem], ammo_pr[listitem]);
	insert_log(query, WEAPON_LOG, 2, PlayerInfo[playerid][pAccID]);

	BuyGunMenu(playerid);
	return 1;
}
Dialog:BuyAdditional(playerid, response, listitem, inputtext[])
{
	if (!response) return BuyGunMenu(playerid);

	new bizid = -1;
	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO) 	return 1;

	new add_name[][36] = {"Mira óptica","Silenciador","Colete leve","Colete padrão","Colete pesado"},
		add_list[] = {INV_OPTICS, INV_SILENCER, 0, 0, 0},
		add_pr[] = {2700, 1000, 1000, 1500, 2000};

	if (GetCash(playerid) < add_pr[listitem]) 		return SendErrorMessage(playerid, "Você precisa de $%i para comprar.", add_pr[listitem]);
	if (add_list[listitem] && !CheckInv(playerid)) 	return SendErrorMessage(playerid, "Não há espaço suficiente no seu inventário. ");

	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	
		return SendErrorMessage(playerid, "Não há munição suficiente à venda. ");
	
	new money = GetCash(playerid);
	if (BizInfo[bizid][bizOwnerID] != 0)
	{
		BizInfo[bizid][bizProduct]--;
		GiveBizCash(bizid, add_pr[listitem]/100*90);
	}

	GiveCash(playerid, -add_pr[listitem]);

	if (add_list[listitem] != 0) GiveInvItem(playerid, add_list[listitem], 1); 
	else {
		new Float:armour;
		if (listitem == 2) 		armour = 60.0;
		else if (listitem == 3) 	armour = 80.0;
		else if (listitem == 4) 	armour = 100.0;
		
		SetPlayerArmour(playerid, armour);
	}

	SendSyntaxMessage(playerid,"Você comprou %s por $%i.", add_name[listitem], add_pr[listitem]);

	new query[128];
	format(query, sizeof(query), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(DEPOIS): %i] comprou %s por -- $%i",GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), add_name[listitem], add_pr[listitem]);
	insert_log(query, WEAPON_LOG, 2, PlayerInfo[playerid][pAccID]);

	BuyGunMenu(playerid);
	return 1;
}
Dialog:BusinessMenu(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;
	
	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	switch(listitem)
	{
		case 0: // Informações da empresa
		{
			new diatxt[300];
			format(diatxt, sizeof(diatxt), "{FFFFFF}bdID:\t\t\t%i\nNome:\t\t %s\nTipo de empresa:\t\t %s\nStatus:\t\t %s\n", BizInfo[bizid][bizID], BizInfo[bizid][bizName], biz_types[BizInfo[bizid][bizType]], (BizInfo[bizid][bizLocked]) ? ("Fechada") : ("Aberta"));
			format(diatxt, sizeof(diatxt), "%sQuantia em caixa:\t\t $%i\nCusto de entrada:\t $%i\n\nCondição do armazém: %i / 400\n", diatxt, BizInfo[bizid][bizCash], BizInfo[bizid][bizEntrance], BizInfo[bizid][bizProduct]);

			new id = biz_prod_types[BizInfo[bizid][bizType]];

			format(diatxt, sizeof(diatxt), "%sPedido: %s por $%i / 1 produto", diatxt, truckcargo_product[id], BizInfo[bizid][bizBuyPrice]);

			Dialog_Show(playerid, BusinessMenuInfo, DIALOG_STYLE_MSGBOX, "Informações da empresa", diatxt, "Voltar", "");
			return 1;
		}
		case 1: // Informações do funcionário
		{
			static dlgtxt[] = "Nome\tCargo\n\
					{FF6347}>{FFFFFF} Contratar um novo funcionário\n";

			new diatxt[sizeof(dlgtxt) + 10*(24+32)],
				count;

			format(diatxt, sizeof(diatxt), "%s", dlgtxt);
			for(new i = 0; i < MAX_WORK_SPACE; i++)
			{
				if (!BizWorkers[bizid][i][bizJobOn]) continue;

				count++;
				format(diatxt, sizeof(diatxt), "%s%i. %s\t%s\n", diatxt, i+1, BizWorkers[bizid][i][bizWorker], BizWorkers[bizid][i][bizPosition]);
			}

			if (!count) format(diatxt, sizeof(diatxt), "%s- Você ainda não contratou ninguém..", dlgtxt);			
			
			Dialog_Show(playerid, WorkerInfo, DIALOG_STYLE_TABLIST_HEADERS, "Informações do funcionário", diatxt, "Selecionar", "Voltar");
			return 1;
		}
		case 2: // Caixa - depositar / retirar
		{
			static dlgtxt[] = "No caixa: $%i\n\
								{FF6347}>{FFFFFF} Depositar\n\
								{FF6347}>{FFFFFF} Retirar";

			new diatxt[sizeof(dlgtxt) + 11];		
			format(diatxt, sizeof(diatxt), dlgtxt, BizInfo[bizid][bizCash]);

			Dialog_Show(playerid, TillOperations, DIALOG_STYLE_TABLIST, "Caixa", diatxt, "Selecionar", "Voltar");
			return 1;
		}
		case 3: // Configuração de preços
		{
			new diatxt[128];
			
			if (BizInfo[bizid][bizType] == BIZ_GAS_STATION)	format(diatxt, sizeof(diatxt), "{FFFFFF}Preço por litro: $%i", BizInfo[bizid][bizEntrance]);
			else 											format(diatxt, sizeof(diatxt), "{FFFFFF}Custo de entrada: $%i", BizInfo[bizid][bizEntrance]);

			new biz = BizInfo[bizid][bizType];
			if (biz == BIZ_STORE || 
				biz == BIZ_BAR ||
				biz == BIZ_PHARMACY || 
				biz == BIZ_PIZZA || 
				biz == BIZ_CLUCKIN || 
				biz == BIZ_BURGER || 
				biz == BIZ_DONUT) format(diatxt, sizeof(diatxt), "%s\n> Alterar preços dos produtos", diatxt);
	
			Dialog_Show(playerid, ChooseConfigPrices, DIALOG_STYLE_TABLIST, "Configuração de preços", diatxt, "Selecionar", "Voltar");
			return 1;
		}
		case 4: // Melhorias da empresa
		{
			Dialog_Show(playerid, BizUpgrade, DIALOG_STYLE_TABLIST, "Melhorias da empresa", "Comprar boombox {33CC33}[$100]{FFFFFF}\nConsertar a porta {33CC33}[$1500]{FFFFFF}", "Selecionar", "Voltar");
			return 1;
		}
		case 5: cmd::furniture(playerid, "");
		case 6: // Compra de produtos
		{
			static dlgtxt[] = "Você está comprando produtos (%s) por $%i por unidade.\nDigite um novo preço de compra para os produtos:";

			new diatxt[sizeof(dlgtxt) + 24 + 3],
				id = biz_prod_types[BizInfo[bizid][bizType]];	

			format(diatxt, sizeof(diatxt), dlgtxt, truckcargo_product[id], BizInfo[bizid][bizBuyPrice]);
			Dialog_Show(playerid, ProductBuying, DIALOG_STYLE_INPUT, "Compra de produtos", diatxt, "Selecionar", "Voltar");
			return 1;
		}
		case 7: // Alterar nome
		{
			static dlgtxt[] = "Nome da sua empresa: %s\nDigite um novo nome:";

			new diatxt[sizeof(dlgtxt) + 24];

			format(diatxt, sizeof(diatxt), dlgtxt, BizInfo[bizid][bizName]);
			Dialog_Show(playerid, ChangeBizName, DIALOG_STYLE_INPUT, "Alteração de nome", diatxt, "Selecionar", "Voltar");
			return 1;
		}
		case 8: // Alterar letreiro na entrada
		{	
			Dialog_Show(playerid, DoorName, DIALOG_STYLE_LIST, "Alteração do letreiro na entrada", "Editar\nRemover", "Selecionar", "Voltar");
			return 1;
		}
		case 9: // Vender a empresa
		{
			Dialog_Show(playerid, SellBiz, DIALOG_STYLE_LIST, "Venda da empresa", "Vender para o jogador\nVender para o estado", "Selecionar", "Voltar");
			return 1;
		}
	}

	return 1;
}
Dialog:SellBiz(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::menuempresa(playerid, "");

    new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 			bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	switch(listitem)
	{
		case 0:
		{
			Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Venda da empresa", "{ffffff}Digite {C3C3C3}[ID/Parte do Nome] [valor da venda]", "Vender", "Cancelar");
		}
		case 1:
		{
			static const str_size[] = "{FFFFFF}Você quer vender sua empresa por $%i?";
			new mes[sizeof(str_size) - 2 + 12];

			format(mes, sizeof(mes), str_size, BizInfo[bizid][bizPrice] / 2);
			Dialog_Show(playerid, SellBizState, DIALOG_STYLE_MSGBOX, "{FFFFFF}Venda da empresa", mes, "Vender", "Cancelar");
		}
	}
	return 1;
}

Dialog:SellBizTo(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

    new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 			bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (bizid < 0 || bizid >= MAX_BIZ || BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return 1;
	    
    new userid, price;

	if (sscanf(inputtext, "ui", userid, price))
	{
	    Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Venda da empresa", "{ffffff}Digite {C3C3C3}[ID/Parte do Nome] [valor da venda]", "Vender", "Cancelar");
	    return SendErrorMessage(playerid, "Formato inválido.");
	}
	
	if (!IsPlayerLogged(userid))
	{
	    Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Venda da empresa", "{ffffff}Digite {C3C3C3}[ID/Parte do Nome] [valor da venda]", "Vender", "Cancelar");
		return SendNotConnected(playerid);
	}
	
	if (!PlayerNearPlayer(3.0, playerid, userid))
	{
	    Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Venda da empresa", "{ffffff}Digite {C3C3C3}[ID/Parte do Nome] [valor da venda]", "Vender", "Cancelar");
		return SendNotNearPlayer(playerid);
	}
	
	if (price < 0 || price > 100000000)
	{
	    Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Venda da empresa", "{ffffff}Digite {C3C3C3}[ID/Parte do Nome] [valor da venda]", "Vender", "Cancelar");
	    return SendErrorMessage(playerid, "Preço de venda de $0 a $100000000.");
	}
	
	pTemp[userid][pBizPrice] = price;
	pTemp[userid][pBizPlayer] = playerid;
	pTemp[userid][pBizID] = bizid;

	SetPVarInt(userid, #accept_time, gettime() + 30);
	SendSyntaxMessage(playerid, "Você ofereceu a %s para comprar sua empresa por $%i.", GetNameEx(userid), price);
	SendSyntaxMessage(userid, "%s quer vender a você uma empresa por $%i. Pressione Y para comprar.", GetNameEx(playerid), price);
	return 1;
}
Dialog:SellBizState(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	PlaySound(playerid, 1052);

	new biz_cost = BizInfo[bizid][bizPrice] / 2, money = PlayerInfo[playerid][pCash], cash;

	GiveCash(playerid, biz_cost);
	SendSyntaxMessage(playerid, "Você vendeu com sucesso sua empresa \"%s\" por $%i.",BizInfo[bizid][bizName], biz_cost);

	if (BizInfo[bizid][bizCash] > 0)
	{
		SendSyntaxMessage(playerid,"Após a venda da empresa, restaram $%i, que foram retirados com sucesso.", BizInfo[bizid][bizCash]);
		GiveCash(playerid, BizInfo[bizid][bizCash]);
		cash = BizInfo[bizid][bizCash];
	}

    new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro(ANTES): %i | Dinheiro(DEPOIS): %i] vendeu a empresa [B_MYSQL: %i] por -- $%i + $%i",
	GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), BizInfo[bizid][bizID], biz_cost, cash);
    insert_log(mes, PROPERTY_LOG, 2, PlayerInfo[playerid][pAccID]);

	RemoveAllWorkers(bizid);
    Sell_Business(bizid);

	return 1;
}

Dialog:BizUpgrade(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (listitem == 0)
	{
		if (BizInfo[bizid][bizRadio] == 1)				return SendErrorMessage(playerid, "Rádio já está instalado nesta empresa.");
		if (GetCash(playerid) < UPGRADE_PRICE_RADIO)	return SendErrorMessage(playerid, "O preço do rádio é de $%i.", UPGRADE_PRICE_RADIO);

		BizInfo[bizid][bizRadio] = 1;
		SQL_SetInteger("business", "radio", BizInfo[bizid][bizRadio], BizInfo[bizid][bizID]);
		GiveCash(playerid, -UPGRADE_PRICE_RADIO);
		SendSyntaxMessage(playerid, "Você comprou um rádio para sua empresa. ");
		SendHelpMessage(playerid, "Digite /setstation, para usar o rádio. ");
	}
	else
	{
		if (BizInfo[bizid][bizDDoor] == 0)			return SendErrorMessage(playerid, "A porta não precisa de reparo.");
		if (GetCash(playerid) < UPGRADE_PRICE_DOOR)	return SendErrorMessage(playerid, "O preço para reparar a porta é de $%i.", UPGRADE_PRICE_DOOR);

		BizInfo[bizid][bizDDoor] = 0;
		SQL_SetInteger("business", "ddoor", BizInfo[bizid][bizDDoor], BizInfo[bizid][bizID]);
		UpdateBusinessDoor(bizid);
		GiveCash(playerid, -UPGRADE_PRICE_DOOR);
		SendSyntaxMessage(playerid, "Você consertou a porta com $%i.", UPGRADE_PRICE_DOOR);
	}
	return 1;
}

Dialog:ProductBuying(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	new price = strval(inputtext), 
		id = biz_prod_types[BizInfo[bizid][bizType]];

	if (id == -1)					return SendErrorMessage(playerid, "Sua empresa não precisa de um produto.");
	if (price < 50 && price > 1000)	return SendErrorMessage(playerid, "Preço de compra de $50 a $1000.");

	BizInfo[bizid][bizBuyPrice] = price;
	SQL_SetInteger("business", "buy_price", price, BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Agora você está comprando %s por $%i por unidade.", truckcargo_product[id], price);
	return 1;
}


Dialog:ChangeBizName(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	new name[32];
	
	if (sscanf(inputtext, "s[32]", name)) return 1;
	if (strlen(name) > 30)				 	return SendErrorMessage(playerid, "O nome da empresa pode ter até 30 caracteres.");
	if (strfind(name, "~n~", true) != -1) 	return SendErrorMessage(playerid, "Você não pode alterar o número de linhas no nome da empresa.");
	if (strfind(name, "\n", true) != -1)	return SendErrorMessage(playerid, "Você não pode alterar o número de linhas no nome da empresa.");
	if (strfind(name, "'", true) != -1) 	return SendErrorMessage(playerid, "Não é possível usar o sinal ' no nome da empresa.");
	if (CheckTextDrawString(name))			return SendErrorMessage(playerid, "O nome da empresa não deve conter o caractere '~'.");

	format(BizInfo[bizid][bizName], 32, "%s", name);
	SQL_SetString("business", "name", BizInfo[bizid][bizName], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Você definiu o nome da empresa: %s", BizInfo[bizid][bizName]);

	return 1;
}

Dialog:TillOperations(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");
	if (!listitem)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (listitem == 1)
	{
		Dialog_Show(playerid, TillDeposit, DIALOG_STYLE_INPUT, "Depósito no caixa", "Digite o valor do dinheiro que\nVocê deseja colocar no caixa:\n\t\t(máximo $25.000.000)", "Selecionar", "Cancelar");
	}
	
	if (listitem == 2)
	{
		static dlgtxt[] = "Digite o valor do dinheiro que\nVocê deseja retirar do caixa:\n\t\t(máximo $%s)";

		new diatxt[sizeof(dlgtxt) + 10];

		format(diatxt, sizeof(diatxt), dlgtxt, FormatNumber(BizInfo[bizid][bizCash]));
		Dialog_Show(playerid, TillWithDraw, DIALOG_STYLE_INPUT, "Retirar dinheiro do caixa", diatxt, "Selecionar", "Cancelar");
	}
	
	return 1;
}

Dialog:TillWithDraw(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	new amount = strval(inputtext);

	if (BizInfo[bizid][bizCash] < amount) 	return SendErrorMessage(playerid, "Não há dinheiro suficiente no caixa.");
	if (amount < 1 || amount > 25000000)	return SendErrorMessage(playerid, "Você pode retirar pelo menos $1 e não mais de $25.000.000.");

	GiveCash(playerid, amount);
	GiveBizCash(playerid, -amount);
	SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Você retirou do caixa $%i.", amount);
	
	return 1;
}


Dialog:TillDeposit(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 			bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	new amount = strval(inputtext);

	if (GetCash(playerid) < amount || amount < 1)	return SendNotCash(playerid);
	if (BizInfo[bizid][bizCash] + amount > 25000000)	return SendErrorMessage(playerid, "No caixa, não pode haver mais de $25.000.000.");

	GiveCash(playerid, -amount);
	GiveBizCash(bizid, amount);
	SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Você depositou no caixa $%i.", amount);
	
	return 1;
}

Dialog:DoorName(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (listitem == 0)
	{
		static dlgtxt[] = "%s\n\
						{FFFFFF}Você pode usar cores HTML no texto. Exemplo de cor: ~FFFF00~ - amarelo.\n\
						Texto de exemplo: \"~FFFF00~Olá\" exibirá este texto - \"{FFFF00}Olá{FFFFFF}\" \n\n\
						Digite o texto abaixo:";

		new diatxt[sizeof(dlgtxt) + 24];

		format(diatxt, sizeof(diatxt), dlgtxt, BizInfo[bizid][bizMes]);
		Dialog_Show(playerid, ChangeDoorName, DIALOG_STYLE_INPUT, "Alterar a inscrição na entrada", diatxt, "Selecionar", "Cancelar");
	}
	else 
	{
		format(BizInfo[bizid][bizMes], 128, "!");
		SQL_SetString("business", "entermsg", "!", BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Agora o texto não será exibido ao entrar. ");
	}

	return 1;
}


Dialog:ChangeDoorName(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 			bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há uma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (strlen(inputtext) < 2 || strlen(inputtext) > 127) return SendErrorMessage(playerid, "O texto da empresa pode conter no máximo 127 caracteres. ");
	if (strfind(inputtext, "\n", true) != -1) 			 return SendErrorMessage(playerid, "Você não pode alterar o número de linhas no nome da empresa.");

	format(BizInfo[bizid][bizMes], 128, "%s", inputtext);
	place_color(BizInfo[bizid][bizMes]);
	SQL_SetString("business", "entermsg", BizInfo[bizid][bizMes], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Você definiu o texto ao entrar na empresa: %s", BizInfo[bizid][bizMes]);

	return 1;
}
Dialog:WorkerInfo(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (listitem == 0) return Dialog_Show(playerid, WorkerInvite, DIALOG_STYLE_INPUT, "Contratar um funcionário", "{FFFFFF}Digite o ID/Nome do jogador que deseja contratar:", ">>>", "Retornar");
	
	listitem -= 1;
	SetPVarInt(playerid, #WorkerID, listitem);

	static dlgtxt[] = "{FFFFFF}NOME:\t\t %s\n\
					CARGO:\t %s\n\
					{FF6347}>{FFFFFF} Mudar cargo\n\
					{FF6347}>{FFFFFF} Demitir";
	
	new diatxt[sizeof(dlgtxt) + 24 + 32];

	format(diatxt, sizeof(diatxt), dlgtxt, BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);

	Dialog_Show(playerid, WorkerEdit, DIALOG_STYLE_TABLIST_HEADERS, "Informação do Funcionário", diatxt, "Selecionar", "Retornar");
	return 1;
}

Dialog:WorkerEdit(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (listitem == 1) return Dialog_Show(playerid, WorkerEditPos, DIALOG_STYLE_INPUT, "Mudar cargo", "{FFFFFF}Digite o novo cargo do funcionário:", "Selecionar", "Cancelar");
	if (listitem == 2)
	{
		listitem = GetPVarInt(playerid, #WorkerID);
		static dlgtxt[] = "{FFFFFF}Você realmente deseja demitir %s (%s)?";
		new diatxt[sizeof(dlgtxt) + 24 + 32];

		format(diatxt, sizeof(diatxt), dlgtxt, BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);
		Dialog_Show(playerid, WorkerFire, DIALOG_STYLE_INPUT, "Demissão de um funcionário", diatxt, "Sim", "Cancelar");
	}
	return 1;
}

Dialog:WorkerInvite(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

	new bizid, userid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (WorkSpace(bizid) == -1)						return SendErrorMessage(playerid, "Você não tem vagas. ");

	if (sscanf(inputtext, "u", userid))				return 1;
	if (userid == playerid)							return SendErrorMessage(playerid, "Você não pode contratar a si mesmo. ");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (pTemp[userid][pHirePlayer] == playerid)		return SendErrorMessage(playerid, "Você já ofereceu ao jogador um emprego. ");

	pTemp[userid][pHirePlayer] = playerid;
	SetPVarInt(userid, #accept_time, gettime() + 30);

	SendSyntaxMessage(userid, "%s convidou você para trabalhar na empresa", GetNameWithMask(playerid));
	SendHelpMessage(userid, "Pressione Y para aceitar a oferta.");
	SendSyntaxMessage(playerid, "Você convidou %s para trabalhar no sua empresa.", GetNameWithMask(userid));

	return 1;
}
Dialog:WorkerEditPos(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;
	
	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	listitem = GetPVarInt(playerid, #WorkerID);

	format(BizWorkers[bizid][listitem][bizPosition], 24, "%s", inputtext);

	SQL_SetString("users", "position", BizWorkers[bizid][listitem][bizPosition], BizWorkers[bizid][listitem][bizJobOn]);

	SendSyntaxMessage(playerid, "Você mudou a posição de %s para %s.", BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);

	for(new i; i < GetPlayerPoolSize(); i++)
	{
		if (PlayerInfo[i][pID] == BizWorkers[bizid][listitem][bizJobOn])
		{
			SendSyntaxMessage(i, "%s mudou sua posição para %s.", GetNameEx(playerid), BizWorkers[bizid][listitem][bizPosition]);
			break;
		}
	}
	
	return 1;
}

Dialog:WorkerFire(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;
	
	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 				bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	listitem = GetPVarInt(playerid, #WorkerID);

	SQL_SetString("users", "position", "-", BizWorkers[bizid][listitem][bizJobOn]);
	SQL_SetInteger("users", "workon", 0, BizWorkers[bizid][listitem][bizJobOn]);
	
	for(new i; i < GetPlayerPoolSize(); i++)
	{
		if (PlayerInfo[i][pID] == BizWorkers[bizid][listitem][bizJobOn])
		{
			SendSyntaxMessage(i, "%s demitiu você da empresa.", GetNameEx(playerid));
			PlayerInfo[i][pWorkOn] = 0;
			break;
		}
	}

	SendSyntaxMessage(playerid, "Você demitiu %s (%s) do sua empresa.", BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);

	BizWorkers[bizid][listitem][bizWorker] = EOS;
	BizWorkers[bizid][listitem][bizPosition] = EOS;
	BizWorkers[bizid][listitem][bizJobOn] = 0;
	return 1;
}


Dialog:BusinessMenuInfo(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::menuempresa(playerid, "");
	return 	cmd::menuempresa(playerid, "");
}

Dialog:ChooseConfigPrices(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::menuempresa(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 			bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	if (listitem == 0)
	{
		Dialog_Show(playerid, ConfigEntranceFee, DIALOG_STYLE_INPUT, "Configuração de preço", "Digite o novo valor:", "Selecionar", "Cancelar");
	}
	else 
	{
		new biz = BizInfo[bizid][bizType];
		if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT)	
			return SendErrorMessage(playerid,"Você não pode mudar os preços nEsta empresa.");
		
		new type;
		switch(biz)
		{
			case BIZ_PIZZA:		type = 0;
			case BIZ_CLUCKIN:	type = 1;
			case BIZ_BURGER:	type = 2;
			case BIZ_DONUT:		type = 3;
			case BIZ_STORE:		type = 4;
			case BIZ_BAR:		type = 5;
			case BIZ_PHARMACY:	type = 6;
		}
		
		new mes[LIMIT_MAX_PRODUCTS*128];

		for(new i = 0; i < max_product[type]; i++)
		{
			format(mes, sizeof(mes), "%s{FFFFFF}Alterar preço do produto #%i {AFAFAF}[%s, $%i]\n", mes, i+1, name_order[type][i], BizInfo[bizid][bizProductPrice][i]);
		}

		Dialog_Show(playerid, ConfigPrices, DIALOG_STYLE_LIST, "Configuração de preço", mes, "Selecionar", "Retornar");
		return 1;
	}

	return 1;
}

Dialog:ConfigEntranceFee(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 			bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)												return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você. ");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	new fee = strval(inputtext);

	if (fee < 0 || fee > 100) 
	{
		Dialog_Show(playerid, ConfigEntranceFee, DIALOG_STYLE_INPUT, "Configuração de preço", "Digite o novo custo:", "Selecionar", "Cancelar");
		return SendErrorMessage(playerid, "Custo possível: de $0 a $100.");
	}

	BizInfo[bizid][bizEntrance] = fee;
	SQL_SetInteger("business", "entrance", BizInfo[bizid][bizEntrance], BizInfo[bizid][bizID]);
	
	if (BizInfo[bizid][bizType] == BIZ_GAS_STATION)	SendSyntaxMessage(playerid, "Você definiu o preço do combustível na empresa [ID: %d] - $%i/l.", bizid, fee);
	else 											SendSyntaxMessage(playerid, "Você definiu um preço para entrada na empresa [ID: %d] $%i.", bizid, fee);
	
	return 1;
}


Dialog:ConfigPrices(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

    new bizid;

	if ((bizid = GetNearInsideBiz(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro do sua empresa.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");
	
	new biz = BizInfo[bizid][bizType];
	if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT)	
		return SendErrorMessage(playerid, "Você não pode mudar os preços nEsta empresa.");
	new type;
	switch(biz)
	{
		case BIZ_PIZZA:		type = 0;
		case BIZ_CLUCKIN:	type = 1;
		case BIZ_BURGER:	type = 2;
		case BIZ_DONUT:		type = 3;
		case BIZ_STORE:		type = 4;
		case BIZ_BAR:		type = 5;
		case BIZ_PHARMACY:	type = 6;
	}

	new mes[128];

	format(mes, sizeof(mes), "{FFFFFF}Mudar preço do produto #%i {AFAFAF}[%s, $%i]", listitem+1, name_order[type][listitem], BizInfo[bizid][bizProductPrice][listitem]);
	Dialog_Show(playerid, ConfigPricesEnd, DIALOG_STYLE_INPUT, "Configuração de Preço", mes, "Digitar", "Fechar");
	SetPVarInt(playerid, "ConfigPrices", listitem);
	return 1;
}

Dialog:ConfigPricesEnd(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

    new bizid;

	if ((bizid = GetNearInsideBiz(playerid)) == -1)					return SendErrorMessage(playerid, "Você deve estar dentro do sua empresa.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	new money = strval(inputtext);
	listitem = GetPVarInt(playerid, "ConfigPrices");

	new biz = BizInfo[bizid][bizType];
	if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT)	return SendErrorMessage(playerid,"Você não pode mudar os preços nEsta empresa.");
	new type;
	switch(biz)
	{
		case BIZ_PIZZA:		type = 0;
		case BIZ_CLUCKIN:	type = 1;
		case BIZ_BURGER:	type = 2;
		case BIZ_DONUT:		type = 3;
		case BIZ_STORE:		type = 4;
		case BIZ_BAR:		type = 5;
		case BIZ_PHARMACY:	type = 6;
	}

	if (money < biz_prices[type][listitem]/2 || money > biz_prices[type][listitem]*2)
	{
	    new mes[256];
		format(mes, sizeof(mes), ""EMBED_RED"O valor inserido deve ser pelo menos $%i e não mais $%i!\n{FFFFFF}Mudar preço do produto #%i {AFAFAF}[%s, $%i]", biz_prices[type][listitem]/2, biz_prices[type][listitem]*2, listitem+1, name_order[type][listitem], BizInfo[bizid][bizProductPrice][listitem]);
		Dialog_Show(playerid, ConfigPricesEnd, DIALOG_STYLE_INPUT, "Configuração de Preço", mes, "Digitar", "Fechar");
		return 1;
	}

	BizInfo[bizid][bizProductPrice][listitem] = money;
	SendSyntaxMessage(playerid,"Você mudou o custo de {FF6347}%s {FFFFFF}para {FF6347}$%i{FFFFFF}.", name_order[type][listitem], BizInfo[bizid][bizProductPrice][listitem]);
	
	Save_Business(bizid);
	DeletePVar(playerid,"ConfigPrices");
	return 1;
}
Dialog:StoreDialog(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

    new bizid = GetNearInsideBiz(playerid);

    if (bizid == -1)														return 1;
    if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	return SendErrorMessage(playerid,"Não há produtos suficientes na empresa.");
    if (GetCash(playerid) < BizInfo[bizid][bizProductPrice][listitem])	return SendNotCash(playerid);
    
    if (BizInfo[bizid][bizType] == BIZ_STORE)
    {
        switch(listitem)
        {
            case 0:
            {
                SetPlayerNumber(playerid);
                SendSyntaxMessage(playerid,"Você comprou um chip SIM por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 1:
            {
                if (GiveInvItem(playerid, INV_RADIO, 1) == -1)	return SendErrorMessage(playerid, "O inventário está cheio.");

                SendSyntaxMessage(playerid, "Você comprou um walkie-talkie por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
                SendSyntaxMessage(playerid, "Use /ajudaradio para mais informações. ");
            }
            case 2:
            {
                if (PlayerInfo[playerid][pLevel] < 2)			    return SendErrorMessage(playerid, "Você precisa ser nível 2 para comprar uma máscara.");
                if (GiveInvItem(playerid, INV_MASK, 1) == -1)	    return SendErrorMessage(playerid, "O inventário está cheio.");
                
                SendSyntaxMessage(playerid, "Você comprou uma máscara por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
                SendHelpMessage(playerid, "/mascara para colocá-la. ");
            }
            case 3:
            {
                if (GiveInvItem(playerid, INV_LIGHTER, 1) == -1)	return SendErrorMessage(playerid, "O inventário está cheio.");

                SendSyntaxMessage(playerid, "Você comprou um isqueiro por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 4:
            {
                if (GiveInvItem(playerid, INV_CIGAR, 20) == -1)	    return SendErrorMessage(playerid, "O inventário está cheio.");

                SendSyntaxMessage(playerid, "Você comprou cigarros por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 5:
            {
                if (GiveInvItem(playerid, INV_DICE, 1) == -1)		return SendErrorMessage(playerid, "O inventário está cheio.");
                
                SendSyntaxMessage(playerid, "Você comprou dados por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
                SendHelpMessage(playerid, "/dado para jogar os dados. ");
            }
            case 6:
            {
                if (GiveInvItem(playerid, INV_GASCAN, 1) == -1)	    return SendErrorMessage(playerid, "O inventário está cheio.");

                SendSyntaxMessage(playerid, "Você comprou um galão por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 7:
            {
                if (GiveInvItem(playerid, INV_PEN, 1) == -1)		    return SendErrorMessage(playerid, "O inventário está cheio.");

                SendSyntaxMessage(playerid, "Você comprou uma caneta por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
                SendHelpMessage(playerid, "/nota para escrever em um caderno. ");
            }
            case 8:
            {
                if (GiveInvItem(playerid, INV_CARDS, 1) == -1)	    return SendErrorMessage(playerid, "O inventário está cheio.");

                SendSyntaxMessage(playerid, "Você comprou cartas de baralho por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
                SendHelpMessage(playerid, "/darcartas para iniciar o jogo.");
            }
            case 9:
            {
                if (GiveInvItem(playerid, 43, 200) == -1)		    return SendErrorMessage(playerid, "O inventário está cheio.");
                SendClientMessageEx(playerid, COLOR_WHITE, "Você comprou uma câmera por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 10:
            {
                if (GiveInvItem(playerid, INV_SIGN, 1) == -1)	    return SendErrorMessage(playerid, "O inventário está cheio.");
                SendSyntaxMessage(playerid, "Você comprou um adesivo para veículo por $%i.",  BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 11:
            {
                if (GiveInvItem(playerid, 14, 1) == -1)	return SendErrorMessage(playerid, "O inventário está cheio.");
                SendSyntaxMessage(playerid, "Você comprou flores por $%i.",  BizInfo[bizid][bizProductPrice][listitem]);
            }
        }
    }
    else if (BizInfo[bizid][bizType] == BIZ_BAR)
    {
        new name_drink[8][13] = {"cerveja", "vodka", "cola", "água", "uísque", "conhaque", "refrigerante", "vinho"};

        if (GiveInvItem(playerid, INV_BEER + listitem, 1) == -1)	return SendErrorMessage(playerid, "O inventário está cheio.");
        SendSyntaxMessage(playerid, "Você comprou %s por $%i.", name_drink[listitem], BizInfo[bizid][bizProductPrice][listitem]);
    }
    else if (BizInfo[bizid][bizType] == BIZ_PHARMACY)
    {
        switch(listitem)
        {
            case 0:
            {
                if (PlayerInfo[playerid][pPrescribe] < gettime())		return SendErrorMessage(playerid, "Você não tem receita.");
                if (PlayerInfo[playerid][pGanjaTime] > gettime())		return SendErrorMessage(playerid, "Você só pode obter maconha medicinal através de: %s.", ConvertSecond(PlayerInfo[playerid][pGanjaTime] - gettime()));
                if (GiveInvItem(playerid, INV_MED_GANJA, 2) == -1)		return SendErrorMessage(playerid, "O inventário está cheio.");

                PlayerInfo[playerid][pGanjaTime] = gettime() + 86400;
                SendSyntaxMessage(playerid, "Você comprou maconha medicinal por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 1:
            {
                if (IsPlayerHasItem(playerid, INV_HEAL))	  return SendErrorMessage(playerid,"Você já tem um kit de primeiros socorros.");
                if (GiveInvItem(playerid, INV_HEAL, 1) == -1) return SendErrorMessage(playerid, "O inventário está cheio.");
                    
                SendSyntaxMessage(playerid, "Você comprou um kit de primeiros socorros por $%i.", BizInfo[bizid][bizProductPrice][listitem]);
            }
            case 2..11: //103..112 
            {
                if (GiveFloatInvItem(playerid, 101+listitem, 100.0) == -1)	return SendErrorMessage(playerid, "O inventário está cheio.");
                SendSyntaxMessage(playerid, "Você comprou '%s' por $%i.", name_order[6][listitem], BizInfo[bizid][bizProductPrice][listitem]);
            }
        }
    }
    else return 1;

    if (BizInfo[bizid][bizOwnerID] != 0)
    {
        BizInfo[bizid][bizProduct]--;
        GiveBizCash(bizid, BizInfo[bizid][bizProductPrice][listitem]);
    }

    GiveCash(playerid, -BizInfo[bizid][bizProductPrice][listitem]);
    return 1;
}

Dialog:ComputerStore(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

    if (IsPlayerPhoneCreated(playerid)) 	Phone_Hide(playerid);
    else 								LoadPhoneTextDraw(playerid);
    
    TextDrawBoxColor(PhDisplay[playerid], 255);
    TextDrawShowForPlayer(playerid, PhDisplay[playerid]);

    TextDrawBoxColor(PhoneTDbox[0][playerid], phone_display_color[listitem]);
    TextDrawShowForPlayer(playerid, PhoneTDbox[0][playerid]);

    for(new i = 1; i < 5; i++)
    {
        if (i < 3) TextDrawColor(PhoneTDbox[i][playerid], phone_model_color[listitem]);
        TextDrawShowForPlayer(playerid, PhoneTDbox[i][playerid]);
        if (i < 4) TextDrawShowForPlayer(playerid, PhArrow[i][playerid]);
    }

    for(new i = 0; i < 2; i++) 
    {
        TextDrawColor(PhBtn[i][playerid], phone_btns_color[listitem]);
        TextDrawBackgroundColor(PhBtn[i][playerid], phone_btns_color[listitem]);
        TextDrawShowForPlayer(playerid, PhBtn[i][playerid]);
    }

    new phone_names[5][10] = {"preto", "azul", "vermelho", "verde", "laranja"};
    new diatxt[64];

    format(diatxt, sizeof(diatxt), "{FFFFFF}Você tem certeza que deseja comprar um celular %s? ", phone_names[listitem]);
    FreezePlayer(playerid);

    SetPVarInt(playerid, #buing_phone, listitem+1);
    ConfirmDialog(playerid, "Comprando um celular", diatxt, "BuyingPhone");
    return 1;
}
this::BuyingPhone(playerid, response)
{
    if (!response)    
    {
        new listitem = PlayerInfo[playerid][pPhoneModel];
        TextDrawBoxColor(PhoneTDbox[0][playerid], phone_display_color[listitem]);
    
        for(new i = 0; i < 5; i++)
        {
            if (i < 3) TextDrawColor(PhoneTDbox[i][playerid], phone_model_color[listitem]);
            TextDrawHideForPlayer(playerid, PhoneTDbox[i][playerid]);
            if (i < 4) TextDrawHideForPlayer(playerid, PhArrow[i][playerid]);
        }

        for(new i = 0; i < 2; i++) 
        {
            TextDrawColor(PhBtn[i][playerid], phone_btns_color[listitem]);
            TextDrawBackgroundColor(PhBtn[i][playerid], phone_btns_color[listitem]);
            TextDrawHideForPlayer(playerid, PhBtn[i][playerid]);
        }

        TextDrawHideForPlayer(playerid, PhDisplay[playerid]);

        SetPVarInt(playerid, #buing_phone, 0);
        cmd::comprar(playerid, "");
        return UnFreezePlayer(playerid);
    }

    new listitem = GetPVarInt(playerid, #buing_phone),
        phone_names[5][10] = {"preto", "azul", "vermelho", "verde", "laranja"};

    if (PlayerInfo[playerid][pPhoneModel] == listitem-1)    return SendErrorMessage(playerid, "Você já tem um celular dessa cor.");
    if (GetCash(playerid) < MOBILEMODELPRICE)                return SendNotCash(playerid);

    GiveCash(playerid, -MOBILEMODELPRICE);

    if (!PlayerInfo[playerid][pNumber])    SetPlayerNumber(playerid);

    PlayerInfo[playerid][pPhoneModel] = listitem-1;

    SendSyntaxMessage(playerid, "Você comprou um celular %s.", phone_names[listitem-1]);
    SendHelpMessage(playerid, "Pressione Y ou digite (/cel)ular para abrir o celular.");

    UnFreezePlayer(playerid);
    SetPVarInt(playerid, #buing_phone, 0);
    
    for(new i = 0; i < 5; i++) {
        TextDrawHideForPlayer(playerid, PhoneTDbox[i][playerid]);
        if (i < 4) TextDrawHideForPlayer(playerid, PhArrow[i][playerid]);
    }

    for(new i = 0; i < 2; i++) TextDrawHideForPlayer(playerid, PhBtn[i][playerid]);
    TextDrawHideForPlayer(playerid, PhDisplay[playerid]);
        
    return 1;
}

Dialog:ChangeBizInterior(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

    new index, bizid = GetPVarInt(playerid, "biz_change");

    if (bizid < 0 || bizid > MAX_BIZ-1 || !BizInfo[bizid][bizID])	return SendErrorMessage(playerid,"Empresa não existe sob este ID.");

    for(new i = 0; i < MAX_INTERIORS; i++)
    {
        if (!AdminTeles[i][telesOn] || BUSSINESS_INT_TYPE != AdminTeles[i][telesType]) continue;

        if (index == listitem)
        {
            index = i;
            break;
        }
        index++;
    }

    SetBizInterior(playerid, bizid, index);
    return 1;
}
