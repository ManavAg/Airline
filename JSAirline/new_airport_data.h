int New_airport_data(struct Fleet *, choices *);

int New_airport_data(struct Fleet *candidate, choices * prev_choices)
{
    
    int i,j;
    int HIGHEST_PRIORITY_FLAG;//to check if we gor any base station with priority=1
    int flag;
    int best_index,selected_index;
    char best_cat;
    float best_grey_index,best_demand;
	float temp_capacity; //NEU_MAC
    int ac_req;
	int *ap_with_belf=NULL;//NEU_SH
    //NEU_VLA old: float ap_demand[AIRPORT_NO],ap_capacity[AIRPORT_NO],line_sum[AIRPORT_NO],BELF_each[AIRPORT_NO]; //NEU_SH
	//NEU_SH old:float ap_demand[AIRPORT_NO],ap_capacity[AIRPORT_NO],line_sum[AIRPORT_NO],capacity_each[AIRPORT_NO],BELF_each[AIRPORT_NO];
    //NEU_VLA start
	float *ap_demand=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *ap_capacity=(float *)malloc(AIRPORT_NO*sizeof(float));
	float *line_sum=(float *)malloc(AIRPORT_NO*sizeof(float));
	//float *BELF_each=(float *)malloc(AIRPORT_NO*sizeof(float));
	



float** BELF_each;

int numRows, numCols;
/* Used as indexes as in matrix[x][y]; */
int x, y; 
/*
 * Get values into numRows and numCols somehow.
 */


/* Allocate pointer memory for the first dimension of a matrix[][]; */
BELF_each = (float **) malloc(AIRPORT_NO * sizeof(float *));
/* Allocate integer memory for the second dimension of a matrix[][]; */
for(x = 0; x < AIRPORT_NO; x++)
{
    BELF_each[x] = (float *) malloc(AIRPORT_NO * sizeof(float));
   }





if (ap_demand == NULL || ap_capacity == NULL || line_sum == NULL || BELF_each == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end	
	float BELF;
        float random_value;
    time_t current_time;
    struct tm * timeinfo;
    struct Data *data,*candidate_data;
    struct Airports *airport,*candidate_airport;
    struct Fleet *temp_head;
    struct Final_grey *grey;
    choices *check_choices;//,*prev_choice;
    FILE *fprocess;
    //FILE *fp;
    //fp=fopen(output_filename,"a");
    //fprintf(fp,"\n new search for aircraft %d, source %d",candidate->ac_index,candidate->ap_index);
    
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in new_airport_data");
            getch();
            exit(1);
        }    
    }        

    //doing grey analysis of all the airports
    //Grey_analysis();
    //going upto the airport where our candidate aircraft is available
    data=head_data;
    for(i=1;i<candidate->ap_index;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    candidate_data=data;
    //reading the demand and capacity from candidate airport to all other airports
    for(j=0;j<AIRPORT_NO;j++)
    {
        ap_demand[j]=candidate_data->demand;
        ap_capacity[j]=Get_capacity(candidate->ac_type,candidate->ap_index,(j+1));//NEU_MAC
		//NEU_MAC old: ap_capacity[j]=candidate_data->capacity;
        candidate_data=candidate_data->next_data;
    }    

    //reading the BELF for takeoff from the candidate airport
    airport=head_airport;
    for(i=1;i<candidate->ap_index;i++)
    airport=airport->next_airport;
//commented by manav
	//    BELF=airport->BELF;


    //starting the search
    best_index=-1;
    /*
    if(best_index==-1)  
    {
        printf("\n looking if any other airport has demand>BELF");
        if(user_choice ==YES)
        fprintf(fprocess,"\n\n looking if any other airport has demand>BELF");
        //going upto the our candidate airport
        grey=head_final_grey;
        for(i=1;i<candidate->ap_index;i++)
        for(j=0;j<AIRPORT_NO;j++)
        grey=grey->next_ap_index;
        
        for(i=0;i<AIRPORT_NO;i++)
        {
            //first look for which airports demand>BELF
            //then among those airports which one is best as per grey indices
            if((ap_demand[i] >= (ap_capacity[i]*BELF/100)) && ap_capacity[i]!=0 )
            {
                //look only for those airports which have not been checked till now
                   check_choices=prev_choices;
                   flag=1;
                   do
                   {
                       if(((check_choices->index)-1)==i)
                       {
                           flag=0;
                           printf("\n Airport number %d has been checked previously",i+1); 
                           if(user_choice==YES)
                            fprintf(fprocess,"\n Airport number %d has been checked previously",i+1);
                          
                           break;
                       }
                       check_choices=check_choices->next;
                   }while(check_choices!=NULL);    
                  
                   if(flag==1)
                   {
                    printf("\n looking for airport number = %d as an option to go",i+1);
                    if(user_choice ==YES)
                               fprintf(fprocess,"\n looking for airport number = %d as an option to go",i+1);
                    if(best_index==-1)//means this is the first node
                    {
                         // if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
                         // {
                              best_index=i;
                              best_cat=grey->grey_char;
                              best_grey_index=grey->grey_index;
                        //  }    
                    }    
                    else
                    {
                       if(best_cat=='s'&& grey->grey_char=='s')
                       {
                           if(best_grey_index < grey->grey_index)
                           {
                               printf("\n changing the best selection becouse we got better option in select category");
                               if(user_choice ==YES)
                               fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
                               best_index=i;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                           }    
                       }    
                       else if(best_cat!='s'&& grey->grey_char=='s')
                       {
                           printf("\n changing the best selection becouse of category");
                           if(user_choice ==YES)
                                fprintf(fprocess,"\n changing the best selection becouse of category");
                           
                           best_index=i;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                       }    
                       else if(best_cat=='p'&& grey->grey_char =='p')
                       {
                           if(best_grey_index < grey->grey_index)
                           {
                               printf("\n changing the best selection becouse we got better option in probable category");
                               if(user_choice ==YES)
                                    fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
                               best_index=i;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                           }    
                       } 
                       
                       
                                // next two sections - allowing reject category also as an option to go
                           //to remoce this, make next two section comment nad at start umcomment if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
                             
                            else if(best_cat=='r'&& grey->grey_char!='r')
                           {
                               printf("\n changing the best selection becouse of category");
                               if(user_choice ==YES)
                                    fprintf(fprocess,"\n changing the best selection becouse of category");
                               
                               best_index=i;
                               best_cat=grey->grey_char;
                               best_grey_index=grey->grey_index;
                           }    
                           else if(best_cat=='r'&& grey->grey_char =='r')
                           {
                               if(best_grey_index > grey->grey_index)
                               {
                                   printf("\n changing the best selection becouse we got better option in reject category");
                                   if(user_choice ==YES)
                                        fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category");
                                   best_index=i;
                                   best_cat=grey->grey_char;
                                   best_grey_index=grey->grey_index;
                               }    
                           } 
                     
                      }//end of else 
                }//end of if(flag==1)    
            }//end of if((ap_demand[i] >= (ap_capacity[i]*BELF/100)) && ap_capacity[i]!=0 )
         grey=grey->next_ap_index;             
        }//end of for(i=0;i<AIRPORT_NO;i++)        
    }//end of  if(best_index==-1)          
    */
    if(best_index==-1)  
    {
        //printf("\n looking if any other airport has outgoing demand>BELF");
        //if(user_choice ==YES)
       // fprintf(fprocess,"\n\n looking if any other airport has outgoing demand>BELF");
        
        //printf("\n looking if for any airport demand >BELF or outgoing demand at any airport >BELF");
        if(user_choice ==YES)
		{fprintf(fprocess,"\n\n Looking if for any airport demand to min 50percent of routes >BELF or outgoing demand to dest. airport >BELF"); //NEU_SH
			for(i=0;i<AIRPORT_NO;i++)
		    for(j=0;j<AIRPORT_NO;j++)
			{printf("\n from airport %d to airport %d = %.2f",i+1,j+1,BELF_each[i][j]);
			printf("**********************************************************************");}
			getch();}	//NEU_SH old: fprintf(fprocess,"\n\n looking if for any airport demand >BELF or outgoing demand at any airport >BELF");
        
        //BELF at each airport
       data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            BELF_each[i][j]=data->BELF;
            data=data->next_data;
        }

        /* NEU_SH deactivation start (line_sum calculation not needed anymore)
		// Initializing line_sum  to zero
        for(i=0;i<AIRPORT_NO;i++)
        {
        line_sum[i]=0;         
        }
        //sum of all outgoing demands at each airport =line sum
        data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            line_sum[i]+=data->demand;
            data=data->next_data;
        }

		//at each airport look how many aircraft already available there and dose it require any more Aircraft                          
        for(i=0;i<AIRPORT_NO;i++)
        {
            //*****temp_head=candidate;//list starts from here, not from head_fleet
            temp_head=candidate;
           // for(j=0;j<fleet_size;j++)
            while(temp_head!=NULL)
            {
                if((temp_head->ap_index==i+1)&&(temp_head->ac_index!=candidate->ac_index))
                {
                    if(user_choice==YES)
                    fprintf(fprocess,"\n Aircraft no. %d is available at airport %d",temp_head->ac_index,i+1);
                    line_sum[i]-=capacity_each[i];
                }   
               
                temp_head=temp_head->next_aircraft;
            }
        } //NEU_SH deactivation end*/
        
		/* NEU_MAC deactivation start (capacity_each[] not needed anymore)
        //takeoff capacity of aircraft at each airport
        data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            capacity_each[i]=data->capacity;//reading the first entry in capacity line
            for(j=0;j<AIRPORT_NO-1;j++)//going to last element in that line
            data=data->next_data;
            if(capacity_each[i]==0)//if first entry is zero, reading last entry
            {
                capacity_each[i]=data->capacity;
            }
            data=data->next_data;//going to next capacity line    
        } NEU_MAC deactivation end */    

        //printing the outgoing demands,takeoff capacity and BELF from each airport
        if(user_choice ==YES)
        {
            //NEU_MAC old: fprintf(fprocess,"\n\t Outgoing demands, Takeoff capacity and BELF ");
			fprintf(fprocess,"\n\t Outgoing Demand, Outgoing Capacity, BELF ");
            for(i=0;i<AIRPORT_NO;i++)
			 for(j=0;j<AIRPORT_NO;j++)
				fprintf(fprocess,"\n from airport %d to airport %d = %.2f, %.2f",i+1,j+1,ap_demand[i],ap_capacity[i]);
				//NEU_MAC old: fprintf(fprocess,"\n from airport %d = %.2f, %.2f and %.2f",i+1,line_sum[i],capacity_each[i],BELF_each[i]);
        }

		//NEU_SH
		//counting the no. of directions with demand>BELF from each airport
		ap_with_belf = (int *)malloc(AIRPORT_NO*sizeof(int));
		data=head_data;
		for(i=0;i<AIRPORT_NO;i++)
		ap_with_belf[i]=0;
		i=0;
		while(data!=NULL)
		{
			
			j=0;
			BELF=data->BELF;
			while(data!=NULL && j<AIRPORT_NO)
			{
				temp_capacity=Get_capacity(candidate->ac_type,i+1,j+1); //NEU_MAC
				//NEU_MAC old: if (data->demand >= (data->capacity*BELF_each[i]) && data->capacity != 0)
				if (data->demand >= (temp_capacity*BELF_each[i][j]) && temp_capacity != 0) //NEU_MAC
				ap_with_belf[i]++;
				data=data->next_data;
				j++;
			}
			//printf("%d\t",ap_with_belf[i]);
			if(user_choice ==YES)
				fprintf(fprocess,"\n Airport %d: %d",i+1,ap_with_belf[i]);//NEU_SH NEU_MAC
			i++;
		}
		//printf("\n\n");
		//NEU_SH

		//NEU_MAC: No modifications needed beyond this point

        //going upto the our candidate airport
        grey=head_final_grey;
         
        //going upto the airport where our candidate aircraft is available
        data=head_data;
        for(i=1;i<candidate->ap_index;i++)
        for(j=0;j<AIRPORT_NO;j++)
        data=data->next_data;   
        
        HIGHEST_PRIORITY_FLAG=NO;
               
        for(i=0;i<AIRPORT_NO;i++)
        {
           if(data->priority!=-1)//don't look the destination if priority=-1
            {
             if((data->priority==1)||(HIGHEST_PRIORITY_FLAG==NO))
				 {  
					 //NEU_SH old:if((  (line_sum[i] >= (capacity_each[i]*BELF_each[i]/100.0)) && (candidate->ap_index!=i+1) ) || ((ap_demand[i] >= (ap_capacity[i]*BELF/100)) && ap_capacity[i]!=0 ))
					 //if ((min. 50% of tertiary AP from Dest. AP have Demand>BELF or demand from candidate AP to dest AP > BELF
					 //min 1:if(((ap_with_belf[i] >= 1 ) && (candidate->ap_index!=i+1)) || ((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
					 if((ap_with_belf[i] >= (AIRPORT_NO/2) && (candidate->ap_index!=i+1)) || ((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
					 //without tert.AP:if(((ap_demand[i] >= ap_capacity[i]*BELF) && ap_capacity[i]!=0))//NEU_SH
					 {
						flag=1;
						/*
						//if first tiem coming in this loop i.e. outgoing demand>BELF then delet all the previous record
						if(ASK_FLAG==NO)//menas i m coming first time in this block, after demand>capacity block in get_airport_data
						{
							//delete all the record of previous choices
							check_choices=prev_choices;
							while(check_choices!=NULL)
							{
								prev_choice=check_choices;
								check_choices=check_choices->next;
								free(prev_choice);
							}
							//making head pointer of choices to NULL again    
							head_choice=NULL;
							ASK_FLAG=YES;//do it when first time coming in this block after that keep the record
						} 
						else
						{ 
							*/              
		                
						//look only for those airports which have not been checked till now
						   check_choices=prev_choices;
		                   
						   do
						   {
							   if(((check_choices->index)-1)==i)
							   {
								   flag=0;
								   //printf("\n Airport number %d has been checked previously",i+1); 
								   if(user_choice==YES)
									fprintf(fprocess,"\n Airport number %d has been checked previously",i+1);
		                            
								   break;
							   }
							   check_choices=check_choices->next;
						   }while(check_choices!=NULL);    
					  // }    
		               
		                    
					  if(flag==1)
						   {
						   // printf("\n looking for airport number = %d as an option to go",i+1);
							if(user_choice ==YES)
									   fprintf(fprocess,"\n looking for airport number = %d as an option to go",i+1);
		                    
							//fprintf(fp,"\n looking for airport number = %d as an option to go",i+1);
		                    
							   if(data->priority==1 && HIGHEST_PRIORITY_FLAG==NO)//first highest priority route, 
							   {
								HIGHEST_PRIORITY_FLAG=YES;
								best_index=-1; 
								if(user_choice ==YES)
									   fprintf(fprocess,"\n Airport number %d has the highest priority (1) to go",i+1);                 
		                        
								}
		                    
		                    
		                    
		                    
							if(best_index==-1)//means this is the first node
							{
								  //if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
								  //{
									  best_index=i;
									  best_cat=grey->grey_char;
									  best_grey_index=grey->grey_index;
									  best_demand=data->demand;
								 // }    
							}    
							else
							{
							   if(best_cat=='s'&& grey->grey_char=='s')
							   {
								   if(best_grey_index < grey->grey_index)
								   {
									  // printf("\n changing the best selection becouse we got better option in select category");
									   if(user_choice ==YES)
									   fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
									   best_index=i;
									   best_cat=grey->grey_char;
									   best_grey_index=grey->grey_index;
									   best_demand=data->demand;
								   } 
								   else if((best_grey_index == grey->grey_index))
								   {
										  selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
									   if(selected_index==(i+1))
									   {
											  best_index=i;
											  best_cat=grey->grey_char;
											  best_grey_index=grey->grey_index;
											  best_demand=data->demand;
		                                 
									   }
										  /*
										  //calculating random variable
										  current_time=time(NULL);
										  timeinfo=localtime(&current_time);
										  srand(time(NULL)*rand()*1000);
										   random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
										  if(user_choice ==YES)
										   fprintf(fprocess,"\n random value = %.2f",random_value);
										  if(random_value>0.5)
										  {
											  // printf("\n changing the best selection because we got better option in select category(with more demand)");
											   if(user_choice ==YES)
											   fprintf(fprocess,"\n changing the best selection becouse we got better option in select category (randomly)");
											   best_index=i;
											   best_cat=grey->grey_char;
											   best_grey_index=grey->grey_index;
											   best_demand=data->demand;
										  }    
										  */
								   }
							   }    
							   else if(best_cat!='s'&& grey->grey_char=='s')
							   {
								  // printf("\n changing the best selection becouse of category");
								   if(user_choice ==YES)
										fprintf(fprocess,"\n changing the best selection becouse of category");
		                           
								   best_index=i;
								   best_cat=grey->grey_char;
								   best_grey_index=grey->grey_index;
								   best_demand=data->demand;
							   }    
							   else if(best_cat=='p'&& grey->grey_char =='p')
							   {
								   if(best_grey_index < grey->grey_index)
								   {
									  // printf("\n changing the best selection becouse we got better option in probable category");
									   if(user_choice ==YES)
											fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
									   best_index=i;
									   best_cat=grey->grey_char;
									   best_grey_index=grey->grey_index;
									   best_demand=data->demand;
								   }
								   else if((best_grey_index == grey->grey_index))
								   {
									   selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
									   if(selected_index==(i+1))
									   {
											  best_index=i;
											  best_cat=grey->grey_char;
											  best_grey_index=grey->grey_index;
											  best_demand=data->demand;
		                                 
									   }
									   /*
										  //calculating random variable
										  current_time=time(NULL);
										  timeinfo=localtime(&current_time);
										  srand(time(NULL)*rand()*1000);
										  random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
										  if(user_choice ==YES)
										   fprintf(fprocess,"\n random value = %.2f",random_value);
										  if(random_value>0.5)
										  {
											  // printf("\n changing the best selection because we got better option in select category(with more demand)");
											   if(user_choice ==YES)
											   fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category (randomly)");
											   best_index=i;
											   best_cat=grey->grey_char;
											   best_grey_index=grey->grey_index;
											   best_demand=data->demand;
										  }    
										  */
								   }   
							   } 
		                     
		                     
		                     
		                     
										  // next two sections - allowing reject category also as an option to go
									   //to remoce this, make next two section comment nad at start umcomment if(grey->grey_char!='r')//this ensure that only those of p and s cat are selected as first choice
		                                 
										else if(best_cat=='r'&& grey->grey_char!='r')
									   {
										   //printf("\n changing the best selection becouse of category");
										   if(user_choice ==YES)
												fprintf(fprocess,"\n changing the best selection becouse of category");
		                                   
										   best_index=i;
										   best_cat=grey->grey_char;
										   best_grey_index=grey->grey_index;
										   best_demand=data->demand;
									   }    
									   else if(best_cat=='r'&& grey->grey_char =='r')
									   {
										   if(best_grey_index > grey->grey_index)
										   {
											   //printf("\n changing the best selection becouse we got better option in reject category");
											   if(user_choice ==YES)
													fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category");
											   best_index=i;
											   best_cat=grey->grey_char;
											   best_grey_index=grey->grey_index;
											   best_demand=data->demand;
										   }
										  else if((best_grey_index == grey->grey_index))
										   {
												  selected_index=Select_if_same_index(best_index+1,i+1,candidate->ap_index,candidate->takeoff_time,candidate->ac_type,candidate);//desti1,desti2,source NEU_MAC (+candidate->ac_type)
												   if(selected_index==(i+1))
												   {
														  best_index=i;
														  best_cat=grey->grey_char;
														  best_grey_index=grey->grey_index;
														  best_demand=data->demand;
		                                             
												   }
												  /*
												  //calculating random variable
												  current_time=time(NULL);
												  timeinfo=localtime(&current_time);
												  srand(time(NULL)*rand()*1000);
												   random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
												  if(user_choice ==YES)
												  fprintf(fprocess,"\n random value = %.2f",random_value);
												  if(random_value>0.5)
												  {
													  // printf("\n changing the best selection because we got better option in select category(with more demand)");
													   if(user_choice ==YES)
													   fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category (randomly)");
													   best_index=i;
													   best_cat=grey->grey_char;
													   best_grey_index=grey->grey_index;
													   best_demand=data->demand;
												  }    
												  */
										   } 
									   } 
							  }//end of else 
					   }//end of if(flag==1)    
					}//end of   if(line_sum[i] >= (capacity_each[i]*BELF_each[i]/100) && (candidate->ap_index!=i+1) )
				}//end of if((data->priority==1 &&  HIGHEST_PRIORITY_FLAG==YES)||HIGHEST_PRIORITY_FLAG==NO)    
				else
				 {
					 if(user_choice ==YES)
								  fprintf(fprocess,"\n The Airport %d don't have priority = 1",i+1);
	                 
	                 
				 }    
				 }//end of if(data->priority!=-1)
				 else
				 {
					 if(user_choice ==YES)
								  fprintf(fprocess,"\n The Airport %d has the priority = -1",i+1);
	                 
				 }
            grey=grey->next_ap_index;  
            data=data->next_data;

        }//end of   for(i=0;i<AIRPORT_NO;i++)       
        
    }//end of if(best_index==-1)     
//fclose(fp);
		free(ap_with_belf);//NEU_SH
		free(ap_demand);//NEU_VLA
		free(ap_capacity);//NEU_VLA
		free(line_sum);//NEU_VLA
		free(BELF_each);//NEU_VLA
//returning the possible destination               
           if(best_index==-1)  
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n No Possible destination ");
                   fclose(fprocess); 
               }
               return(NOT_FOUND);
           }    
           else
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n Possible destination = %d ",(best_index+1));
                   fclose(fprocess); 
               }    
              return(best_index+1);
           } 
}//end of function    
