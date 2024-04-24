CMD:chopshop(playerid, params[])
{
    if (!IsAdminDuty(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_3) && !isLeader(playerid))
        return SendErrorMessage(playerid, "Voc� n�o pode usar este comando.");
    if (!IsAdminDuty(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_3) && Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_GANG && Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_MAFIA)
        return SendErrorMessage(playerid, "Voc� n�o pode usar este comando.");

    if (GetChopshopID(playerid) != -1)
    {
        Dialog_Show(playerid, DIALOG_CHOPSHOP, DIALOG_STYLE_LIST, "Chopshop", "Excluir Chopshop\nAtualizar lista de ve�culos\nEstat�sticas do Chopshop\nAlterar porcentagem\nEditar posi��o do Chopshop", "Selecionar", "<<");
    }
    else if (IsAdminDuty(playerid) && CheckAdmin(playerid, ADMIN_LEVEL_2) || !IsFactionHasChopShop(PlayerInfo[playerid][pFaction])) {
        Dialog_Show(playerid, DIALOG_CHOPSHOP, DIALOG_STYLE_LIST, "Chopshop", "Criar Chopshop", "Selecionar", "<<");
    }
           
    return 1;
}