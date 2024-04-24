// Comando para o menu de anexos
CMD:attmenu(playerid, params[])
{
    new option[24], secoption, thirdoption, fourthoption;

    if (sscanf(params, "s[24]D(-1)D(-1)D(-1)", option, secoption, thirdoption, fourthoption))
    {
        SendHelpMessage(playerid, "/attmenu [opções]");
        SendSyntaxMessage(playerid, "listar / comprar / vender / editar");
        return 1;
    }

    new vehicle = GetPlayerVehicleID(playerid);

    if (!vehicle || !IsDriver(playerid)) return SendErrorMessage(playerid, "Você deve estar no banco do motorista.");
    if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Você não é o proprietário deste veículo.");
        
    if (CompareStrings(option, "comprar"))
    {
        new slot, mes[sizeof(car_attach)*64];

        if (sscanf(params, "{s[7]}d", slot)) return SendHelpMessage(playerid, "/attmenu comprar [ID slots]");
        if (VehicleInfo[vehicle][carAttachObj][slot] != 0) return SendErrorMessage(playerid, "Este slot já está ocupado.");
            
        SetPVarInt(playerid, #veh_editor, vehicle);
        SetPVarInt(playerid, #veh_slot, slot);

        for(new i = 0; i < sizeof(car_attach); i++)
        {
            format(mes, sizeof(mes), "%s%i(0.0, 0.0, 90.0, 1.0)\t~g~~h~$250\n", mes, car_attach[i]);
        }

        Dialog_Show(playerid, MODEL_SELECTION_OBJATT, DIALOG_STYLE_PREVIEW_MODEL, "Lista de anexos de veículos", mes, "Selecionar", "Fechar");        
        return 1;
    }
    else if (CompareStrings(option, "vender"))
    {
        new slot;

        if (sscanf(params, "{s[7]}d", slot)) return SendHelpMessage(playerid, "/attmenu vender [ID slots]");
        if (VehicleInfo[vehicle][carAttach][slot] == INVALID_OBJECT_ID) return SendErrorMessage(playerid, "Este slot está vazio.");
            
        VehicleInfo[vehicle][carAttachObj][slot] = 0;
        VehicleInfo[vehicle][carAttachPosX][slot] = 0.0;
        VehicleInfo[vehicle][carAttachPosY][slot] = 0.0;
        VehicleInfo[vehicle][carAttachPosZ][slot] = 0.0;
        VehicleInfo[vehicle][carAttachRotX][slot] = 0.0;
        VehicleInfo[vehicle][carAttachRotY][slot] = 0.0;
        VehicleInfo[vehicle][carAttachRotZ][slot] = 0.0;
        DestroyDynamicObject(VehicleInfo[vehicle][carAttach][slot]);
        VehicleInfo[vehicle][carAttach][slot] = INVALID_OBJECT_ID;
        SendSyntaxMessage(playerid, "Você excluiu com sucesso o item.");
        Save_CarAttach(vehicle, slot);
        return 1;
    }
    else if (CompareStrings(option, "editar"))
    {
        new slot;

        if (sscanf(params, "{s[7]}d", slot)) return SendHelpMessage(playerid, "/attmenu editar [ID slots]");
        if (VehicleInfo[vehicle][carAttach][slot] == INVALID_OBJECT_ID) return SendErrorMessage(playerid, "Este slot está vazio.");

        new Float:x, Float:y, Float:z;
        AntiCheatGetVehiclePos(vehicle, x, y, z);

        SetPVarInt(playerid, #veh_editor, vehicle);
        SetPVarInt(playerid, #veh_slot, slot);
        SetPVarInt(playerid, #veh_obj, VehicleInfo[vehicle][carAttachObj][slot]);
        SetPVarInt(playerid, "Veículo:AttEdit", 1);
        
        DestroyDynamicObject(VehicleInfo[vehicle][carAttach][slot]);
        VehicleInfo[vehicle][carAttach][slot] = INVALID_OBJECT_ID;
        VehicleInfo[vehicle][carAttach][slot] = CreateDynamicObject(VehicleInfo[vehicle][carAttachObj][slot], x, y, z + 2.0, 0.0, 0.0, 0.0);
        EditDynamicObject(playerid, VehicleInfo[vehicle][carAttach][slot]);
        return 1;
    }
    else if (CompareStrings(option, "listar"))
    {
        new message[1024] = "{FFFFFF}Slot\t{FFFFFF}ID do Objeto";

        for(new i = 0; i < MAX_TRUNK_INVENTORY; i++)
        {
            if (VehicleInfo[vehicle][carAttach][i] == INVALID_OBJECT_ID)
            {
                format(message, sizeof(message), "%s\n%i\tNenhum", message, i);
            }
            else
            {
                format(message, sizeof(message), "%s\n%i\t%d", message, i, VehicleInfo[vehicle][carAttachObj][i]);
            }
        }
        Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Lista de anexos", message, "Fechar", "");
    }
    else
    {
        SendHelpMessage(playerid, "/attmenu [opções]");
        SendSyntaxMessage(playerid, "listar / comprar / vender / editar");
    }
    return 1;
}


ALTX:clothing("/itens");
CMD:clothing(playerid, params[])
{
    if (GetPVarInt(playerid, #edit_attach) == 1 || GetPVarInt(playerid, #buy_attach) != 0)	return SendErrorMessage(playerid, "Você precisa terminar de comprar/editar o item.");

	new count, string[750];

	for(new i; i < MAX_PLAYER_ATTACH; i++)
	{
	    if (AttachInfo[playerid][i][aID] == 0) continue;
	    
		format(string, sizeof(string), "%s\n{FFFFFF}%i: %s %s", string, i+1, AttachInfo[playerid][i][aName], (AttachInfo[playerid][i][aUsed]) ? ("{30A90F}[Usando]") : ("{AFAFAF}[Guardado]"));
	    count++;
	}

	if (!count)	return SendErrorMessage(playerid, "Você não tem itens.");

    Dialog_Show(playerid, Clothing, DIALOG_STYLE_LIST, "{FF8000}Selecione um slot", string, "Selecionar", "Fechar");
	return 1;
}

CMD:coh(playerid, params[])
{
    if (GetPVarInt(playerid, #money_hand) && (
		IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) ||
		IsPlayerAttachedObjectSlotUsed(playerid, 8)
	))
    {
        SetPVarInt(playerid, #money_hand, 0);
		
 		RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
		RemovePlayerAttachedObject(playerid, 8);
 		return 1;
    }

	if (GetCash(playerid) < 300)								return SendNotCash(playerid);
    if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	return SendWarningMessage(playerid,"Suas mãos estão ocupada agora.");

	new type;
	if (sscanf(params, "d", type)) return SendHelpMessage(playerid, "/coh [1 - mão direita | 2 - mão esquerda]");

	if (type == 1)		SetPlayerAttachedObject(playerid, ATTACH_HANDS, 1212, 6, 0.062999, 0.004999, 0.013000, -106.300003, -3.699995, -84.700004, 0.766000, 1.000000, 0.773000);
	else if (type == 2)	SetPlayerAttachedObject(playerid, 8, 1212, 5, 0.083999, -0.005000, -0.015000, -106.300003, -3.699995, -84.700004, 0.766000, 1.000000, 0.829999);
	else 				return SendHelpMessage(playerid, "/coh [1 - mão direita | 2 - mão esquerda]");
	
    SetPVarInt(playerid, #money_hand, 1);
	return 1;
}