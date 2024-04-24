ALTX:dropmoney("/dmoney","/dropardinheiro","/soltardinheiro");
CMD:dropmoney(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
        return SendErrorMessage(playerid, "Você não pode usar essa função agora.");
    if (CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendErrorMessage(playerid, "Primeiro saia do modo administrador.");
    if (IsPlayerInAnyVehicle(playerid))
        return SendErrorMessage(playerid, "Você não deve estar em um veículo.");
    if (PlayerInfo[playerid][pHours] < 2)
        return SendErrorMessage(playerid, "Você jogou menos de 2 horas no servidor. Essa função não está disponível.");

    new amount;

    if (sscanf(params, "i", amount))
        return SendHelpMessage(playerid, "/soltardinheiro [quantidade]");
    if (amount < 1 || amount > 3000)
        return SendErrorMessage(playerid, "Você inseriu uma quantidade incorreta ($1 - $3000).");
    if (amount > GetCash(playerid))
        return SendNotCash(playerid);
    if (ItemDrop_Drop(playerid, 9999, amount) == -1)
        return SendErrorMessage(playerid, "O número máximo de itens descartados foi atingido.");
    SendAdmMessage("%s [ID: %i] jogou $%i no chão.", GetNameEx(playerid), playerid, amount);
    GiveCash(playerid, -amount);

    SendSyntaxMessage(playerid, "Você jogou $%i no chão. Digite {ffc46a}/pegaritem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", amount);
    return 1;
}

ALTX:drop("/soltar","/dropar");
CMD:drop(playerid, params[])
{
    if (!GetPVarInt(playerid, #attach_item))
        return 1;
    
    if (ItemDrop_Drop(playerid, pTemp[playerid][tempItem], pTemp[playerid][tempQ], pTemp[playerid][tempEx]) == -1)
        return SendErrorMessage(playerid, "O número máximo de itens descartados foi atingido.");

    SendSyntaxMessage(playerid, "Você largou %s. Digite {ffc46a}/pitem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", inventory_items[pTemp[playerid][tempItem]][itemName], pTemp[playerid][tempQ]);

    RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    SetPVarInt(playerid, #attach_item, 0);

    pTemp[playerid][tempItem] = 0;
    pTemp[playerid][tempEx] = 0;
    pTemp[playerid][tempQ] = 0;
    return 1;
}

ALTX:dropgun("/dgun","/dropararma","/soltararma");
CMD:dropgun(playerid, params[])
{
    if (PlayerInfo[playerid][pPlayerWeapon] == 0)
        return SendErrorMessage(playerid, "Você não possui armas nas mãos.");
    
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])
        return SendErrorMessage(playerid, "Você não pode usar essa funcionalidade no momento.");
    
    if (CheckAdmin(playerid, ADMIN_LEVEL_1) || IsTester(playerid))
        return SendErrorMessage(playerid, "Primeiro saia do modo administrativo.");
    
    if (PlayerInfo[playerid][pOnDuty])
        return SendErrorMessage(playerid, "Esta função não está disponível enquanto estiver de serviço.");
    
    if (IsPlayerInAnyVehicle(playerid))
        return SendErrorMessage(playerid, "Você não deve estar em um veículo.");
    
    new wep = CheckPlayerWeaponEx(playerid);
    if (!wep)
        return SendAdmMessage("O jogador %s [ID:%i] é suspeito de usar cheats em armas!", GetNameEx(playerid), playerid);
    
    if (wep >= 22 && wep <= 34)
    {
        new am = CheckAmmo(PlayerInfo[playerid][pAmmoType]);
        if (PlayerInfo[playerid][pPlayerAmmo] > am)
            PlayerInfo[playerid][pPlayerAmmo] = am;
    }
    
    if (ItemDrop_Drop(playerid, PlayerInfo[playerid][pPlayerWeapon], PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]) == -1)
        return SendErrorMessage(playerid, "O número máximo de itens descartados foi atingido.");
    
    if (PlayerInfo[playerid][pPlayerAmmo])
        SM2(playerid, "Você largou %s [%s / %i rodadas]. Digite {ffc46a}/pitem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);
    else
        SM2(playerid, "Você largou %s. Digite {ffc46a}/pitem{FFFFFF} ou sente-se e pressione {ffc46a}H{FFFFFF} para pegar.", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName]);
    
    if (PlayerInfo[playerid][pPlayerAmmo])
        ResetPlayerWeapons(playerid);
    else
        RemovePlayerAttachedObject(playerid, 9);
    
    PlayerInfo[playerid][pPlayerWeapon] = 0;
    PlayerInfo[playerid][pPlayerAmmo] = 0;
    PlayerInfo[playerid][pAmmoType] = 0;
    PlayerInfo[playerid][pWepSerial] = 0;
    
    return 1;
}


ALTX:pickupitem("/pitem","´/pegaritem");
CMD:pegaritem(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) 
        return SendErrorMessage(playerid, "Você não pode usar esta funcionalidade agora.");
    
    new dropid = -1;
    
    if ((dropid = ItemDrop_Nearest(playerid)) == -1)
        return 1;
    
    if (IsPlayerCrouching(playerid))
        ItemDrop_Pickup(playerid, dropid);
    else
        InfoMSG(playerid, "Você precisa ~y~se abaixar ~w~para pegar ~w~um objeto~y~", 3);
    
    return 1;
}
