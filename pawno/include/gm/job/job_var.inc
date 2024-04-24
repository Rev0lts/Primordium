enum JOBS_ENUM
{
	jobID,
	jobType,
	Float:jobX,
	Float:jobY,
	Float:jobZ,
	jobInt,
	jobVW,
	jobPickUp,
	jobOn
}
new JobsInfo[MAX_JOBS][JOBS_ENUM];

//	TRABALHOS
#define MAX_JOBS_TYPES 12
new jobs_names[MAX_JOBS_TYPES][32] = {
	"Nenhum",
	"Caminhoneiro",
	"Mecânico",
	"Motorista de táxi",
	"Traficante de drogas",
	"Armeiro",
	"Entregador de pizza",
	"Ladrão de carros",
	"Lenhador",
	"Lixeiro",
	"Mineiro",
	"Pescador"
};