Dialog:DOCSGUN(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::duty(playerid, "");
    if (!IsPlayerFactionDocs(playerid)) return 1;
    if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro, voc� deve estar de servi�o.");
    if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid)) return SendErrorMessage(playerid, "Voc� n�o est� no vesti�rio.");

    GameTextForPlayer(playerid, "~r~-", 1500, 4);

    switch(listitem)
    {
        case 0: if (GiveInvItem(playerid, INV_RADIO, 1) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // r�dio
        case 1: if (!GivePlayerWeaponEx(playerid, 3, 1, GetDefaultAmmoType(3), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 2: if (!GivePlayerWeaponEx(playerid, 41, 999, GetDefaultAmmoType(41), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 3: if (!GivePlayerWeaponEx(playerid, 24, 85, GetDefaultAmmoType(24), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 4: if (GiveInvItem(playerid, 85, CheckAmmo(85)) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // muni��o desert eagle 85
        case 5: if (!GivePlayerWeaponEx(playerid, 29, 91, GetDefaultAmmoType(29), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 6: if (GiveInvItem(playerid, 91, CheckAmmo(91)) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // muni��o MP5 91
        case 7: if (!GivePlayerWeaponEx(playerid, 25, 89, GetDefaultAmmoType(25), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 8: if (GiveInvItem(playerid, 86, CheckAmmo(86)) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // muni��o shotgun 86
        case 9: if (GiveInvItem(playerid, 89, CheckAmmo(89)) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // muni��o shotgun n�o letal 89
        case 10: if (!GivePlayerWeaponEx(playerid, 31, 98, GetDefaultAmmoType(31), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 11: if (GiveInvItem(playerid, 98, CheckAmmo(98)) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // muni��o M4 98
        case 12: if (GiveInvItem(playerid, 99, CheckAmmo(99)) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // muni��o M4 n�o letal 99
        case 13: if (!GivePlayerWeaponEx(playerid, 34, 96, GetDefaultAmmoType(34), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 14: if (GiveInvItem(playerid, 96, CheckAmmo(96)) == -1) return SendErrorMessage(playerid, "Invent�rio cheio!"); // muni��o Sniper Rifle 96
        case 15: if (!GivePlayerWeaponEx(playerid, 17, 1, GetDefaultAmmoType(17), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 16: if (!GivePlayerWeaponEx(playerid, 43, 50, GetDefaultAmmoType(43), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Invent�rio cheio!");
        case 17: ResetPlayerWeaponsEx(playerid);
    }

    GameTextForPlayer(playerid, "~g~+", 1500, 4);

    Dialog_Show(playerid, DOCSGUN, DIALOG_STYLE_LIST, "Armas", "{FFFFFF}� Cassetete\n� Spray de pimenta\n� Desert Eagle (.50AE .ext)\n\t{ffc46a}- Pegar muni��o extra (.50AE .ext)\n{FFFFFF}� MP5 (9x19mm .ext)\n\t{ffc46a}- Pegar muni��o extra (9x19mm .ext)\n{FFFFFF}� Shotgun (12-G BIRDSHOT)\n\t{ffc46a}- Pegar muni��o extra (12-G BIRDSHOT)\n\t{ffc46a}- Pegar muni��o n�o letal (12-G BEANBAG)\n{FFFFFF}� M4 (5.56x45mm (drum))\n\t{ffc46a}- Pegar muni��o extra (5.56x45mm (drum))\n\t{ffc46a}- Pegar muni��o n�o letal (5.56x45mm (rubber))\n{FFFFFF}� Rifle de Precis�o (7.62x51mm)\n\t{ffc46a}- Pegar muni��o extra (7.62x51mm)\n{FFFFFF}� Granada de g�s\n� C�mera\n{FF6347}� Devolver todas as armas", "Selecionar", "Fechar");
    return 1;
}