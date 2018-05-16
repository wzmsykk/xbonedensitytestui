/* Header msg.h for bmd Application */

#if !defined (_STATEERR_H_)
#define _STATEERR_H_

//******************************** DEFINE  ************************

// NVRAM
											//	01234567890123456789012345678901
#define DEFAULT_SERIAL_NUMBER      	"111-222-111                     "
#define DEFAULT_PMT_VALUE    			0
#define DEFAULT_ZMOTOR_OFFSET			128
#define DEFAULT_TMOTOR_OFFSET			33
#define DEFAULT_GAIN						10000
#define DEFAULT_OFFSET					0
#define DEFAULT_PRINTER					0
#define DEFAULT_AVG_MU					2850000
#define DEFAULT_EXP_CNT					0
#define DEFAULT_ACC_MU					0

// OPTION
#define DEFAULT_ERASE_TIME			  		30000		// 30 seconds
#define DEFAULT_ERASE_MAX_COUNT			10													  	
#define DEFAULT_ERASE_MAX_PERCENT		70			// 1/10th of a percent
#define DEFAULT_PMT_SAT_MAX_COUNT		4000													  	
#define DEFAULT_PMT_SAT_MAX_PERCENT		100			// 1/10th of a percent
#define DEFAULT_LOW_SIGNAL_MEAN_COUNT	2900													  	


//******************************** State  ************************
// Main Status (ulMainStatus)

#define ST_SYS_INITIALIZE  	0x00000001
#define ST_SYS_SELF_TEST		0x00000002
#define ST_SYS_SERVICE			0x00000004
#define ST_SYS_REMOTE			0x00000008		
#define ST_SYS_EXAM_START		0x00000010		// Set when calling start PrepareExpose
#define ST_SYS_AUTOCAL			0x00000020
#define ST_SYS_NVRAM_VALID		0x00000040		// Once valid, share memory version is useable
#define ST_SYS_PC_AUTOSCAN		0x00000080		// For manufacturing
#define ST_SYS_EXAM_CANCEL 	0x00000100		// Cancel the scanning
#define ST_SYS_SEND_RAW_DATA  0x00000200
#define ST_SYS_ERROR				0x00001000
#define ST_SYS_BATCH_PRN_DONE	0x00010000		// Remote batch print and send exam done
#define ST_SYS_BATCH_DONE		0x00020000		// Remote batch send exam done 
#define ST_SYS_SM_REFERENCE	0x04000000
#define ST_SYS_SM_HISTOGRAM	0x08000000
#define ST_SYS_SM_PAT_INFO		0x10000000
#define ST_SYS_SM_IMAGE			0x20000000
#define ST_SYS_SM_MIDLINE		0x40000000
#define ST_SYS_SM_RESULT		0x80000000

// ACQ main
#define ST_MAIN_ACQ_BUSY	  	0x0001
#define ST_MAIN_IMAGE_BUSY	  	0x0002
#define ST_MAIN_PRINT_BUSY	  	0x0004
#define ST_MAIN_DEVICE_BUSY 	0x0008


// ACQ status
#define ST_ACQ_INIT				0x0001
#define ST_ACQ_AUTOCALIBRATE	0x0002
#define ST_ACQ_CALIB_ERASE		0x0004
#define ST_ACQ_CALIB_PMT		0x0008
#define ST_ACQ_PREPARE_EXPOSE	0x0010
#define ST_ACQ_EXPOSE_XRAY		0x0020
#define ST_ACQ_COLLECT_DATA	0x0040

// ERASE status
#define ST_ERS_INIT				0x0001
#define ST_ERS_ERASING			0x0002		
#define ST_ERS_SCREEN_ERASED 	0x0004
#define ST_ERS_ERROR			 	0x0008

#define ST_IMAGE_INIT			0x0001
#define ST_IMAGE_PROCESS_DATA	0x0002
#define ST_IMAGE_RECOMPUTE		0x0004
#define ST_IMAGE_BMD				0x0008
#define ST_IMAGE_SCORE			0x0010

#define ST_PRINT_INIT			0x0001
#define ST_PRINT_RESULT			0x0002
#define ST_PRINT_INFO			0x0004
#define ST_PRINT_DEBUG			0x0008

#define ST_DEVICE_INIT				0x0001
#define ST_DEVICE_XRAY_MODULE		0x0010
#define ST_DEVICE_ERASE_MODULE	0x0020
#define ST_DEVICE_OPTICS_MODULE	0x0040
#define ST_DEVICE_FPANEL_MODULE	0x0080

// Image status
#define ST_IMAGE_RESET			0x0001

// Print
#define ST_PRINT_INFO_BUSY		0x0001
#define ST_PRINT_RESULT_BUSY	0x0002


// Device
#define ST_DEVICE_INIT_DONE	0x0001
#define ST_IMAGE_UNKNOWN_CMD	0x0002



// IO status
// Motor status
// Xray status


// Constant for motor status
#define ST_ZMOTOR_HOMING					0x00
#define ST_ZMOTOR_AT_RECEIVE_OFFSET		0x10	// low power
#define ST_ZMOTOR_MOVING_HOME				0x20
#define ST_ZMOTOR_AT_HOME					0x30	// low power
#define ST_ZMOTOR_MOVING_END				0x40
#define ST_ZMOTOR_AT_END					0x50	// low power
#define ST_ZMOTOR_AT_KNOWN_POSITION		0x70	// low power
#define ST_ZMOTOR_ILLEGAL_CMD				0x80
#define ST_ZMOTOR_ERR_FIND_HOME_FLAG	0x90	// low power
#define ST_ZMOTOR_ERR_CLEAR_HOME_FLAG	0xB0 	// low power
#define ST_ZMOTOR_ERR_VERIFY_HOME_FLAG	0xD0	// low power
#define ST_ZMOTOR_UNKNOWN_ERROR			0xF0	// low power
#define ST_ZMOTOR_MASK		  				0xF0	// low power
#define ST_ZMOTOR_ERROR						0x80

#define ST_TMOTOR_MOVING					0x01
#define ST_TMOTOR_AT_HOME					0x03	// low power
#define ST_TMOTOR_ACCELERATING			0x02
#define ST_TMOTOR_SPINNING					0x04
#define ST_TMOTOR_AT_ERASE					0x05	// low power
#define ST_TMOTOR_ILLEGAL_COMMAND		0x08
#define ST_TMOTOR_ERR_FIND_HOME_FLAG	0x09	// low power
#define ST_TMOTOR_ERR_CLEAR_HOME_FLAG	0x0B	// low power
#define ST_TMOTOR_ERR_VERIFY_HOME_FLAG	0x0D	// low power
#define ST_TMOTOR_UNKNOWN_ERROR			0x0F	// low power
#define ST_TMOTOR_MASK		  				0x0F	// low power
#define ST_TMOTOR_ERROR						0x08



// Constant for XRAY status
#define ST_XRAY_READY							0x01
#define ST_XRAY_ON_OFF							0x02
#define ST_XRAY_VOLT_FAIL						0x04
#define ST_XRAY_CURRENT_FAIL					0x08
#define ST_XRAY_ARC_FAIL						0x10
#define ST_XRAY_ERROR_MASK					  	0x1C

// Constant for Simple IO status
#define ST_IO_ERASER_PRESENT 				0x01
#define ST_IO_OPTIX_MODULE_PRESENT 		0x02
#define ST_IO_FRONT_PANEL_PRESENT 		0x04
#define ST_IO_MONOBLOCK_PRESENT			0x08
#define ST_IO_OVERTEMP						0x10










/*************************** ERROR CODE ************************************/

// ERROR CODE
#define ERR_SERIAL_UNKNOWN_CMD									1          
#define ERR_SERIAL_INVALID_TIMEFORMAT							2          
#define ERR_SERIAL_INVALID_DATEFORMAT							3          
#define ERR_SERIAL_MISSING_IMAGE_DATA						   4            
#define ERR_SERIAL_SENDING_IMAGE_DATA							5             
#define ERR_SERIAL_INVALID_MULTIBYTE_CHAR						6             
#define ERR_SERIAL_AUTOCALIBRATING								7     
#define ERR_MAIN_INVALID_COMMON_MSG								8       
#define ERR_MAIN_INVALID_SERVICE_MSG							9       
#define ERR_MAIN_SHMEM_NOT_MAPPED								10      
#define ERR_MAIN_ACQ_BUSY											11      
#define ERR_MAIN_ACQ_NOT_INITIALIZED							12      
#define ERR_MAIN_IMAGE_BUSY										13      
#define ERR_MAIN_IMAGE_NOT_INITIALIZED							14      
#define ERR_MAIN_PRINT_BUSY										15      
#define ERR_MAIN_PRINT_NOT_INITIALIZED							16      
#define ERR_MAIN_DEVICE_BUSY										17      
#define ERR_MAIN_DEVICE_NOT_INITIALIZED						18      
#define ERR_MAIN_COLLECTING_DATA									19      
#define ERR_MAIN_TIMEOUT_LOCATING_UI_PROCESS					20      
#define ERR_MAIN_TIMEOUT_LOCATING_SERIAL_PROCESS			21       
#define ERR_MAIN_TIMEOUT_LOCATING_XRAY_PROCESS				22      
#define ERR_MAIN_TIMEOUT_LOCATING_BMDIO_PROCESS				23     
#define ERR_MAIN_TIMEOUT_LOCATING_MOTOR_PROCESS				24        
#define ERR_MAIN_SERVER_PROCESS_NOT_INITIALIZED 			25        
#define ERR_MAIN_OPEN_OPTIONS_FILE_FAILED						26     
#define ERR_MAIN_ACQ_RESPONSE_TASK_FULL						27     
#define ERR_DEVICE_TIMEOUT_WAITING_SECONDARY_SERVER		28        
#define ERR_SERIAL_TIMEOUT_ACQUIRE_PID							29           
#define ERR_IMAGE_TIMEOUT_ACQUIRE_PID							30           
#define ERR_IMAGE_EMPTY_HISTOGRAM							   31       
#define ERR_IMAGE_INIT_SKIN_EDGES							   32       
#define ERR_IMAGE_ZERO_FINGERS								   33       
#define ERR_IMAGE_THRESHOLD_BACKGROUND						   34       
#define ERR_IMAGE_COMPUTE_MIDLINE							   35       
#define ERR_IMAGE_FIND_JOINTS								      36       
#define ERR_IMAGE_INIT_PROFILE_BONE_EDGES					   37       
#define ERR_IMAGE_FIND_PROFILE_BONE_EDGES					   38       
#define ERR_IMAGE_RECOMPUTE_BMD								   39        
#define ERR_IMAGE_UNKNOWN_CMD										40           
#define ERR_PRINT_UNKNOWN_CMD										41           
#define ERR_PRINT_PAPER_OUT										42       
#define ERR_PRINT_PAPER_JAM										43       
#define ERR_PRINT_COVER_OPEN										44       
#define ERR_PRINT_OFF_LINE											45       
#define ERR_PRINT_TURNED_OFF										46       
#define ERR_PRINT_DISCONN											47       
#define ERR_PRINT_GENERIC											48       
#define ERR_PRINT_UNKNOWN											49       
#define ERR_ACQ_UNKNOWN_CMD										50           
#define ERR_ACQ_CANNOT_ATTACH_PROXY								51           
#define ERR_ACQ_TIMEOUT_GET_MAIN_PID							52           
#define ERR_DEVICE_SEND_MSG_TO_SECONDARY_SERVER				53           
#define ERR_ACQ_SEND_MSG_TO_SECONDARY_SERVER					54           
#define ERR_ACQ_DEWARP_FILE_OPEN_OR_READ						55           
#define ERR_ACQ_DEWARP_FILE_NOT_MATCH							56           
#define ERR_ACQ_WEDGE_NOT_FOUND									57       
#define ERR_ACQ_WEDGE_NOT_CORRECT								58       
#define ERR_ACQ_SCREEN_IS_NOT_ERASE								59       
#define ERR_ACQ_INVALID_NVRAM_VALUE    						60       
#define ERR_ACQ_ERS_SEND_MSG_FAIL								61       
#define ERR_ACQ_ERASE_NOT_INIT									62       
#define ERR_XRAY_VOLT_FAIL											63           
#define ERR_XRAY_CURRENT_FAIL										64           
#define ERR_XRAY_ARC_FAIL											65           
#define ERR_XRAY_UNKNOWN_MSG										66           
#define ERR_XRAY_UNKNOWN_ERROR									67           
#define ERR_XRAY_TIMEOUT_WAITING_READY							68           
#define ERR_XRAY_TURN_ON_FAIL										69           
#define ERR_XRAY_TURN_OFF_FAIL									70           
#define ERR_XRAY_CLOCK_SETRES										71           
#define ERR_XRAY_CREATE_TIMER										72           
#define ERR_XRAY_EXPOSE_KEY_IS_SHORTED							73           
#define ERR_XRAY_CANCEL_BEFORE_COMPLETED						74       
#define ERR_XRAY_DISABLE_KEYPAD_FAIL							75       
#define ERR_XRAY_ENABLE_KEYPAD_FAIL								76       
#define ERR_MOTOR_UNKNOWN_MSG  									77           
#define ERR_MOTOR_T_ILLEGAL_COMMAND								78           
#define ERR_MOTOR_T_FIND_HOME_FLAG								79           
#define ERR_MOTOR_T_CLEAR_HOME_FLAG								80           
#define ERR_MOTOR_T_VERIFY_HOME_FLAG							81           
#define ERR_MOTOR_T_UNKNOWN_ERROR								82           
#define ERR_MOTOR_T_STOP_TIMEOUT									83           
#define ERR_MOTOR_T_AT_HOME_TIMEOUT								84           
#define ERR_MOTOR_T_SPIN_TIMEOUT									85           
#define ERR_MOTOR_T_ERASE_TIMEOUT								86           
#define ERR_MOTOR_T_UNKNOWN_ERRCODE								87           
#define ERR_MOTOR_Z_MOTOR_NOT_SPINNING							88           
#define ERR_MOTOR_Z_ILLEGAL_CMD									89           
#define ERR_MOTOR_Z_FIND_HOME_FLAG								90           
#define ERR_MOTOR_Z_CLEAR_HOME_FLAG								91           
#define ERR_MOTOR_Z_VERIFY_HOME_FLAG							92           
#define ERR_MOTOR_Z_UNKNOWN_ERROR								93           
#define ERR_MOTOR_Z_HOMING_TIMEOUT								94           
#define ERR_MOTOR_Z_TILL_HOME_TIMEOUT							95           
#define ERR_MOTOR_Z_STOP_TIMEOUT									96           
#define ERR_MOTOR_Z_END_TIMEOUT									97           
#define ERR_MOTOR_Z_TILL_END_TIMEOUT							98           
#define ERR_MOTOR_Z_OFFSET_TIMEOUT								99           
#define ERR_MOTOR_Z_UNKNOWN_ERRCODE								100          
#define ERR_MOTOR_RESET_ERROR										101          
#define ERR_IO_UNKNOWN_CMD_SYSTEM_MSG							102       
#define ERR_IO_UNKNOWN_ACQ_INPUT_MSG							103       
#define ERR_IO_UNKNOWN_FPLED_MSG									104       
#define ERR_IO_UNKNOWN_ERASER_MSG								105       
#define ERR_IO_UNKNOWN_MSG											106       
#define ERR_IO_LASER_VALUE_INVALID								107       
#define ERR_IO_PMT_VALUE_INVALID									108       
#define ERR_IO_NO_ERASER_MODULE									109      
#define ERR_IO_NO_OPTIX_MODULE									110      
#define ERR_IO_NO_FPANEL_MODULE									111      
#define ERR_IO_NO_XRAY_MODULE										112      
#define ERR_IO_FAIL_LASER_TEST									113      
#define ERR_IO_FAIL_PMT_TEST										114      
#define ERR_NVRAM_INVALID_READ_REQUEST							115          
#define ERR_NVRAM_INVALID_WRITE_REQUEST						116          
#define ERR_NVRAM_NOT_ACKNOWLEDGED								117          
#define ERR_NVRAM_INVALID_CHECKSUM								118          
#define ERR_NVRAM_INVALID_SIZE_SET								119          
#define ERR_NVRAM_INVALID_PMT_VALUE								120          
#define ERR_NVRAM_INVALID_ZMOTOR_OFFSET_VALUE				121             
#define ERR_NVRAM_INVALID_TMOTOR_OFFSET_VALUE				122             
#define ERR_IMAGE_OPEN_LOG_LUT_FILE                      123
#define ERR_IMAGE_READ_LOG_LUT_FILE                      124
#define ERR_IMAGE_INTERPOLATE_MISSING_EDGES              125      
#define ERR_MAIN_DEVICE_RESPONSE_TASK_FULL					126
#define ERR_MAIN_CANCEL_SCAN_NOT_IN_EXAM_START				127
#define ERR_IMAGE_MU_OUT_OF_RANGE								128
#define ERR_IMAGE_FIND_PROFILE_SKIN_EDGES						129
#define ERR_IMAGE_MEAN_OUT_OF_RANGE								130
#define ERR_IMAGE_SNR_OUT_OF_RANGE								131
#define ERR_UI_DEVICE_BUSY_TIMEOUT								132
#define ERR_UI_IMAGE_BUSY_TIMEOUT								133
#define ERR_ACQ_FAIL_ERASE_TEST									134
#define ERR_CODE_BAD_VALUE                					135
#define ERR_CODE_BAD_PARITY               					136
#define ERR_CODE_BAD_ENCRYPT              					137
#define ERR_CODE_BAD_STRING_DIGIT         					138
#define ERR_CODE_BAD_STRING_SEPERATOR     					139
#define ERR_CODE_TABLE_NOT_LOADED         					140
#define ERR_CODE_TABLE_BAD_CHECKSUM       					141
#define ERR_CODE_TABLE_INVALID_FORMAT     					142
#define ERR_CODE_TABLE_INITIALIZED        					143
#define ERR_CODE_TABLE_NOT_INITIALIZED    					144
#define ERR_CODE_TABLE_OPEN               					145
#define ERR_CODE_TABLE_WRITE              					146
#define ERR_CODE_TABLE_BMD_SERIAL_NUMBER  					147
#define ERR_CODE_TABLE_SERIAL_MASK        					148
#define ERR_CODE_BAD_SEQUENCE_NUMBER      					149
#define ERR_CODE_BAD_SERIAL_NUMBER        					150
#define ERR_CODE_BAD_SERIAL_MASK          					151
#define ERR_CODE_MONTHLY_INVALID_OFFSET   					152
#define ERR_CODE_MONTHLY_NO_CODE          					153
#define ERR_CODE_NO_SCANS                 					154
#define ERR_COPY_PRO_SERIAL_NUMBER        					155
#define ERR_COPY_PRO_SERIAL_MASK          					156
#define ERR_COPY_PRO_INSTALL_TIME         					157
#define ERR_COPY_PRO_NOT_INITIALIZED      					158
#define ERR_COPY_PRO_INVALID_FORMAT       					159
#define ERR_COPY_PRO_OPEN                 					160
#define ERR_COPY_PRO_WRITE                					161
#define ERR_CODE_MONTHLY_NOT_STARTED      					162   
#define ERR_COPY_PRO_MFG_TIME             					163 
#define ERR_NVRAM_SERIAL_NUM										164
#define ERR_CODE_NO_MFG_SCANS                            165
#define ERR_CODE_ALREADY_UNLIMITED_SCANS                 166
#define ERR_CODE_NEED_INSTALL_CODE                       167
#define ERR_MAIN_CORRUPTED_BMDINI_FILE                   168
#define ERR_IO_OVERTEMPERATURE									169
// ERR_UNKNOWN_ERROR must be the last valid error.
// ERR_MAX == ERR_UNKNOWN_ERROR
#define ERR_UNKNOWN_ERROR											170
#define ERR_MAX														170
#endif	//_STATEERR_H_      
																