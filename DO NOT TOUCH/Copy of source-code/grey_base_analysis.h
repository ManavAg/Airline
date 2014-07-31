#include "get_base_cost_index.h"
#include "get_base_demand_index.h"
#include "get_base_time_index.h"
//#include "get_base_network_design_index.h"
#include "get_base_route_priority_index.h"
//#include "get_base_service_quality_index.h"


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
    Get_fly_time(candidate->ap_index,candidate->takeoff_time,fly_time,AIRPORT_NO); 
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
    


