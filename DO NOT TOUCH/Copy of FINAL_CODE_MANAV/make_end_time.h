// for end time of each aircraft
void Make_end_time( );  
void Make_end_time() 
{ 
    int i;
    struct Fleet *fleet;
    fleet=head_fleet;
    for(i=0;i<fleet_size;i++)
    {
        if(fleet->life < max_time_slot)
        fleet->end_time=(fleet->life)-(fleet->takeoff_time);
        else
        fleet->end_time=(max_time_slot)-(fleet->takeoff_time);
        fleet=fleet->next_aircraft;
    } 
    //printing the end time detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in make_end_time");
            getch();
            exit(1);
        }
        fleet=head_fleet;
        fprintf(fprocess,"\n\n Aircraft\tEnd-time");
        for(i=0;i<fleet_size;i++)
        {
            fprintf(fprocess,"\n %d\t\t%.2f",fleet->ac_index,fleet->end_time);
            fleet=fleet->next_aircraft;
        }   
        fclose(fprocess); 
    }//end of if(user_choice==YES)           
}//end of function    
