PayDay(i)
{
    if (!IsPlayerLogged(i)) return 1;

    new oldbank = PlayerInfo[i][pBank], savings, checks = 300, tax_house, rent_house, debt;

    new ora, minut, sec;
    gettime(ora, minut, sec);

    if (ora == 18 && minut == 0 || ora == 19 && minut == 0 || ora == 20 && minut == 0)
    {
        PlayerInfo[i][pPunctePremium]++; // substitua com sua variável para pontos de respeito
        SendWarningMessage(i, "Você recebeu um ponto extra de Halloween porque o happy hour está ativo."); // mensagem enviada ao jogador naquela hora
        return 1;
    }

    if (IsPlayerFactionPolice(i) || IsPlayerFactionDocs(i) || IsPlayerFactionNews(i)) checks += (PlayerInfo[i][pRank] * 30);
    if (IsPlayerFactionMedic(i) || IsPlayerFactionFire(i) || IsPlayerFactionMedic(i)) checks += 100 + (PlayerInfo[i][pRank] * 35);

    if (PlayerInfo[i][pJob] == JOB_MECHANIC) checks += 50;
    else if (PlayerInfo[i][pJob] == JOB_TAXI) checks += 50;

    if (PlayerInfo[i][pSavings] > 35000 && PlayerInfo[i][pSavings] < 100000) savings = (PlayerInfo[i][pSavings]) / 100;
    PlayerInfo[i][pBank] += savings;

    if (PlayerInfo[i][pBank] < 0)
    {
        debt = floatround(float(checks) * 0.8);
        PlayerInfo[i][pBank] += debt,
            checks -= debt;
    }

    new houses_tax[3] = {20, 50, 100};

    for (new h; h != MAX_HOUSES; h++)
    {
        if (HouseInfo[h][hID] == 0 || HouseInfo[h][hOwnerID] != PlayerInfo[i][pID]) continue;

        tax_house += houses_tax[HouseInfo[h][hClass]];
    }

    new houseid = -1;
    if (PlayerInfo[i][pRentPrice] != 0 && PlayerInfo[i][pRentKey] != 0 && (houseid = GetHouseIDByMysql(PlayerInfo[i][pRentKey])) != -1)
    {
        rent_house += PlayerInfo[i][pRentPrice];
        HouseInfo[houseid][hCash] += PlayerInfo[i][pRentPrice];
    }
    PlayerInfo[i][pBank] -= tax_house + rent_house;

    //SMS
    new msg[256];
    format(msg, sizeof(msg), "|______________CHEQUE DO BANCO______________|\nSaldo: $%i\nSalário: $%i\n", oldbank, checks);
    if (savings) format(msg, sizeof(msg), "%sEconomias recebidas: $%i, com juros: 1\n", msg, savings);
    if (rent_house) format(msg, sizeof(msg), "%sCobrança de aluguel da casa: $%i\n", msg, rent_house);
    if (tax_house) format(msg, sizeof(msg), "%sImposto sobre imóveis: $%i\n", msg, tax_house);
    format(msg, sizeof(msg), "%s|_________________________________________|\nNovo saldo: $%i", msg, PlayerInfo[i][pBank]);
    SendSMStoPlayer(i, "1313", "Banco Red County", msg, 0);
    ////////////////////

    SendSyntaxMessage(i, "Você pode pegar seu salário no banco usando o comando /paycheck.");
    if (debt) SendSyntaxMessage(i, "Você está em dívida em uma conta bancária. O banco deduziu parte do seu salário para pagar a dívida.");

    PlayerInfo[i][pPayDayTime] = 0;
    PlayerInfo[i][pPayCheckMoney] += checks;
    PlayerInfo[i][pHours]++;
    PlayerInfo[i][pCareer]++;

    if (PlayerInfo[i][pWepLic] && PlayerInfo[i][pWepCount] > 0)
    {
        PlayerInfo[i][pWepCount]--;
        if (!PlayerInfo[i][pWepCount]) SendHelpMessage(i, "Você pode comprar armas novamente na loja de armas! (/gps)");
    }

    //PlayerInfo[i][pExp]++;
    //new expamount = (PlayerInfo[i][pLevel]+1) * LEVEL_EXP;

    if (PlayerInfo[i][pHours] >= 8 && PlayerInfo[i][pLevel] == 0)
    {
        PlayerInfo[i][pLevel]++;
        PlaySound(i, 1052);

        format(msg, sizeof(msg), "~y~SUBIU DE NIVEL~n~~w~Agora voce e nivel %i", PlayerInfo[i][pLevel]);
        GameTextForPlayer(i, msg, 5000, 1);

        SendServerMessage(i, "Você não é mais um 'Iniciante', todas as funções do projeto estão agora disponíveis para você.");
        SendServerMessage(i, "Além disso, seu saldo recebeu $%i, você pode retirá-los junto com o salário no banco.", SERVER_BONUS);

        PlayerInfo[i][pPayCheckMoney] += SERVER_BONUS;

        SetPlayerScore(i, PlayerInfo[i][pLevel]);
    }

    if (PlayerInfo[i][pForbid] > 0)
    {
        PlayerInfo[i][pForbid]--;
        SQL_SetInteger("users", "Forbid", PlayerInfo[i][pForbid], PlayerInfo[i][pID]);

        if (PlayerInfo[i][pForbid] == 0)
            SendClientMessage(i, COLOR_ORANGE, "[ ! ] {FFFFFF}A proibição de uso de armas acabou, agora você pode usá-las novamente.");

        Save_User(i);
    }
    return 1;
}
