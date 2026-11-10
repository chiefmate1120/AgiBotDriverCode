#define MOTOR_TYPE_JMF038		(0)
#define MOTOR_TYPE_JM2516		(1)
#define MOTOR_TYPE_JMF025		(2)

#define DEFAULT_MOTOR_TYPE	(MOTOR_TYPE_JMF025)

#if DEFAULT_MOTOR_TYPE == MOTOR_TYPE_JMF038

//Current Params
#define CURRENT_KP													(15000)
#define CURRENT_KI													(2000)
#define CURRENT_MAXOUTPUT										(30000)
#define CURRENT_INTERGRAL_LIMIT_MAX					(30000)

//Speed Params
#define SPEED_KP													(20000)
#define SPEED_KI													(300)
#define SPEED_KD													(150)
#define SPEED_MAXOUTPUT										(30000)
#define SPEED_INTERGRAL_LIMIT_MAX					(30000)

//Position Params
#define POSITION_KP										(500)
#define POSITION_KI										(5)
#define POSITION_KD										(500)

#define POSITION_MAXOUTPUT						(3000)
#define POSITION_INTERGRAL_LIMIT_MAX	(80)

//Motor Params
#define DIR_OPPOSITE													(0)
#define DIR_POSITIVE													(1)

#define MOTOR_POLE_PAIR_NUM                   (10)

#define CURRENT_DIR														(DIR_POSITIVE)
#define SPEED_DIR															(DIR_POSITIVE)
#define POSITION_DIR													(DIR_POSITIVE)


#elif DEFAULT_MOTOR_TYPE == MOTOR_TYPE_JM2516


//Current Params
#define CURRENT_KP													(5000)
#define CURRENT_KI													(2000)
#define CURRENT_MAXOUTPUT										(30000)
#define CURRENT_INTERGRAL_LIMIT_MAX					(30000)

//Speed Params
#define SPEED_KP													(20000)
#define SPEED_KI													(300)
#define SPEED_KD													(150)
#define SPEED_MAXOUTPUT										(30000)
#define SPEED_INTERGRAL_LIMIT_MAX					(30000)

//Position Params
#define POSITION_KP										(2000)
#define POSITION_KI										(10)
#define POSITION_KD										(1000)

#define POSITION_MAXOUTPUT						(3000)
#define POSITION_INTERGRAL_LIMIT_MAX	(300)


//Motor Params
#define DIR_OPPOSITE													(0)
#define DIR_POSITIVE													(1)

#define MOTOR_POLE_PAIR_NUM                   (7)

#define CURRENT_DIR														(DIR_OPPOSITE)
#define SPEED_DIR															(DIR_OPPOSITE)
#define POSITION_DIR													(DIR_OPPOSITE)


#elif DEFAULT_MOTOR_TYPE == MOTOR_TYPE_JMF025


//Current Params
#define CURRENT_KP													(4000)
#define CURRENT_KI													(1000)
#define CURRENT_MAXOUTPUT										(30000)
#define CURRENT_INTERGRAL_LIMIT_MAX					(10000)

//Speed Params
#define SPEED_KP													(20000)
#define SPEED_KI													(300)
#define SPEED_KD													(150)
#define SPEED_MAXOUTPUT										(30000)
#define SPEED_INTERGRAL_LIMIT_MAX					(30000)

//Position Params
#define POSITION_KP										(2000)
#define POSITION_KI										(15)
#define POSITION_KD										(1500)

#define POSITION_MAXOUTPUT						(2800)
#define POSITION_INTERGRAL_LIMIT_MAX	(100)

//Motor Params
#define DIR_OPPOSITE													(0)
#define DIR_POSITIVE													(1)

#define MOTOR_POLE_PAIR_NUM                   (7)

#define CURRENT_DIR														(DIR_OPPOSITE)
#define SPEED_DIR															(DIR_OPPOSITE)
#define POSITION_DIR													(DIR_OPPOSITE)

#elif


//Current Params
#define CURRENT_KP													(0)
#define CURRENT_KI													(0)
#define CURRENT_MAXOUTPUT										(0)
#define CURRENT_INTERGRAL_LIMIT_MAX					(0)

#define POSITION_KP										(5000)
#define POSITION_KI										(15)
#define POSITION_KD										(10000)
#endif
