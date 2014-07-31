#include "get_fly_time.h"
//#include "get_extra_data.h"
#include "get_cost_index.h"
#include "get_demand_index.h"
#include "get_time_index.h"
#include "get_route_priority_index.h"
//#include "get_network_design_index.h"
#include "get_grey_index.h"
#include "get_grey_category.h"

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
    


