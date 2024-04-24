Dialog:FDArmoury(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::duty(playerid, "");
    if (!IsPlayerFactionMedic(playerid) || !IsPlayerFactionFire(playerid)) return 1;
    if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro voc� precisa entrar de servi�o.");
    if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid)) return SendErrorMessage(playerid, "Voc� n�o est� no vesti�rio / no ve�culo oficial.");
	        
	switch(listitem)
	{
		case 0: GivePlayerWeaponEx(playerid, 9, 1);
		case 1: GivePlayerWeaponEx(playerid, 42, 999);
		case 2: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~r~-", 1500, 4);

	switch(listitem)
	{
		case 0: if (GiveInvItem(playerid, INV_RADIO, 1) == -1) return SendErrorMessage(playerid, "Invent�rio est� cheio!");
		case 1: if (!GivePlayerWeaponEx(playerid, 9, 1, GetDefaultAmmoType(9), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio est� cheio!");
		case 2: if (!GivePlayerWeaponEx(playerid, 42, 999, GetDefaultAmmoType(42), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio est� cheio!");
		case 3: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~g~+", 1500, 4);
	
	Dialog_Show(playerid, FDArmoury, DIALOG_STYLE_LIST, "Equipamento", "� Walkie-talkie\n� Serra el�trica\n� Extintor de inc�ndio\n� Devolver todo o equipamento", "Selecionar", "Retornar");
 	return 1;
}