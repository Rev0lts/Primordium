PP_OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (clickedid == PP_Framework[2])
	{
	    new hour, minute, second, str[64];
		gettime(hour, minute, second);
		format(str, sizeof(str), "%02d:%02d", hour, minute);

	    PlayerTextDrawSetString(playerid, PP_Btn[playerid][0], str);
  		pTemp[playerid][pPayphoneNumStr] = EOS;
		
		//PlayerTextDrawHide(playerid, PP_Btn[playerid][0]);
		//PlayerTextDrawShow(playerid, PP_Btn[playerid][0]);
  		return 1;
	}
	if (clickedid == PP_Framework[8])
	{		
		return cancelPayphone(playerid);
	}
	if (clickedid == PP_Framework[7])
	{
	    if (GetCash(playerid) < 2) return SendErrorMessage(playerid, "Dinheiro insuficiente, você deve ter $2 em mãos. ");
		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
	    	return SendCellMessage(playerid, "Você possui uma chamada não concluída. Digite '/finalizar' para desligar.");

	    PlayerTextDrawSetString(playerid, PP_Btn[playerid][0], "Ligar ..");
		
		cmd::server_ame(playerid, "jogou uma moeda em um orelhão.");
		GiveCash(playerid,-2);
		GameTextForPlayer(playerid, "~r~-$2.", 3000, 1);

		CallNumber(playerid, pTemp[playerid][pPayphoneNumStr], 1);
		pTemp[playerid][pPayphoneNumStr] = EOS;
	    return 1;
	}
	return 1;
}
PP_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	new qustr[128];

	for(new i = 1; i < 10; i++)
	{
		if (playertextid == PP_Btn[playerid][i])
		{
			if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
				return SendCellMessage(playerid, "Você possui uma chamada não concluída. Digite '/finalizar' para desligar.");

			if (strlen(pTemp[playerid][pPayphoneNumStr]) >= 8) return 1;
			format(pTemp[playerid][pPayphoneNumStr], 64, "%s%d", pTemp[playerid][pPayphoneNumStr], i);
			format(qustr, sizeof(qustr), "%s", pTemp[playerid][pPayphoneNumStr]);
			PlayerTextDrawSetString(playerid, PP_Btn[playerid][0], qustr);

			//PlayerTextDrawHide(playerid, PP_Btn[playerid][0]);
			//PlayerTextDrawShow(playerid, PP_Btn[playerid][0]);
		}
	}
	if (playertextid == PP_Btn[playerid][10])
	{
		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
			return SendCellMessage(playerid, "Você possui uma chamada não concluída. Digite '/finalizar' para desligar.");

		if (strlen(pTemp[playerid][pPayphoneNumStr]) >= 8) return 1;
		format(pTemp[playerid][pPayphoneNumStr], 64, "%s0", pTemp[playerid][pPayphoneNumStr]);
		format(qustr, sizeof(qustr), "%s", pTemp[playerid][pPayphoneNumStr]);
		PlayerTextDrawSetString(playerid, PP_Btn[playerid][0], qustr);

		//PlayerTextDrawHide(playerid, PP_Btn[playerid][0]);
		//PlayerTextDrawShow(playerid, PP_Btn[playerid][0]); // íå îá
	}
	return 1;
}

PP_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rz)
{
    if (GetPVarInt(playerid, #edit_pp) != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
    {
        new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;

        GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
        GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

        new id = GetPVarInt(playerid, #edit_pp)-1;

        DeletePVar(playerid, #edit_pp);
        if (id < 0 || id >= MAX_PAYPHONES || !PayphoneInfo[id][phID]) 	return SendErrorMessage(playerid, "Telefone público não encontrado.");
        if (objectid != PayphoneInfo[id][phObj])						return SendErrorMessage(playerid, "Erro ao editar o telefone público");

        SetDynamicObjectPos(objectid, x, y, z);
        SetDynamicObjectRot(objectid, 0.0, 0.0, rz);

        GetDynamicObjectPos(objectid, PayphoneInfo[id][phX], PayphoneInfo[id][phY], PayphoneInfo[id][phZ]);
        PayphoneInfo[id][phRZ] = rz;

        Update_Payphone(id);

        new query[128];
        mysql_format(dbHandle, query, sizeof(query), "UPDATE `payphones` SET `x`=%f,`y`=%f,`z`=%f,`rz`=%f WHERE `id`=%i", x, y, z, rz, PayphoneInfo[id][phID]);
        mysql_tquery(dbHandle, query);

        SendAdmMessageToPlayer(playerid, "Posição do telefone público [ID: %i] alterada.", id);
    }
    return 1;
}
