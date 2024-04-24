CMD:cadaver(playerid, params[])
{
    new i;

    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
        return SendErrorMessage(playerid, "Você não pode usar esta funcionalidade agora.");
    if (!IsPlayerLogged(playerid))                   return SendNotConnected(playerid);
    if ((i = GetNearestCorpse(playerid)) == -1)      return SendErrorMessage(playerid, "Não há corpos perto de você.");
  
    Dialog_Show(playerid, CorpseInfo, DIALOG_STYLE_MSGBOX, "Informações sobre um corpo desconhecido", CorpInfo[i][cNote], "Opções", "Fechar");
    return 1;
}
