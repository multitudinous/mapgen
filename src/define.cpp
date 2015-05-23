#include "define.h"


// Grid spacing limits
const double Define::D_MIN_GRID_SPACING_METERS = 60;
const double Define::D_MIN_GRID_SPACING_FEET =   200;
const double Define::D_MIN_GRID_SPACING_ACRES =  1;
const double Define::D_MIN_GRID_SPACING_HECT =   .5;
const double Define::D_MIN_GRID_SPACING_MILES =  .038;
const double Define::D_MIN_GRID_SPACING_KM =     .060;

/* length  conversions */
double Define::M2F 		= 3.280839895;    /* meters -> feet */
double Define::F2M 		= 0.3048006096;   /* feet to meters */
double Define::KN2MPH	=	1.150779451;    /* knots to mph */
double Define::KM2MI	  =	0.621371192;    /* kilometers to miles */
double Define::MEPS2MIPH = 2.2369362912; /* meters/sec to miles/hour */
double Define::MEPS2KMPH = 3.6000;       // meters/sec to kilometers/hour
double Define::ONE_MILE 	= 5280.0;  		  // 5280.0 feet in mile
double Define::MI2KM	  	= 1.60934400061; /* miles to kilometers*/
double Define::I2M       = 0.0254;				/* inches to meters */
double Define::M2I       = 39.37007874; 	/* meters to inches */
double Define::M2Y       = 1.0936;       /* meters to yards */

/* area conversions and definitions */
double Define::ACRE      = 43560.0;      /* 1 acre in sq ft */
double Define::H2A       = 2.47104;       /* 1 hectare  = 2.47104 acres */
double Define::A2H       = 0.40469;       /* 1 acre  = 0.40469 hectares */
double Define::SQFT2H    = 9.29036e-6;    /* 1 sq ft  = 9.29036e-6 hectares */

/* weigth conversions */
double Define::P2K = 0.45359237;    // from lbs to kg  kg = P2k*lbs 
double Define::OZ2K = 0.0283495;    // from dry ounce to kg  kg = OZ2K*dry ounces
double Define::MT2P = 2204.6226;    // metric tonnes to Lbs Lbs = MT*MT2P
double Define::MT2K = 1000.0;       // metric tonnes to Kilograms  MT*MT2K = Kg

/* volume conversions */
double Define::OZ2LT = 2.95735e-2;  // from ounces to liters
double Define::GAL2LT = 3.78541;    // gallons to liters liters = gals * GAL2LT
double Define::IMPGAL2LT = 4.546;   // IMPERIAL gallons to liters
double Define::PINT2LT = 0.473176;  // US pint to liters
double Define::QUART2LT = 0.94635;  // US quart to liters

/* math constants */
double Define::PI      = 3.1415926535897932; /* Pi */
double Define::TWOPI   = (2.0*Define::PI);
double Define::HALFPI  = (0.5*Define::PI);

/* space geodetic stuff */
double Define::R2D    =(180.0/Define::PI);// Scale radians to degrees
double Define::D2R    =(Define::PI/180.0);// Scale degrees to radians
double Define::SEC2RAD=(Define::PI/648000.0);// Scale arcseconds to radians

/*  WGS-84 constants */
double Define::EARTH_A = 6378137.0e0;
double Define::EARTH_B = 6356752.314e0;

double Define::EARTH_R  =  6371000.0e0;
double Define::EARTH_A2 =  (Define::EARTH_A*Define::EARTH_A);
                            // 4.068063316e13
double Define::EARTH_B2 =  (Define::EARTH_B*Define::EARTH_B);
                            // 4.04083000e13
double Define::EARTH_R2 =  (Define::EARTH_R*Define::EARTH_R);
                            // 4.05896410e13
double Define::ESQ      =  (Define::EARTH_A2 - Define::EARTH_B2)
                            /Define::EARTH_A2;
                            // 6.69438007e-3


double Define::SMALL         = 1e-2;
double Define::TINY          = 1e-4;
double Define::LARGE         = 1e4;

double Define::A2M     = 63.6150344641;
double Define::SQM2A   = 2.47105381465e-4;
double Define::SQM2H   = 1.00000000000e-4;

double Define::SECS_IN_24HR = 86400;

double Define::EPSILON_PIVDIAM = 0.0001;

#ifndef UNDER_CE
const char Define::AC_DIR_LEFT[] =    "Left";
const char Define::AC_DIR_RIGHT[]=    "Right";
const char Define::AC_DIR_CENTERED[]= "Centered";
#endif


double Define::GOLDEN_RATIO_CONJUGATE = 0.618033988749895;
