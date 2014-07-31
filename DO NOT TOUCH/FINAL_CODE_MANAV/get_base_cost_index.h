void Get_base_cost_index (int,int);
void Get_base_cost_index(int source, int actype)//NEU_MAC
{
    int i,j,k,flag;//,desti_index; NEU_MAC
    //int freq;
    //float source_cost_ratio,desti_cost_ratio;
    //float cost[AIRPORT_NO];
    //NEU_VLA old: float base_cost_ratio[total_bases];
	//NEU_VLA start
	float *base_cost_ratio=(float *)malloc(total_bases*sizeof(float));
	float *cost_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(total_bases*sizeof(float));
	if (base_cost_ratio == NULL || cost_ratio == NULL || temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end
	//NEU_VLA old:float cost_ratio[AIRPORT_NO],max_ratio,temp_cost_ratio,cost,total_cost;
    float max_ratio,temp_cost_ratio,cost,total_cost;//NEU_VLA
    float temp_demand;
    //NEU_VLA old: float temp[total_bases];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Bases *base;
    struct Grey_indices *select,*probable,*reject;
    
	//NEU_MAC Start
	//initialise variable length arrays 
	float *base_route_cost=(float *)malloc(total_bases*sizeof(float));

	//calculation of specific route cost&total cost
	total_cost=0;
	base=head_base;
	for(i=0;i<total_bases;i++)
	{
		base_route_cost[i]=Get_cost(actype,source,base->base_index);
		total_cost+=base_route_cost[i];
		if(base!=NULL)
		base=base->next_base;
	}
	
	for(k=0;k<total_bases;k++)
	{
		if(total_cost==0)
        base_cost_ratio[k]=0;
        else
        base_cost_ratio[k]=base_route_cost[k]/total_cost;
        temp[k]=base_cost_ratio[k];//copying all cost ratio in a temp array  
	}
	free(base_route_cost);
	//NEU_MAC End


/* NEU_MAC paragraph deactivated start
	//Calculating total cost
	total_cost=0;
    base=head_base;
    for(k=0;k<total_bases;k++)
    {
        data=head_data;
        for(i=1;i<source;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
        
        for(i=1;i<base->base_index;i++)
        data=data->next_data;
        
        total_cost+=data->TOC;
        base=base->next_base;
    } 
    base=head_base;
        //calculating cost ratio from source to all possible base stations
        for(k=0;k<total_bases;k++)
        {
            desti_index=base->base_index;
            //going upto te right line in data array
            data=head_data;
            for(i=1;i<source;i++)
            for(j=0;j<AIRPORT_NO;j++)
            data=data->next_data;
            for(i=1;i<desti_index;i++)
            data=data->next_data;
            //if(source==desti_index)
            //base_cost_ratio[k]=1;
            //else
            if(total_cost==0)
            base_cost_ratio[k]=0;
            else
            base_cost_ratio[k]=data->TOC/total_cost;
            temp[k]=base_cost_ratio[k];//copying all cost ratio in a temp array  
            base=base->next_base;
        }
paragraph deactivated end NEU_MAC */              

    //sorting the array
    Bubble_sort(temp,total_bases);
    //maximum value of cost ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,total_bases,SMALL->cost);//array,size,percentile
    perbig_number = Percentile_calc(temp,total_bases,BIG->cost);
    //calculating the median
    median_number= Median_calc(temp,total_bases);
    
    
    //making an array of time ratio for all the airports
    for(i=0;i<AIRPORT_NO;i++)
    {
        flag=NO;
        //check if i+1th airport is our base airport
        base=head_base;
        for(j=0;j<total_bases;j++)
        {
            if(i+1 == base->base_index)
            {
                //if yes then copy its demand ratio
                flag=YES;
                cost_ratio[i]=base_cost_ratio[j];
                break;
            }
            base=base->next_base;
        }
        //else make ratio=0
        if(flag==NO)
        cost_ratio[i]=1;
    }    

    //calculating the cost index for select category
    
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if cost ratio <20th percentile ,cost index =1
        //else cost index = (x-max value)/(20th percen - max value)
        /*
        if(cost_ratio[j]==0)
        select->cost_index=0.0;
        else if (cost_ratio[j] >perbig_number)
        select->cost_index=1;
        else if(perbig_number==0)
        select->cost_index=0;
        else
        select->cost_index=cost_ratio[j]/perbig_number;
        
        select = select->next_ap_grey;
           */
        if(source==j+1)
        select->cost_index=1;
        else if(cost_ratio[j] < persmall_number)
        select->cost_index=1.0;
        else if(persmall_number==max_ratio)
        select->cost_index=0;
        else
        select->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       select=select->next_ap_grey;     
    }
   
    //calculating the cost index for probable category 
    
    probable=head_probable;    
    for(j=0;j<AIRPORT_NO;j++)
    {
        
        //if cost ratio <median ,cost index = cost ratio/median;
        //else if cost ratio >median, cost index = (cost ratio-max value)/(median - max value)
        if(cost_ratio[j]==0)
        probable->cost_index=0.0;
        else if(cost_ratio[j]< median_number)
        probable->cost_index=cost_ratio[j]/median_number;
        else if (median_number==max_ratio)
        probable->cost_index=0;
        else 
        probable->cost_index=(cost_ratio[j]-max_ratio)/(median_number-max_ratio);
       probable=probable->next_ap_grey;   
       
    }
    
    //calculating the cost index for reject category
    
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {
        
         if(source==j+1)
        reject->cost_index=0.0;
        else if (cost_ratio[j] >perbig_number)
        reject->cost_index=1;
        else if(perbig_number==0)
        reject->cost_index=0;
        else
        reject->cost_index=cost_ratio[j]/perbig_number;
        //data=data->next_data;
        reject = reject->next_ap_grey;
        // if cost ratio >80th percentile, cost index =1
        //else cost index = x/80th percen 
        /*
        if(cost_ratio[j]==0)
        reject->cost_index=1.0;
        else if(cost_ratio[j] < persmall_number)
        reject->cost_index=1.0;
        else if(persmall_number==max_ratio)
        reject->cost_index=0;
        else
        reject->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       reject=reject->next_ap_grey; 
       */
              
    }
    
	free(base_cost_ratio); //NEU_VLA
	free(cost_ratio); //NEU_VLA
	free(temp); //NEU_VLA


     //printing the cost index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_cost_index");
             getch();
             exit(1);
         }    
         
         /*
         fprintf(fprocess,"\n Cost ratio (tons/Rs.) => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",cost_ratio[j]);
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);  
          fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number);   
        */
         fprintf(fprocess,"\n cost Index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->cost_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->cost_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->cost_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
          

    
