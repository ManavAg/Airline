#include "check_end_time.h"
void Check_takeoff_availability(struct Fleet *, int,float );
 
void Check_takeoff_availability(struct Fleet *candidate,  int desti_index, float landing_time)
{
    int i;
    char ch='y';
    float next_takeoff_time,time_diff;
    FILE *fprocess; 
    
    
        next_takeoff_time=landing_time+ TURN_AROUND_TIME;
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
