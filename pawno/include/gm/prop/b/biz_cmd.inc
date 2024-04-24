ALTX:biz("/business","/empresa");
CMD:biz(playerid, params[])
{
	new option[16], secoption[129];
	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
	    SendHelpMessage(playerid, "/empresa [comprar / taxas / nome / info]");
	    SendSyntaxMessage(playerid, "Outros comandos: /menuempresa /mobilia");
	    return 1;
	}

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1)                 bizid = GetNearInsideBiz(playerid);
	if (bizid == -1)                                                  return SendErrorMessage(playerid, "Não há empresa perto de você.");

	if (CompareStrings(option, "info"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])        return SendErrorMessage(playerid, "Esta empresa não pertence a você.");
	    
	    new diatxt[300];
	    format(diatxt, sizeof(diatxt), "{FFFFFF}ID:\t\t\t%i\nNome:\t\t %s\nTipo de empresa:\t\t %s\nStatus:\t\t %s\n", BizInfo[bizid][bizID], BizInfo[bizid][bizName], biz_types[BizInfo[bizid][bizType]], (BizInfo[bizid][bizLocked]) ? ("Fechada") : ("Aberta"));
	    format(diatxt, sizeof(diatxt), "%s Quantidade em caixa:\t\t $%i\nCusto de entrada:\t $%i\n\nCondição do armazém: %i / 400\n", diatxt, BizInfo[bizid][bizCash], BizInfo[bizid][bizEntrance], BizInfo[bizid][bizProduct]);

	    new id = biz_prod_types[BizInfo[bizid][bizType]];

	    format(diatxt, sizeof(diatxt), "%s Pedido: %s por $%i / 1 produto", diatxt, truckcargo_product[id], BizInfo[bizid][bizBuyPrice]);

	    Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Informação da empresa", diatxt, ">>", "");
	    return 1;
	}

	else if (CompareStrings(option, "taxas"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])    return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    new fee;

	    if (sscanf(secoption, "i", fee)) return SendErrorMessage(playerid, "/empresa taxas [preço (0-100)]");
	    if (fee < 0 || fee > 100)        return SendErrorMessage(playerid, "Taxa de entrada de $0 a $100.");

	    BizInfo[bizid][bizEntrance] = fee;
	    SQL_SetInteger("business", "entrance", BizInfo[bizid][bizEntrance], BizInfo[bizid][bizID]);
	    SendSyntaxMessage(playerid, "Você definiu uma taxa de entrada para a empresa [ID: %d] $%i.", bizid, fee);
	    return 1;
	}

	else if (CompareStrings(option, "texto"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    new name[129];
	    
	    if (sscanf(secoption, "s[128]", name))
	    {
	        SendHelpMessage(playerid, "/empresa texto [texto]");
	        SendSyntaxMessage(playerid, "Para apagar o texto na entrada, digite {FFFF00}/empresa texto apagar"); 
	        SendSyntaxMessage(playerid, "Cores HTML podem ser usadas no texto. Exemplo de cor: ~FFFF00~ - amarelo.");
	        SendSyntaxMessage(playerid, "Texto de exemplo \"~FFFF00~Olá\" vai mostrar este texto - \"{FFFF00}Olá\""); //verificação
	        return 1;
	    }
	    if (CompareStrings(name, "apagar"))
	    {
	        
	        return 1;
	    }

	    if (strlen(name) < 2 || strlen(name) > 127) return SendErrorMessage(playerid, "O texto da empresa pode conter no máximo 127 caracteres.");
	    if (strfind(name, "\n", true) != -1)       return SendErrorMessage(playerid, "Você não pode alterar o número de linhas no nome da empresa.");

	    format(BizInfo[bizid][bizMes], 128, "%s", name);
	    place_color(BizInfo[bizid][bizMes]);
	    SQL_SetString("business", "entermsg", BizInfo[bizid][bizMes], BizInfo[bizid][bizID]);
	    SendSyntaxMessage(playerid, "Você definiu o texto ao entrar na empresa: %s", BizInfo[bizid][bizMes]);
	    return 1;
	}


	else if (CompareStrings(option, "nome"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])    return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    new name[32];

	    if (sscanf(secoption, "s[32]", name))         return SendHelpMessage(playerid, "/empresa nome [nome]");
	    if (strlen(name) > 30)                        return SendErrorMessage(playerid, "O nome da empresa pode ter até 30 caracteres.");
	    if (strfind(name, "~n~", true) != -1)        return SendErrorMessage(playerid, "Você não pode alterar o número de linhas no nome da empresa.");
	    if (CheckTextDrawString(name))                return SendErrorMessage(playerid, "O nome da empresa não deve conter o caractere '~'.");

	    format(BizInfo[bizid][bizName], 32, "%s", name);
	    SQL_SetString("business", "name", BizInfo[bizid][bizName], BizInfo[bizid][bizID]);
	    SendSyntaxMessage(playerid, "Você definiu o nome da empresa: %s", BizInfo[bizid][bizName]);
	    return 1;
	}

	else if (CompareStrings(option, "depositar"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    new amount;

	    if (sscanf(secoption, "d", amount))
	    {
	        SendHelpMessage(playerid, "/empresa depositar [quantia]");
	        SendSyntaxMessage(playerid, "No momento no caixa $%i", BizInfo[bizid][bizCash]);
	        return 1;
	    }
	    if (GetCash(playerid) < amount || amount < 1)        return SendNotCash(playerid);
	    if (BizInfo[bizid][bizCash] + amount > 25000000)    return SendErrorMessage(playerid, "No caixa, não mais que $25.000.000.");

	    GiveCash(playerid, -amount);
	    GiveBizCash(bizid, amount);
	    SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
	    SendSyntaxMessage(playerid, "Você depositou no caixa $%i.", amount);
	    return 1;
	}

	else if (CompareStrings(option, "sacar"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Esta empresa não pertence a você.");
	    
	    new amount;

	    if (sscanf(secoption, "d", amount))
	    {
	        SendHelpMessage(playerid, "/empresa sacar [quantia]");
	        SendSyntaxMessage(playerid, "No momento no caixa $%i.", BizInfo[bizid][bizCash]);
	        return 1;
	    }
	    if (BizInfo[bizid][bizCash] < amount)    return SendErrorMessage(playerid, "Não há dinheiro suficiente no caixa.");
	    if (amount < 1 || amount > 25000000)    return SendErrorMessage(playerid, "Você pode sacar pelo menos $1 e no máximo $25.000.000.");

	    GiveCash(playerid, amount);
	    GiveBizCash(playerid, -amount);
	    SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
	    SendSyntaxMessage(playerid, "Você retirou do caixa $%i.", amount);
	    return 1;
	}

	else if (CompareStrings(option, "comprar"))
	{
	    if (GetPlayerBizes(playerid) >= GetMaxPlayerBiz(playerid))         return SendErrorMessage(playerid, "Você não pode comprar mais empresas.");
	    if (BizInfo[bizid][bizOwnerID] != 0)                               return SendErrorMessage(playerid, "Esta empresa não está à venda.");
	    if (BizInfo[bizid][bizFreeze] != 0)                                return SendErrorMessage(playerid, "Esta empresa não pode ser comprada.");
	    if (PlayerInfo[playerid][pLevel] < BizInfo[bizid][bizLevelbuy])    return SendErrorMessage(playerid, "Para comprar esta empresa você precisa de nível %i.", BizInfo[bizid][bizLevelbuy]);
	    if (GetCash(playerid) < BizInfo[bizid][bizPrice])                  return SendNotCash(playerid);

	    BizInfo[bizid][bizOwnerID] = PlayerInfo[playerid][pID];
	    BizInfo[bizid][bizLocked] = 0;
	    BizInfo[bizid][bizProduct] = 150;
	    BizInfo[bizid][bizBuyPrice] = 50;
	    format(BizInfo[bizid][bizOwner], MAX_PLAYER_NAME, "%s", GetNameEx(playerid));
	    GiveCash(playerid, -BizInfo[bizid][bizPrice]);
	    Save_Business(bizid);
	    SendSyntaxMessage(playerid, "Você adquiriu uma empresa %s por $%i.", BizInfo[bizid][bizName], BizInfo[bizid][bizPrice]);

	    for(new i = 0; i < MAX_GARAGES; i++)
	    {
	        if (!GarageInfo[i][garageID] || GarageInfo[i][garageBiz] != BizInfo[bizid][bizID]) continue;
	        
	        GarageInfo[i][garageOwner] = PlayerInfo[playerid][pID];
	        SQL_SetInteger("garage", "owner", GarageInfo[i][garageOwner], GarageInfo[i][garageID]);
	        SendSyntaxMessage(playerid, "Junto com a empresa você adquiriu uma garagem [ID: %i].", i);
	    }

	    new mes[256];
	    format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro (ANTES): %i | Dinheiro (DEPOIS): %i] comprou uma empresa [B_MYSQL: %i] por -- $%i.", 
	    GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), PlayerInfo[playerid][pCash] + BizInfo[bizid][bizPrice], GetCash(playerid), BizInfo[bizid][bizID], BizInfo[bizid][bizPrice]);
	    insert_log(mes, PROPERTY_LOG, 4, PlayerInfo[playerid][pAccID]);
	    return 1;
	}


	else if (CompareStrings(option, "vender"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    static const str_size[] = "{FFFFFF}Você quer vender sua empresa por $%i?";
	    new mes[sizeof(str_size) - 2 + 12];

	    format(mes, sizeof(mes), str_size, BizInfo[bizid][bizPrice] / 2);
	    Dialog_Show(playerid, SellBiz, DIALOG_STYLE_MSGBOX, "{FFFFFF}Venda da Empresa", mes, "Vender", "Cancelar");
	    return 1;
	}

	else if (CompareStrings(option, "comprarprod"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    new price, id = biz_prod_types[BizInfo[bizid][bizType]];

	    if (id == -1)						return SendErrorMessage(playerid, "Sua empresa não precisa de um produto.");
	    if (sscanf(secoption, "i", price))	return SendHelpMessage(playerid, "/empresa comprarprod [preço]");
	    if (price > 50 && price < 1000) 		return SendErrorMessage(playerid, "Preço de compra de $50 a $1000.");

	    BizInfo[bizid][bizBuyPrice] = price;
	    SQL_SetInteger("business", "buy_price", price, BizInfo[bizid][bizID]);
	    SendSyntaxMessage(playerid, "Agora você está comprando %s por $%i por unidade.", truckcargo_product[id], price);
	    return 1;
	}

	else if (CompareStrings(option, "upgrade"))
	{
	    new opt[16];

	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");
	    if (sscanf(secoption, "s[16]", opt)) 
	    {
	        SendHelpMessage(playerid, "/empresa upgrade [rádio | repararporta]");
	        return 1;
	    }

	    if (CompareStrings(opt, "rádio"))
	    {
	        if (BizInfo[bizid][bizRadio] == 1)				return SendErrorMessage(playerid, "Rádio já está instalado nesta empresa.");
	        if (GetCash(playerid) < UPGRADE_PRICE_RADIO)	return SendErrorMessage(playerid, "O preço do rádio é $%i.", UPGRADE_PRICE_RADIO);

	        BizInfo[bizid][bizRadio] = 1;
	        SQL_SetInteger("business", "radio", BizInfo[bizid][bizRadio], BizInfo[bizid][bizID]);
	        GiveCash(playerid, -UPGRADE_PRICE_RADIO);
	        SendSyntaxMessage(playerid, "Você comprou um rádio para sua empresa.");
	        SendSyntaxMessage(playerid, "Digite /setstation para usar o rádio. ");
	        return 1;
	    }

	    else if (CompareStrings(opt, "repararporta"))
	    {
	        if (BizInfo[bizid][bizDDoor] == 0)			return SendErrorMessage(playerid, "A porta não precisa de reparo.");
	        if (GetCash(playerid) < UPGRADE_PRICE_DOOR)	return SendErrorMessage(playerid, "O preço para reparar a porta é $%i.", UPGRADE_PRICE_DOOR);

	        BizInfo[bizid][bizDDoor] = 0;
	        SQL_SetInteger("business", "ddoor", BizInfo[bizid][bizDDoor], BizInfo[bizid][bizID]);
	        UpdateBusinessDoor(bizid);
	        GiveCash(playerid, -UPGRADE_PRICE_DOOR);
	        SendSyntaxMessage(playerid, "Você consertou a porta por $%i.", UPGRADE_PRICE_DOOR);
	        return 1;
	    }
	}

	else if (CompareStrings(option, "contratar"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    new userid;

	    if (sscanf(secoption, "u", userid))				return SendHelpMessage(playerid, "/empresa contratar [ID/Nome Parcial]");
	    if (userid == playerid)							return SendErrorMessage(playerid, "Você não pode se contratar.");
	    if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	    if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
	    if (pTemp[userid][pHirePlayer] == playerid)		return SendErrorMessage(playerid, "Você já ofereceu ao jogador um emprego.");

	    pTemp[userid][pHirePlayer] = playerid;
	    SetPVarInt(userid, #accept_time, gettime() + 30);
	    SendSyntaxMessage(userid, "%s convidou você para trabalhar na empresa", GetNameWithMask(playerid));
	    SendHelpMessage(userid, "Pressione Y para aceitar a oferta.");
	    SendSyntaxMessage(playerid, "Você convidou %s para trabalhar na sua empresa.", GetNameWithMask(userid));
	    return 1;
	}

	else if (CompareStrings(option, "demitir"))
	{
	    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

	    new userid;

	    if (sscanf(secoption, "u", userid))							return SendHelpMessage(playerid, "/empresa demitir [ID/Nome Parcial]");
	    if (userid == playerid)										return SendErrorMessage(playerid, "Você não pode se demitir do trabalho.");
	    if (!IsPlayerLogged(userid))								return SendNotConnected(playerid);
	    if (PlayerInfo[userid][pWorkOn] != BizInfo[bizid][bizID])	return SendErrorMessage(playerid, "O jogador não trabalha na sua empresa.");

	    PlayerInfo[userid][pWorkOn] = 0;

	    SendSyntaxMessage(userid, "%s te demitiu da empresa.", GetNameWithMask(playerid));
	    SendSyntaxMessage(playerid, "Você demitiu %s da sua empresa.", GetNameWithMask(userid));
	    return 1;
	}


	else if (CompareStrings(option, "precos"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "This business does not belong to you.");

		new biz = BizInfo[bizid][bizType];
		if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT)	
			return SendErrorMessage(playerid, "You cannot change prices in this business.");
		
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
			format(mes, sizeof(mes), "%s{FFFFFF}Edit product price #%i {AFAFAF}[%s, $%i]\n", mes, i+1, name_order[type][i], BizInfo[bizid][bizProductPrice][i]);
		}

		Dialog_Show(playerid, ConfigPrices, DIALOG_STYLE_LIST, "{9ACD32}Price configuration", mes, "Select", "Return");
		return 1;
	}

	else
	{
		SendHelpMessage(playerid, "/biz [options]");
		SendSyntaxMessage(playerid, "info | taxas | texto | nome | contratar | demitir");
		SendSyntaxMessage(playerid, "depositar | sacar | upgrade | comprarprod");
		SendSyntaxMessage(playerid, "comprar | vender | precos");
	}
	return 1;
}

CMD:menuempresa(playerid, params[])
{
    new bizid;

    if ((bizid = GetNearOutsideBiz(playerid)) == -1)                 bizid = GetNearInsideBiz(playerid);
    if (bizid == -1)                                                    return SendErrorMessage(playerid, "Não há nenhuma empresa perto de você.");
    if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID])        return SendErrorMessage(playerid, "Esta empresa não pertence a você.");

    static diastr[] = "Informações da empresa\n\
                    Informações do funcionário\n\
                    Caixa - depositar / sacar\n\
                    Configuração de preços\n\
                    Melhorias da empresa\n\
                    Edição de mobília\n\
                    Compra de produtos\n\
                    Alterar nome\n\
                    Alterar mensagem de entrada\n\
                    Vender empresa";

    Dialog_Show(playerid, MenuEmpresa, DIALOG_STYLE_LIST, "Menu da Empresa", diastr, "Selecionar", "Sair");
    return 1;
}


CMD:comida(playerid, params[])
{
	if (isnull(params))
	{
		SendHelpMessage(playerid, "/comida [opção]");
		SCM(playerid, COLOR_LIGHTRED,"/comida pedir {FFFFFF}- abrir o menu com pratos.");
		SCM(playerid, COLOR_LIGHTRED,"/comida colocar {FFFFFF}- colocar o prato na mesa. ");
		SCM(playerid, COLOR_LIGHTRED,"/comida pegar {FFFFFF}- pegar a bandeja com o prato da mesa. ");
		SCM(playerid, COLOR_LIGHTRED,"/comida descartar {FFFFFF}- descartar a bandeja com o prato. ");
		return 1;
	}

	if (CompareStrings(params, "pedir"))
	{
	    new bizid;

		if ((bizid = GetNearInsideBiz(playerid)) == -1)	return SendErrorMessage(playerid, "Você precisa estar dentro da empresa. ");
		if (BizInfo[bizid][bizType] != BIZ_PIZZA && BizInfo[bizid][bizType] != BIZ_CLUCKIN && BizInfo[bizid][bizType] != BIZ_BURGER && BizInfo[bizid][bizType] != BIZ_DONUT)
		{
		    SendErrorMessage(playerid, "Você não está em um restaurante.");
			return 1;
		}

		for(new i; i != 4; i++)
		{
		    TextDrawShowForPlayer(playerid, TD_restaurant[i]);
		}

		new mes[128], type = BizInfo[bizid][bizType] - BIZ_PIZZA;

		for(new i; i != max_product[type]; i++)
		{
		    format(mes, sizeof(mes), "%s~n~~r~Saude: +%i~n~~r~Reduz fome~n~~p~Preco: $%i~n~", name_order[type][i], 30 + (i * 10), BizInfo[bizid][bizProductPrice][i]);
			PlayerTextDrawSetString(playerid, TD_restaurant_prod[playerid][i], mes);
			PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i]);
            PlayerTextDrawSetPreviewModel(playerid, TD_restaurant_prod[playerid][i + 4], model_order[type][i]);
            PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i + 4]);
		}

		SelectTextDraw(playerid, 0x00cb77FF);
		SetPVarInt(playerid, #restaurant, 1);
		return 1;
	}

	else if (CompareStrings(params, "colocar"))
	{
        if (!pTemp[playerid][pFoodStand]) return SendErrorMessage(playerid, "Você não tem uma bandeja em suas mãos.");

		new dropid = -1;
        for(new i; i != MAX_FOOD_DROPS; i++)
        {
            if (FoodDrop[i][foodDropped] != 0)	continue;

			dropid = i;
			break;
		}

		if (dropid == -1) return SendErrorMessage(playerid, "O número máximo de bandejas de comida foi alcançado.");

        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
        GetXYInFrontOfPlayer(playerid, FoodDrop[dropid][foodPosX], FoodDrop[dropid][foodPosY], FoodDrop[dropid][foodPosZ], 0.5);
		FoodDrop[dropid][foodPosZ] -= 0.1;
		FoodDrop[dropid][foodInt] = GetInterior(playerid);
		FoodDrop[dropid][foodVW] = GetWorld(playerid);
		FoodDrop[dropid][foodModel] = pTemp[playerid][pFoodStand];
  		FoodDrop[dropid][foodObject] = CreateDynamicObject(pTemp[playerid][pFoodStand], FoodDrop[dropid][foodPosX], FoodDrop[dropid][foodPosY], FoodDrop[dropid][foodPosZ], -25.300008, 23.200012, -16.699985, GetWorld(playerid), GetInterior(playerid));
		EditDynamicObject(playerid, FoodDrop[dropid][foodObject]);
		SetPVarInt(playerid, #edit_food, dropid+1);
        FoodDrop[dropid][foodDropped] = 1;
        FoodDrop[dropid][foodTime] = 15;
		OnPlayAnim(playerid,"CASINO","cards_win",4.1,0,0,0,0,0);
		pTemp[playerid][pFoodStand] = 0;
		SendSyntaxMessage(playerid, "Você está editando a posição da bandeja. Pressione 'Espaço' para mover a câmera.");
		return 1;
	}

	else if (CompareStrings(params, "pegar"))
	{
	    if (pTemp[playerid][pFoodStand] != 0 || IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

		new dropid = DropFood_Nearest(playerid);

		if (dropid == -1) return SendErrorMessage(playerid, "Não há nenhuma bandeja próxima a você.");

		OnPlayAnim(playerid,"CARRY","crry_prtial",4.1,1,1,1,1,1);
		SetPlayerAttachedObject(playerid, ATTACH_HANDS, FoodDrop[dropid][foodModel], 5, 0.091998, 0.021999, 0.409, -104.9, 27.6, 0, 1, 1, 1);
        pTemp[playerid][pFoodStand] = FoodDrop[dropid][foodModel];
        FoodDrop[dropid][foodModel] = 0;
        FoodDrop[dropid][foodDropped] = 0;
        if (IsValidDynamicObject(FoodDrop[dropid][foodObject])) DestroyDynamicObject(FoodDrop[dropid][foodObject]);

		return 1;
	}

	else if (CompareStrings(params, "descartar"))
	{
		if (pTemp[playerid][pFoodStand] == 0) return SendErrorMessage(playerid, "Você não tem uma bandeja em suas mãos.");

		OnPlayAnim(playerid,"CASINO","cards_win",4.1,0,0,0,0,0);
		RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
		pTemp[playerid][pFoodStand] = 0;
		return 1;
	}

	else
	{
		SendHelpMessage(playerid, "/comida [opção]");
		SCM(playerid, COLOR_LIGHTRED,"/comida pedir {FFFFFF}- abrir o menu com pratos.");
		SCM(playerid, COLOR_LIGHTRED,"/comida colocar {FFFFFF}- colocar o prato na mesa. ");
		SCM(playerid, COLOR_LIGHTRED,"/comida pegar {FFFFFF}- pegar a bandeja com o prato da mesa. ");
		SCM(playerid, COLOR_LIGHTRED,"/comida descartar {FFFFFF}- descartar a bandeja com o prato. ");
	}
	return 1;
}
