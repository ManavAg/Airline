//updating the demand
//updating the count of freq offered

void Demand_update(struct Fleet  *,int );
void Demand_update(struct Fleet *candidate,int desti_index)
{
    int i,j;
    struct Data *data,*temp_data;
	float temp_capacity;//NEU_MAC
    //going upto the airport where our candidate aircraft is available
    data=head_data;
    for(i=1;i<candidate->ap_index;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    //going upto the destination airport
    for(j=1;j<desti_index;j++)
    data=data->next_data;
    
    //updating the count of freq offered on that route
    data->freq_offered++;
    
    //storing the operating cost info
	//NEU_MAC old:candidate->data->TOC;
    candidate->operating_cost=Get_cost(candidate->ac_type,candidate->ap_index,desti_index);//NEU_MAC

	//setting demand data
	temp_capacity=Get_capacity(candidate->ac_type,candidate->ap_index,desti_index);//NEU_MAC
	//NEU_MAC old: if(data->demand > data->capacity) candidate->demand_carried=data->capacity;
	if(data->demand > temp_capacity)//NEU_MAC
    candidate->demand_carried=temp_capacity;//NEU_MAC
    else
    candidate->demand_carried=data->demand;
    //NEU_MAC old:candidate->residual_capacity=data->capacity-candidate->demand_carried;
    candidate->residual_capacity=temp_capacity-candidate->demand_carried;//NEU_MAC
    //updating ,remaining demand = orignal demand - carried demand
    data->demand-=candidate->demand_carried;
    
    //printing the updated demand
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in demand_update");
            getch();
            exit(1);
        } 
		fprintf(fprocess,"\n Aircraft %d of Type %d is going from airport %d to airport %d",candidate->ac_index,candidate->ac_type,candidate->ap_index,desti_index);//NEU_MAC
        fprintf(fprocess,"\n Updated Demand -\n");
        temp_data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess," %.2f\t",temp_data->demand);
                temp_data=temp_data->next_data;
            }
            fprintf(fprocess,"\n");
        }
        fclose(fprocess);
    }
               
                      
}//end of function    
