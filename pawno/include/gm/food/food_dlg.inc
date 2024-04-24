Dialog:BuyStandFood(playerid, response, listitem, inputtext[]) //tåntî
{
	if (!response)	return 1;

	new price_food[4] = {PRICE_BURGER, PRICE_PIZZA, PRICE_CHICKEN, PRICE_HOTDOG}, name_food[4][16] = {"hambúrguer", "pizza", "frango", "cachorro-quente"};

	if (GetCash(playerid) < price_food[listitem])				return SendNotCash(playerid);
	if (GiveInvItem(playerid, INV_BURGER + listitem, 1) == -1)	return SendErrorMessage(playerid, "O inventário está cheio.");

	SendSyntaxMessage(playerid, "Você comprou um(a) %s por $%i.", name_food[listitem], price_food[listitem]);
	GiveCash(playerid, -price_food[listitem]);
	return 1;
}
