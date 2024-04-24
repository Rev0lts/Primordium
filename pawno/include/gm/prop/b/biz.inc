Biz_OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
    for(new i = 0; i < MAX_BIZ; i++)
    {
        if (pickupid != BizInfo[i][bizPickUp] || 0 == BizInfo[i][bizID]) continue;

        new 
            title[128],
            mes[128]
        ;

        if (BizInfo[i][bizFreeze] == 1) // informa��o
        {
            format(title, sizeof(title), "~h~~p~%s~n~~y~Estado", BizInfo[i][bizName]);
            format(mes, sizeof(mes), "~w~Taxa de entrada: ~g~$%i~n~~w~Digite ~y~/entrar~w~ ou pressione ~y~~h~'F'", BizInfo[i][bizEntrance]);

            CreateLableText(playerid, title, mes);
            return 1;
        }
        else
        {
            if (BizInfo[i][bizOwnerID] == 0)
            {
                format(title, sizeof(title), "~h~~p~EMPRESA � VENDA!~n~~w~%s", BizInfo[i][bizName]);
                format(mes, sizeof(mes), "~w~Pre�o: ~g~$%i~n~~w~Pressione ~y~~h~'F'~w~ ou digite ~y~/empresa comprar", BizInfo[i][bizPrice]);

                CreateLableText(playerid, title, mes);
            }
            else
            {
                format(title, sizeof(title), "~h~~p~%s~n~~w~~y~%s", BizInfo[i][bizName], BizInfo[i][bizOwner]);
                format(mes, sizeof(mes), "~w~Taxa de entrada: ~g~$%i~n~~w~Digite ~y~/entrar~w~ ou pressione ~y~~h~'F'", BizInfo[i][bizEntrance]);
                
                CreateLableText(playerid, title, mes);
            }
            return 1;
        }
    }
    return 1;
}

Biz_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT *, (SELECT name FROM users WHERE id = business.owner) as ownername, (SELECT last_login FROM users WHERE id = business.owner) as last_login FROM `business`", "LoadDynamicBiz");
}

Fo_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid) 
{
    for(new i; i != 4; i++)
    {
        if (playertextid != TD_restaurant_prod[playerid][i + 4])    continue;

        RestaurantMenuHide(playerid);

        if (pTemp[playerid][pFoodStand] != 0 || IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))    return SendWarningMessage(playerid, "Suas m�os j� est�o ocupadas.");

        new bizid;

        if ((bizid = GetNearInsideBiz(playerid)) == -1)    return SendErrorMessage(playerid, "Voc� deve estar dentro da empresa.");

        if (BizInfo[bizid][bizType] != BIZ_PIZZA && BizInfo[bizid][bizType] != BIZ_CLUCKIN && BizInfo[bizid][bizType] != BIZ_BURGER && BizInfo[bizid][bizType] != BIZ_DONUT)
        {
            return SendErrorMessage(playerid, "Voc� n�o est� em um restaurante.");
        }

        new type = BizInfo[bizid][bizType] - BIZ_PIZZA;

        if (GetCash(playerid) < BizInfo[bizid][bizProductPrice][i])            return SendNotCash(playerid);
        if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])    return SendErrorMessage(playerid, "N�o h� produtos suficientes na empresa.");
            
        cmd::server_ame(playerid, "faz um pedido de comida.");

        SendSyntaxMessage(playerid, "Voc� pediu {FF6347}%s{FFFFFF} por {FF6347}$%s{FFFFFF}.", name_order[type][i], FormatNumber(BizInfo[bizid][bizProductPrice][i]));
        SendSyntaxMessage(playerid, "Voc� pode colocar a refei��o na mesa {FF6347}/comida colocar{FFFFFF} ou {FF6347}/comida soltar{FFFFFF} para jogar fora a bandeja.");

        GiveCash(playerid, - BizInfo[bizid][bizProductPrice][i]);

        if (BizInfo[bizid][bizOwnerID] != 0)
        {
            BizInfo[bizid][bizProduct]--;
            GiveBizCash(bizid, BizInfo[bizid][bizProductPrice][i]);
        }
        
        OnPlayAnim(playerid,"CARRY","crry_prtial",4.1,1,1,1,1,1);
        SetPlayerAttachedObject(playerid, ATTACH_HANDS, model_order[type][i], 5, 0.091998, 0.021999, 0.409, -104.9, 27.6, 0, 1, 1, 1);
        pTemp[playerid][pFoodStand] = model_order[type][i];

        new Float: health;

        if (AntiCheatGetHealth(playerid, health))    SetPlayerHealth(playerid, health + 30.0 + (i * 10.0));
        if (health + 30.0 + (i * 10.0) > 150.0)        SetPlayerHealth(playerid, 150.0);

        return 1;
    }
    return 1;
}


Food_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;
    
    GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
    GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

    if (GetPVarInt(playerid, #edit_food) != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
    {
        new id = GetPVarInt(playerid, #edit_food)-1;
        
        DeletePVar(playerid, #edit_food);

        if (GetDistanceBetweenPoints(x, y, z, FoodDrop[id][foodPosX], FoodDrop[id][foodPosY], FoodDrop[id][foodPosZ]) > 10.0)
        {
            SetDynamicObjectPos(objectid, FoodDrop[id][foodPosX], FoodDrop[id][foodPosY], FoodDrop[id][foodPosZ]);
            return SendErrorMessage(playerid, "O objeto est� muito longe da posi��o inicial.");
        }

        if (objectid != FoodDrop[id][foodObject])    return SendErrorMessage(playerid, "Erro ao editar o item.");
        SetDynamicObjectPos(objectid, x, y, z);
        SetDynamicObjectRot(objectid, rx, ry, rz);

        GetDynamicObjectPos(objectid, FoodDrop[id][foodPosX], FoodDrop[id][foodPosY], FoodDrop[id][foodPosZ]);
        return 1;
    }
    return 1;
}