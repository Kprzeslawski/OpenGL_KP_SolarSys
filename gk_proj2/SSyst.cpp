#include "SSyst.h"

SSyst::SSyst(){

}

DATA_PREC SSyst::positionvector(DATA_PREC time, DATA_PREC planetrad) {
	
	//t = sqrt(4pi*r3/(G*Msun))
	
	//zamiast 4pi oraz g* msun uzyjemy wartosic w taki sposob aby otrzymac wartosc 365 (dni) dla ziemi
	//podawana wartosc czasu bedziemy dzielic modulo przez okres a nastepnie
	//jeszcze raz z uwzglednieniem okresu aby otrzymac kat w rad
	//aby otrzymac faze

	//po przeksztalceniu t = r^(3/2) * const
	//365 = r(ziemi)^(3/2) * const
	//const = 365/r(ziemi)^(3/2)

	DATA_PREC per = pow(planetrad,1.5) * 427.493;
	return (time - floor( time / per ) * per) * 2 * M_PI / per ;
}

DATA_PREC SSyst::sunD() { return Dsun; }

DATA_PREC SSyst::merqD() { return Dmerq; }
DATA_PREC SSyst::venD() { return Dven; }
DATA_PREC SSyst::eartD() { return Deart; }
DATA_PREC SSyst::marD() { return Dmar; }
DATA_PREC SSyst::jupD() { return Djup; }
DATA_PREC SSyst::satD() { return Dsat; }
DATA_PREC SSyst::uraD() { return Dura; }
DATA_PREC SSyst::neptD() { return Dnept; }

DATA_PREC SSyst::merqR() { return Rmerq; }
DATA_PREC SSyst::venR() { return Rven; }
DATA_PREC SSyst::eartR() { return Reart; }
DATA_PREC SSyst::marR() { return Rmar; }
DATA_PREC SSyst::jupR() { return Rjup; }
DATA_PREC SSyst::satR() { return Rsat; }
DATA_PREC SSyst::uraR() { return Rura; }
DATA_PREC SSyst::neptR() { return Rnept; }