
#include "check_airport_timing.h"
void Get_fly_time(int,float,float *,int);
void Get_fly_time(int source,float takeoff_time,float *fly_time,int size)
{

    int i,j,k,l;
    int desti_index,source_index;
    float block_time,max_delay,delay,time_diff,landing_time,next_takeoff_time;
    struct Airports *airport;
    time_t current_time;
    struct tm * timeinfo;
    FILE *fprocess;
    
       
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
            {
                next_takeoff_time=landing_time+TURN_AROUND_TIME;
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
    


