//This function do 2 works
//1. go to the point from where the aircraft is flying empty
//2. store the detail
#include "store_updated_route.h"

void Trace_demand(int, int );

void Trace_demand(int our_ac,int final_destination)
{
    int i,j;
    float total_demand,min_residual;
    struct empty_detail *head_empty_detail = NULL,*last_empty_detail,*empty; 
    struct route_detail *item;
    struct Data *temp_data;
    FILE *fprocess;
    
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
                {
                    printf("\n cann't open the Output/PROCESS file in trace_demand");
                    getch();
                    exit(1);
                }
    }    
    //1. go to the point from where the aircraft is flying empty
    //item=last_route->prev;//since last route in current route so we are going one step back
    item=last_route;
    
    while(item->prev!=NULL )//from last to the first flight
    {
            
        if(item->ac_number == our_ac)//if this detail is of our ac then record it in another struct empty detail
        {
           
            if(item->residual_capacity !=0)
            {
                empty=(struct empty_detail *)malloc(sizeof(struct empty_detail));
                empty->flight=item->flight_no;
                empty->source=item->sr_index;
                empty->residual_capacity=item->residual_capacity;
                empty->next_empty_detail=NULL;
                empty->prev_empty_detail=NULL;
                if(head_empty_detail==NULL)
                {
                    head_empty_detail=empty;
                    last_empty_detail=empty;
                }
                else
                {
                    last_empty_detail->prev_empty_detail=empty;
                    empty->next_empty_detail=last_empty_detail;
                    last_empty_detail=last_empty_detail->prev_empty_detail ;
                }       
               
            }//end of if (residual_capacity !=0)
            else
            {
               break; //no need to go more back from here
            }  //end of else   
        }//end of if(ac_number == our_ac)
        item=item->prev;
    }//end of while(item!=NULL)
    
    
    empty=head_empty_detail;
    if(empty!=NULL)
    {
        
        if(user_choice==YES)
        {           
            fprintf(fprocess,"\n Empty detail");
            fprintf(fprocess,"\n Flight Ac  Source empty");
        }    
            
        
        while(empty!=NULL)
        {
            if(user_choice==YES)
            fprintf(fprocess,"\n %d\t%d\t%d\t%.2f ",empty->flight,our_ac,empty->source,empty->residual_capacity);
            empty=empty->prev_empty_detail;
        }
        
    }   
        
        //2. go to the point from where we need to update the demand
        total_demand=0.0;
        min_residual=last_route->residual_capacity;//our current residual capacity
        empty=head_empty_detail;
        last_empty_detail=NULL;
        //if(empty!=NULL)
        //{
            //fprintf(fprocess,"\n Min. req= %.2f",min_residual);
            while(empty!=NULL && total_demand<min_residual)
            {
                //read the demand between its source and our final destination
                temp_data=head_data;
                //going upto the source airport from where our aircraft takesoff empty
                for(i=1;i<empty->source;i++)
                for(j=0;j<AIRPORT_NO;j++)
                temp_data=temp_data->next_data;
                //going upto the final destination airport
                for(j=1;j<final_destination;j++)
                temp_data=temp_data->next_data;
                
                if(empty->residual_capacity < min_residual)
                {
                    min_residual=empty->residual_capacity;
                    total_demand=0;
                }
                total_demand+=temp_data->demand;  
                last_empty_detail=empty;//keeping record of last detail, because when we will go out empty=NULL  
                empty=empty->prev_empty_detail;
                
            }//end of while    
            //update the demand from last_empty_detail
            if(last_empty_detail!=NULL)
            {
                if(user_choice==YES)
                {
                    fprintf(fprocess,"\n Start to take extra cargo from airport %d in flight %d",last_empty_detail->source,last_empty_detail->flight);
                    fclose(fprocess);
                }    
                
                
                //find and store how much extra to carry from these airports
                Store_updated_route(last_empty_detail,final_destination,min_residual);
            }    
        
}//end of function    
    
