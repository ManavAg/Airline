int Select_if_same_index(int,int,int,float,int); 

int Select_if_same_index(int desti1,int desti2,int source,float takeoff_time, int actype)//NEU_MAC
{
    int i,j;
    int selected_index;
    float random_value;
    float desti1_demand,desti2_demand,desti1_cost,desti2_cost,desti1_flytime,desti2_flytime,desti1_qos,desti2_qos;
	float fBH1=0,fBH2=0; //NEU_UT
    //NEU_VLA old: float fly_time[AIRPORT_NO];
	//NEU_VLA start
	float *fly_time=(float *)malloc(AIRPORT_NO*sizeof(float));
	if (fly_time == NULL) 
	{  
	   printf("Couldn't allocate memory\n");
	   exit(1);
	} 
	//NEU_VLA end	

    time_t current_time;
    struct tm * timeinfo;
    struct Data *data;
    FILE *fprocess;
    
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


    //for desti 1
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    
    for(i=1;i<desti1;i++)
    data=data->next_data;
    
    desti1_demand=data->demand;   
    //NEU_MAC old: desti1_cost=data->TOC;		  
	desti1_cost=Get_cost(actype,source,desti1);//NEU_MAC
    if(data->initial_freq_req!=0)
    desti1_qos=(1.0*data->freq_offered)/data->initial_freq_req;
    else
    desti1_qos=1; 
 
  //for desti 2
    data=head_data;
    for(i=1;i<source;i++)
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    
    for(i=1;i<desti2;i++)
    data=data->next_data;
    
    desti2_demand=data->demand;
    //NEU_MAC old: desti2_cost=data->TOC;
	desti2_cost=Get_cost(actype,source,desti2);//NEU_MAC
    if(data->initial_freq_req!=0)
    desti2_qos=(1.0*data->freq_offered)/data->initial_freq_req;
    else
    desti2_qos=1; 
    //flytime
    Get_fly_time(source,takeoff_time,fly_time,AIRPORT_NO);
    desti1_flytime=fly_time[desti1-1];
    desti2_flytime=fly_time[desti2-1];

    
 switch(objective_pointer)
        {
        case 1:
        
                // min- the reverse of demand i.e. max the demand
                if(desti1_demand > desti2_demand)
                selected_index=desti1;
                else if (desti1_demand == desti2_demand)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;    
                
                break;
        case 2:
                // min- the total operating cost
                 if(desti1_cost < desti2_cost)
                selected_index=desti1;
                else if (desti1_cost == desti2_cost)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 3:
                // min- the reverse of time i.e. max time i.e. max ac utilization
                 if(desti1_flytime > desti2_flytime)
                selected_index=desti1;
                else if (desti1_flytime == desti2_flytime)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 4:
                 // min. - the variance in QoS i.e. improving the QoS
                 if(desti1_qos < desti2_qos)
                selected_index=desti1;
                else if (desti1_qos == desti2_qos)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 5:
                 // min. - cost per demand
                 if((desti1_cost/desti1_demand) < (desti2_cost/desti2_demand))
                selected_index=desti1;
                else if ((desti1_cost/desti1_demand) == (desti2_cost/desti2_demand))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 6:
                  // min. - time per demand
                  if((desti1_flytime/desti1_demand) < (desti2_flytime/desti2_demand))
                selected_index=desti1;
                else if ((desti1_flytime/desti1_demand) == (desti2_flytime/desti2_demand))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
		//NEU_UT
		case 7:
		// select the higher utilization
                //blocktime (for utilization function)
				fBH1=Get_route_BT(source,desti1);
				fBH2=Get_route_BT(source,desti2);
				if((fBH1/desti1_flytime) > (fBH2/desti2_flytime))
                selected_index=desti1;
                else if ((fBH1/desti1_flytime) == (fBH2/desti2_flytime))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
		break;
		//NEU_UT
        case 8:
                // min- the demand 
                 if(desti1_demand < desti2_demand)
                selected_index=desti1;
                else if (desti1_demand == desti2_demand)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 9:
                // min- the reverse of cost i.e. max cost
                if(desti1_cost > desti2_cost)
                selected_index=desti1;
                else if (desti1_cost == desti2_cost)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;

        break;
        case 10:
                // min- the total time aircrafts operated
                
                 if(desti1_flytime < desti2_flytime)
                selected_index=desti1;
                else if (desti1_flytime == desti2_flytime)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;

        break;
        case 11:
                // min- the reverse of variance in QoS i.e. max the variance i.e. min QoS
                if(desti1_qos > desti2_qos)
                selected_index=desti1;
                else if (desti1_qos == desti2_qos)
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 12:
                // min. - demand per cost
                
                 if((desti1_demand/desti1_cost) < (desti2_demand/desti2_cost))
                selected_index=desti1;
                else if ((desti1_demand/desti1_cost) == (desti2_demand/desti2_cost))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
        break;
        case 13:
                  // min. - demand per time 
                  if((desti1_demand/desti1_flytime) < (desti2_demand/desti2_flytime))
                selected_index=desti1;
                else if ((desti1_demand/desti1_flytime) == (desti2_demand/desti2_flytime))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;

        break;
		//NEU_UT
		case 14:
			// select the lower utilization
                //blocktime (for utilization function)
				fBH1=Get_route_BT(source,desti1);
				fBH2=Get_route_BT(source,desti2);
				if((fBH1/desti1_flytime) < (fBH2/desti2_flytime))
                selected_index=desti1;
                else if ((fBH1/desti1_flytime) == (fBH2/desti2_flytime))
                {
                    //calculating random variable
                                          current_time=time(NULL);
                                          timeinfo=localtime(&current_time);
                                          srand(time(NULL)*rand()*1000);
                                           random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                          if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                          if(random_value>0.5)
                                          selected_index=desti2;
                                          else
                                          selected_index=desti1;
                }
                else
                selected_index=desti2;
		break;
		//NEU_UT
        default:
        printf("\n Error : Problem in objective function option");
        getch();
        exit(0);
                                        
        }   
        if(user_choice==YES)
        fclose(fprocess); 
		free(fly_time);//NEU_VLA
        return(selected_index);
}//end of function       
