CMD:noticias(playerid,params[])
{
    if (!IsPlayerFactionNews(playerid)) return SendErrorMessage(playerid, "Você precisa ser um funcionário da SAN.");
    if (PlayerInfo[playerid][pSettings] & togNews) return SendErrorMessage(playerid, "Você desativou o feed de notícias.");
    if (pTemp[playerid][pNews] != 0) return SendErrorMessage(playerid, "Você já está no ar. Use /sairnoticias para sair do ar.");

    pTemp[playerid][pNews] = 1;
    SendSyntaxMessage(playerid, "Você começou a reportagem. Use o chat local para transmitir.");
    return 1;
}

CMD:sairnoticias(playerid,params[])
{
    if (!IsPlayerFactionNews(playerid)) return SendErrorMessage(playerid, "Você precisa ser um funcionário da SAN.");
    if (pTemp[playerid][pNews] == 0) return SendErrorMessage(playerid, "Você não está no ar. Use /noticias para entrar ao vivo.");

    pTemp[playerid][pNews] = 0;
    SendSyntaxMessage(playerid, "Você terminou o seu relatório.");
    return 1;
}

CMD:entrevistar(playerid, params[])
{
    if (!IsPlayerFactionNews(playerid)) return SendErrorMessage(playerid, "Você deve ser um funcionário da SAN.");

    new userid;

    if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/entrevistar [ID/Parte do Nome]");
    if (PlayerInfo[playerid][pSettings] & togNews) return SendErrorMessage(playerid, "Você desativou o feed de notícias.");
    if (pTemp[playerid][pNews] == 0) return SendErrorMessage(playerid, "Você não está no ar. Use /noticias para entrar ao vivo.");
    if (userid == playerid) return SendErrorMessage(playerid, "Você não pode convidar a si mesmo para uma entrevista.");
    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
    if (pTemp[playerid][pInterviewPlayer] != INVALID_PLAYER_ID) return SendErrorMessage(playerid, "Você foi convidado para uma entrevista, por favor, espere um pouco ou aceite o convite.");
    if (pTemp[userid][pInterviewPlayer] != INVALID_PLAYER_ID) return SendErrorMessage(playerid, "Um convite para uma entrevista já foi enviado ao jogador.");

    pTemp[userid][pInterviewPlayer] = playerid;
    SetPVarInt(userid, #accept_time, gettime() + 30);

    SendSyntaxMessage(playerid, "%s recebeu um convite para uma entrevista, por favor, espere.", GetNameWithMask(userid));
    SendSyntaxMessage(userid, "%s o convida para uma entrevista.");
    SendSyntaxMessage(userid, "Pressione Y para concordar com a entrevista.");
    return 1;
}

CMD:sairentrevista(playerid, params[])
{
    if (pTemp[playerid][pInterview] != INVALID_PLAYER_ID)
    {
        SendSyntaxMessage(playerid, "Você terminou a entrevista.");
        pTemp[playerid][pInterview] = INVALID_PLAYER_ID;
        return 1;
    }

    if (!IsPlayerFactionNews(playerid)) return SendErrorMessage(playerid, "Você deve ser um funcionário da SAN.");

    new userid;

    if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/sairentrevista [ID/Parte do Nome]");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
    if (pTemp[userid][pInterview] == INVALID_PLAYER_ID) return SendErrorMessage(playerid, "Você não está entrevistando este jogador.");

    SendSyntaxMessage(playerid, "Você terminou a entrevista com %s.", GetNameWithMask(userid));
    SendSyntaxMessage(userid, "%s terminou a entrevista com você.", GetNameWithMask(playerid));
    pTemp[userid][pInterview] = INVALID_PLAYER_ID;
    return 1;
}
