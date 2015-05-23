#ifndef CDEFINE_H
#define CDEFINE_H


class Define  
{
public:

    // flags and boolean
    enum
    {
		I_OFF   = 0,
		I_ON    = 1,
		I_FALSE = 0,
		I_TRUE  = 1,
		I_NO    = 0,
		I_YES   = 1
    };

    // actions on bits
    enum
    {
		I_BIT_NO_CHANGE,
		I_BIT_SET_ON,
		I_BIT_SET_OFF,
		I_BIT_TOGGLE
    };

    // address of nodes on rs-485 network
    enum
    {
		I_PC_ADDRESS_KB_HIT = 0x80,
		I_PC_ADDRESS_LB_ACK = 0x81,
		I_LB_ADDRESS = 0x90,
		I_MP_ADDRESS = 0x98,
		I_DS_ADDRESS = 0xA0
    };
    
    // length  conversions
	enum
    {
		I_ENGLISH = 0,
		I_METRIC,
		I_FEET    = 0,
		I_METERS
    };

    // systems
    enum
    {
		I_GEO = 0,
		I_UTM
    };

    // directions
    enum
    {
		I_LEFT = 0,
		I_RIGHT,
		I_STRAIGHT
    };

    // coordinate systems
    enum
    {
      I_UTMN = 0
    };

	// earth quadrants
	// Update DWR 12/19
	enum
	{
		SOUTH = 1,
		NORTH = 2,
		EAST  = 0,
		WEST  = 1
	};

    static const double D_MIN_GRID_SPACING_METERS;
    static const double D_MIN_GRID_SPACING_FEET;
    static const double D_MIN_GRID_SPACING_ACRES;
    static const double D_MIN_GRID_SPACING_HECT;
    static const double D_MIN_GRID_SPACING_MILES;
    static const double D_MIN_GRID_SPACING_KM;

    static double   M2F;    /* meters -> feet */
    static double   F2M;    /* feet to meters */
    static double   KN2MPH; /* knots to mph */
    static double   KM2MI;  /* kilometers to miles */
    static double   MEPS2MIPH;  /* meters/sec to miles/hour */
    static double   MEPS2KMPH;  // m/s to kilometers/hour
    static double   ONE_MILE;	 // 5280.0 feet in mile
    static double   MI2KM;     /* miles to kilometers*/
    static double   I2M;				/* inches to meters */
	static double   M2I;				/* meters to inches */
    static double   M2Y;        /* meters to yards */

    /* area conversions and definitions */
    enum
    {
		I_ACRES = 0,
		I_HECTARES
    };

    // Area stuff
    static double   A2M;
    static double   SQM2A;
    static double   A2H;       /* 1 acre  = 0.40469 hectares */
    static double   SQFT2H;    /* 1 sq ft  = 9.29036e-6 hectares */
    static double   SQM2H;     /* 1 sq meter = 0.0001 hectares*/
    static double   ACRE;      /* 1 acre in sq ft */
    static double   H2A;       /* 1 hectare  = 2.47104 acres */

	/* weight conversions */
	static double   P2K;     // from lbs to kg  kg = P2k*lbs
	static double   OZ2K;    // from dry ounce to kilogram
   	static double   MT2P;    // metric tonnes to Lbs Lbs = MT*MT2P
	static double 	MT2K;    // metric tonnes to Kilograms  MT*MT2K = Kg

    /* volume conversions */
    static double OZ2LT;          // from ounces to liters
    static double GAL2LT;         // gallons to liters liters = gals * GAL2LT
    static double IMPGAL2LT;      // IMPERIAL gallons to liters
    static double PINT2LT;        // US pint to liters
    static double QUART2LT;       // US quart to liters

    /* math constants */
    static double   PI; /* Pi */
    static double   TWOPI;
    static double   HALFPI;


    /* space geodetic stuff */
    enum
    {
		I_MINSAT = 4,
		I_MAXSAT = 32,
		I_MAXAGE = 56   // UpDate DWR 12/19
    };

    //static double   LIGHT;      /* Speed of light  (m/s) */
    static double   R2D;   /* Scale radians to degrees */
    static double   D2R;   /* Scale degrees to radians */
    static double   SEC2RAD; /* Scale arcseconds to radians */

    /*  WGS-84 constants */
    static double   EARTH_A;
    static double   EARTH_B;

    /*  static double   EARTH_A 6378206.4e0*/ /* clark 1866, birdseye 1929 spheroid */
    /*  static double   EARTH_B 6356584.8e0*/

    static double   EARTH_R;
    static double   EARTH_A2;
    static double   EARTH_B2;
    static double   EARTH_R2;
    static double   ESQ;
    //static double   EARTH_E;

    // disk size constants
    enum
    {
		I_ONE_MEG = 1024 // this is one meg in KB 1024kb = 1 meg
    };

    static double   SMALL;
    static double   TINY;
    static double   LARGE;

    // Time stuff
    static double SECS_IN_24HR;

    static const char AC_DIR_LEFT[12];
    static const char AC_DIR_RIGHT[12];
    static const char AC_DIR_CENTERED[12];

	// epsilons
	static double EPSILON_PIVDIAM;

    static double GOLDEN_RATIO_CONJUGATE;

    enum AlignH
    {
        align_h_ctr = 1,
        align_h_lft = 2,
        align_h_rht = 4
    };

    enum AlignV
    {
        align_v_ctr = 8,
        align_v_top = 16,
        align_v_btm = 32
    };

};

#endif // CDEFINE_H
