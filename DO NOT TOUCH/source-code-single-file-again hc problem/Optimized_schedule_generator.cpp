/* To understanf GADO code , use the GADO code only
 * This program is highly modified to fit as per our requirments -Ajeet Singh
 */

/* The author of this software is Khaled Rasheed.
 * Copyright (c) 1998,1999,2000,2001,2002 by Khaled Rasheed. All rights
 * reserved. Distribution in any possible way is strictly prohibited
 * without explicit permission from the author.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, THE AUTHOR DOES NOT MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
/*******************************************************/
//files added by ajeet
/********************************************************/
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<conio.h>
typedef struct choice_history choices; 


#define YES 1
#define NO 0
#define MAX_SIZE 20
#define NOT_FOUND 0
#define FOUND 1
//struct for storing start and end timings of any slot on the airport (landing and takeoff both)
struct timing
{
    float start,end;
    struct timing *next_time;
};    
//struct storing details related to airport

//CHMANAV 15 NEW DATA STRUCTURE FOR TAT
struct tats //NEU_DOC
{
    float tat;
	struct tats *next_tat;
};   
struct Airports
{
    char name[MAX_SIZE];
    int ap_index;
    float delay; //CHMANAV3 UNDECLARED BELF AS AIRPORT DEPENEDENT AND DECLARED IT AS DATA DEPENDENT
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
    float priority,BELF;   //CHMANAV1 DECLARED BELF TO BE DATA DEPENDENT NOT AIRPORT DEPENDENT
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
    float takeoff_time;
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
//CHMANAV 30 Hanger_capacity new data structure
struct Hanger_capacity //NEU_DOC
{
    int cap;
	struct Hanger_capacity *next_hc;
};   
//storing info of base station in the network
struct Bases
{
    int base_index;
    char base_name[MAX_SIZE];
    //int hanger_capacity; CHMANAV 31 hanger_capcity in struct base commented
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
float Get_cost(int, int, int);
void Bubble_sort (float *,int);
void Cargo_scheduling(double *, int,double *,int);
float Check_airport_timing(float,int,int );
int Check_end_time(struct Fleet *,float, int,float);
void Check_landing_availability(struct Fleet *, int );
 
void Check_takeoff_availability(struct Fleet *, int,float );
 
void Demand_update(struct Fleet  *,int );
void Everything_fine(struct Fleet *,float ,float ,int );
void Free_list();
int Get_airport_data(struct Fleet *);
int Get_ap_index(char *);
void Get_base_cost_index (int,int);

int Get_base_data(struct Fleet *);
void Get_base_demand_index(int );
void Get_base_time_index (int,float *,int, float);
float Get_cost(int, int, int);
void Get_base_route_priority_index(int );
float Get_capacity(int, int, int);

void Get_cost_index(int,int);
void Get_demand_index(int,int);
void Get_fly_time(int,float,float *,int,struct Fleet *);
void Get_grey_category();
void Get_grey_index();

float Get_route_BT(int, int);
void Get_route_priority_index(int );
void Get_time_index(int ,float *,int ,float );
void Get_your_flight(struct Fleet *);

void Go_to_base(struct Fleet *);          
     
void Grey_analysis (struct Fleet *);

void Grey_base_analysis (struct Fleet *);
int Select_if_same_index(int,int,int,float,int, struct Fleet *);
 
void Initial_freq_req();
void Initialize_grey_index (void);
void Insert_sort(struct Fleet *);
void Make_end_time( ); 
 float Median_calc(float *,int);
 int New_airport_data(struct Fleet *,choices *);

int New_base_data(struct Fleet *,choices *);
void No_holding();
void Objective_calc(double *);
float Percentile_calc(float *,int,float);
void Read_input();
void Sorted_fleet();
void Store_route_info(struct Fleet *,float ,float ,int );
void Store_updated_route(struct empty_detail *,int ,float );
void Trace_demand(int, int );
void Write_output();

//#define TURN_AROUND_TIME 45
        
//GLOBAL VARIABLES
struct Grey_indices *head_select=NULL;//grey index in select category for demand,cost,time and route priority
struct Grey_indices *head_reject=NULL;//grey index in reject category for demand,cost,time and route priority
struct Grey_indices *head_probable=NULL;//grey index in probable category for demand,cost,time and route priority
struct Airports *head_airport=NULL;//airport detail like, name, takeoff and landing slots, BELF
struct Data *head_data=NULL;//network data like demand, TOC,Block time, aircraft capacity, route priority
struct Fleet *head_fleet=NULL;//fleet detail like airport, takeoff time, end time, life time
struct Hanger_capacity *head_hc=NULL;//fleet detail like airport, takeoff time, end time, life time
struct Fleet *head_aircraft_list=NULL;//sorted list of aircraft
struct AC_data *head_ac_data=NULL;//list of aircraft types NEU_MAC
struct aea_input *head_aea_input=NULL;//list for aea data for each aircraft type NEU_DOC
struct Bases *head_base=NULL;//base station detail, name, hanger capacity
struct Grey *head_grey=NULL;//priorities of grey indices
struct tats *head_tat=NULL;
struct Final_grey *head_final_grey=NULL;//final grey index and cat. of each OD pair
struct Whitening_decision *BIG=NULL,*SMALL=NULL;//used in grey analysis for taking decision of selection or rejection
//struct choice_history *head_choice=NULL;//used to keep the record of all previous choices in finding possible destination
struct route_detail *head_route=NULL,*last_route=NULL;//final network schedule detail
#define total_grey_indices 4 
//int total_grey_indices=5; 
int user_choice,fleet_size;//total_grey_indices=5; 
//NEU statt int nun const int -> dafür nicht einlesen aus inputfile!! (read_input())
//NEU_VLA const int AIRPORT_NO=6;
//NEU_VLA const int total_bases=4;
int total_bases;//NEU_VLA wieder zurück
int AIRPORT_NO;//NEU_VLA wieder zurück
int ACTYPE_NO=0; //NEU_MAC
char cCostWithAEA;
//NEU
float max_time_slot;  
int STATUS;//status become YES if all the constraints meet, then we go to next aircraft in queue else we leep looking for other options for this aircraft
int SENDING_TO_BASE;//indicate whether the aircraft is on normal schedule or we are sending it to base station
//int ASK_FLAG;//Used for deleting the record of previously checked airports, when we looks for Outgoing demand > capacity
//CHMANAV 14 Turn AOUND TIME COMMENTED
//float TURN_AROUND_TIME;
int GREY_ANALYSIS;
float initial_total_demand;
int objective_pointer;
//to return objective values - demand, cost,time
//double obj_value[3];

char input_filename[2*MAX_SIZE+10];// 4 more for extension ".txt" and 1 for end \0 character
char output_filename[7*MAX_SIZE+10];//since we are adding seed number,folder name also in output file name so giving it a big name space
char filename[7*MAX_SIZE+10];//since we are adding folder name also in it
char all_cases_filename[7*MAX_SIZE+10];//since we are adding seed number,folder name and "_all_trials" also in output file name so giving it a big name space
char best_cases_filename[7*MAX_SIZE+10];
char foldername[2*MAX_SIZE+10];
char compare_filename[7*MAX_SIZE+10];
char temp_filename[7*MAX_SIZE+10];
void Bubble_sort (float *,int);

float Get_cost(int, int, int);









float Get_cost(int actype, int source, int destination)

{
	//input values: A/C-Type, Source, Destination
	//to be calculated: Blockhours, Possible Payload
	float fCost=0, fBH=0;
	int i,j;
	struct AC_data *temp_acdata=NULL;
	struct static_cost *temp_cost;

	if(source==destination)
	return 0;

	//calculate BH if cCostWithAEA is "y"
	if(cCostWithAEA==121)
	{
		fBH = Get_route_BT(source,destination);

		//employ AE method START
		
		//Input Data at interface for DOC calc
		double L_stage=2000;	//stage distance in km
		double W_payload=15;	//payload in metric tons
		double T_flight=16000;	//time of flight (oder vielleicht eher weglassen und direkt blockhours nehmen
		//Input Data:
		double F_resval=0.1;   //Aircraft Residual value as a fraction of original ADP=0,1
		double Life=14;		//useful operational life of the aircraft	
		double F_int=0.053;		//factor on account of rate of interest = 0,053
		double R_ins=0.5;		//interest rate =0,5%
		double MSSP=100;		//manufacturers standard study price in million $
		double BFE=6;		//Buyer Furnished Equipment in million $
		double MTOW=100;		//max takeoff weight in metric tons
		double W_fuel=20;		//block fuel weight in metric tons
		double Price_fuel=0.21;	//price of fuel in $/kg
		double C_coc=246.5;		//cost of cockpit crew member $/h
		double C_cab=81;		//cost of cabin crew member $/h
		double N_coc=2;		//no. of cockpit crew members
		double N_cab=0;		//no. of cabin crew members
		double W_af=60;		//Airframe weight in metric tons i.e Manufcaturers Empty Wight +2% less weight of bare engines
		double R_lab=63;		//labor rate incl. burden = $63/h (1989 price)
		double engine_cost=10; //total cost of engines in million $
		double No_Engines=4;	//number of engines
		double BPR=5;		//bypass pressure ratio
		double OPR=20;		//overall enginepressure ratio
		double N_c=2;		//Number of compr. stages incl fan stages
		double T_sls=80	;	//Sea level static TO thrust in metric tons
		double N_shaft=2;	//no. of shafts
		double K;

		//--E_tdel->Eng_labour->C_maint->C_opr-----
		if(N_shaft==1)
		K=0.5;
		else if(N_shaft==2)
		K=0.57;
		else if (N_shaft==3)
		K=0.64;
		else
		exit(1);
		//--total_invest->C_depr->C_stand--
		double ADP=MSSP+BFE+0.06*(MSSP+BFE)+0.025*MSSP;
		double Airfr_spares=0.1*(ADP-engine_cost);
		double Eng_spares=0.3*engine_cost;
		//--U_ann->C_depr->C_stand---------
		double T_blk=T_flight/3600+0.25;
		//--Eng_labour->C_maint->C_opr-----
		double E_tdel=0.21*R_lab*(1.27-0.2*pow(BPR,0.2))*(0.032*N_c+K)*pow((1+T_sls),0.4);
		double C_del=1.3*E_tdel;
		//--Eng_material->C_maint->C_opr---
		double E_tdem=2.56*pow((1+T_sls),0.8)*(1.27-0.2*pow(BPR,0.2))*((0.4*pow((OPR/20),1.3)+0.4)+(0.032*N_c+K));
		double C_dem=1.3*E_tdem;
		//--C_depr->C_stand----
		double total_invest=ADP+Airfr_spares+Eng_spares;
		double U_ann=T_blk*3750/(T_blk+0.5);
		//--C_crew->C_opr------
		double Coc_crew_cost=C_coc*N_coc;
		double Cab_crew_cost=C_cab*N_cab;
		//--C_maint->C_opr-----
		double Airfr_labour=(0.09*W_af+6.7-350/(W_af+75))*((0.8+0.68*(T_blk-0.25))/T_blk)*R_lab;
		double Airfr_material=((4.2+2.2*(T_blk-0.25))/T_blk)*(ADP-engine_cost);
		double Eng_labour=E_tdel+C_del;
		double Eng_material=E_tdem+C_dem;
		double E_doc=(No_Engines*(E_tdel+E_tdem)*(T_flight-0.25+1.3)/T_flight);
		//--C_stand------------
		double C_depr=total_invest*1000000*(1-F_resval)/(U_ann*Life);
		double C_intr=total_invest*1000000*F_int/U_ann;
		double C_ins=R_ins*ADP*1000000/U_ann;
		//--C_apt--------------
		double C_nav=0.5*L_stage*pow((MTOW/50),0.5)/T_blk;//stimmt
		double C_hand=100*W_payload/T_blk;
		double C_land=7.8*MTOW/T_blk;
		//--C_opr--------------
		double C_fuel=W_fuel*1000*Price_fuel/T_blk;//pricefuel is per kilo?!
		double C_crew=Coc_crew_cost+Cab_crew_cost;//stimmt
		double C_maint=Airfr_labour+Airfr_material+E_doc;
		//--DOC_total----------
		double C_stand=C_depr+C_intr+C_ins;//stimmt
		double C_apt=C_nav+C_hand+C_land;//stimmt
		double C_opr=C_fuel+C_crew+C_maint;		
		//--FINAL--------------
		double DOC_total=C_stand+C_opr+C_apt;
		printf("\n\n%.5f",DOC_total);
		//employ AE method START

		fCost=DOC_total*40*fBH/600000;//DOC*(dollar to rupies)*BH/(10000*60min)
		return fCost; //Lakh Rupies for the flight

	}
	else if(cCostWithAEA==110) //means: take cost value from input file (ac-specific)
	{
		temp_acdata=head_ac_data;
		//go to the actype cost data
		while(temp_acdata->ac_type!=actype)
			temp_acdata=temp_acdata->next_type;
		temp_cost=temp_acdata->static_cost;
		//going up to right place
		for(i=1;i<source;i++)
		for(j=0;j<AIRPORT_NO;j++)
			temp_cost=temp_cost->next_cost;
		for(i=1;i<destination;i++)
			temp_cost=temp_cost->next_cost;
		//printf("\n\nACtype: %d Airport: %d Nach: %d Cost: %f",actype,source,destination,temp_cost->cost);
		return temp_cost->cost;
	}
	else
	{
		printf("Error in cCostWithAEA");
		getch();
		exit(1);
	}
}    //end of function
void Bubble_sort( float *array, int size)
{
    int i,j;
    float t;
    //printf("\n\n In -");
    //for(i=0;i<size;i++)
   // printf("\n %.2f",array[i]);
    for(i=1;i<=size-1;i++)
    {
        for(j=1;j<=size-i;j++)
        {
            if(array[j-1]<=array[j])
            {
                //printf("\n cahnging %d and %d",j-1,j);
                t=array[j-1];
                array[j-1]=array[j];
                array[j]=t;
            }    
            //else
            //continue;
        }    
    }  
   /// printf("\n\n out -");
   // for(i=0;i<size;i++)
   // printf("\n %.2f",array[i]);  
}    
//#include"include_files.h"

void Cargo_scheduling(double *, int,double *,int);
void Cargo_scheduling(double *x, int size,double *obj_value,int print_choice)
{  
    time_t end_time;
    struct tm * timeinfo;
    struct Fleet *candidate;
	
	//=====================================/
    //assigning x to grey priority 
    int i;
    //total_grey_indices=5; //declared in include_files
    
    //FILE *fp=fopen(output_filename,"w");
    //fclose(fp);
    
    
 
    
    struct Grey *grey,*last_grey;
    for(i=0;i<total_grey_indices;i++)
    {
        grey=(struct Grey *)malloc(sizeof(struct Grey));
        grey->priority=x[i];
        grey->next_grey=NULL;
        if(head_grey==NULL)
        {
            head_grey=grey;
            last_grey=grey;
        }
        else
        {
            last_grey->next_grey=grey;
            last_grey=last_grey->next_grey;
        }
    } 
    //=====================//
    /*
    time(&start_time);
    timeinfo = localtime ( &start_time );
    printf("\n Starts at : %s ",asctime(timeinfo));
    printf("\n -------Airline Network Optimization for cargo Airlines--------BETA version");
    printf("\n >>>>****for testing and evalution only **** not for commercial use****<<<<");
    printf("\n ---------------------------------------------------------------------------");
    printf("\n\n Are you interested in -");
    printf("\n 1. Complete detail of scheduling process");
    printf("\n 2. Only final schedule");
    printf("\n Enter your choice - ");
    do
    {
        scanf("%d",&user_choice);
        if (user_choice==2)
        user_choice=NO;
        else if(user_choice!=YES && user_choice!=NO)
        printf("\n Please enter a valid choice (1 or 2)- ");
    }while(user_choice!=YES && user_choice !=NO);    
    */ 
    user_choice=NO;
      
    Read_input();
    
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","w");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't make the file Output/PROCESS in main");
            getch();
            exit(1);
            }
        fprintf(fprocess," -------Airline Network Optimization for cargo Airlines--------BETA version");
        fprintf(fprocess,"\n >>>>****for testing and evalution only **** not for commercial use****<<<<\n");
        fprintf(fprocess,"\n ===========================================================================");
        fprintf(fprocess,"\n This program is designed and developed at ");
        fprintf(fprocess,"\n Aerospace Engineering Department, IIT Bombay \n");    
        fprintf(fprocess,"\n For any query/suggestion, contact - ");
        fprintf(fprocess,"\n Prof. Rajkumar S. Pant , LTA systems lab, rkpant@aero.iitb.ac.in");
        fprintf(fprocess,"\n Ajeet Singh, MTech Student(2005-2007), ajeet@aero.iitb.ac.in");
		fprintf(fprocess,"\n Florian Maul, Technical University of Munich, florian.maul@mytum.de");
        fprintf(fprocess,"\n ===========================================================================\n");
        //fprintf(fprocess,"\n The Schedule is generated on :\n %s ",asctime(timeinfo));
        fprintf(fprocess,"\n ===========================================================================\n");
        
        
        fclose(fprocess);        
    } //end of if  (user_choice==YES)  
    
    // calcualting initial freq required between each OD pair
    Initial_freq_req();
    //initializing the grey indices to zero
    Initialize_grey_index();
    //Calculating the end time of each aircraft for todays operations (taking care of maximum time slot)
    Make_end_time();
    //Make a shorted list of aircrafts as per takeoff time
    Sorted_fleet();
    
    //starting the scheduling process
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't make the file Output/PROCESS in main");
            getch();
            exit(1);
            }
        fprintf(fprocess,"\n\n ***************************************\n");
        fprintf(fprocess,"\n Starting the scheduling process -------\n");
        fprintf(fprocess,"\n ***************************************\n");
        fclose(fprocess);
    }    
    
    candidate=head_aircraft_list;
    while(candidate != NULL)
            {
                               
                STATUS=NO;
                SENDING_TO_BASE=NO;
                if(user_choice==YES)
                {
                    FILE *fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't make the file Output/PROCESS in main");
                        getch();
                        exit(1);
                        }
                    fprintf(fprocess,"\n ------------------------------");
                    fprintf(fprocess,"\n\n\n Scheduling the aircraft %d\n\n\n",candidate->ac_index);
                    fprintf(fprocess," ------------------------------\n");
                    if(candidate->takeoff_time>=max_time_slot)
                    {
                        fprintf(fprocess,"\n ------------------------------\n");
                        fprintf(fprocess,"\n OPERATING TIME OVER FOR THE DAY\n");
                        fprintf(fprocess,"\n ------------------------------");
                        fclose(fprocess);
                        break;
                    }    
                    fclose(fprocess);
                }
                /*
                printf("\n ------------------------------");
                printf("\n Scheduling the aircraft %d",candidate->ac_index);
                printf("\n ------------------------------");  
                */
                //doing grey analysis of all the airports
                GREY_ANALYSIS=YES;
                Grey_analysis(candidate);
                GREY_ANALYSIS=NO;
                
                Get_your_flight(candidate);
                                              
                if(STATUS==NO)
                {
                    SENDING_TO_BASE=YES;
                    //again grey anlysis because when going to base, landing time check will be little change
                    GREY_ANALYSIS=YES;
                    Grey_base_analysis(candidate);
                    GREY_ANALYSIS=NO;
                    Go_to_base(candidate);
                }  
                
                candidate=candidate->next_aircraft;
               
            }    //end of while (candidate != NULL)
    
	//NEU delete holding
	No_holding();
	//NEU	

    //calculating the objective values       Now with Real Utilization
    Objective_calc(obj_value);
    
    //Adjust_remaining_demand();
    if(print_choice==YES)
    {
        Write_output();
        /*
        time(&end_time);
        timeinfo = localtime ( &end_time );
        printf("\n\n Ends at : %s ",asctime(timeinfo));
        //diff_time = difftime(end_time,start_time);
        //printf("\n Time taken = %.2lf Seconds or %.2lf Minutes",diff_time,diff_time/60.0);
        printf("\n--------------------------------------------------------");   
        printf("\n\t\t !!! Completed !!!\n\n");
        if(user_choice==YES)
        {
                FILE *fprocess=fopen("Output/PROCESS.txt","a");
                if(fprocess==NULL)
                   {
                    
                    printf("\n Cann't make the file Output/PROCESS in main");
                    getch();
                    exit(1);
                    }
                fprintf(fprocess,"\n\n\n--------------------------------------------------------"); 
                fprintf(fprocess,"\n \n Ends at : %s ",asctime(timeinfo));
                fprintf(fprocess,"\n--------------------------------------------------------");   
                fprintf(fprocess,"\n\t\t !!! Completed !!!\n\n");
                fclose(fprocess);
            }//end of if (user_choice==YES)   
    
        printf("\n ===========================================================================");
        printf("\n This program is designed and developed at ");
        printf("\n Aerospace Engineering Department, IIT Bombay \n");    
        printf("\n For any suggestion/query, contact - ");
        printf("\n Prof. Rajkumar S. Pant , LTA systems lab, rkpant@aero.iitb.ac.in");
        printf("\n Ajeet Singh, MTech Student(2005-2007), ajeet@aero.iitb.ac.in");
        printf("\n ===========================================================================\n");
        printf("\n Press any key to exit---\n");
        
        getch();
        */
    }//end of if (print_choice==YES)    
        
    //GLOBAL VARIABLES
	//NEU4
	Free_list();
	//NEU4

	head_select=NULL;//grey index in select category for demand,cost,time and route priority
	head_reject=NULL;//grey index in reject category for demand,cost,time and route priority
	head_probable=NULL;//grey index in probable category for demand,cost,time and route priority
	head_airport=NULL;//airport detail like, name, takeoff and landing slots, BELF
	head_data=NULL;//network data like demand, TOC,Block time, aircraft capacity, route priority
	head_fleet=NULL;//fleet detail like airport, takeoff time, end time, life time
	head_ac_data=NULL;//NEU_MAC
	head_aea_input=NULL;//NEU_DOC
	head_aircraft_list=NULL;//sorted list of aircraft
	head_base=NULL;//base station detail, name, hanger capacity
	head_grey=NULL;//priorities of grey indices
	head_final_grey=NULL;//final grey index and cat. of each OD pair
	BIG=NULL;
	SMALL=NULL;//used in grey analysis for taking decision of selection or rejection
	//struct choice_history *head_choice=NULL;//used to keep the record of all previous choices in finding possible destination
	head_route=NULL;
	last_route=NULL;//final network schedule detail
   
   // getch(); 
   // return (obj_value);
}    
float Check_airport_timing(float,int,int );

float Check_airport_timing(float new_time, int ap_index,int landing)
{
    
    int slots,i;
    float time_diff=-1,start,end;
    struct Airports *airport;
    struct timing *slot_time;
    FILE *fprocess;
    if(user_choice==YES && GREY_ANALYSIS==NO)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't find the file Output/PROCESS in check_airport_timing");
            getch();
            exit(1);
            }
    }    
    
    airport=head_airport;
    for(i=1;i<ap_index;i++)
    airport=airport->next_airport;
    if(landing==YES)
    {
        slots=airport->landing_slots;
        slot_time=airport->landing_time;
        if(user_choice==YES && GREY_ANALYSIS==NO)
        {
            fprintf(fprocess,"\n checking landing timings on destination airport");
        }    
    
    }
    else
    {
        slots=airport->takeoff_slots;
        slot_time=airport->takeoff_time;
        if(user_choice==YES && GREY_ANALYSIS==NO)
        {
            fprintf(fprocess,"\n checking takeoff timings on destination airport");
        }
    }    
    if(user_choice==YES && GREY_ANALYSIS==NO)
        {
            fprintf(fprocess,"\n slots\t Timings");
            fprintf(fprocess,"\n %d\t",slots);
        }
    for(i=0;i<slots;i++)
    {
        start=slot_time->start;
        end=slot_time->end;
        if(user_choice==YES && GREY_ANALYSIS==NO)
        {
            fprintf(fprocess," %.2f-%.2f ",start,end);
        }
        if(new_time>=start && new_time<=end)
        {
            time_diff=0;

            break;
        }
        else if (new_time<start)
        {
            time_diff=start-new_time;
        
            break;
        } 
            
        slot_time=slot_time->next_time;   
    } 
 
    if(user_choice==YES && GREY_ANALYSIS==NO)
    {
        fprintf(fprocess,"\n time difference = %.2f",time_diff);
        fclose(fprocess);
    }    
    return(time_diff);        
    
}    

//#include"everything_fine.h"
int Check_end_time(struct Fleet *,float, int,float);

int Check_end_time(struct Fleet *candidate, float new_take_off, int desti_index, float time_diff)
{
    int i,j;
    int base_aircraft;
    char ch='y';
    float max_delay=0,delay,block_time,landing_time,base_time_diff;
	float original_route_blocktime;
    struct Bases *base;
    struct Airports *airport;
    struct Fleet *temp_head;
    time_t current_time;
    struct tm * timeinfo;    
    FILE *fprocess;
    struct Hanger_capacity *hc;
	int manav,manas;
    //generate blocktime for the original route again NEU_BH
	original_route_blocktime=Get_route_BT(candidate->ap_index,desti_index);
//CHMANAV 18 defining tat for usage for particular candidate aircraft
	struct tats *tat;
	  tat=head_tat;
	  for(i=1;i<candidate->ac_index;i++)
		  tat=tat->next_tat;

    //check for maintenance center reachable from destination or not
    base=head_base;
    for(i=0;i<total_bases;i++)
    {
         if(user_choice==YES)
            {
                fprocess=fopen("Output/PROCESS.txt","a");
                if(fprocess==NULL)
                {
                    printf("\n Cann't open the file Output/PROCESS in check_end_time");
                    getch();
                    exit(1);
                }    
            } 
        //find out how many aircraft are present their already
        base_aircraft=0;
        temp_head=candidate;
            while(temp_head!=NULL)//look all the fleets
            {
                if(temp_head->ap_index==base->base_index && temp_head->ac_index!=candidate->ac_index)//if aircraft is on our base station
                {
                    if(user_choice==YES)
                        fprintf(fprocess,"\n Aircraft no. %d is available at base airport %d",temp_head->ac_index,base->base_index);
                    base_aircraft++;
                }    
                temp_head=temp_head->next_aircraft;
            }
         if(user_choice==YES)
         {
             fprintf(fprocess,"\n Total %d aircraft are available on base airport %d",base_aircraft,base->base_index);
             fclose(fprocess);
         }    
        
        //if base station can handle more aircraft then only its my possible base station
        //I cann't assume that aircraft which are already their will leave the base staion for me
      //CHMANAV 33 new snippet for getting base and aircraft dependent hanger capacity
		 hc=head_hc;
		 for (manav=0;manav<i;manav++)
		  for (manas=1;manas<candidate->ac_type;manas++)
			  hc=hc->next_hc;
			
        if(hc->cap - base_aircraft >0)
        {
            //time req to reach upto maintenance center from destination, if we go there
            block_time=Get_route_BT(desti_index,base->base_index);//(source,destination)
            //reading the max delay in landing at the maintenance airport
            if(block_time!=0)
            {
                airport=head_airport;
                for(j=0;j<base->base_index;j++)
                {
                    max_delay=airport->delay;
                    airport=airport->next_airport;
                }    
                //calculating the random delay in landing                    
                if(max_delay==0)
                delay=0;
                else
                {
                    current_time=time(NULL);
                    timeinfo=localtime(&current_time);
                    srand(time(NULL)*rand());//initializing random function
                    //delay= (max_delay * (timeinfo->tm_sec /60.0)* ( (rand()% (int)max_delay)/max_delay));
                    delay= (max_delay * (((timeinfo->tm_sec /60.0)+( (rand()% (int)max_delay)/max_delay))/2));
                }    
                    //printf("\n %f",delay);
            }    
            else
				//CHMANAV 19 tat used instead of TURN_AROUND_TIME
            delay= -(tat->tat+time_diff);//blocktime =0 means we are on the base itself, so no need of TURN AROUND TIME and time diff in takeoff
            
            landing_time=new_take_off+block_time+delay;
            if(user_choice==YES)
            {
                fprocess=fopen("Output/PROCESS.txt","a");
                if(fprocess==NULL)
                   {
                    
                    printf("\n Cann't open the file Output/PROCESS in check_end_time");
                    getch();
                    exit(1);
                    }
                fprintf(fprocess,"\n Block time required to go to base %d from destination = %.2f",base->base_index,block_time);
                fprintf(fprocess,"\n Maximum possible delay in landing on the base station= %.2f",max_delay);
                fprintf(fprocess,"\n Delay in landing on the base station = %.2f",delay);
                fprintf(fprocess,"\n landing on the base station = %.2f",landing_time);
                fclose(fprocess);
            }    
            
            base_time_diff=Check_airport_timing(landing_time,base->base_index,YES);//YES means check for landing, NO means check for takeoff
            if(user_choice==YES)
            {
                fprocess=fopen("Output/PROCESS.txt","a");
                if(fprocess==NULL)
                   {
                    
                    printf("\n Cann't open the file Output/PROCESS in check_end_time");
                    getch();
                    exit(1);
                    }
            } 
            if(base_time_diff >=0)
            {
                ch='y';
                landing_time+=base_time_diff;
                if(user_choice==YES)
                fprintf(fprocess,"\n New Landing time on base (due to unavailability of base station for landing) = %.2f",landing_time);
                             
                    
            }  
            else if(base_time_diff < 0)
            {
                ch='n';
                if(user_choice==YES)
                fprintf(fprocess,"\n We cann't go to this base airport from our destination since base airport will not be available for landing today");   
            }
           
            if(ch== 'y' || ch== 'Y')
            {
                if(user_choice==YES)
                {
                    
                    fprintf(fprocess,"\n flying time required = %.2f (to land on base station)", (landing_time - candidate->takeoff_time));
                    fprintf(fprocess,"\n flying time available = %.2f",candidate->end_time);    
					fprintf(fprocess,"\n BH remaining = %.2f",candidate->BH_remain);//NEU_BH
                    fclose(fprocess);
                }    
                //NEU_BH old: if(candidate->end_time >= (landing_time - candidate->takeoff_time))
                if(candidate->end_time >= (landing_time - candidate->takeoff_time) && candidate->BH_remain >= (original_route_blocktime + block_time))//NEU_BH
                {
                    Everything_fine(candidate,new_take_off,time_diff,desti_index);
                    break;
                }//end of  if(end_time >= (landing_time - candidate->take_off))  
                
            }//end of if(ch== 'y' || ch== 'Y') 
            if(user_choice==YES)
                fclose(fprocess);    
        }    //end of  if(base->hanger_capacity - base_aircraft >0)
         
         else
            {
                if(user_choice==YES)
                {
                    fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't open the file Output/PROCESS in check_end_time");
                        getch();
                        exit(1);
                        }
                    fprintf(fprocess,"\n Not checking the Base station %d since its full upto its capacity",base->base_index);
                    fclose(fprocess);
                }    
            }   
        //going to next base station
        base=base->next_base;
        
    } //end of   for(i=0;i<total_bases;i++)
    
        if(STATUS==NO)
        {
                if(user_choice==YES)
                {
                    fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't open the file Output/PROCESS in check_end_time");
                        getch();
                        exit(1);
                        }
                    fprintf(fprocess,"\n------------ Aircraft don't have that much time to make this journey------------\n");
                    fclose(fprocess); 
                }     
            }    
// line inserted to take care of compilation error
		return 0;
}//end of function    
////#include"get_route_BT.h"
////#include"check_airport_timing.h"
//#include"check_takeoff_availability.h"
void Check_landing_availability(struct Fleet *, int );
 
void Check_landing_availability(struct Fleet *candidate,  int desti_index)
{
    int i;
    char ch='y';
    float max_delay,delay;
    float block_time;
    float landing_time,next_takeoff_time,time_diff;
    struct Airports *airport;
    time_t current_time;
    struct tm * timeinfo;
    //int random;
    FILE *fprocess;
       

    //finding the block time between these OD pair
    block_time=Get_route_BT(candidate->ap_index,desti_index);//(source,destination)
    //reading the max delay in landing at the destination airport
    airport=head_airport;
    for(i=0;i<desti_index;i++)
    {
        max_delay=airport->delay;
        airport=airport->next_airport;
    }    
    if(max_delay==0)
    delay=0;
    else
    {
        //calculating the random delay in landing
        current_time=time(NULL);
        timeinfo=localtime(&current_time);
        srand(time(NULL)*rand());//initializing the random function
        //random=rand();
        //delay= (max_delay * (timeinfo->tm_sec /60.0)* ((rand()%1000)/1000.0));
        //delay= (max_delay * (timeinfo->tm_sec /60.0)* ( (rand()% (int)max_delay)/max_delay));
        delay= (max_delay * (((timeinfo->tm_sec /60.0)+( (rand()% (int)max_delay)/max_delay))/2));
    }    
    
    if(SENDING_TO_BASE==YES)
    {
        //if we are on the base station itself then delay in landing = zero
        if(candidate->ap_index==desti_index)
        delay=0;
    }  
      
   // printf("\n max delay possible in landing = %.2f",max_delay);
    //printf("\n delay = %f",delay);
    landing_time=candidate->takeoff_time+block_time+delay;
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't open the file Output/PROCESS in check_landing_availability");
            getch();
            exit(1);
            }
         fprintf(fprocess,"\n Aircraft takeoff time = %.2f",candidate->takeoff_time);
         fprintf(fprocess,"\n Block time from %d to %d= %.2f",candidate->ap_index,desti_index,block_time);
         fprintf(fprocess,"\n Maximum possible delay in landing = %.2f",max_delay);
         fprintf(fprocess,"\n Delay in landing = %.2f",delay);
         fprintf(fprocess,"\n Landing time =  %.2f",landing_time);
         fclose(fprocess); 
    } 
    time_diff=Check_airport_timing(landing_time,desti_index,YES);//YES means check for landing, NO means check for takeoff
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't open the file Output/PROCESS in check_landing_availability");
            getch();
            exit(1);
            }
    } 
    if(time_diff >0 && (landing_time+time_diff <=max_time_slot) && (SENDING_TO_BASE==NO))
    {
        /*
        printf("\n\n\n For the Aircraft %d on flight from Airport %d to Airport %d  : ",candidate->ac_index,candidate->ap_index,desti_index);
        printf("\n There will be delay  of %.2f mins in Landing, due to airport unavailability",time_diff);
        printf("\n Do you want to go for this flight? (y/n):");
        */
                        if(user_choice==YES)
                        {
                                
                            fprintf(fprocess,"\n For the Aircraft %d on flight from Airport %d to Airport %d  : ",candidate->ac_index,candidate->ap_index,desti_index);
                            fprintf(fprocess,"\n There is a delay  of %.2f mins in Landing, due to airport unavailability",time_diff);
                            //fprintf(fprocess,"\n Do you want to go for this flight? (y/n):");
                        }    
        /*
        fflush(stdin);                
        ch=getchar();
                        if(user_choice==YES)
                        fprintf(fprocess,"\n User Choice = %c",ch);
                        
       */
        ch='y';
        //if(ch=='y'||ch=='Y')
        //{
            landing_time+=time_diff;
                            if(user_choice==YES)
                            fprintf(fprocess,"\n New Landing time = %.2f",landing_time);
                            
        //}    
    }  
    else if(time_diff >0  && (SENDING_TO_BASE==YES))
    landing_time+=time_diff;//if going to base that don't ask the user choice
    else if(time_diff < 0)//not able to land
    {
        ch='n';
        if(user_choice==YES)
        fprintf(fprocess,"\n We cann't go to this destination since airport will not be available for landing today");   
    }//end of else if(time_diff < 0)
    else if ((landing_time+time_diff > max_time_slot))   
    landing_time+=time_diff;
                              
    if(user_choice==YES)
    fclose(fprocess);
                      
    if((ch== 'y' || ch== 'Y') && (landing_time <= max_time_slot))//now check for airport availability for next takeoff
    {
       if(SENDING_TO_BASE==NO)
       Check_takeoff_availability(candidate,desti_index,landing_time);
       else//sending to base, skipping the steps of takeoff time checking
       {
           if(user_choice==YES)
                {
                    fprocess=fopen("Output/PROCESS.txt","a");
                        if(fprocess==NULL)
                        {
                            printf("\n Cann't open the file Output/PROCESS in check_landing_availability");
                            getch();
                            exit(1);
                        } 
                    fprintf(fprocess,"\n flying time required = %.2f (to land on base station)", (landing_time - candidate->takeoff_time));
                    fprintf(fprocess,"\n flying time available = %.2f",candidate->end_time);
					fprintf(fprocess,"\n BH available = %.2f",candidate->BH_remain);//NEU_BH
                    fclose(fprocess);
                } 
           //checking the end time
           //NEU_BH old: if(candidate->end_time >= (landing_time - candidate->takeoff_time))
		   if(candidate->end_time >= (landing_time - candidate->takeoff_time) && candidate->BH_remain >= block_time)//NEU_BH
           {
                    time_diff=0;
                    Everything_fine(candidate,landing_time,time_diff,desti_index);
                   // break;
            }//end of  if(end_time >= (landing_time - candidate->take_off))  
       }  //end of else  
    }//end of if (ch== 'y' || ch== 'Y'&& landing_time < max_time_slot)  
    
    
    if(ch=='n' || ch=='N')
    {
        if(user_choice==YES)
        {
            fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
            {
                printf("\n Cann't open the file Output/PROCESS in check_landing_availability");
                getch();
                exit(1);
            }    
            fprintf(fprocess,"\n------------ Airport is not available for landing------------\n");
            fclose(fprocess);
        }
    }        
    else if (landing_time > max_time_slot)    
    {
        if(user_choice==YES)
        {
            fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
            {
                printf("\n Cann't open the file Output/PROCESS in check_landing_availability");
                getch();
                exit(1);
            }    
            fprintf(fprocess,"\n------------ This landing is out of maximum operating time slot------------\n");
            fclose(fprocess);
        }
    }   
    
}//end of function     
//#include"check_end_time.h"
void Check_takeoff_availability(struct Fleet *, int,float );
 
void Check_takeoff_availability(struct Fleet *candidate,  int desti_index, float landing_time)
{
    int i;
    char ch='y';
    float next_takeoff_time,time_diff;
    FILE *fprocess; 
    
    //CHMANAV 21 defining tat for usage for particular candidate aircraft
	struct tats *tat;
	  tat=head_tat;
	  for(i=1;i<candidate->ac_index;i++)
		  tat=tat->next_tat;

        next_takeoff_time=landing_time+ tat->tat;
        if(user_choice==YES)
        {
            fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
               {
                
                printf("\n Cann't open the file Output/PROCESS in check_takeoff_availability");
                getch();
                exit(1);
                }
             fprintf(fprocess,"\n Next takeoff time = %.2f",next_takeoff_time);
             fclose(fprocess); 
        } 
        
        time_diff=Check_airport_timing(next_takeoff_time,desti_index,NO);//YES means check for landing, NO means check for takeoff
        if(user_choice==YES)
        {
            fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
               {
                
                printf("\n Cann't open the file Output/PROCESS in check_takeoff_availability");
                getch();
                exit(1);
                }
        } 
        if(time_diff >0 && (next_takeoff_time+time_diff < max_time_slot))//ask to user only if this flight is possible
        {
            /*
            printf("\n\n\n For the Aircraft %d on flight from Airport %d to Airport %d  : ",candidate->ac_index,candidate->ap_index,desti_index);
            printf("\n There will be delay  of %.2f mins in next takeoff, due to airport unavailability",time_diff);
            printf("\n Do you want to go for this flight? (y/n):");
                */
                            if(user_choice==YES)
                            {
                                    
                                fprintf(fprocess,"\n For the Aircraft %d on flight from Airport %d to Airport %d  : ",candidate->ac_index,candidate->ap_index,desti_index);
                                fprintf(fprocess,"\n There is a delay  of %.2f mins in next takeoff, due to airport unavailability",time_diff);
                               // fprintf(fprocess,"\n Do you want to go for this flight? (y/n):");
                            }    
            /*                
            fflush(stdin);
            ch=getchar();
                            if(user_choice==YES)
                            fprintf(fprocess,"\n User Choice = %c",ch);
                            
            */
            ch='y';
            if(ch=='y'||ch=='Y')
            {
                next_takeoff_time+=time_diff;
                                if(user_choice==YES)
                                fprintf(fprocess,"\n New Takeoff time = %.2f",next_takeoff_time);
                                
            }    
        }  
        else if(time_diff < 0)
        {
            ch='n';
            if(user_choice==YES)
            fprintf(fprocess,"\n We cann't go to this destination since airport will not be available for next flight today");   
        }//end of else if(time_diff < 0)
        else if (next_takeoff_time+time_diff >= max_time_slot)
                next_takeoff_time+=time_diff; //the onlychance to make this flight is that our destination is a base station and it is free
                
        if(user_choice==YES)
        fclose(fprocess);
        
        if( (ch== 'y' || ch== 'Y'))// && (next_takeoff_time < max_time_slot))
        {
            //check for maintenance stations
            Check_end_time(candidate,next_takeoff_time,desti_index,time_diff);
        }    
    
    if(ch=='n' || ch=='N')
    {
        if(user_choice==YES)
        {
            fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
            {
                printf("\n Cann't open the file Output/PROCESS in check_takeoff_availability");
                getch();
                exit(1);
            }    
            fprintf(fprocess,"\n------------ Airport is not available for takeoff------------\n");
            fclose(fprocess);
        }
    } 
    /*
    else if(next_takeoff_time >= max_time_slot)
    {
        if(user_choice==YES)
        {
            fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
            {
                printf("\n Cann't open the file Output/PROCESS in check_takeoff_availability");
                getch();
                exit(1);
            }    
            fprintf(fprocess,"\n------------ With this takeoff aircraft will not be able to land with in maximum operating time slot------------\n");
            fclose(fprocess);
        }
    }  
    */        
}//end of function    

      
//updating the demand
//updating the count of freq offered

void Demand_update(struct Fleet  *,int );
void Demand_update(struct Fleet *candidate,int desti_index)
{
    int i,j;
    struct Data *data,*temp_data;
	float temp_capacity;//NEU_MAC
    //going upto the airport where our candidate aircraft is available
    data=head_data;
    for(i=1;i<candidate->ap_index;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    //going upto the destination airport
    for(j=1;j<desti_index;j++)
    data=data->next_data;
    
    //updating the count of freq offered on that route
    data->freq_offered++;
    
    //storing the operating cost info
	//NEU_MAC old:candidate->data->TOC;
    candidate->operating_cost=Get_cost(candidate->ac_type,candidate->ap_index,desti_index);//NEU_MAC

	//setting demand data
	temp_capacity=Get_capacity(candidate->ac_type,candidate->ap_index,desti_index);//NEU_MAC
	//NEU_MAC old: if(data->demand > data->capacity) candidate->demand_carried=data->capacity;
	if(data->demand > temp_capacity)//NEU_MAC
    candidate->demand_carried=temp_capacity;//NEU_MAC
    else
    candidate->demand_carried=data->demand;
    //NEU_MAC old:candidate->residual_capacity=data->capacity-candidate->demand_carried;
    candidate->residual_capacity=temp_capacity-candidate->demand_carried;//NEU_MAC
    //updating ,remaining demand = orignal demand - carried demand
    data->demand-=candidate->demand_carried;
    
    //printing the updated demand
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in demand_update");
            getch();
            exit(1);
        } 
		fprintf(fprocess,"\n Aircraft %d of Type %d is going from airport %d to airport %d",candidate->ac_index,candidate->ac_type,candidate->ap_index,desti_index);//NEU_MAC
        fprintf(fprocess,"\n Updated Demand -\n");
        temp_data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess," %.2f\t",temp_data->demand);
                temp_data=temp_data->next_data;
            }
            fprintf(fprocess,"\n");
        }
        fclose(fprocess);
    }
               
                      
}//end of function    
//if everything is fine 
//#include"demand_update.h"
//#include"store_route_info.h"

void Everything_fine(struct Fleet *,float ,float ,int );

void Everything_fine(struct Fleet *candidate,float new_take_off,float time_diff,int desti_index)
{
    int i;
    float used_time;
    struct Bases *base;
    struct Fleet *item;
    struct Airports *airport;
    FILE *fprocess;
    struct Hanger_capacity *hc;
    //update the end time  
    used_time= new_take_off-candidate->takeoff_time; 
    candidate->end_time-=used_time;    
	candidate->BH_remain-=Get_route_BT(candidate->ap_index, desti_index);//NEU_BH

    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in everything_fine");
            getch();
            exit(1);
        } 
        fprintf(fprocess,"\n Everything Fine");
        if(SENDING_TO_BASE==NO)
        fprintf(fprocess,"\n Used time = %.2f (till to get ready for next takeoff)",used_time);
        else
        fprintf(fprocess,"\n Used time = %.2f ( to reach upto the base station)",used_time);
        fprintf(fprocess,"\n Remaining End time for the aircraft %d = %.2f",candidate->ac_index,candidate->end_time);
		fprintf(fprocess,"\n Remaining BH = %.2f",candidate->BH_remain); //NEU_BH
        fclose(fprocess);
    }                                        
    
    //if aircraft send to base station, then update the base station capacity
    if(SENDING_TO_BASE==YES)
    {
        base=head_base;
        for(i=0;i<total_bases;i++)
        {
            //CHMANAV 33 new snippet for getting base and aircraft dependent hanger capacity
		 hc=head_hc;
		 for (int manav=0;manav<i;manav++)
		  for (int manas=1;manas<candidate->ac_type;manas++)
			  hc=hc->next_hc;
			
			if(desti_index==base->base_index)
            {
                hc->cap--;
                if(user_choice==YES)
                {
                    fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                    {
                        printf("\n Cann't open the file Output/PROCESS in everything_fine");
                        getch();
                        exit(1);
                    } 
                    fprintf(fprocess,"\n Remaining hanger capacity on base airport %d = %d",desti_index,hc->cap);
                    fclose(fprocess);
                }
                break;
            }
            base=base->next_base;    
        }    
        
    }     
                                  
    //  updated the demand to new values
    Demand_update(candidate,desti_index);
    
    //storing the route info and finding the extra carried demand and demand adjusted on these routes
    Store_route_info(candidate,new_take_off,time_diff,desti_index);
                         
                    //re-inserting the aircraft as per its next takeoff time      
                  if(SENDING_TO_BASE==NO)
                  {
                          
                            
                          //making a new node from the candidate node with new start airport n take off time
                          item=(struct Fleet *)malloc(sizeof(struct Fleet));
                          item->ac_index=candidate->ac_index;
                          item->ap_index=desti_index;
						  item->BH_remain=candidate->BH_remain;//NEU_BH
						  item->BH_max=candidate->BH_max;//NEU_BH
						  item->ac_type=candidate->ac_type;//NEU_MAC
                          airport=head_airport;
                          for(i=1;i<desti_index;i++)
                          {
                              airport=airport->next_airport;
                          }    
                          strcpy(item->ap_name,airport->name);
                          item->end_time=candidate->end_time;
                          item->life=candidate->life;
                          item->takeoff_time=new_take_off;
                          item->demand_carried=0;
                          item->residual_capacity=0;
                          item->next_aircraft=NULL;
                          Insert_sort(item); // inserting new node in the list
                          if(user_choice==YES)
                          {
                              FILE *fprocess=fopen("Output/PROCESS.txt","a");
                                if(fprocess==NULL)
                                   {
                                    
                                    printf("\n Cann't open the file Output/PROCESS in everything_fine");
                                    getch();
                                    exit(1);
                                    }
                                fprintf(fprocess,"\n Re-inserting the aircraft as per its takeoff time at airport %d ",item->ap_index);
                                fclose(fprocess);
                            }   
                      } //end of if(REACHED_BASE==NO)   
                         
                          //it shows that we schedule it succesfully
                            STATUS =YES; 
                
                          
}//end of function  
/* Algorithm ------------- 

                    (1.2.1.1)
store n update			
{
	Store the Source station name and takeoff time 
store the destination name and landing time
Update the demand in system
Update the remaining end time of aircraft 
IF(destination is a base station n last stop)
{
	update the hanger availability of base station
}

Update the aircraft info
{
	station = destination station
	take off time = new take off time
}
restore the aircraft in the sorted list as per its take off time
status =yes

}

*/
 
void Free_list(void);

void Free_list()
{

	//to be deleted with free (struct types for temp pointers):
	//Grey_indices: head_select, head_reject, head_probable (OK)
	//Airports: head_airport (OK)
	//Data: head_data (OK)
	//Fleet: head_fleet, head_aircraft_list (already free in sorted_fleet, OK)
	//AC_data: head_ac_data and sublist static_cost
	//AEA_input
	//Bases: head_base
	//Grey: head_grey
	//Final_grey: head_final_grey
	//Whitening_decision: BIG, SMALL
	//route_detail: head_route, last_route
	
	struct Fleet *pTempFleet=NULL;
	struct AC_data *pTempACdata=NULL;
	struct static_cost *pTempStaticCost1=NULL;
	struct static_cost *pTempStaticCost2=NULL;
	struct Grey_indices *pTempGreyIndices=NULL;
	struct Airports *pTempAirports=NULL;
	struct timing *pTempTiming1=NULL;
	struct timing *pTempTiming2=NULL;
	struct Data *pTempData=NULL;
	struct aea_input *pAEAinput;
	struct Bases *pTempBases=NULL;
	struct Grey *pTempGrey=NULL;
	struct Final_grey *pTempFinalGrey=NULL;
	struct route_detail *pTempRouteDetail=NULL;
	struct extra *pTempExtra1=NULL;
	struct extra *pTempExtra2=NULL;

	//free memory in head_select, head_reject, head_probable
	//[allocated in initialize_grey_index (each set=Airport No.]
	while(head_select!=NULL)
		{
			pTempGreyIndices=head_select;
			head_select=head_select->next_ap_grey;
			free(pTempGreyIndices);
		}
	while(head_reject!=NULL)
		{
			pTempGreyIndices=head_reject;
			head_reject=head_reject->next_ap_grey;
			free(pTempGreyIndices);
		}
	while(head_probable!=NULL)
		{
			pTempGreyIndices=head_probable;
			head_probable=head_probable->next_ap_grey;
			free(pTempGreyIndices);
		}
	pTempGreyIndices=NULL;

	//free memory in head_airport
	while(head_airport!=NULL)
		{
			//first delete sublist takeoff
			pTempTiming1=head_airport->takeoff_time;
			while(pTempTiming1!=NULL)
			{
				pTempTiming2=pTempTiming1;
				pTempTiming1=pTempTiming1->next_time;
				free(pTempTiming2);
			}
			//second delete sublist landing
			pTempTiming1=head_airport->landing_time;
			while(pTempTiming1!=NULL)
			{
				pTempTiming2=pTempTiming1;
				pTempTiming1=pTempTiming1->next_time;
				free(pTempTiming2);
			}
			pTempTiming2=NULL;
			//third delete mainlist
			pTempAirports=head_airport;
			head_airport=head_airport->next_airport;
			free(pTempAirports);
		}
	pTempAirports=NULL;

	//free memory in head_data
	//[allocated in read_input (airportno^^^2)]
	while(head_data!=NULL)
	{
		pTempData=head_data;
		head_data=head_data->next_data;
		free(pTempData);
	}
	pTempData=NULL;

	//free memory in head_aircraft_list 
	//[allocated in sorted fleet (Fleet Size) & everything_fine (sets = total flights-Fleet Size)]
	while(head_aircraft_list!=NULL)
		{
			pTempFleet=head_aircraft_list;
			head_aircraft_list=head_aircraft_list->next_aircraft;
			free(pTempFleet);
		}
	pTempFleet=NULL;

	//free memory in head_ac_data
	//[allocated in read_input (no. of ac_types)]
	while(head_ac_data!=NULL)
		{
			//first delete sublist static_cost
			pTempStaticCost1=head_ac_data->static_cost;
			while(pTempStaticCost1!=NULL)
			{
				pTempStaticCost2=pTempStaticCost1;
				pTempStaticCost1=pTempStaticCost1->next_cost;
				free(pTempStaticCost2);
			}
			pTempStaticCost2=NULL;	
			//then main list
			pTempACdata=head_ac_data;
			head_ac_data=head_ac_data->next_type;
			free(pTempACdata);
		}
	pTempACdata=NULL;

	//free memory in head_aea_input
	//[allocated in read_input (no of actypes)]
	while(head_aea_input!=NULL)
	{
		pAEAinput=head_aea_input;
		head_aea_input=head_aea_input->next_ac;
		free(pAEAinput);
	}
	pAEAinput=NULL;


	//free memory in head_base
	//[allocated in read_input (no of bases)]
	while(head_base!=NULL)
	{
		pTempBases=head_base;
		head_base=head_base->next_base;
		free(pTempBases);
	}
	pTempBases=NULL;

	//free memory in head_grey
	//[allocated in cargo_schedulling(no. of grey indices)]
	while(head_grey!=NULL)
	{
		pTempGrey=head_grey;
		head_grey=head_grey->next_grey;
		free(pTempGrey);
	}
	pTempGrey=NULL;

	//free memory in head_final_grey
	//[allocated in get_grey_category (no. aiports)] 
	while(head_final_grey!=NULL)
	{
		pTempFinalGrey=head_final_grey;
		head_final_grey=head_final_grey->next_ap_index;
		free(pTempFinalGrey);
	}
	pTempFinalGrey=NULL;

	//free memory in BIG, SMALL
	//[allocated in read_input (1)]
	free(BIG);
	free(SMALL);
	BIG=NULL;
	SMALL=NULL;

	//free memory in head_route (&last_route)
	//[allocated in store_route_info(no. routes)&store_updated_route(extras)]
		while(head_route!=NULL)
		{
			//first delete sublist extra
			pTempExtra1=head_route->extra_detail;
			while(pTempExtra1!=NULL)
			{
				pTempExtra2=pTempExtra1;
				pTempExtra1=pTempExtra1->next_extra;
				free(pTempExtra2);
			}
			pTempExtra2=NULL;
			//second delete mainlist
			pTempRouteDetail=head_route;
			head_route=head_route->next;
			free(pTempRouteDetail);
		}
	pTempRouteDetail=NULL;

} // end of function
//#include"if_same_index.h"
int Get_airport_data(struct Fleet *);


int Get_airport_data(struct Fleet *candidate)
{
    int i,j;
    int HIGHEST_PRIORITY_FLAG;//to check if we gor any base station with priority=1
    int best_index,selected_index;
    char best_cat;
    float best_grey_index,best_demand;
	float temp_capacity; //NEU_MAC
    int ac_req;
	int *ap_with_belf=NULL;//NEU_SH
    //NEU_VLA old:float ap_demand[AIRPORT_NO],ap_capacity[AIRPORT_NO],line_sum[AIRPORT_NO],BELF_each[AIRPORT_NO];//NEU_SH
	//NEU_VLA start
	float *ap_demand=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *ap_capacity=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *line_sum=(float *)malloc(AIRPORT_NO*sizeof(float));
	//float *BELF_each=(float *)malloc(AIRPORT_NO*sizeof(float)); CHMANAV5 DEFINING BELF_EACH AS A 2D POINTER RATHER THAN AS 1D POINTER
float** BELF_each;

int numRows, numCols;
/* Used as indexes as in matrix[x][y]; */
int x, y; 
/*
 * Get values into numRows and numCols somehow.
 */


/* Allocate pointer memory for the first dimension of a matrix[][]; */
BELF_each = (float **) malloc(AIRPORT_NO * sizeof(float *));
/* Allocate integer memory for the second dimension of a matrix[][]; */
for(x = 0; x < AIRPORT_NO; x++)
{
    BELF_each[x] = (float *) malloc(AIRPORT_NO * sizeof(float));
   }
	if (ap_demand == NULL || ap_capacity == NULL || line_sum == NULL || BELF_each == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end
	//NEU_SH old:float ap_demand[AIRPORT_NO],ap_capacity[AIRPORT_NO],line_sum[AIRPORT_NO],capacity_each[AIRPORT_NO],BELF_each[AIRPORT_NO];
    float BELF;
        float random_value;
    time_t current_time;
    struct tm * timeinfo;
    struct Data *data,*candidate_data;
    struct Airports *airport,*candidate_airport;
    struct Fleet *temp_head;
    struct Final_grey *grey;
    FILE *fprocess;
    //FILE *fp;
    //fp=fopen(output_filename,"a");
    //fprintf(fp,"\n for aircraft %d, source %d",candidate->ac_index,candidate->ap_index);
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in get_airport_data");
            getch();
            exit(1);
        }    
    }     

    //going upto the airport where our candidate aircraft is available
    data=head_data;
    for(i=1;i<candidate->ap_index;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    candidate_data=data;
    //reading the demand and capacity from candidate airport to all other airports
    for(j=0;j<AIRPORT_NO;j++)
    {
        ap_demand[j]=candidate_data->demand;
        //NEU_MAC old: ap_capacity[j]=candidate_data->capacity;
		ap_capacity[j]=Get_capacity(candidate->ac_type,candidate->ap_index,(j+1));//NEU_MAC
        candidate_data=candidate_data->next_data;
    }

    //reading the BELF for takeoff from the candidate airport
    airport=head_airport;
    for(i=1;i<candidate->ap_index;i++)
    airport=airport->next_airport;
//    BELF=airport->BELF; CHMANAV4 COMMENTED THE BELF ONLY DPENDENT ON TAKEOFF AIRPORT 
    
    if(user_choice ==YES)
        fprintf(fprocess,"\n\n Source airport = %d",candidate->ap_index);


    //starting the search
    best_index=-1;
    
    if(best_index==-1)  
    {
        if(user_choice ==YES)
        fprintf(fprocess,"\n\n Looking if for any airport demand to min 50percent of routes >BELF or outgoing demand to dest. airport >BELF"); //NEU_SH
        
        //BELF at each airport
        //airport=head_airport;		CHMANAV7 CHANGING BELF from AIRPORT DEPENDENT
		data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
		for(j=0;j<AIRPORT_NO;j++)
        {
            BELF_each[i][j]=data->BELF;
            data=data->next_data;
        }
		
		
		/* NEU_SH deactivation start (line_sum calculation not needed anymore)
		// Initializing line_sum  to zero
        for(i=0;i<AIRPORT_NO;i++)
        {
        line_sum[i]=0;         
        }
        //sum of all outgoing demands at each airport =line sum
        data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            line_sum[i]+=data->demand;
            data=data->next_data;
        } 

		//NEU_SH
        //at each airport look how many aircraft already available there and does it require any more Aircraft                          
        for(i=0;i<AIRPORT_NO;i++)
        {
			capacity_each[i]=0;
			//*****temp_head=candidate;//list starts from here, not from head_fleet
            temp_head=candidate;
           // for(j=0;j<fleet_size;j++)
            while(temp_head!=NULL)
            {
                if((temp_head->ap_index==i+1)&&(temp_head->ac_index!=candidate->ac_index))
                {
                    if(user_choice==YES)
                    fprintf(fprocess,"\n Aircraft no. %d is available at airport %d",temp_head->ac_index,i+1);
					//search for the lowest outgoing capacity possible from the tertiary airport NEU_MAC start
					for(j=0;j<AIRPORT_NO;j++)
					{
						temp_capacity=Get_capacity(temp_head->ac_type,temp_head->ap_index,j+1);
						if (capacity_each[i]==0 || (capacity_each[i] > temp_capacity && temp_capacity!=0 ))
						capacity_each[i]=temp_capacity;
					} 
					line_sum[i]-=capacity_each[i]; //substracts lowest outgoing capacity possible from the tertiary airport
					//NEU_MAC end
                }    
                temp_head=temp_head->next_aircraft;
            }
        }//NEU_SH
		//NEU_SH deactivation end*/
   
        /* NEU_MAC deactivation start (capacity_each[] not needed anymore)
		//takeoff capacity of aircraft at each airport
        data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            capacity_each[i]=data->capacity;//reading the first entry in capacity line
            for(j=0;j<AIRPORT_NO-1;j++)//going to last element in that line
            data=data->next_data;
            if(capacity_each[i]==0)//if first entry is zero, reading last entry //why??! strange..
            {
                capacity_each[i]=data->capacity; //HERE!
            }
            data=data->next_data;//going to next capacity line    
        } NEU_MAC deactivation end */

		//printing the outgoing demands,takeoff capacity and BELF from each airport
        if(user_choice ==YES)
        {
            //NEU_MAC old: fprintf(fprocess,"\n\nOutgoing demands, Takeoff capacity and BELF ");
			fprintf(fprocess,"\n\nOutgoing Demand, Outgoing Capacity, BELF ");//NEU_MAC
            for(i=0;i<AIRPORT_NO;i++)//CHMANAV8 PRINITNG THE BELF AS ROUTE DEPENDENT ONLY
			for(j=0;j<AIRPORT_NO;j++)
				fprintf(fprocess,"\n from airport %d to airport %d = %.2f, %.2f, %.2f",i+1,j+1,ap_demand[i],ap_capacity[i],BELF_each[i][j]);
				//NEU_MAC old: fprintf(fprocess,"\n from airport %d = %.2f, %.2f and %.2f",i+1,line_sum[i],capacity_each[i],BELF_each[i]);


        }   

		//NEU_SH
		//counting the no. of directions with demand>BELF from each airport
		ap_with_belf = (int *)malloc(AIRPORT_NO*sizeof(int));
		data=head_data;
		for(i=0;i<AIRPORT_NO;i++)
		ap_with_belf[i]=0;
		i=0;
		if(user_choice ==YES)
			fprintf(fprocess,"\n Tertiary Destinations with Demand>BELF:");//NEU_MAC
		while(data!=NULL)
		{
			j=0;
			while(data!=NULL && j<AIRPORT_NO)
			{
				temp_capacity=Get_capacity(candidate->ac_type,i+1,j+1); //NEU_MAC
				//NEU_MAC old: if (data->demand >= (data->capacityBELF_each[i]) && data->capacity != 0)
				//CHMANAV9 BELF_EACH IS NOW A 2D POINTER
				if (data->demand >= (temp_capacity*BELF_each[i][j]) && temp_capacity != 0) //NEU_MAC
				ap_with_belf[i]++;
				data=data->next_data;
				j++;
			}
			//printf("%d\t",ap_with_belf[i]);
			if(user_choice ==YES)
				fprintf(fprocess,"\n Airport %d: %d",i+1,ap_with_belf[i]);//NEU_SH NEU_MAC
			i++;
		}
		//printf("\n\n");
		//NEU_SH

		//NEU_MAC: No modifications needed beyond this point

        //going upto the our candidate airport
        grey=head_final_grey;
        //going upto the airport where our candidate aircraft is available
        data=head_data;
        for(i=1;i<candidate->ap_index;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
       

         HIGHEST_PRIORITY_FLAG=NO;
                
        for(i=0;i<AIRPORT_NO;i++)
        {
            if(data->priority!=-1)//don't look the destination if priority=-1
            {
             if((data->priority==1)||(HIGHEST_PRIORITY_FLAG==NO))
             {  
				//NEU_SH old: if( outgoing demand > BELF at any airport or demand for any airport > BELF
				//NEU_SH old: if((  (line_sum[i] >= (capacity_each[i]*BELF_each[i]/100.0)) && (candidate->ap_index!=i+1) ) || ((ap_demand[i] >= (ap_capacity[i]*BELF/100)) && ap_capacity[i]!=0 ))
				//if ((min. 50% of tertiary AP from Dest. AP have Demand>BELF or demand from candidate AP to dest AP > BELF
				//min1:if(((ap_with_belf[i] >= 1) && (candidate->ap_index!=i+1)) || ((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
				if(((ap_with_belf[i] >= AIRPORT_NO/2) && (candidate->ap_index!=i+1)) || ((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH	
				//without tert AP:if(((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
				 {
						if(user_choice ==YES)
							fprintf(fprocess,"\n looking for airport number = %d as an option to go",i+1);
                
						if(data->priority==1 && HIGHEST_PRIORITY_FLAG==NO)//first highest priority route, 
						{
							HIGHEST_PRIORITY_FLAG=YES;
							best_index=-1; 
							if(user_choice ==YES)
								fprintf(fprocess,"\n Airport number %d has the highest priority (1) to go",i+1);                 
		                    
						}    
                
                
						if(best_index==-1)//means this is the first node
						{
							best_index=i;
							best_cat=grey->grey_char;
							best_grey_index=grey->grey_index;
							best_demand=data->demand;    
						}    
						else
						{
						   if(best_cat=='s'&& grey->grey_char=='s')
						   {
							   if(best_grey_index < grey->grey_index)
							   {
								   //printf("\n changing the best selection becouse we got better option in select category");
								   if(user_choice ==YES)
								   fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
								   best_index=i;
								   best_cat=grey->grey_char;
								   best_grey_index=grey->grey_index;
									best_demand=data->demand;
							   }
							   else if((best_grey_index == grey->grey_index))
							   {
									selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
									   if(selected_index==(i+1))
									   {
											  best_index=i;
											  best_cat=grey->grey_char;
											  best_grey_index=grey->grey_index;
											  best_demand=data->demand;
		                                 
									   }

							   }
		                       
		                           
		                           
						   }    
						   else if(best_cat!='s'&& grey->grey_char=='s')
						   {
							   //printf("\n changing the best selection becouse of category");
							   if(user_choice ==YES)
									fprintf(fprocess,"\n changing the best selection becouse of category");
		                       
							   best_index=i;
							   best_cat=grey->grey_char;
							   best_grey_index=grey->grey_index;
								best_demand=data->demand;
						   }    
						   else if(best_cat=='p'&& grey->grey_char =='p')
						   {
							   if(best_grey_index < grey->grey_index)
							   {
		                           
								   //printf("\n changing the best selection becouse we got better option in probable category");
								   if(user_choice ==YES)
										fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
								   best_index=i;
								   best_cat=grey->grey_char;
								   best_grey_index=grey->grey_index;
									best_demand=data->demand;
							   }
							   else if((best_grey_index == grey->grey_index))
							   {
								   selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
									   if(selected_index==(i+1))
									   {
											  best_index=i;
											  best_cat=grey->grey_char;
											  best_grey_index=grey->grey_index;
											  best_demand=data->demand;
		                                 
									   }

							   }
						   } 
                   
                            // next two sections - allowing reject category also as an option to go
                           //to remoce this, make next two section comment nad at start umcomment if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
                             
                            else if(best_cat=='r'&& grey->grey_char!='r')
                           {
                               //printf("\n changing the best selection becouse of category");
                               if(user_choice ==YES)
                                    fprintf(fprocess,"\n changing the best selection becouse of category");
                               
                               best_index=i;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                                best_demand=data->demand;
                           }    
                           else if(best_cat=='r'&& grey->grey_char =='r')
                           {
                               if(best_grey_index > grey->grey_index)
                               {
                                  // printf("\n changing the best selection becouse we got better option in reject category");
                                   if(user_choice ==YES)
                                        fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category");
                                   best_index=i;
                                   best_cat=grey->grey_char;
                                   best_grey_index=grey->grey_index;
                                    best_demand=data->demand;
                               }
                               else if((best_grey_index == grey->grey_index))
                               {
                                   selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
                                   if(selected_index==(i+1))
                                   {
                                          best_index=i;
                                          best_cat=grey->grey_char;
                                          best_grey_index=grey->grey_index;
                                          best_demand=data->demand;
                                     
                                   }


                               } 
                           } 
                           
                 
                 
                 
                 
                  }//end of else 
               
            }//end of   if(line_sum[i] >= (capacity_each[i]*BELF_each[i]/100) && (candidate->ap_index!=i+1) )
             }//end of if((data->priority==1||HIGHEST_PRIORITY_FLAG==NO)    
			 else
			 {
				 if(user_choice ==YES)
							  fprintf(fprocess,"\n The Airport %d don't have priority = 1",i+1);
	             
	             
			 }    
			 }//end of if(data->priority!=-1)
			 else
			 {
				 if(user_choice ==YES)
							  fprintf(fprocess,"\n The Airport %d has the priority = -1",i+1);
	             
			 }    
				  grey=grey->next_ap_index;  
				  data=data->next_data;

		 }   //end of   for(i=0;i<AIRPORT_NO;i++)  
            
                
                          
        
    }//end of if(best_index==-1)     
//fclose(fp);
	free(ap_with_belf);//NEU_SH
	free(ap_demand);//NEU_VLA
	free(ap_capacity);//NEU_VLA
	free(line_sum);//NEU_VLA
	free(BELF_each);//NEU_VLA
//returning the possible destination               
           if(best_index==-1)  
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n No Possible destination ");
                   fclose(fprocess); 
               }
               return(NOT_FOUND);
           }    
           else
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n Possible destination = %d ",(best_index+1));
                   fclose(fprocess); 
               }    
              return(best_index+1);
           } 
   
}//end of function    
int Get_ap_index(char *);

//finding index of the source airport
int Get_ap_index(char *ap_name)
{   
    struct Airports *airport;
   
    int i,flag=NOT_FOUND;
    airport=head_airport;   
    //while(fscanf(fp,"%s %f",temp,&delay)!=EOF && flag!=FOUND)
    for(i=0;i<AIRPORT_NO && flag!=FOUND;i++)
    {
        if(strcmp(airport->name,ap_name)==0)// match
        flag=FOUND;
        else
        airport=airport->next_airport;
    }    
                
if(flag!=FOUND)
    return(NOT_FOUND);
else
    return(airport->ap_index);
        
}    

void Get_base_cost_index (int,int);
void Get_base_cost_index(int source, int actype)//NEU_MAC
{
    int i,j,k,flag;//,desti_index; NEU_MAC
    //int freq;
    //float source_cost_ratio,desti_cost_ratio;
    //float cost[AIRPORT_NO];
    //NEU_VLA old: float base_cost_ratio[total_bases];
	//NEU_VLA start
	float *base_cost_ratio=(float *)malloc(total_bases*sizeof(float));
	float *cost_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(total_bases*sizeof(float));
	if (base_cost_ratio == NULL || cost_ratio == NULL || temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end
	//NEU_VLA old:float cost_ratio[AIRPORT_NO],max_ratio,temp_cost_ratio,cost,total_cost;
    float max_ratio,temp_cost_ratio,cost,total_cost;//NEU_VLA
    float temp_demand;
    //NEU_VLA old: float temp[total_bases];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Bases *base;
    struct Grey_indices *select,*probable,*reject;
    
	//NEU_MAC Start
	//initialise variable length arrays 
	float *base_route_cost=(float *)malloc(total_bases*sizeof(float));

	//calculation of specific route cost&total cost
	total_cost=0;
	base=head_base;
	for(i=0;i<total_bases;i++)
	{
		base_route_cost[i]=Get_cost(actype,source,base->base_index);
		total_cost+=base_route_cost[i];
		if(base!=NULL)
		base=base->next_base;
	}
	
	for(k=0;k<total_bases;k++)
	{
		if(total_cost==0)
        base_cost_ratio[k]=0;
        else
        base_cost_ratio[k]=base_route_cost[k]/total_cost;
        temp[k]=base_cost_ratio[k];//copying all cost ratio in a temp array  
	}
	free(base_route_cost);
	//NEU_MAC End


/* NEU_MAC paragraph deactivated start
	//Calculating total cost
	total_cost=0;
    base=head_base;
    for(k=0;k<total_bases;k++)
    {
        data=head_data;
        for(i=1;i<source;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
        
        for(i=1;i<base->base_index;i++)
        data=data->next_data;
        
        total_cost+=data->TOC;
        base=base->next_base;
    } 
    base=head_base;
        //calculating cost ratio from source to all possible base stations
        for(k=0;k<total_bases;k++)
        {
            desti_index=base->base_index;
            //going upto te right line in data array
            data=head_data;
            for(i=1;i<source;i++)
            for(j=0;j<AIRPORT_NO;j++)
            data=data->next_data;
            for(i=1;i<desti_index;i++)
            data=data->next_data;
            //if(source==desti_index)
            //base_cost_ratio[k]=1;
            //else
            if(total_cost==0)
            base_cost_ratio[k]=0;
            else
            base_cost_ratio[k]=data->TOC/total_cost;
            temp[k]=base_cost_ratio[k];//copying all cost ratio in a temp array  
            base=base->next_base;
        }
paragraph deactivated end NEU_MAC */              

    //sorting the array
    Bubble_sort(temp,total_bases);
    //maximum value of cost ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,total_bases,SMALL->cost);//array,size,percentile
    perbig_number = Percentile_calc(temp,total_bases,BIG->cost);
    //calculating the median
    median_number= Median_calc(temp,total_bases);
    
    
    //making an array of time ratio for all the airports
    for(i=0;i<AIRPORT_NO;i++)
    {
        flag=NO;
        //check if i+1th airport is our base airport
        base=head_base;
        for(j=0;j<total_bases;j++)
        {
            if(i+1 == base->base_index)
            {
                //if yes then copy its demand ratio
                flag=YES;
                cost_ratio[i]=base_cost_ratio[j];
                break;
            }
            base=base->next_base;
        }
        //else make ratio=0
        if(flag==NO)
        cost_ratio[i]=1;
    }    

    //calculating the cost index for select category
    
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if cost ratio <20th percentile ,cost index =1
        //else cost index = (x-max value)/(20th percen - max value)
        /*
        if(cost_ratio[j]==0)
        select->cost_index=0.0;
        else if (cost_ratio[j] >perbig_number)
        select->cost_index=1;
        else if(perbig_number==0)
        select->cost_index=0;
        else
        select->cost_index=cost_ratio[j]/perbig_number;
        
        select = select->next_ap_grey;
           */
        if(source==j+1)
        select->cost_index=1;
        else if(cost_ratio[j] < persmall_number)
        select->cost_index=1.0;
        else if(persmall_number==max_ratio)
        select->cost_index=0;
        else
        select->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       select=select->next_ap_grey;     
    }
   
    //calculating the cost index for probable category 
    
    probable=head_probable;    
    for(j=0;j<AIRPORT_NO;j++)
    {
        
        //if cost ratio <median ,cost index = cost ratio/median;
        //else if cost ratio >median, cost index = (cost ratio-max value)/(median - max value)
        if(cost_ratio[j]==0)
        probable->cost_index=0.0;
        else if(cost_ratio[j]< median_number)
        probable->cost_index=cost_ratio[j]/median_number;
        else if (median_number==max_ratio)
        probable->cost_index=0;
        else 
        probable->cost_index=(cost_ratio[j]-max_ratio)/(median_number-max_ratio);
       probable=probable->next_ap_grey;   
       
    }
    
    //calculating the cost index for reject category
    
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {
        
         if(source==j+1)
        reject->cost_index=0.0;
        else if (cost_ratio[j] >perbig_number)
        reject->cost_index=1;
        else if(perbig_number==0)
        reject->cost_index=0;
        else
        reject->cost_index=cost_ratio[j]/perbig_number;
        //data=data->next_data;
        reject = reject->next_ap_grey;
        // if cost ratio >80th percentile, cost index =1
        //else cost index = x/80th percen 
        /*
        if(cost_ratio[j]==0)
        reject->cost_index=1.0;
        else if(cost_ratio[j] < persmall_number)
        reject->cost_index=1.0;
        else if(persmall_number==max_ratio)
        reject->cost_index=0;
        else
        reject->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       reject=reject->next_ap_grey; 
       */
              
    }
    
	free(base_cost_ratio); //NEU_VLA
	free(cost_ratio); //NEU_VLA
	free(temp); //NEU_VLA


     //printing the cost index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_cost_index");
             getch();
             exit(1);
         }    
         
         /*
         fprintf(fprocess,"\n Cost ratio (tons/Rs.) => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",cost_ratio[j]);
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);  
          fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number);   
        */
         fprintf(fprocess,"\n cost Index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->cost_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->cost_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->cost_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
          

    

int Get_base_data(struct Fleet *);


int Get_base_data(struct Fleet *candidate)
{
    int i,j,k;
    int HIGHEST_PRIORITY_FLAG;//to check if we gor any base station with priority=1
    int base_aircraft;
    int best_index,selected_index;
    char best_cat;
    float best_grey_index,best_demand;
    struct Bases *base;
    struct Final_grey *grey;
    struct Fleet *temp_head;
    struct Data *data;
    float random_value;
    time_t current_time;
    struct tm * timeinfo;
    FILE *fprocess;
	struct Hanger_capacity *hc;
    //FILE *fp;
    //fp=fopen(output_filename,"a");
    //fprintf(fp,"\n searching for base for  %d aircraft ",candidate->ac_index);
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in get_base_data");
            getch();
            exit(1);
        }    
    }        

 
    //starting the search
    base=head_base;
    best_index=-1;
    if(best_index==-1)  
    {
   
        
        HIGHEST_PRIORITY_FLAG=NO;
        
        for(k=0;k<total_bases;k++)
        {
                         
            //going upto the our candidate airport to get grey details of all airports
            grey=head_final_grey;
           
            //reading the grey detail of the base station on route from our candidate airport
            for(i=1;i<base->base_index;i++)
            grey=grey->next_ap_index;
            
            //going upto the airport where our candidate aircraft is available
            data=head_data;
            for(i=1;i<candidate->ap_index;i++)
            for(j=0;j<AIRPORT_NO;j++)
            data=data->next_data;
            
            //reading the detail of the base station on route from our candidate airport
            for(i=1;i<base->base_index;i++)
            data=data->next_data;
            
            if(data->priority!=-1)//don't look the base if priority=-1
            {
             if((data->priority==1 )||(HIGHEST_PRIORITY_FLAG==NO))
             { 
            
            //find out how many aircraft are present their already
            base_aircraft=0;
            temp_head=candidate;
                while(temp_head!=NULL)//look all the fleets
                {
                    if((temp_head->ap_index==base->base_index)&&(temp_head->ac_index != candidate->ac_index))//if aircraft is on our base station
                    {
                        if(user_choice==YES)
                            fprintf(fprocess,"\n Aircraft no. %d is available at base airport %d",temp_head->ac_index,base->base_index);
                        //fprintf(fp,"\n Aircraft no. %d is available at base airport %d",temp_head->ac_index,base->base_index);
                       
                        base_aircraft++;
                    }    
                    temp_head=temp_head->next_aircraft;
                }
             if(user_choice==YES)
             {
                 fprintf(fprocess,"\n Total %d aircraft are available on base airport %d",base_aircraft,base->base_index);
                 
             }    
             //fprintf(fp,"\n Total %d aircraft are available on base airport %d",base_aircraft,base->base_index);
                 
            
            //if base station can handle more aircraft then only its my possible base station
            //I cann't assume that aircraft which are already their will leave the base staion for me
          
              //if we are on the base station itself then no need to check anything we will not go to anywhere
          /*
          if(base->base_index==candidate->ap_index)
          {
              best_index=base->base_index;
              if(user_choice ==YES)
                        fprintf(fprocess,"\n We are on the base station itself. No need to check other options");
              break;
          }      
          else if(base->hanger_capacity- base_aircraft >0)
          */
          //fprintf(fp,"\n base capacity = %d",base->hanger_capacity);
//CHMANAV 34 new snippet for getting base and aircraft dependent hanger capacity
		 hc=head_hc;
		 for (int manav=0;manav<i;manav++)
		  for (int manas=1;manas<candidate->ac_type;manas++)
			  hc=hc->next_hc;
			
        if(hc->cap - base_aircraft >0)
                  {
                //printf("\n looking for base airport number = %d as an option to go",base->base_index);
                if(user_choice ==YES)
                           fprintf(fprocess,"\n looking for base airport number = %d as an option to go",base->base_index);
                
                //fprintf(fp,"\n looking for base airport number = %d as an option to go",base->base_index);
                
                if(data->priority==1 && HIGHEST_PRIORITY_FLAG==NO)//first highest priority route, 
                {
                    HIGHEST_PRIORITY_FLAG=YES;
                    best_index=-1; 
                    if(user_choice ==YES)
                           fprintf(fprocess,"\n Base Airport number %d has the highest priority (1) to go",i+1);                 
                    
                } 
                
                //fprintf(fp,"\n (%d/%d/%d)",best_index,base->base_index,candidate->ap_index);
                
                if(best_index==-1)//means this is the first node
                {
                          best_index=base->base_index;
                          best_cat=grey->grey_char;
                          best_grey_index=grey->grey_index;
                          best_demand=data->demand;
                         
                }    
                else
                {
                    //fprintf(fp,"[ %c/%c ]",best_cat,grey->grey_char);
                    //fprintf(fp,"[ %.2f/%.2f ]",best_grey_index,grey->grey_index);
                   if(best_cat=='s'&& grey->grey_char=='s')
                   {
                       //fprintf(fp,"s[ %.2f/%.2f ]",best_grey_index,grey->grey_index);
                       if(best_grey_index < grey->grey_index)
                       {
                          // printf("\n changing the best selection becouse we got better option in select category");
                           if(user_choice ==YES)
                           fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
                           best_index=base->base_index;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                           best_demand=data->demand;
                       } 
                       
                       else if((best_grey_index == grey->grey_index))
                           {
                               //NEU_MAC old:selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time);//desti1,desti2,source
                               selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//NEU_MAC
							   if(selected_index==base->base_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                 
                               }    
                               /*
                               if(base->base_index==candidate->ap_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                  }
                                  else
                                  {    
                                      //calculating random variable
                                      current_time=time(NULL);
                                        timeinfo=localtime(&current_time);
                                        srand(time(NULL)*rand()*1000);
                                       random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                      if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                      fprintf(fp,"\n random value = %.2f",random_value);
                                      if(random_value>0.5)
                                      {
                                          // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                           if(user_choice ==YES)
                                           fprintf(fprocess,"\n changing the best selection becouse we got better option in select category (randomly)");
                                           best_index=base->base_index;
                                           best_cat=grey->grey_char;
                                           best_grey_index=grey->grey_index;
                                            best_demand=data->demand;
                                        }
                                  }       
                                  */ 
                           }
                               
                   }    
                   else if(best_cat!='s'&& grey->grey_char=='s')
                   {
                       
                       //printf("\n changing the best selection becouse of category");
                       if(user_choice ==YES)
                            fprintf(fprocess,"\n changing the best selection becouse of category");
                       
                       best_index=base->base_index;
                       best_cat=grey->grey_char;
                       best_grey_index=grey->grey_index;
                       best_demand=data->demand;
                   }    
                   else if(best_cat=='p'&& grey->grey_char =='p')
                   {
                       if(best_grey_index < grey->grey_index)
                       {
                           //printf("\n changing the best selection becouse we got better option in probable category");
                           if(user_choice ==YES)
                                fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
                           best_index=base->base_index;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                           best_demand=data->demand;
                       }
                       
                       else if((best_grey_index == grey->grey_index))
                           {
                                selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC
                               if(selected_index==base->base_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                 
                               }
                               /*
                               if(base->base_index==candidate->ap_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                  }
                                  else
                                  {    
                                       //calculating random variable
                                      current_time=time(NULL);
                                        timeinfo=localtime(&current_time);
                                        srand(time(NULL)*rand()*1000);
                                       random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                      if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                      fprintf(fp,"\n random value = %.2f",random_value);
                                      if(random_value>0.5)
                                      {
                                          // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                           if(user_choice ==YES)
                                           fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category (randomly)");
                                           best_index=base->base_index;
                                           best_cat=grey->grey_char;
                                           best_grey_index=grey->grey_index;
                                            best_demand=data->demand;
                                        } 
                                    }   
                                    */ 
                           } 
                             
                   } 
                   else if(best_cat=='r'&& grey->grey_char!='r')
                   {
                       //printf("\n changing the best selection becouse of category");
                       if(user_choice ==YES)
                            fprintf(fprocess,"\n changing the best selection becouse of category");
                       
                       best_index=base->base_index;
                       best_cat=grey->grey_char;
                       best_grey_index=grey->grey_index;
                       best_demand=data->demand;
                   }    
                   else if(best_cat=='r'&& grey->grey_char =='r')
                   {
                       if(best_grey_index > grey->grey_index)
                       {
                           //printf("\n changing the best selection becouse we got better option in reject category");
                           if(user_choice ==YES)
                                fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category");
                           best_index=base->base_index;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                           best_demand=data->demand;
                       } 
                       
                       else if((best_grey_index == grey->grey_index))
                           {
                                selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC
                               if(selected_index==base->base_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                 
                               }
                               /*
                              if(base->base_index==candidate->ap_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                  }
                                  else
                                  {    
                                      //calculating random variable
                                      current_time=time(NULL);
                                        timeinfo=localtime(&current_time);
                                        srand(time(NULL)*rand()*1000);
                                      random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                      if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                     fprintf(fp,"\n random value = %.2f",random_value);
                                     if(random_value>0.5)
                                      {
                                          // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                           if(user_choice ==YES)
                                           fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category (randomly)");
                                           best_index=base->base_index;
                                           best_cat=grey->grey_char;
                                           best_grey_index=grey->grey_index;
                                            best_demand=data->demand;
                                        } 
                                    } 
                                    */   
                           }   
                           
                   } 
                  }//end of else 
                
            }//end of if(base->hanger_capacity- base_aircraft >0)
            else
            {
                if(user_choice==YES)
                {
                    
                    fprintf(fprocess,"\n Base station %d is full upto its capacity",base->base_index);
                    
                }    
            }   
            }//end of if((data->priority==1 &&  HIGHEST_PRIORITY_FLAG==YES)||HIGHEST_PRIORITY_FLAG==NO)    
           else
             {
                 if(user_choice ==YES)
                              fprintf(fprocess,"\n The Base Airport %d don't have priority = 1",base->base_index);
                 
                 
             }    
             }//end of if(data->priority!=-1)
             else
             {
                 if(user_choice ==YES)
                              fprintf(fprocess,"\n The Base Airport %d has the priority = -1",base->base_index);
                 
             }
         base=base->next_base;            
        }//end of for(i=0;i<total_bases;i++)  
    }//end of  if(best_index==-1)          
// fclose(fp);
//returning the possible destination               
           if(best_index==-1)  
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n No Possible base station ");
                   fclose(fprocess); 
               }
               return(NOT_FOUND);
           }    
           else
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n Possible base station = %d ",best_index);
                   fclose(fprocess); 
               }    
              return(best_index);
           } 
}//end of function    
//void Get_base_network_design_index (int);
//void Get_base_network_design_index(int source)
void Get_base_demand_index(int );
void Get_base_demand_index(int source)
{
    int i,j,k,flag;
    //NEU_VLA old: float base_demand_ratio[total_bases],demand_ratio[AIRPORT_NO];
    float max_ratio;
    //NEU_VLA old: float temp[total_bases];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Bases *base;
    struct Grey_indices *select,*probable,*reject;
	//NEU_VLA start
	float *base_demand_ratio=(float *)malloc(total_bases*sizeof(float));
	float *temp=(float *)malloc(total_bases*sizeof(float));
	float *demand_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (base_demand_ratio == NULL || temp == NULL || demand_ratio == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
    //NEU_VLA end
    float line_sum;//[AIRPORT_NO],column_sum[AIRPORT_NO],total_demand;
    
   //getting sum of demand from source to all base stations
    base=head_base;
    line_sum=0;
    for(k=0;k<total_bases;k++)
    {
        data=head_data;
        for(i=1;i<source;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
        
        for(i=1;i<base->base_index;i++)
        data=data->next_data;
        
        line_sum+=data->demand;
        base=base->next_base;
        
    }        
   
        
        base=head_base;
        //calculating demand ratio from source to all possible base stations
        for(k=0;k<total_bases;k++)
        {
            //going upto te right line in data array
            data=head_data;
            for(i=1;i<source;i++)
            for(j=0;j<AIRPORT_NO;j++)
            data=data->next_data;
            for(i=1;i<base->base_index;i++)
            data=data->next_data;
           
            //demand ratio = (sum of to n fro demands on both nodes on that route)/total deamnd
            if(line_sum ==0)
            base_demand_ratio[k]=0;
            else
            //demand ratio = demand to destination/ total outgoing demand from that source
            base_demand_ratio[k]=(data->demand)/line_sum;//less demand, less ratio
            //data->current_demand_concentration=demand_ratio[j];
            temp[k]=base_demand_ratio[k];//copying all demand ratio in a temp array 
            base=base->next_base; 
            
        } 
      
    //printf("\n\n Before -");
   // for(i=0;i<AIRPORT_NO;i++)
    //printf("\n %.2f",temp[i]);
    //sorting the array
    Bubble_sort(temp,total_bases);//(array name,size)
    //printf("\n\n After -");
    //for(i=0;i<AIRPORT_NO;i++)
    //printf("\n %.2f",temp[i]);
    //getch();
    
    //maximum value of demand ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,total_bases,SMALL->demand);//array,size,percentile
    perbig_number = Percentile_calc(temp,total_bases,BIG->demand);
    //calculating the median
    median_number= Median_calc(temp,total_bases);
    //making an array of demand ratio for all the airports
    for(i=0;i<AIRPORT_NO;i++)
    {
        flag=NO;
        //check if i+1th airport is our base airport
        base=head_base;
        for(j=0;j<total_bases;j++)
        {
            if(i+1 == base->base_index)
            {
                //if yes then copy its demand ratio
                flag=YES;
                demand_ratio[i]=base_demand_ratio[j];
                break;
            }
            base=base->next_base;
        }
        //else make ratio=0
        if(flag==NO)
        demand_ratio[i]=0;
    }    




    //calculating the demand index for select category
    select=head_select;

        for(i=0;i<AIRPORT_NO;i++)
        {
            // if deamnd ratio >80th percentile, demand index =1
            //else demand index = x/80th percen 
            if(demand_ratio[i]==0)
            select->demand_index=0;
            else if (demand_ratio[i] >perbig_number)
            select->demand_index=1;
            else if(perbig_number==0)
            select->demand_index=0;
            else
            select->demand_index=demand_ratio[i]/perbig_number;
            select = select->next_ap_grey;
       
        }
        
   
    
    //calculating the demand index for probable category  
    probable=head_probable;  
    for(i=0;i<AIRPORT_NO;i++)
    {
        //if demand ratio <median ,demand index = demand ratio/median;
        //else if demand ratio >median, demand index = (demand ratio-max value)/(median - max value)
        if(demand_ratio[i]==0)
        probable->demand_index=0.0;
        else if(demand_ratio[i]< median_number)
        probable->demand_index=demand_ratio[i]/median_number;
        else if(median_number==max_ratio)
        probable->demand_index=0;
        else 
        probable->demand_index=(demand_ratio[i]-max_ratio)/(median_number-max_ratio);
        probable=probable->next_ap_grey;
    }
    
    //calculating the demand index for reject category
    reject=head_reject;
    for(i=0;i<AIRPORT_NO;i++)
    {

        //if demand ratio <20th percentile ,demand index =1
        //else demand index = (x-max value)/(20th percen - max value)
        if(demand_ratio[i]==0)
        reject->demand_index=1.0;
        else if(demand_ratio[i] < persmall_number)
        reject->demand_index=1.0;
        else if(persmall_number==max_ratio)
        reject->demand_index=0;
        else
        reject->demand_index=((demand_ratio[i]-max_ratio)/(persmall_number-max_ratio));
        reject=reject->next_ap_grey;  
    }    
     
	free(base_demand_ratio); //NEU_VLA
	free(temp); //NEU_VLA
	free(demand_ratio); //NEU_VLA


    
     //printing the demand index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_demand_index");
             getch();
             exit(1);
         }    
         fprintf(fprocess,"\n\n Total demand to bases = %.2f",line_sum);
         /*
         fprintf(fprocess,"\n Demand concentration => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.4f\t",demand_ratio[j]);    
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);       
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n Demand Index in different categories : (Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
         fprintf(fprocess,"\n");    
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->demand_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->demand_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->demand_index);
                 reject=reject->next_ap_grey;
             }
          
               
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
     
void Get_base_route_priority_index(int );

void Get_base_route_priority_index(int source)
{
    int i,j,k,flag,desti_index;
    float max_priority;
    //NEU_VLA old: float base_priority[total_bases],temp[total_bases],priority[AIRPORT_NO];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Bases *base;
    struct Grey_indices *select,*probable,*reject;
	//NEU_VLA start
	float *base_priority=(float *)malloc(total_bases*sizeof(float));
	float *temp=(float *)malloc(total_bases*sizeof(float));
	float *priority=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (base_priority == NULL || temp == NULL || priority == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end
        
    //copying all priorities in a temp array  
            //going upto te right line in data array
   base=head_base;
   for(k=0;k<total_bases;k++)
   {
        desti_index=base->base_index;
        //going upto te right line in data array
        data=head_data;
        for(i=1;i<source;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
        for(i=1;i<desti_index;i++)
        data=data->next_data;
   
        base_priority[k]=data->priority;
        temp[k]=base_priority[k];
        base=base->next_base;
    }    
    //sorting the array
    Bubble_sort(temp,total_bases);
    //maximum value of time ratio is at temp[0]
    max_priority = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,total_bases,SMALL->route_priority);//array,size,percentile
    perbig_number = Percentile_calc(temp,total_bases,BIG->route_priority);
    //calculating the median
    median_number= Median_calc(temp,total_bases);
 
 //making an array of route priority ratio for all the airports
    for(i=0;i<AIRPORT_NO;i++)
    {
        flag=NO;
        //check if i+1th airport is our base airport
        base=head_base;
        for(j=0;j<total_bases;j++)
        {
            if(i+1 == base->base_index)
            {
                //if yes then copy its demand ratio
                flag=YES;
                priority[i]=base_priority[j];
                break;
            }
            base=base->next_base;
        }
        //else make ratio=0
        if(flag==NO)
        priority[i]=-1;
    }
    //calculating the route prioroity index for select category
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if priority > BIG percentile, route_priority_index=1
        //else route_priority_index = (priority+1)/(BIG percentile+1)
        if(priority[j]==1)
        select->route_priority_index=1;
        else if (priority[j]==-1)
        select->route_priority_index=0;
        else if (priority[j] >= perbig_number)
        select->route_priority_index=1.0;
        else
        select->route_priority_index=(priority[j]+1)/(perbig_number+1);
        select=select->next_ap_grey;     
        
    }

    //calculating the route prioroity index for probable category
    probable=head_probable;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if priority > Median, route_priority_index=(priority-max prioroity)/(median-max priority)
        //else route_priority_index = (priority+1)/(median+1)
        if(priority[j]==1)
        probable->route_priority_index=0;
        else if (priority[j]==-1)
        probable->route_priority_index=0;
        else if(median_number==max_priority)
        probable->route_priority_index=1;
        else if (priority[j] < median_number)
        probable->route_priority_index=(priority[j]+1)/(median_number+1);
        else if(median_number==max_priority)
        probable->route_priority_index=0;
        else
        probable->route_priority_index=((priority[j]-max_priority)/(median_number-max_priority));
        

        probable=probable->next_ap_grey;     
        
    }
    

    //calculating the route prioroity index for reject category
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {

        //if priority <small percentile, route_priority_index=1;
        //else route_priority_index=(priority-max prioroity)/(small percentile-max priority)
        if(priority[j]==1)
        reject->route_priority_index=0;
        else if (priority[j]==-1)
        reject->route_priority_index=1;
        else if (priority[j] < persmall_number)
        reject->route_priority_index=1.0;
        else if (persmall_number==max_priority)
        reject->route_priority_index=0;
        else
        reject->route_priority_index=((priority[j]-max_priority)/(persmall_number-max_priority));

        reject=reject->next_ap_grey;     
        
    }
	
	free(base_priority);//NEU_VLA
	free(temp);//NEU_VLA
	free(priority);//NEU_VLA

    //printing the route priority index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_route_priority_index");
             getch();
             exit(1);
         } 
         /*
         fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);   
         
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n route_priority_index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->route_priority_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->route_priority_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->route_priority_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
         
         /*
         fprintf(fprocess,"\n\n Route Priority Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->route_priority_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n Route Priority Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->route_priority_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n Route Priority Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->route_priority_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);     
     }//end of if(user_choice==YES)  
     
}   //end of function 

*/
void Get_base_time_index (int,float *,int, float);
void Get_base_time_index(int source,float *flytime,int size,float takeoff_time)
{
    int i,j,k,desti_index,flag;
    float total_fly_time;
    //NEU_VLA old: float base_time_ratio[total_bases];
    //NEU_VLA old: float time_ratio[AIRPORT_NO],max_ratio,temp_time_ratio,time;
	float max_ratio,temp_time_ratio,time;//NEU_VLA
    //NEU_VLA old: float temp[total_bases];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Bases *base;
    struct Grey_indices *select,*probable,*reject;

	//NEU_VLA start
	float *base_time_ratio=(float *)malloc(total_bases*sizeof(float));
	float *time_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(total_bases*sizeof(float));
	if (base_time_ratio == NULL || time_ratio == NULL || temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end

total_fly_time=0;
   base=head_base;
   for(k=0;k<total_bases;k++)
   {
       desti_index=base->base_index;
       total_fly_time+=flytime[desti_index-1];
       base=base->next_base;
   }    
 
   base=head_base;
   for(k=0;k<total_bases;k++)
   {
        desti_index=base->base_index;
        //going upto te right line in data array

            
        if(source==desti_index)//best case i m on base station itself
            base_time_ratio[k]=0;
        else if(flytime[desti_index-1]>(max_time_slot-takeoff_time))//if flying time is more then remaining time, don't go on such routes
            base_time_ratio[k]=1;
        else
            base_time_ratio[k]=flytime[desti_index-1]/total_fly_time;
        temp[k]=base_time_ratio[k]; //copying all time ratio in a temp array 
            
        base=base->next_base;
    }            
 /*
       time=0;
        for(j=0;j<total_bases;j++)
        time+=base_time_ratio[j];
 
        if(time==0)//means all demand satisfied no demand to any base station
        {
            base=head_base;
           for(k=0;k<total_bases;k++)
            {
                desti_index=base->base_index;
                //going upto te right line in data array
                data=head_data;
                for(i=1;i<source;i++)
                for(j=0;j<AIRPORT_NO;j++)
                data=data->next_data;
                for(i=1;i<desti_index;i++)
                data=data->next_data;
                
                
                if(source==desti_index)
                base_time_ratio[k]=0;
                else
                base_time_ratio[k]=1/flytime[desti_index-1];
                //cost_ratio[i][j]=((((data->TOC)/temp_demand)/fly_time[i*AIRPORT_NO+j])/((system_cost/system_demand)/max_time_slot));
                //cost_ratio[i][j]=(data->TOC/temp_demand)/(system_cost/system_demand);
                //cost_ratio[i][j]=(data->TOC/temp_demand);
                temp[k]=base_time_ratio[k];//copying all cost ratio in a temp array  
                base=base->next_base;
            }    
        }           
   */
    //sorting the array
    Bubble_sort(temp,total_bases);
    //maximum value of time ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,total_bases,SMALL->time);//array,size,percentile
    perbig_number = Percentile_calc(temp,total_bases,BIG->time);
    //calculating the median
    median_number= Median_calc(temp,total_bases);
    
    //making an array of time ratio for all the airports
    for(i=0;i<AIRPORT_NO;i++)
    {
        flag=NO;
        //check if i+1th airport is our base airport
        base=head_base;
        for(j=0;j<total_bases;j++)
        {
            if(i+1 == base->base_index)
            {
                //if yes then copy its demand ratio
                flag=YES;
                time_ratio[i]=base_time_ratio[j];
                break;
            }
            base=base->next_base;
        }
        //else make ratio=0
        if(flag==NO)
        time_ratio[i]=1;
    }    

    //calculating the time index for select category
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        if(source==j+1)
        select->time_index=1;
        //else if (time_ratio[i][j] >=perbig_number)
       // reject->time_index=1;
        //else
       // reject->time_index=time_ratio[i][j]/perbig_number;
        else if (time_ratio[j]==0 && persmall_number== 0)
        select->time_index=0;
        else if(time_ratio[j] < persmall_number)
        select->time_index=1.0;
        else if(persmall_number==max_ratio)
        select->time_index=0;
        else
        select->time_index=((time_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
        select = select->next_ap_grey;
        /*
        //if time ratio <20th percentile ,time index =1
        //else time index = (x-max value)/(20th percen - max value)
        if(time_ratio[j]==0)
        select->time_index=0.0;
        else if (time_ratio[j] >perbig_number)
        select->time_index=1;
        else if (perbig_number==0)
        select->time_index=0;
        else
        select->time_index=time_ratio[j]/perbig_number;
        select=select->next_ap_grey;  
        */   
    }
    
    //calculating the time index for probable category  
    probable=head_probable;  
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if time ratio <median ,time index = time ratio/median;
        //else if time ratio >median, time index = (time ratio-max value)/(median - max value)
        if(time_ratio[j]==0)
        probable->time_index=0.0;
        else if(time_ratio[j]<median_number)
        probable->time_index=time_ratio[j]/median_number;
        else if (median_number==max_ratio)
        probable->time_index=0;
        else 
        probable->time_index=(time_ratio[j]-max_ratio)/(median_number-max_ratio);
        probable=probable->next_ap_grey;
    }
    
    //calculating the time index for reject category
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {
        /*
        // if time ratio >80th percentile, time index =1
        //else time index = x/80th percen 
        if(time_ratio[j]==0)
        reject->time_index=1.0;
        else if(time_ratio[j] < persmall_number)
        reject->time_index=1.0;
        else if(persmall_number==max_ratio)
        reject->time_index=0;
        else
        reject->time_index=((time_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
        reject = reject->next_ap_grey;
        */
        if(source==j+1)
        reject->time_index=0;
        //else if(time_ratio[i][j] <= persmall_number)
        //select->time_index=1.0;
        //else
        //select->time_index=((time_ratio[i][j]-max_ratio)/(persmall_number-max_ratio));
        //else if( time_ratio[j] == 0 && perbig_number==0)//all demand satisfied
        //select->time_index=0;
        else if (time_ratio[j] >perbig_number)
        reject->time_index=1;
        else if (perbig_number==0)
        reject->time_index=0;
        else
        reject->time_index=time_ratio[j]/perbig_number;
        reject=reject->next_ap_grey;
    }    
     
	free(base_time_ratio);//NEU_VLA
	free(time_ratio);//NEU_VLA
	free(temp);//NEU_VLA
    
     //printing the time index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_time_index");
             getch();
             exit(1);
         }    
         fprintf(fprocess,"\n\n Remaining Operating Time = %.2f",max_time_slot-takeoff_time);
         /*
         fprintf(fprocess,"\n Time ratio (tons/min.) => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[j]);
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);  
             
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n Time Index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->time_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->time_index);
                 probable=probable->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->time_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
         
}    //end of function
         
         
         /*
         fprintf(fprocess,"\n\n demand/Time ratio");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[i][j]);
             
         }
           
         fprintf(fprocess,"\n\n demand/Time ratio sorted");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",temp[i*AIRPORT_NO+j]);
             
         }  
         
         fprintf(fprocess,"\n\n %.2f percentile = % .2f",BIG->cost,perbig_number); 
         fprintf(fprocess,"\n %.2f percentile = % .2f",SMALL->cost,persmall_number);
         fprintf(fprocess,"\n median = % .2f",median_number);
         */
       /*
         fprintf(fprocess,"\n\n demand/time Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->time_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n demand/time Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->time_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n demand/time Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->time_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);  
         */
         /*
         fprocess=fopen("Output/TEMP.txt","a");
         fprintf(fprocess,"\n Total System demand = %.2f",system_demand);
         fprintf(fprocess,"\n\n time ratio");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[i][j]);
             
         }
           
         fprintf(fprocess,"\n\n time ratio sorted");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",temp[i*AIRPORT_NO+j]);
             
         }  
         
         fprintf(fprocess,"\n\n %.2f percentile = % .2f",BIG->cost,perbig_number); 
         fprintf(fprocess,"\n %.2f percentile = % .2f",SMALL->cost,persmall_number);
         fprintf(fprocess,"\n median = % .2f",median_number);
         fclose(fprocess);  
         */ 

float Get_capacity(int, int, int);
float Get_capacity(int iAircraftType, int source, int destination)

{
	//input values: Blockhours, Aircraftype
	int i,j;
	float fCapacity=0;
	//float fBH;
	float fR1,fR2,fR3,fPL1,fPL2;
	struct AC_data *temp_ac_data;
	struct Data *temp_data;

	/*//calculate Blocktime
	fBH = Get_route_BT(source,destination);*/

	if(source==destination)
	return fCapacity=0;

	//calculate Distance
	temp_data=head_data;
	//going up to right place
	for(i=1;i<source;i++)
	for(j=0;j<AIRPORT_NO;j++)
		temp_data=temp_data->next_data;
	for(i=1;i<destination;i++)
		temp_data=temp_data->next_data;
	
	//going to data of aircraft type
	temp_ac_data=head_ac_data;
	while(temp_ac_data->ac_type!=iAircraftType)
		temp_ac_data=temp_ac_data->next_type;

	//calculation of Payload <- Distance
	if(temp_data->distance<=temp_ac_data->fR1)
		fCapacity=temp_ac_data->fPL1;
	else if((temp_data->distance>temp_ac_data->fR1) && (temp_data->distance<=temp_ac_data->fR2))
		fCapacity=temp_ac_data->fPL1-((temp_data->distance-temp_ac_data->fR1)*(temp_ac_data->fPL1-temp_ac_data->fPL2)/(temp_ac_data->fR2-temp_ac_data->fR1));
	else if((temp_data->distance>temp_ac_data->fR2) && (temp_data->distance<=temp_ac_data->fR3))
		fCapacity=temp_ac_data->fPL2-((temp_data->distance-temp_ac_data->fR2)*temp_ac_data->fPL2/(temp_ac_data->fR3-temp_ac_data->fR2));
	else
	{
		printf("Route Impossible >maximumRange for Type %d",iAircraftType);
		getch();
		exit(1);
	}

	return fCapacity;

}    //end of function
         
          

    

void Get_cost_index(int,int);
void Get_cost_index(int source, int actype) //NEU_MAC

{
    int i,j;
    //int freq;
    //float source_cost_ratio,desti_cost_ratio;
    //float cost[AIRPORT_NO];
    //NEU_VLA old:float cost_ratio[AIRPORT_NO],max_ratio,temp_cost_ratio,cost,total_cost;
	float max_ratio,temp_cost_ratio,cost,total_cost;//NEU_VLA
    float temp_demand;
    //NEU_VLA old: float temp[AIRPORT_NO];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Grey_indices *select,*probable,*reject;
    //FILE *fp;
    
    //fp=fopen("Output/Process.txt","a");

	//NEU_VLA start
	float *cost_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (cost_ratio == NULL || temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end

	//NEU_MAC Start
	//initialise variable length arrays 
	float *route_cost=(float *)malloc(AIRPORT_NO*sizeof(float));

	//calculation of specific route cost&total cost
	total_cost=0;
	for(i=0;i<AIRPORT_NO;i++)
	{
		route_cost[i]=Get_cost(actype,source,(i+1));
		total_cost+=route_cost[i];
	}

	//calculation of cost ratio
	for(j=0;j<AIRPORT_NO;j++)
	{
		if(source==j+1)
        cost_ratio[j]=0;
        else
        cost_ratio[j]=route_cost[j]/total_cost;
        temp[j]=cost_ratio[j];//copying all cost ratio in a temp array  
	}
	free(route_cost);
	//NEU_MAC End

    /* NEU_MAC paragraph deactivated start
	//going upto the right line
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    //calculation of total cost 
    total_cost=0;  
    //fprintf(fp,"\n");  
     for(j=0;j<AIRPORT_NO;j++)
     {
         total_cost+=data->TOC;
         //fprintf(fp,"+ %.2f",data->TOC); 
         data=data->next_data;
     } 
	//going to the data point
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
   	//calculating the cost ratio
    for(j=0;j<AIRPORT_NO;j++)
    {

        if(source==j+1)
        cost_ratio[j]=0;
        else
        cost_ratio[j]=data->TOC/total_cost;

        temp[j]=cost_ratio[j];//copying all cost ratio in a temp array  
        data=data->next_data;
    } paragraph deactivated end NEU_MAC */
             

    //sorting the array
    Bubble_sort(temp,AIRPORT_NO);
    //maximum value of cost ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,AIRPORT_NO,SMALL->cost);//array,size,percentile
    perbig_number = Percentile_calc(temp,AIRPORT_NO,BIG->cost);
    //calculating the median
    median_number= Median_calc(temp,AIRPORT_NO);

    //calculating the cost index for select category
    //data=head_data;
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if cost ratio <20th percentile ,cost index =1
        //else cost index = (x-max value)/(20th percen - max value)
        if(source==j+1)
        select->cost_index=0;
        else if(cost_ratio[j] < persmall_number)
        select->cost_index=1.0;
        else if(persmall_number==max_ratio)
        select->cost_index=0;
        else
        select->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       select=select->next_ap_grey; 
       //data=data->next_data;   
        /*
        if(source==j+1)
        select->cost_index=0.0;
        else if (cost_ratio[j] >perbig_number)
        select->cost_index=1;
        else if(perbig_number==0)
        select->cost_index=0;
        else
        select->cost_index=cost_ratio[j]/perbig_number;
        data=data->next_data;
        select = select->next_ap_grey;
               */
    }
   
    //calculating the cost index for probable category 
    //data=head_data;
    probable=head_probable;    
    for(j=0;j<AIRPORT_NO;j++)
    {
        
        //if cost ratio <median ,cost index = cost ratio/median;
        //else if cost ratio >median, cost index = (cost ratio-max value)/(median - max value)
        if(source==j+1)
        probable->cost_index=0.0;
        else if(cost_ratio[j]==0 && median_number==0)
        probable->cost_index=0;
        else if(cost_ratio[j]< median_number)
        probable->cost_index=cost_ratio[j]/median_number;
        else if (median_number==max_ratio)
        probable->cost_index=0;
        else 
        probable->cost_index=(cost_ratio[j]-max_ratio)/(median_number-max_ratio);
       probable=probable->next_ap_grey;   
       //data=data->next_data;     
    }
    
    //calculating the cost index for reject category
    //data=head_data;
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {
        if(source==j+1)
        reject->cost_index=1;
        else if (cost_ratio[j] >perbig_number)
        reject->cost_index=1;
        else if(perbig_number==0)
        reject->cost_index=0;
        else
        reject->cost_index=cost_ratio[j]/perbig_number;
        //data=data->next_data;
        reject = reject->next_ap_grey;
        
        /*
        // if cost ratio >80th percentile, cost index =1
        //else cost index = x/80th percen 
        if(source==j+1)
        reject->cost_index=1.0;
        else if(cost_ratio[j] < persmall_number)
        reject->cost_index=1.0;
        else if(persmall_number==max_ratio)
        reject->cost_index=0;
        else
        reject->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       reject=reject->next_ap_grey; 
       data=data->next_data;        
       */
    }
    
	free(cost_ratio);//NEU_VLA
	free(temp);//NEU_VLA


     //printing the cost index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_cost_index");
             getch();
             exit(1);
         }    
         
         /*
         fprintf(fprocess,"\n Cost ratio  => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",cost_ratio[j]);
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);  
          fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number);   
        */
         fprintf(fprocess,"\n cost Index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->cost_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->cost_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->cost_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
          

    
void Get_demand_index(int,int);
void Get_demand_index(int source, int actype)
{
    int i,j;
	float ac_capacity; //NEU_MAC
    //NEU_VLA old: float demand_ratio[AIRPORT_NO],temp_demand;
	float temp_demand;//NEU_VLA
    float max_ratio;
    //NEU_VLA old: float temp[AIRPORT_NO];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Grey_indices *select,*probable,*reject;
    struct Airports *airport;
    //NEU_VLA old: float line_sum[AIRPORT_NO],total_demand,column_sum[AIRPORT_NO];
	float total_demand;//NEU_VLA

	//NEU_VLA start
	float *demand_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *line_sum=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *column_sum=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (demand_ratio == NULL || temp == NULL|| line_sum == NULL || column_sum == NULL)
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end

    //line sum = outgoing demand and column sum = incoming demand
    //initializing them to zero
    for(i=0;i<AIRPORT_NO;i++)
    {
         column_sum[i]=0;
         line_sum[i]=0;         
    }
    //sum of all outgoing demands at each airport =line sum
    //sum of all incoming demands at each airport = column sum
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        line_sum[i]+=data->demand;
        column_sum[j]+=data->demand;
        data=data->next_data;
    }    
    //printf("\n Sum of demand");
    //for(i=0;i<AIRPORT_NO;i++)
    //printf("\n %f %f",line_sum[i],column_sum[i]);
    
    //total demand = sum of all outgoing demands ( or sum of all incoming demands)
    total_demand=0;
    for(i=0;i<AIRPORT_NO;i++)
    total_demand+=line_sum[i];
    
    //going up to the right line in data array
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;

	//calculating demand ratio from source to all possible destinations
    for(j=0;j<AIRPORT_NO;j++)
    {
        
		//Get Capacity for the flight (NEU_MAC)
		ac_capacity=Get_capacity(actype,source,j+1);//NEU_MAC

		//NEU_MAC old_start
		/*if(data->demand > data->capacity)
		temp_demand=data->capacity;
        else
        temp_demand=data->demand; old_end*/
		if(data->demand > ac_capacity)
		temp_demand=ac_capacity;
        else
        temp_demand=data->demand;
		//NEU_MAC
        
        
        //deamnd ratio = (sum of to n fro demands on both nodes on that route)/total deamnd
        if(source ==j+1)
        demand_ratio[j]=0.0;   
        else if(total_demand==0)
        demand_ratio[j]=0;        
        else if(line_sum[source-1]==0)
        demand_ratio[j]=0;
        else
        demand_ratio[j]=temp_demand/line_sum[source-1];
        
        
        //demand_ratio[j]=(data->demand+line_sum[j])/total_demand;
        temp[j]=demand_ratio[j];//copying all demand ratio in a temp array  
        data=data->next_data;
    }    
   
    Bubble_sort(temp,AIRPORT_NO);//(array name,size)
    
    //maximum value of demand ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,AIRPORT_NO,SMALL->demand);//array,size,percentile
    perbig_number = Percentile_calc(temp,AIRPORT_NO,BIG->demand);
    //calculating the median
    median_number= Median_calc(temp,AIRPORT_NO);

    //calculating the demand index for select category
    select=head_select;
    for(i=0;i<AIRPORT_NO;i++)
    {
        // if deamnd ratio >80th percentile, demand index =1
        //else demand index = x/80th percen 
        if(i+1==source)
        select->demand_index=0.0;
        else if(total_demand==0)
        select->demand_index=0.0;
        else if (demand_ratio[i] >perbig_number)
        select->demand_index=1;
        else if(perbig_number==0)
        select->demand_index=0;
        else
        select->demand_index=demand_ratio[i]/perbig_number;
        select = select->next_ap_grey;
   
    }
    
    //calculating the demand index for probable category  
    probable=head_probable;  
    for(i=0;i<AIRPORT_NO;i++)
    {
        //if demand ratio <median ,demand index = demand ratio/median;
        //else if demand ratio >median, demand index = (demand ratio-max value)/(median - max value)
        if(i+1==source)
        probable->demand_index=0.0;
        else if(total_demand==0)
        probable->demand_index=0.0;
        else if(demand_ratio[i]==0 && median_number==0)
        probable->demand_index=0;
        else if(demand_ratio[i]< median_number)
        probable->demand_index=demand_ratio[i]/median_number;
        else if(median_number==max_ratio)
        probable->demand_index=0;
        else 
        probable->demand_index=(demand_ratio[i]-max_ratio)/(median_number-max_ratio);
        probable=probable->next_ap_grey;
    }
    
    //calculating the demand index for reject category
    reject=head_reject;
    for(i=0;i<AIRPORT_NO;i++)
    {

        //if demand ratio <20th percentile ,demand index =1
        //else demand index = (x-max value)/(20th percen - max value)
        if(i+1==source)
        reject->demand_index=1.0;
        else if(total_demand==0)
        reject->demand_index=1.0;
        else if(demand_ratio[i] < persmall_number)
        reject->demand_index=1.0;
        else if(persmall_number==max_ratio)
        reject->demand_index=0;
        else
        reject->demand_index=((demand_ratio[i]-max_ratio)/(persmall_number-max_ratio));
        reject=reject->next_ap_grey;  
    }    

    free(demand_ratio);//NEU_VLA
	free(temp);//NEU_VLA
	free(line_sum);//NEU_VLA
	free(column_sum);//NEU_VLA
    
     //printing the demand index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_demand_index");
             getch();
             exit(1);
         }    
         fprintf(fprocess,"\n\n Total demand = %.2f",total_demand);
         /*
         fprintf(fprocess,"\n Demand concentration => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.4f\t",demand_ratio[j]);    
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);       
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n Demand Index in different categories : (Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
         fprintf(fprocess,"\n");    
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->demand_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->demand_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->demand_index);
                 reject=reject->next_ap_grey;
             }
          
               
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function

//#include"check_airport_timing.h"
//CHMANAV 22 arguem,ents increased for candidate
void Get_fly_time(int,float,float *,int, struct Fleet *);
void Get_fly_time(int source,float takeoff_time,float *fly_time,int size,struct Fleet *candidate)
{

    int i,j,k,l;
    int desti_index,source_index;
    float block_time,max_delay,delay,time_diff,landing_time,next_takeoff_time;
    struct Airports *airport;
    time_t current_time;
    struct tm * timeinfo;
    FILE *fprocess;
    
       //CHMANAV 23 defining tat for usage for particular candidate aircraft
	struct tats *tat;
	  tat=head_tat;
	  for(i=1;i<candidate->ac_index;i++)
		  tat=tat->next_tat;

    //calculate the landing time on all the cities from this node
    
    for(k=0;k<AIRPORT_NO;k++)
    {        
            source_index=source;
            desti_index=k+1;
            //finding the block time between these OD pair
            block_time=Get_route_BT(source_index,desti_index);//(source,destination)
            //reading the max delay in landing at the destination airport
           
             airport=head_airport;
        
            for(j=0;j<desti_index;j++)
            {
                max_delay=airport->delay;
                airport=airport->next_airport;
            }    
            
            if(max_delay==0)
            delay=0;
            else if(source_index==desti_index)
            delay=0;
            else
            {
                //calculating the random delay in landing
                current_time=time(NULL);
                timeinfo=localtime(&current_time);
                srand(time(NULL)*rand());//initializing the random function
                delay= (max_delay * (((timeinfo->tm_sec /60.0)+( (rand()% (int)max_delay)/max_delay))/2));
            }    
            
   
           
                landing_time=takeoff_time+block_time+delay;
                time_diff=Check_airport_timing(landing_time,desti_index,YES);//YES means check for landing, NO means check for takeoff
                landing_time+=time_diff;
            
            if(source_index==desti_index)
            fly_time[k]=0;
            else if(SENDING_TO_BASE==YES)   
            fly_time[k]=landing_time-takeoff_time;
            else
            {//CHMANAV 23 using tat instead of TURN_AROUND_TIME
                next_takeoff_time=landing_time+tat->tat;
                time_diff=Check_airport_timing(next_takeoff_time,desti_index,NO);
                next_takeoff_time+=time_diff;
                fly_time[k]=next_takeoff_time-takeoff_time;
            }    
    
    }//end of for(i=0;i<AIRPORT_NO;i++)
    /*
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
            {
                printf("\n Cann't open the file Output/PROCESS in get_fly_time");
                getch();
                exit(1);
            }
           fprintf(fprocess,"\n Fly time between source to all possible destination");
           fprintf(fprocess,"\n");
           for(i=0;i<AIRPORT_NO;i++)
            {
               fprintf(fprocess,"\t%.2f",fly_time[i]); 
            }
            fclose(fprocess);    
                
    } 
    */
}//end of function
    


void Get_grey_category();

void Get_grey_category()
{
    int j;
    struct Grey_indices *select,*probable,*reject;
    struct Final_grey *final,*last_final;
    
    head_final_grey=NULL;
    select=head_select;
    probable=head_probable;
    reject=head_reject;
    
    //final category = cat. which has highest value of grey index
 
    for(j=0;j<AIRPORT_NO;j++)
    {
        final=(struct Final_grey *)malloc(sizeof(struct Final_grey));
        if(select->grey_index>=probable->grey_index && select->grey_index>=reject->grey_index)
        {
            final->grey_char='s';//select cat.
            final->grey_index=select->grey_index;
        }
        else if (probable->grey_index>=reject->grey_index)    
        {
            final->grey_char='p';//probable cat.
            final->grey_index=probable->grey_index;
        } 
        else
        {
            final->grey_char='r';// reject cat.
            final->grey_index=reject->grey_index;
        }  
           
        final->next_ap_index=NULL;
        if(head_final_grey==NULL)
        {
            head_final_grey=final;
            last_final=final;
        }
        
        else
        {
            last_final->next_ap_index=final;
            last_final=last_final->next_ap_index;
        }
        
        select=select->next_ap_grey;
        probable=probable->next_ap_grey;
        reject=reject->next_ap_grey;  
    }
    
    //printing the final grey detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't open the file Output/PROCESS in get_grey_category");
            getch();
            exit(1);
            }
        fprintf(fprocess,"\n\n Final Grey Category");
        final=head_final_grey;
        fprintf(fprocess,"\n");
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess," %c\t",final->grey_char);
                final=final->next_ap_index;
            }
         
        fprintf(fprocess,"\n\n Final Grey Index");       
        final=head_final_grey;
        fprintf(fprocess,"\n");
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess," %.2f\t",final->grey_index);
                final=final->next_ap_index;
            }
                 
        fclose(fprocess);
    }//end of if(user_choice==YES)    
}//end of function                
            
        
            
    
void Get_grey_index();

void Get_grey_index()
{
    int j,k;
    struct Grey_indices *select,*probable,*reject;
    struct Grey *grey_priority;
    float index;

    //calculating the grey index in select category
    select=head_select;

    for(j=0;j<AIRPORT_NO;j++)
    {
        select->grey_index=0;
        grey_priority=head_grey;
        for(k=0;k<total_grey_indices;k++)
        {
            switch(k)
            {
                case 0:
                    index=select->demand_index; 
                    break;
                case 1:
                    index=select->cost_index;
                    break;
                case 2:
                    index=select->time_index;
                    break;
                /*
                case 3:
                    index=select->network_design_index;
                    break;
                    */
                case 3:
                    index=select->route_priority_index;
                    break;
                    
                default :
                    {
                        printf("\n Error in reading grey indices priorities in get_grey-index\n Please check the input data");
                        getch();
                        exit(1);
                    }    
                
            }//end of switch  
            //grey index is the weightage sum of all the indices      
            select->grey_index+=(grey_priority->priority*index);
            grey_priority=grey_priority->next_grey;
        }
        select=select->next_ap_grey;
    }      
    

    //calculating the grey index in probable category
    probable=head_probable;
    
    for(j=0;j<AIRPORT_NO;j++)
    {
        probable->grey_index=0;
        grey_priority=head_grey;
        for(k=0;k<total_grey_indices;k++)
        {
            switch(k)
            {
                case 0:
                    index=probable->demand_index;            
                    break;
                case 1:
                    index=probable->cost_index;
                    break;
                case 2:
                    index=probable->time_index;
                    break;
                /*
                case 3:
                    index=probable->network_design_index;
                    break;
                    */
                case 3:
                    index=probable->route_priority_index;
                    break;
                    
                default :
                    {
                        printf("\n Error in reading grey indices priorities in get_grey_index\n Please check the input data");
                        getch();
                        exit(1);
                    }    
                
            }//end of switch  
            //grey index is the weightage sum of all the indices      
            probable->grey_index+=(grey_priority->priority*index);
            grey_priority=grey_priority->next_grey;
        }
        probable=probable->next_ap_grey;
    } 
    
    //calculating the grey index in reject category
    reject=head_reject;
    
    for(j=0;j<AIRPORT_NO;j++)
    {
        reject->grey_index=0;
        grey_priority=head_grey;
        for(k=0;k<total_grey_indices;k++)
        {
            switch(k)
            {
                case 0:
                    index=reject->demand_index;            
                    break;
                case 1:
                    index=reject->cost_index;
                    break;
                case 2:
                    index=reject->time_index;
                    break;
               /*
                 case 3:
                     index=reject->network_design_index;
                     break;
                     */
                case 3:
                    index=reject->route_priority_index;
                    break;
                    
                default :
                    {
                        printf("\n Error in reading grey indices priorities in get_grey_index\n Please check the input data");
                        getch();
                        exit(1);
                    }    
                
            }//end of switch  
            //grey index is the weightage sum of all the indices      
            reject->grey_index+=(grey_priority->priority*index);
            grey_priority=grey_priority->next_grey;
        }
        reject=reject->next_ap_grey;
    }   
    
    
    //printing the grey index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_grey_index");
             getch();
             exit(1);
         }    
                  

         fprintf(fprocess,"\n Grey index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->grey_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->grey_index);
                 probable=probable->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->grey_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
/*
         fprintf(fprocess,"\n\n grey Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->grey_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n grey Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->grey_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n grey Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->grey_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);     
     }//end of if(user_choice==YES)    
 

}//end of function    

*/
float Get_route_BT(int, int);

float Get_route_BT(int source, int desti)
{
    
    int i,j;
    struct Data *data;
    
    data=head_data;
    for(i=1;i<source;i++) // going to the required airport
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    for(j=1;j<desti;j++)//going to the required column in that line
        data=data->next_data;
    return(data->block_time);//now we are at right position-read it

}  //end of function  
void Get_route_priority_index(int );

void Get_route_priority_index(int source)
{
    int i,j;
    float max_priority;
    //NEU_VLA old:float temp[AIRPORT_NO];
	//NEU_VLA start
	float *temp=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Grey_indices *select,*probable,*reject;
        
    //copying all priorities in a temp array  
            //going upto te right line in data array
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    
    for(j=0;j<AIRPORT_NO;j++)
    {
        temp[j]=data->priority;
        data=data->next_data;
    }    
    //sorting the array
    Bubble_sort(temp,AIRPORT_NO);
    //maximum value of time ratio is at temp[0]
    max_priority = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,AIRPORT_NO,SMALL->route_priority);//array,size,percentile
    perbig_number = Percentile_calc(temp,AIRPORT_NO,BIG->route_priority);
    //calculating the median
    median_number= Median_calc(temp,AIRPORT_NO);

    //calculating the route prioroity index for select category
    select=head_select;
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if priority > BIG percentile, route_priority_index=1
        //else route_priority_index = (priority+1)/(BIG percentile+1)
        if(data->priority==1)
        select->route_priority_index=1;
        else if (data->priority==-1)
        select->route_priority_index=0;
        else if(source==j+1)
        select->route_priority_index=0.0;
        else if (data->priority >= perbig_number)
        select->route_priority_index=1.0;
        else
		select->route_priority_index=(data->priority)/(perbig_number);//NEU
		//NEU old: select->route_priority_index=(data->priority+1)/(perbig_number+1);
        select=select->next_ap_grey;     
        data=data->next_data;
    }

    //calculating the route prioroity index for probable category
    probable=head_probable;
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if priority > Median, route_priority_index=(priority-max prioroity)/(median-max priority)
        //else route_priority_index = (priority+1)/(median+1)
        if(data->priority==1)
        probable->route_priority_index=0;
        else if (data->priority==-1)
        probable->route_priority_index=0;
        else if(source==j+1)
        probable->route_priority_index=0.0;
        else if(median_number==max_priority)
        probable->route_priority_index=1;
        else if (data->priority < median_number)
		probable->route_priority_index=(data->priority)/(median_number);//NEU
        //NEU old: probable->route_priority_index=(data->priority+1)/(median_number+1);
        else if(median_number==max_priority)
        probable->route_priority_index=0;
        else
        probable->route_priority_index=((data->priority-max_priority)/(median_number-max_priority));
        

        probable=probable->next_ap_grey;     
        data=data->next_data;
    }
    

    //calculating the route prioroity index for reject category
    reject=head_reject;
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    for(j=0;j<AIRPORT_NO;j++)
    {

        //if priority <small percentile, route_priority_index=1;
        //else route_priority_index=(priority-max prioroity)/(small percentile-max priority)
        if(data->priority==1)
        reject->route_priority_index=0;
        else if (data->priority==-1)
        reject->route_priority_index=1;
        else if(source==j+1)
        reject->route_priority_index=1.0;
        else if (data->priority < persmall_number)
        reject->route_priority_index=1.0;
        else if (persmall_number==max_priority)
        reject->route_priority_index=0;
        else
        reject->route_priority_index=((data->priority-max_priority)/(persmall_number-max_priority));

        reject=reject->next_ap_grey;     
        data=data->next_data;
    }

	free(temp);//NEU_VLA

    //printing the route priority index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_route_priority_index");
             getch();
             exit(1);
         } 
         /*
         fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);   
         
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n route_priority_index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->route_priority_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->route_priority_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->route_priority_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
         
         /*
         fprintf(fprocess,"\n\n Route Priority Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->route_priority_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n Route Priority Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->route_priority_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n Route Priority Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->route_priority_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);     
     }//end of if(user_choice==YES)  
     
}   //end of function 

*/
//void Get_time_index (int,float *,int, float,float *);
//void Get_time_index(int source,float *flytime,int size,float takeoff_time,float *extra_fly)
void Get_time_index(int ,float *,int ,float );
void Get_time_index(int source,float *flytime,int size,float takeoff_time)
{
    int i,j,k;
    float temp_demand,total_fly;
	//NEU_VLA old: float time_ratio[AIRPORT_NO],max_ratio,temp_time_ratio;
    float max_ratio,temp_time_ratio;
    //NEU_VLA old: float temp[AIRPORT_NO];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Grey_indices *select,*probable,*reject;
	//NEU_VLA start
	float *time_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (temp == NULL || time_ratio == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end

 
       //going upto the right line in data array
        data=head_data;
        for(i=1;i<source;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
        
        total_fly=0;
        for(j=0;j<AIRPORT_NO;j++)
        total_fly+=flytime[j];
        
        //calculating time index for all the destination from source
        for(j=0;j<AIRPORT_NO;j++)
        {
            /*           
            if(data->demand > data->capacity)
            temp_demand=data->capacity;
            else
            temp_demand=data->demand;
            */
            if(max_time_slot==takeoff_time)
            time_ratio[j]=1;
            else if(source==j+1)
            time_ratio[j]=1;
            else if(flytime[j]>(max_time_slot-takeoff_time))//if flying time is more then remaining time, don't go on such routes
            time_ratio[j]=1;
            else
            //time_ratio[i][j]=(temp_demand/fly_time[i*AIRPORT_NO+j])/(system_demand/max_time_slot);
            //time_ratio[i][j]=(temp_demand/fly_time[i*AIRPORT_NO+j]);
            //time_ratio[j]=data->current_demand_concentration/temp_time_ratio;
            //time_ratio[j]=(temp_demand+extra_out_demand[j])/(flytime[j]+extra_fly[j]);
            //time_ratio[j]=(flytime[j]+extra_fly[j])/total_fly;
            time_ratio[j]= flytime[j]/total_fly;

            temp[j]=time_ratio[j]; //copying all time ratio in a temp array 
            data=data->next_data;
        }
   
    //sorting the array
    Bubble_sort(temp,AIRPORT_NO);
    //maximum value of time ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,AIRPORT_NO,SMALL->time);//array,size,percentile
    perbig_number = Percentile_calc(temp,AIRPORT_NO,BIG->time);
    //calculating the median
    median_number= Median_calc(temp,AIRPORT_NO);

    //calculating the time index for select category
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        // if time ratio >80th percentile, time index =1
        //else time index = x/80th percen 
        if(source==j+1)
        select->time_index=0;
        //else if (time_ratio[i][j] >=perbig_number)
       // reject->time_index=1;
        //else
       // reject->time_index=time_ratio[i][j]/perbig_number;
        else if (time_ratio[j]==0 && persmall_number== 0)
        select->time_index=0;
        else if(time_ratio[j] < persmall_number)
        select->time_index=1.0;
        else if(persmall_number==max_ratio)
        select->time_index=0;
        else
        select->time_index=((time_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
        select = select->next_ap_grey;
        /*
        //if time ratio <20th percentile ,time index =1
        //else time index = (x-max value)/(20th percen - max value)
        if(source==j+1)
        select->time_index=0.0;
        //else if(time_ratio[i][j] <= persmall_number)
        //select->time_index=1.0;
        //else
        //select->time_index=((time_ratio[i][j]-max_ratio)/(persmall_number-max_ratio));
        //else if( time_ratio[j] == 0 && perbig_number==0)//all demand satisfied
        //select->time_index=0;
        else if (time_ratio[j] >perbig_number)
        select->time_index=1;
        else if (perbig_number==0)
        select->time_index=0;
        else
        select->time_index=time_ratio[j]/perbig_number;
        select=select->next_ap_grey;    
        */ 
    }
    
    //calculating the time index for probable category  
    probable=head_probable;  
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if time ratio <median ,time index = time ratio/median;
        //else if time ratio >median, time index = (time ratio-max value)/(median - max value)
        if(source==j+1)
        probable->time_index=0.0;
        else if (time_ratio[j]==0 && median_number== 0)
        probable->time_index=0;
        else if(time_ratio[j]<median_number)
        probable->time_index=time_ratio[j]/median_number;
        else if (median_number==max_ratio)
        probable->time_index=0;
        else 
        probable->time_index=(time_ratio[j]-max_ratio)/(median_number-max_ratio);
        probable=probable->next_ap_grey;
    }
    
    //calculating the time index for reject category
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {
        /*
        // if time ratio >80th percentile, time index =1
        //else time index = x/80th percen 
        if(source==j+1)
        reject->time_index=1.0;
        //else if (time_ratio[i][j] >=perbig_number)
       // reject->time_index=1;
        //else
       // reject->time_index=time_ratio[i][j]/perbig_number;
        else if (time_ratio[j]==0 && persmall_number== 0)
        reject->time_index=1;
        else if(time_ratio[j] < persmall_number)
        reject->time_index=1.0;
        else if(persmall_number==max_ratio)
        reject->time_index=0;
        else
        reject->time_index=((time_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
        reject = reject->next_ap_grey;
        */
        
         //if time ratio <20th percentile ,time index =1
        //else time index = (x-max value)/(20th percen - max value)
        if(source==j+1)
        reject->time_index=1;
        //else if(time_ratio[i][j] <= persmall_number)
        //select->time_index=1.0;
        //else
        //select->time_index=((time_ratio[i][j]-max_ratio)/(persmall_number-max_ratio));
        //else if( time_ratio[j] == 0 && perbig_number==0)//all demand satisfied
        //select->time_index=0;
        else if (time_ratio[j] >perbig_number)
        reject->time_index=1;
        else if (perbig_number==0)
        reject->time_index=0;
        else
        reject->time_index=time_ratio[j]/perbig_number;
        reject=reject->next_ap_grey;   
    }    
    
	free(time_ratio);//NEU_VLA
	free(temp);//NEU_VLA
    
     //printing the time index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_time_index");
             getch();
             exit(1);
         }    
         fprintf(fprocess,"\n\n Remaining Operating Time = %.2f",max_time_slot-takeoff_time);
         /*
         fprintf(fprocess,"\n Time ratio (tons/min.) => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[j]);
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);  
             
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n Time Index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->time_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->time_index);
                 probable=probable->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->time_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
         
         /*
         fprintf(fprocess,"\n\n demand/Time ratio");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[i][j]);
             
         }
           
         fprintf(fprocess,"\n\n demand/Time ratio sorted");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",temp[i*AIRPORT_NO+j]);
             
         }  
         
         fprintf(fprocess,"\n\n %.2f percentile = % .2f",BIG->cost,perbig_number); 
         fprintf(fprocess,"\n %.2f percentile = % .2f",SMALL->cost,persmall_number);
         fprintf(fprocess,"\n median = % .2f",median_number);
         */
       /*
         fprintf(fprocess,"\n\n demand/time Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->time_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n demand/time Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->time_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n demand/time Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->time_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);  
         */
         /*
         fprocess=fopen("Output/TEMP.txt","a");
         fprintf(fprocess,"\n Total System demand = %.2f",system_demand);
         fprintf(fprocess,"\n\n time ratio");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[i][j]);
             
         }
           
         fprintf(fprocess,"\n\n time ratio sorted");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",temp[i*AIRPORT_NO+j]);
             
         }  
         
         fprintf(fprocess,"\n\n %.2f percentile = % .2f",BIG->cost,perbig_number); 
         fprintf(fprocess,"\n %.2f percentile = % .2f",SMALL->cost,persmall_number);
         fprintf(fprocess,"\n median = % .2f",median_number);
         fclose(fprocess);  
         */ 

//#include"get_airport_data.h"
//#include"new_airport_data.h"
//#include"check_landing_availability.h"
void Get_your_flight(struct Fleet *);

void Get_your_flight(struct Fleet *candidate)
{
    int desti_index;
    choices *prev_choice,*head_choice;
	//NEU
	struct choice_history *pTempChoice=NULL;
	//NEU
    /*
    //everytime when a new aircraft starts, forget all previous choices
    current_choice=head_choice;
    while(current_choice!=NULL)
    {
        prev_choice=current_choice;
        current_choice=current_choice->next;
        free(prev_choice);
    }  
     */ 
    head_choice=NULL;
    
    desti_index= Get_airport_data(candidate);//get_airport_data
    
    while(desti_index!=NOT_FOUND)
        {
           
            Check_landing_availability(candidate,desti_index);
            
            if(STATUS==YES)
            {
                break;               
            }
            else
            {
                //keeping a record of all the destinations which we have already tested
                prev_choice=(choices *)malloc(sizeof(choices));
                prev_choice->index=desti_index;
                prev_choice->next=NULL;
                if(head_choice==NULL)
                {
                    head_choice=prev_choice;
                   
                }    
                else
                {
                    prev_choice->next=head_choice;
                    head_choice=prev_choice;
                }    
               //head_choice points to the last choice
               desti_index= New_airport_data(candidate,head_choice);
            }    //end of else
                    
       }//end of while(desti_index!=NOT_FOUND)
        
    if(desti_index==NOT_FOUND)
        {
            if(user_choice==YES)
            {
            FILE *fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
               {
                
                printf("\n Cann't open the file Output/PROCESS in get_your_flight");
                getch();
                exit(1);
                }
            fprintf(fprocess,"\n------------ No destination found ------------\n");
            fclose(fprocess);
            }
        }    
	
	//free memory in head_choice
	while(head_choice!=NULL)
	{
		pTempChoice=head_choice;
		head_choice=head_choice->next;
		free(pTempChoice);
	}


}  

/*Algorithm----------------
possible destination =get next destination as per demand			---(1.1)
        do
			{
            IF(any possible destination)
            {
                check airport availability		----(1.2)
          		IF ( *Status = yes)
        		Break;
        		ELSE
                possible destination =check another airport as destination             ----(1.3)
        	}
        	}while (there is any possibility)
        	
        IF(*Status =NO)
        	{
        		possible destination = the maintenance station to go  -----------(1.4)
        		do
        		{
                		IF(any possible destination)
                        {
                                check airport availability		----(1.2)
                        		IF ( *Status = yes)
                        		Break;
                        		ELSE
                        		possible destination =check another maintenance airport as destination			-----(1.5)
                		}
        	     }while (there is any possibility)
        
        	}
        
  }
*/
  
//#include"get_base_data.h"
//#include"new_base_data.h"
void Go_to_base(struct Fleet *);          
     
void Go_to_base(struct Fleet *candidate)
     {    
          int base_index;
          choices *prev_choice,*head_base_choice=NULL;
		  //NEU
		  struct choice_history *pTempChoice=NULL;
		  //NEU
          
          SENDING_TO_BASE=YES;
          base_index= Get_base_data(candidate);
          
          while(base_index!=NOT_FOUND)
            {
                
                Check_landing_availability(candidate,base_index);
                
                if(STATUS==YES)
                {
                    break;
                }
                else
                {
                    
                     //keeping a record of all the destinations which we have already tested
                    prev_choice=(choices *)malloc(sizeof(choices));
                    prev_choice->index=base_index;
                    prev_choice->next=NULL;
                    if(head_base_choice==NULL)
                    {
                        head_base_choice=prev_choice;
                       // last=prev_choice;
                    }    
                    else
                    {
                        prev_choice->next=head_base_choice;
                        head_base_choice=prev_choice;
                    }    
                    
                    base_index= New_base_data(candidate,head_base_choice);
                } //end of else   
                       
            }//end of while(base_index!=NOT_FOUND)
            
           
           if(base_index==NOT_FOUND)
           {
                if(user_choice==YES)
                {
                    FILE *fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't open the file Output/PROCESS in go_to_base");
                        getch();
                        exit(1);
                        }
                    fprintf(fprocess,"\n------------ No base station is possible ------------\n");
                    fclose(fprocess);
                }
            }    
		//free memory in head_base_choice
		while(head_base_choice!=NULL)
		{
			pTempChoice=head_base_choice;
			head_base_choice=head_base_choice->next;
			free(pTempChoice);
		}         
              
        }//end of function    
        
        
        
        /*
        
        //Check_airport_availability(candidate,base_index);
                //reading average delay on the destination airport
                    
                    fp = fopen("Input/AIRPORT NAMES.txt","r");
                   if(fp==NULL)
                    {
                    #ifdef PROCESS                       
                    fprintf(fprocess,"\n Cann't find the file Input/AIRPORT NAMES in in go_to_base");
                    fclose(fprocess);
                    #endif
                    printf("\n Cann't find the file Input/AIRPORT NAMES in in go_to_base");
                    getch();
                    exit(1);
                    }  
                    for(i=1;i<=base_index;i++)  
                    fscanf(fp,"%s %f",temp,&average_delay);
                    fclose(fp);
                    
                    #ifdef PROCESS
                    fprintf(fprocess,"\n average delay on the base airport %d = %f",base_index,average_delay);
                    #endif
                    #ifdef PROCESS
                   fclose(fprocess);
                   #endif
                    
                    block_time=Get_route_BT(candidate->ap_index,base_index);//(source,destination)

                    #ifdef PROCESS
                    fprocess=fopen("Output/PROCESS.doc","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't find the file Output/PROCESS in go_to_base");
                        getch();
                        exit(1);
                        }
                    fclose(fprocess);
                    #endif
                    
                    new_take_off = candidate->take_off +(block_time+ average_delay +TURN_AROUND_TIME);
                    time_diff =0;
                    Check_end_time(candidate,new_take_off,base_index,time_diff);
                    
                    
                    */
//#include"get_fly_time.h"
////#include"get_extra_data.h"
//#include"get_cost_index.h"
//#include"get_demand_index.h"
//#include"get_time_index.h"
//#include"get_route_priority_index.h"
////#include"get_network_design_index.h"
//#include"get_grey_index.h"
//#include"get_grey_category.h"

void Grey_analysis (struct Fleet *);

void Grey_analysis(struct Fleet *candidate)
{
    int i;
    //NEU_VLA old: float fly_time[AIRPORT_NO];//,extra_fly[AIRPORT_NO],extra_cost[AIRPORT_NO],extra_out_demand[AIRPORT_NO];
	//NEU_VLA start
	float *fly_time=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (fly_time == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end	
	/*
    for(i=0;i<AIRPORT_NO;i++)
    {
        extra_fly[i]=0;
        extra_cost[i]=0;
        extra_out_demand[i]=0;
    }  
    */  
    //find fly time from here to all other airports
    Get_fly_time(candidate->ap_index,candidate->takeoff_time,fly_time,AIRPORT_NO,candidate); 
    //Get_extra datas, to find outgoing demand, sum of time and sum of cost
    //if(SENDING_TO_BASE==NO)
    //Get_extra_data(candidate,fly_time,AIRPORT_NO,extra_fly,extra_cost,extra_out_demand);
    //Calculate demand concentration for each OD pair
    Get_demand_index(candidate->ap_index,candidate->ac_type);//NEU_MAC (added actype)
    //Get_demand_index(candidate->ap_index,extra_out_demand);//
    //Calculate time index for each OD pair, time taken to deliver the cargo
    Get_time_index(candidate->ap_index,fly_time,AIRPORT_NO,candidate->takeoff_time);
    //Get_time_index(candidate->ap_index,fly_time,AIRPORT_NO,candidate->takeoff_time,extra_fly_time);
    //Calculate cost index for each OD pair, operating cost to deliver the cargo
    Get_cost_index(candidate->ap_index,candidate->ac_type);//NEU_MAC (added actype)
    //Get_cost_index(candidate->ap_index,extra_cost);
    //Calculate route priority index for each OD pair
    Get_route_priority_index(candidate->ap_index);
    //calculate design index for each airport
    //Get_network_design_index(candidate->ap_index);
    // weighted sum of all the indices in all the three categories select, probable and reject
    Get_grey_index();
    //OD pair belongs to the category which ever is having highest value
    Get_grey_category();

	free(fly_time);//NEU_VLA
      
}
    


//#include"get_base_cost_index.h"
//#include"get_base_demand_index.h"
//#include"get_base_time_index.h"
////#include"get_base_network_design_index.h"
//#include"get_base_route_priority_index.h"
////#include"get_base_service_quality_index.h"


void Grey_base_analysis (struct Fleet *);

void Grey_base_analysis(struct Fleet *candidate)
{
    int i;
    //NEU_VLA old: float fly_time[AIRPORT_NO];
	//NEU_VLA start
	float *fly_time=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (fly_time == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end	
    //find fly time from here to all base airports
    Get_fly_time(candidate->ap_index,candidate->takeoff_time,fly_time,AIRPORT_NO,candidate); 
    //Calculate demand concentration for each OD pair
    Get_base_demand_index(candidate->ap_index);//
    //Calculate time index for each OD pair, time taken to deliver the cargo
    Get_base_time_index(candidate->ap_index,fly_time,AIRPORT_NO,candidate->takeoff_time);
    //Calculate cost index for each OD pair, operating cost to deliver the cargo
    Get_base_cost_index(candidate->ap_index,candidate->ac_type);//NEU_MAC
    //Calculate route priority index for each OD pair
    Get_base_route_priority_index(candidate->ap_index);
    //calculate network_design index for each base airport
    //Get_base_network_design_index(candidate->ap_index);
    // weighted sum of all the indices in all the three categories select, probable and reject
    Get_grey_index();
    //OD pair belongs to the category which ever is having highest value
    Get_grey_category();
 
	free(fly_time);//NEU_VLA
}
    


//int Select_if_same_index(int,int,int,float,int); 

int Select_if_same_index(int desti1,int desti2,int source,float takeoff_time, int actype,struct Fleet *candidate)//NEU_MAC
{
    int i,j;
    int selected_index;
    float random_value;
    float desti1_demand,desti2_demand,desti1_cost,desti2_cost,desti1_flytime,desti2_flytime,desti1_qos,desti2_qos;
	float fBH1=0,fBH2=0; //NEU_UT
    //NEU_VLA old: float fly_time[AIRPORT_NO];
	//NEU_VLA start
	float *fly_time=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (fly_time == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end	

    time_t current_time;
    struct tm * timeinfo;
    struct Data *data;
    FILE *fprocess;
    
     if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in new_airport_data");
            getch();
            exit(1);
        }    
    }        


    //for desti 1
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    
    for(i=1;i<desti1;i++)
    data=data->next_data;
    
    desti1_demand=data->demand;   
    //NEU_MAC old: desti1_cost=data->TOC;		  
	desti1_cost=Get_cost(actype,source,desti1);//NEU_MAC
    if(data->initial_freq_req!=0)
    desti1_qos=(1.0*data->freq_offered)/data->initial_freq_req;
    else
    desti1_qos=1; 
 
  //for desti 2
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    
    for(i=1;i<desti2;i++)
    data=data->next_data;
    
    desti2_demand=data->demand;
    //NEU_MAC old: desti2_cost=data->TOC;
	desti2_cost=Get_cost(actype,source,desti2);//NEU_MAC
    if(data->initial_freq_req!=0)
    desti2_qos=(1.0*data->freq_offered)/data->initial_freq_req;
    else
    desti2_qos=1; 
    //flytime
    Get_fly_time(source,takeoff_time,fly_time,AIRPORT_NO,candidate);
    desti1_flytime=fly_time[desti1-1];
    desti2_flytime=fly_time[desti2-1];

    
 switch(objective_pointer)
        {
        case 1:
        
                // min- the reverse of demand i.e. max the demand
                if(desti1_demand > desti2_demand)
                selected_index=desti1;
                else if (desti1_demand == desti2_demand)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;    
                
                break;
        case 2:
                // min- the total operating cost
                 if(desti1_cost < desti2_cost)
                selected_index=desti1;
                else if (desti1_cost == desti2_cost)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 3:
                // min- the reverse of time i.e. max time i.e. max ac utilization
                 if(desti1_flytime > desti2_flytime)
                selected_index=desti1;
                else if (desti1_flytime == desti2_flytime)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 4:
                 // min. - the variance in QoS i.e. improving the QoS
                 if(desti1_qos < desti2_qos)
                selected_index=desti1;
                else if (desti1_qos == desti2_qos)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 5:
                 // min. - cost per demand
                 if((desti1_cost/desti1_demand) < (desti2_cost/desti2_demand))
                selected_index=desti1;
                else if ((desti1_cost/desti1_demand) == (desti2_cost/desti2_demand))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 6:
                  // min. - time per demand
                  if((desti1_flytime/desti1_demand) < (desti2_flytime/desti2_demand))
                selected_index=desti1;
                else if ((desti1_flytime/desti1_demand) == (desti2_flytime/desti2_demand))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
		//NEU_UT
		case 7:
		// select the higher utilization
                //blocktime (for utilization function)
				fBH1=Get_route_BT(source,desti1);
				fBH2=Get_route_BT(source,desti2);
				if((fBH1/desti1_flytime) > (fBH2/desti2_flytime))
                selected_index=desti1;
                else if ((fBH1/desti1_flytime) == (fBH2/desti2_flytime))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
		break;
		//NEU_UT
        case 8:
                // min- the demand 
                 if(desti1_demand < desti2_demand)
                selected_index=desti1;
                else if (desti1_demand == desti2_demand)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 9:
                // min- the reverse of cost i.e. max cost
                if(desti1_cost > desti2_cost)
                selected_index=desti1;
                else if (desti1_cost == desti2_cost)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;

        break;
        case 10:
                // min- the total time aircrafts operated
                
                 if(desti1_flytime < desti2_flytime)
                selected_index=desti1;
                else if (desti1_flytime == desti2_flytime)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;

        break;
        case 11:
                // min- the reverse of variance in QoS i.e. max the variance i.e. min QoS
                if(desti1_qos > desti2_qos)
                selected_index=desti1;
                else if (desti1_qos == desti2_qos)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 12:
                // min. - demand per cost
                
                 if((desti1_demand/desti1_cost) < (desti2_demand/desti2_cost))
                selected_index=desti1;
                else if ((desti1_demand/desti1_cost) == (desti2_demand/desti2_cost))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 13:
                  // min. - demand per time 
                  if((desti1_demand/desti1_flytime) < (desti2_demand/desti2_flytime))
                selected_index=desti1;
                else if ((desti1_demand/desti1_flytime) == (desti2_demand/desti2_flytime))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;

        break;
		//NEU_UT
		case 14:
			// select the lower utilization
                //blocktime (for utilization function)
				fBH1=Get_route_BT(source,desti1);
				fBH2=Get_route_BT(source,desti2);
				if((fBH1/desti1_flytime) < (fBH2/desti2_flytime))
                selected_index=desti1;
                else if ((fBH1/desti1_flytime) == (fBH2/desti2_flytime))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
		break;
		//NEU_UT
        default:
        printf("\n Error : Problem in objective function option");
        getch();
        exit(0);
                                        
        }   
        if(user_choice==YES)
        fclose(fprocess); 
		free(fly_time);//NEU_VLA
        return(selected_index);
}//end of function       

//Program files
//#include"data_structure.h"
//#include"read_input.h"
//#include"get_route_BT.h"
//#include"get_capacity.h"//NEU_MAC
//#include"get_cost.h"//NEU_MAC
//#include"initial_freq_req.h"
//#include"initialize_grey_index.h"
//#include"make_end_time.h"
//#include"sorted_fleet.h"
//#include"grey_analysis.h"
//#include"get_your_flight.h"
//#include"grey_base_analysis.h"
//#include"go_to_base.h"
////#include"adjust_remaining_demand.h"
//#include"objective_calc.h"
//#include"write_output.h"
//NEU
//#include"no_holding.h"
//#include"free_list.h"
//NEU
void Initial_freq_req();
void Initial_freq_req()
  {
    int i,j,k;//NEU_MAC (+k)
    struct Data *data;
	float temp_capacity,save_capacity; //NEU_MAC
     
    data=head_data;
    initial_total_demand=0;//global variable
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        //NEU_MAC old: if(data->capacity==0)
		if(i==j)
        data->initial_freq_req=0;
        else
		{	//NEU_MAC Start
			save_capacity=0;
			for(k=0;k<ACTYPE_NO;k++)
			{
				temp_capacity=Get_capacity(k+1,i+1,j+1);
				if (save_capacity==0 || (save_capacity < temp_capacity && temp_capacity!=0 ))
					save_capacity=temp_capacity;
			}
			data->initial_freq_req=(int)ceil(data->demand/save_capacity);
		}	//NEU_MAC End
		//NEU_MAC old: data->initial_freq_req=(int)ceil(data->demand/data->capacity);
        data->freq_offered=0;
        initial_total_demand+=data->demand;
        data=data->next_data;
    }    
    //printing the Inital freq requ detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in initial_freq_req");
            getch();
            exit(1);
        }
        data=head_data;
        fprintf(fprocess,"\n\n Initial frequency required to satisfy all the demand");
        for(i=0;i<AIRPORT_NO;i++)
        {
            fprintf(fprocess,"\n");
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess,"%d\t",data->initial_freq_req);
                data=data->next_data;
            }
        }   
        fclose(fprocess); 
    }//end of if(user_choice==YES)   
}//end of function
void Initialize_grey_index (void);
void Initialize_grey_index()
{
    int i;

    struct Grey_indices *select,*last_select,*probable,*last_probable,*reject,*last_reject;
   

    //Initializing the indices for select category
    for(i=0;i<AIRPORT_NO;i++)
    {
       select=(struct Grey_indices *)malloc(sizeof(struct Grey_indices));
       select->cost_index=0;
       select->demand_index=0;
       select->grey_index=0;
       select->route_priority_index=0;
       select->time_index=0;
       //select->network_design_index=0;
       select->next_ap_grey=NULL;
        
        if(head_select==NULL)
        {
            head_select=select;
            last_select=select;
        }
        
        else
        {
            last_select->next_ap_grey=select;
            last_select=last_select->next_ap_grey;
        } 
               
    }
    
    //Initializing the indices for probable category    
    for(i=0;i<AIRPORT_NO;i++)
    {
        probable=(struct Grey_indices *)malloc(sizeof(struct Grey_indices));
        probable->cost_index=0;
        probable->demand_index=0;
        probable->grey_index=0;
        probable->route_priority_index=0;
        //probable->network_design_index=0;
        probable->time_index=0;

        probable->next_ap_grey=NULL;
        if(head_probable==NULL)
        {
            head_probable=probable;
            last_probable=probable;
        }
        else
        {
            last_probable->next_ap_grey=probable;
            last_probable=last_probable->next_ap_grey;
        }        
    }
    
    //Initializing the indices for reject category
    for(i=0;i<AIRPORT_NO;i++)
    {
        reject=(struct Grey_indices *)malloc(sizeof(struct Grey_indices)); 
        reject->cost_index=0;
        reject->demand_index=0;
        reject->grey_index=0;
        reject->route_priority_index=0;
        reject->time_index=0;
        //reject->network_design_index=0;

        reject->next_ap_grey=NULL;
        if(head_reject==NULL)
        {
            head_reject=reject;
            last_reject=reject;
        }
        else
        {
            last_reject->next_ap_grey=reject;
            last_reject=last_reject->next_ap_grey;
        }        
    }
    
  
 
  

}    //end of function

//struct Fleet * Insert_sort(struct Fleet *,struct Fleet *);
void Insert_sort(struct Fleet *);

//struct Fleet * Insert_sort(struct Fleet *head_aircraft_list,struct Fleet *item)
void Insert_sort(struct Fleet *item)
{
    struct Fleet *prev=NULL,*list;
    
    list =head_aircraft_list; // list is now pointing towars the start of the list
    if(head_aircraft_list==NULL) //means there is no item in the list
        head_aircraft_list=item;
     
    else
    {
      if(list->takeoff_time > item->takeoff_time)//insert at start
        {
              item->next_aircraft=list;
              head_aircraft_list=item;
        }    
    
      else
         {
               while(list->takeoff_time <= item->takeoff_time)
                     {
                               prev=list;
                               if(list->next_aircraft==NULL) // insert at  last node
                                {
                                         list=NULL;
                                         break;
                                }    
          
                               list=list->next_aircraft;
                       }    
      
            prev->next_aircraft=item;
            item->next_aircraft=list;
    
    
       }
      
 
     }   
     //return(head_aircraft_list);
    
}   //end of function 

// for end time of each aircraft
void Make_end_time( );  
void Make_end_time() 
{ 
    int i;
    struct Fleet *fleet;
    fleet=head_fleet;
    for(i=0;i<fleet_size;i++)
    {
        if(fleet->life < max_time_slot)
        fleet->end_time=(fleet->life)-(fleet->takeoff_time);
        else
        fleet->end_time=(max_time_slot)-(fleet->takeoff_time);
        fleet=fleet->next_aircraft;
    } 
    //printing the end time detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in make_end_time");
            getch();
            exit(1);
        }
        fleet=head_fleet;
        fprintf(fprocess,"\n\n Aircraft\tEnd-time");
        for(i=0;i<fleet_size;i++)
        {
            fprintf(fprocess,"\n %d\t\t%.2f",fleet->ac_index,fleet->end_time);
            fleet=fleet->next_aircraft;
        }   
        fclose(fprocess); 
    }//end of if(user_choice==YES)           
}//end of function    
float Median_calc(float *,int);

float Median_calc(float *array ,int size)
{   int i;
    if((size%2)==0)//even number
    {
        i=size/2;
        return((array[i-1]+array[i])/2);
    }    
    else//odd number
    {
        i=(size+1)/2;
        return(array[i-1]);
    }    
}    
int New_airport_data(struct Fleet *, choices *);

int New_airport_data(struct Fleet *candidate, choices * prev_choices)
{
    
    int i,j;
    int HIGHEST_PRIORITY_FLAG;//to check if we gor any base station with priority=1
    int flag;
    int best_index,selected_index;
    char best_cat;
    float best_grey_index,best_demand;
	float temp_capacity; //NEU_MAC
    int ac_req;
	int *ap_with_belf=NULL;//NEU_SH
    //NEU_VLA old: float ap_demand[AIRPORT_NO],ap_capacity[AIRPORT_NO],line_sum[AIRPORT_NO],BELF_each[AIRPORT_NO]; //NEU_SH
	//NEU_SH old:float ap_demand[AIRPORT_NO],ap_capacity[AIRPORT_NO],line_sum[AIRPORT_NO],capacity_each[AIRPORT_NO],BELF_each[AIRPORT_NO];
    //NEU_VLA start
	float *ap_demand=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *ap_capacity=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *line_sum=(float *)malloc(AIRPORT_NO*sizeof(float));
	//float *BELF_each=(float *)malloc(AIRPORT_NO*sizeof(float)); //CHMANAV10 SAME CHANGES AS IN GET_AIRPORT_DATA FUNCTION TO MAKE BELF ROUTE DEPENDENT
float** BELF_each;

int numRows, numCols;
/* Used as indexes as in matrix[x][y]; */
int x, y; 
/*
 * Get values into numRows and numCols somehow.
 */


/* Allocate pointer memory for the first dimension of a matrix[][]; */
BELF_each = (float **) malloc(AIRPORT_NO * sizeof(float *));
/* Allocate integer memory for the second dimension of a matrix[][]; */
for(x = 0; x < AIRPORT_NO; x++)
{
    BELF_each[x] = (float *) malloc(AIRPORT_NO * sizeof(float));
   }
if (ap_demand == NULL || ap_capacity == NULL || line_sum == NULL || BELF_each == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end	
	float BELF;
        float random_value;
    time_t current_time;
    struct tm * timeinfo;
    struct Data *data,*candidate_data;
    struct Airports *airport,*candidate_airport;
    struct Fleet *temp_head;
    struct Final_grey *grey;
    choices *check_choices;//,*prev_choice;
    FILE *fprocess;
    //FILE *fp;
    //fp=fopen(output_filename,"a");
    //fprintf(fp,"\n new search for aircraft %d, source %d",candidate->ac_index,candidate->ap_index);
    
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in new_airport_data");
            getch();
            exit(1);
        }    
    }        

    //doing grey analysis of all the airports
    //Grey_analysis();
    //going upto the airport where our candidate aircraft is available
    data=head_data;
    for(i=1;i<candidate->ap_index;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    candidate_data=data;
    //reading the demand and capacity from candidate airport to all other airports
    for(j=0;j<AIRPORT_NO;j++)
    {
        ap_demand[j]=candidate_data->demand;
        ap_capacity[j]=Get_capacity(candidate->ac_type,candidate->ap_index,(j+1));//NEU_MAC
		//NEU_MAC old: ap_capacity[j]=candidate_data->capacity;
        candidate_data=candidate_data->next_data;
    }    

    //reading the BELF for takeoff from the candidate airport
    airport=head_airport;
    for(i=1;i<candidate->ap_index;i++)
    airport=airport->next_airport;
//CHMANAV11 REFER TO CHANGE IN GET_AIRPORT DATA FOR BELF FEATURE
   // BELF=airport->BELF;


    //starting the search
    best_index=-1;
    /*
    if(best_index==-1)  
    {
        printf("\n looking if any other airport has demand>BELF");
        if(user_choice ==YES)
        fprintf(fprocess,"\n\n looking if any other airport has demand>BELF");
        //going upto the our candidate airport
        grey=head_final_grey;
        for(i=1;i<candidate->ap_index;i++)
        for(j=0;j<AIRPORT_NO;j++)
        grey=grey->next_ap_index;
        
        for(i=0;i<AIRPORT_NO;i++)
        {
            //first look for which airports demand>BELF
            //then among those airports which one is best as per grey indices
            if((ap_demand[i] >= (ap_capacity[i]*BELF/100)) && ap_capacity[i]!=0 )
            {
                //look only for those airports which have not been checked till now
                   check_choices=prev_choices;
                   flag=1;
                   do
                   {
                       if(((check_choices->index)-1)==i)
                       {
                           flag=0;
                           printf("\n Airport number %d has been checked previously",i+1); 
                           if(user_choice==YES)
                            fprintf(fprocess,"\n Airport number %d has been checked previously",i+1);
                          
                           break;
                       }
                       check_choices=check_choices->next;
                   }while(check_choices!=NULL);    
                  
                   if(flag==1)
                   {
                    printf("\n looking for airport number = %d as an option to go",i+1);
                    if(user_choice ==YES)
                               fprintf(fprocess,"\n looking for airport number = %d as an option to go",i+1);
                    if(best_index==-1)//means this is the first node
                    {
                         // if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
                         // {
                              best_index=i;
                              best_cat=grey->grey_char;
                              best_grey_index=grey->grey_index;
                        //  }    
                    }    
                    else
                    {
                       if(best_cat=='s'&& grey->grey_char=='s')
                       {
                           if(best_grey_index < grey->grey_index)
                           {
                               printf("\n changing the best selection becouse we got better option in select category");
                               if(user_choice ==YES)
                               fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
                               best_index=i;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                           }    
                       }    
                       else if(best_cat!='s'&& grey->grey_char=='s')
                       {
                           printf("\n changing the best selection becouse of category");
                           if(user_choice ==YES)
                                fprintf(fprocess,"\n changing the best selection becouse of category");
                           
                           best_index=i;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                       }    
                       else if(best_cat=='p'&& grey->grey_char =='p')
                       {
                           if(best_grey_index < grey->grey_index)
                           {
                               printf("\n changing the best selection becouse we got better option in probable category");
                               if(user_choice ==YES)
                                    fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
                               best_index=i;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                           }    
                       } 
                       
                       
                                // next two sections - allowing reject category also as an option to go
                           //to remoce this, make next two section comment nad at start umcomment if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
                             
                            else if(best_cat=='r'&& grey->grey_char!='r')
                           {
                               printf("\n changing the best selection becouse of category");
                               if(user_choice ==YES)
                                    fprintf(fprocess,"\n changing the best selection becouse of category");
                               
                               best_index=i;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                           }    
                           else if(best_cat=='r'&& grey->grey_char =='r')
                           {
                               if(best_grey_index > grey->grey_index)
                               {
                                   printf("\n changing the best selection becouse we got better option in reject category");
                                   if(user_choice ==YES)
                                        fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category");
                                   best_index=i;
                                   best_cat=grey->grey_char;
                                   best_grey_index=grey->grey_index;
                               }    
                           } 
                     
                      }//end of else 
                }//end of if(flag==1)    
            }//end of if((ap_demand[i] >= (ap_capacity[i]*BELF/100)) && ap_capacity[i]!=0 )
         grey=grey->next_ap_index;             
        }//end of for(i=0;i<AIRPORT_NO;i++)        
    }//end of  if(best_index==-1)          
    */
    if(best_index==-1)  
    {
        //printf("\n looking if any other airport has outgoing demand>BELF");
        //if(user_choice ==YES)
       // fprintf(fprocess,"\n\n looking if any other airport has outgoing demand>BELF");
        
        //printf("\n looking if for any airport demand >BELF or outgoing demand at any airport >BELF");
        if(user_choice ==YES)
			fprintf(fprocess,"\n\n Looking if for any airport demand to min 50percent of routes >BELF or outgoing demand to dest. airport >BELF"); //NEU_SH
			//NEU_SH old: fprintf(fprocess,"\n\n looking if for any airport demand >BELF or outgoing demand at any airport >BELF");
        
        //CHMANAV12 BELF at each route refer to get_airport_data 
        //airport=head_airport;
data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
		for(j=0;j<AIRPORT_NO;j++)
        {
            BELF_each[i][j]=data->BELF;
            data=data->next_data;
        }

        /* NEU_SH deactivation start (line_sum calculation not needed anymore)
		// Initializing line_sum  to zero
        for(i=0;i<AIRPORT_NO;i++)
        {
        line_sum[i]=0;         
        }
        //sum of all outgoing demands at each airport =line sum
        data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            line_sum[i]+=data->demand;
            data=data->next_data;
        }

		//at each airport look how many aircraft already available there and dose it require any more Aircraft                          
        for(i=0;i<AIRPORT_NO;i++)
        {
            //*****temp_head=candidate;//list starts from here, not from head_fleet
            temp_head=candidate;
           // for(j=0;j<fleet_size;j++)
            while(temp_head!=NULL)
            {
                if((temp_head->ap_index==i+1)&&(temp_head->ac_index!=candidate->ac_index))
                {
                    if(user_choice==YES)
                    fprintf(fprocess,"\n Aircraft no. %d is available at airport %d",temp_head->ac_index,i+1);
                    line_sum[i]-=capacity_each[i];
                }   
               
                temp_head=temp_head->next_aircraft;
            }
        } //NEU_SH deactivation end*/
        
		/* NEU_MAC deactivation start (capacity_each[] not needed anymore)
        //takeoff capacity of aircraft at each airport
        data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            capacity_each[i]=data->capacity;//reading the first entry in capacity line
            for(j=0;j<AIRPORT_NO-1;j++)//going to last element in that line
            data=data->next_data;
            if(capacity_each[i]==0)//if first entry is zero, reading last entry
            {
                capacity_each[i]=data->capacity;
            }
            data=data->next_data;//going to next capacity line    
        } NEU_MAC deactivation end */    

        //printing the outgoing demands,takeoff capacity and BELF from each airport
        if(user_choice ==YES)
        {
            //NEU_MAC old: fprintf(fprocess,"\n\t Outgoing demands, Takeoff capacity and BELF ");
			fprintf(fprocess,"\n\t Outgoing Demand, Outgoing Capacity, BELF ");
            for(i=0;i<AIRPORT_NO;i++)//CHMANAV13 PRINTING BELF TO BE ROUT DEPENDENT
				for(j=0;j<AIRPORT_NO;j++)
				fprintf(fprocess,"\n from airport %d to airport %d = %.2f, %.2f",i+1,j+1,ap_demand[i],ap_capacity[i]);
			//NEU_MAC old: fprintf(fprocess,"\n from airport %d = %.2f, %.2f and %.2f",i+1,line_sum[i],capacity_each[i],BELF_each[i]);
        }

		//NEU_SH
		//counting the no. of directions with demand>BELF from each airport
		ap_with_belf = (int *)malloc(AIRPORT_NO*sizeof(int));
		data=head_data;
		for(i=0;i<AIRPORT_NO;i++)
		ap_with_belf[i]=0;
		i=0;
		while(data!=NULL)
		{//CHMANAV14 TREATING BELF_each AS 2D pointer EVERYWHERE
			j=0;
			BELF=data->BELF;
			while(data!=NULL && j<AIRPORT_NO)
			{
				temp_capacity=Get_capacity(candidate->ac_type,i+1,j+1); //NEU_MAC
				//NEU_MAC old: if (data->demand >= (data->capacity*BELF_each[i]) && data->capacity != 0)
				if (data->demand >= (temp_capacity*BELF_each[i][j]) && temp_capacity != 0) //NEU_MAC
				ap_with_belf[i]++;
				data=data->next_data;
				j++;
			}
			//printf("%d\t",ap_with_belf[i]);
			if(user_choice ==YES)
				fprintf(fprocess,"\n Airport %d: %d",i+1,ap_with_belf[i]);//NEU_SH NEU_MAC
			i++;
		}
		//printf("\n\n");
		//NEU_SH

		//NEU_MAC: No modifications needed beyond this point

        //going upto the our candidate airport
        grey=head_final_grey;
         
        //going upto the airport where our candidate aircraft is available
        data=head_data;
        for(i=1;i<candidate->ap_index;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;   
        
        HIGHEST_PRIORITY_FLAG=NO;
               
        for(i=0;i<AIRPORT_NO;i++)
        {
           if(data->priority!=-1)//don't look the destination if priority=-1
            {
             if((data->priority==1)||(HIGHEST_PRIORITY_FLAG==NO))
				 {  
					 //NEU_SH old:if((  (line_sum[i] >= (capacity_each[i]*BELF_each[i]/100.0)) && (candidate->ap_index!=i+1) ) || ((ap_demand[i] >= (ap_capacity[i]*BELF/100)) && ap_capacity[i]!=0 ))
					 //if ((min. 50% of tertiary AP from Dest. AP have Demand>BELF or demand from candidate AP to dest AP > BELF
					 //min 1:if(((ap_with_belf[i] >= 1 ) && (candidate->ap_index!=i+1)) || ((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
					 if((ap_with_belf[i] >= (AIRPORT_NO/2) && (candidate->ap_index!=i+1)) || ((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
					 //without tert.AP:if(((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
					 {
						flag=1;
						/*
						//if first tiem coming in this loop i.e. outgoing demand>BELF then delet all the previous record
						if(ASK_FLAG==NO)//menas i m coming first time in this block, after demand>capacity block in get_airport_data
						{
							//delete all the record of previous choices
							check_choices=prev_choices;
							while(check_choices!=NULL)
							{
								prev_choice=check_choices;
								check_choices=check_choices->next;
								free(prev_choice);
							}
							//making head pointer of choices to NULL again    
							head_choice=NULL;
							ASK_FLAG=YES;//do it when first time coming in this block after that keep the record
						} 
						else
						{ 
							*/              
		                
						//look only for those airports which have not been checked till now
						   check_choices=prev_choices;
		                   
						   do
						   {
							   if(((check_choices->index)-1)==i)
							   {
								   flag=0;
								   //printf("\n Airport number %d has been checked previously",i+1); 
								   if(user_choice==YES)
									fprintf(fprocess,"\n Airport number %d has been checked previously",i+1);
		                            
								   break;
							   }
							   check_choices=check_choices->next;
						   }while(check_choices!=NULL);    
					  // }    
		               
		                    
					  if(flag==1)
						   {
						   // printf("\n looking for airport number = %d as an option to go",i+1);
							if(user_choice ==YES)
									   fprintf(fprocess,"\n looking for airport number = %d as an option to go",i+1);
		                    
							//fprintf(fp,"\n looking for airport number = %d as an option to go",i+1);
		                    
							   if(data->priority==1 && HIGHEST_PRIORITY_FLAG==NO)//first highest priority route, 
							   {
								HIGHEST_PRIORITY_FLAG=YES;
								best_index=-1; 
								if(user_choice ==YES)
									   fprintf(fprocess,"\n Airport number %d has the highest priority (1) to go",i+1);                 
		                        
								}
		                    
		                    
		                    
		                    
							if(best_index==-1)//means this is the first node
							{
								  //if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
								  //{
									  best_index=i;
									  best_cat=grey->grey_char;
									  best_grey_index=grey->grey_index;
									  best_demand=data->demand;
								 // }    
							}    
							else
							{
							   if(best_cat=='s'&& grey->grey_char=='s')
							   {
								   if(best_grey_index < grey->grey_index)
								   {
									  // printf("\n changing the best selection becouse we got better option in select category");
									   if(user_choice ==YES)
									   fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
									   best_index=i;
									   best_cat=grey->grey_char;
									   best_grey_index=grey->grey_index;
									   best_demand=data->demand;
								   } 
								   else if((best_grey_index == grey->grey_index))
								   {
										  selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
									   if(selected_index==(i+1))
									   {
											  best_index=i;
											  best_cat=grey->grey_char;
											  best_grey_index=grey->grey_index;
											  best_demand=data->demand;
		                                 
									   }
										  /*
										  //calculating random variable
										  current_time=time(NULL);
										  timeinfo=localtime(&current_time);
										  srand(time(NULL)*rand()*1000);
										   random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
										  if(user_choice ==YES)
										   fprintf(fprocess,"\n random value = %.2f",random_value);
										  if(random_value>0.5)
										  {
											  // printf("\n changing the best selection because we got better option in select category(with more demand)");
											   if(user_choice ==YES)
											   fprintf(fprocess,"\n changing the best selection becouse we got better option in select category (randomly)");
											   best_index=i;
											   best_cat=grey->grey_char;
											   best_grey_index=grey->grey_index;
											   best_demand=data->demand;
										  }    
										  */
								   }
							   }    
							   else if(best_cat!='s'&& grey->grey_char=='s')
							   {
								  // printf("\n changing the best selection becouse of category");
								   if(user_choice ==YES)
										fprintf(fprocess,"\n changing the best selection becouse of category");
		                           
								   best_index=i;
								   best_cat=grey->grey_char;
								   best_grey_index=grey->grey_index;
								   best_demand=data->demand;
							   }    
							   else if(best_cat=='p'&& grey->grey_char =='p')
							   {
								   if(best_grey_index < grey->grey_index)
								   {
									  // printf("\n changing the best selection becouse we got better option in probable category");
									   if(user_choice ==YES)
											fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
									   best_index=i;
									   best_cat=grey->grey_char;
									   best_grey_index=grey->grey_index;
									   best_demand=data->demand;
								   }
								   else if((best_grey_index == grey->grey_index))
								   {
									   selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
									   if(selected_index==(i+1))
									   {
											  best_index=i;
											  best_cat=grey->grey_char;
											  best_grey_index=grey->grey_index;
											  best_demand=data->demand;
		                                 
									   }
									   /*
										  //calculating random variable
										  current_time=time(NULL);
										  timeinfo=localtime(&current_time);
										  srand(time(NULL)*rand()*1000);
										  random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
										  if(user_choice ==YES)
										   fprintf(fprocess,"\n random value = %.2f",random_value);
										  if(random_value>0.5)
										  {
											  // printf("\n changing the best selection because we got better option in select category(with more demand)");
											   if(user_choice ==YES)
											   fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category (randomly)");
											   best_index=i;
											   best_cat=grey->grey_char;
											   best_grey_index=grey->grey_index;
											   best_demand=data->demand;
										  }    
										  */
								   }   
							   } 
		                     
		                     
		                     
		                     
										  // next two sections - allowing reject category also as an option to go
									   //to remoce this, make next two section comment nad at start umcomment if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
		                                 
										else if(best_cat=='r'&& grey->grey_char!='r')
									   {
										   //printf("\n changing the best selection becouse of category");
										   if(user_choice ==YES)
												fprintf(fprocess,"\n changing the best selection becouse of category");
		                                   
										   best_index=i;
										   best_cat=grey->grey_char;
										   best_grey_index=grey->grey_index;
										   best_demand=data->demand;
									   }    
									   else if(best_cat=='r'&& grey->grey_char =='r')
									   {
										   if(best_grey_index > grey->grey_index)
										   {
											   //printf("\n changing the best selection becouse we got better option in reject category");
											   if(user_choice ==YES)
													fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category");
											   best_index=i;
											   best_cat=grey->grey_char;
											   best_grey_index=grey->grey_index;
											   best_demand=data->demand;
										   }
										  else if((best_grey_index == grey->grey_index))
										   {
												  selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
												   if(selected_index==(i+1))
												   {
														  best_index=i;
														  best_cat=grey->grey_char;
														  best_grey_index=grey->grey_index;
														  best_demand=data->demand;
		                                             
												   }
												  /*
												  //calculating random variable
												  current_time=time(NULL);
												  timeinfo=localtime(&current_time);
												  srand(time(NULL)*rand()*1000);
												   random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
												  if(user_choice ==YES)
												  fprintf(fprocess,"\n random value = %.2f",random_value);
												  if(random_value>0.5)
												  {
													  // printf("\n changing the best selection because we got better option in select category(with more demand)");
													   if(user_choice ==YES)
													   fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category (randomly)");
													   best_index=i;
													   best_cat=grey->grey_char;
													   best_grey_index=grey->grey_index;
													   best_demand=data->demand;
												  }    
												  */
										   } 
									   } 
							  }//end of else 
					   }//end of if(flag==1)    
					}//end of   if(line_sum[i] >= (capacity_each[i]*BELF_each[i]/100) && (candidate->ap_index!=i+1) )
				}//end of if((data->priority==1 &&  HIGHEST_PRIORITY_FLAG==YES)||HIGHEST_PRIORITY_FLAG==NO)    
				else
				 {
					 if(user_choice ==YES)
								  fprintf(fprocess,"\n The Airport %d don't have priority = 1",i+1);
	                 
	                 
				 }    
				 }//end of if(data->priority!=-1)
				 else
				 {
					 if(user_choice ==YES)
								  fprintf(fprocess,"\n The Airport %d has the priority = -1",i+1);
	                 
				 }
            grey=grey->next_ap_index;  
            data=data->next_data;

        }//end of   for(i=0;i<AIRPORT_NO;i++)       
        
    }//end of if(best_index==-1)     
//fclose(fp);
		free(ap_with_belf);//NEU_SH
		free(ap_demand);//NEU_VLA
		free(ap_capacity);//NEU_VLA
		free(line_sum);//NEU_VLA
		free(BELF_each);//NEU_VLA
//returning the possible destination               
           if(best_index==-1)  
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n No Possible destination ");
                   fclose(fprocess); 
               }
               return(NOT_FOUND);
           }    
           else
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n Possible destination = %d ",(best_index+1));
                   fclose(fprocess); 
               }    
              return(best_index+1);
           } 
}//end of function    
    

int New_base_data(struct Fleet *,choices *);


int New_base_data(struct Fleet *candidate,choices * prev_choices)
{
    int i,j,k;
    int HIGHEST_PRIORITY_FLAG;//to check if we gor any base station with priority=1
    int base_aircraft=0;
    int flag;
    int best_index,selected_index;
    char best_cat;
    float best_grey_index,best_demand;
    struct Bases *base;
    struct Final_grey *grey;
    struct Fleet *temp_head;
    struct Data *data;
    float random_value;
    time_t current_time;
    struct tm * timeinfo;
    choices *check_choices;
    FILE *fprocess;
	struct Hanger_capacity *hc;
    //FILE *fp;
    //fp=fopen(output_filename,"a");
    //fprintf(fp,"\n looking for new base for  %d aircraft ",candidate->ac_index);
    
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in new_base_data");
            getch();
            exit(1);
        }    
    }        

 
    //starting the search
    base=head_base;
    best_index=-1;
    if(best_index==-1)  
    {

        HIGHEST_PRIORITY_FLAG=NO;
        for(k=0;k<total_bases;k++)
        {
                //going upto the our candidate airport to get grey details of all airports
            grey=head_final_grey;

            //reading the grey detail of the base station
            for(i=1;i<base->base_index;i++)
            grey=grey->next_ap_index;

            //going upto the airport where our candidate aircraft is available
            data=head_data;
            for(i=1;i<candidate->ap_index;i++)
            for(j=0;j<AIRPORT_NO;j++)
            data=data->next_data;
            
            //reading the detail of the base station on route from our candidate airport
            for(i=1;i<base->base_index;i++)
            data=data->next_data;
            
            if(data->priority!=-1)//don't look the base if priority=-1
            {
             if((data->priority==1 )||(HIGHEST_PRIORITY_FLAG==NO))
             {
            //find out how many aircraft are present their already
            base_aircraft=0;
            temp_head=candidate;
                while(temp_head!=NULL)//look all the fleets
                {
                    if((temp_head->ap_index==base->base_index)&&(temp_head->ac_index!=candidate->ac_index))//if aircraft is on our base station
                    {
                        if(user_choice==YES)
                            fprintf(fprocess,"\n Aircraft no. %d is available at base airport %d",temp_head->ac_index,base->base_index);
                        base_aircraft++;
                    }    
                    temp_head=temp_head->next_aircraft;
                }
             if(user_choice==YES)
             {
                 fprintf(fprocess,"\n Total %d aircraft are available on base airport %d",base_aircraft,base->base_index);
                 
             }    
            
            //if base station can handle more aircraft then only its my possible base station
            //I cann't assume that aircraft which are already their will leave the base staion for me
          
            
//CHMANAV 35 new snippet for getting base and aircraft dependent hanger capacity 
		 hc=head_hc;
		 for (int manav=0;manav<i;manav++)
		  for (int manas=1;manas<candidate->ac_type;manas++)
			  hc=hc->next_hc;
			
        if(hc->cap - base_aircraft >0)
                  {
                 //look only for those airports which have not been checked till now
                   check_choices=prev_choices;
                   flag=1;
                   do
                   {
                       if(check_choices->index==base->base_index)
                       {
                           flag=0;
                          // printf("\n Base airport number %d has been checked previously",base->base_index); 
                           if(user_choice==YES)
                            fprintf(fprocess,"\n Base airport number %d has been checked previously",base->base_index);
                          
                           break;
                       }
                       check_choices=check_choices->next;
                   }while(check_choices!=NULL);    
                  
                   if(flag==1)
                   {
                       // printf("\n looking for base airport number = %d as an option to go",base->base_index);
                        if(user_choice ==YES)
                                   fprintf(fprocess,"\n looking for base airport number = %d as an option to go",base->base_index);
                        
                       if(data->priority==1 && HIGHEST_PRIORITY_FLAG==NO)//first highest priority route, 
                        {
                            HIGHEST_PRIORITY_FLAG=YES;
                            best_index=-1; 
                            if(user_choice ==YES)
                                   fprintf(fprocess,"\n Base Airport number %d has the highest priority (1) to go",i+1);                 
                            
                        } 
                        
                        //fprintf(fp," (%d/%d/%d)",best_index,base->base_index,candidate->ap_index);
                
                        if(best_index==-1)//means this is the first node
                        {
                                  best_index=base->base_index;
                                  best_cat=grey->grey_char;
                                  best_grey_index=grey->grey_index;
                                  best_demand=data->demand;
                                 
                        }    
                        else
                        {
                            //fprintf(fp,"[ %c/%c ]",best_cat,grey->grey_char);
                            //fprintf(fp,"[ %.2f/%.2f ]",best_grey_index,grey->grey_index);
                           if(best_cat=='s'&& grey->grey_char=='s')
                           {
                               if(best_grey_index < grey->grey_index)
                               {
                                  // printf("\n changing the best selection becouse we got better option in select category");
                                   if(user_choice ==YES)
                                   fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
                                   best_index=base->base_index;
                                   best_cat=grey->grey_char;
                                   best_grey_index=grey->grey_index;
                                   best_demand=data->demand;
                               } 
                               else if((best_grey_index == grey->grey_index))
                               {
                                    selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+actype)
                                   if(selected_index==base->base_index)
                                   {
                                          best_index=base->base_index;
                                          best_cat=grey->grey_char;
                                          best_grey_index=grey->grey_index;
                                          best_demand=data->demand;
                                     
                                   }
                                   /*
                                   if(base->base_index==candidate->ap_index)
                                   {
                                          best_index=base->base_index;
                                          best_cat=grey->grey_char;
                                          best_grey_index=grey->grey_index;
                                          best_demand=data->demand;
                                      }
                                      else
                                      {    
                                          //calculating random variable
                                          current_time=time(NULL);
                                            timeinfo=localtime(&current_time);
                                            srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          fprintf(fp,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          {
                                              // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                               if(user_choice ==YES)
                                               fprintf(fprocess,"\n changing the best selection becouse we got better option in select category (randomly)");
                                               best_index=base->base_index;
                                               best_cat=grey->grey_char;
                                               best_grey_index=grey->grey_index;
                                                best_demand=data->demand;
                                            } 
                                        }  
                                        */     
                               }
                           }    
                           else if(best_cat!='s'&& grey->grey_char=='s')
                           {
                              // printf("\n changing the best selection becouse of category");
                               if(user_choice ==YES)
                                    fprintf(fprocess,"\n changing the best selection becouse of category");
                               
                               best_index=base->base_index;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                               best_demand=data->demand;
                           }    
                           else if(best_cat=='p'&& grey->grey_char =='p')
                           {
                               if(best_grey_index < grey->grey_index)
                               {
                                  // printf("\n changing the best selection becouse we got better option in probable category");
                                   if(user_choice ==YES)
                                        fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
                                   best_index=base->base_index;
                                   best_cat=grey->grey_char;
                                   best_grey_index=grey->grey_index;
                                   best_demand=data->demand;
                               } 
                               else if((best_grey_index == grey->grey_index))
                               {
                                    selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source  NEU_MAC (+actype)
                                   if(selected_index==base->base_index)
                                   {
                                          best_index=base->base_index;
                                          best_cat=grey->grey_char;
                                          best_grey_index=grey->grey_index;
                                          best_demand=data->demand;
                                     
                                   }
                                   /*
                                   if(base->base_index==candidate->ap_index)
                                   {
                                          best_index=base->base_index;
                                          best_cat=grey->grey_char;
                                          best_grey_index=grey->grey_index;
                                          best_demand=data->demand;
                                      }
                                      else
                                      {    
                                           //calculating random variable
                                          current_time=time(NULL);
                                            timeinfo=localtime(&current_time);
                                            srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          fprintf(fp,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          {
                                              // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                               if(user_choice ==YES)
                                               fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category (randomly)");
                                               best_index=base->base_index;
                                               best_cat=grey->grey_char;
                                               best_grey_index=grey->grey_index;
                                                best_demand=data->demand;
                                            } 
                                        }  
                                        */  
                               } 
                           } 
                           else if(best_cat=='r'&& grey->grey_char!='r')
                           {
                              // printf("\n changing the best selection becouse of category");
                               if(user_choice ==YES)
                                    fprintf(fprocess,"\n changing the best selection becouse of category");
                               
                               best_index=base->base_index;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                               best_demand=data->demand;
                           }    
                           else if(best_cat=='r'&& grey->grey_char =='r')
                           {
                               if(best_grey_index > grey->grey_index)
                               {
                                  // printf("\n changing the best selection becouse we got better option in reject category");
                                   if(user_choice ==YES)
                                        fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category");
                                   best_index=base->base_index;
                                   best_cat=grey->grey_char;
                                   best_grey_index=grey->grey_index;
                                   best_demand=data->demand;
                               }  
                              else if((best_grey_index == grey->grey_index))
                               {
                                    selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source  NEU_MAC (+actype)
                                   if(selected_index==base->base_index)
                                   {
                                          best_index=base->base_index;
                                          best_cat=grey->grey_char;
                                          best_grey_index=grey->grey_index;
                                          best_demand=data->demand;
                                     
                                   }
                                   /*
                                   if(base->base_index==candidate->ap_index)
                                   {
                                          best_index=base->base_index;
                                          best_cat=grey->grey_char;
                                          best_grey_index=grey->grey_index;
                                          best_demand=data->demand;
                                      }
                                      else
                                      {    
                                          //calculating random variable
                                          current_time=time(NULL);
                                            timeinfo=localtime(&current_time);
                                            srand(time(NULL)*rand()*1000);
                                          random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          fprintf(fp,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          {
                                              // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                               if(user_choice ==YES)
                                               fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category (randomly)");
                                               best_index=base->base_index;
                                               best_cat=grey->grey_char;
                                               best_grey_index=grey->grey_index;
                                                best_demand=data->demand;
                                            } 
                                        }  
                                        */  
                               }  
                           } 
                          }//end of else 
                        }//end of if(flag==1)
            }//end of if(base->hanger_capacity- base_aircraft >0)
            else
            {
                if(user_choice==YES)
                {
                    
                    fprintf(fprocess,"\n Base station %d is full upto its capacity",base->base_index);
                    
                }    
            } 
            }//end of if((data->priority==1 &&  HIGHEST_PRIORITY_FLAG==YES)||HIGHEST_PRIORITY_FLAG==NO)    
            else
             {
                 if(user_choice ==YES)
                              fprintf(fprocess,"\n The Base Airport %d don't have priority = 1",base->base_index);
                 
                 
             }    
             }//end of if(data->priority!=-1)
             else
             {
                 if(user_choice ==YES)
                              fprintf(fprocess,"\n The Base Airport %d has the priority = -1",base->base_index);
                 
             }
         base=base->next_base;            
        }//end of for(i=0;i<total_bases;i++)  
    }//end of  if(best_index==-1)          
 
 //fclose(fp);
//returning the possible destination               
           if(best_index==-1)  
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n No Possible base station ");
                   fclose(fprocess); 
               }
               return(NOT_FOUND);
           }    
           else
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n Possible base station = %d ",best_index);
                   fclose(fprocess); 
               }    
              return(best_index);
           } 
}//end of function    
void No_holding();

void No_holding()
{
	int iBlockTime=0;
	//connect pTempRoute with final route data head_route
	struct route_detail *pTempRoute=head_route;
	while(pTempRoute!=NULL)
		{
		iBlockTime=Get_route_BT(pTempRoute->sr_index,pTempRoute->desti_index);
		//erase holding if Block time in schedule != real Blocktime
		if (pTempRoute->landing-pTempRoute->takeoff!=iBlockTime)
			pTempRoute->takeoff=pTempRoute->landing-iBlockTime;
		pTempRoute=pTempRoute->next;
		}
}
void Objective_calc(double *);

void Objective_calc(double *obj_value)
{
         
         int i,j,OD_served;
         int system_freq_offered,system_initial_freq_req;
		 //NEU_UT old:float total_demand,total_cost,total_time,start_time;
		 float total_demand,total_cost,total_time,total_utilization,start_time; //NEU_UT
		 float prev_time;
         float demand,cost,time,QoS;
         //NEU_VLA old: float sigma_variance,ave_QoS,sum_QoS,variance_QoS,QoS_all[AIRPORT_NO*AIRPORT_NO];
		 float sigma_variance,ave_QoS,sum_QoS,variance_QoS;//NEU_VLA
		//NEU_VLA start
		float *QoS_all=(float *)malloc((AIRPORT_NO*AIRPORT_NO)*sizeof(float));
		if (QoS_all == NULL) 
		{  
		   printf("Couldn't allocate memory\n");
		   exit(1);
		} 
		//NEU_VLA end
   		 float *utilization=NULL; //NEU_UT
		 struct Fleet *temp_fleet; //NEU_UT
         struct route_detail *item;
         struct extra *extra_item; 
         struct Data *temp_data;
        
        total_demand=0;
        total_cost=0;
        total_time=0;
		total_utilization=0; //NEU_UT
		
		//NEU_UT array for calculating individual utilization for each AC
		utilization = (float *)malloc(fleet_size*sizeof(float));
		//initialise array
		for(i=0;i<fleet_size;i++)
			utilization[i]=0;
		//NEU_UT

        for(i=0;i<fleet_size;i++)
        {
            demand=0;
            cost=0;
            time=0;
            item=head_route;
            while(item->ac_number!=i+1)
                item=item->next;
            start_time=item->takeoff;//first takeoff of the aircraft
                
            item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
                {
                   //calculating total demand carried
                    demand+=item->original_cargo;
                    if(item->extra_detail!=NULL)
                    {
                        
                        extra_item=item->extra_detail;
                        while(extra_item!=NULL)
                        {
                            if(extra_item->extra_cargo!=0)
                            demand+=extra_item->extra_cargo;
                            extra_item=extra_item->next_extra;
                        }    
                    }
					
					//NEU_UT
					//calculating the BH per AC
					if(item->takeoff!=item->landing)
						utilization[i]+=item->landing-item->takeoff;
					//NEU_UT
                    
					//calculating total time and cost taken for carrying this demand
                    if(item->takeoff!=item->landing)
                    {
                        time=item->landing;
                        prev_time=item->landing;
                    }    
                    else
                    {
                        time=prev_time;
                    }    
                    cost+=item->operating_cost;
					
                }  //end of if(item->ac_number==i+1)  
                item=item->next;
            }//end of while(item!=NULL)
            
            total_demand+=demand;
            total_cost+=cost;
            total_time+=(time-start_time);//add only the flying time
        }    //end of for(i=0;i<fleet_size;i++)
        
		//calculation of Utilization NEU_UT
		temp_fleet=head_aircraft_list;
		for(i=0;i<fleet_size;i++)
		{
			utilization[i]=utilization[i]/temp_fleet->life;
			total_utilization+=utilization[i];
			temp_fleet=temp_fleet->next_aircraft;
		}
		total_utilization=total_utilization/fleet_size;
		//NEU_UT
	
        //calculating QoS 
        temp_data=head_data;
        OD_served=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            {
                if(temp_data->initial_freq_req!=0)
                QoS_all[i*AIRPORT_NO+j]=(1.0*temp_data->freq_offered)/temp_data->initial_freq_req;
                else
                QoS_all[i*AIRPORT_NO+j]=1.0;
                if(temp_data->freq_offered!=0)
                OD_served++;
                
            }   
            else
            QoS_all[i*AIRPORT_NO+j]=0;
            
            temp_data=temp_data->next_data; 
        }    
        //if(system_initial_freq_req==0)
        //QoS=1.0;
        //else
        //QoS=((1.0 *system_freq_offered)/system_initial_freq_req)*((1.0*counter)/(AIRPORT_NO*(AIRPORT_NO-1)));
        sum_QoS=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            sum_QoS+=QoS_all[i*AIRPORT_NO+j];
        }    
        ave_QoS=sum_QoS/(AIRPORT_NO*(AIRPORT_NO-1));
        
        
        sigma_variance=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            sigma_variance+=((QoS_all[i*AIRPORT_NO+j]-ave_QoS)*(QoS_all[i*AIRPORT_NO+j]-ave_QoS));
        }    
        
        variance_QoS=sigma_variance/(AIRPORT_NO*(AIRPORT_NO-1));
        //===============objective function=================//
        obj_value[0]=total_demand;
        obj_value[1]=total_cost;
        obj_value[2]=total_time;
        obj_value[3]=variance_QoS; 
		obj_value[4]=total_utilization;//NEU_UT
          

			/*for(i=0;i<fleet_size;i++)//NEU_UT
			printf("\n%.2f\n",utilization[i]);//NEU_UT
			printf("\nOBJV:%.2f\n",obj_value[4]);//NEU_UT*/

		free(utilization);//NEU_BH
		free(QoS_all);//NEU_VLA            
}    
float Percentile_calc(float *,int,float);

float Percentile_calc(float *array ,int size,float per)
{
    float index;
    int place;
    index = ceil((per*size)/100.0);
    place =(size-(int)(index))+1;//sience our array is arranged in reverse order, desending 
    return(array[place-1]);   

}    
//#include"get_ap_index.h"
void Read_input();

void Read_input()
{
    FILE *fp;
    char ch;
    int i,j,k;//NEU_DOC
    int total_capacity=0;
    float cost,system_cost;
    struct timing *current_time,*last_time;
    struct Airports *airport,*last_airport;
    struct Data *data,*last_data;
    struct Fleet *fleet,*last_fleet;
    struct Bases *base,*last_base;
    struct Grey *grey,*last_grey;
	struct AC_data *ac_data=NULL,*last_ac_data=NULL;//NEU_MAC
	struct static_cost *head_static_cost, *static_cost, *last_static_cost;//NEU_DOC
	struct aea_input *aea_input, *last_aea_input;//NEU_DOC
    float sum;
	struct Hanger_capacity *hc,*last_hc;
    
    
    //opening the input file
 
    fp=fopen(input_filename,"r");
    if(fp==NULL)
    {
        printf("\n cann't open the %s file",input_filename);
        getch();
        exit(1);
    } 
    

    /*
    //opening the input file
    fp=fopen("Input.txt","r");
    if(fp==NULL)
    {
        printf("\n cann't open the \"Input\" file");
        getch();
        exit(1);
    }  
    */  
    //reading the initial comment lines
    for(i=0;i<6;i++)
    {
        do
        {
            ch=getc(fp);
        }while(ch!='\n');
    }  
    //reading total number of airports in the network
    //NEU die zeile wurde deaktiviert TEMPORÄR WG VARIABLE LENGTH ARRAY
	fscanf(fp,"%d",&AIRPORT_NO);//wieder rück
	//NEU
    
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }

    //Reading the Airport details
    for(i=0;i<AIRPORT_NO;i++)
    {
        
        airport=(struct Airports *)malloc(sizeof(struct Airports));
        fscanf(fp,"%d %s %f %d",&airport->ap_index,airport->name,&airport->delay,&airport->takeoff_slots);
        airport->takeoff_time=NULL;
		//NEU
		airport->next_airport=NULL;
		//NEU
        for(j=0;j<airport->takeoff_slots;j++)
        {
            current_time=(struct timing *)malloc(sizeof(struct timing));
            fscanf(fp,"%f-%f",&current_time->start,&current_time->end);
            current_time->next_time=NULL;
            if(airport->takeoff_time==NULL)
            {
                airport->takeoff_time=current_time;
                last_time=current_time;
            }
            else
            {
                last_time->next_time=current_time;
                last_time=current_time;
            }            
        }
        fscanf(fp,"%d",&airport->landing_slots);
        airport->landing_time=NULL;
        for(j=0;j<airport->landing_slots;j++)
        {
            current_time=(struct timing *)malloc(sizeof(struct timing));
            fscanf(fp,"%f-%f",&current_time->start,&current_time->end);
            current_time->next_time=NULL;
            if(airport->landing_time==NULL)
            {
                airport->landing_time=current_time;
                last_time=current_time;
            }
            else
            {
                last_time->next_time=current_time;
                last_time=current_time;
            }
        }
       // fscanf(fp,"%f",&airport->BELF);
        
        if(head_airport==NULL)
        {
            head_airport=airport;
            last_airport=airport;
        }
        else
        {
            last_airport->next_airport=airport;
            last_airport=airport;
        }    
    
    } 
    /*
    //printing the airport details
    printf("\n Airport Detail -");
    airport=head_airport;
    for(j=0;j<AIRPORT_NO;j++)
    { 
       
       printf("\n %d %s %.2f %d ",airport->ap_index,airport->name,airport->delay,airport->takeoff_slots);
       last_time=airport->takeoff_time;
        for(i=0;i<airport->takeoff_slots;i++)
        {
            printf(" %.2f-%.2f ",last_time->start,last_time->end);
            last_time=last_time->next_time;
        }  
        printf(" %d ",airport->landing_slots);
        last_time=airport->landing_time;
        for(i=0;i<airport->landing_slots;i++)
        {
            printf(" %.2f-%.2f ",last_time->start,last_time->end);
            last_time=last_time->next_time;
        } 
        printf(" %.2f ",airport->BELF);
        airport=airport->next_airport;
    } 
    */
    //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the demand array
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        data=(struct Data *)malloc(sizeof(struct Data));
        fscanf(fp,"%f",&data->demand);
        data->next_data=NULL;
        if(head_data==NULL)
        {
            head_data=data;
            last_data=data;
        }
        else
        {
            last_data->next_data=data;
            last_data=last_data->next_data;
        }
    } 
    
     /*operating cost read deactivated NEU_DOC
	 //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }           
    //reading the  Operating cost array
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->TOC);
        data=data->next_data;
    }NEU_DOC deactivation end*/
   /* 
    //calculating total system operating cost
    data=head_data;
    system_cost=0;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        system_cost+=data->TOC;
        data=data->next_data;
    }    
    //calculating the cost ratio at each airport
    airport=head_airport;
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    {
        cost=0;            
        for(j=0;j<AIRPORT_NO;j++)
        {
            cost+=data->TOC;
            data=data->next_data;
        }
        airport->ap_cost_ratio=cost/system_cost;
        airport=airport->next_airport;
    }        
    
    
    
  */  
    
    
    //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }           
    //reading the  Block Time array
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->block_time);
        data=data->next_data;
    }     
    //reading the comment lines NEU_PLR
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//reading the  Distance array NEU_PLR
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->distance);
        data=data->next_data;
    }     
    /* reading of capacity deactivated NEU_PLR
	//reading the comment lines NEU_DOC
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the  Aircraft capacity Array
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->capacity);
        data=data->next_data;
    }deactivation end NEU_PLR*/
    //reading the comment lines
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//reading no. of ac_types NEU_MAC
	fscanf(fp,"%d",&ACTYPE_NO);
    //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }  
    //reading total aircraft available in fleet
    fscanf(fp,"%d",&fleet_size);      
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }  
    //reading the fleet detail
//CHMANAV 20 tat inputted for particular aircrafts 
	struct tats *tat,*last_tat;
	head_tat=NULL;
    for(i=0;i<fleet_size;i++)
    {
        fleet=(struct Fleet *)malloc(sizeof(struct Fleet));
        tat=(struct tats *)malloc(sizeof(struct tats));
        fscanf(fp,"%d %d %s %f %f %f %f",&fleet->ac_index,&fleet->ac_type,fleet->ap_name,&fleet->takeoff_time,&fleet->life,&fleet->BH_max,&tat->tat); //NEU_MAC NEU_BH (ac_type,BH_max,BH_remain)
		fleet->BH_remain=fleet->BH_max; //NEU_BH
        fleet->ap_index=Get_ap_index(fleet->ap_name);
        if(fleet->ap_index==NOT_FOUND)
        {
            printf("\n Airport not found in the list, Please check the airport names");
            getch();
            exit(1);
        }    
        fleet->next_aircraft=NULL;
        if(head_fleet==NULL)
        {
            head_fleet=fleet;
            last_fleet=fleet;
        }
        else
        {
            last_fleet->next_aircraft=fleet;
            last_fleet=last_fleet->next_aircraft;
        }
		tat->next_tat=NULL;
        if(head_tat==NULL)
        {
            head_tat=tat;
            last_tat=tat;
        }
        else
        {
            last_tat->next_tat=tat;
            last_tat=last_tat->next_tat;
        }
    }  
    /*
    //printing the fleet detail
    printf("\n Fleet detail");
    fleet=head_fleet;
    for(i=0;i<fleet_size;i++)
    {
        printf("\n %d %s %d %.2f %.2f ",fleet->ac_index,fleet->ap_name,fleet->ap_index,fleet->takeoff_time,fleet->life);
        fleet=fleet->next_aircraft;
    }    
    */
	
	//reading the comment lines NEU_DOC
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	fscanf(fp,"%c",&cCostWithAEA);
	if (cCostWithAEA!=110 && cCostWithAEA!=121)
	{
		printf("\nCost Function Input must be y or n\nEND");
		getch();
        exit(0);
	}
	//reading the comment lines NEU_MAC
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//reading fleet type specification NEU_MAC NEU_PLR
	for(i=0;i<ACTYPE_NO;i++)
    {
		ac_data=(struct AC_data *)malloc(sizeof(struct AC_data));
        fscanf(fp,"%d %s %f %f %f %f %f",&ac_data->ac_type,&ac_data->ac_code,&ac_data->fR1,&ac_data->fR2,&ac_data->fR3,&ac_data->fPL1,&ac_data->fPL2); //NEU_MAC
        ac_data->next_type=NULL;
        if(head_ac_data==NULL)
        {
            head_ac_data=ac_data;
            last_ac_data=ac_data;
        }
        else
        {
            last_ac_data->next_type=ac_data;
            last_ac_data=last_ac_data->next_type;
        }
    }  
	//reading the comment lines NEU_DOC
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//Reading Static Operating Cost NEU_DOC
	ac_data=head_ac_data;
	for(i=0;i<ACTYPE_NO;i++)
	{

		head_static_cost=NULL;
		for(j=0;j<(AIRPORT_NO*AIRPORT_NO);j++)
		{
			static_cost=(struct static_cost *)malloc(sizeof(struct static_cost));
			fscanf(fp,"%f",&static_cost->cost); //NEU_MAC
			static_cost->next_cost=NULL;
			if(head_static_cost==NULL)
			{
				head_static_cost=static_cost;
				last_static_cost=static_cost;
			}
			else
			{
				last_static_cost->next_cost=static_cost;
				last_static_cost=last_static_cost->next_cost;
			}
		}
		ac_data->static_cost=head_static_cost;
		ac_data=ac_data->next_type;
		for(k=0;k<2;k++)
		{    
			do
			{
					ch=getc(fp);
			}while(ch!='\n');
		}
	}

	/*//reading the comment lines NEU_MAC
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }*/
    //reading the total number of bases in network 
	//NEU die zeile wurde deaktiviert
    fscanf(fp,"%d",&total_bases);//NEU_VLA bases wieder einlesbar, alles durch malloc ersetzt
	//NEU
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    } 
    //Reading the base station detail
	head_hc=NULL;
    for(i=0;i<total_bases;i++)
    {
        base=(struct Bases *)malloc(sizeof(struct Bases));

data=head_data;
        fscanf(fp,"%d %s",&base->base_index,base->base_name);
		
		for (j=0;j<ACTYPE_NO;j++)
		{
			hc=(struct Hanger_capacity *)malloc(sizeof(struct Hanger_capacity));
			fscanf(fp,"%d",&hc->cap);
		hc->next_hc=NULL;
        if(head_hc==NULL)
        {
            head_hc=hc;
            last_hc=hc;
        }
        else
        {
            last_hc->next_hc=hc;
            last_hc=last_hc->next_hc;
        }
}
		
        base->base_index=Get_ap_index(base->base_name);
        base->next_base=NULL;
        if(head_base==NULL)
        {
            head_base=base;
            last_base=base;
        }
        else
        {
            last_base->next_base=base;
            last_base=last_base->next_base;
        }
		    }
    hc=head_hc;
    while(hc!=NULL)
    {
        total_capacity+=hc->cap;
        hc=hc->next_hc;
    }
	
    if(fleet_size>total_capacity)
    {
        printf("\n Error : Total base capacity is less then the fleet size");
        getch();
        exit(1);
    }        
    //reading the comment lines
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }            
    //reading the max. time slot for operation
    fscanf(fp,"%f",&max_time_slot);

    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }   
    //reading the route priorities   
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->priority);
        data=data->next_data;
    }  
    //CHMANAV2 READ INPUT FOR ROUTE DEPENDENT BELF
	for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }   
    //reading the route BELFs   
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->BELF);
        data=data->next_data;
    }  
    //checking the data entries, main diagnol elements should be zero
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {//CHMANAV6 ADDED A CONTRAINT TO THROW AN ERROR IF BELF IS OUTSIDE 0 AND 1
		if (data->BELF<0.0 || data->BELF>1.0)
		{
           printf("\n\n ERROR ! - the elements in BELF input should be between zero and one\n\n");
           getch();
           exit(1);
        }
         if(i==j)
        {
            if(data->block_time!=0)
            {
                printf("\n\n ERROR ! - the diagonal elements in Block time input should be zero\n\n");
                getch();
                exit(1);
            }    
            if(data->demand!=0)
            {
                printf("\n\n ERROR ! - the diagonal elements in Demand input should be zero\n\n");
                getch();
                exit(1);
            }
            if(data->distance!=0) //NEU_PLR (was ->capacity)
            {
                printf("\n\n ERROR ! - the diagonal elements in Aircraft capacity input should be zero\n\n");
                getch();
                exit(1);
            }
            /*if(data->TOC!=0) deactivated NEU_PLR
            {
                printf("\n\n ERROR ! - the diagonal elements in Total operating cost input should be zero\n\n");
                getch();
                exit(1);
            }*/
            if(data->priority!=0)
            {
                printf("\n\n ERROR ! - the diagonal elements in Priority of Routes input should be 0 \n\n");
                getch();
                exit(1);
            }
        }    
        data=data->next_data;
    }    
        
  /*
    //reading the comment lines
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    } 
    //reading the total number of grey indices
    fscanf(fp,"%d",&total_grey_indices);
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    
    //reading the grey indices priorities
    for(i=0;i<total_grey_indices;i++)
    {
        grey=(struct Grey *)malloc(sizeof(struct Grey));
        fscanf(fp,"%f",&grey->priority);
        grey->next_grey=NULL;
        if(head_grey==NULL)
        {
            head_grey=grey;
            last_grey=grey;
        }
        else
        {
            last_grey->next_grey=grey;
            last_grey=last_grey->next_grey;
        }
    } 
    
   
    //checking - the sum of priorities should =1
    grey=head_grey;
    sum=0;
    for(i=0;i<total_grey_indices;i++)
    {
        sum+=grey->priority;
        grey=grey->next_grey;
    }
    if(sum!=1)
    {
        printf("\n\n Error -");
        printf("\n The sum of grey Indices priorites should = 1");
        printf("\n Please check the input file");
        printf("\n Press any key to exit-");
        getch();
        exit(1);
    }        
    */
    
    //reading the comment lines
    for(i=0;i<4;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //assiging the memory for reading the whitening function values or decision values
    BIG=(struct Whitening_decision *)malloc(sizeof(struct Whitening_decision));
    SMALL=(struct Whitening_decision *)malloc(sizeof(struct Whitening_decision));
   
     //reading the whitening values for demand index  
    fscanf(fp,"%f %f",&BIG->demand,&SMALL->demand);
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for cost index  
    fscanf(fp,"%f %f",&BIG->cost,&SMALL->cost);
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for time index  
    fscanf(fp,"%f %f",&BIG->time,&SMALL->time);
    /*
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for Demand satisfaction index 
     fscanf(fp,"%f %f",&BIG->network_design,&SMALL->network_design);
     */
     //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for route priority index  
    fscanf(fp,"%f %f",&BIG->route_priority,&SMALL->route_priority);
   

     // CHMANAV 17 TURN AROUND TIME INPUT COMMENTED
     //reading the comment lines
    /*for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the TURN AROUND TIME
    fscanf(fp,"%f",&TURN_AROUND_TIME);
    *///closing the input file
    fclose(fp);

	//read AEA input file NEU_DOC
	if(cCostWithAEA==121)
	{
		fp=fopen("DataAEA.txt","r");
		if(fp==NULL)
		{
			printf("\n Can't find DataAEA.txt for AEA calculation");
			getch();
			exit(1);
		}
		//reading the initial lines
		for(i=0;i<4;i++)
		{
			do
			{
				ch=getc(fp);
			}while(ch!='\n');
		}  
		for(j=0;j<ACTYPE_NO;j++)
		{
			aea_input=(struct aea_input *)malloc(sizeof(struct aea_input));
			//read comment lines
			for(i=0;i<4;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}  
			//Read Airframe data
			fscanf(fp,"%lf %lf %lf %lf %lf",&aea_input->MSSP,&aea_input->BFE,&aea_input->MTOW,&aea_input->W_fuel,&aea_input->W_af);
			//next row
			for(i=0;i<3;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}
			//Read Engine Data
			fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf",&aea_input->engine_cost,&aea_input->No_Engines,&aea_input->BPR,&aea_input->OPR,&aea_input->N_c,&aea_input->T_sls,&aea_input->N_shaft);
			//next row
			for(i=0;i<3;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}
			//Read Finance Data
			fscanf(fp,"%lf %lf %lf %lf",&aea_input->Price_fuel,&aea_input->F_resval,&aea_input->F_int,&aea_input->R_ins);
			//next row
			for(i=0;i<3;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}
			//read Other Data
			fscanf(fp,"%lf %lf %lf %lf %lf %lf",&aea_input->Life,&aea_input->C_coc,&aea_input->C_cab,&aea_input->N_coc,&aea_input->N_cab,&aea_input->R_lab);
			aea_input->next_ac=NULL;
			if(head_aea_input==NULL)
			{
				head_aea_input=aea_input;
				last_aea_input=aea_input;
			}
			else
			{
				last_aea_input->next_ac=aea_input;
				last_aea_input=last_aea_input->next_ac;
			}
		}
		fclose(fp);
	}//end of if(cCostWithAEA==121)//NEU_DOC

}//end of function    
//#include"insert_sort.h"
void Sorted_fleet();

void Sorted_fleet()
{
    int i;
    struct Fleet *fleet;
    struct Fleet *candidate;
    fleet=head_fleet; //memory of head_fleet allocated in read_input()
    for(i=0;i<fleet_size;i++)
    {
        candidate=(struct Fleet *)malloc(sizeof(struct Fleet));
        candidate->ac_index=fleet->ac_index;
        candidate->ap_index=fleet->ap_index;
		candidate->ac_type=fleet->ac_type; //NEU_MAC
        candidate->end_time=fleet->end_time;
        candidate->life=fleet->life;
        candidate->takeoff_time=fleet->takeoff_time;
        strcpy(candidate->ap_name,fleet->ap_name);
        candidate->demand_carried=0;
        candidate->residual_capacity=0;
		candidate->BH_max=fleet->BH_max; //NEU_BH
		candidate->BH_remain=fleet->BH_remain; //NEU_BH
        candidate->next_aircraft=NULL;
        Insert_sort(candidate); //memory allocated for candidate goes in head_aircraft_list
        fleet=fleet->next_aircraft;
    }
    //free the memory (was allocated for head_fleet)
	fleet=head_fleet;
    while(fleet!=NULL)
    {
        candidate=fleet;
        fleet=fleet->next_aircraft;
        free(candidate);
    }
    //printing the sorted fleet detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in sorted_fleet");
            getch();
            exit(1);
        }    
        fprintf(fprocess,"\n\n Fleet Sorted as per Takeoff time- ");
        fleet = head_aircraft_list;
        fprintf(fprocess,"\n Ac_No\tType\tAirport Ap_No\tTakeoff_time\tEnd_time\tBH_max\tBH_remain");
        for(i=0;i<fleet_size;i++)
        {
            //old: fprintf(fprocess,"\n %d\t%s\t%d\t%.2f\t\t%.2f",fleet->ac_index,fleet->ap_name,fleet->ap_index,fleet->takeoff_time,fleet->end_time);
			fprintf(fprocess,"\n %d\t%d\t%s\t%d\t%.2f\t\t%.2f\t%.2f\t%.2f",fleet->ac_index,fleet->ac_type,fleet->ap_name,fleet->ap_index,fleet->takeoff_time,fleet->end_time,fleet->BH_max,fleet->BH_remain); //NEU_BH
            fleet=fleet->next_aircraft;
        }  
        fclose(fprocess);  
    }//end of  if(user_choice==YES)           
}//end of function    
//#include"trace_demand.h"
void Store_route_info(struct Fleet *,float ,float ,int );

void Store_route_info(struct Fleet *candidate,float new_take_off,float time_diff,int desti_index)
{

    int i;
	struct route_detail *item;
	struct AC_data *temp_acdata ;//NEU_MAC
   //CHMANAV 25 defining tat for usage for particular candidate aircraft
	struct tats *tat;
	  tat=head_tat;
	  for(i=1;i<candidate->ac_index;i++)
		  tat=tat->next_tat;

    //storing all the info in a new list called route detail
    item=(struct route_detail*)malloc(sizeof(struct route_detail));
    item->ac_number=candidate->ac_index;
    item->sr_index=candidate->ap_index;
	item->ac_type=candidate->ac_type;//NEU_MAC Start
	temp_acdata=head_ac_data;
	for(i=0;i<ACTYPE_NO;i++)
	{
		if(temp_acdata->ac_type==candidate->ac_type)
			strcpy(item->ac_code,temp_acdata->ac_code);
		temp_acdata=temp_acdata->next_type;
	}//NEU_MAC End
    item->desti_index=desti_index;
    item->takeoff=candidate->takeoff_time;
    item->cargo_carried=candidate->demand_carried;
    item->original_cargo=candidate->demand_carried;//between that OD pair
    item->residual_capacity=candidate->residual_capacity;
    item->operating_cost=candidate->operating_cost;
    if(head_route==NULL)
    item->flight_no=1;
    else
    item->flight_no=last_route->flight_no+1;
    if(SENDING_TO_BASE==NO)//CHMANAV 24 using tat instead of turn around time
    item->landing=new_take_off-time_diff-tat->tat;
    else
    item->landing=new_take_off;
    
    item->next=NULL;
    item->prev=NULL;
    item->extra_detail=NULL;
    
    if(head_route==NULL)
    {
        head_route=item;
        last_route=item;
    }
    else
    {
        last_route->next=item;
        item->prev=last_route;
        last_route=last_route->next;
    }     
    
       if(item->residual_capacity!=0 && item->prev!=NULL)//not checking for 1st flight
       {
           if(user_choice==YES)
           {
           FILE *fprocess = fopen("Output/PROCESS.txt","a");
           if(fprocess==NULL)
            {
                printf("\n cann't open the Output/PROCESS file in store_route_info");
                getch();
                exit(1);
            }
           fprintf(fprocess,"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
           fprintf(fprocess,"\n tracing the extra demand for flight no. %d",item->flight_no);
           fclose(fprocess);
           }
            //start tracing the extra demand or adjusted demand    
           Trace_demand(item->ac_number,desti_index);   
           
       }   
      
      
       
       
  
}//end of function         
void Store_updated_route(struct empty_detail *,int ,float );

void Store_updated_route(struct empty_detail *empty,int final_destination,float demand_req)
{
    int i,j;
    float demand=0,min_residual=0,total_carried,demand_carry=0;
    struct Data *temp_data;
    struct empty_detail *find_empty,*temp_empty;
    struct Fleet *fleet;
    struct extra *extra_carry,*last_carry;
    struct route_detail *item;
	//NEU
	struct empty_detail *pTempDeleter;
    FILE *fprocess;

    total_carried=0.0;
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
            {
                printf("\n cann't open the Output/PROCESS file in store_updated_route");
                getch();
                exit(1);
            }
        fprintf(fprocess,"\n demand required = %.2f ",demand_req);
    }    
              do
              { 
                //read the demand between its source and our final destination
                temp_data=head_data;
                //going upto the source airport from where our aircraft takesoff empty
                for(i=1;i<empty->source;i++)
                for(j=0;j<AIRPORT_NO;j++)
                temp_data=temp_data->next_data;
                //going upto the final destination airport
                for(j=1;j<final_destination;j++)
                temp_data=temp_data->next_data;
                if(user_choice==YES)
                fprintf(fprocess,"\n demand between %d and %d = %.2f ",empty->source,final_destination,temp_data->demand);
                //if min demand is satisfied then finding next min demand in remaining network
                if(demand_req==0.0)
                {
                    find_empty=empty;
                    if(find_empty->next_empty_detail!=NULL)
                    {
                        min_residual=find_empty->residual_capacity;
                        while(find_empty->next_empty_detail!=NULL)
                        {
                            find_empty=find_empty->next_empty_detail;
                            if(min_residual>find_empty->residual_capacity)
                            min_residual=find_empty->residual_capacity;
                        }
                        demand_req=min_residual-total_carried;
                        if(user_choice==YES)
                        fprintf(fprocess,"\n new demand  req = %.2f",demand_req);
                    }
                }//end of if(demand_req==0)
                
                //finding how much extra to carry
                if( temp_data->demand >= demand_req)
                  {
                      demand_carry=demand_req;
                      
                  }
                  else
                  {
                      demand_carry=temp_data->demand;                      
                      
                  }
                  if(user_choice==YES)
                  fprintf(fprocess,"\n Extra demand carried = %.2f",demand_carry);
                  //updating the main demand data
                  temp_data->demand-=demand_carry;
                  
                  
                  //updating the cargo carried and residual capacity in main route detail
                  extra_carry=(struct extra *)malloc(sizeof(struct extra));
                  extra_carry->desti_index=final_destination;
                  extra_carry->extra_cargo=demand_carry;
                  extra_carry->next_extra=NULL;
                  
                  //where to add this info in main route detail 
                  item=head_route;
                  for(i=1;i<empty->flight;i++)
                      item=item->next;
                  //now item showin right place to add info    
                  
                  //adding it 
                  if( item->extra_detail==NULL)
                  {
                      item->extra_detail=extra_carry;
                      
                  }
                  else
                  {
                      last_carry=item->extra_detail;
                      while(last_carry->next_extra!=NULL)
                      last_carry=last_carry->next_extra;
                      last_carry->next_extra=extra_carry;
                      
                  }
                     
                  //updating the info of main route detail
                  temp_empty=empty;
                  while(temp_empty!=NULL)
                  {
                      item=head_route;
                      for(i=1;i<temp_empty->flight;i++)
                      item=item->next;
                      
                      if(user_choice==YES)
                      {
                          fprintf(fprocess,"\n before carrying extra demand-");
                          fprintf(fprocess,"\n cargo carried on the flight %d = %.2f",item->flight_no,item->cargo_carried);
                          fprintf(fprocess,"\n empty = %.2f",item->residual_capacity);        
                      } 
                      
                      item->cargo_carried+=demand_carry;
                      item->residual_capacity-=demand_carry;
                      temp_empty=temp_empty->next_empty_detail;
                      
                      if(user_choice==YES)
                      {
                          fprintf(fprocess,"\n After carrying extra demand");
                          fprintf(fprocess,"\n total cargo carried on the flight %d = %.2f",item->flight_no,item->cargo_carried);
                          fprintf(fprocess,"\n Remaining empty = %.2f",item->residual_capacity);    
                      }  
                      
                  }    
                  
                    
                  
                  
                  
                  //remaining demand required
                  demand_req-=demand_carry;
                  if(user_choice==YES)
                  fprintf(fprocess,"\n demand required now = %.2f",demand_req);
                  //total extra demand which we had already collected
                  total_carried+=demand_carry;
                  
                  //go on next detail to update
				  //NEU
				  pTempDeleter=empty;
				  //NEU
                  empty=empty->next_empty_detail;
				  //NEU
				  free(pTempDeleter);
                  //NEU
              }while(empty!=NULL);    
               if(user_choice==YES)
               {
                    fprintf(fprocess,"\n Updated Demand -\n");
                    temp_data=head_data;
                    for(i=0;i<AIRPORT_NO;i++)
                    {
                        for(j=0;j<AIRPORT_NO;j++)
                        {
                            fprintf(fprocess," %.2f\t",temp_data->demand);
                            temp_data=temp_data->next_data;
                        }
                        fprintf(fprocess,"\n");
                    }
                   fprintf(fprocess,"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
                   fclose(fprocess);  
               }    

	//NEU
	pTempDeleter=NULL;
	//NEU
}//end of function    
//This function do 2 works
//1. go to the point from where the aircraft is flying empty
//2. store the detail
//#include"store_updated_route.h"

void Trace_demand(int, int );

void Trace_demand(int our_ac,int final_destination)
{
    int i,j;
    float total_demand,min_residual;
    struct empty_detail *head_empty_detail = NULL,*last_empty_detail,*empty; 
    struct route_detail *item;
    struct Data *temp_data;
    FILE *fprocess;
    
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
                {
                    printf("\n cann't open the Output/PROCESS file in trace_demand");
                    getch();
                    exit(1);
                }
    }    
    //1. go to the point from where the aircraft is flying empty
    //item=last_route->prev;//since last route in current route so we are going one step back
    item=last_route;
    
    while(item->prev!=NULL )//from last to the first flight
    {
            
        if(item->ac_number == our_ac)//if this detail is of our ac then record it in another struct empty detail
        {
           
            if(item->residual_capacity !=0)
            {
                empty=(struct empty_detail *)malloc(sizeof(struct empty_detail));
                empty->flight=item->flight_no;
                empty->source=item->sr_index;
                empty->residual_capacity=item->residual_capacity;
                empty->next_empty_detail=NULL;
                empty->prev_empty_detail=NULL;
                if(head_empty_detail==NULL)
                {
                    head_empty_detail=empty;
                    last_empty_detail=empty;
                }
                else
                {
                    last_empty_detail->prev_empty_detail=empty;
                    empty->next_empty_detail=last_empty_detail;
                    last_empty_detail=last_empty_detail->prev_empty_detail ;
                }       
               
            }//end of if (residual_capacity !=0)
            else
            {
               break; //no need to go more back from here
            }  //end of else   
        }//end of if(ac_number == our_ac)
        item=item->prev;
    }//end of while(item!=NULL)
    
    
    empty=head_empty_detail;
    if(empty!=NULL)
    {
        
        if(user_choice==YES)
        {           
            fprintf(fprocess,"\n Empty detail");
            fprintf(fprocess,"\n Flight Ac  Source empty");
        }    
            
        
        while(empty!=NULL)
        {
            if(user_choice==YES)
            fprintf(fprocess,"\n %d\t%d\t%d\t%.2f ",empty->flight,our_ac,empty->source,empty->residual_capacity);
            empty=empty->prev_empty_detail;
        }
        
    }   
        
        //2. go to the point from where we need to update the demand
        total_demand=0.0;
        min_residual=last_route->residual_capacity;//our current residual capacity
        empty=head_empty_detail;
        last_empty_detail=NULL;
        //if(empty!=NULL)
        //{
            //fprintf(fprocess,"\n Min. req= %.2f",min_residual);
            while(empty!=NULL && total_demand<min_residual)
            {
                //read the demand between its source and our final destination
                temp_data=head_data;
                //going upto the source airport from where our aircraft takesoff empty
                for(i=1;i<empty->source;i++)
                for(j=0;j<AIRPORT_NO;j++)
                temp_data=temp_data->next_data;
                //going upto the final destination airport
                for(j=1;j<final_destination;j++)
                temp_data=temp_data->next_data;
                
                if(empty->residual_capacity < min_residual)
                {
                    min_residual=empty->residual_capacity;
                    total_demand=0;
                }
                total_demand+=temp_data->demand;  
                last_empty_detail=empty;//keeping record of last detail, because when we will go out empty=NULL  
                empty=empty->prev_empty_detail;
                
            }//end of while    
            //update the demand from last_empty_detail
            if(last_empty_detail!=NULL)
            {
                if(user_choice==YES)
                {
                    fprintf(fprocess,"\n Start to take extra cargo from airport %d in flight %d",last_empty_detail->source,last_empty_detail->flight);
                    fclose(fprocess);
                }    
                
                
                //find and store how much extra to carry from these airports
                Store_updated_route(last_empty_detail,final_destination,min_residual);
            }    
        
}//end of function    
void Write_output();
void Write_output()
{
    int i,j;
    int OD_served;
    int freq,flag;
    int system_freq_offered,system_initial_freq_req;
    float demand,cost,route_time,objective_value,index[total_grey_indices],QoS,remaining_demand;
    //NEU_UT old:	float total_demand,total_cost,total_time,start_time;
	float total_demand,total_cost,total_time,start_time,total_utilization;//NEU_UT
	float prev_route_time;
	char to[20];
	struct Airports *airport,*last_airport,*airport1,*last_airport1;
    //NEU_VLA old: float sigma_variance,ave_QoS,sum_QoS,variance_QoS,QoS_all[AIRPORT_NO*AIRPORT_NO];
	float sigma_variance,ave_QoS,sum_QoS,variance_QoS;//NEU_VLA
    //NEU_VLA start
		float *QoS_all=(float *)malloc((AIRPORT_NO*AIRPORT_NO)*sizeof(float));
		if (QoS_all == NULL) 
		{  
		   printf("Couldn't allocate memory\n");
		   exit(1);
		} 
	//NEU_VLA end
    float *utilization=NULL; //NEU_UT
	struct Fleet *temp_fleet; //NEU_UT
	struct route_detail *item;
    struct extra *extra_item;   
    struct Grey *grey_priority;
    struct Data *temp_data;
    
    time_t current_time;
    struct tm * timeinfo;
    
                 time(&current_time);
                 timeinfo = localtime ( &current_time );
    
    FILE *fprocess=fopen(output_filename,"w");
                if(fprocess==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",output_filename);
                    printf("\n cann't make the "); puts(output_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
                fprintf(fprocess," -------Airline Network Optimization for cargo Airlines--------BETA version");
                fprintf(fprocess,"\n >>>>****for testing and evalution only **** not for commercial use****<<<<\n");
                fprintf(fprocess,"\n ===========================================================================");
                fprintf(fprocess,"\n This program is designed and developed at ");
                fprintf(fprocess,"\n Aerospace Engineering Department, IIT Bombay \n");    
                fprintf(fprocess,"\n For any query/suggestion, contact - ");
                fprintf(fprocess,"\n Prof. Rajkumar S. Pant , LTA systems lab, rkpant@aero.iitb.ac.in");
                fprintf(fprocess,"\n Ajeet Singh, MTech Student(2005-2007), ajeet@aero.iitb.ac.in");
				fprintf(fprocess,"\n Florian Maul, Technical University of Munich, florian.maul@mytum.de");
				fprintf(fprocess,"\n Manav Agarwal, Undergraduate 2007-2012, IIT Bombay, manav.agarwal@iitb.ac.in");
                fprintf(fprocess,"\n ===========================================================================\n");

                fprintf(fprocess,"\n The Schedule is generated on :\n %s ",asctime(timeinfo));//writing the start time
                fprintf(fprocess,"\n ===========================================================================\n");



				
			
                //fprintf(fp," Objective function value = %f\n",it);//writing the best objective value returned by GA
                //fprintf(fp,"\n ===========================================================================\n");
           /*
             fclose(fprocess);
         FILE *fprocess=fopen(output_filename,"a");
        if(fprocess==NULL)
        {
            //printf("\n cann't make the %s file in write_output",output_filename);
            printf("\n cann't make the "); puts(output_filename);printf(" file in write_output");
            getch();
            exit(1);
        }    
        */
        fprintf(fprocess,"\n\n ------------------------------------------");
        fprintf(fprocess,"\n System Detail");
        fprintf(fprocess,"\n ------------------------------------------");
        fprintf(fprocess,"\n Aircraft\tDemand\tCost\tTime");
        total_demand=0;
        total_cost=0;
        total_time=0;
		total_utilization=0; //NEU_UT

		//NEU_UT array for calculating individual utilization for each AC
		utilization = (float *)malloc(fleet_size*sizeof(float));
		//initialise array
		for(i=0;i<fleet_size;i++)
			utilization[i]=0;
		//NEU_UT

        for(i=0;i<fleet_size;i++)
        {
            demand=0;
            cost=0;
            route_time=0;
            item=head_route;

            while(item->ac_number!=i+1)
                item=item->next;
            start_time=item->takeoff;//first takeoff of the aircraft
                
            item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
                {
                   //calculating total demand carried
                    demand+=item->original_cargo;
                    if(item->extra_detail!=NULL)
                    {
                        
                        extra_item=item->extra_detail;
                        while(extra_item!=NULL)
                        {
                            if(extra_item->extra_cargo!=0)
                            demand+=extra_item->extra_cargo;
                            extra_item=extra_item->next_extra;
                        }    
                    }

					//NEU_UT
					//calculating the BH per AC
					if(item->takeoff!=item->landing)
						utilization[i]+=item->landing-item->takeoff;
					//NEU_UT

                    //calculating total time and cost taken for carrying this demand
                    if(item->takeoff!=item->landing)
                    {
                        route_time=item->landing;
                        prev_route_time=item->landing;
                    }    
                    else
                    {
                        route_time=prev_route_time;
                    }    
                    cost+=item->operating_cost;
                }    
                item=item->next;
            }
            fprintf(fprocess,"\n   %d\t\t%.2f\t%.2f\t%.2f",i+1,demand,cost,route_time-start_time);
            total_demand+=demand;
            total_cost+=cost;
            total_time+=(route_time-start_time);//add only the flying time
        } //end of for(i=0;i<fleet_size;i++)

		//calculation of Utilization NEU_UT
		temp_fleet=head_aircraft_list;
		for(i=0;i<fleet_size;i++)
		{
			utilization[i]=utilization[i]/temp_fleet->life;
			total_utilization+=utilization[i];
		
        //printf("\n -----------------------------%f ",temp_fleet->TAT);
         
			temp_fleet=temp_fleet->next_aircraft;
		}
		total_utilization=total_utilization/fleet_size;
		//NEU_UT

        fprintf(fprocess,"\n ------------------------------------------");
        //fprintf(fprocess,"\n Sum\t\t%.2f\t%.2f\t%.2f\t%.2e\t%.2e\t%.2e",total_demand,total_cost,total_time,(total_cost/total_demand)/total_time,(total_cost/total_demand),total_demand/total_time); 
        fprintf(fprocess,"\n Sum\t\t%.2f\t%.2f\t%.2f",total_demand,total_cost,total_time); 
        
        grey_priority=head_grey;
        for(i=0;i<total_grey_indices;i++)
        {
            switch(i)
                {
                    case 0:
                        index[0]=grey_priority->priority;//priority of demand index 
                        break;
                    case 1:
                        index[1]=grey_priority->priority;//priority of cost index 
                        break;
                    case 2:
                        index[2]=grey_priority->priority;//priority of time index 
                        break;
                        
                  /*
                     case 3:
                        index[3]=grey_priority->priority;  //priority of network design index
                        break;
                        */
                    case 3:
                        index[3]=grey_priority->priority;//priority of route priority index
                        break;
                        
                    
                    default :
                        {
                            printf("\n Error in reading grey indices priorities in write_output\n Please check the input data");
                            getch();
                            exit(1);
                        }    
                    
                }//end of switch  
         
                grey_priority=grey_priority->next_grey;
         }
         
         
        //objective_value =(index[0]*total_demand)+(index[1]/total_cost)+(index[2]/total_time);
       // objective_value =(index[0]*total_demand)-(index[1]*total_cost)-(index[2]*total_time);
        fprintf(fprocess,"\n\n Priority of Demand Index  \t= %.4f",index[0]);
        fprintf(fprocess,"\n Priority of Cost Index  \t= %.4f",index[1]);
        fprintf(fprocess,"\n Priority of Time index  \t= %.4f",index[2]);
        //fprintf(fprocess,"\n Priority of Network Design index = %.2f",index[3]);
        fprintf(fprocess,"\n Priority of Route Prio. index \t= %.4f",index[3]);
        
        //fprintf(fprocess,"\n Priority of Route priority index = %.2f",index[3]);
        //fprintf(fprocess,"\n Priority of Quality of service Index = %.2f",index[4]);
        //fprintf(fprocess,"\n\n Objective function value = %.4f",objective_value);
        /*
        system_initial_freq_req=0;
        system_freq_offered=0;
        temp_data=head_data;
        counter=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            system_initial_freq_req +=temp_data->initial_freq_req;
            if(i!=j)
            system_freq_offered+=temp_data->freq_offered;
            if(temp_data->freq_offered!=0 && i!=j)
            counter++;
            temp_data=temp_data->next_data;
        } 
        
        if(system_initial_freq_req==0)
        QoS=1.0;
        else
        QoS=((1.0 *system_freq_offered)/system_initial_freq_req)*((1.0*counter)/(AIRPORT_NO*(AIRPORT_NO-1)));
       */
         /*
        fprintf(fprocess,"\n\n");
        fprintf(fprocess,"\t%.2e",(total_cost/total_demand)/total_time);
        fprintf(fprocess,"\t%.2e",total_cost/total_demand);
        fprintf(fprocess,"\t%.2e",total_demand/total_cost);
        fprintf(fprocess,"\t%.2e",total_demand/total_time);
        fprintf(fprocess,"\t%.2e",QoS);
        fprintf(fprocess,"\t%d",counter);
       */
       temp_data=head_data;
        /*
        counter=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            system_initial_freq_req +=temp_data->initial_freq_req;
            if(i!=j)
            system_freq_offered+=temp_data->freq_offered;
            if(temp_data->freq_offered!=0 && i!=j)
            counter++;
            temp_data=temp_data->next_data;
        } 
        */
       
         OD_served=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            {
                if(temp_data->initial_freq_req!=0)
                QoS_all[i*AIRPORT_NO+j]=(1.0*temp_data->freq_offered)/temp_data->initial_freq_req;
                else
                QoS_all[i*AIRPORT_NO+j]=1.0;
                if(temp_data->freq_offered!=0)
                OD_served++;
                
            }   
            else
            QoS_all[i*AIRPORT_NO+j]=0;
            
            temp_data=temp_data->next_data; 
        }    
        //if(system_initial_freq_req==0)
        //QoS=1.0;
        //else
        //QoS=((1.0 *system_freq_offered)/system_initial_freq_req)*((1.0*counter)/(AIRPORT_NO*(AIRPORT_NO-1)));
        sum_QoS=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            sum_QoS+=QoS_all[i*AIRPORT_NO+j];
        }    
        ave_QoS=sum_QoS/(AIRPORT_NO*(AIRPORT_NO-1));
        
        
        sigma_variance=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            sigma_variance+=((QoS_all[i*AIRPORT_NO+j]-ave_QoS)*(QoS_all[i*AIRPORT_NO+j]-ave_QoS));
        }    
        
        variance_QoS=sigma_variance/(AIRPORT_NO*(AIRPORT_NO-1));
       
       temp_data=head_data;
       remaining_demand=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
            {
                remaining_demand+=temp_data->demand;
                temp_data=temp_data->next_data;
            }
            
        fprintf(fprocess,"\n\n Tons/Lakhs Rs \t= %.4f",total_demand/total_cost);
        fprintf(fprocess,"\n Tons/Hour \t= %.4f",total_demand*60/total_time);
        fprintf(fprocess,"\n Variance QOS \t= %.4f",variance_QoS);
		fprintf(fprocess,"\n\n Fleet Utilization [%%] \t= %.3f",total_utilization*100);//NEU_UT
        fprintf(fprocess,"\n No. of OD pair served \t= %d",OD_served);
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Network Schedule");
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Flight Ac     Type  Source Takeoff Destination Landing Payload	Empty	Cost");
        item=head_route;
        while(item!=NULL)
        {
		airport=head_airport;
			while(item->sr_index!=airport->ap_index)
				airport=airport->next_airport;
    

		airport1=head_airport;
			while(item->desti_index!=airport1->ap_index)
				airport1=airport1->next_airport;




            fprintf(fprocess,"\n %d \t%d \t%s  %s     %.2f\t%s\t%.2f\t%.2f\t%.2f\t%.2f ",item->flight_no,item->ac_number,item->ac_code,airport->name,item->takeoff,airport1->name,item->landing,item->cargo_carried,item->residual_capacity,item->operating_cost);//NEU_MAC (+ac_type)
            item=item->next;
        }
        
        //checking if their any extra carried detail in the network
        flag=NO;
        item=head_route;
        while(item!=NULL)
        {
            if(item->extra_detail!=NULL)
            {
                extra_item=item->extra_detail;
                while(extra_item!=NULL)
                {
                    if(extra_item->extra_cargo!=0)
                    {
                        fprintf(fprocess,"\n\n Extra Carried demand detail -");
                        fprintf(fprocess,"\n [ Flight Ac\tExtra_Cargo for Destination ]");
                        flag=YES;
                        break;
                    } 
                    extra_item=extra_item->next_extra;
                }   //end of  while(extra_item!=NULL)
                if(flag==YES)
                break;
            }   //end of if(item->extra_detail!=NULL)
            item=item->next;
        }//end of while(item!=NULL)
        //printing the extra carried detail
        item=head_route;
        while(item!=NULL)
        {
            if(item->extra_detail!=NULL)
            {
                
                extra_item=item->extra_detail;
                while(extra_item!=NULL)
                {
						airport1=head_airport;
			while(extra_item->desti_index!=airport1->ap_index)
				airport1=airport1->next_airport;
                    if(extra_item->extra_cargo!=0)
                    fprintf(fprocess,"\n [   %d\t  %d\t%.2f\t\t%s\t ]",item->flight_no,item->ac_number,extra_item->extra_cargo,airport->name);
                    extra_item=extra_item->next_extra;
                }    
            }   
            item=item->next;
        }   
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Aircraft Schedule"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        for(i=0;i<fleet_size;i++)
        {
			fprintf(fprocess,"\n\n\n For Aircraft %d)\n",i+1);
            fprintf(fprocess,"\n Flight Type   Source Takeoff  Destination Landing Payload\tEmpty\tCost");
			item=head_route;
            while(item!=NULL)
            {
						airport=head_airport;
			while(item->sr_index!=airport->ap_index)
				airport=airport->next_airport;
    

		airport1=head_airport;
			while(item->desti_index!=airport1->ap_index)
				airport1=airport1->next_airport;

                if(item->ac_number==i+1)
					fprintf(fprocess,"\n %d\t%s\t%s\t%-7.2f\t%s\t   %-7.2f   %-7.2f\t%-7.2f\t%-7.2f ",item->flight_no,item->ac_code,airport->name,item->takeoff,airport1->name,item->landing,item->cargo_carried,item->residual_capacity,item->operating_cost);
				item=item->next;
            }
			fprintf(fprocess,"\n Utilization [%%] = %.3f",utilization[i]*100);	//NEU_UT
            //checking if their any extra carried detail for the i+1th aircraft
            flag=NO;
            item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
                {
                    
                    if(item->extra_detail!=NULL)
                    {
                        extra_item=item->extra_detail;
                        while(extra_item!=NULL)
                        {
                            if(extra_item->extra_cargo!=0)
                            {
                                fprintf(fprocess,"\n\n Extra Carried demand detail -");
                                fprintf(fprocess,"\n [ Flight\tExtra_Cargo for \tDestination ]");
                                flag=YES;
                                break;
                            } 
                            extra_item=extra_item->next_extra;
                        }
                        if(flag==YES)
                        break;       
                    } //end of if(item->extra_detail!=NULL)           
                }    //end of if(item->ac_number==i+1)
                item=item->next;
            }  //end of while(item!=NULL)             
            //printing the extra carried detail
            item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
                {
                    
                    if(item->extra_detail!=NULL)
                    {
                        
                        extra_item=item->extra_detail;
                        while(extra_item!=NULL)
                        {
                        	airport1=head_airport;
			while(extra_item->desti_index!=airport1->ap_index)
				airport1=airport1->next_airport;
							if(extra_item->extra_cargo!=0)
                            fprintf(fprocess,"\n [   %d\t\t   %-7.2f\t\t   %s\t    ]",item->flight_no,extra_item->extra_cargo,airport->name);
                            extra_item=extra_item->next_extra;
                        }    
                    }   
                }    //end of if(item->ac_number==i+1)
                item=item->next;
            }  //end of while(item!=NULL) 
            fprintf(fprocess,"\n ------------------------------");
            
        }  //end of for(i=0;i<fleet_size;i++)  
        
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Remaining Demand"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n\n");
        temp_data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess," %.2f\t",temp_data->demand);
                temp_data=temp_data->next_data;
            }
            fprintf(fprocess,"\n");
        }
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Remaining Demand = %.2f",remaining_demand);
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Frequency of operation between OD pairs"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n\n");
        
        temp_data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            for(j=0;j<AIRPORT_NO;j++)
            {
                if(i==j)
                fprintf(fprocess," %d\t",0);
                else
                fprintf(fprocess," %d\t",temp_data->freq_offered);
                temp_data=temp_data->next_data;
            }    
            fprintf(fprocess,"\n");
        }    
        
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Quality of Service between OD pairs"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n\n");
        for(i=0;i<AIRPORT_NO;i++)
        {
            for(j=0;j<AIRPORT_NO;j++)
            {
                
                fprintf(fprocess," %.2f\t",QoS_all[i*AIRPORT_NO+j]);
                
            }    
            fprintf(fprocess,"\n");
        }    
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        /*
        fprintf(fprocess,"\n Sum of QoS = %f",sum_QoS);
        fprintf(fprocess,"\n Ave of QoS = %f",ave_QoS);
        fprintf(fprocess,"\n sigma of variance in QoS = %f",sigma_variance);
        fprintf(fprocess,"\n variance of QoS = %f",variance_QoS);
        */
        fclose(fprocess);

		free(utilization);//NEU_BH
		free(QoS_all);//NEU_VLA  
    
}//end of function    

//#include"cargo_schedulling.h"
long int all_trials_counter; //global variable
int best_case;
double min_objective_value;
double best_x[total_grey_indices];//no. of variables = total_grey_indices (declared in include_files.h)
//NEU_UT old:double best_obj_value[4];
double best_obj_value[5];//NEU_UT

/********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#ifdef __alpha
#include <sys/time.h>
#include <sys/resource.h>
#else
#ifdef __sun
#include <sys/time.h>
#include <sys/resource.h>
extern int getrusage( int who, struct rusage *rusage);
#else
#include <time.h>
#endif
#endif

#define maxn 25
#define popsize 250
#define storesize 20000
#define goup ((double)evals/(double)maxevals)
#define godown (1-(double)evals/(double)maxevals)
#define sqr(x) ((x)*(x))

void reseed();
void displaypop();
extern double drand48(); 
extern void srand48(long int rseedy); 

typedef struct individual 
{
  double       r[maxn+1];
  double       fitness;
  double       mom;
  double       violation;
  double       norm;
  char         cause;
}
individual;

/* Changes carried out for Windows Version of the code */


////////////////////////


double drand48(void);


#define C 16807
#define A 2147483647.0
double yz;

#pragma CODE_SECTION(srand48,  "xcode");
#pragma CODE_SECTION(drand48,  "icode");

extern void srand48(long int seed) {
yz= (double) seed;
} 

double drand48() {
long int ki;
double uu;

ki=(C* yz)/A;
yz=C* yz-ki*A;
uu=yz/(A-1);
return uu;
}

///////////////////////////

static individual pop[popsize+1],store[storesize+1],
  newborn,best,fittest,center;
static char ga_start[300]="",ga_end[300]="";
static int n,maxpop=100,maxstore=1000,minevalpop=10,maxevals=10000,
  maxevals_increment=1000,nnofset=0,nnk=2, maxtry=1000000,evals=0,
  succevals=0,feasevals=0,ins=0,outs=0,redundants=0,    
  population=0,stored=0, addnewborn=0,
  candidate1, candidate2, trycount, reseedcount=0,
  maxreseeds=20,no_screen=0,in_reseed=0,no_penalty_decrease=1,
  easy_start=1,gready_finish=1,nonlinear_penalty=0;
static double verybig=1e100, commonvar1=0,commonvar2=0,
  minr[maxn+1],maxr[maxn+1],sclr[maxn+1], nndist,
  currentradius1,currentradius2,currentradius,initialradius=0,
  reseedfraction=0.25,mutation_factor=1,GC_factor=0.2,GC_scale=1,GC_shrink=1,
  penaltycoef=1,penaltymult=2,penaltypower=1, rejecttolerance=1,
  stoptolerance=0.001,initialcrowdingfactor=0.9,finalcrowdingfactor=0;
  /* =========================================== */
enum {mode_read, mode_write, mode_append};
FILE* myfopen(char* f_name, int mode)
{
    char* mode_str;
    FILE* fp;

    if(mode == mode_read)
      mode_str = "r";
    else if(mode == mode_write)
      mode_str = "w";
    else mode_str = "a";
    fp = fopen(f_name, mode_str);

    if (fp == NULL)
    {
      if(mode == mode_read)
	mode_str = "read";
      else if(mode == mode_write)
	mode_str = "write";
      else mode_str = "append";
    
      printf("Open of %s for %s failed\n", f_name,
		 mode_str);
		 getch();
      exit(1);
    }
    return(fp);
}

void display(individual b)
{
  int i;
  
  printf("--- %.10g  (%.10g %.10g) ",b.fitness,b.violation,b.mom);
  for(i=1;i<=n;i++) printf("%.20g ",b.r[i]*sclr[i]);
  printf("& %d %d %d %d %c %d %d %d\n",
	 evals,succevals,feasevals,trycount,b.cause,ins,outs,redundants);
  fflush(stdout);
}

double distance(individual x,individual y)
{  
  int i;
  double temp=0;
 
  for(i=1;i<=n;i++)
    temp+=sqr(x.r[i]-y.r[i]);
/*    temp+=sqr((x.r[i]-y.r[i])/(maxr[i]-minr[i])); */
  return sqrt(temp); 
}

int selectgood(int p)
{
  int chosen;
  double s,m;

  if((easy_start)&&(goup<0.25)) 
    chosen= 1+floor(drand48()*(p-1));   /* no pressure */
  else
    if((gready_finish)&&(goup>0.75))
      chosen=1+floor(drand48()*drand48()*(p-1));   /* high pressure */
  else                               /* medium pressure */
    {
      s=drand48()*p*(p+1)/2.0;
      m=(2*p+1-sqrt(sqr(2*p+1)-8*s))/2.0;
      if(m<=0) chosen=1; else chosen=ceil(m);
    }
  return chosen;
}

double radius(void)
{
  double temp1=0,temp2=0;
  int i,j,count=0;
  
  for(i=1;i<=n;i++) center.r[i]=0;
  for(j=1;j<population;j++)
    /*    if(pop[j].fitness<verybig)  */
      {
	count+=1;
	for(i=1;i<=n;i++) center.r[i]+=pop[j].r[i];
	temp2+=distance(pop[1],pop[j]);
      }
  if(count<=0) count++;
  for(i=1;i<=n;i++) center.r[i]/=count;
  for(j=1;j<population;j++)
    /*    if(pop[j].fitness<verybig)  */
      {
	temp1+=distance(center,pop[j]);
      }
  temp1/=count;
  temp2/=count;
  currentradius1=temp1;
  currentradius2=temp2;
 
  return (temp1+temp2)/2;
}

int is_evaluable(individual x)
{
  return (x.fitness<verybig);
}
int is_feasible(individual x)
{
  return ((x.fitness<verybig)&&(x.violation==0));  
}

double get_fitness(double x, double y) 
{ 
  return (x+penaltycoef*pow(y,penaltypower));
}

void recomputefitness()
{
  int i,j;
  individual temp;
  double proposed;
  
  if(fittest.fitness>=best.fitness) 
    {
      if(no_penalty_decrease) return;
      j=0;
      for(i=1;i<population;i++) if(!(is_feasible(pop[i]))) j=1;
      if((j)||(penaltycoef<=0)) 
	return; 
      else penaltycoef/=sqrt(sqrt(penaltymult));
    }
  else
    {  
      proposed=(best.mom-fittest.mom)/
	(pow(fittest.violation,penaltypower)-pow(best.violation,penaltypower));
      if(proposed<=penaltycoef) return;
      if(proposed>penaltymult*penaltycoef) 
	penaltycoef*=penaltymult;
      else penaltycoef=proposed;
    }
  
  for(i=1;i<=population;i++)
    {
      pop[i].fitness=get_fitness(pop[i].mom,pop[i].violation);
      temp=pop[i];
      j=i-1;
      while (((j > 0) && (pop[j].fitness > temp.fitness)))
	{
	  pop[j + 1] = pop[j];
	  j-=1;
	}
      if(j<i-1) pop[j+1]=temp;
    }
  best.fitness=get_fitness(best.mom,best.violation);
  fittest=pop[1];
  printf("*** Penaly Coef=%g\n",penaltycoef);
}

double       ftness(individual   x,
		    double (*func)(double []),
		    double (*penalty)(double []))
{  
  double temp1,temp2;
  int   will_display=1;

  temp1=func(x.r);
  if((temp1>=1e10)||(temp1<=-1e10)) temp1=verybig;
  commonvar1=temp1; 
  temp2=penalty(x.r);
  if(nonlinear_penalty)
    {
      if(temp2<1) temp2=sqrt(temp2); else temp2=sqr(temp2);
    }
  if(temp1==verybig) temp2=temp1;
  commonvar2=temp2; 
  temp2=get_fitness(temp1,commonvar2);
  evals+=1;
  if(temp2<verybig) succevals+=1;
  if((temp1==temp2)&&(temp1!=verybig)) feasevals+=1;
  if ((commonvar2<best.violation)
      ||((temp2<best.fitness)&&(commonvar2==best.violation)))
    {
      /* printf("## %.15g %.15g %.15g
	 %.15g\n",temp2,best.fitness,best.fitness-temp2,distance(x,best)); */
      printf("+++");
      if (temp2<fittest.fitness)
	{
	  printf("&&&");
	  will_display=0;
	}
      best = x;
      best.mom=commonvar1;
      best.violation=commonvar2;
      best.fitness=temp2;
      display(best);
    }
  if (temp2<fittest.fitness)
    {
      fittest = x;
      fittest.mom=commonvar1;
      fittest.violation=commonvar2;
      fittest.fitness=temp2;
      if(will_display) display(fittest);
    }
  if (((evals % (2*maxpop))==0)&&(evals>0))
    recomputefitness();
  
  if ((evals % 100)==0)
    {
#ifdef __alpha
      struct rusage r_usage;
#endif
#ifdef __sun
      struct rusage r_usage;
#endif
      printf("$$$ %d %.5g (%.5g %.5g) %.5g (%.5g %.5g)",evals,best.fitness,best.violation,best.mom,fittest.fitness,fittest.violation,fittest.mom);
#ifndef __alpha
#ifndef __sun
      printf(" CPU time = %f\n",(double)clock()/CLOCKS_PER_SEC);
#endif
#endif
#ifdef __alpha
      getrusage(RUSAGE_SELF,&r_usage);
      printf(" CPU:opt= %f  ",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
      getrusage(RUSAGE_CHILDREN,&r_usage);
      printf("sim = %f\n",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
#endif
#ifdef __sun
      getrusage(RUSAGE_SELF,&r_usage);
      printf(" CPU:opt= %f  ",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
      getrusage(RUSAGE_CHILDREN,&r_usage);
      printf("sim = %f\n",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
#endif
      {
	currentradius=radius();
	printf("### RADII %.10g %.10g %.10g %.10g\n",
	       currentradius1,currentradius2,currentradius,
	       reseedfraction*initialradius*godown*godown);
	if((currentradius<reseedfraction*initialradius*godown*godown)
	   &&(reseedcount<maxreseeds))
	  reseed();
      }
      fflush(stdout);
      if ((evals % 1000)==0) displaypop();
    }
  return temp2;
}
  /* ============================================ */
int knearest_neighbor(individual x, int k)
{
  int i,j,buffer[storesize],buffersize=0;
  double d,tempdist[storesize],tempfit,tempfit2;
  
  for(i=stored;i>=stored-k+1;i--)
    {
      d=distance(x,store[i]);
      j=buffersize;
      while((j > 0) && (tempdist[j]>d))
      {
	buffer[j+1]=buffer[j];
	tempdist[j+1]=tempdist[j];
	j-=1;
      }
      buffer[j+1]=i;
      tempdist[j+1]=d;
      buffersize++;
    }

  for(i=stored-k;i>=1;i--)
    {
      if((store[i].norm<x.norm+tempdist[k])
	 &&(store[i].norm>x.norm-tempdist[k]))
	if((d=distance(x,store[i]))<tempdist[k])
	  {
	    j=k;
	    while((j > 0) && (tempdist[j]>d))
	      {
		buffer[j+1]=buffer[j];
		tempdist[j+1]=tempdist[j];
		j-=1;
	      }
	    buffer[j+1]=i;
	    tempdist[j+1]=d;
	  }
    }

  i=buffer[1];
  nndist=tempdist[1];
  tempfit=get_fitness(store[i].mom,store[i].violation);
  for(j=2;j<=buffersize;j++)
    {
      int z=buffer[j];
      tempfit2=get_fitness(store[z].mom,store[z].violation);
      if(tempfit2<tempfit)
      {
	i=z;
	tempfit=tempfit2;
      }
    }
  return i;
}

int screens(individual x)
{/* machine_learning_dicision function */
  int nn;
  double tempfit,rad=rejecttolerance*initialradius*godown*godown/population; 
  double threshold;
  
  if(no_screen) return 1;
  if(evals<maxevals/4) return 1; /* too early to rely on history */
  if(drand48()<0.01) return 1;   /* to prevent infinite looping */
  nn=knearest_neighbor(x,nnk);
  if(nndist<=rad) {redundants+=1;return 0;}  /* redundant point */  
  tempfit=get_fitness(store[nn].mom,store[nn].violation);
  if(nnofset>0) threshold=pop[population-nnofset].fitness;
  else threshold=godown*pop[population].fitness
	 +goup*pop[population-1].fitness;
  return 
    (tempfit<threshold); 
}

double ga_gradient(individual x,individual y)
{
  double d;
  
  if((d=distance(x,y))<=1e-20) return -1;
  else return fabs(y.fitness-x.fitness)/d;
}

void displaypop()
{  
  int i,j; 
  FILE *IF;
  
  if(!(strcmp(ga_end,"NONE"))) return;
  IF=myfopen(ga_end,mode_append);
  fprintf(IF,"%d\n",population);    
  for(i=population;i>=1;i--)
    {
      for(j=1;j<=n;j++) fprintf(IF,"%.20g ",pop[i].r[j]*sclr[j]);
      fprintf(IF,"%.20g %.20g\n",pop[i].mom,pop[i].violation);
    }
  fclose(IF); 
}

  /* ============================================ */
void         introduce(individual   x)
{
  int      i,replaced;
  double replaceddistance;
  
  if(!(in_reseed))
    if(stored<maxstore)
      {
	stored+=1;
	store[stored]=x;
      }
    else
      {
	replaced=1 + drand48()*(stored - 1);
	store[replaced]=x;
      }
  
  if(population>=maxpop)
    { int limit;
    
      if(!(is_evaluable(x))) return; /* unevaluable */
      limit=maxpop*(1-(godown*initialcrowdingfactor+goup*finalcrowdingfactor));
      replaced=maxpop;
      replaceddistance=distance(x,pop[replaced]);
      if(!((pop[replaced].fitness>=verybig)))
	for(i=maxpop-1;((i>limit)&&(pop[i].fitness>x.fitness));i--)
	  if(distance(x,pop[i])<replaceddistance)
	    {  
	      replaced=i;
	      replaceddistance=distance(x,pop[i]);
	    } 
      i=replaced;
      if((pop[i].violation<=best.violation)&&(pop[i].mom<=best.mom))
	{   /****** never replace the best most feasible point ****/
	  if(i>=population) i--;
	  else i=population;
	}
      while(i<maxpop)
	{
	  pop[i]=pop[i+1]; 
	  i++;
	}
      population-=1;
    }
  i = population;
  while (((i > 0) && (pop[i].fitness > x.fitness)))
      {
	pop[i + 1] = pop[i];
	i-=1;
      }
  pop[i + 1] = x;
  population+=1;
}

  /* ============================================ */

double       bounded(double       min,
                     double       max)
{
  
  return min + drand48() * (max - min);
  
}

  /* ============================================ */

double       variation(int index,
		       char         typec,
		       double       current,
                       double       min,
                       double       max)
{
  double       temp,amount,size,selector;

  if(typec=='P') size=0.15*mutation_factor;
  else if(typec=='R') size=0.10*mutation_factor;
  else size=0.05*mutation_factor;
  
  selector=drand48();
  if(selector<0.1)
    { /*********** gready mutation ***********/
      if(drand48()>0.5) /*** up mutation ****/
	amount=(max-current)*drand48()*drand48();
      else /*** down mutation ****/
	amount=-(current-min)*drand48()*drand48();
      temp=current+amount;      
    }
  else if(selector<0.4)
    {  /*********** Shrinking window mutation ***********/ 
      amount=0.5*size*godown*(max-min);
      temp = bounded(current-amount,current+amount);
      if (temp < min)
	temp = min;  
      else
	if (temp > max) 
	  temp = max; 
    }
  else
    { /*********** Non Uniform mutation ***********/
      if(drand48()>0.5) /*** up mutation ****/
	amount=(max-current)*size*godown*drand48();
      else /*** down mutation ****/
	amount=-(current-min)*size*godown*drand48();
      temp=current+amount;
    }
  return temp;
}

  /* ============================================ */

void generate(double (*func)(double []),double (*penalty)(double []))
{
  individual   indbuf;
  int i,starts=0,flag=0;
  FILE *IF;
  double dum;
  
  if(strcmp(ga_start,"NONE"))
    {   
      IF=myfopen(ga_start,mode_read);
      fscanf(IF,"%d",&starts);
      printf("Reading %d points from %s\n",starts,ga_start);
      if(0==starts) fclose(IF); else flag=1;
    }
  population = 0;
  do 
    {
      if(evals<starts)
	{
	  for(i=1;i<=n;i++)
	    {
	      fscanf(IF,"%lf",&indbuf.r[i]);
	      indbuf.r[i]/=sclr[i];
	    }
	  fscanf(IF,"%lf",&dum);
	  fscanf(IF,"%lf",&dum);
	}
      else
	{
	  for(i=1;i<=n;i++)
	    indbuf.r[i] = bounded(minr[i],maxr[i]); 
	}
      indbuf.cause='Z';
      indbuf.fitness = ftness(indbuf,func,penalty);
      indbuf.mom=commonvar1;
      indbuf.violation=commonvar2;
      indbuf.norm=0;
      for(i=1;i<=n;i++) indbuf.norm+=sqr(indbuf.r[i]);
      indbuf.norm=sqrt(indbuf.norm);
      introduce(indbuf);
      if(is_evaluable(indbuf))
	{ 
	  printf("%g %d %d %d\n",indbuf.fitness,population,evals,succevals);
	  fflush(stdout);
	}
    }
  while (!((population>=maxpop)&&(evals>=minevalpop)));
  printf("end of random explore phase\n");
  initialradius=currentradius=radius(); 
  printf("Initial radius=%.10g\n",initialradius);
  if(flag) fclose(IF);
  fflush(stdout);
}
  /* ============================================ */
void         reseed()
{
  int i;
  
  if((goup>0.95)&&(gready_finish)) return;
  in_reseed=1;
  for(i=1;i<=stored;i++) 
    store[i].fitness=get_fitness(store[i].mom,store[i].violation);

  reseedcount+=1;
  population = 2;
  pop[2]=best; /****** never lose the best most feasible point ***/
  
  for(i=stored;i>=1;i--)
    if(distance(store[i],pop[1])>2*reseedfraction*initialradius*godown*godown) 
      introduce(store[i]);
  currentradius=radius();  
  printf("end of reseed phase:Population=%d RADII %.10g %.10g %.10g %.10g\n",
	 population,currentradius1,currentradius2,currentradius,
	 reseedfraction*initialradius*godown*godown);
  fflush(stdout);
  in_reseed=0;
}
  /* ============================================ */
void   guidedcrossover(individual   x,
                       individual   y,
		       double (*func)(double []),
		       double (*penalty)(double []))
{
  double   ratio,size;
  int i;
  
  size=GC_scale*0.5*(GC_shrink+(1-GC_shrink)*godown);
  ratio = bounded(-size,0.2*size);
  newborn.cause='G';
  for(i=1;i<=n;i++)
    newborn.r[i] = ratio * x.r[i] + (1 - ratio) * y.r[i];
      
  for(i=1;i<=n;i++)
    {
      if (newborn.r[i]<minr[i]) newborn.r[i]=minr[i]; 
      if (newborn.r[i]>maxr[i]) newborn.r[i]=maxr[i];
    }
  newborn.norm=0;
  for(i=1;i<=n;i++) newborn.norm+=sqr(newborn.r[i]);
  newborn.norm=sqrt(newborn.norm);
/* &&& machine learning decision */
  addnewborn=0;

  if(screens(newborn))
    { 
      newborn.fitness = ftness(newborn,func,penalty);
      newborn.mom=commonvar1 ;
      newborn.violation=commonvar2 ;
      addnewborn=1;
    }
}
	
  /* ============================================ */
void   crossover(individual   x,
                       individual   y,
		       double (*func)(double []),
		       double (*penalty)(double []))
{ 
  int i,crosspoint;
  double selector=drand48();


  if(selector<0.25)    /*************** Line crossover *************/
    { double ratio;   
      newborn.cause='L';
      if(x.fitness<y.fitness) 
	ratio = bounded(-2,3+2*goup); 
      else ratio = bounded(-2-2*goup,3);
      
      for(i=1;i<=n;i++)
	{    
	  newborn.r[i] = ratio * x.r[i] + (1 - ratio) * y.r[i];
	}
    }
  else if(selector<0.5) /********** Point crossover  **********/
    {
      newborn.cause='P';    
      crosspoint=floor(bounded(2,n));
     
      for(i=1;i<crosspoint;i++)
	newborn.r[i] = x.r[i];
      newborn.r[crosspoint]=
	(drand48()<0.5)? x.r[crosspoint]:y.r[crosspoint];
      for(i=crosspoint+1;i<=n;i++)
	newborn.r[i] = y.r[i];
    }
  else if(selector<0.75) /********** double linear crossover  **********/
    { double ratio1,ratio2;
    
      newborn.cause='D';    
      crosspoint=floor(bounded(2,n));
      ratio1 = bounded(-2,3); 
      ratio2 = bounded(-2,3); 
      for(i=1;i<=crosspoint;i++)
	  newborn.r[i] = ratio1 * x.r[i] + (1 - ratio1) * y.r[i];	

      for(i=crosspoint+1;i<=n;i++)
	  newborn.r[i] = ratio2 * x.r[i] + (1 - ratio2) * y.r[i];
    }
  else  /********** random crossover  **********/
    {
      newborn.cause='R';    
      
      for(i=1;i<=n;i++)
	newborn.r[i]=(drand48()<0.5)? x.r[i]:y.r[i];
    }

  for(i=1;i<=n;i++)
    {    
      outs+=1;
      if (newborn.r[i]<minr[i])
	newborn.r[i]=minr[i];
      else
	if (newborn.r[i]>maxr[i])
	  newborn.r[i]=maxr[i];    
	else
	  {
	    outs-=1;
	    ins+=1;
	  }
    }
  /******************** mutation *****************/  
  for(i=1;i<=n;i++)
    if(drand48()<0.4*godown*godown+0.1)
      newborn.r[i]=variation(i,newborn.cause,newborn.r[i],minr[i],maxr[i]); 

  newborn.norm=0;
  for(i=1;i<=n;i++) newborn.norm+=sqr(newborn.r[i]);
  newborn.norm=sqrt(newborn.norm);
/* &&& machine learning decision */
  addnewborn=0;
  if(screens(newborn))
  {
    newborn.fitness = ftness(newborn,func,penalty);
    newborn.mom=commonvar1 ;
    newborn.violation=commonvar2 ;
    addnewborn=1;    
  }
}


  /* ============================================ */
double ga(int global_n, double *scales, double *x, double *bl, double
*bu, char *seed_file, char *ga_input, double (*func)(double []), double (*penalty)(double []))//,double *best_x_in_ga) 
//last variable in function call is newly added to store the value of design variables for best case
{
  int i;
  FILE *IF;
  long int rseed;
  
  n=global_n;    /*** important link ******/
  
  IF=myfopen(seed_file,mode_read);
  fscanf(IF,"%ld",&rseed);
  fclose(IF);

  IF=myfopen(ga_input,mode_read);
  fscanf(IF,"maxpop=%d\n",&maxpop);
  fscanf(IF,"maxstore=%d\n",&maxstore);
  fscanf(IF,"minevalpop=%d\n",&minevalpop);
  fscanf(IF,"maxevals=%d\n",&maxevals);
  fscanf(IF,"maxevals_increment=%d\n",&maxevals_increment);
  fscanf(IF,"penaltycoef=%lf\n",&penaltycoef);
  fscanf(IF,"penaltymult=%lf\n",&penaltymult);
  fscanf(IF,"penaltypower=%lf\n",&penaltypower);
  fscanf(IF,"stoptolerance=%lf\n",&stoptolerance);
  fscanf(IF,"rejecttolerance=%lf\n",&rejecttolerance);
  fscanf(IF,"maxreseeds=%d\n",&maxreseeds);
  fscanf(IF,"reseedfraction=%lf\n",&reseedfraction);
  fscanf(IF,"mutation_factor=%lf\n",&mutation_factor);
  fscanf(IF,"GC_factor=%lf\n",&GC_factor);
  fscanf(IF,"GC_scale=%lf\n",&GC_scale);
  fscanf(IF,"GC_shrink=%lf\n",&GC_shrink);
  fscanf(IF,"Nonlinear penalty=%d\n",&nonlinear_penalty);
  fscanf(IF,"No penalty decrease=%d\n",&no_penalty_decrease);
  fscanf(IF,"Easy start=%d\n",&easy_start);
  fscanf(IF,"Gready finish=%d\n",&gready_finish);
  fscanf(IF,"No Screen=%d\n",&no_screen);
  fscanf(IF,"K for Knearest neighbor=%d\n",&nnk);
  fscanf(IF,"screening offset=%d\n",&nnofset);
  fscanf(IF,"initial_crowding_factor=%lf\n",&initialcrowdingfactor);
  fscanf(IF,"final_crowding_factor=%lf\n",&finalcrowdingfactor);
  fscanf(IF,"Starting point file=%s\n",ga_start); 
  fscanf(IF,"Ending point file=%s",ga_end); 
  fclose(IF);

  printf("Random seed=%ld\n",rseed);
  printf("Population size=%d\n",maxpop);
  printf("Store size=%d\n",maxstore);
  printf("Mimimum # of pop evals=%d\n",minevalpop);
  printf("Maximum # of Evaluations=%d\n",maxevals);
  printf("Evaluations increment over maximum=%d\n",maxevals_increment);
  printf("penaltycoef=%g\n",penaltycoef);
  printf("penaltymult=%g\n",penaltymult);
  printf("penaltypower=%g\n",penaltypower);
  printf("stoptolerance=%g\n",stoptolerance);
  printf("rejecttolerance=%g\n",rejecttolerance);
  printf("maxreseeds=%d\n",maxreseeds);
  printf("reseedfraction=%g\n",reseedfraction);
  printf("mutation_factor=%g\n",mutation_factor);
  printf("GC_factor=%g\n",GC_factor);
  printf("GC_scale=%g\n",GC_scale);
  printf("GC_shrink=%g\n",GC_shrink);
  printf("Nonlinear penalty=%d\n",nonlinear_penalty);
  printf("No penalty decrease=%d\n",no_penalty_decrease);
  printf("Easy start=%d\n",easy_start);
  printf("Gready finish=%d\n",gready_finish);
  printf("No Screen=%d\n",no_screen);
  printf("K for Knearest neighbor=%d\n",nnk);
  printf("screening offset=%d\n",nnofset);
  printf("initial_crowding_factor=%g\n",initialcrowdingfactor);
  printf("final_crowding_factor=%g\n",finalcrowdingfactor);
  printf("Starting point file=%s\n",ga_start);
  printf("Ending point file=%s\n",ga_end);
  fflush(stdout);
  
  srand48(rseed);
  for(i=1;i<=n;i++) 
    {
      minr[i]=bl[i-1];
      maxr[i]=bu[i-1];
      sclr[i]=scales[i-1];
    }
  
  best.fitness=best.mom=best.violation=verybig;
  fittest.fitness=fittest.mom=fittest.violation=verybig;

  generate(func,penalty);
  
  for (trycount = 1;
       ((trycount<=maxtry)&&(evals<=maxevals)
	&&(currentradius>stoptolerance*initialradius)); trycount++)
    {
      int j;
      double maxgradient,tempgradient;
      
      if(drand48()<GC_factor*goup)
	{
          candidate1 = selectgood(population);
          candidate2 = (candidate1==1)? 2 : 1;
	  maxgradient=ga_gradient(pop[candidate1],pop[candidate2]);
	  if((goup<0.95)||(!(gready_finish)))
	    for(j=1;j<population;j++)
	      if((j!=candidate1)&&
		 ((tempgradient=ga_gradient(pop[candidate1],pop[j]))
		  >maxgradient))
		{
		  candidate2 = j;
		  maxgradient=tempgradient;
		}
	  if(pop[candidate1].fitness<=pop[candidate2].fitness)
	    guidedcrossover(pop[candidate2],pop[candidate1],
			    func,penalty); 
	  else
	    guidedcrossover(pop[candidate1],pop[candidate2],
			    func,penalty); 
        }      
      else
        {  
          candidate1 = selectgood(population);
	  do
	    {
	      candidate2 = selectgood(population);
	    }
	  while(candidate1==candidate2);
	  crossover(pop[candidate1],pop[candidate2],func,penalty);
	}
      
/* &&& machine learning decision */
      if(addnewborn!=1) 
	continue;    
      introduce(newborn);      
      if((evals>=maxevals)&&(currentradius>stoptolerance*initialradius))
      {
	maxevals+=maxevals_increment;
	rejecttolerance=0;
	/* no_screen=1; */
	reseedcount=maxreseeds;
      }
    }
  
  for(i=1;i<=n;i++) x[i]=best.r[i];
  //new added loop to store the values of design variables for best case
  //for(i=1;i<=n;i++) best_x_in_ga[i]=best.r[i];
  displaypop();
  return best.fitness;
}
/******************* main interface  **********************/
double objective_function(double x[1+maxn])
{
   //NEU_UT old: double obj_value[4];
   double obj_value[5];//NEU_UT
   double design_x[total_grey_indices];
   double sum_of_variables;
   double output_value;
   int i;
   FILE *fp;
     int j;
    
   //NEU 
   for(i=0;i<total_grey_indices;i++)
   design_x[i]=x[i+1];//since our variable in x[1,2,3]
   sum_of_variables=1;
   //alter Code Anfang
   /*sum_of_variables=0;
   for(i=0;i<total_grey_indices;i++)
   sum_of_variables+=design_x[i];*/
   //alter Code Ende
   double dSumPercent=0;
   for(i=0;i<total_grey_indices;i++)
   dSumPercent+=design_x[i];
   for(i=0;i<total_grey_indices;i++)
   design_x[i]=design_x[i]/dSumPercent;
   //NEU 
   
   if(sum_of_variables==1)//constraint in our program
   {
       //Cargo_scheduling(design_x,3,obj_value,NO);//NO means don't print anything
       Cargo_scheduling(design_x,total_grey_indices,obj_value,YES);//YES means  print schedule
       //-----------------description of the array returned from cargo_scheduling function--------------------------//
       //obj_value[0]= demand
       //obj_value[1]=cost
       //obj_value[2]=time
       //obj_value[3]=QoS_variance
       //objective value is minimized in GADO's GA code
       //to convert to maximization problem reverse your objective function
       //-----------------------------different objective function--------------------------------------//
       
        switch(objective_pointer)
        {
        case 1:
        output_value= 1/(obj_value[0]); // min- the reverse of demand i.e. max the demand
        break;
        case 2:
        output_value= obj_value[1]; // min- the total operating cost
        break;
        case 3:
        output_value= 1/(obj_value[2]); // min- the reverse of time i.e. max time i.e. max ac utilization
        break;
        case 4:
        output_value = obj_value[3]; // min. - the variance in QoS i.e. improving the QoS
        break;
        case 5:
        output_value = obj_value[1]/obj_value[0]; // min. - cost per demand
        break;
        case 6:
        output_value=obj_value[2]/obj_value[0];  // min. - time per demand
        break;
		//NEU_UT
		case 7:
		output_value=1/(obj_value[4]);  //min the reverse of Utilization
		break;//NEU_UT
        case 8:
        output_value= obj_value[0]; // min- the demand 
        break;
        case 9:
        output_value= 1/(obj_value[1]); // min- the reverse of cost i.e. max cost
        break;
        case 10:
        output_value= obj_value[2]; // min- the total time aircrafts operated
        break;
        case 11:
        output_value= 1/(obj_value[3]); // min- the reverse of variance in QoS i.e. max the variance i.e. min QoS
        break;
        case 12:
        output_value = obj_value[0]/obj_value[1]; // min. - demand per cost
        break;
        case 13:
        output_value=obj_value[0]/obj_value[2];  // min. - demand per time 
        break;
		//NEU_UT
		case 14:
        output_value=obj_value[4];  //min the Utilization
        break;
		//NEU_UT
        default:
        printf("\n Error : Problem in objective function option");
        getch();
        exit(0);
                                        
        }    
        //------------------------------------------------------------// 
           
              // output_value = obj_value[1]/obj_value[0]; // min. - cost per demand
       //because max demand =125 and min cost =48.6 in diff runs
       //  output_value = ((obj_value[1]/48.6)+(125/obj_value[0]));// min. - cost per demand i.e. max. demand per cost
       // output_value=obj_value[2]/obj_value[0];  // min. - time per demand
       //output_value = ((obj_value[2]/(max_time_slot*fleet_size))+(initial_total_demand/obj_value[0])); //min. - time per demand i.e. max. - demand per time
       // output_value = obj_value[3]; // min. - the variance in QoS i.e. improving the QoS
       //output_value= 1/(obj_value[0]); // min- the reverse of demand i.e. max the demand
       //output_value= obj_value[1]; // min- the total operating cost
       //output_value= obj_value[2]; // min- the total time aircrafts operated
       //output_value= 1/(obj_value[2]); // min- the reverse of total time aircrafts operated i.e. max aircraft utilization
       if((best_case==NO)||(output_value <= min_objective_value ))
       { 
           min_objective_value=output_value;
           for(i=0;i<total_grey_indices;i++)
           best_x[i]=design_x[i];

		   //NEU old:for(i=0;i<4;i++)
           for(i=0;i<5;i++) //NEU_UT
           best_obj_value[i]=obj_value[i];
           best_case=YES;

          //storing this schedule    
          FILE *fp1,*fp2;
          char ch;
           fp1=fopen(output_filename,"r");
           if(fp1==NULL)
           {
               //printf("\n Error in writing in the %s file ",all_cases_filename);
               printf("\n cann't open the "); puts(output_filename);printf(" file in main optimization program");
               getch();
               exit(1);
           }
           fp2=fopen(best_cases_filename,"w");
           if(fp2==NULL)
           {
               //printf("\n Error in writing in the %s file ",all_cases_filename);
               printf("\n cann't make the "); puts(best_cases_filename);printf(" file in main optimization program");
               getch();
               exit(1);
           }
           do
           {
               ch=getc(fp1);
               putc(ch,fp2);
           }while(ch!=EOF);
           fclose(fp1);
           fclose(fp2);    
       }    
           
           
  }    
   else
   output_value= verybig;
   
   //----------printing the record---------------//
   all_trials_counter++; //global variable , initialized in main program before calling the optimizer
   fp=fopen(all_cases_filename,"a");
   if(fp==NULL)
   {
       //printf("\n Error in writing in the %s file ",all_cases_filename);
       printf("\n cann't make the "); puts(all_cases_filename);printf(" file in main optimization program");
       getch();
       exit(1);
   }
   fprintf(fp," %ld )  ",all_trials_counter);
   
   
   //NEU
   for( j=0;j<total_grey_indices;j++)
          fprintf(fp,"%f + ",design_x[j]);
   //alter Code Anfang
	//	  for( j=1;j<=total_grey_indices;j++)
   //     fprintf(fp,"%f + ",x[j]);
   //alter Code Ende
   //NEU
   fprintf(fp," = %f",sum_of_variables);    
   fprintf(fp,", objective value => %f \n",output_value);
   
   fclose(fp);
   //-------------------------------------------//
   return(output_value);
   
 
  /* this should be replaced with the code that calls your simulator, or
analysis tool and reports the measure of merit corresponding to the
variable set x[] which is the optimization variables set */
}



double penalty_function(double x[1+maxn])
{
  double temp;
  
  return 0;
  
  /* This should be replaced by a function of the constraint violations, if
any occur. I propose multiplying a large number by the sum of the absolute
values of the constraint violations */  

}

//NEU void eingefügt!
void main()
{
    double bl[10],bu[10],it,x[10],scale[10];
      unsigned long trials=1000;
      int i,j,k,l;
      
    
    
    //Ajeet ---- My new variables-------//
    //double best_x[4];//no. of variables+1
    //double final_x[3];
    //double obj_value[4];
    char *ch,temp_ch[2];//NEU old: temp_ch[1] -> access violation
    int objective_flag;
    //long int best_try_counter;
    //---------------------------------------//
    //for running different initial vaules in GA code
    int seed_counter,max_seed_counter;
    // max_seed_counter=20;
    //long int new_seed[20]={1,2,3,4,5,10,20,30,40,50,100,200,500,1000,5000,10000,20000,50000,100000,999999};
    long int initial_seed_value,new_seed;
    int seed_generator,seed_gap;
    //---------------------------------------//
    time_t start_time,end_time;
    struct tm * timeinfo;
    double diff_time;
    FILE *fp;
        
    //----------user input before calling the optimizer----------//
    printf("\n -------Airline Network Optimization for cargo Airlines--------BETA version");
    printf("\n >>>>****for testing and evalution only **** not for commercial use****<<<<");
    printf("\n ---------------------------------------------------------------------------");
    
    //-----------getting objective function from user-----------------------//
    printf("\n ------Possible Objective Functions------ ");
    printf("\n 1. Max. Cargo");  
    printf("\n 2. Min. Cost");  
    printf("\n 3. Max. Time ( or max. aircraft utilization)");
    printf("\n 4. Max. Quality of Service");
    printf("\n 5. Max. Cargo/Rs.");
    printf("\n 6. Max. Cargo/min.");     
	printf("\n 7. Max. Fleet Utilization");//NEU_UT
    printf("\n -----Worst case analysis----------");
    printf("\n 8. Min. Cargo");
    printf("\n 9. Max. Cost");
    printf("\n 10. Min. Time"); 
    printf("\n 11. Min. Quality of Service");
    printf("\n 12. Min. Cargo/Rs.");
    printf("\n 13. Min. Cargo/min.");
	printf("\n 14. Min. Fleet Utilization");//NEU_UT
    printf("\n -----------------------------------");
    printf("\n Choose your Objective Function : ");
    do
    {
        objective_flag=NO;
        scanf("%d",&objective_pointer);
        fflush(stdin);
        switch(objective_pointer)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
			case 13://NEU_UT
			case 14://NEU_UT

                objective_flag=YES;
                break;
            default:
                objective_flag=NO;
                break;
        }
        if(objective_flag==NO)
        printf("\n Enter a valid Choice :");  
    }while(objective_flag!=YES);
    fflush(stdin);

    //----------------------------------------------------------------------//
    printf("\n Enter the Input File name (to read network detail) \n ( without "".txt"" Extension) max 40 character - : ");
    //scanf("%s",&input_filename);//global variable in my program
    gets(input_filename);
    
    printf("\n Enter the Output File name (to store the final airline schedules ) \n ( without "".txt"" Extension) max 40 character - : ");
    //scanf("%s",&filename);//global variable in my program
    gets(filename);
    printf("\n Enter the Folder name (to store all output files)\n max 40 character - : ");
    //scanf("%s",&foldername);
    gets(foldername);
    printf("\n--------------------------------------------------------------------");
    printf("\n Make sure that folder is already created \n (press any key to continue) - ");
    getch();
  

        
    time(&start_time);
    timeinfo = localtime ( &start_time );
    strcat(foldername,"/");
        
    strcpy(compare_filename,input_filename);
    strcat(input_filename,".txt");
    //adding folder name with the output file name
    strcpy(temp_filename,foldername);
    strcat(temp_filename,filename);
    strcpy(filename,temp_filename);
    //adding folder name with the compare file name
    strcpy(temp_filename,foldername);
    strcat(temp_filename,compare_filename);
    strcpy(compare_filename,temp_filename);
    strcat(compare_filename,"_compare_schedules.txt");
    fp=fopen(compare_filename,"w");
            if(fp==NULL)
                {
                    printf("\n cann't make the "); puts(compare_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
			fprintf(fp,"                       Indices                                      Network Parameters                                                     Started at : %s",asctime(timeinfo));//NEU
            fprintf(fp,"\n Seed                  Demand/Cost/Time/Priority                    Demand/Cost/Time/VarQoS/Utilization                                    Objective Function Values");//NEU
			fprintf(fp,"\n -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");//NEU
	fclose(fp);       
    //----reading the inital seed value and no. of times to run the code again
    fp=fopen("ga_run","r");
    if(fp==NULL)
    {
        printf("\n Cann't open the file ga_run");
        getch();
        exit(1);
    }    
    fscanf(fp,"max no. of runs (for optimization code)=%d\n",&max_seed_counter);
    fscanf(fp,"initial seed value (any random number)=%ld",&initial_seed_value); 
    //----------running for different initial values ---------------------//
    seed_generator=0;
    seed_gap=1;
    new_seed=initial_seed_value;
    for(seed_counter=0;seed_counter<max_seed_counter;seed_counter++)
    {
            trials=1000;
            //==================================//
            //optimization variavles limit
                  /*
                  bl[0]=0;
                  bu[0]=1;
                  scale[0]=1;
                  bl[1]=0;
                  bu[1]=1;
                  scale[1]=1;
                  bl[2]=0;
                  bu[2]=1;
                  scale[2]=1;
                  bl[3]=0;
                  bu[3]=1;
                  scale[3]=1;
                  bl[4]=0;
                  bu[4]=1;
                  scale[4]=1;
                  */
                  for(i=0;i<total_grey_indices;i++)//total_grey_indices (declared in include_files.h)
                  {
                      bl[i]=0;
                      bu[i]=1;
                      scale[i]=1;
                  }    
            //==================================//
            //-------- adding the seed value with the output file name----------//
            strcpy(output_filename,filename);
            strcat(output_filename,"_");
            //generating a new seed value
            if(seed_generator==0)
            {
                seed_generator=max_seed_counter/5;
                new_seed+=((seed_gap-1)*seed_generator*initial_seed_value);
                seed_gap+=(initial_seed_value/10);
                seed_gap++;
            }
            else
            {
                
                new_seed+=((1*seed_gap)+((seed_counter*5)/max_seed_counter));
            }    
            seed_generator--;    
            //writing the seed value in the file
            fp=fopen("ga_seed","w");
            if(fp==NULL)
                {
                    printf("\n Error in writing in ga_seed file in main optimization program");
                    getch();
                    exit(1);
                }
            //fprintf(fp,"%ld",new_seed[seed_counter]);
            fprintf(fp,"%ld",new_seed);
            fclose(fp);
            //reading back as an character
            fp=fopen("ga_seed","r");
            if(fp==NULL)
                {
                    printf("\n Error in reading from ga_seed file in main optimization program");
                    getch();
                    exit(1);
                }
            fscanf(fp,"%c",temp_ch);
			temp_ch[1]='\0';//NEU
            while(!feof(fp))
            {
				strcat(output_filename,temp_ch);
                fscanf(fp,"%c",&temp_ch);
            }    
            fclose(fp);
            strcpy(all_cases_filename,output_filename);
            strcpy(best_cases_filename,output_filename);
            strcat(all_cases_filename,"_all_trials.txt");
            //strcat(best_cases_filename,"_best_trials.txt");
            strcat(best_cases_filename,"_final_schedule.txt");
            strcpy(output_filename,foldername);
            strcat(output_filename,"/");
            strcat(output_filename,"temp_file.txt");
            
            //strcat(output_filename,"_final_schedule.txt");
            //strcat(output_filename,".txt");
            //----------deleting all the privious info, if all cases file is already existing-----------------//
            fp=fopen(all_cases_filename,"w");
            if(fp==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",all_cases_filename);
                    printf("\n cann't make the "); puts(all_cases_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
            fprintf(fp," trial_no )----------variables-----------sum of variables--------objective value------------------\n");
            fclose(fp);
            //--------------reinitializing all the GADO's global variables before calling the GA code again------------------//
             maxpop=100;maxstore=1000;minevalpop=10;maxevals=10000;
              maxevals_increment=1000;nnofset=0;nnk=2; maxtry=1000000;evals=0;
              succevals=0;feasevals=0;ins=0;outs=0;redundants=0;    
              population=0;stored=0; addnewborn=0;
              candidate1; candidate2; trycount; reseedcount=0;
              maxreseeds=20;no_screen=0;in_reseed=0;no_penalty_decrease=1;
              easy_start=1;gready_finish=1;nonlinear_penalty=0;
             verybig=1e100; commonvar1=0;commonvar2=0;
             initialradius=0;
              reseedfraction=0.25;mutation_factor=1;GC_factor=0.2;GC_scale=1;GC_shrink=1;
              penaltycoef=1;penaltymult=2;penaltypower=1; rejecttolerance=1;
              stoptolerance=0.001;initialcrowdingfactor=0.9;finalcrowdingfactor=0;
              
                
            //----------calling the optimizer--------------------------------//
            //x is design variable supplied by optimizer
            all_trials_counter=0; //global variable
            best_case=NO;//global variable, an indicator to store the best case
            it=ga(total_grey_indices, scale,x, bl, bu,"ga_seed","ga_input",objective_function,penalty_function);
            
              
            
            //----------printing all the best cases--------//
            /*
            fp=fopen(best_cases_filename,"w");
            if(fp==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",best_cases_filename);
                    printf("\n cann't make the "); puts(best_cases_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
           fprintf(fp," -----------------Variables--------------------Objective Function----\n");
           best_try_counter=0;
           for(i=0;i<population;i++)
           {
               
               fprintf(fp," %ld )",best_try_counter); 
               for( j=1;j<4;j++)
                fprintf(fp," %f ",pop[i].r[j]);
               fprintf(fp," Objective value => %f \n",objective_function(pop[i].r));
               best_try_counter++;
            }    
            fclose(fp);
            */
             //=======================================================//
            // print the Final best schedule for this seed
            //=======================================================//
             
             //for(i=0;i<3;i++)
             //final_x[i]=best_x[i+1];
            
           // Cargo_scheduling(final_x,3,obj_value,YES);//YES means  print 
            
            fp=fopen(compare_filename,"a");
            if(fp==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",best_cases_filename);
                    printf("\n cann't open the "); puts(compare_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
            fprintf(fp,"\n %-15ld => ",new_seed);
            for(i=0;i<total_grey_indices;i++)
            fprintf(fp,"%9.4f ",best_x[i]);
            fprintf(fp," => ");
			//NEU_UT alt: for(i=0;i<4;i++)
            for(i=0;i<5;i++) //NEU_UT
            fprintf(fp,"%12.4f ",best_obj_value[i]);
            fprintf(fp," => ");
            fprintf(fp,"%12.4f",it);//our objective function value
            time(&end_time);
            timeinfo = localtime ( &end_time );
            fprintf(fp," , Generated on: %s ",asctime(timeinfo));
        
            fclose(fp);
    }    //end of for(seed_counter=0;seed_counter<max_seed_counter;seed_counter++)
    
        time(&end_time);
        timeinfo = localtime ( &end_time );
        printf("\n\n Ends at : %s ",asctime(timeinfo));
        diff_time = difftime(end_time,start_time);
        printf("\n Time taken = %.2lf Seconds or %.2lf Minutes",diff_time,diff_time/60.0);
        printf("\n--------------------------------------------------------");   
        printf("\n\t\t !!! Completed !!!\n");
        printf("\n ===========================================================================");
        printf("\n This program is designed and developed at ");
        printf("\n Aerospace Engineering Department, IIT Bombay \n");    
        printf("\n For any suggestion/query, contact - ");
        printf("\n Prof. Rajkumar S. Pant, LTA Systems Lab, rkpant@aero.iitb.ac.in");
        printf("\n Ajeet Singh, M.Tech. Student(2005-2007), ajeet@aero.iitb.ac.in");
		printf("\n Florian Maul, Technical University of Munich, florian.maul@mytum.de");
        printf("\n ===========================================================================");
        printf("\n Press any key to exit---\n");

        getch();
}   //end of main function 

/*
int main()
{
  double bl[10],bu[10],it,x[10],scale[10];
  unsigned long trials=1000;
  int i,j,k,l;
  
  bl[0]=0;
  bu[0]=10-0.0001;
  scale[0]=1;
  bl[1]=0;
  bu[1]=40-0.0001;
  scale[1]=1;
  bl[2]=0;
  bu[2]=10-0.0001;
  scale[2]=1;
  bl[3]=0;
  bu[3]=40-0.0001;
  scale[3]=1;  

  it=ga(4, scale,x, bl,
  bu,"ga_seed","ga_input",objective_function,penalty_function);

  return 0; 
}


*/
