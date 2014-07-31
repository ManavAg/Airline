//if everything is fine 
#include "demand_update.h"
#include "store_route_info.h"

void Everything_fine(struct Fleet *,float ,float ,int );

void Everything_fine(struct Fleet *candidate,float new_take_off,float time_diff,int desti_index)
{
    int i;
    float used_time;
    struct Bases *base;
    struct Fleet *item;
    struct Airports *airport;
    FILE *fprocess;
    
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
            if(desti_index==base->base_index)
            {
                base->hanger_capacity--;
                if(user_choice==YES)
                {
                    fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                    {
                        printf("\n Cann't open the file Output/PROCESS in everything_fine");
                        getch();
                        exit(1);
                    } 
                    fprintf(fprocess,"\n Remaining hanger capacity on base airport %d = %d",desti_index,base->hanger_capacity);
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
 
