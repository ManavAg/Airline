#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<conio.h>

#define YES 1
#define NO 0
#define MAX_SIZE 20
#define NOT_FOUND 0
#define FOUND 1
//#define TURN_AROUND_TIME 45
        
//GLOBAL VARIABLES
struct Grey_indices *head_select=NULL;//grey index in select category for demand,cost,time and route priority
struct Grey_indices *head_reject=NULL;//grey index in reject category for demand,cost,time and route priority
struct Grey_indices *head_probable=NULL;//grey index in probable category for demand,cost,time and route priority
struct Airports *head_airport=NULL;//airport detail like, name, takeoff and landing slots, BELF
struct Data *head_data=NULL;//network data like demand, TOC,Block time, aircraft capacity, route priority
struct Fleet *head_fleet=NULL;//fleet detail like airport, takeoff time, end time, life time
struct Fleet *head_aircraft_list=NULL;//sorted list of aircraft
struct AC_data *head_ac_data=NULL;//list of aircraft types NEU_MAC
struct aea_input *head_aea_input=NULL;//list for aea data for each aircraft type NEU_DOC
struct Bases *head_base=NULL;//base station detail, name, hanger capacity
struct Grey *head_grey=NULL;//priorities of grey indices
struct Final_grey *head_final_grey=NULL;//final grey index and cat. of each OD pair
struct Whitening_decision *BIG=NULL,*SMALL=NULL;//used in grey analysis for taking decision of selection or rejection
//struct choice_history *head_choice=NULL;//used to keep the record of all previous choices in finding possible destination
struct route_detail *head_route=NULL,*last_route=NULL;//final network schedule detail
#define total_grey_indices 4 
struct tats *head_tat;
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
float TURN_AROUND_TIME;
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

//Program files
#include "data_structure.h"
#include "read_input.h"
#include "get_route_BT.h"
#include "get_capacity.h"//NEU_MAC
#include "get_cost.h"//NEU_MAC
#include "initial_freq_req.h"
#include "initialize_grey_index.h"
#include "make_end_time.h"
#include "sorted_fleet.h"
#include "grey_analysis.h"
#include "get_your_flight.h"
#include "grey_base_analysis.h"
#include "go_to_base.h"
//#include "adjust_remaining_demand.h"
#include "objective_calc.h"
#include "write_output.h"
//NEU
#include "no_holding.h"
#include "free_list.h"
//NEU
