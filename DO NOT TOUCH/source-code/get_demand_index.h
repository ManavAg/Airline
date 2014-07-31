void Get_demand_index(int,int);
void Get_demand_index(int source, int actype)
{
    int i,j;
	float ac_capacity; //NEU_MAC
    //NEU_VLA old: float demand_ratio[AIRPORT_NO],temp_demand;
	float temp_demand;//NEU_VLA
    float max_ratio;
    //NEU_VLA old: float temp[AIRPORT_NO];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Grey_indices *select,*probable,*reject;
    struct Airports *airport;
    //NEU_VLA old: float line_sum[AIRPORT_NO],total_demand,column_sum[AIRPORT_NO];
	float total_demand;//NEU_VLA

	//NEU_VLA start
	float *demand_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *line_sum=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *column_sum=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (demand_ratio == NULL || temp == NULL|| line_sum == NULL || column_sum == NULL)
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end

    //line sum = outgoing demand and column sum = incoming demand
    //initializing them to zero
    for(i=0;i<AIRPORT_NO;i++)
    {
         column_sum[i]=0;
         line_sum[i]=0;         
    }
    //sum of all outgoing demands at each airport =line sum
    //sum of all incoming demands at each airport = column sum
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        line_sum[i]+=data->demand;
        column_sum[j]+=data->demand;
        data=data->next_data;
    }    
    //printf("\n Sum of demand");
    //for(i=0;i<AIRPORT_NO;i++)
    //printf("\n %f %f",line_sum[i],column_sum[i]);
    
    //total demand = sum of all outgoing demands ( or sum of all incoming demands)
    total_demand=0;
    for(i=0;i<AIRPORT_NO;i++)
    total_demand+=line_sum[i];
    
    //going up to the right line in data array
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;

	//calculating demand ratio from source to all possible destinations
    for(j=0;j<AIRPORT_NO;j++)
    {
        
		//Get Capacity for the flight (NEU_MAC)
		ac_capacity=Get_capacity(actype,source,j+1);//NEU_MAC

		//NEU_MAC old_start
		/*if(data->demand > data->capacity)
		temp_demand=data->capacity;
        else
        temp_demand=data->demand; old_end*/
		if(data->demand > ac_capacity)
		temp_demand=ac_capacity;
        else
        temp_demand=data->demand;
		//NEU_MAC
        
        
        //deamnd ratio = (sum of to n fro demands on both nodes on that route)/total deamnd
        if(source ==j+1)
        demand_ratio[j]=0.0;   
        else if(total_demand==0)
        demand_ratio[j]=0;        
        else if(line_sum[source-1]==0)
        demand_ratio[j]=0;
        else
        demand_ratio[j]=temp_demand/line_sum[source-1];
        
        
        //demand_ratio[j]=(data->demand+line_sum[j])/total_demand;
        temp[j]=demand_ratio[j];//copying all demand ratio in a temp array  
        data=data->next_data;
    }    
   
    Bubble_sort(temp,AIRPORT_NO);//(array name,size)
    
    //maximum value of demand ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,AIRPORT_NO,SMALL->demand);//array,size,percentile
    perbig_number = Percentile_calc(temp,AIRPORT_NO,BIG->demand);
    //calculating the median
    median_number= Median_calc(temp,AIRPORT_NO);

    //calculating the demand index for select category
    select=head_select;
    for(i=0;i<AIRPORT_NO;i++)
    {
        // if deamnd ratio >80th percentile, demand index =1
        //else demand index = x/80th percen 
        if(i+1==source)
        select->demand_index=0.0;
        else if(total_demand==0)
        select->demand_index=0.0;
        else if (demand_ratio[i] >perbig_number)
        select->demand_index=1;
        else if(perbig_number==0)
        select->demand_index=0;
        else
        select->demand_index=demand_ratio[i]/perbig_number;
        select = select->next_ap_grey;
   
    }
    
    //calculating the demand index for probable category  
    probable=head_probable;  
    for(i=0;i<AIRPORT_NO;i++)
    {
        //if demand ratio <median ,demand index = demand ratio/median;
        //else if demand ratio >median, demand index = (demand ratio-max value)/(median - max value)
        if(i+1==source)
        probable->demand_index=0.0;
        else if(total_demand==0)
        probable->demand_index=0.0;
        else if(demand_ratio[i]==0 && median_number==0)
        probable->demand_index=0;
        else if(demand_ratio[i]< median_number)
        probable->demand_index=demand_ratio[i]/median_number;
        else if(median_number==max_ratio)
        probable->demand_index=0;
        else 
        probable->demand_index=(demand_ratio[i]-max_ratio)/(median_number-max_ratio);
        probable=probable->next_ap_grey;
    }
    
    //calculating the demand index for reject category
    reject=head_reject;
    for(i=0;i<AIRPORT_NO;i++)
    {

        //if demand ratio <20th percentile ,demand index =1
        //else demand index = (x-max value)/(20th percen - max value)
        if(i+1==source)
        reject->demand_index=1.0;
        else if(total_demand==0)
        reject->demand_index=1.0;
        else if(demand_ratio[i] < persmall_number)
        reject->demand_index=1.0;
        else if(persmall_number==max_ratio)
        reject->demand_index=0;
        else
        reject->demand_index=((demand_ratio[i]-max_ratio)/(persmall_number-max_ratio));
        reject=reject->next_ap_grey;  
    }    

    free(demand_ratio);//NEU_VLA
	free(temp);//NEU_VLA
	free(line_sum);//NEU_VLA
	free(column_sum);//NEU_VLA
    
     //printing the demand index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_demand_index");
             getch();
             exit(1);
         }    
         fprintf(fprocess,"\n\n Total demand = %.2f",total_demand);
         /*
         fprintf(fprocess,"\n Demand concentration => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.4f\t",demand_ratio[j]);    
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);       
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n Demand Index in different categories : (Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
         fprintf(fprocess,"\n");    
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->demand_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->demand_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->demand_index);
                 reject=reject->next_ap_grey;
             }
          
               
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function