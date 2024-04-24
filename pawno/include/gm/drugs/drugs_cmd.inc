CMD:planta(playerid, params[])
{
    new option[16], secoption[6];
    
    if (sscanf(params, "s[16]S()[6]", option, secoption))
    {
        SendHelpMessage(playerid, "/plantaa [op��es]");
        SendSyntaxMessage(playerid, "plantar / colher");
        return 1;
    }

    if (CompareStrings(option, "plantar"))
    {
        new drugname[16];

        if (sscanf(secoption, "s[6]", drugname))
        {
            SendHelpMessage(playerid, "/plantaa plantar [maconha/cocaina/papoula]");
            return 1;
        }

        new type;
        if (CompareStrings(drugname, "maconha"))
        {
            if (!IsPlayerHasItem(playerid, INV_SEED_GANJA)) return SendErrorMessage(playerid, "Voc� n�o tem sementes de maconha.");
            type = 1;
        }
        else if (CompareStrings(drugname, "cocaina"))
        {
            if (!IsPlayerHasItem(playerid, INV_SEED_COCA)) return SendErrorMessage(playerid, "Voc� n�o tem sementes de coca�na.");
            type = 2;
        }
        else if (CompareStrings(drugname, "papoula"))
        {
            if (!IsPlayerHasItem(playerid, INV_SEED_POPPY)) return SendErrorMessage(playerid, "Voc� n�o tem sementes de papoula.");
            type = 3;
        }
        else 
        {
            SendHelpMessage(playerid, "/plantaa plantar [maconha/cocaina/sementes]");
            return 1;
        }
        
        if (Plant_Nearest(playerid, 2.0) != -1)    return SendErrorMessage(playerid, "J� existe uma planta plantada ao seu lado.");
        if (pTemp[playerid][pPlantTime] > 0)    return SendErrorMessage(playerid, "Voc� j� est� plantando uma planta.");
        
        pTemp[playerid][pPlantTime] = 15;

        switch(type)
        {
            case 1: format(drugname, sizeof(drugname), "maconha");
            case 2: format(drugname, sizeof(drugname), "coca�na");
            case 3: format(drugname, sizeof(drugname), "papoula");
        }

        pTemp[playerid][pPlantType] = type;
        
        SendSyntaxMessage(playerid, "Voc� come�ou a plantar um arbusto de %s. Use {FF6347}/stop{FFFFFF}, se quiser desfazer a a��o.", drugname);
        
        FreezePlayer(playerid);
        OnPlayAnim(playerid, "BOMBER","BOM_Plant_Loop", 4.0, 1, 0, 0, 0, 0);
        return 1;
    }
    
    else if (CompareStrings(option, "colher"))
    {
        new plant = -1;
        if ((plant = Plant_Nearest(playerid)) == -1)            return SendErrorMessage(playerid, "N�o h� plantas perto de voc�.");
        if (CropsInfo[plant][pGrowTime] > gettime())            return SendErrorMessage(playerid, "A planta ainda n�o est� madura.");

        new drugname[16],
            type,
            Float: gPlant;


        switch(CropsInfo[plant][pType])
        {
            case 1:
            {
                gPlant = float(randomEx(10, 30)) / 10.0;
                type = INV_GANJA;
                format(drugname, sizeof(drugname), "maconha");
            }
            case 2:
            {
                gPlant = float(randomEx(70, 300)) / 10.0;
                type = INV_COCA;
                format(drugname, sizeof(drugname), "folha de coca�na");
            }
            case 3:
            {
                gPlant = float(randomEx(50, 90)) / 10.0;
                type = INV_POPPY;
                format(drugname, sizeof(drugname), "papoula");
            }
        }
        if (GiveFloatInvItem(playerid, type, gPlant) == -1) return SendErrorMessage(playerid, "Invent�rio cheio.");

        DestroyCrop(plant);
        OnPlayAnim(playerid, "BOMBER", "BOM_Plant_Loop", 4.0, 0, 0, 0, 0, 0);

        SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "* %s coletou da planta.", GetNameWithMask(playerid));
        SendSyntaxMessage(playerid, "Voc� coletou %.1fg de %s.", gPlant, drugname);
    }
    else 
    {
        SendHelpMessage(playerid, "/planta [adicionar / pegar]");
    }
    return 1;
}

CMD:misturardrogas(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return SendErrorMessage(playerid, "Voc� n�o pode usar esse recurso no momento.");

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid,"Voc� deve estar no laborat�rio!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new f = GetClosestFurniture(playerid, 1.5, CENTRIFUGE_OBJECT, DRUG_LAB);
    if (f == -1) return SendErrorMessage(playerid, "N�o h� centr�fuga pr�xima a voc�.");

	new i;
	switch(b_type) {
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}

	if (i == -1 && ShowMixingDialog(playerid, Part_Mixing)) return SendHelpMessage(playerid,"Selecione 3 reagentes dos quais voc� deseja criar uma droga. {FF6347} (eles n�o devem ser repetidos)");
	else if (Chemistry[i][IsWorking] == true) 				return SendErrorMessage(playerid, "A centr�fuga est� em funcionamento agora!");
	else if (Chemistry[i][IsWorking] == false && Chemistry[i][Amount] > 0.0)
	{
		if (GetPVarInt(playerid, "CarryingDrugCrate"))  return SendWarningMessage(playerid, "Suas m�os j� est�o ocupadas.");

		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
		SetPlayerAttachedObject(playerid, 9, 1575, 1, -0.039999, 0.414759, -0.042998, -31.400018, 92.117507, 112.900047);
		
		SetPlayerHoldingCrate(playerid,
			Chemistry[i][Type],
			Chemistry[i][Amount],
			Chemistry[i][Quality],
			Chemistry[i][Extra][0],
			Chemistry[i][Extra][1],
			Chemistry[i][Extra][2],
			Chemistry[i][Material]
		);
		
		SendHelpMessage(playerid, "Leve a mistura de reagente para o reator {ffc46a}/reator{FFFFFF}. Solte no ch�o -{ffc46a}/dropmix{FFFFFF}.");
		ClearFurnitureData(f, i);

		ApplyAnimation(playerid, "carry", "liftup", 1, 0, 1, 1, 0, 1000);
		//ApplyAnimation(playerid, "carry", "liftup", 1, 0, 1, 1, 0, 1000);
	}
	return 1;
}


CMD:secardrogas(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return SendErrorMessage(playerid, "Voc� n�o pode usar esse recurso no momento.");

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new f = GetClosestFurniture(playerid, 1.5, DRYER_OBJECT, DRUG_LAB);
    if (f == -1) return SendErrorMessage(playerid, "N�o h� planta de secagem pr�xima a voc�.");

	new i;
	switch(b_type) {
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}

	if (i == -1) 
	{
		for(new g = 0; g < MAX_INV_SLOTS; g++) P_CHEMICAL[playerid][g][pSlot] = 0;

		new pickDialog[MAX_INV_SLOTS*64 + 64] = "Selecione um item para secar:",
			count = 0;
		
		for(new g = 0; g < MAX_INV_SLOTS; g++)
		{
			if (Inventory[playerid][g][pInvItem] == 0)												continue;
			if (Inventory[playerid][g][pInvItem] != 116 && Inventory[playerid][g][pInvItem] != 115)	continue;

			format(gString, sizeof(gString), "\n%s (%%.1fg)", 
				E_CHEMICAL[Inventory[playerid][g][pInvItem]-103][NAME], 
				Inventory[playerid][g][pInvQFloat]
			);
			strcat(pickDialog, gString);
			
			P_CHEMICAL[playerid][count][pSlot] = g;
			count++;
		}

		if (count <= 0)
		{
			Dialog_Show(playerid, None, DIALOG_STYLE_LIST, "Planta de Secagem", "Voc� n�o tem folhas de papoula ou coca..", ">>", ""); //Falta de Material
			return 0;
		}
		else Dialog_Show(playerid, ConfDryer, DIALOG_STYLE_TABLIST_HEADERS, "Planta de Secagem", pickDialog, "Selecionar", "<<");
	} 
	else if (Chemistry[i][IsWorking] == true) return SendErrorMessage(playerid, "A secadora est� em funcionamento agora!");
	else if (Chemistry[i][IsWorking] == false && Chemistry[i][Amount] > 0.0)
	{
		switch(Chemistry[i][Material])
		{
			case 12: if (GiveFloatInvItem(playerid, 117, Chemistry[i][Amount]) == -1) return SendErrorMessage(playerid, "Invent�rio est� cheio.");
			case 13: if (GiveFloatInvItem(playerid, 118, Chemistry[i][Amount]) == -1) return SendErrorMessage(playerid, "Invent�rio est� cheio.");
		}
		SM2(playerid, "Adicionado ao seu invent�rio %s (%.1f).", E_CHEMICAL[Chemistry[i][Material]][NAME], Chemistry[i][Amount]);
		ClearFurnitureData(f, i);		
	}
	return 1;
}


CMD:reator(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return SendErrorMessage(playerid, "Voc� n�o pode usar esse recurso no momento.");

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Voc� deve estar no laborat�rio!");

	new f = GetClosestFurniture(playerid, 1.5, REACTOR_OBJECT, DRUG_LAB);
    if (f == -1) return SendErrorMessage(playerid, "N�o h� um reator pr�ximo a voc�.");
	
	if (GetClosestFurniture(playerid, 2.5, PUMP_OBJECT, DRUG_LAB) == -1) return SendErrorMessage(playerid, "Uma esta��o de bombeamento deve estar conectada ao reator.");
	
	new i;
	switch(b_type) {
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}
	if (i == -1) {
		if (!GetPVarInt(playerid, "CarryingDrugCrate")) return SendErrorMessage(playerid, "Voc� n�o est� segurando uma mistura de reagentes.");
		else {
			if ((i = GetFreeChemistyIndex()) == -1)	return SendErrorMessage(playerid, "Limite excedido! Entre em contato com o administrador t�cnico!");

			SendHelpMessage(playerid, "Para iniciar a s�ntese do medicamento, digite {ffc46a}/reator{FFFFFF}.");

			ReactDrug(i, GetPVarInt(playerid, "nativeCAS"), GetPVarFloat(playerid, "tdrug_Amount"), GetPVarFloat(playerid, "tdrug_Quality"),  GetPVarInt(playerid, "BreakingBad_1"),  GetPVarInt(playerid, "BreakingBad_2"),  GetPVarInt(playerid, "BreakingBad_3"), GetPVarInt(playerid, "tdrug_Type"), b_type, building, f);
			ResetDrugCrate(playerid);

			ApplyAnimation(playerid, "carry", "putdwn", 1, 0, 1, 1, 0, 1000);
			//ApplyAnimation(playerid, "carry", "putdwn", 1, 0, 1, 1, 0, 1000);
		}
	} else if (Chemistry[i][IsWorking] == true) {
		static str_size[] = "> VALOR ATUAL: %s%.0f 'C\n\n\
					{FFFFFF}Modos:\n\n\
					1.\t{8fbfd0} Resfriamento{FFFFFF}\t( x < 0 ) \n\
					2.\t{FF6347} Aquecimento{FFFFFF}\t( x > 0 )\n\n\
					{FFFFFF}Defina a temperatura (x) no sensor:";
		new mes[sizeof(str_size) + 6];

		format(mes, sizeof(mes), str_size, (Chemistry[i][Temperature] > 0) ? ("+") : (""), Chemistry[i][Temperature]);
		Dialog_Show(playerid, ControlSelect, DIALOG_STYLE_INPUT, "sensor de temperatura", mes, ">>>", "Cancelar");
	} else {
		if (Chemistry[i][Amount] == -1)	return SendErrorMessage(playerid, "N�o h� nada no reator."); 

		new title[32];
		format(title, sizeof(title), "%s", E_CHEMICAL[Chemistry[i][Material]][NAME]);
		format(gString, sizeof(gString), "{FFFFFF}Estrutura do elemento - %s\nOAS: {FFFF00}%.1f%%{FFFFFF}\n\n\tAn�lise: %.1f / g", E_CHEMICAL[Chemistry[i][Material]][CODE], Chemistry[i][Quality], Chemistry[i][Amount]);
				
		Dialog_Show(playerid, Purification, DIALOG_STYLE_MSGBOX, title, gString, ">>", "Cancelar");
	}
	return 1;
}

CMD:pegarmistura(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return SendErrorMessage(playerid, "Voc� n�o pode usar esse recurso no momento.");

	new dropid = MixDrop_Nearest(playerid);
	if (dropid == -1) 								return SendErrorMessage(playerid, "Nenhuma mistura de reagentes est� pr�xima a voc�.");
	if (GetPVarInt(playerid, #pDelay))				return 1;
	if (GetPVarInt(playerid, "CarryingDrugCrate"))  	return SendWarningMessage(playerid, "Suas m�os j� est�o ocupadas.");

	SetPlayerChatBubble(playerid, "* levanta a mistura de reagentes *", COLOR_PURPLE, Range_Normal, 10000);

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	SetPlayerAttachedObject(playerid, 9, 1575, 1, -0.039999, 0.414759, -0.042998, -31.400018, 92.117507, 112.900047);

	SetPlayerHoldingCrate(playerid,
		DropChemistry[dropid][Type],
		DropChemistry[dropid][Amount],
		DropChemistry[dropid][Quality],
		DropChemistry[dropid][Extra][0],
		DropChemistry[dropid][Extra][1],
		DropChemistry[dropid][Extra][2],
		DropChemistry[dropid][Material]
	);
		
	DropChemistry_Delete(dropid);
	
	ApplyAnimation(playerid, "carry", "liftup", 1, 0, 1, 1, 0, 1000);
	//ApplyAnimation(playerid, "carry", "liftup", 1, 0, 1, 1, 0, 1000);
	return 1;
}

CMD:largarmistura(playerid, params[])
{
	if (GetPVarInt(playerid, "CarryingDrugCrate") == 0) return SendErrorMessage(playerid, "Voc� n�o tem uma mistura de reagentes em suas m�os!"); 

	for(new i; i != MAX_CHEM_DROPS; i++)
	{
	    if (DropChemistry[i][temp_ID] != 0) continue;

		DropChemistry[i][Material] = GetPVarInt(playerid, "nativeCAS");
		DropChemistry[i][Type] = GetPVarInt(playerid, "tdrug_Type");
		DropChemistry[i][Amount] = GetPVarFloat(playerid, "tdrug_Amount");
		DropChemistry[i][Quality] = GetPVarFloat(playerid, "tdrug_Quality");
		DropChemistry[i][Extra][0] = GetPVarInt(playerid, "BreakingBad_1");
		DropChemistry[i][Extra][1] = GetPVarInt(playerid, "BreakingBad_2");
		DropChemistry[i][Extra][2] = GetPVarInt(playerid, "BreakingBad_3");
		DropChemistry[i][dropInt] = GetInterior(playerid);
		DropChemistry[i][dropVW] = GetWorld(playerid);
		DropChemistry[i][dropTime] = gettime();

		GetXYInFrontOfPlayer(playerid, DropChemistry[i][dropPosX], DropChemistry[i][dropPosY], DropChemistry[i][dropPosZ], 1.5);
		DropChemistry[i][dropPosZ] -= 1.0;
		DropChemistry[i][dropPosRZ] = 90.0;

		new query[350];
		mysql_format(dbHandle, query, sizeof(query), 
			"INSERT INTO `drop_chemistry` (`Material`,`Amount`,`Quality`,`Extra0`,`Extra1`,`Extra2`,`dropPosX`,`dropPosY`,`dropPosZ`,`dropPosRX`,`dropPosRY`,`dropPosRZ`,`dropInt`,`dropVW`,`dropTime`) VALUES (%i,%f,%f,%i,%i,%i,%f,%f,%f,%f,%f,%f,%i,%i,%i)",
			DropChemistry[i][Material],
			DropChemistry[i][Amount],
			DropChemistry[i][Quality],
			DropChemistry[i][Extra][0],
			DropChemistry[i][Extra][1],
			DropChemistry[i][Extra][2],
			DropChemistry[i][dropPosX], DropChemistry[i][dropPosY], DropChemistry[i][dropPosZ],
			DropChemistry[i][dropPosRX], DropChemistry[i][dropPosRY], DropChemistry[i][dropPosRZ],
			DropChemistry[i][dropInt],
			DropChemistry[i][dropVW],
			DropChemistry[i][dropTime]
		);
		mysql_tquery(dbHandle, query, "GetIDforChemistryDrop", "d", i);

		SetPlayerChatBubble(playerid, "* joga fora uma mistura de reagentes *", COLOR_PURPLE, 20.0, 4000);
		SendSyntaxMessage(playerid, "Voc� jogou fora a mistura de reagentes. Digite {ffc46a}/pickupmix{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.");

		DropChemistry[i][dropObject] = CreateDynamicObject(1575, DropChemistry[i][dropPosX], DropChemistry[i][dropPosY], DropChemistry[i][dropPosZ], 93.7, 120.0, 120.0, GetWorld(playerid), GetInterior(playerid));
		EditDynamicObject(playerid, DropChemistry[i][dropObject]);
		SetPVarInt(playerid, #edit_drug, i+1);

		ResetDrugCrate(playerid);
		break;
	}
	return 1;
}