CMD:cadaver(playerid, params[])
{
    new i;

    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
        return SendErrorMessage(playerid, "Voc� n�o pode usar esta funcionalidade agora.");
    if (!IsPlayerLogged(playerid))                   return SendNotConnected(playerid);
    if ((i = GetNearestCorpse(playerid)) == -1)      return SendErrorMessage(playerid, "N�o h� corpos perto de voc�.");
  
    Dialog_Show(playerid, CorpseInfo, DIALOG_STYLE_MSGBOX, "Informa��es sobre um corpo desconhecido", CorpInfo[i][cNote], "Op��es", "Fechar");
    return 1;
}