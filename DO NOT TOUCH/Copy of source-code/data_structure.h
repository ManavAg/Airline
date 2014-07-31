//struct for storing start and end timings of any slot on the airport (landing and takeoff both)
struct timing
{
    float start,end;
    struct timing *next_time;
};    
//struct storing details related to airport
struct Airports
{
    char name[MAX_SIZE];
    int ap_index;
    float delay;
    int takeoff_slots,landing_slots;
    //float ap_cost_ratio;
    //float initial_demand_concentration;
    //float current_demand_concentration;
    struct timing *takeoff_time;
    struct timing *landing_time;
    struct Airports *next_airport;
};    
//storing data on each route between each OD pairs
struct Data
{
    float demand,block_time;//NEU_DOC NEU_PLR -> TOC,capacity removed
	float distance;//NEU_PLR
    float priority;
	float BELF;
    //float initial_demand_concentration;
    //float current_demand_concentration;
    int initial_freq_req;
    int freq_offered;
    struct Data *next_data;
};    
//storing info related to aircraft
struct Fleet
{
    int ac_index,ap_index;
	int ac_type;//NEU_MAC
	char ap_name[MAX_SIZE];//on which it is available
    float takeoff_time,TAT;
    float life;
    float end_time;
    float demand_carried,residual_capacity,operating_cost;//when going from here to possible destination
	float BH_max, BH_remain;//NEU_BH
	struct Fleet *next_aircraft;
};    
//storing info related to aircraft types
struct AC_data //NEU_MAC
{
    int ac_type;
	char ac_code[4];//for 3 digit code
	struct static_cost *static_cost;
	float fR1,fR2,fR3,fPL1,fPL2;//Payload Range Diagram Points
	struct AC_data *next_type;
};   

struct static_cost //NEU_DOC
{
    float cost;
	struct static_cost *next_cost;
};   
//storing info of base station in the network
struct Bases
{
    int base_index;
    char base_name[MAX_SIZE];
    int hanger_capacity;
    struct Bases *next_base;
};    
//storing the priorities of grey indices
struct Grey
{
    float priority;
    struct Grey *next_grey;
};    
//stroing the grey indices detail betwen each OD pair
struct Grey_indices
{
    float route_priority_index;//,network_design_index;
    float time_index,cost_index,demand_index;
    float grey_index;
    struct Grey_indices *next_ap_grey;
};

//storing the whitening decision values for each grey index
struct Whitening_decision
{
    float demand,cost,time,route_priority;//,network_design;
};    
//storing the final grey index and category between each OD pair
struct Final_grey
{
    float grey_index;
    char grey_char;
    struct Final_grey *next_ap_index;
};
//stroing the history of all the previous possible destinations
struct choice_history
{
    int index;
    struct choice_history *next;
};
typedef struct choice_history choices; 
// storing the detail of final route
struct route_detail
{
    float takeoff,landing,cargo_carried,residual_capacity,operating_cost,original_cargo;
    int sr_index,desti_index,line_number,ac_number,flight_no;
	int ac_type;//NEU_MAC
	char ac_code[4];//for 3 digit code NEU_MAC
    //float extra_cargo;
    struct extra *extra_detail;
    struct route_detail *next;
    struct route_detail *prev;
};   
//struct for storing info that on this flight no, from this airport, our aircraft is carrying how much extra cargo ,for which destination
struct extra
{
    int desti_index;
    float extra_cargo;
    struct extra *next_extra;
};    
//struct for storing detail that our aircraft is from which source how much empty
 struct empty_detail
    {
        int source,flight;
        float residual_capacity;
        struct empty_detail *next_empty_detail;
        struct empty_detail *prev_empty_detail;
    };
    struct tats //NEU_DOC
{
    float tat;
	struct tats *next_tat;
};  
struct temporary_demand
{
    int source,desti,flight,distance;
    int parent_airport,parent_flight;
    float residual_capacity;
    struct temporary_demand *next_temp_demand;
    struct temporary_demand *line_temp_demand;
};  

//struct to store the aea input data from input file
struct aea_input //NEU_DOC
{
	double MSSP,BFE,MTOW,W_fuel,W_af;
	double engine_cost,No_Engines,BPR,OPR,N_c,T_sls,N_shaft,K;
	double Price_fuel,F_resval,F_int,R_ins;
	double Life,C_coc,C_cab,N_coc,N_cab,R_lab;
	struct aea_input *next_ac;
};

      
