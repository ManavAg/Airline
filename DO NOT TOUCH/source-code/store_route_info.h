#include "trace_demand.h"
void Store_route_info(struct Fleet *,float ,float ,int );

void Store_route_info(struct Fleet *candidate,float new_take_off,float time_diff,int desti_index)
{

    int i;
	struct route_detail *item;
	struct AC_data *temp_acdata ;//NEU_MAC
   
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
    if(SENDING_TO_BASE==NO)
    item->landing=new_take_off-time_diff-TURN_AROUND_TIME;
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
