enum CARFIX_ENUM
{
	Float: fixX,
	Float: fixY,
	Float: fixZ,
	fixVW,
	fixInt,
	fixPickUp
}

enum FACTION_ENUM
{
	fID,
	fPickUp,
	fName[32],
	fShortName[8],
	Float:fPosX,
	Float:fPosY,
	Float:fPosZ,
	fSpawnInt,
	fSpawnVW,
	fJoinRank,
	fType,
	fRanks,
	fChatON,
	fColor,
	Float:fUniformX,
	Float:fUniformY,
	Float:fUniformZ,
	fUniformInt,
	fUniformVW,
	Carfix[CARFIX_ENUM],
	
	fPoint,
	fPointTime,
	fON
};
new Factions[MAX_FACTIONS][FACTION_ENUM];
new FactionRanks[MAX_FACTIONS][20][32];

enum RANK_ENUM
{
	rInvite,
	rUninvite,
	rRank,
	r_eRank,
	rSpawn,
	rChat,
	rTow,
	rBodyCam,
	r_eRights,
};
new FRank[MAX_FACTIONS][20][RANK_ENUM];

enum RANK_SETTINGS_DATA
{
	right_id,
	right_name[64],
	right_cmd[32]
};

new RS[9][RANK_SETTINGS_DATA] = {
	{0, "{FFFFFF}Admissão na facção ", "/convidar"},
	{1, "{FFFFFF}Exclusão da facção", "/demitir"},
	{2, "{FFFFFF}Mudança de rank", "/definircargo"},
	{3, "{FFFFFF}Mudança do nome do rank", "/editarnomecargo"},
	{4, "{FFFFFF}Mudando o ponto de spawn", "/editarspawn"},
	{5, "{FFFFFF}Gerenciamento do chat", "/fchat"},
	{6, "{FFFFFF}Enviando veículo para o spawn", "/vfrespawn"},
	{7, "{FFFFFF}Visualização de bodycam", "/mdc"},
	{8, "{FFFFFF}Mudança dos direitos de rank", "/editarcargos"}
};


enum BORT_ENUM
{
    bortOn,
	bortName[32],
	bortObject,
	bortUsing
}
new BortInfo[MAX_PLAYERS][MAX_BORTES][BORT_ENUM];