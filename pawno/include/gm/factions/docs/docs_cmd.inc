CMD:isolar(playerid, params[])
{
    new userid, time;

    if (!IsPlayerFactionDocs(playerid)) return SendErrorMessage(playerid, "Você deve ser um funcionário da prisão.");
    if (sscanf(params, "uI(0)", userid, time)) return SendHelpMessage(playerid, "/isolar [ID/nome do jogador] [tempo (minutos)]");
    if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Você deve estar de serviço.");
    if (!PlayerToPoint(50.0, playerid, -1774.4573, -488.8803, 621.1163)) return SendErrorMessage(playerid, "Você deve estar no isolamento.");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
    if (userid == playerid) return 1;

    if (PlayerInfo[userid][pIsolateTime])
    {
        SendSyntaxMessage(playerid, "Você liberou %s do isolamento.", GetNameEx(userid));
        SendSyntaxMessage(userid, "Você foi liberado do isolamento.");
        PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));

        new mes[5];

        format(mes, sizeof(mes), "A%i", PlayerInfo[userid][pPrisoned]+100);
        UpdateDynamic3DTextLabelText(PrisonLabel[userid], 0x00FF00FF, mes);
        SendSyntaxMessage(userid, "Seu número de prisioneiro é - A%i(/numeroprisao).", PlayerInfo[userid][pPrisoned]+100);
        PlayerInfo[userid][pIsolateTime] = 0;

        foreach(new i : Player)
        {
            if (!IsPlayerLogged(i) || PlayerInfo[i][pJailed] != PRISON_JAIL) continue;

            SendSyntaxMessage(i, "%s %s liberou %s do isolamento.", GetRankName(playerid), GetNameEx(playerid), GetNameEx(userid));
        }
        return 1;
    }

    if (time < 1 || time > 360) return SendErrorMessage(playerid, "O tempo deve estar entre 1 e 360 minutos.");

    SendSyntaxMessage(playerid, "Você colocou %s no isolamento.", GetNameEx(userid));
    SendSyntaxMessage(userid, "Você foi colocado no isolamento.");
    PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));
    SendSyntaxMessage(userid, "Seu número de prisioneiro é - B%i(/numeroprisao).", PlayerInfo[userid][pPrisoned]+100);
    PlayerInfo[userid][pIsolateTime] = 60 * time;
    PutPlayerInPrison(userid);

    foreach(new i : Player)
    {
        if (!IsPlayerLogged(i) || PlayerInfo[i][pJailed] != PRISON_JAIL) continue;

        SendSyntaxMessage(i, "%s %s colocou %s no isolamento.", GetRankName(playerid), GetNameEx(playerid), GetNameEx(userid));
    }
    return 1;
}

CMD:libertar(playerid, params[])
{
    new userid;

    if (!IsPlayerFactionDocs(playerid)) return SendErrorMessage(playerid, "Você deve ser um funcionário da prisão.");
    if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/libertar [ID/nome do jogador]");
    if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Você deve estar de serviço.");
    if (!PlayerToPoint(30.0, playerid, 555.5505, -474.4340, 915.6733)) return SendErrorMessage(playerid, "Você não está na prisão.");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
    if (userid == playerid) return 1;
    if (PlayerInfo[userid][pJailed] != PRISON_JAIL) return SendErrorMessage(playerid, "O jogador não está na prisão.");
    if (PlayerInfo[playerid][pRank] < 6) return SendErrorMessage(playerid, "Você não pode libertar um prisioneiro da prisão.");

    SetPlayerPosEx(userid, 135.2682, 1947.4398, 19.3663);
    SetInterior(userid, 0);
    SetWorld(userid, 0);
    PlayerInfo[userid][pIsolateTime] = 0;
    PlayerInfo[userid][pJailTime] = 0;
    PlayerInfo[userid][pJailed] = 0;

    if (IsValidDynamic3DTextLabel(PrisonLabel[userid]))
    {
        DestroyDynamic3DTextLabel(PrisonLabel[userid]);
        PrisonLabel[userid] = Text3D:INVALID_3DTEXT_ID;
    }

    GameTextForPlayer(userid, "~g~VOCÊ FOI LIBERTADO~n~~w~TENTE SE COMPORTAR BEM", 5000, 1);
    SendSyntaxMessage(userid, "Você recebeu anistia de %s %s.", GetRankName(playerid), GetNameEx(playerid));
    return 1;
}
