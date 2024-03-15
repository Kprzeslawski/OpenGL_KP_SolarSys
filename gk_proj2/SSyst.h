#pragma once
#include "libndef.h"


class SSyst
{
	//scaled for earth to fit on display
	// so eart rad should be around 0.9
	// scale 149597887~0.9
	// 166219874~1

	const DATA_PREC Dsun = 0.00837866;

	// planetary radius
	const DATA_PREC Rmerq = 0.348;
	const DATA_PREC Rven = 0.650;
	const DATA_PREC Reart = 0.9;
	const DATA_PREC Rmar = 1.371;
	const DATA_PREC Rjup = 4.683;
	const DATA_PREC Rsat = 8.583;
	const DATA_PREC Rura = 17.272;
	const DATA_PREC Rnept = 27.062;
	//km!!!!!

	//planetary diameter
	const DATA_PREC Dmerq = 1.985e-6;
	const DATA_PREC Dven = 2.935e-5;
	const DATA_PREC Deart = 3.594e-5;
	const DATA_PREC Dmar = 3.862e-6;
	const DATA_PREC Djup = 8.411e-4;
	const DATA_PREC Dsat = 7.0e-4;
	const DATA_PREC Dura = 5.224e-4;
	const DATA_PREC Dnept = 2.962e-4;
	//km

public:
	SSyst();

	/*
	funkcja zwracjaca kat ramienia od slonca do planety 
	w zaleznosci od jej dystansu od centrum (czyli okresu obiegu)
	oraz przesuniecia w czsie
	*/
	DATA_PREC positionvector(DATA_PREC time, DATA_PREC planetrad);


	DATA_PREC sunD();

	//diameters getters
	DATA_PREC merqD();
	DATA_PREC venD();
	DATA_PREC eartD();
	DATA_PREC marD();
	DATA_PREC jupD();
	DATA_PREC satD();
	DATA_PREC uraD();
	DATA_PREC neptD();

	//radius getters
	DATA_PREC merqR();
	DATA_PREC venR();
	DATA_PREC eartR();
	DATA_PREC marR();
	DATA_PREC jupR();
	DATA_PREC satR();
	DATA_PREC uraR();
	DATA_PREC neptR();




};

