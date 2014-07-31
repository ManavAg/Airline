#include "everything_fine.h"
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
    //generate blocktime for the original route again NEU_BH
	original_route_blocktime=Get_route_BT(candidate->ap_index,desti_index);

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
			    for(j=0;j<ACTYPE_NO;j++)
             fprintf(fprocess,"\n Total %d aircraft are available on base airport %d",base_aircraft,base->base_index);
             fclose(fprocess);
		   }  
		         //if base station can handle more aircraft then only its my possible base station
        //I cann't assume that aircraft which are already their will leave the base staion for me
        if(base->hc - base_aircraft >0)
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
            delay= -((candidate->TAT)+time_diff);//blocktime =0 means we are on the base itself, so no need of TURN AROUND TIME and time diff in takeoff
            
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
