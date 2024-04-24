Dialog:Clothing(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new string[128], count;

	for(new i = 0; i < MAX_PLAYER_ATTACH; i++)
	{
	    if (AttachInfo[playerid][i][aID] == 0) continue;
		if (count == listitem)
		{
		    listitem = i;
		    break;
		}
		count++;
	}

	SetPVarInt(playerid, #edit_slot_attach, listitem);

	format(string, sizeof(string), "%s\nEdit item\nDestroy an item", (AttachInfo[playerid][listitem][aUsed]) ? ("Remove item") : ("Attach item"));
	Dialog_Show(playerid, Clothing1, DIALOG_STYLE_LIST, AttachInfo[playerid][listitem][aName], string, "Select", "Close");
	return 1;
}

Dialog:Clothing1(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    new slot = GetPVarInt(playerid, #edit_slot_attach);

	switch(listitem)
	{
		case 0:
		{
			if (IsPlayerAttachedObjectSlotUsed(playerid, slot))
			{
				InfoMSG(playerid, "~w~Você ~r~removeu~w~ as roupas", 5);
				RemovePlayerAttachedObject(playerid, slot);
				AttachInfo[playerid][slot][aUsed] = 0;

				new query[128];
				mysql_format(dbHandle, query, sizeof(query), "UPDATE `player_attach` SET `used`=0 WHERE `id` = %i", AttachInfo[playerid][slot][aID]);
				mysql_tquery(dbHandle, query);
			}
			else
			{
				InfoMSG(playerid, "~w~Você ~g~vestiu~w~ as roupas", 5);
				AttachInfo[playerid][slot][aUsed] = 1;
				SetPlayerAttachedObject(playerid, slot, AttachInfo[playerid][slot][aModel], AttachInfo[playerid][slot][aBone], AttachInfo[playerid][slot][aOffsetX], AttachInfo[playerid][slot][aOffsetY], AttachInfo[playerid][slot][aOffsetZ], AttachInfo[playerid][slot][aRotX], AttachInfo[playerid][slot][aRotY], AttachInfo[playerid][slot][aRotZ], AttachInfo[playerid][slot][aScaleX], AttachInfo[playerid][slot][aScaleY], AttachInfo[playerid][slot][aScaleZ]);

                new query[128];
				mysql_format(dbHandle, query, sizeof(query), "UPDATE `player_attach` SET `used`=1 WHERE `id` = %i", AttachInfo[playerid][slot][aID]);
				mysql_tquery(dbHandle, query);
			}
			cmd::clothing(playerid, "");
		}
		case 1:
		{
			if (!IsPlayerAttachedObjectSlotUsed(playerid, slot))		return SendErrorMessage(playerid, "Um objeto deste slot não está instalado.");
			if (GetPVarInt(playerid, #edit_attach) == 1)				return SendErrorMessage(playerid, "Você já está editando o objeto, termine a edição para começar um novo.");

            Dialog_Show(playerid, SelectEditBone, DIALOG_STYLE_LIST, "Parte do corpo","- Deixar inalterado\n1: Costas\n2: Cabeça\n3: Ombro esquerdo\n4: Ombro direito\n5: Mão esquerda\n6: Braço direito\n7: Coxa esquerda\n8: Coxa direita\n9: Perna esquerda\n10: Perna direita\n11: Panturrilha direita\n12: Panturrilha esquerda\n13: Antebraço esquerdo\n14: Antebraço direito\n15: Clavícula esquerda\n16: Clavícula direita\n17: Pescoço\n18: Mandíbula","Selecionar","Cancelar");
		}
		case 2:
		{
			InfoMSG(playerid, "~w~Você ~r~removeu~w~ as roupas", 5);
			RemovePlayerAttachFromSlot(playerid, slot);
		}
	}
	return 1;
}


Dialog:ClothesCategory(playerid, response, listitem, inputtext[])
{
	new str[1024];

	if (!response) return 1;

	switch (listitem)
	{
		case 0:
		{
			for (new x; x < sizeof(HatsInfo); x++)
			{
				format(str, sizeof(str), "%s%s - $%i\n", str, HatsInfo[x][toyName], HatsInfo[x][toyPrice]);
			}
			Dialog_Show(playerid, HatsDialog, DIALOG_STYLE_LIST, "Selecione o produto que você precisa", str, "Selecionar", "Fechar");
		}
		case 1:
		{
			for (new x; x < sizeof(GlassesInfo); x++)
			{
				format(str, sizeof(str), "%s%s - $%i\n", str, GlassesInfo[x][toyName], GlassesInfo[x][toyPrice]);
			}
			Dialog_Show(playerid, GlassesDialog, DIALOG_STYLE_LIST, "Selecione o produto que você precisa", str, "Selecionar", "Fechar");
		}
		case 2:
		{
			for (new x; x < sizeof(BandanasInfo); x++)
			{
				format(str, sizeof(str), "%s%s - $%i\n", str, BandanasInfo[x][toyName], BandanasInfo[x][toyPrice]);
			}
			Dialog_Show(playerid, BandanasDialog, DIALOG_STYLE_LIST, "Selecione o produto que você precisa", str, "Selecionar", "Fechar");
		}
		case 3:
		{
			for (new x; x < sizeof(HeadphonesInfo); x++)
			{
				format(str, sizeof(str), "%s%s - $%i\n", str, HeadphonesInfo[x][toyName], HeadphonesInfo[x][toyPrice]);
			}
			Dialog_Show(playerid, HeadphonesDialog, DIALOG_STYLE_LIST, "Selecione o produto que você precisa", str, "Selecionar", "Fechar");
		}
	}
	return 1;
}


Dialog:SelectEditBone(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::clothing(playerid, "");

    new slot = GetPVarInt(playerid, #edit_slot_attach);

	if (!IsPlayerAttachedObjectSlotUsed(playerid, slot))	return SendErrorMessage(playerid, "Você não instalou um objeto para este slot. ");
	if (GetPVarInt(playerid, #edit_attach) == 1)			return SendErrorMessage(playerid, "Você já está editando o objeto, termine a edição para começar um novo. ");

	if (listitem != 0)
	{
	    RemovePlayerAttachedObject(playerid, slot);
		SetPlayerAttachedObject(playerid, slot, AttachInfo[playerid][slot][aModel], listitem);
	}

	InfoMSG(playerid, "~w~ Você está ~y~editando ~w~ o objeto no personagem", 5);
	SetPVarInt(playerid, #edit_attach, 1);
	EditAttachedObject(playerid, slot);
	return 1;
}


Dialog:SelectBone(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    new faction = PlayerInfo[playerid][pFaction];

    if (!IsFactionLegal(faction)) return SendErrorMessage(playerid, "Você deve ser um policial ou guarda de prisão.");

    new i = -1;

    if ((i = GetNextToySlot(playerid)) == -1) return SendErrorMessage(playerid, "Todos os slots de roupas estão ocupados.");

    new index = GetPVarInt(playerid, #select_attach);

    if (IsFactionMedic(faction) || IsFactionFire(faction))
    {
        SetPlayerAttachedObject(playerid, i, attach_fd_model[index], listitem + 1);
        EditAttachedObject(playerid, i);
        InsertPlayerAttach(playerid, i, attach_fd_model[index], listitem + 1, attach_fd_name[index]);
        SetPVarInt(playerid, #edit_faction, 1);
    }
    else if (IsFactionCity(faction))
    {
        SetPlayerAttachedObject(playerid, i, attach_city_model[index], listitem + 1);
        EditAttachedObject(playerid, i);
        InsertPlayerAttach(playerid, i, attach_city_model[index], listitem + 1, attach_city_name[index]);
        SetPVarInt(playerid, #edit_faction, 1);
    }
    else
    {
        SetPlayerAttachedObject(playerid, i, attach_police_model[index], listitem + 1);
        EditAttachedObject(playerid, i);
        InsertPlayerAttach(playerid, i, attach_police_model[index], listitem + 1, attach_police_name[index]);
        SetPVarInt(playerid, #edit_faction, 1);
    }
    return 1;
}


Dialog:BizAttachDialog(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    new string[1024];

    switch(listitem) {
        case 0: { // Guitarras elétricas
            for(new x; x < sizeof(GuitarsInfo); x++)
            {
                format(string, sizeof(string), "%s%s - $%i\n", string, GuitarsInfo[x][toyName], GuitarsInfo[x][toyPrice]);
            }
            Dialog_Show(playerid, GuitarsDialog, DIALOG_STYLE_LIST, "{FFFFFF}Selecione um produto", string, "Selecionar", "Fechar");
        }
        case 1: { // Relógios
            for(new x; x < sizeof(ClockInfo); x++)
            {
                format(string, sizeof(string), "%s%s - $%i\n", string, ClockInfo[x][toyName], ClockInfo[x][toyPrice]);
            }
            Dialog_Show(playerid, ClockDialog, DIALOG_STYLE_LIST, "{FFFFFF}Selecione um produto", string, "Selecionar", "Fechar");
        }
        case 2: { // Produtos de hardware
            for(new x; x < sizeof(HardwareStore); x++)
            {
                format(string, sizeof(string), "%s%s - $%i\n", string, HardwareStore[x][toyName], HardwareStore[x][toyPrice]);
            }
            Dialog_Show(playerid, HardwareStore, DIALOG_STYLE_LIST, "{FFFFFF}Selecione um produto", string, "Selecionar", "Fechar");
        }
        case 3: { // Equipamento esportivo
            for(new x; x < sizeof(SportsInfo); x++)
            {
                format(string, sizeof(string), "%s%s - $%i\n", string, SportsInfo[x][toyName], SportsInfo[x][toyPrice]);
            }
            Dialog_Show(playerid, SportsDialog, DIALOG_STYLE_LIST, "{FFFFFF}Selecione um produto", string, "Comprar", "Fechar");
        }
        case 4: { // Roupas
            Dialog_Show(playerid, ClothesCategory, DIALOG_STYLE_LIST, "{FFFFFF}Selecione uma categoria", "Chapéus\nÓculos\nBandanas\nFones de ouvido", "Selecionar", "Fechar");
        }
    }
    return 1;
}


Dialog:HatsDialog(playerid, response, listitem, inputtext[])
{
    if (!response)	return 1;

	new i = -1;

	if ((i = GetNextToySlot(playerid)) == -1)				return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    if (GetCash(playerid) < HatsInfo[listitem][toyPrice])	return SendNotCash(playerid);

	SetPlayerAttachedObject(playerid, i, HatsInfo[listitem][toyModel], HatsInfo[listitem][toyBone], HatsInfo[listitem][toyOffestX], HatsInfo[listitem][toyOffestY], HatsInfo[listitem][toyOffestZ], HatsInfo[listitem][toyRotX], 
	HatsInfo[listitem][toyRotY], HatsInfo[listitem][toyRotZ], HatsInfo[listitem][toyScaleX], HatsInfo[listitem][toyScaleY], HatsInfo[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
	SetPVarInt(playerid, #buy_attach, HatsInfo[listitem][toyPrice]);
	InsertPlayerAttach(playerid, i, HatsInfo[listitem][toyModel], HatsInfo[listitem][toyBone], HatsInfo[listitem][toyName]);
	return 1;
}

Dialog:GlassesDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new i = -1;

	if ((i = GetNextToySlot(playerid)) == -1)				return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    if (GetCash(playerid) < GlassesInfo[listitem][toyPrice])	return SendNotCash(playerid);

	SetPlayerAttachedObject(playerid, i, GlassesInfo[listitem][toyModel], GlassesInfo[listitem][toyBone], GlassesInfo[listitem][toyOffestX], GlassesInfo[listitem][toyOffestY], GlassesInfo[listitem][toyOffestZ],
	GlassesInfo[listitem][toyRotX], GlassesInfo[listitem][toyRotY], GlassesInfo[listitem][toyRotZ], GlassesInfo[listitem][toyScaleX], GlassesInfo[listitem][toyScaleY], GlassesInfo[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
	SetPVarInt(playerid, #buy_attach, GlassesInfo[listitem][toyPrice]);
	InsertPlayerAttach(playerid, i, GlassesInfo[listitem][toyModel], GlassesInfo[listitem][toyBone], GlassesInfo[listitem][toyName]);
	return 1;
}

Dialog:BandanasDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new i = -1;

	if ((i = GetNextToySlot(playerid)) == -1)					return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    if (GetCash(playerid) < BandanasInfo[listitem][toyPrice])	return SendNotCash(playerid);

	SetPlayerAttachedObject(playerid, i, BandanasInfo[listitem][toyModel], BandanasInfo[listitem][toyBone], BandanasInfo[listitem][toyOffestX], BandanasInfo[listitem][toyOffestY], BandanasInfo[listitem][toyOffestZ], 
	BandanasInfo[listitem][toyRotX], BandanasInfo[listitem][toyRotY], BandanasInfo[listitem][toyRotZ], BandanasInfo[listitem][toyScaleX], BandanasInfo[listitem][toyScaleY], BandanasInfo[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
	SetPVarInt(playerid, #buy_attach, BandanasInfo[listitem][toyPrice]);
	InsertPlayerAttach(playerid, i, BandanasInfo[listitem][toyModel], BandanasInfo[listitem][toyBone], BandanasInfo[listitem][toyName]);
	return 1;
}
Dialog:HeadphonesDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new i = -1;

	if ((i = GetNextToySlot(playerid)) == -1)					return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    if (GetCash(playerid) < HeadphonesInfo[listitem][toyPrice])	return SendNotCash(playerid);

	SetPlayerAttachedObject(playerid, i, HeadphonesInfo[listitem][toyModel], HeadphonesInfo[listitem][toyBone], HeadphonesInfo[listitem][toyOffestX], HeadphonesInfo[listitem][toyOffestY], HeadphonesInfo[listitem][toyOffestZ],
	HeadphonesInfo[listitem][toyRotX], HeadphonesInfo[listitem][toyRotY], HeadphonesInfo[listitem][toyRotZ], HeadphonesInfo[listitem][toyScaleX], HeadphonesInfo[listitem][toyScaleY], HeadphonesInfo[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
	SetPVarInt(playerid, #buy_attach, HeadphonesInfo[listitem][toyPrice]);
	InsertPlayerAttach(playerid, i, HeadphonesInfo[listitem][toyModel], HeadphonesInfo[listitem][toyBone], HeadphonesInfo[listitem][toyName]);
	return 1;
}

Dialog:SportsDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new i = -1;

	if ((i = GetNextToySlot(playerid)) == -1)				return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    if (GetCash(playerid) < SportsInfo[listitem][toyPrice])	return SendNotCash(playerid);

    if (SportsInfo[listitem][toyModel] == 19878)
	{
	    if (GiveInvItem(playerid, 77, 1) == -1)	return SendErrorMessage(playerid, "Inventário cheio.");

		SendSyntaxMessage(playerid, "Você comprou um skate.");
		return 1;
	}
	SetPlayerAttachedObject(playerid, i, SportsInfo[listitem][toyModel], SportsInfo[listitem][toyBone], SportsInfo[listitem][toyOffestX], SportsInfo[listitem][toyOffestY], SportsInfo[listitem][toyOffestZ], SportsInfo[listitem][toyRotX], 
	SportsInfo[listitem][toyRotY], SportsInfo[listitem][toyRotZ], SportsInfo[listitem][toyScaleX], SportsInfo[listitem][toyScaleY], SportsInfo[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
	SetPVarInt(playerid, #buy_attach, SportsInfo[listitem][toyPrice]);
	InsertPlayerAttach(playerid, i, SportsInfo[listitem][toyModel], SportsInfo[listitem][toyBone], SportsInfo[listitem][toyName]);
	return 1;
}


Dialog:ClockDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new i = -1;

	if ((i = GetNextToySlot(playerid)) == -1)				return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    if (GetCash(playerid) < ClockInfo[listitem][toyPrice])	return SendNotCash(playerid);

	SetPlayerAttachedObject(playerid, i, ClockInfo[listitem][toyModel], ClockInfo[listitem][toyBone], ClockInfo[listitem][toyOffestX], ClockInfo[listitem][toyOffestY], ClockInfo[listitem][toyOffestZ], ClockInfo[listitem][toyRotX], 
	ClockInfo[listitem][toyRotY], ClockInfo[listitem][toyRotZ], ClockInfo[listitem][toyScaleX], ClockInfo[listitem][toyScaleY], ClockInfo[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
	SetPVarInt(playerid, #buy_attach, ClockInfo[listitem][toyPrice]);
	InsertPlayerAttach(playerid, i, ClockInfo[listitem][toyModel], ClockInfo[listitem][toyBone], ClockInfo[listitem][toyName]);
	return 1;
}


Dialog:BarberDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new i = -1;

	if ((i = GetNextToySlot(playerid)) == -1)				return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    if (GetCash(playerid) < BarberShop[listitem][toyPrice])	return SendNotCash(playerid);

	SetPlayerAttachedObject(playerid, i, BarberShop[listitem][toyModel], BarberShop[listitem][toyBone], BarberShop[listitem][toyOffestX], BarberShop[listitem][toyOffestY], BarberShop[listitem][toyOffestZ], BarberShop[listitem][toyRotX], 
	BarberShop[listitem][toyRotY], BarberShop[listitem][toyRotZ], BarberShop[listitem][toyScaleX], BarberShop[listitem][toyScaleY], BarberShop[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
	SetPVarInt(playerid, #buy_attach, BarberShop[listitem][toyPrice]);
	InsertPlayerAttach(playerid, i, BarberShop[listitem][toyModel], BarberShop[listitem][toyBone], BarberShop[listitem][toyName]);
	return 1;
}

Dialog:HardwareStore(playerid, response, listitem, inputtext[])
{
    // TO DO = MOVER PARA VEÍCULOS

    if (!response) return 1;
    if (GetCash(playerid) < HardwareStore[listitem][toyPrice]) return SendNotCash(playerid);

    if (listitem == 14)
    {
        if (!IsValidVehicle(PlayerInfo[playerid][pCarKey])) return SendErrorMessage(playerid, "Your vehicle is not spawned.");

        static const str_size[] = "{ffffff}Duplicate for %s. Type {C3C3C3}[ID/Part Of Name] {ffffff}who do you want to give duplicate keys to:";
        new mes[sizeof(str_size) - 2 + 1 + 32];

        format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(PlayerInfo[playerid][pCarKey])-400]);
        Dialog_Show(playerid, DuplicateKey, DIALOG_STYLE_INPUT, "Duplicate", mes, "Buy", "Cancel");
        return 1;
    }

    new i = -1;

    if ((i = GetNextToySlot(playerid)) == -1) return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");

    SetPlayerAttachedObject(playerid, i, HardwareStore[listitem][toyModel], HardwareStore[listitem][toyBone], HardwareStore[listitem][toyOffestX], HardwareStore[listitem][toyOffestY], HardwareStore[listitem][toyOffestZ], 
    HardwareStore[listitem][toyRotX], HardwareStore[listitem][toyRotY], HardwareStore[listitem][toyRotZ], HardwareStore[listitem][toyScaleX], HardwareStore[listitem][toyScaleY], HardwareStore[listitem][toyScaleZ]);
    EditAttachedObject(playerid, i);
    SetPVarInt(playerid, #buy_attach, HardwareStore[listitem][toyPrice]);
    InsertPlayerAttach(playerid, i, HardwareStore[listitem][toyModel], HardwareStore[listitem][toyBone], HardwareStore[listitem][toyName]);
    return 1;
}

Dialog:GuitarsDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (listitem == 3)
	{
		if (GetCash(playerid) < GuitarsInfo[listitem][toyPrice])	return SendNotCash(playerid);
		if (GiveInvItem(playerid, INV_BOOMBOX, 1) == -1)		return SendErrorMessage(playerid, "Inventário cheio.");

		GiveCash(playerid, -GuitarsInfo[listitem][toyPrice]);
		SendSyntaxMessage(playerid, "Você comprou uma boombox por $%i.", GuitarsInfo[listitem][toyPrice]);
	}
	else
	{
	    new i = -1;

		if ((i = GetNextToySlot(playerid)) == -1)				return SendErrorMessage(playerid, "Todas as vagas de acessórios estão ocupadas.");
    	if (GetCash(playerid) < GuitarsInfo[listitem][toyPrice])	return SendNotCash(playerid);

		SetPlayerAttachedObject(playerid, i, GuitarsInfo[listitem][toyModel], 1, 0.0, -0.145, 0.0, 14.8, 85.1, -19.2, 1.0, 1.0, 1.0);
        EditAttachedObject(playerid, i);
		SetPVarInt(playerid, #buy_attach, GuitarsInfo[listitem][toyPrice]);
		InsertPlayerAttach(playerid, i, GuitarsInfo[listitem][toyModel], 1, GuitarsInfo[listitem][toyName]);
	}
	return 1;
}