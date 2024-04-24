Interior_OnGameModeInit()
{
	buy_interior = CreateMenu("_", 1, 20.0, 160.0, 80.0);
	AddMenuItem(buy_interior, 0, "> PROXIMO");
	AddMenuItem(buy_interior, 0, "> VOLTAR");
	AddMenuItem(buy_interior, 0, "> SELECIONAR");
	AddMenuItem(buy_interior, 0, "> SAIR");
	return 1;
}