ALTX:paypark("/pagarestacionamento");
CMD:pagarpark(playerid, params[]) // Pagar estacionamento
{
    new id = -1,
        vid = -1,
        Float:x, Float:y, Float:z;

    if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você deve estar fora do veículo.");
    if ((id = ParkMeter_Nearest(playerid)) == -1) return SendErrorMessage(playerid, "Não há parquímetro próximo a você.");
    if (!pMeter[id][parkAvailable]) return SendErrorMessage(playerid, "Estacionamento já pago!");

    if (GetPlayerMoney(playerid) < CASH_PARKING) return SendErrorMessage(playerid, "Você não tem dinheiro suficiente. Você precisa de $%d para pagar pelo estacionamento.", CASH_PARKING);

    for (new i = 1; i < MAX_VEHICLES; i++)
    {
        if (IsValidVehicle(i) && (!IsAPlane(i) || !IsABiker(i) || !IsAHelicopter(i) || !IsABoat(i) || !IsABicycle(i)))
        {
            GetVehiclePos(i, x, y, z);
            if (IsPlayerInRangeOfPoint(playerid, 6.0, x, y, z))
            {
                vid = i;
                break;
            }
        }
    }

    if (vid == -1) return SendErrorMessage(playerid, "Não há veículo adequado nas proximidades.");

    SendClientMessageEx(playerid, 0x33AA33FF, "> Você pagou pelo estacionamento do seu %s por $%d.", VehicleNames[GetVehicleModel(vid) - 400], CASH_PARKING);

    GivePlayerMoney(playerid, -CASH_PARKING);

    pMeter[id][parkVehicle] = vid;
    pMeter[id][parkAvailable] = false;

    return 1;
}
