Dialog:CITYGUN(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::duty(playerid, "");
    if (!IsPlayerFactionCity(playerid)) return 1;
    if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro você precisa entrar de serviço.");
	if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid)) return SendErrorMessage(playerid, "Você não está no vestiário.");
		
	GameTextForPlayer(playerid, "~r~-", 1500, 4);

	switch(listitem)
	{
		case 0: if (GiveInvItem(playerid, INV_RADIO, 1) == -1) return SendErrorMessage(playerid, "Inventário cheio!");
		case 1: if (!GivePlayerWeaponEx(playerid, 24, 85, GetDefaultAmmoType(24), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventário cheio!");
		case 2: if (GiveInvItem(playerid, 85, CheckAmmo(85)) == -1) return SendErrorMessage(playerid, "Inventário cheio!");
		case 3: if (!GivePlayerWeaponEx(playerid, 29, 91, GetDefaultAmmoType(29), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventário cheio!");
		case 4: if (GiveInvItem(playerid, 91, CheckAmmo(91)) == -1) return SendErrorMessage(playerid, "Inventário cheio!");
		case 5: if (!GivePlayerWeaponEx(playerid, 31, 98, GetDefaultAmmoType(31), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventário cheio!");
		case 6: if (GiveInvItem(playerid, 98, CheckAmmo(98)) == -1) return SendErrorMessage(playerid, "Inventário cheio!");
		case 7: if (GiveInvItem(playerid, 99, CheckAmmo(99)) == -1) return SendErrorMessage(playerid, "Inventário cheio!");
		case 8: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~g~+", 1500, 4);

	Dialog_Show(playerid, CITYGUN, DIALOG_STYLE_LIST, "Armas", "• Walkie-talkie\n• Desert Eagle ( .50AE .ext )\n\t{ffc46a}- Pegar munição extra (.50AE .ext)\n{FFFFFF} • MP5 (9x19mm .ext)\n\t{ffc46a} - Pegar munição extra (9x19mm .ext)\n{FFFFFF} • M4 (5.56x45mm (tambor))\n\t{ffc46a} - Pegar munição extra (5.56x45mm (tambor))\n\t{ffc46a} - Pegar munição não letal (5.56x45mm (borracha))\n{FF6347} • Entregar todas as armas", "Selecionar", "Fechar");
 	return 1;
}
