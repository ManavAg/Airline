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

