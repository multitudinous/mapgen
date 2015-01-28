#include "CDefine.h"

CDefine::CDefine()
{

}

CDefine::~CDefine()
{

}

// Grid spacing limits
const double CDefine::D_MIN_GRID_SPACING_METERS = 60;
const double CDefine::D_MIN_GRID_SPACING_FEET =   200;
const double CDefine::D_MIN_GRID_SPACING_ACRES =  1;
const double CDefine::D_MIN_GRID_SPACING_HECT =   .5;
const double CDefine::D_MIN_GRID_SPACING_MILES =  .038;
const double CDefine::D_MIN_GRID_SPACING_KM =     .060;

/* length  conversions */
double CDefine::M2F 		= 3.280839895;    /* meters -> feet */
double CDefine::F2M 		= 0.3048006096;   /* feet to meters */
double CDefine::KN2MPH	=	1.150779451;    /* knots to mph */
double CDefine::KM2MI	  =	0.621371192;    /* kilometers to miles */
double CDefine::MEPS2MIPH = 2.2369362912; /* meters/sec to miles/hour */
double CDefine::MEPS2KMPH = 3.6000;       // meters/sec to kilometers/hour
double CDefine::ONE_MILE 	= 5280.0;  		  // 5280.0 feet in mile
double CDefine::MI2KM	  	= 1.60934400061; /* miles to kilometers*/
double CDefine::I2M       = 0.0254;				/* inches to meters */
double CDefine::M2I       = 39.37007874; 	/* meters to inches */
double CDefine::M2Y       = 1.0936;       /* meters to yards */

/* area conversions and definitions */
double CDefine::ACRE      = 43560.0;      /* 1 acre in sq ft */
double CDefine::H2A       = 2.47104;       /* 1 hectare  = 2.47104 acres */
double CDefine::A2H       = 0.40469;       /* 1 acre  = 0.40469 hectares */
double CDefine::SQFT2H    = 9.29036e-6;    /* 1 sq ft  = 9.29036e-6 hectares */

/* weigth conversions */
double CDefine::P2K = 0.45359237;    // from lbs to kg  kg = P2k*lbs 
double CDefine::OZ2K = 0.0283495;    // from dry ounce to kg  kg = OZ2K*dry ounces
double CDefine::MT2P = 2204.6226;    // metric tonnes to Lbs Lbs = MT*MT2P
double CDefine::MT2K = 1000.0;       // metric tonnes to Kilograms  MT*MT2K = Kg

/* volume conversions */
double CDefine::OZ2LT = 2.95735e-2;  // from ounces to liters
double CDefine::GAL2LT = 3.78541;    // gallons to liters liters = gals * GAL2LT
double CDefine::IMPGAL2LT = 4.546;   // IMPERIAL gallons to liters
double CDefine::PINT2LT = 0.473176;  // US pint to liters
double CDefine::QUART2LT = 0.94635;  // US quart to liters

/* math constants */
double CDefine::PI      = 3.1415926535897932; /* Pi */
double CDefine::TWOPI   = (2.0*CDefine::PI);
double CDefine::HALFPI  = (0.5*CDefine::PI);

/* space geodetic stuff */
double CDefine::R2D    =(180.0/CDefine::PI);// Scale radians to degrees
double CDefine::D2R    =(CDefine::PI/180.0);// Scale degrees to radians
double CDefine::SEC2RAD=(CDefine::PI/648000.0);// Scale arcseconds to radians

/*  WGS-84 constants */
double CDefine::EARTH_A = 6378137.0e0;
double CDefine::EARTH_B = 6356752.314e0;

double CDefine::EARTH_R  =  6371000.0e0;
double CDefine::EARTH_A2 =  (CDefine::EARTH_A*CDefine::EARTH_A);
                            // 4.068063316e13
double CDefine::EARTH_B2 =  (CDefine::EARTH_B*CDefine::EARTH_B);
                            // 4.04083000e13
double CDefine::EARTH_R2 =  (CDefine::EARTH_R*CDefine::EARTH_R);
                            // 4.05896410e13
double CDefine::ESQ      =  (CDefine::EARTH_A2 - CDefine::EARTH_B2)
                            /CDefine::EARTH_A2;
                            // 6.69438007e-3


double CDefine::SMALL         = 1e-2;
double CDefine::TINY          = 1e-4;
double CDefine::LARGE         = 1e4;

double CDefine::A2M     = 63.6150344641;
double CDefine::SQM2A   = 2.47105381465e-4;
double CDefine::SQM2H   = 1.00000000000e-4;

double CDefine::SECS_IN_24HR = 86400;

double CDefine::EPSILON_PIVDIAM = 0.0001;

#ifndef UNDER_CE
const char CDefine::AC_DIR_LEFT[] =    "Left";
const char CDefine::AC_DIR_RIGHT[]=    "Right";
const char CDefine::AC_DIR_CENTERED[]= "Centered";
#endif

