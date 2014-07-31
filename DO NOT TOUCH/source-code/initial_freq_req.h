void Initial_freq_req();
void Initial_freq_req()
  {
    int i,j,k;//NEU_MAC (+k)
    struct Data *data;
	float temp_capacity,save_capacity; //NEU_MAC
     
    data=head_data;
    initial_total_demand=0;//global variable
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        //NEU_MAC old: if(data->capacity==0)
		if(i==j)
        data->initial_freq_req=0;
        else
		{	//NEU_MAC Start
			save_capacity=0;
			for(k=0;k<ACTYPE_NO;k++)
			{
				temp_capacity=Get_capacity(k+1,i+1,j+1);
				if (save_capacity==0 || (save_capacity < temp_capacity && temp_capacity!=0 ))
					save_capacity=temp_capacity;
			}
			data->initial_freq_req=(int)ceil(data->demand/save_capacity);
		}	//NEU_MAC End
		//NEU_MAC old: data->initial_freq_req=(int)ceil(data->demand/data->capacity);
        data->freq_offered=0;
        initial_total_demand+=data->demand;
        data=data->next_data;
    }    
    //printing the Inital freq requ detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in initial_freq_req");
            getch();
            exit(1);
        }
        data=head_data;
        fprintf(fprocess,"\n\n Initial frequency required to satisfy all the demand");
        for(i=0;i<AIRPORT_NO;i++)
        {
            fprintf(fprocess,"\n");
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess,"%d\t",data->initial_freq_req);
                data=data->next_data;
            }
        }   
        fclose(fprocess); 
    }//end of if(user_choice==YES)   
}//end of function