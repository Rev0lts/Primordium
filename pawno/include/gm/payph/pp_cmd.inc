ALTX:payphone("/pp","/telefone");
CMD:payphone(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
        return SendErrorMessage(playerid, "Você não pode usar este recurso no momento.");

    new id = -1;
    if ((id = Payphone_Nearest(playerid)) == -1)
        return SendErrorMessage(playerid, "Não há telefone público próximo a você.");

    if (PayphoneInfo[id][phUsed] != INVALID_PLAYER_ID)
        return SendErrorMessage(playerid, "O telefone público já está em uso.");

    if (IsPlayerPhoneCreated(playerid))
        return SendErrorMessage(playerid, "Você já está usando seu telefone.");

    if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
        return SendErrorMessage(playerid, "Você tem uma chamada não concluída. Digite /finalizar para desligar.");

    pTemp[playerid][pPayphone] = id;
    PayphoneInfo[id][phUsed] = playerid;

    new info[128];
    format(info, sizeof(info), "Digite '~y~/pc~w~' para retornar o ~y~cursor~w~.~n~O número deste telefone público:~y~ %i", PayphoneInfo[id][phNumber]);
    InfoMSG(playerid, info, 5);

    SetPlayerFacingAngle(playerid, PayphoneInfo[id][phRZ]);
    SetCameraBehindPlayer(playerid);

    if (PayphoneInfo[id][phState] == PAYPHONE_STATE_RINGING)
    {
        PayphoneInfo[id][phState] = PAYPHONE_STATE_INCALL;

        new targetid = PayphoneInfo[id][phCaller];

        pTemp[playerid][pCallLine] = targetid;
        pTemp[targetid][pCallLine] = playerid;

        pTemp[playerid][pCallStatus] = 2;
        pTemp[playerid][pCallTime] = 0;

        pTemp[targetid][pCallStatus] = 2;
        pTemp[targetid][pCallTime] = 0;

        Update_Payphone(id);

        if (0 == IsPlayerPhoneCreated(targetid))
            Phone_Show(targetid, PAGE_CALL);
        else
        {
            ph_showpage(targetid, PAGE_CALL);
        }

        SM2(playerid, "Você atendeu a chamada.");
        SM2(targetid, "O assinante atendeu a chamada.");
    }

    PlayerOnPayphone(playerid);
    return 1;
}
