#include "bubble_sort.h"
#include "percentile_calc.h"
#include "median_calc.h"
void Get_cost_index(int,int);
void Get_cost_index(int source, int actype) //NEU_MAC

{
    int i,j;
    //int freq;
    //float source_cost_ratio,desti_cost_ratio;
    //float cost[AIRPORT_NO];
    //NEU_VLA old:float cost_ratio[AIRPORT_NO],max_ratio,temp_cost_ratio,cost,total_cost;
	float max_ratio,temp_cost_ratio,cost,total_cost;//NEU_VLA
    float temp_demand;
    //NEU_VLA old: float temp[AIRPORT_NO];
    float perbig_number,persmall_number,median_number; 
    struct Data *data;
    struct Grey_indices *select,*probable,*reject;
    //FILE *fp;
    
    //fp=fopen("Output/Process.txt","a");

	//NEU_VLA start
	float *cost_ratio=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *temp=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (cost_ratio == NULL || temp == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end

	//NEU_MAC Start
	//initialise variable length arrays 
	float *route_cost=(float *)malloc(AIRPORT_NO*sizeof(float));

	//calculation of specific route cost&total cost
	total_cost=0;
	for(i=0;i<AIRPORT_NO;i++)
	{
		route_cost[i]=Get_cost(actype,source,(i+1));
		total_cost+=route_cost[i];
	}

	//calculation of cost ratio
	for(j=0;j<AIRPORT_NO;j++)
	{
		if(source==j+1)
        cost_ratio[j]=0;
        else
        cost_ratio[j]=route_cost[j]/total_cost;
        temp[j]=cost_ratio[j];//copying all cost ratio in a temp array  
	}
	free(route_cost);
	//NEU_MAC End

    /* NEU_MAC paragraph deactivated start
	//going upto the right line
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
    //calculation of total cost 
    total_cost=0;  
    //fprintf(fp,"\n");  
     for(j=0;j<AIRPORT_NO;j++)
     {
         total_cost+=data->TOC;
         //fprintf(fp,"+ %.2f",data->TOC); 
         data=data->next_data;
     } 
	//going to the data point
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;
   	//calculating the cost ratio
    for(j=0;j<AIRPORT_NO;j++)
    {

        if(source==j+1)
        cost_ratio[j]=0;
        else
        cost_ratio[j]=data->TOC/total_cost;

        temp[j]=cost_ratio[j];//copying all cost ratio in a temp array  
        data=data->next_data;
    } paragraph deactivated end NEU_MAC */
             

    //sorting the array
    Bubble_sort(temp,AIRPORT_NO);
    //maximum value of cost ratio is at temp[0]
    max_ratio = temp[0];
    //calculating the percentile
    persmall_number = Percentile_calc(temp,AIRPORT_NO,SMALL->cost);//array,size,percentile
    perbig_number = Percentile_calc(temp,AIRPORT_NO,BIG->cost);
    //calculating the median
    median_number= Median_calc(temp,AIRPORT_NO);

    //calculating the cost index for select category
    //data=head_data;
    select=head_select;
    for(j=0;j<AIRPORT_NO;j++)
    {
        //if cost ratio <20th percentile ,cost index =1
        //else cost index = (x-max value)/(20th percen - max value)
        if(source==j+1)
        select->cost_index=0;
        else if(cost_ratio[j] < persmall_number)
        select->cost_index=1.0;
        else if(persmall_number==max_ratio)
        select->cost_index=0;
        else
        select->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       select=select->next_ap_grey; 
       //data=data->next_data;   
        /*
        if(source==j+1)
        select->cost_index=0.0;
        else if (cost_ratio[j] >perbig_number)
        select->cost_index=1;
        else if(perbig_number==0)
        select->cost_index=0;
        else
        select->cost_index=cost_ratio[j]/perbig_number;
        data=data->next_data;
        select = select->next_ap_grey;
               */
    }
   
    //calculating the cost index for probable category 
    //data=head_data;
    probable=head_probable;    
    for(j=0;j<AIRPORT_NO;j++)
    {
        
        //if cost ratio <median ,cost index = cost ratio/median;
        //else if cost ratio >median, cost index = (cost ratio-max value)/(median - max value)
        if(source==j+1)
        probable->cost_index=0.0;
        else if(cost_ratio[j]==0 && median_number==0)
        probable->cost_index=0;
        else if(cost_ratio[j]< median_number)
        probable->cost_index=cost_ratio[j]/median_number;
        else if (median_number==max_ratio)
        probable->cost_index=0;
        else 
        probable->cost_index=(cost_ratio[j]-max_ratio)/(median_number-max_ratio);
       probable=probable->next_ap_grey;   
       //data=data->next_data;     
    }
    
    //calculating the cost index for reject category
    //data=head_data;
    reject=head_reject;
    for(j=0;j<AIRPORT_NO;j++)
    {
        if(source==j+1)
        reject->cost_index=1;
        else if (cost_ratio[j] >perbig_number)
        reject->cost_index=1;
        else if(perbig_number==0)
        reject->cost_index=0;
        else
        reject->cost_index=cost_ratio[j]/perbig_number;
        //data=data->next_data;
        reject = reject->next_ap_grey;
        
        /*
        // if cost ratio >80th percentile, cost index =1
        //else cost index = x/80th percen 
        if(source==j+1)
        reject->cost_index=1.0;
        else if(cost_ratio[j] < persmall_number)
        reject->cost_index=1.0;
        else if(persmall_number==max_ratio)
        reject->cost_index=0;
        else
        reject->cost_index=((cost_ratio[j]-max_ratio)/(persmall_number-max_ratio));
        
       reject=reject->next_ap_grey; 
       data=data->next_data;        
       */
    }
    
	free(cost_ratio);//NEU_VLA
	free(temp);//NEU_VLA


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
         fprintf(fprocess,"\n Cost ratio  => ");
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
         
          

    
