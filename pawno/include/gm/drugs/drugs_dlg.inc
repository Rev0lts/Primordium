Dialog:MsgStartMix(playerid, response, listitem, inputtext[])
{
	if (!response)	return ResetMixingInfo(playerid);

	new f = GetClosestFurniture(playerid, 2.0, CENTRIFUGE_OBJECT, DRUG_LAB);
	if (f == -1)	{ ResetMixingInfo(playerid); return SendErrorMessage(playerid, "N�o h� uma centr�fuga perto de voc�."); }

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) { ResetMixingInfo(playerid); return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!"); }

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) { ResetMixingInfo(playerid); return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!"); }

	new i = GetFreeChemistyIndex();
	if (i == -1)	{ ResetMixingInfo(playerid); return SendErrorMessage(playerid,"Limite excedido! Entre em contato com o administrador t�cnico."); }

	switch(b_type)
	{
		case 0: {
			HFurn[building][f][ChemIdx] = i; 
			SQL_SetInteger("furnitures", "ChemIdx", i, HFurn[building][f][fID]);
		}
		case 1: {
			BFurn[building][f][ChemIdx] = i;
			SQL_SetInteger("furnitures", "ChemIdx", i, BFurn[building][f][fID]);
		}
		case 2: {
			GFurn[building][f][ChemIdx] = i;
			SQL_SetInteger("furnitures", "ChemIdx", i, GFurn[building][f][fID]);
		}
	}

	Chemistry[i][BuildingID] = building;
	Chemistry[i][BuildingType] = b_type;
	Chemistry[i][FurnID] = f;

	Chemistry[i][Extra][0] = Inventory[playerid][GetPVarInt(playerid, "chemicalItem")][pInvItem]-103;
	Chemistry[i][Extra][1] = Inventory[playerid][GetPVarInt(playerid, "chemicalItem1")][pInvItem]-103;
	Chemistry[i][Extra][2] = Inventory[playerid][GetPVarInt(playerid, "chemicalItem2")][pInvItem]-103;

	new
		time = floatround(GetPVarFloat(playerid, #DrugsAmount)*5),
		type = E_CHEMICAL[Chemistry[i][Extra][2]][TYPE],
		quality,
		Float: mix_amount = GetPVarFloat(playerid, #DrugsAmount) * float(randomEx(80, 99)) / 100.0
	;

	new	Float:chemsves[3];

	for(new d; d < 3; d++)
	{
		new pv[16];
		if (!d) 	format(pv, sizeof(pv), "chemicalItem");
		else 	format(pv, sizeof(pv), "chemicalItem%d", d);
				
		chemsves[d] = Inventory[playerid][GetPVarInt(playerid, pv)][pInvQFloat];
	}

	if (chemsves[2] - chemsves[1] > 0 && chemsves[2] - chemsves[0] > 0)
	{
		new Float: c_quality = chemsves[2] - chemsves[1] + chemsves[2] - chemsves[0];

		if (c_quality > 0.0 && c_quality <= 4.0)
		{
			if (CheckDrug(Chemistry[i][Extra][0], Chemistry[i][Extra][1], Chemistry[i][Extra][2])) quality = randomEx(15, 20);
			else																				  quality = randomEx(5, 10);
		}
		else if (c_quality > 4.0 && c_quality <= 10.0)
		{
			if (CheckDrug(Chemistry[i][Extra][0], Chemistry[i][Extra][1], Chemistry[i][Extra][2])) quality = randomEx(20, 30);
			else																				  quality = randomEx(10, 15);
		}
		else if (c_quality > 10.0 && c_quality <= 15.0)
		{
			if (CheckDrug(Chemistry[i][Extra][0], Chemistry[i][Extra][1], Chemistry[i][Extra][2])) quality = randomEx(30, 40);
			else																				  quality = randomEx(15, 20);
		}
		else if (c_quality > 15.0)
		{
			if (CheckDrug(Chemistry[i][Extra][0], Chemistry[i][Extra][1], Chemistry[i][Extra][2]))
			{
				if (chanceHandler(20))	quality = randomEx(75, 90);
				else 					quality = randomEx(40, 50);
			}
			else						quality = randomEx(25, 30);
		}
		else quality = 0;
	}
	else
	{
		if (chemsves[1] - chemsves[2] > 10 || chemsves[0] - chemsves[2] > 10)
			quality = randomEx(1, 5);
		else
			quality = randomEx(1, 15);
	}

	if (Chemistry[i][Extra][2] == 12 || Chemistry[i][Extra][2] == 13)
	{
		if (quality - 9 >= 0) quality -= randomEx(5, 9);
		else quality = 0;
	}

	SendHelpMessage(playerid, "%.1f (Reagentes) >>> %.1f (Drogas), Temperatura favor�vel: %s%d 'C", Inventory[playerid][GetPVarInt(playerid, "chemicalItem")][pInvQFloat] + Inventory[playerid][GetPVarInt(playerid, "chemicalItem2")][pInvQFloat] + Inventory[playerid][GetPVarInt(playerid, "chemicalItem1")][pInvQFloat], mix_amount, (E_CHEMICAL[Chemistry[i][Extra][2]][POINT] > 0) ? ("+") : (""), E_CHEMICAL[Chemistry[i][Extra][2]][POINT]);
	SendClientMessageEx(playerid, COLOR_LIGHTRED, "Tecnologia: %s > %s > %s", E_CHEMICAL[Chemistry[i][Extra][0]][NAME], E_CHEMICAL[Chemistry[i][Extra][1]][NAME], E_CHEMICAL[Chemistry[i][Extra][2]][NAME]);
	SendClientMessageEx(playerid, COLOR_LIGHTRED, "Ingrediente principal: %s", E_CHEMICAL[Chemistry[i][Extra][2]][CODE]);

	StartMixingDrug(i, time, mix_amount, float(quality), type, Chemistry[i][Extra][2]);

	RemoveFloatInvItem(playerid, Chemistry[i][Extra][0]+103, 0, GetPVarInt(playerid, "chemicalItem"));
	RemoveFloatInvItem(playerid, Chemistry[i][Extra][1]+103, 0, GetPVarInt(playerid, "chemicalItem1"));
	RemoveFloatInvItem(playerid, Chemistry[i][Extra][2]+103, 0, GetPVarInt(playerid, "chemicalItem2"));

	ResetMixingInfo(playerid);

	SendHelpMessage(playerid, "Ap�s terminar o processo, use {ffc46a}/miturardrogas{FFFFFF}.");
	return 1;
}

Dialog:ControlSelect(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new f = GetClosestFurniture(playerid, 1.5, REACTOR_OBJECT, DRUG_LAB);
	if (f == -1) return SendErrorMessage(playerid, "N�o h� um reator pr�ximo a voc�.");

	
	new i;
	switch(b_type) {
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}
	if (strval(inputtext) < 500 || strval(inputtext) > 500)	{
		cmd::reator(playerid, "");
		return SendErrorMessage(playerid, "Valor de temperatura inv�lido!");
	}

    if (strval(inputtext) > 0)
    {
        Chemistry[i][Control] = 1;
		Chemistry[i][Temperature] = strval(inputtext);
	}
	else if (strval(inputtext) < 0)
	{
	    Chemistry[i][Control] = 2;
		Chemistry[i][Temperature] = strval(inputtext);
	}
	else 
	{
		Chemistry[i][Control] = 0;
		Chemistry[i][Temperature] = 0;
	}

	SendSyntaxMessage(playerid, "Voc� definiu a temperatura em %s%.1f 'C.", (Chemistry[i][Temperature] > 0) ? ("+") : (""), Chemistry[i][Temperature]);
	return 1;
}

Dialog:Purification(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new f = GetClosestFurniture(playerid, 1.5, REACTOR_OBJECT, DRUG_LAB);
	if (f == -1) return SendErrorMessage(playerid, "N�o h� um reator pr�ximo a voc�.");

	new i;
	switch (b_type)
	{
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}

	new bb1 = Chemistry[i][Extra][0];
	new bb2 = Chemistry[i][Extra][1];
	new bb3 = Chemistry[i][Extra][2];

	if (CheckDrug(bb3, bb2, bb1))
	{
		new drugname[16];

		switch (bb3)
		{
			case 2: format(drugname, sizeof(drugname), "MDMA");
			case 4: format(drugname, sizeof(drugname), "Metanfetamina");
			case 5: format(drugname, sizeof(drugname), "LSD");
			case 11: format(drugname, sizeof(drugname), "Anfetamina");
			case 12: format(drugname, sizeof(drugname), "Coca�na");
			case 13: format(drugname, sizeof(drugname), "Hero�na");
		}

		if (GiveDrugInvItem(playerid, drugname, Chemistry[i][Amount], floatround(Chemistry[i][Quality]), E_CHEMICAL[Chemistry[i][Material]][TYPE]) == -1)
			return SendErrorMessage(playerid, "Invent�rio est� cheio.");
		ClearFurnitureData(f, i);
	}
	else Dialog_Show(playerid, DrugName, DIALOG_STYLE_INPUT, "Nome da droga", "Digite o nome da droga...", ">>", "Cancelar");
	return 1;
}


Dialog:DrugName(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new f = GetClosestFurniture(playerid, 1.5, REACTOR_OBJECT, DRUG_LAB);
    if (f == -1) return SendErrorMessage(playerid, "N�o h� um reator pr�ximo a voc�.");
	
	new i;
	switch(b_type) {
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}

	if (isnull(inputtext) || strlen(inputtext) > 16)	return SendErrorMessage(playerid, "Caracteres de 1 a 16 s�o permitidos!");

	if (CompareStrings(inputtext, "MDMA") || 
		CompareStrings(inputtext, "Metanfetamina") || 
		CompareStrings(inputtext, "LSD") || 
		CompareStrings(inputtext, "Anfetamina") || 
		CompareStrings(inputtext, "Coca�na") || 
		CompareStrings(inputtext, "Hero�na"))	return SendErrorMessage(playerid, "Voc� n�o pode nomear uma droga assim!");

	if (GiveDrugInvItem(playerid, inputtext, Chemistry[i][Amount], floatround(Chemistry[i][Quality]), E_CHEMICAL[Chemistry[i][Material]][TYPE])  == -1)	return SendErrorMessage(playerid, "Invent�rio est� cheio.");
	ClearFurnitureData(f, i);

	return 1;
}


Dialog:DryerDlg(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::secardrogas(playerid, "");
	
	listitem = GetPVarInt(playerid, #DreyrList);
	DeletePVar(playerid, #DreyrList);

	if (Inventory[playerid][listitem][pInvQFloat] < 0.1) return 1;

	new f = GetClosestFurniture(playerid, 2.0, DRYER_OBJECT, DRUG_LAB);
	if (f == -1)	{ ResetMixingInfo(playerid); return SendErrorMessage(playerid, "N�o h� instala��o de secagem pr�xima a voc�.");}

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) { ResetMixingInfo(playerid); return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio"); }

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) { ResetMixingInfo(playerid); return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio"); }

	new i = GetFreeChemistyIndex();
	if (i == -1)	{ ResetMixingInfo(playerid); return SendErrorMessage(playerid, "Limite excedido! Entre em contato com o administrador t�cnico"); }

	switch(b_type) {
		case 0: {
			HFurn[building][f][ChemIdx] = i; 
			SQL_SetInteger("furnitures", "ChemIdx", i, HFurn[building][f][fID]);
		}
		case 1: {
			BFurn[building][f][ChemIdx] = i;
			SQL_SetInteger("furnitures", "ChemIdx", i, BFurn[building][f][fID]);
		}
		case 2: {
			GFurn[building][f][ChemIdx] = i;
			SQL_SetInteger("furnitures", "ChemIdx", i, GFurn[building][f][fID]);
		}
	}

	new time = floatround(Inventory[playerid][listitem][pInvQFloat] * 1.5);

	Chemistry[i][BuildingID] = building;
	Chemistry[i][BuildingType] = b_type;
	Chemistry[i][FurnID] = f;
	Chemistry[i][Extra][2] = Inventory[playerid][listitem][pInvItem]-103;

	SendHelpMessage(playerid, "Para coletar reagentes secos, digite {ffc46a}/secardrogas{FFFFFF}.");

	StartMixingDrug(i, time, Inventory[playerid][listitem][pInvQFloat]*0.85, 0, 0, Chemistry[i][Extra][2]);
	RemoveFloatInvItem(playerid, Inventory[playerid][listitem][pInvItem], 0, listitem);
	return 1;
}


Dialog:ConfDryer(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	listitem = P_CHEMICAL[playerid][listitem][pSlot];
	if (Inventory[playerid][listitem][pInvQFloat] < 0.1) return 1;

	SetPVarInt(playerid, #DreyrList, listitem);

	new str_dlg[] = "{FFFFFF}Voc� est� confiante na sua escolha?\n\
					{33aa33}Selecionado:{FFFFFF} %s (%%.1fg >> %%.1fg)\n\n\
					Tempo aproximado: %i minutos",
		msg[sizeof(str_dlg) + 32 + 4 + 4],
		Float:f_time = Inventory[playerid][listitem][pInvQFloat] * 1.5,
		time = floatround(f_time)
	;
	
	format(msg, sizeof(msg), str_dlg, E_CHEMICAL[Inventory[playerid][listitem][pInvItem]-103][NAME], 
									Inventory[playerid][listitem][pInvQFloat], 
									Inventory[playerid][listitem][pInvQFloat]*0.85, 
									time);
	Dialog_Show(playerid, DryerDlg, DIALOG_STYLE_MSGBOX, "Secagem de Planta:", msg, "Sim", "Voltar");
	return 1;
}


stock Drugs_OnDialogResponse(playerid, dialogid, response, listitem)
{
	switch(dialogid)
	{
		case Part_Mixing:
		{
		    if (!response) return ResetMixingInfo(playerid);
			
			listitem = P_CHEMICAL[playerid][listitem][pSlot];

			if (Inventory[playerid][listitem][pInvQFloat] < 3.0) 
			{
				SendErrorMessage(playerid, "Voc� n�o pode usar o reagente se sua quantidade for inferior a 3g!");
				return ShowMixingDialog(playerid, Part_Mixing);
			}

		    SetPVarInt(playerid, "chosingItem", 1);
		    SetPVarInt(playerid, "chemicalItem", listitem);

			if (E_CHEMICAL[Inventory[playerid][listitem][pInvItem]-103][MAIN]) SetPVarInt(playerid, "MainChemical", listitem+1);

		    P_CHEMICAL[playerid][listitem][Selected] = true;
		    ShowMixingDialog(playerid, Extra_Mixing);
	    }
	    case Extra_Mixing:
	    {
		    if (!response) return ResetMixingInfo(playerid);
			
			listitem = P_CHEMICAL[playerid][listitem][pSlot];
			
			if (Inventory[playerid][GetPVarInt(playerid, "chemicalItem")][pInvItem] == Inventory[playerid][listitem][pInvItem]) return ShowMixingDialog(playerid, Extra_Mixing);
			if (Inventory[playerid][listitem][pInvQFloat] < 3.0) 
			{
				SendErrorMessage(playerid, "Voc� n�o pode usar o reagente se sua quantidade for inferior a 3g!");
				return ShowMixingDialog(playerid, Extra_Mixing);
			}
			
			if (GetPVarInt(playerid, "MainChemical") && E_CHEMICAL[Inventory[playerid][listitem][pInvItem]-103][MAIN]) 
			{
				SendErrorMessage(playerid, "Voc� n�o pode misturar esses reagentes!");
				return ShowMixingDialog(playerid, Extra_Mixing);
			}
			if (E_CHEMICAL[Inventory[playerid][listitem][pInvItem]-103][MAIN]) SetPVarInt(playerid, "MainChemical", listitem+1);

		    SetPVarInt(playerid, "chosingItem", 1);
		    SetPVarInt(playerid, "chemicalItem1", listitem);

		    P_CHEMICAL[playerid][listitem][Selected] = true;
		    ShowMixingDialog(playerid, Final_Mixing);
	    }
	    case Final_Mixing:
	    {
		    if (!response) return ResetMixingInfo(playerid);
			
			listitem = P_CHEMICAL[playerid][listitem][pSlot];
			
			if (Inventory[playerid][listitem][pInvItem] == Inventory[playerid][GetPVarInt(playerid, "chemicalItem")] [pInvItem]
			|| Inventory[playerid][listitem][pInvItem] == Inventory[playerid][GetPVarInt(playerid, "chemicalItem1")][pInvItem])
			{
			    return ShowMixingDialog(playerid, Final_Mixing);
			}
			if (Inventory[playerid][listitem][pInvQFloat] < 3.0) 
			{
				SendErrorMessage(playerid, "Voc� n�o pode usar o reagente se sua quantidade for inferior a 3g!");
				return ShowMixingDialog(playerid, Final_Mixing);
			}
			if (GetPVarInt(playerid, "MainChemical") && E_CHEMICAL[Inventory[playerid][listitem][pInvItem]-103][MAIN]) 
			{
				SendErrorMessage(playerid, "Voc� n�o pode misturar esses reagentes!");
				return ShowMixingDialog(playerid, Final_Mixing);
			}

			SetPVarFloat(playerid, #DrugsAmount, (Inventory[playerid][GetPVarInt(playerid, "chemicalItem")][pInvQFloat] + Inventory[playerid][GetPVarInt(playerid, "chemicalItem1")][pInvQFloat] + Inventory[playerid][listitem][pInvQFloat]) / 9 );
			
			if (Inventory[playerid][GetPVarInt(playerid, "chemicalItem")][pInvQFloat] + Inventory[playerid][GetPVarInt(playerid, "chemicalItem1")][pInvQFloat] + Inventory[playerid][listitem][pInvQFloat] > 1000.0) 
				return SendErrorMessage(playerid, "Voc� n�o pode carregar mais de 1kg de reagentes para s�ntese de drogas.");

			if (!GetPVarInt(playerid, "MainChemical"))
			{
				SetPVarInt(playerid, "chemicalItem2", listitem); 
			}
			else if (GetPVarInt(playerid, "chemicalItem") == GetPVarInt(playerid, "MainChemical")-1)
			{
				SetPVarInt(playerid, "chemicalItem2", GetPVarInt(playerid, "chemicalItem"));
				SetPVarInt(playerid, "chemicalItem", listitem);
			}
			else if (GetPVarInt(playerid, "chemicalItem1") == GetPVarInt(playerid, "MainChemical")-1)
			{
				SetPVarInt(playerid, "chemicalItem2", GetPVarInt(playerid, "chemicalItem"));
				SetPVarInt(playerid, "chemicalItem1", listitem);
			}

			SetPVarInt(playerid, #DrugsTemp, E_CHEMICAL[Inventory[playerid][GetPVarInt(playerid, "chemicalItem2")][pInvItem]-103][POINT]);

			new	chemsname[3][32],
				chemscode[3][32],
				Float:chemsves[3];

			for(new i; i < 3; i++)
			{
				new pv[16];
				if (!i) 	format(pv, sizeof(pv), "chemicalItem");
				else 	format(pv, sizeof(pv), "chemicalItem%d", i);
				
				format(chemsname[i], 32, "%s", E_CHEMICAL[Inventory[playerid][GetPVarInt(playerid, pv)][pInvItem]-103][NAME]);
				format(chemscode[i], 32, "%s", E_CHEMICAL[Inventory[playerid][GetPVarInt(playerid, pv)][pInvItem]-103][CODE]);
				chemsves[i] = Inventory[playerid][GetPVarInt(playerid, pv)][pInvQFloat];
			}

			static str_size[] = "{b5c8b6}COMPOSI��O:{FFFFFF}\n\
							1. %s {b5c8b6}[%s]{FFFFFF} (%.1fg) - {39c622}PRINCIPAL{FFFFFF}\n\
							2. %s {b5c8b6}[%s]{FFFFFF} (%.1fg)\n\
							3. %s {b5c8b6}[%s]{FFFFFF} (%.1fg)\n\n\
							Temperatura favor�vel:\t%s%i 'C {b5c8b6}(para reator){FFFFFF}\n\
							Peso aproximado da droga:\t~%.1f";
			new msg[sizeof(str_size) + 32*3 + 32*3 + 6 + 6 + 93];

			format(msg, sizeof(msg), str_size, 
				chemsname[2], chemscode[2], chemsves[2], 
				chemsname[1], chemscode[1], chemsves[1],
				chemsname[0], chemscode[0], chemsves[0],
				(GetPVarInt(playerid, #DrugsTemp) > 0) ? ("+") : (""), 
				GetPVarInt(playerid, #DrugsTemp),
				GetPVarFloat(playerid, #DrugsAmount)
			);

			if (Inventory[playerid][GetPVarInt(playerid, "chemicalItem2")][pInvItem]-103 == 12) strcat(msg, "\n\n{FF6347}ATEN��O!{FFFFFF}\nSeque as folhas de coca para aumentar a pureza!");
			if (Inventory[playerid][GetPVarInt(playerid, "chemicalItem2")][pInvItem]-103 == 13) strcat(msg, "\n\n{FF6347}ATEN��O!{FFFFFF}\n� melhor secar papoula para melhorar a limpeza!");

			Dialog_Show(playerid, MsgStartMix, DIALOG_STYLE_MSGBOX, "Centrifuga", msg, "Iniciar", "Cancelar");
	    }
    }
	return 1;
}