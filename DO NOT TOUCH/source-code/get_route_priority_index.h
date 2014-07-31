void Get_route_priority_index(int );

void Get_route_priority_index(int source)
{
    int i,j;
    float max_priority;
    //NEU_VLA old:float temp[AIRPORT_NO];
	//NEU_VLA start
	float *temp=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Grey_indices *select,*probable,*reject;
        
    //copying all priorities in a temp array  
            //going upto te right line in data array
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    
    for(j=0;j<AIRPORT_NO;j++)
    {
        temp[j]=data->priority;
        data=data->next_data;
    }    
    //sorting the array
    Bubble_sort(temp,AIRPORT_NO);
    //maximum value of time ratio is at temp[0]
    max_priority = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,AIRPORT_NO,SMALL->route_priority);//array,size,percentile
    perbig_number = Percentile_calc(temp,AIRPORT_NO,BIG->route_priority);
    //calculating the median
    median_number= Median_calc(temp,AIRPORT_NO);

    //calculating the route prioroity index for select category
    select=head_select;
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if priority > BIG percentile, route_priority_index=1
        //else route_priority_index = (priority+1)/(BIG percentile+1)
        if(data->priority==1)
        select->route_priority_index=1;
        else if (data->priority==-1)
        select->route_priority_index=0;
        else if(source==j+1)
        select->route_priority_index=0.0;
        else if (data->priority >= perbig_number)
        select->route_priority_index=1.0;
        else
		select->route_priority_index=(data->priority)/(perbig_number);//NEU
		//NEU old: select->route_priority_index=(data->priority+1)/(perbig_number+1);
        select=select->next_ap_grey;     
        data=data->next_data;
    }

    //calculating the route prioroity index for probable category
    probable=head_probable;
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if priority > Median, route_priority_index=(priority-max prioroity)/(median-max priority)
        //else route_priority_index = (priority+1)/(median+1)
        if(data->priority==1)
        probable->route_priority_index=0;
        else if (data->priority==-1)
        probable->route_priority_index=0;
        else if(source==j+1)
        probable->route_priority_index=0.0;
        else if(median_number==max_priority)
        probable->route_priority_index=1;
        else if (data->priority < median_number)
		probable->route_priority_index=(data->priority)/(median_number);//NEU
        //NEU old: probable->route_priority_index=(data->priority+1)/(median_number+1);
        else if(median_number==max_priority)
        probable->route_priority_index=0;
        else
        probable->route_priority_index=((data->priority-max_priority)/(median_number-max_priority));
        

        probable=probable->next_ap_grey;     
        data=data->next_data;
    }
    

    //calculating the route prioroity index for reject category
    reject=head_reject;
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    for(j=0;j<AIRPORT_NO;j++)
    {

        //if priority <small percentile, route_priority_index=1;
        //else route_priority_index=(priority-max prioroity)/(small percentile-max priority)
        if(data->priority==1)
        reject->route_priority_index=0;
        else if (data->priority==-1)
        reject->route_priority_index=1;
        else if(source==j+1)
        reject->route_priority_index=1.0;
        else if (data->priority < persmall_number)
        reject->route_priority_index=1.0;
        else if (persmall_number==max_priority)
        reject->route_priority_index=0;
        else
        reject->route_priority_index=((data->priority-max_priority)/(persmall_number-max_priority));

        reject=reject->next_ap_grey;     
        data=data->next_data;
    }

	free(temp);//NEU_VLA

    //printing the route priority index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_route_priority_index");
             getch();
             exit(1);
         } 
         /*
         fprintf(fprocess,"\n");
         for(i=0;i<AIRPORT_NO;i++)
         fprintf(fprocess,"  %.2f\t",temp[i]);   
         
         fprintf(fprocess," \n %.2f\t%.2f\t%.2f",persmall_number,median_number,perbig_number); 
         */
         fprintf(fprocess,"\n route_priority_index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->route_priority_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->route_priority_index);
                 probable=probable->next_ap_grey;
             }
              fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->route_priority_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
         
         
         /*
         fprintf(fprocess,"\n\n Route Priority Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->route_priority_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n Route Priority Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->route_priority_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n Route Priority Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->route_priority_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);     
     }//end of if(user_choice==YES)  
     
}   //end of function 

*/
