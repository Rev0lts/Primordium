Garage_OnPlayerEnterDynamicCP(playerid, checkpointid) // checkpoint
{
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
    {
        for(new i = 0; i < MAX_GARAGES; i++)
        {
            if (!GarageInfo[i][garageID] || GarageInfo[i][garageCP] != checkpointid) continue;

            if (GarageInfo[i][garageOwner])
            {
                SendSyntaxMessage(playerid, "A garagem pertence a %s.", GarageInfo[i][garageOwnerName]);
                SendHelpMessage(playerid, "/entrar sair (/g)aragem info");

                new mes[64];
                format(mes, sizeof(mes), "~h~~p~Garagem~n~%s", GarageInfo[i][garageOwnerName]);

                CreateLableText(playerid, mes, "~h~Pressione ~y~~h~'F' ~w~ para entrar ou~n~digite ~y~~h~/entrar~h~~w~ para entrar");
            }
            else if (!GarageInfo[i][garageOwner] && !GarageInfo[i][garageHouse] && !GarageInfo[i][garageBiz])
            {
                SendSyntaxMessage(playerid, "Garagem à venda. Preço: $%i.", GarageInfo[i][garagePrice]);
                SendHelpMessage(playerid, "/entrar (/g)aragem info (/g)aragem comprar");

                new mes[64];
                format(mes, sizeof(mes), "~h~~p~GARAGEM À VENDA!~n~~g~PREÇO: ~g~$%i", GarageInfo[i][garagePrice]);

                CreateLableText(playerid, mes, "~h~Pressione ~y~~h~'F' ~w~ para comprar ou~n~digite ~y~~h~/g comprar~h~~w~ para compra");
            } else if (!GarageInfo[i][garageOwner] && GarageInfo[i][garageHouse]) {
                SendSyntaxMessage(playerid, "Esta garagem é vendida com a casa.");
                SendHelpMessage(playerid, "/entrar (/g)aragem info");

                new mes[64];
                format(mes, sizeof(mes), "~h~~p~GARAGEM À VENDA!~n~~y~(Junto com a casa %i)", GarageInfo[i][garageHouse]);

                CreateLableText(playerid, mes, "~h~Pressione ~y~~h~'F' ~w~ para comprar ou~n~digite ~y~~h~/g comprar~h~~w~ para compra");
            }
            else if (!GarageInfo[i][garageOwner] && GarageInfo[i][garageBiz])
            {
                SendSyntaxMessage(playerid, "Esta garagem está sendo vendida junto com o negócio.");
                SendHelpMessage(playerid, "/entrar (/g)aragem info");

                CreateLableText(playerid, "~h~~p~GARAGEM À VENDA!~n~~y~(Junto com o negócio)", "~h~Pressione ~y~~h~'F' ~w~ para comprar ou~n~digite ~y~~h~/g comprar~h~~w~ para compra");
            }
            return 1;
        }
    }
    return 1;
}


Garage_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT *, (SELECT name FROM users WHERE id = garage.owner) as ownername FROM `garage`", "LoadDynamicGarage");
}