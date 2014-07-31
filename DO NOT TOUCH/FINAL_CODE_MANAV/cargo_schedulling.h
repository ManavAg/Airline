#include "include_files.h"

void Cargo_scheduling(double *, int,double *,int);
void Cargo_scheduling(double *x, int size,double *obj_value,int print_choice)
{  
    time_t end_time;
    struct tm * timeinfo;
    struct Fleet *candidate;
	struct Fleet *temp_fleet;
	struct tats *tat;
	
	//=====================================/
    //assigning x to grey priority 
    int i;
    //total_grey_indices=5; //declared in include_files
    
    //FILE *fp=fopen(output_filename,"w");
    //fclose(fp);
    
    
 
    
    struct Grey *grey,*last_grey;
    for(i=0;i<total_grey_indices;i++)
    {
        grey=(struct Grey *)malloc(sizeof(struct Grey));
        grey->priority=x[i];
        grey->next_grey=NULL;
        if(head_grey==NULL)
        {
            head_grey=grey;
            last_grey=grey;
        }
        else
        {
            last_grey->next_grey=grey;
            last_grey=last_grey->next_grey;
        }
    } 
    //=====================//
    /*
    time(&start_time);
    timeinfo = localtime ( &start_time );
    printf("\n Starts at : %s ",asctime(timeinfo));
    printf("\n -------Airline Network Optimization for cargo Airlines--------BETA version");
    printf("\n >>>>****for testing and evalution only **** not for commercial use****<<<<");
    printf("\n ---------------------------------------------------------------------------");
    printf("\n\n Are you interested in -");
    printf("\n 1. Complete detail of scheduling process");
    printf("\n 2. Only final schedule");
    printf("\n Enter your choice - ");
    do
    {
        scanf("%d",&user_choice);
        if (user_choice==2)
        user_choice=NO;
        else if(user_choice!=YES && user_choice!=NO)
        printf("\n Please enter a valid choice (1 or 2)- ");
    }while(user_choice!=YES && user_choice !=NO);    
    */ 
    user_choice=NO;
      
    Read_input();
    
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","w");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't make the file Output/PROCESS in main");
            getch();
            exit(1);
            }
        fprintf(fprocess," -------Airline Network Optimization for cargo Airlines--------BETA version");
        fprintf(fprocess,"\n >>>>****for testing and evalution only **** not for commercial use****<<<<\n");
        fprintf(fprocess,"\n ===========================================================================");
        fprintf(fprocess,"\n This program is designed and developed at ");
        fprintf(fprocess,"\n Aerospace Engineering Department, IIT Bombay \n");    
        fprintf(fprocess,"\n For any query/suggestion, contact - ");
        fprintf(fprocess,"\n Prof. Rajkumar S. Pant , LTA systems lab, rkpant@aero.iitb.ac.in");
        fprintf(fprocess,"\n Ajeet Singh, MTech Student(2005-2007), ajeet@aero.iitb.ac.in");
		fprintf(fprocess,"\n Florian Maul, Technical University of Munich, florian.maul@mytum.de");
        fprintf(fprocess,"\n ===========================================================================\n");
        //fprintf(fprocess,"\n The Schedule is generated on :\n %s ",asctime(timeinfo));
        fprintf(fprocess,"\n ===========================================================================\n");
        
        
        fclose(fprocess);        
    } //end of if  (user_choice==YES)  
    
    // calcualting initial freq required between each OD pair
    Initial_freq_req();
    //initializing the grey indices to zero
    Initialize_grey_index();
    //Calculating the end time of each aircraft for todays operations (taking care of maximum time slot)
    Make_end_time();
    //Make a shorted list of aircrafts as per takeoff time
    Sorted_fleet();
    
    //starting the scheduling process
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't make the file Output/PROCESS in main");
            getch();
            exit(1);
            }
        fprintf(fprocess,"\n\n ***************************************\n");
        fprintf(fprocess,"\n Starting the scheduling process -------\n");
        fprintf(fprocess,"\n ***************************************\n");
        fclose(fprocess);
    }    
    
    candidate=head_aircraft_list;
	while(candidate != NULL)
            {
               tat=head_tat;
               for (i=1;i<candidate->ac_index;i++)
				   tat=tat->next_tat;
			   candidate->TAT=tat->tat;
                STATUS=NO;
                SENDING_TO_BASE=NO;
                
				if(user_choice==YES)
                {
                    FILE *fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't make the file Output/PROCESS in main");
                        getch();
                        exit(1);
                        }
                    fprintf(fprocess,"\n ------------------------------");
                    fprintf(fprocess,"\n\n\n Scheduling the aircraft %d\n\n\n",candidate->ac_index);
                    fprintf(fprocess," ------------------------------\n");
					

                    if(candidate->takeoff_time>=max_time_slot)
                    {
                        fprintf(fprocess,"\n ------------------------------\n");
                        fprintf(fprocess,"\n OPERATING TIME OVER FOR THE DAY\n");
                        fprintf(fprocess,"\n ------------------------------");
                        fclose(fprocess);
                        break;
                    }    
                    fclose(fprocess);
                }
                /*
                printf("\n ------------------------------");
                printf("\n Scheduling the aircraft %d",candidate->ac_index);
                printf("\n ------------------------------");  
                */
                //doing grey analysis of all the airports
                GREY_ANALYSIS=YES;
                Grey_analysis(candidate);
                GREY_ANALYSIS=NO;
                //printf("\n -----------------------------%f ",candidate->TAT);
                Get_your_flight(candidate);
                                              
                if(STATUS==NO)
                {
                    SENDING_TO_BASE=YES;
                    //again grey anlysis because when going to base, landing time check will be little change
                    GREY_ANALYSIS=YES;
                    Grey_base_analysis(candidate);
                    GREY_ANALYSIS=NO;
                    Go_to_base(candidate);
                }  
                
                candidate=candidate->next_aircraft;
               
            }    //end of while (candidate != NULL)
    
	//NEU delete holding
	No_holding();
	//NEU	

    //calculating the objective values       Now with Real Utilization
    Objective_calc(obj_value);
    
    //Adjust_remaining_demand();
    if(print_choice==YES)
    {
        Write_output();
        /*
        time(&end_time);
        timeinfo = localtime ( &end_time );
        printf("\n\n Ends at : %s ",asctime(timeinfo));
        //diff_time = difftime(end_time,start_time);
        //printf("\n Time taken = %.2lf Seconds or %.2lf Minutes",diff_time,diff_time/60.0);
        printf("\n--------------------------------------------------------");   
        printf("\n\t\t !!! Completed !!!\n\n");
        if(user_choice==YES)
        {
                FILE *fprocess=fopen("Output/PROCESS.txt","a");
                if(fprocess==NULL)
                   {
                    
                    printf("\n Cann't make the file Output/PROCESS in main");
                    getch();
                    exit(1);
                    }
                fprintf(fprocess,"\n\n\n--------------------------------------------------------"); 
                fprintf(fprocess,"\n \n Ends at : %s ",asctime(timeinfo));
                fprintf(fprocess,"\n--------------------------------------------------------");   
                fprintf(fprocess,"\n\t\t !!! Completed !!!\n\n");
                fclose(fprocess);
            }//end of if (user_choice==YES)   
    
        printf("\n ===========================================================================");
        printf("\n This program is designed and developed at ");
        printf("\n Aerospace Engineering Department, IIT Bombay \n");    
        printf("\n For any suggestion/query, contact - ");
        printf("\n Prof. Rajkumar S. Pant , LTA systems lab, rkpant@aero.iitb.ac.in");
        printf("\n Ajeet Singh, MTech Student(2005-2007), ajeet@aero.iitb.ac.in");
        printf("\n ===========================================================================\n");
        printf("\n Press any key to exit---\n");
        
        getch();
        */
    }//end of if (print_choice==YES)    
        
    //GLOBAL VARIABLES
	//NEU4
	Free_list();
	//NEU4

	head_select=NULL;//grey index in select category for demand,cost,time and route priority
	head_reject=NULL;//grey index in reject category for demand,cost,time and route priority
	head_probable=NULL;//grey index in probable category for demand,cost,time and route priority
	head_airport=NULL;//airport detail like, name, takeoff and landing slots, BELF
	head_data=NULL;//network data like demand, TOC,Block time, aircraft capacity, route priority
	head_fleet=NULL;//fleet detail like airport, takeoff time, end time, life time
	head_ac_data=NULL;//NEU_MAC
	head_aea_input=NULL;//NEU_DOC
	head_aircraft_list=NULL;//sorted list of aircraft
	head_base=NULL;//base station detail, name, hanger capacity
	head_grey=NULL;//priorities of grey indices
	head_final_grey=NULL;//final grey index and cat. of each OD pair
	BIG=NULL;
	SMALL=NULL;//used in grey analysis for taking decision of selection or rejection
	//struct choice_history *head_choice=NULL;//used to keep the record of all previous choices in finding possible destination
	head_route=NULL;
	last_route=NULL;//final network schedule detail
   
   // getch(); 
   // return (obj_value);
}    
