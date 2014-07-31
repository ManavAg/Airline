//#include "get_route_BT.h"
//#include "check_airport_timing.h"
#include "check_takeoff_availability.h"
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
