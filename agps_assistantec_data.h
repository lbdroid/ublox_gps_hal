#ifndef __AGPS_ASSISTANTEC_DATA_H__
#define __AGPS_ASSISTANTEC_DATA_H__ //!< multiple inclusion guard

#ifndef uint8
 #define uint8	uint8_t 
#endif
#ifndef uint16
 #define uint16	uint16_t
#endif
#ifndef uint32
 #define uint32	uint32_t
#endif
#ifndef int8
 #define int8	int8_t 
#endif
#ifndef int16
 #define int16	int16_t
#endif
#ifndef int32
 #define int32	int32_t
#endif

#define MN_MAX_DGPS_CORRECTION_SAT_INFO  (8)
#define MN_MAX_NAVIGATION_MODEL_SAT_INFO  (8)
#define MN_MAX_ALMANAC_SAT_INFO  (8)
#define MN_MAX_ACQUISITION_SAT_INFO  (8)
#define MN_MAX_GPS_TOW_ASSIST  (8)
typedef struct 
{
   uint16 primaryScramblingCode;
} mn_agps_PrimaryCPICH_Info;

typedef struct 
{
  uint8 referenceIdentity;
} mn_agps_UtranGPSReferenceTime_tdd_t;
typedef struct 
{
  mn_agps_PrimaryCPICH_Info referenceIdentity;
} mn_agps_UtranGPSReferenceTime_fdd_t;
typedef struct 
{
  uint16 ms_part;
  uint32 ls_part;
} mn_agps_UtranGpsTimingOfCell_t;
typedef struct mn_agps_GPS_TOW_Assist_tlm_Reserved {
   uint32  numbits;
   uint8 data[1];
} mn_agps_GPS_TOW_Assist_tlm_Reserved;
typedef struct mn_agps_GPS_TOW_Assist_tlm_Message {
   uint32  numbits;
   uint8 data[2];
} mn_agps_GPS_TOW_Assist_tlm_Message;
typedef struct {
  uint8 satID;
  mn_agps_GPS_TOW_Assist_tlm_Message mntlm_Message;
  mn_agps_GPS_TOW_Assist_tlm_Reserved mntlm_Reserved;
  uint8 alert;
  uint8 antiSpoof;
} mn_agps_GpsTowAssistSingle_t;
typedef struct
{
  uint8 NumGpsTowAssist;
  mn_agps_GpsTowAssistSingle_t mnGpsTowAssist[MN_MAX_GPS_TOW_ASSIST];	
} mn_agps_GpsTowAssistList_t;
typedef struct {
  mn_agps_UtranGpsTimingOfCell_t mnutran_GPSTimingOfCell;
  uint8                                                ModeType;//fdd:0 tdd:1
  mn_agps_UtranGPSReferenceTime_fdd_t           fdd;	
  mn_agps_UtranGPSReferenceTime_tdd_t           tdd;
  uint16 sfn;
} mn_agps_UtranGpsReferenceTime_t;
typedef struct 
{
  uint8 butran_GPSReferenceTimePresent;
  uint8 bsfn_tow_UncertaintyPresent;
  uint8 butran_GPS_DriftRatePresent;
  uint8 bgps_TOW_AssistListPresent;

  uint16 gps_Week;
  uint32 gps_tow_1msec;
  mn_agps_UtranGpsReferenceTime_t mnutran_GPSReferenceTime;
  uint32 sfn_tow_Uncertainty;
  uint32 utran_GPS_DriftRate;
  mn_agps_GpsTowAssistList_t mngps_TOW_AssistList;
} mn_agps_UePositioningGpsReferenceTime_t;
typedef struct {
  uint32 latitudeSign;			// sign
  uint32 latitude;				// x * Pi * 2^-23
  int32 longitude;				// x * Pi * 2^-23
  uint32 altitudeDirection;		// sign
  uint16 altitude;				// meter
  uint8 uncertaintySemiMajor;	// 10 * (1.1 ^ x - 1)
  uint8 uncertaintySemiMinor;	// 10 * (1.1 ^ x - 1)
  uint8 orientationMajorAxis;	// x * 2
  uint8 uncertaintyAltitude;	// 45 * (1.025 ^ x - 1)
  uint8 confidence;				// %
} mn_agps_EllipsoidPointAltitudeEllipsoide_t;
typedef struct
{
  uint8 satID;
  uint8 iode;
  uint32 udre;
  int16 prc; //[-2047..2047] changed from int8
  int8 rrc; //[-127..127] changed from int16
  int8 dummy1;//[-127..127] changed from uint8
  int8 dummy2;//[-7..7]changed from uint8
  int8 dummy3;//[-127..127]changed from uint8
  int8 dummy4;//[-7..7]changed from uint8
  
}mn_agps_DgpsCorrectionSatInfoSingle_t;
typedef struct 
{
   uint8 codeOnL2;
   uint8 uraIndex;
   uint8 satHealth;
   uint8 l2Pflag;
   uint32 reserved1;
   uint32 reserved2;
   uint32 reserved3;
   uint16 reserved4;
   uint16 iodc;
   uint8 t_GD;
   uint8 af2;
   uint16 t_oc;
   uint16 af1;
   uint16 c_rs;
   uint32 af0;
   uint16 delta_n;
   uint32 m0;
   uint16 c_uc;
   uint16 c_us;
   uint32 e;
   uint32 a_Sqrt;
   uint16 t_oe;
   uint8 fitInterval;
   uint8 aodo;
   uint16 c_ic;
   uint16 c_is;
   uint32 omega0;
   uint32 i0;
   uint16 c_rc;
   uint16 iDot;
   uint32 omega;
   uint32 omegaDot;
}mn_agps_EphemerisParameter_t;
typedef struct 
{
   uint8 satID;
  uint32 satelliteStatus;
  mn_agps_EphemerisParameter_t mnephemerisParameter;
} mn_agps_NavigationModelSatInfoSingle_t;
typedef struct
{
  uint8 NumDgpsCorrectionSatInfo;
  mn_agps_DgpsCorrectionSatInfoSingle_t mnDgpsCorrectionSatInfo[MN_MAX_DGPS_CORRECTION_SAT_INFO];	
} mn_agps_DgpsCorrectionSatInfoList_t;
typedef struct 
{
  mn_agps_EllipsoidPointAltitudeEllipsoide_t mnellipsoidPointAltitudeEllipsoide;
} mn_agps_UePositioningGpsReferenceLocation_t;
typedef struct
{
  uint8 NumNavigationModelSatInfo;
  mn_agps_NavigationModelSatInfoSingle_t mnNavigationModelSatInfo[MN_MAX_NAVIGATION_MODEL_SAT_INFO];	
} mn_agps_NavigationModelSatInfoList_t;
typedef struct {
   uint32 gps_TOW;
   uint32 statusHealth;
   mn_agps_DgpsCorrectionSatInfoList_t mndgps_CorrectionSatInfoList;
} mn_agps_UePositioningGpsDgpsCorrections_t;
typedef struct 
{
   mn_agps_NavigationModelSatInfoList_t mnNavigationModelSatInfoList;
} mn_agps_UePositioningGpsNavigationModel_t;
typedef struct 
{
   uint8 alfa0;
   uint8 alfa1;
   uint8 alfa2;
   uint8 alfa3;
   uint8 beta0;
   uint8 beta1;
   uint8 beta2;
   uint8 beta3; 
} mn_agps_UePositioningGPSIonosphericModel_t;
typedef struct 
{
   uint32 a1;
   uint32 a0;
   uint8 t_ot;
   uint8 wn_t;
   uint8 delta_t_LS;
   uint8 wn_lsf;
   uint8 dn;
   uint8 delta_t_LSF;
} mn_agps_UePositioningGPSUtcModel_t;
typedef struct {
   uint8 dataID;
   uint8 satID;
   uint16 e;
   uint8 t_oa;
   uint16 deltaI;
   uint16 omegaDot;
   uint8 satHealth;
   uint32 a_Sqrt;
   uint32 omega0;
   uint32 m0;
   uint32 omega;
   uint16 af0;
   uint16 af1; 
} mn_agps_GpsAlmanacSatInfoSingle_t;
typedef struct
{
	uint8 NumAlmanacSatInfo;
	mn_agps_GpsAlmanacSatInfoSingle_t mnAlmanacSatInfo[MN_MAX_ALMANAC_SAT_INFO];	
} mn_agps_GpsAlmanacSatInfoList_t;
typedef struct 
{
   uint8 wn_a;
   mn_agps_GpsAlmanacSatInfoList_t mnalmanacSatInfoList;
   uint32 sv_GlobalHealth;
} mn_agps_UePositioningGPSAlmanac_t;
typedef struct 
{
  uint8 azimuth;
  uint8 elevation;
} mn_agps_AzimuthAndElevation_t;
typedef struct 
{
   int8 doppler1stOrder;
   uint32 dopplerUncertainty;
} mn_agps_ExtraDopplerInfo_t;
typedef struct 
{
  uint8 satID;
  int16 doppler0thOrder;
  mn_agps_ExtraDopplerInfo_t mnextraDopplerInfo;
  uint16 codePhase;
  uint8 integerCodePhase;
  uint8 gps_BitNumber;
  uint32 codePhaseSearchWindow;
  mn_agps_AzimuthAndElevation_t mnazimuthAndElevation;
} mn_agps_AcquisitionSatInfoSingle_t;
typedef struct
{
  uint8 NumAcquisitionSatInfo;
  mn_agps_AcquisitionSatInfoSingle_t mnAcquisitionSatInfo[MN_MAX_ACQUISITION_SAT_INFO];	
} mn_agps_AcquisitionSatInfoList_t;
typedef struct 
{
  uint8 butran_GPSReferenceTimePresent;
  uint32 gps_ReferenceTime;
  mn_agps_UtranGpsReferenceTime_t mnutran_GPSReferenceTime;
  mn_agps_AcquisitionSatInfoList_t mnsatelliteInformationList;
} mn_agps_UePositioningGPSAcquisitionAssistance_t;
#define MN_MAX_BAD_SAT (16)
typedef struct  
{
  uint16      NumElem;                  
  uint8       elem[MN_MAX_BAD_SAT];
} mn_agps_UePositioningBadSatList_t;

typedef struct
{
    uint16                                       valid; /*bit0=1 mean reference_time valid*/
    																										/*bit1=1 mean reference_location valid*/
																										    /*bit2=1 mean dgps_corrections valid*/
																										    /*bit3=1 mean navigation_model valid*/
																										    /*bit4=1 mean ionospheric_model valid*/
																										    /*bit5=1 mean utc_model valid*/
																										    /*bit6=1 mean almanac valid*/
																										    /*bit7=1 mean acquisition_assistance valid*/
																										    /*bit8=1 mean bad_sat_list valid*/
    mn_agps_UePositioningGpsReferenceTime_t			reference_time;
    mn_agps_UePositioningGpsReferenceLocation_t		reference_location;
    mn_agps_UePositioningGpsDgpsCorrections_t		dgps_corrections;
    mn_agps_UePositioningGpsNavigationModel_t		navigation_model;
    mn_agps_UePositioningGPSIonosphericModel_t		ionospheric_model;
    mn_agps_UePositioningGPSUtcModel_t				utc_model;
    mn_agps_UePositioningGPSAlmanac_t				almanac;
    mn_agps_UePositioningGPSAcquisitionAssistance_t	acquisition_assistance;
    mn_agps_UePositioningBadSatList_t				bad_sat_list;
}agps_assist_t;

typedef struct
{
   uint8 satelliteID;
   uint8 c_N0;
   int32 doppler;//corrected from uint32
   uint16 wholeGPS_Chips;
   uint16 fractionalGPS_Chips;
   uint32 multipathIndicator;
   uint8 pseudorangeRMS_Error;
}APP_MN_AGPS_MeasurementParam_t;

typedef  struct
{
    uint8 NumGpsMeasurementParam;
    APP_MN_AGPS_MeasurementParam_t  GpsMeasurementParam[16];//maxSat=16
}APP_MN_AGPS_MeasurementParamList_t;

typedef struct{
	uint8 result;
	uint8 ep_type;   
	uint8 calc_3d_positon;
	uint8 lat_sign;
	uint32 gps_tow_msec;
	uint32 latitude;
	int32 longitude; //corrected from uint32
	uint8 uncert_code;
	uint8 confidence;
	uint8 uncert_smi;
	uint8 uncert_sma;
	uint8 orientation;
	uint8 alt_dir;
	uint32 alt;
	uint8 uncert_alt;
	APP_MN_AGPS_MeasurementParamList_t measurement_paramlist;
}agps_report_msg_t;

typedef struct{
        	uint8 satID;
	uint8 iode;
}sat_data_info_t;

typedef struct{
	uint8 error_cause;
	uint8 almanceRequest;
	uint8 utcModelRequest;
                uint8 ionosphericModelRequest;
	uint8 navigationModelRequest;
	uint8 dgpsCorrectionsRequest;
	uint8 referenceLocationRequest;
	uint8 referenceTimeRequest;
	uint8 acquisitionAssistanceRequest;
	uint8 realTimeIntegrityRequest;
	uint8 addition_data_flag;
	uint8 gps_week;
	uint8 gps_toe;
	uint8 tToeLimit;
	uint8 sat_data_info_num;
                sat_data_info_t sat_data[32];
}agps_report_fail_msg_t;

#endif // __AGPS_ASSISTANTEC_DATA_H__
