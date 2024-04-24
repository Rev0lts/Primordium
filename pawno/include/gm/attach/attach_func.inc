forward OnPlayerLoadAttach(playerid);
forward OnAttachInsert(playerid, slot, model, bone, name[]);

public OnPlayerLoadAttach(playerid)
{
	for(new i = 0; i < MAX_PLAYER_ATTACH; i++)
	{
		AttachInfo[playerid][i][aID] = 0;
		AttachInfo[playerid][i][aUsed] = 0;
	}

	if (!cache_num_rows()) return 1;

    for(new i = 0; i < cache_num_rows(); i++)
	{
     	if (i > MAX_PLAYER_ATTACH) break;

	    AttachInfo[playerid][i][aID] = cache_get_field_content_int(i, "id", dbHandle);
	    cache_get_field_content(i, "name", AttachInfo[playerid][i][aName], dbHandle, 32);
		AttachInfo[playerid][i][aModel] = cache_get_field_content_int(i, "model", dbHandle);
		AttachInfo[playerid][i][aBone] = cache_get_field_content_int(i, "bone", dbHandle);
		AttachInfo[playerid][i][aOffsetX] = cache_get_field_content_float(i, "offestx", dbHandle);
		AttachInfo[playerid][i][aOffsetY] = cache_get_field_content_float(i, "offesty", dbHandle);
		AttachInfo[playerid][i][aOffsetZ] = cache_get_field_content_float(i, "offestz", dbHandle);
		AttachInfo[playerid][i][aRotX] = cache_get_field_content_float(i, "rotx", dbHandle);
		AttachInfo[playerid][i][aRotY] = cache_get_field_content_float(i, "roty", dbHandle);
		AttachInfo[playerid][i][aRotZ] = cache_get_field_content_float(i, "rotz", dbHandle);
		AttachInfo[playerid][i][aScaleX] = cache_get_field_content_float(i, "scalex", dbHandle);
		AttachInfo[playerid][i][aScaleY] = cache_get_field_content_float(i, "scaley", dbHandle);
		AttachInfo[playerid][i][aScaleZ] = cache_get_field_content_float(i, "scalez", dbHandle);
		AttachInfo[playerid][i][aUsed] = cache_get_field_content_int(i, "used", dbHandle);
	}
	return 1;
}

public OnAttachInsert(playerid, slot, model, bone, name[])
{
	AttachInfo[playerid][slot][aID] = cache_insert_id();
	AttachInfo[playerid][slot][aModel] = model;
	AttachInfo[playerid][slot][aBone] = bone;
	format(AttachInfo[playerid][slot][aName], 128, "%s", name);
	AttachInfo[playerid][slot][aUsed] = 1;
	return 1;
}

InsertPlayerAttach(playerid, slot, model, bone, name[])
{
    new query[128 + 128];
	mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `player_attach` (userid, name, model, bone) VALUES (%i, '%s', %i, %i)", PlayerInfo[playerid][pID], name, model, bone);
	mysql_tquery(dbHandle, query, "OnAttachInsert", "iiiis", playerid, slot, model, bone, name);
	return 1;
}

UpdatePlayerAttach(playerid, slot, Float:X, Float:Y, Float:Z, Float:RX, Float:RY, Float:RZ, Float:SC, Float:SY, Float:SZ, bone)
{
    new query[128 + 128];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `player_attach` SET `offestx`=%f,`offesty`=%f,`offestz`=%f,`rotx`=%f,`roty`=%f,`rotz`=%f,`scalex`=%f,`scaley`=%f,`scalez`=%f,`used`=%i,`bone`=%i WHERE `id` = %i", X, Y, Z, RX, RY, RZ, SC, SY, SZ, AttachInfo[playerid][slot][aUsed], bone, AttachInfo[playerid][slot][aID]);
	mysql_tquery(dbHandle, query);
	return 1;
}

LoadPlayerAttach(playerid)
{
    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `player_attach` WHERE `userid` = %i", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query, "OnPlayerLoadAttach", "i", playerid);
	return 1;
}

RemovePlayerAttachFromSlot(playerid, slot)
{
    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `player_attach` WHERE `id` = %i", AttachInfo[playerid][slot][aID]);
	mysql_tquery(dbHandle, query);

	AttachInfo[playerid][slot][aID] = 0;
	AttachInfo[playerid][slot][aName] = EOS;
	AttachInfo[playerid][slot][aModel] = 0;
	AttachInfo[playerid][slot][aBone] = 0;
	AttachInfo[playerid][slot][aOffsetX] = 0.0;
	AttachInfo[playerid][slot][aOffsetY] = 0.0;
	AttachInfo[playerid][slot][aOffsetZ] = 0.0;
	AttachInfo[playerid][slot][aRotX] = 0.0;
	AttachInfo[playerid][slot][aRotY] = 0.0;
	AttachInfo[playerid][slot][aRotZ] = 0.0;
	AttachInfo[playerid][slot][aScaleX] = 0.0;
	AttachInfo[playerid][slot][aScaleY] = 0.0;
	AttachInfo[playerid][slot][aScaleZ] = 0.0;
	AttachInfo[playerid][slot][aUsed] = 0;

	if (IsPlayerAttachedObjectSlotUsed(playerid, slot))	RemovePlayerAttachedObject(playerid, slot);
	return 1;
}

Save_AttachWeapon(playerid, i)
{
	new query[256];
	
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `weap_%i_bone` = %i, `weap_%i_x` = %f, `weap_%i_y` = %f, `weap_%i_z` = %f, `weap_%i_rx` = %f, `weap_%i_ry` = %f, `weap_%i_rz` = %f WHERE `id` = %i",
	i, PlayerInfo[playerid][pWeaponBone][i], i, PlayerInfo[playerid][pWeaponX][i], i, PlayerInfo[playerid][pWeaponY][i], i, PlayerInfo[playerid][pWeaponZ][i], i, PlayerInfo[playerid][pWeaponRX][i], i, PlayerInfo[playerid][pWeaponRY][i],
	i, PlayerInfo[playerid][pWeaponRZ][i], PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query);
	return 1;
}

GetNextToySlot(playerid)
{
	for(new i = 0; i < MAX_PLAYER_ATTACH; i++)
	{
		if (AttachInfo[playerid][i][aID] == 0) return i;
	}
	return -1;
}

Dialog:MODEL_SELECTION_FA(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionLegal(faction))	return SendErrorMessage(playerid, "Você deve ser um policial / guarda de prisão / paramédico / governo.");

	Dialog_Show(playerid, SelectBone, DIALOG_STYLE_LIST, "Parte do corpo","1: Costas\n2: Cabeça\n3: Ombro esquerdo\n4: Ombro direito\n5: Mão esquerda\n6:Braço direito\n7: Coxa esquerda\n8: Coxa direita\n9: Perna esquerda\n10: Perna direita\n11: Panturrilha direita\n12: Panturrilha esquerda\n13: Antebraço esquerdo\n14: Antebraço direito\n15: Clavícula esquerda\n16: Clavícula direita\n17: Pescoço\n18: Mandíbula","Selecionar","Cancelar");
	SetPVarInt(playerid, #select_attach, listitem);
	return 1;
}

Dialog:MODEL_SELECTION_OBJATT(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new 
		vehicle = GetPVarInt(playerid, #veh_editor), 
		carslot = GetPVarInt(playerid, #veh_slot), 
		attobj = car_attach[listitem],

		Float:x, 
		Float:y, 
		Float:z
	;

	GetVehiclePos(vehicle, x, y, z);
	VehicleInfo[vehicle][carAttach][carslot] = CreateDynamicObject(attobj, x, y, z+2, 0.0, 0.0, 0.0);
	
	SetPVarInt(playerid, #veh_obj, attobj);
	SetPVarInt(playerid, "Vehicle:AttEdit", 0);

	EditDynamicObject(playerid, VehicleInfo[vehicle][carAttach][carslot]);
		
	if (IsPlayerInAnyVehicle(playerid))	InfoMSG(playerid, "Você pode usar \"~y~W~w~\"~n~para mover a câmera ao redor", 5);
	else								InfoMSG(playerid, "Você pode usar \"~y~ESPAÇO~w~\",~n~para mover a câmera ao redor", 5);
		
	return 1;
}