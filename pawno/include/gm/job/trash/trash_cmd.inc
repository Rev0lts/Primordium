ALTX:pickuptrash("/ptrash","/pegarlixo","/plixo");
CMD:pickuptrash(playerid, params[])
{
    if (PlayerInfo[playerid][pJob] != JOB_TRASHMASTER)	return SendErrorMessage(playerid, "Você não é um coletor de lixo."); // Mensagem de erro se o jogador não for um coletor de lixo.
	if (IsPlayerInAnyVehicle(playerid))                  return SendErrorMessage(playerid, "Você não pode usar este comando em um veículo!"); // Mensagem de erro se o jogador estiver em um veículo.
    if (HasTrash[playerid])                              return SendErrorMessage(playerid, "Você já está segurando um saco de lixo!"); // Mensagem de erro se o jogador já estiver segurando um saco de lixo.

	if (!pTemp[playerid][pRentveh])  return SendErrorMessage(playerid, "Primeiro você precisa alugar um veículo de trabalho! (/rcj)"); // Mensagem de erro se o jogador não tiver alugado um veículo.
    if (!pTemp[playerid][pJob])      return SendErrorMessage(playerid, "Primeiro você precisa alugar um veículo de trabalho! (/rcj)"); // Mensagem de erro se o jogador não estiver em um trabalho.

    new vehicleid = pTemp[playerid][pRentveh];
    if (!IsTrashMasterCar(vehicleid) || !IsATrashTruck(vehicleid)) return SendErrorMessage(playerid, "Você não está em um caminhão de lixo!"); // Mensagem de erro se o jogador não estiver em um caminhão de lixo.

    new id = Trash_Nearest(playerid);
	if (id == -1)                      return SendErrorMessage(playerid, "Não há latas de lixo perto de você!"); // Mensagem de erro se não houver latas de lixo próximas.
	if (TrashData[id][TrashLevel] < 1) return SendErrorMessage(playerid, "Lixeira vazia!"); // Mensagem de erro se a lixeira estiver vazia.

    new Float: x, Float: y, Float: z;
    GetVehicleBoot(vehicleid, x, y, z);
    if (GetPlayerDistanceFromPoint(playerid, x, y, z) >= 30.0) return SendErrorMessage(playerid, "Não há um caminhão de lixo perto de você!"); // Mensagem de erro se o jogador estiver longe demais do caminhão de lixo.

    TrashData[id][TrashLevel]--;
	KillTimer(TrashData[id][TrashTimer]);
    
    TrashData[id][TrashTimer] = SetTimerEx("FillTrash", REFILL_TIME * 1000, false, "i", id);
	
    TrashCP[playerid] = CreateDynamicCP(x, y, z, 3.0, .playerid = playerid);
	HasTrash[playerid] = true;
	
    ApplyAnimation(playerid, "CARRY", "liftup105", 4.1, 0, 0, 0, 0, 0);
	SetPlayerAttachedObject(playerid, ATTACHMENT_INDEX, 1264, 6, 0.222, 0.024, 0.128, 1.90, -90.0, 0.0, 0.5,0.5, 0.5);
	
    Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, TrashData[id][TrashLabel], E_STREAMER_COLOR, (TrashData[id][TrashLevel] == 0) ? 0xE74C3CFF : 0xF39C12FF);
	
    InfoMSG(playerid, "Pegue o ~h~~g~saco de lixo ~w~e leve-o para o caminhao de lixo.~n~ Voce pode descartar o saco - ~r~~k~~CONVERSATION_NO~.", 5);
	return 1;
}
