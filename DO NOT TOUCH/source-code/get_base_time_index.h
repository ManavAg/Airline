void Get_base_time_index (int,float *,int, float);
void Get_base_time_index(int source,float *flytime,int size,float takeoff_time)
{
    int i,j,k,desti_index,flag;
    float total_fly_time;
    //NEU_VLA old: float base_time_ratio[total_bases];
    //NEU_VLA old: float time_ratio[AIRPORT_NO],max_ratio,temp_time_ratio,time;
	float max_ratio,temp_time_ratio,time;//NEU_VLA
    //NEU_VLA old: float temp[total_bases];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Bases *base;
    struct Grey_indices *select,*probable,*reject;

	//NEU_VLA start
	float *base_time_ratio=(float *)malloc(total_bases*sizeof(float));
	float *time_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(total_bases*sizeof(float));
	if (base_time_ratio == NULL || time_ratio == NULL || temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end

total_fly_time=0;
   base=head_base;
   for(k=0;k<total_bases;k++)
   {
       desti_index=base->base_index;
       total_fly_time+=flytime[desti_index-1];
       base=base->next_base;
   }    
 
   base=head_base;
   for(k=0;k<total_bases;k++)
   {
        desti_index=base->base_index;
        //going upto te right line in data array

            
        if(source==desti_index)//best case i m on base station itself
            base_time_ratio[k]=0;
        else if(flytime[desti_index-1]>(max_time_slot-takeoff_time))//if flying time is more then remaining time, don't go on such routes
            base_time_ratio[k]=1;
        else
            base_time_ratio[k]=flytime[desti_index-1]/total_fly_time;
        temp[k]=base_time_ratio[k]; //copying all time ratio in a temp array 
            
        base=base->next_base;
    }            
 /*
       time=0;
        for(j=0;j<total_bases;j++)
        time+=base_time_ratio[j];
 
        if(time==0)//means all demand satisfied no demand to any base station
        {
            base=head_base;
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
                
                
                if(source==desti_index)
                base_time_ratio[k]=0;
                else
                base_time_ratio[k]=1/flytime[desti_index-1];
                //cost_ratio[i][j]=((((data->TOC)/temp_demand)/fly_time[i*AIRPORT_NO+j])/((system_cost/system_demand)/max_time_slot));
                //cost_ratio[i][j]=(data->TOC/temp_demand)/(system_cost/system_demand);
                //cost_ratio[i][j]=(data->TOC/temp_demand);
                temp[k]=base_time_ratio[k];//copying all cost ratio in a temp array  
                base=base->next_base;
            }    
        }           
   */
    //sorting the array
    Bubble_sort(temp,total_bases);
    //maximum value of time ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,total_bases,SMALL->time);//array,size,percentile
    perbig_number = Percentile_calc(temp,total_bases,BIG->time);
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
                time_ratio[i]=base_time_ratio[j];
                break;
            }
            base=base->next_base;
        }
        //else make ratio=0
        if(flag==NO)
        time_ratio[i]=1;
    }    

    //calculating the time index for select category
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        if(source==j+1)
        select->time_index=1;
        //else if (time_ratio[i][j] >=perbig_number)
       // reject->time_index=1;
        //else
       // reject->time_index=time_ratio[i][j]/perbig_number;
        else if (time_ratio[j]==0 && persmall_number== 0)
        select->time_index=0;
        else if(time_ratio[j] < persmall_number)
        select->time_index=1.0;
        else if(persmall_number==max_ratio)
        select->time_index=0;
        else
        select->time_index=((time_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
        select = select->next_ap_grey;
        /*
        //if time ratio <20th percentile ,time index =1
        //else time index = (x-max value)/(20th percen - max value)
        if(time_ratio[j]==0)
        select->time_index=0.0;
        else if (time_ratio[j] >perbig_number)
        select->time_index=1;
        else if (perbig_number==0)
        select->time_index=0;
        else
        select->time_index=time_ratio[j]/perbig_number;
        select=select->next_ap_grey;  
        */   
    }
    
    //calculating the time index for probable category  
    probable=head_probable;  
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if time ratio <median ,time index = time ratio/median;
        //else if time ratio >median, time index = (time ratio-max value)/(median - max value)
        if(time_ratio[j]==0)
        probable->time_index=0.0;
        else if(time_ratio[j]<median_number)
        probable->time_index=time_ratio[j]/median_number;
        else if (median_number==max_ratio)
        probable->time_index=0;
        else 
        probable->time_index=(time_ratio[j]-max_ratio)/(median_number-max_ratio);
        probable=probable->next_ap_grey;
    }
    
    //calculating the time index for reject category
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {
        /*
        // if time ratio >80th percentile, time index =1
        //else time index = x/80th percen 
        if(time_ratio[j]==0)
        reject->time_index=1.0;
        else if(time_ratio[j] < persmall_number)
        reject->time_index=1.0;
        else if(persmall_number==max_ratio)
        reject->time_index=0;
        else
        reject->time_index=((time_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
        reject = reject->next_ap_grey;
        */
        if(source==j+1)
        reject->time_index=0;
        //else if(time_ratio[i][j] <= persmall_number)
        //select->time_index=1.0;
        //else
        //select->time_index=((time_ratio[i][j]-max_ratio)/(persmall_number-max_ratio));
        //else if( time_ratio[j] == 0 && perbig_number==0)//all demand satisfied
        //select->time_index=0;
        else if (time_ratio[j] >perbig_number)
        reject->time_index=1;
        else if (perbig_number==0)
        reject->time_index=0;
        else
        reject->time_index=time_ratio[j]/perbig_number;
        reject=reject->next_ap_grey;
    }    
     
	free(base_time_ratio);//NEU_VLA
	free(time_ratio);//NEU_VLA
	free(temp);//NEU_VLA
    
     //printing the time index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_time_index");
             getch();
             exit(1);
         }    
         fprintf(fprocess,"\n\n Remaining Operating Time = %.2f",max_time_slot-takeoff_time);
         /*
         fprintf(fprocess,"\n Time ratio (tons/min.) => ");
         for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[j]);
                      fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);  
             
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n Time Index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->time_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->time_index);
                 probable=probable->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->time_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
         
}    //end of function
         
         
         /*
         fprintf(fprocess,"\n\n demand/Time ratio");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[i][j]);
             
         }
           
         fprintf(fprocess,"\n\n demand/Time ratio sorted");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",temp[i*AIRPORT_NO+j]);
             
         }  
         
         fprintf(fprocess,"\n\n %.2f percentile = % .2f",BIG->cost,perbig_number); 
         fprintf(fprocess,"\n %.2f percentile = % .2f",SMALL->cost,persmall_number);
         fprintf(fprocess,"\n median = % .2f",median_number);
         */
       /*
         fprintf(fprocess,"\n\n demand/time Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->time_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n demand/time Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->time_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n demand/time Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->time_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);  
         */
         /*
         fprocess=fopen("Output/TEMP.txt","a");
         fprintf(fprocess,"\n Total System demand = %.2f",system_demand);
         fprintf(fprocess,"\n\n time ratio");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",time_ratio[i][j]);
             
         }
           
         fprintf(fprocess,"\n\n time ratio sorted");
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             fprintf(fprocess," %.2f\t",temp[i*AIRPORT_NO+j]);
             
         }  
         
         fprintf(fprocess,"\n\n %.2f percentile = % .2f",BIG->cost,perbig_number); 
         fprintf(fprocess,"\n %.2f percentile = % .2f",SMALL->cost,persmall_number);
         fprintf(fprocess,"\n median = % .2f",median_number);
         fclose(fprocess);  
         */ 

