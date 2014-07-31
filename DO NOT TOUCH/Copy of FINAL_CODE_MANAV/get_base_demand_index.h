//void Get_base_network_design_index (int);
//void Get_base_network_design_index(int source)
void Get_base_demand_index(int );
void Get_base_demand_index(int source)
{
    int i,j,k,flag;
    //NEU_VLA old: float base_demand_ratio[total_bases],demand_ratio[AIRPORT_NO];
    float max_ratio;
    //NEU_VLA old: float temp[total_bases];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Bases *base;
    struct Grey_indices *select,*probable,*reject;
	//NEU_VLA start
	float *base_demand_ratio=(float *)malloc(total_bases*sizeof(float));
	float *temp=(float *)malloc(total_bases*sizeof(float));
	float *demand_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (base_demand_ratio == NULL || temp == NULL || demand_ratio == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
    //NEU_VLA end
    float line_sum;//[AIRPORT_NO],column_sum[AIRPORT_NO],total_demand;
    
   //getting sum of demand from source to all base stations
    base=head_base;
    line_sum=0;
    for(k=0;k<total_bases;k++)
    {
        data=head_data;
        for(i=1;i<source;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
        
        for(i=1;i<base->base_index;i++)
        data=data->next_data;
        
        line_sum+=data->demand;
        base=base->next_base;
        
    }        
   
        
        base=head_base;
        //calculating demand ratio from source to all possible base stations
        for(k=0;k<total_bases;k++)
        {
            //going upto te right line in data array
            data=head_data;
            for(i=1;i<source;i++)
            for(j=0;j<AIRPORT_NO;j++)
            data=data->next_data;
            for(i=1;i<base->base_index;i++)
            data=data->next_data;
           
            //demand ratio = (sum of to n fro demands on both nodes on that route)/total deamnd
            if(line_sum ==0)
            base_demand_ratio[k]=0;
            else
            //demand ratio = demand to destination/ total outgoing demand from that source
            base_demand_ratio[k]=(data->demand)/line_sum;//less demand, less ratio
            //data->current_demand_concentration=demand_ratio[j];
            temp[k]=base_demand_ratio[k];//copying all demand ratio in a temp array 
            base=base->next_base; 
            
        } 
      
    //printf("\n\n Before -");
   // for(i=0;i<AIRPORT_NO;i++)
    //printf("\n %.2f",temp[i]);
    //sorting the array
    Bubble_sort(temp,total_bases);//(array name,size)
    //printf("\n\n After -");
    //for(i=0;i<AIRPORT_NO;i++)
    //printf("\n %.2f",temp[i]);
    //getch();
    
    //maximum value of demand ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,total_bases,SMALL->demand);//array,size,percentile
    perbig_number = Percentile_calc(temp,total_bases,BIG->demand);
    //calculating the median
    median_number= Median_calc(temp,total_bases);
    //making an array of demand ratio for all the airports
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
                demand_ratio[i]=base_demand_ratio[j];
                break;
            }
            base=base->next_base;
        }
        //else make ratio=0
        if(flag==NO)
        demand_ratio[i]=0;
    }    




    //calculating the demand index for select category
    select=head_select;

        for(i=0;i<AIRPORT_NO;i++)
        {
            // if deamnd ratio >80th percentile, demand index =1
            //else demand index = x/80th percen 
            if(demand_ratio[i]==0)
            select->demand_index=0;
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
        if(demand_ratio[i]==0)
        probable->demand_index=0.0;
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
        if(demand_ratio[i]==0)
        reject->demand_index=1.0;
        else if(demand_ratio[i] < persmall_number)
        reject->demand_index=1.0;
        else if(persmall_number==max_ratio)
        reject->demand_index=0;
        else
        reject->demand_index=((demand_ratio[i]-max_ratio)/(persmall_number-max_ratio));
        reject=reject->next_ap_grey;  
    }    
     
	free(base_demand_ratio); //NEU_VLA
	free(temp); //NEU_VLA
	free(demand_ratio); //NEU_VLA


    
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
         fprintf(fprocess,"\n\n Total demand to bases = %.2f",line_sum);
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
         
     
