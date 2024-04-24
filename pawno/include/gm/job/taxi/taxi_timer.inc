forward Taxi_FiveSecTimer();
public Taxi_FiveSecTimer()
{
    foreach (new i : Player)
    {
        if (!IsPlayerLogged(i) || !TaxiDuty[i] || TaxiPassenger[i] == INVALID_PLAYER_ID) continue;

        new targetid = TaxiPassenger[i], vehicle = GetPlayerVehicleID(i);

        if (!IsDriver(i))
        {
            new mes[32];
            format(mes, sizeof(mes), "~r~+$%i TOTAL: $%i", TaxiMoney[i], TaxiMade[i]);
            GameTextForPlayer(i, mes, 2000, 1);

            TaxiPassenger[i] = INVALID_PLAYER_ID;
            TaxiMoney[i] = 0;
            continue;
        }

        if (!IsPlayerLogged(targetid) || !IsPlayerInVehicle(targetid, vehicle))
        {
            new mes[32];
            format(mes, sizeof(mes), "~r~+$%i TOTAL: $%i", TaxiMoney[i], TaxiMade[i]);
            GameTextForPlayer(i, mes, 2000, 1);

            TaxiPassenger[i] = INVALID_PLAYER_ID;
            TaxiMoney[i] = 0;
            continue;
        }

        if (GetCash(targetid) >= TaxiFare[i])
        {
            GiveCash(targetid, -TaxiFare[i]);
            TaxiMade[i] += TaxiFare[i];
            TaxiMoney[i] += TaxiFare[i];

            new mes[32];
            format(mes, sizeof(mes), "~y~VALOR: $%i", TaxiMoney[i]);
            GameTextForPlayer(i, mes, 5050, 1);
            GameTextForPlayer(targetid, mes, 5050, 1);
        }
        else
        {
            new mes[32];
            format(mes, sizeof(mes), "~r~+$%i TOTAL: $%i", TaxiMoney[i], TaxiMade[i]);
            GameTextForPlayer(i, mes, 2000, 1);
            SendSyntaxMessage(i, "O passageiro ficou sem dinheiro. Você levando ele de graça.");
            TaxiPassenger[i] = INVALID_PLAYER_ID;
            TaxiMoney[i] = 0;
        }
    }
    SetTimer("Taxi_FiveSecTimer", 5000, false);
    return 1;
}
