Faction_EditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
    new Float:oldX, Float:oldY, Float:oldZ,
	Float:oldRotX, Float:oldRotY, Float:oldRotZ;

    GetDynamicObjectPos(modelid, oldX, oldY, oldZ);
	GetDynamicObjectRot(modelid, oldRotX, oldRotY, oldRotZ);

    if (GetPVarInt(playerid, #edit_faction) == 1 && (response || !response))
	{
	    DeletePVar(playerid, #edit_faction);
		RemovePlayerAttachedObject(playerid, index);

        AttachInfo[playerid][index][aBone] = boneid;
		AttachInfo[playerid][index][aOffsetX] = fOffsetX;
		AttachInfo[playerid][index][aOffsetY] = fOffsetY;
		AttachInfo[playerid][index][aOffsetZ] = fOffsetZ;
		AttachInfo[playerid][index][aRotX] = fRotX;
		AttachInfo[playerid][index][aRotY] = fRotY;
		AttachInfo[playerid][index][aRotZ] = fRotZ;
		AttachInfo[playerid][index][aScaleX] = fScaleX;
		AttachInfo[playerid][index][aScaleY] = fScaleY;
		AttachInfo[playerid][index][aScaleZ] = fScaleZ;
		UpdatePlayerAttach(playerid, index, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, boneid);

	   	AttachInfo[playerid][index][aUsed] = 1;
		SetPlayerAttachedObject(playerid, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);

		SendSyntaxMessage(playerid, "Use /itens para editar objetos anexados.");
		return 1;
	}
    return 1;
}

Factions_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT * FROM `factions` ORDER BY `factionid`", "LoadDynamicFactions");
}

Fact_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	new Float:oldX, Float:oldY, Float:oldZ,
	Float:oldRotX, Float:oldRotY, Float:oldRotZ;

    GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
	GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

	if (pTemp[playerid][pEditBort] != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
	{
	    new bort = pTemp[playerid][pEditBort]-1;
	    pTemp[playerid][pEditBort] = 0;

	    if (BortInfo[playerid][bort][bortOn] == 0 || objectid != BortInfo[playerid][bort][bortObject])	
			return SendErrorMessage(playerid, "Você está editando o item errado.");

	    SetDynamicObjectPos(objectid, x, y, z);
	    SetDynamicObjectRot(objectid, rx, ry, rz);

	    SendSyntaxMessage(playerid, "Posição '%s' alterada com sucesso.", BortInfo[playerid][bort][bortName]);
	}
	return 1;
}