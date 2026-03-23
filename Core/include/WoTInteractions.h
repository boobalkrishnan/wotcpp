#ifndef WOTINTERACTIONS
#define WOTINTERACTIONS


union ThingInteractionValue {
  bool boolean;
  double number;
  char stringin[255];
  uint64_t integer;
};

typedef uint8_t ThingInteractionValueType;
enum{
  VAL_NO_STATE=0,
  VAL_BOOLEAN,
  VAL_NUMBER,
  VAL_STRING,
  VAL_INTEGER,
  VAL_OBJECT
};

typedef uint8_t InteractionUnits;

enum{
	eNONE=0,
	eCELCUIS,
	eFARHENITE,
	ePERCENT,
	eMILLISECONDS
};

typedef uint8_t ThingInteractionType;
enum{
	eALARM=0,
	eBOOLEAN,
	eBRIGHTNESS,
	eCOLOR,
	eCOLOR_TEMPERATURE,
	eCURRENT,
	eFREQUENCY,
	eHEATING_COOLING,
	eIMAGE,
	eINSTANTANEOUS_POWER,
	eLEAK,
	eLEVEL,
	eLOCKED,
	eMOTION,
	eON_OFF,
	eOPEN,
	ePUSHED,
	eTARGET_TEMPERATURE,
	eTEMPERATURE,
	eTHERMOSTAT,
	eVIDEO,
	eVOLTAGE,
	eKEEP_LAST
};

class WoTForms
{
public:
  std::string href;
  std::string type;
  std::string rel;
  std::string anchor;
  std::string sizes;
  std::string hreflang;
  WoTForms &
  operator= (const WoTForms &other)
  {
    if (this == &other)
      { // Handle self-assignment
        return *this;
      }
    else
      {
        this->href = other.href;
        this->type = other.type;
        this->rel = other.rel;
        this->anchor = other.anchor;
        this->sizes = other.sizes;
        this->hreflang = other.hreflang;
        return *this;
      }
  }
};

#endif /* WOTINTERACTIONS */
