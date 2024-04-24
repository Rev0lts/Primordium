Dialog:FurnitureDialog(playerid, response, listitem, inputtext[])
{
    if (!response) return pTemp[playerid][pFurniture] = -1;
     
    switch(listitem)
    {
        case 0: return ShowFurnitureCategories(playerid);
        case 1: return Dialog_Show(playerid, CategoryEditFurniture, DIALOG_STYLE_LIST, "{FFFFFF}Editando mobiliário", "Editar com o mouse\nLista de mobiliário", "Selecionar", "Retornar");
        case 2:
        {
            if (!GetFurnitures(playerid)) return SendErrorMessage(playerid, "Nenhum mobiliário comprado.");
            Dialog_Show(playerid, HandleSellFurnitures, DIALOG_STYLE_MSGBOX, "{FFFFFF}Venda de todo o mobiliário", "{ffffff}Tem certeza que deseja vender todo o mobiliário?", "Vender", "Cancelar");
        }
    }
    return 1;
}

Dialog:FurnitureCategories(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::furniture(playerid, "");
    if (listitem == 28 && GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) == -1)        return SendErrorMessage(playerid, "Você deve estar fora para comprar este mobiliário. ");
    else if (listitem != 28 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) == -1) return SendErrorMessage(playerid, "Você deve estar dentro para comprar este mobiliário. ");
        
    FurnBuingDialog(playerid, listitem);
    return 1;
}


Dialog:CategoryEditFurniture(playerid, response, listitem, inputtext[])
{
    if (!response)   return cmd::furniture(playerid, "");
    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1))    return 1;

    new furnitures = GetFurnitures(playerid);

    if (!furnitures) return SendErrorMessage(playerid, "Você não tem mobiliário.");

    if (!listitem)
    {
        SelectObject(playerid);
        SendErrorMessage(playerid, "Selecione um dos objetos de mobiliário clicando com o mouse.");
        return 1;
    }

    ShowFurnitureList(playerid);
    return 1;
}


Dialog:FurnitureList(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::furniture(playerid, "");
    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1))	return 1;

	new building = pTemp[playerid][pFurniture],
	    count,
		FName[64],
	    list = pTemp[playerid][pDialogList];

	if (!listitem && list)																		 return ShowFurnitureList(playerid, list-1);
	if ((listitem == MAX_FURNITURE_LIST && !list) || (listitem == MAX_FURNITURE_LIST+1 && list)) return ShowFurnitureList(playerid, list+1);
	if (list)																					 listitem -= 1;
	
	new start_itter = MAX_FURNITURE_LIST * list;
	
	switch(pTemp[playerid][pFurnitureB])
	{
		case 0: {
			for(new i; i < MAX_FURNITURE; i++)
			{
				if (!HFurn[building][i][fOn] || !IsValidDynamicObject(HFurn[building][i][fObject])) continue;
					
				if (count == listitem + start_itter)
				{
					count = i;
					break;
				}
				count++;
			}

			if (HFurn[building][count][fVirtualWorld] != GetWorld(playerid) || HFurn[building][count][fInterior] != GetInterior(playerid)) 
			{
				return SendErrorMessage(playerid, "Este objeto está na rua, saia da casa para editar.");
			}

			SetPVarInt(playerid, "chose_slot", count);
			format(FName, sizeof(FName), "%s", HFurn[building][count][fName]);
		} case 1: {
			for(new i; i < MAX_FURNITURE; i++)
			{
				if (!BFurn[building][i][fOn] || !IsValidDynamicObject(BFurn[building][i][fObject])) continue;
					
				if (count == listitem + start_itter)
				{
					count = i;
					break;
				}
				count++;
			}

			if (BFurn[building][count][fVirtualWorld] != GetWorld(playerid) || BFurn[building][count][fInterior] != GetInterior(playerid)) 
			{
				return SendErrorMessage(playerid, "Este objeto está na rua, saia da casa para editar.");
			}

			SetPVarInt(playerid, "chose_slot", count);
			format(FName, sizeof(FName), "%s", BFurn[building][count][fName]);
		} case 2: {
			for(new i; i < MAX_FURNITURE; i++)
			{
				if (!GFurn[building][i][fOn] || !IsValidDynamicObject(GFurn[building][i][fObject])) continue;
					
				if (count == listitem + start_itter)
				{
					count = i;
					break;
				}
				count++;
			}

			if (GFurn[building][count][fVirtualWorld] != GetWorld(playerid) || GFurn[building][count][fInterior] != GetInterior(playerid)) 
			{
				return SendErrorMessage(playerid, "Este objeto está na rua, saia da casa para editar.");
			}

			SetPVarInt(playerid, "chose_slot", count);
			format(FName, sizeof(FName), "%s", GFurn[building][count][fName]);
		}
	}

 	Dialog_Show(playerid, EditFurniture, DIALOG_STYLE_LIST, FName, "Informacao\nMudar posição\nEditar textura\nVender\nMudar nome\nCriar cópia", "Selecionar", "Retornar");
	return 1;
}

Dialog:HandleEditTexture(playerid, response, listitem, inputtext[])
{
	if (!response) return CallRemoteFunction("ShowEditFurniture", "i", playerid);

    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1))	return 1;

	new building = pTemp[playerid][pFurniture], slot = GetPVarInt(playerid, "chose_slot");

	if (listitem < 5)
	{
		new title[32];

		format(title, 32, "Texture (%i)", listitem+1);
 		Dialog_Show(playerid, HandleEditTexture2, DIALOG_STYLE_LIST, title, "Mudar textura\nRemover textura", "Selecionar", "Retornar");
        SetPVarInt(playerid, "chose_texture", listitem);
		return 1;
	}

	new name[64], furnid;

	switch(pTemp[playerid][pFurnitureB])
	{
		case 0: {
			format(name, sizeof(name), "%s", HFurn[building][GetPVarInt(playerid, "chose_slot")][fName]);
			furnid = HFurn[building][slot][fID];
			for(new i; i < 5; i++)
			{
				HFurn[building][slot][fTexture][i] = -1;
				SetDynamicObjectMaterial(HFurn[building][slot][fObject], i, -1, "none", "none", 0);
			}
		} case 1: { 
			format(name, sizeof(name), "%s", BFurn[building][GetPVarInt(playerid, "chose_slot")][fName]);
			furnid = BFurn[building][slot][fID];
			for(new i; i < 5; i++)
			{
				BFurn[building][slot][fTexture][i] = -1;
				SetDynamicObjectMaterial(BFurn[building][slot][fObject], i, -1, "none", "none", 0);
			}
		} case 2: {
			format(name, sizeof(name), "%s", GFurn[building][GetPVarInt(playerid, "chose_slot")][fName]);
			furnid = GFurn[building][slot][fID];

			for(new i; i < 5; i++)
			{
				GFurn[building][slot][fTexture][i] = -1;
				SetDynamicObjectMaterial(GFurn[building][slot][fObject], i, -1, "none", "none", 0);
			}
		}
	}

 	SendSyntaxMessage(playerid, "Todas as texturas de %s foram removidas.", name);


	new query[128];

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `furnitures` SET `texture_0`=-1,`texture_1`=-1,`texture_2`=-1,`texture_3`=-1,`texture_4`=-1 WHERE `id`=%i",furnid);
	mysql_tquery(dbHandle, query);

	return 1;
}

Dialog:HandleEditTexture2(playerid, response, listitem, inputtext[])
{
	if (!response)	return CallRemoteFunction("ShowEditFurniture", "i", playerid);

    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1)) return 1;

	new building = pTemp[playerid][pFurniture], slot = GetPVarInt(playerid, "chose_slot"), texture = GetPVarInt(playerid, "chose_texture");

	switch(listitem)
	{
	    case 0:
		{
		    new Float:x, Float:y, Float:z, Float:facing, Float:distance = 3.0;

		    GetPlayerPos(playerid, x, y, z);
		    GetPlayerFacingAngle(playerid, facing);

		  	x += (distance * floatsin(-facing, degrees));
		    y += (distance * floatcos(-facing, degrees));
            InfoMSG(playerid, "~r~Pressione ~w~\" ~y~Y ~w~\" e \" ~y~N ~w~\" para usar o menu de textura~n~\" ~y~H ~w~\" - mudar o slot do material~n~\" ~y~ESPACO~w~ \" - aplicar textura~n~\" ~y~ENTER ~w~\" para sair", 20);
			pTemp[playerid][p3DMenu] = Create3DMenu(playerid, x, y, z, facing, 16);
		    Select3DMenu(playerid, pTemp[playerid][p3DMenu]);
			return 1;
		}
		case 1:
	    {
			switch(pTemp[playerid][pFurnitureB])
			{
				case 0: {
					if (HFurn[building][slot][fTexture][texture] == -1) return SendErrorMessage(playerid, "Esta textura não está sendo usada.");
					SendSyntaxMessage(playerid, "Textura [ID: %i] do objeto %s excluída.", texture+1, HFurn[building][slot][fName]);

					new query[128];
					mysql_format(dbHandle, query, sizeof(query), "UPDATE `furnitures` SET `texture_%i` = -1 WHERE `id` = %i", texture, HFurn[building][slot][fID]);
					mysql_tquery(dbHandle, query);

					HFurn[building][slot][fTexture][texture] = -1;
					SetDynamicObjectMaterial(HFurn[building][slot][fObject], texture, -1, "none", "none", 0);
				} case 1: {
					if (BFurn[building][slot][fTexture][texture] == -1) return SendErrorMessage(playerid, "Esta textura não está sendo usada.");
					SendSyntaxMessage(playerid, "Textura [ID: %i] do objeto %s deletada.", texture+1, BFurn[building][slot][fName]);

					new query[128];
					mysql_format(dbHandle, query, sizeof(query), "UPDATE `furnitures` SET `texture_%i` = -1 WHERE `id` = %i", texture, BFurn[building][slot][fID]);
					mysql_tquery(dbHandle, query);

					BFurn[building][slot][fTexture][texture] = -1;
					SetDynamicObjectMaterial(BFurn[building][slot][fObject], texture, -1, "none", "none", 0);
				} case 2: {
					if (GFurn[building][slot][fTexture][texture] == -1)	return SendErrorMessage(playerid, "Esta textura não está sendo usada.");
					SendSyntaxMessage(playerid, "Textura [ID: %i] do objeto %s deletada.", texture+1, GFurn[building][slot][fName]);

					new query[128];
					mysql_format(dbHandle, query, sizeof(query), "UPDATE `furnitures` SET `texture_%i` = -1 WHERE `id` = %i", texture, GFurn[building][slot][fID]);
					mysql_tquery(dbHandle, query);

					GFurn[building][slot][fTexture][texture] = -1;
					SetDynamicObjectMaterial(GFurn[building][slot][fObject], texture, -1, "none", "none", 0);
				}
			}
	        return 1;
	    }
	}
	return 1;
}

Dialog:InformationFurniture(playerid, response, listitem, inputtext[])
{
    return CallRemoteFunction("ShowEditFurniture", "i", playerid);
}

Dialog:EditFurniture(playerid, response, listitem, inputtext[])
{
	if (!response)   return cmd::furniture(playerid, "");
    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1))	return 1;

	new building = pTemp[playerid][pFurniture], 
        slot = GetPVarInt(playerid, "chose_slot");

	new FCat, name[64], f_price, f_obj, f_vw, f_int;

	switch(pTemp[playerid][pFurnitureB])
	{
		case 0: {
			format(name, sizeof(name), "%s", HFurn[building][slot][fName]);
			FCat = HFurn[building][slot][fCategory];
			f_price = HFurn[building][slot][fMarketPrice]; 
			f_obj = HFurn[building][slot][fObject];
			f_vw = HFurn[building][slot][fVirtualWorld];
			f_int = HFurn[building][slot][fInterior];
		} case 1: { 
			format(name, sizeof(name), "%s", BFurn[building][slot][fName]);
			FCat = BFurn[building][slot][fCategory];
			f_price = BFurn[building][slot][fMarketPrice]; 
			f_obj = BFurn[building][slot][fObject];
			f_vw = BFurn[building][slot][fVirtualWorld];
			f_int = BFurn[building][slot][fInterior];
		} case 2: { 
			format(name, sizeof(name), "%s", GFurn[building][slot][fName]);
			FCat = GFurn[building][slot][fCategory];
			f_price = GFurn[building][slot][fMarketPrice]; 
			f_obj = GFurn[building][slot][fObject];
			f_vw = GFurn[building][slot][fVirtualWorld];
			f_int = GFurn[building][slot][fInterior];
		}
	}

    if (f_vw != GetWorld(playerid) || f_int != GetInterior(playerid)) return SendErrorMessage(playerid, "Este objeto está na rua, saia da casa para editar.");
	    		
	switch(listitem)
	{
	    case 0:
	    {
	        new mes[228];

	        format(mes, sizeof(mes), "{C3C3C3}Categoria:\t{ffffff}%s\n{C3C3C3}Nome:\t{ffffff}%s\n{C3C3C3}Preço:\t{ffffff}$%s", furniture_title[FCat], name, FormatNumber(f_price));
	        Dialog_Show(playerid, InformationFurniture, DIALOG_STYLE_MSGBOX, "{FFFFFF}Informação", mes, "Retornar", "");
	        return 1;
	    }
	    case 1:
	    {
	        InfoMSG(playerid, "Segure \" ~y~ESPACO ~w~\" e pressione \" ~y~MMB ~w~\" para mover o objeto para mais perto de você~n~Pressione \" ~r~ESC ~w~\" para desfazer a acao", 5);
	        SetPVarInt(playerid, "edit_furniture", 1);
	        EditDynamicObject(playerid, f_obj);
	        return 1;
	    }
	    case 2:
	    {
	        new str_tex[256], tex_name[5][32] = {"Vazio", "Vazio", "Vazio", "Vazio", "Vazio"}; // Vazio

	        switch(pTemp[playerid][pFurnitureB])
	        {
	            case 0: for(new i; i != 5; i++) if (HFurn[building][slot][fTexture][i] != -1) format(tex_name[i], 32, "%s", GetTextureName(HFurn[building][slot][fTexture][i]));
	            case 1: for(new i; i != 5; i++) if (BFurn[building][slot][fTexture][i] != -1) format(tex_name[i], 32, "%s", GetTextureName(BFurn[building][slot][fTexture][i]));
	            case 2: for(new i; i != 5; i++) if (GFurn[building][slot][fTexture][i] != -1) format(tex_name[i], 32, "%s", GetTextureName(GFurn[building][slot][fTexture][i]));
	        }           

	        format(str_tex, sizeof(str_tex), "{FFFFFF}Textura 1: {C3C3C3}%s\n{FFFFFF}Textura 2: {C3C3C3}%s\n{FFFFFF}Textura 3: {C3C3C3}%s\n{FFFFFF}Textura 4: {C3C3C3}%s\n{FFFFFF}Textura 5: {C3C3C3}%s\n{FF6347}> Remover texturas", tex_name[0], tex_name[1], tex_name[2], tex_name[3], tex_name[4]);
	        Dialog_Show(playerid, HandleEditTexture, DIALOG_STYLE_LIST, "{FFFFFF}Substituição de texturas", str_tex, "Selecionar", "Retornar");
	        return 1;
	    }
	    case 3:
	    {
	        new mes[268], sellprice = (f_price - (f_price & 2))  / 2;

	        format(mes, sizeof(mes), "{C3C3C3}Categoria:\t{ffffff}%s\n{C3C3C3}Nome:\t{ffffff}%s\n{C3C3C3}Preço:\t{ffffff}$%s\n\n{FF6347}Você receberá pela venda: $%s", furniture_title[FCat], name, FormatNumber(f_price), FormatNumber(sellprice));
	        Dialog_Show(playerid, HandleSellFurniture, DIALOG_STYLE_MSGBOX, "{FFFFFF}Vender", mes, "Vender", "Retornar");
	        return 1;
	    }
	    case 4:
	    {
	        Dialog_Show(playerid, FurnitureRename, DIALOG_STYLE_INPUT, name, "\t{ffffff}Digite um novo nome para este mobiliário:\t\n\t- Mínimo de caracteres 1.\t\n\t- Máximo de caracteres 32.\t", "Editar", "Retornar");
	        return 1;
	    }
	    case 5:
	    {
	        new mes[228];
	        format(mes, sizeof(mes), "{C3C3C3}Categoria:\t{ffffff}%s\n{C3C3C3}Nome:\t{ffffff}%s\n{C3C3C3}Preço:\t{ffffff}$%s\n{ffffff}", furniture_title[FCat], name, FormatNumber(f_price));
	        Dialog_Show(playerid, CopyFurniture, DIALOG_STYLE_MSGBOX, "{FFFFFF}Criar cópia", mes, "Comprar", "Retornar");
	    }
	}
	return 1;
}

Dialog:FurnitureRename(playerid, response, listitem, inputtext[])
{
	if (!response)																																												return 1;
    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1))	return 1;

	new building = pTemp[playerid][pFurniture], slot = GetPVarInt(playerid, "chose_slot"), f_id, name[64];

	switch(pTemp[playerid][pFurnitureB])
	{
		case 0: {
			format(name, sizeof(name), "%s", HFurn[building][slot][fName]);
			f_id = HFurn[building][slot][fCategory];
		} case 1: { 
			format(name, sizeof(name), "%s", BFurn[building][slot][fName]);
			f_id = BFurn[building][slot][fCategory];
		} case 2: { 
			format(name, sizeof(name), "%s", GFurn[building][slot][fName]);
			f_id = GFurn[building][slot][fCategory];
		}
	}

	if (strlen(inputtext) < 1 || strlen(inputtext) > 32)
	{
	    Dialog_Show(playerid, FurnitureRename, DIALOG_STYLE_INPUT, name, "\t\t{ffffff}Digite um novo nome para este mobiliário:\t\t\n\t\t\t - Mínimo de caracteres 1.\t\t\n\t\t\t - Máximo de caracteres 32.\t\t", "Editar", "Retornar");
	    return SendErrorMessage(playerid, "Nome de 1 a 32 caracteres.");
	}


	SendSyntaxMessage(playerid, "Você alterou o nome do item: %s{ffffff} para %s.", name, inputtext);
    format(name, 32, "%s", inputtext);
    
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `furnitures` SET `name` = '%e' WHERE `id` = %i", name, f_id);
	mysql_tquery(dbHandle, query);
	return 1;
}
	
Dialog:BuyFurniture(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
        FurnBuingDialog(playerid, GetPVarInt(playerid, "category_furniture"));
	    return 1;
	}

    if (pTemp[playerid][pFurniture] == -1) return 1;

    new building = pTemp[playerid][pFurniture], 
        index = GetPVarInt(playerid, "furniture_index");

    listitem = GetPVarInt(playerid, "category_furniture");

    if (GetCash(playerid) < furniture_list[listitem][index][frPrice]) 								  return SendNotCash(playerid);
	if (listitem == 28 && GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) == -1)               return SendErrorMessage(playerid, "Você precisa estar fora para comprar este mobiliário.");
	else if (listitem != 28 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) == -1)            return SendErrorMessage(playerid, "Você precisa estar dentro para comprar este mobiliário.");
	if (GetFurnitures(playerid) >= GetMaxPlayerFurniture(playerid))                                        return SendErrorMessage(playerid, "Você já comprou o número máximo de mobiliário.");
	if (listitem == 28 && GetOutsideFurnitures(playerid) >= 9 && GetOutsideFurnitures(playerid) <= 0)      return SendErrorMessage(playerid, "Apenas 10 objetos podem ser colocados do lado de fora.");

		
    new i = -1;

	switch(pTemp[playerid][pFurnitureB]) {
		case 0: {
			for(new a; a < MAX_FURNITURE; a++)
			{
				if (HFurn[building][a][fOn]) continue;
				
				i = a;
				break;
			}
		} case 1: {
			for(new a; a < MAX_FURNITURE; a++)
			{
				if (BFurn[building][a][fOn]) continue;
				
				i = a;
				break;
			}
		} case 2: {
			for(new a; a < MAX_FURNITURE; a++)
			{
				if (GFurn[building][a][fOn]) continue;
				
				i = a;
				break;
			}
		}
	}

	if (i == -1) return 1;

	SetPVarInt(playerid, "buy_furniture", 1);

	new Float:x, Float:y, Float:z;
    GetXYInFrontOfPlayer(playerid, x, y, z, 3.0);
	
	switch(pTemp[playerid][pFurnitureB]) {
		case 0: OnPlayerCallBuyFurniture(playerid, building, 0, i, furniture_list[listitem][index][frModel], listitem, index, x, y, z-1.0);
		case 1: OnPlayerCallBuyFurniture(playerid, building, 1, i, furniture_list[listitem][index][frModel], listitem, index, x, y, z-1.0);
		case 2: OnPlayerCallBuyFurniture(playerid, building, 2, i, furniture_list[listitem][index][frModel], listitem, index, x, y, z-1.0);
	}
	return 1;
}

Dialog:CopyFurniture(playerid, response, listitem, inputtext[])
{
	if (!response)
		return CallRemoteFunction("ShowEditFurniture", "i", playerid);

    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1))
		return 1;

	if (GetFurnitures(playerid) >= GetMaxPlayerFurniture(playerid))
		return SendErrorMessage(playerid, "Você já comprou o número máximo de mobiliário.");

    new building = pTemp[playerid][pFurniture], slot = GetPVarInt(playerid, "chose_slot");

	switch(pTemp[playerid][pFurnitureB])
	{
		case 0: HouseFurnCopy(playerid, building, slot);
		case 1: BizFurnCopy(playerid, building, slot);
		case 2: GarageFurnCopy(playerid, building, slot);
	}	
	
	return 1;
}

Dialog:HandleSellFurnitures(playerid, response, listitem, inputtext[])
{
	if (!response)																																												return cmd::furniture(playerid, "");
    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1))	return 1;
    	
	new building = pTemp[playerid][pFurniture];

	if (HouseInfo[building][hOwnerID] != PlayerInfo[playerid][pID] && pTemp[playerid][pBuildHouse] != HouseInfo[building][hID])	return SendErrorMessage(playerid, "Você não pode controlar o mobiliário nesta casa.");
	if (GetPVarInt(playerid, "buy_furniture"))																						return SendErrorMessage(playerid, "A ação já está em andamento.");

        
	switch(pTemp[playerid][pFurnitureB]) {
		case 0: HouseHandleSellFurn(playerid, building);
		case 1: BizHandleSellFurn(playerid, building);
		case 2: GarageHandleSellFurn(playerid, building);
	}	
	return 1;
}

Dialog:HandleSellFurniture(playerid, response, listitem, inputtext[])
{
    if (!response) return CallRemoteFunction("ShowEditFurniture", "i", playerid);
    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1)) return 1;
        
    new building = pTemp[playerid][pFurniture], name[64], f_price, slot = GetPVarInt(playerid, "chose_slot");

    switch(pTemp[playerid][pFurnitureB])
    {
        case 0: {
            if (!HFurn[building][slot][fOn]) return SendErrorMessage(playerid, "Não há acessórios comprados neste slot.");
            if (HouseInfo[building][hOwnerID] != PlayerInfo[playerid][pID] && pTemp[playerid][pBuildHouse] != HouseInfo[building][hID]) return SendErrorMessage(playerid, "Você não pode operar o mobiliário nesta casa.");
            
            f_price = HFurn[building][slot][fMarketPrice];
            format(name, sizeof(name), "%s", HFurn[building][slot][fName]);
        } 
        case 1: {
            if (!BFurn[building][slot][fOn]) return SendErrorMessage(playerid, "Não há acessórios comprados neste slot.");
            if (BizInfo[building][bizOwnerID] != PlayerInfo[playerid][pID] && pTemp[playerid][pBuildHouse] != BizInfo[building][bizID]) return SendErrorMessage(playerid, "Você não pode operar o mobiliário neste negócio.");

            f_price = BFurn[building][slot][fMarketPrice];
            format(name, sizeof(name), "%s", BFurn[building][slot][fName]);
        } 
        case 2: {
            if (!GFurn[building][slot][fOn]) return SendErrorMessage(playerid, "Não há acessórios comprados neste slot.");
            if (GarageInfo[building][garageOwner] != PlayerInfo[playerid][pID] && pTemp[playerid][pBuildHouse] != GarageInfo[building][garageID]) return SendErrorMessage(playerid, "Você não pode operar o mobiliário nesta garagem.");

            f_price = GFurn[building][slot][fMarketPrice];
            format(name, sizeof(name), "%s", GFurn[building][slot][fName]);
        }
    }
        
    new sellprice = (f_price - (f_price & 2)) / 2;
    SendSyntaxMessage(playerid, "Você vendeu %s, recebendo 50% do valor total, por $%s.", name, FormatNumber(sellprice));

    if (!GetPVarInt(playerid, "buy_furniture")) GiveCash(playerid, sellprice);
        
    OnPlayerSellFurniture(pTemp[playerid][pFurnitureB], building, slot);
    return 1;
}


Dialog:MODEL_SEL_FURNITURE(playerid, response, listitem, inputtext[])
{
    if (!response)
    {
        if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1)) return 1;
        
        ShowFurnitureCategories(playerid);
        return 1;
    }

    if (pTemp[playerid][pFurniture] == -1 || (GetNearOutBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1 && GetNearInBuilding(playerid, pTemp[playerid][pFurnitureB]) != -1)) return 1;
        
    new building = pTemp[playerid][pFurniture], 
        list = GetPVarInt(playerid, "category_furniture");

    if (GetFurnitures(playerid) >= GetMaxPlayerFurniture(playerid)) return SendErrorMessage(playerid, "Você já comprou o número máximo de mobiliário.");
    if (list == 28 && GetOutsideFurnitures(playerid) >= 9 && GetOutsideFurnitures(playerid) <= 0) return SendErrorMessage(playerid, "Apenas 10 objetos podem ser colocados fora da casa.");
    
    new model = furniture_list[list][listitem][frModel];
    if (list == 29 && (model == REACTOR_OBJECT || model == DRYER_OBJECT || model == MIXER_OBJECT)) {
        if (GetClosestFurniture(playerid, 30.0, model, DRUG_LAB) != -1) return SendErrorMessage(playerid, "Você já tem '%s'.", furniture_list[list][listitem][frName]);
    }

    new i = -1;

    switch(pTemp[playerid][pFurnitureB])
    {
        case 0: {
            for(new a; a < MAX_FURNITURE; a++)
            {
                if (!HFurn[building][a][fOn])
                {
                    i = a;
                    break;
                }                
            }
            if (i == -1) return 1;
        } case 1: {
            for(new a; a < MAX_FURNITURE; a++)
            {
                if (!BFurn[building][a][fOn])
                {
                    i = a;
                    break;
                }                
            }
            if (i == -1) return 1;
        } case 2: {
            for(new a; a < MAX_FURNITURE; a++)
            {
                if (!GFurn[building][a][fOn])
                {
                    i = a;
                    break;
                }                
            }
            if (i == -1) return 1;
        }
    }    

    static const str_size[] = "{C3C3C3}Categoria:\t{ffffff}%s\n{C3C3C3}Nome:\t{ffffff}%s\n{C3C3C3}Preço:\t{ffffff}$%s\n{ffffff}";
    new mes[sizeof(str_size) - 6 + 11 + 32 + 64];

    format(mes, sizeof(mes), str_size, furniture_title[list], furniture_list[list][listitem][frName], FormatNumber(furniture_list[list][listitem][frPrice]));
    SetPVarInt(playerid, "furniture_index", listitem);

    Dialog_Show(playerid, BuyFurniture, DIALOG_STYLE_MSGBOX, "Comprando mobiliário", mes, "Comprar", "Retornar");
    return 1;
}


FurnBuingDialog(playerid, listitem)
{
    SetPVarInt(playerid, "category_furniture", listitem);
    
    new objects = furniture_objects[listitem], object_names[153], object_amounts[153];

	for(new i; i < objects; i++)
	{
		object_names[i] = furniture_list[listitem][i][frModel];
	    object_amounts[i] = furniture_list[listitem][i][frPrice];
	}

	new mes[sizeof(object_names)*16];
	for(new i = 0; i < sizeof(object_names); i++)
	{
		if (object_names[i]) format(mes, sizeof(mes), "%s%i\t~g~$%i\n", mes, object_names[i], object_amounts[i]); 
	}
	
	new title[64];
	format(title, sizeof(title), "COMPRAR MOBILIA > %s", furniture_title[listitem]);
	Dialog_Show(playerid, MODEL_SEL_FURNITURE, DIALOG_STYLE_PREVIEW_MODEL, title, mes, "Selecionar", "Fechar");
}