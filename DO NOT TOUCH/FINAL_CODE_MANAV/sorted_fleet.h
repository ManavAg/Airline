#include "insert_sort.h"
void Sorted_fleet();

void Sorted_fleet()
{
    int i;
    struct Fleet *fleet;
    struct Fleet *candidate;
    fleet=head_fleet; //memory of head_fleet allocated in read_input()
    for(i=0;i<fleet_size;i++)
    {
        candidate=(struct Fleet *)malloc(sizeof(struct Fleet));
        candidate->ac_index=fleet->ac_index;
        candidate->TAT=fleet->TAT;
        candidate->ap_index=fleet->ap_index;
		candidate->ac_type=fleet->ac_type; //NEU_MAC
        candidate->end_time=fleet->end_time;
        candidate->life=fleet->life;
        candidate->takeoff_time=fleet->takeoff_time;
        strcpy(candidate->ap_name,fleet->ap_name);
        candidate->demand_carried=0;
        candidate->residual_capacity=0;
		candidate->BH_max=fleet->BH_max; //NEU_BH
		candidate->BH_remain=fleet->BH_remain; //NEU_BH
        candidate->next_aircraft=NULL;
        Insert_sort(candidate); //memory allocated for candidate goes in head_aircraft_list
        fleet=fleet->next_aircraft;
    }
    //free the memory (was allocated for head_fleet)
	fleet=head_fleet;
    while(fleet!=NULL)
    {
        candidate=fleet;
        fleet=fleet->next_aircraft;
        free(candidate);
    }
    //printing the sorted fleet detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in sorted_fleet");
            getch();
            exit(1);
        }    
        fprintf(fprocess,"\n\n Fleet Sorted as per Takeoff time- ");
        fleet = head_aircraft_list;
        fprintf(fprocess,"\n Ac_No\tType\tAirport Ap_No\tTakeoff_time\tEnd_time\tBH_max\tBH_remain");
        for(i=0;i<fleet_size;i++)
        {
            //old: fprintf(fprocess,"\n %d\t%s\t%d\t%.2f\t\t%.2f",fleet->ac_index,fleet->ap_name,fleet->ap_index,fleet->takeoff_time,fleet->end_time);
			fprintf(fprocess,"\n %d\t%d\t%s\t%d\t%.2f\t\t%.2f\t%.2f\t%.2f",fleet->ac_index,fleet->ac_type,fleet->ap_name,fleet->ap_index,fleet->takeoff_time,fleet->end_time,fleet->BH_max,fleet->BH_remain); //NEU_BH
            fleet=fleet->next_aircraft;
        }  
        fclose(fprocess);  
    }//end of  if(user_choice==YES)           
}//end of function    
