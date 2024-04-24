Mechanic_SecondTimer(playerid)
{
		if (RepairTime[playerid] > 0 && RepairService[playerid] != 0)
		{
			new Float:x, Float:y, Float:z;
			GetVehiclePos(RepairToVehicle[playerid], x, y, z);
			if (RepairTime[playerid] == 1)
			{
				if (RepairService[playerid] == 1)
				{
					JobMessage(playerid, "~h~~p~MOTOR REPARADO..", 6);
					VehicleInfo[RepairToVehicle[playerid]][carDeath] = 0;
					if (VehicleInfo[RepairToVehicle[playerid]][carOwnerID] != 0)     SetVehicleHealth(RepairToVehicle[playerid], GetVehicleMaxHealth(GetVehicleModel(RepairToVehicle[playerid])) + VehicleInfo[RepairToVehicle[playerid]][carArmour]);
					else                                                            SetVehicleHealth(RepairToVehicle[playerid], 1000.0);
				}
				else if (RepairService[playerid] == 2)
				{
					JobMessage(playerid, "~h~~p~VOCE TERMINOU DE POLIR COM SUCESSO!", 6);
					RepairVehicleBody(RepairToVehicle[playerid]);
				}
				else if (RepairService[playerid] == 3)
				{
					JobMessage(playerid, "~h~~p~VOCE REPAROU O MOTOR COM SUCESSO!", 6);
					VehicleInfo[RepairToVehicle[playerid]][carEngine] = 20.0;
				}
				else if (RepairService[playerid] == 4)
				{
					JobMessage(playerid, "~h~~p~VOCE PINTOU O VEICULO COM SUCESSO ", 6);
					ChangeVehicleColor(RepairToVehicle[playerid], GetPVarInt(playerid, "color1"), GetPVarInt(playerid, "color2"));
					VehicleInfo[RepairToVehicle[playerid]][carColor1] = GetPVarInt(playerid, "color1");
					VehicleInfo[RepairToVehicle[playerid]][carColor2] = GetPVarInt(playerid, "color2");
					DeletePVar(playerid, "color1");
					DeletePVar(playerid, "color2");
				}
				
				ResetPlayerWeapons(playerid);
				RepairService[playerid] = 0;
				RepairTowTruck[playerid] = INVALID_VEHICLE_ID;
				RepairTime[playerid] = 0;
				RepairOffer[playerid] = INVALID_PLAYER_ID;
				RepairToVehicle[playerid] = INVALID_VEHICLE_ID;
			}
			else if (IsPlayerInRangeOfPoint(playerid, 4.2, x, y, z))
			{
				if (HoldingKey(playerid, KEY_FIRE) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && PlayerHoldingWeapon(playerid, 41))
				{
					new veh = RepairTowTruck[playerid], mes[128];

					if (RepairService[playerid] == 1)
					{
						format(mes, sizeof(mes), "~h~~p~REPARO DE MOTOR~n~~w~VOCE TEM ~b~%i ~w~PECAS RESTANTES", VehicleInfo[veh][carComps]);
						ShowJobMessage(playerid, mes);
						VehicleInfo[veh][carComps] -= 4;
					}
					if (RepairService[playerid] == 2)
					{
						format(mes, sizeof(mes), "~h~~p~POLIMENTO~n~~w~VOCE TEM ~b~%i ~w~PECAS RESTANTES", VehicleInfo[veh][carComps]);
						ShowJobMessage(playerid, mes);
						VehicleInfo[veh][carComps] -= 5;
					}
					if (RepairService[playerid] == 3)
					{
						format(mes, sizeof(mes), "~h~~p~RECUPERACAO DO MOTOR~n~~w~VOCE TEM ~b~%i ~w~PRODUTOS RESTANTES~n~~y~Espere %i", VehicleInfo[veh][carComps], RepairTime[playerid]);
						ShowJobMessage(playerid, mes);
						VehicleInfo[veh][carComps] -= 5;
					}
					if (RepairService[playerid] == 5)
					{
						format(mes, sizeof(mes), "~h~~p~SERVICO DE PINTURA~n~~w~VOCÊ TEM ~b~%i ~w~PRODUTOS RESTANTES~n~~y~Espere %i", VehicleInfo[veh][carComps], RepairTime[playerid]);
						ShowJobMessage(playerid, mes);
						VehicleInfo[veh][carComps] -= 3;
					}
					RepairTime[playerid]--;
				}
			}
			else
			{
				ShowJobMessage(playerid, "~h~~p~VOCE DEVE FICAR PERTO DO TRANSPORTE!");
			}
		}
		
		if (GetCash(playerid) > PlayerInfo[playerid][pCash])  SetCash(playerid, PlayerInfo[playerid][pCash]);

        return 1;
}
