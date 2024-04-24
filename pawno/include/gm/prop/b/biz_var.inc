new biz_prod_types[22] =
	{-1, 2, 23, 21, 3, -1, 0, 11, -1, 4, 4, 4, 12, 15, -1, 1, 1, 1, 1, 1, 22, -1};
	
new biz_prod_point[22] =
	{0, 5, 5, 10, 5, 0, 10, 3, 0, 3, 3, 3, 10, 20, 0, 5, 5, 5, 5, 5, 10, 0};

#define MAX_BIZ_TYPES 25
new biz_types[MAX_BIZ_TYPES][64] = {
    "Outros",
    "Bar / Clube",
    "Loja 24/7",
    "Munição",
    "Loja de Roupas",
    "Barbearia",
    "Posto de Combustível",
    "Oficina Mecânica",
    "Banco",
    "Concessionária de Carros",
    "Salão de Veículos Aquáticos",
    "Salão de Veículos Aéreos",
    "Estúdio de Tuning",
    "Agência de Publicidade",
    "Emissão de Licenças",
    "Well Stacked Pizza",
    "Cluckin' Bell",
    "Burger Shot",
    "Rusty Brown's Ring Donuts",
    "Restaurante",
    "Construtora",
    "Farmácia",
    "Casa de Penhores",
    "Loja de Celulares",
    // "Armazém",
    //"Lavanderia",
    "Hipermercado"
};


#define LIMIT_MAX_PRODUCTS (12)
new const max_product[] =
	{4, 4, 4, 3, 12, 8, 12};

new const name_order[][][32] =
{
    // Comentários removidos foram substituídos por exemplos genéricos.
    {"Buster", "Double D-Luxe", "Full Rack", "Refeição com Salada", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio"},
    {"Refeição Pequena Clukin", "Refeição Grande Clukin", "Refeição Enorme Clukin", "Refeição com Salada", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio"},
    {"Refeição Infantil Moo", "Torre de Carne", "Pilha de Carne", "Refeição com Salada", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio"},
    {"Porção Pequena", "Porção Média", "Porção Grande", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio", "Vazio"},
    {"Cartão SIM", "Walkie talkie", "Máscara", "Isqueiro", "Cigarros", "Dados", "Galão", "Caneta", "Cartas de Jogar", "Câmera", "Adesivo para Veículo", "Flores"},
    {"Cerveja", "Vodka", "Cola", "Água", "Uísque", "Conhaque", "Refrigerante", "Vinho", "Vazio", "Vazio", "Vazio", "Vazio"},
    {"Maconha Medicinal", "Kit de Primeiros Socorros", "Fósforo Vermelho", "Cetona de Metilbenzila", "Metilamina", "Heliotropina", "Efetamina", "Camaleão Mineral", "Clorofórmio", "Éter", "Cloreto de Hidrogênio", "Metanol"}
};


new const model_order[7][12] =
{
    {2218, 2219, 2220, 2355, 0, 0, 0, 0, 0, 0, 0, 0},
    {2215, 2216, 2217, 2353, 0, 0, 0, 0, 0, 0, 0, 0},
    {2213, 2214, 2212, 2354, 0, 0, 0, 0, 0, 0, 0, 0},
    {2221, 2223, 2222, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {19792, 19942, 19801, 19998, 19897, 1906, 1650, 3027, 11748, 367, 2656, 325}, // -sim -���� -INV_CARSOUND -
    {2958, 1668, 1546, 1455, 1520, 19820, 19346, 1664, 0, 0, 0, 0},
    {1578, 11736, 2709, 2709, 2709, 2709, 2709, 2709, 2709, 2709, 2709, 2709}
};

new const biz_prices[7][12] =
{
    {100, 100, 200, 300, 0, 0, 0, 0, 0, 0, 0, 0},
    {100, 100, 200, 300, 0, 0, 0, 0, 0, 0, 0, 0},
    {100, 100, 200, 300, 0, 0, 0, 0, 0, 0, 0, 0},
    {100, 100, 200, 300, 0, 0, 0, 0, 0, 0, 0, 0},
    {300, 500, 1500, 50, 100, 100, 1000, 50, 100, 1500, 3000, 150},
    {100, 200, 100, 50, 300, 500, 100, 300, 0, 0, 0, 0},
    {1000, 1500, 1000, 1000, 1500, 1000, 1500, 1000, 1000, 1000, 2000, 1500}
};