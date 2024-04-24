CMD:comprarcomida(playerid, params[]) //tåntî
{ 
	if (!CheckInv(playerid)) return SendErrorMessage(playerid, "Seu inventário está completamente cheio.");

	for(new i; i != MAX_FOOD_STAND; i++)
	{
	    if (FoodStand[i][fStandOn] != 1 || !PlayerToPoint(3, playerid,FoodStand[i][fPosX], FoodStand[i][fPosY], FoodStand[i][fPosZ]) || FoodStand[i][fVW] != GetWorld(playerid) || FoodStand[i][fInt] != GetInterior(playerid)) continue;

		new mes[128];
		format(mes, sizeof(mes), "Hambúrguer\t$%i\nPizza\t$%i\nFrango\t$%i\nCachorro-quente\t$%i", PRICE_BURGER, PRICE_PIZZA, PRICE_CHICKEN, PRICE_HOTDOG);
		Dialog_Show(playerid, BuyStandFood, DIALOG_STYLE_TABLIST, "{ffffff}Tenda de comida", mes, "Comprar", "Cancelar");
		
		break;
	}
	return 1;
}
