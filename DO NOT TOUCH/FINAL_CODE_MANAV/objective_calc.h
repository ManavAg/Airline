void Objective_calc(double *);

void Objective_calc(double *obj_value)
{
         
         int i,j,OD_served;
         int system_freq_offered,system_initial_freq_req;
		 //NEU_UT old:float total_demand,total_cost,total_time,start_time;
		 float total_demand,total_cost,total_time,total_utilization,start_time; //NEU_UT
		 float prev_time;
         float demand,cost,time,QoS;
         //NEU_VLA old: float sigma_variance,ave_QoS,sum_QoS,variance_QoS,QoS_all[AIRPORT_NO*AIRPORT_NO];
		 float sigma_variance,ave_QoS,sum_QoS,variance_QoS;//NEU_VLA
		//NEU_VLA start
		float *QoS_all=(float *)malloc((AIRPORT_NO*AIRPORT_NO)*sizeof(float));
		if (QoS_all == NULL) 
		{  
		   printf("Couldn't allocate memory\n");
		   exit(1);
		} 
		//NEU_VLA end
   		 float *utilization=NULL; //NEU_UT
		 struct Fleet *temp_fleet; //NEU_UT
         struct route_detail *item;
         struct extra *extra_item; 
         struct Data *temp_data;
        
        total_demand=0;
        total_cost=0;
        total_time=0;
		total_utilization=0; //NEU_UT
		
		//NEU_UT array for calculating individual utilization for each AC
		utilization = (float *)malloc(fleet_size*sizeof(float));
		//initialise array
		for(i=0;i<fleet_size;i++)
			utilization[i]=0;
		//NEU_UT

        for(i=0;i<fleet_size;i++)
        {
            demand=0;
            cost=0;
            time=0;
            item=head_route;
            while(item->ac_number!=i+1)
                item=item->next;
            start_time=item->takeoff;//first takeoff of the aircraft
                
            item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
                {
                   //calculating total demand carried
                    demand+=item->original_cargo;
                    if(item->extra_detail!=NULL)
                    {
                        
                        extra_item=item->extra_detail;
                        while(extra_item!=NULL)
                        {
                            if(extra_item->extra_cargo!=0)
                            demand+=extra_item->extra_cargo;
                            extra_item=extra_item->next_extra;
                        }    
                    }
					
					//NEU_UT
					//calculating the BH per AC
					if(item->takeoff!=item->landing)
						utilization[i]+=item->landing-item->takeoff;
					//NEU_UT
                    
					//calculating total time and cost taken for carrying this demand
                    if(item->takeoff!=item->landing)
                    {
                        time=item->landing;
                        prev_time=item->landing;
                    }    
                    else
                    {
                        time=prev_time;
                    }    
                    cost+=item->operating_cost;
					
                }  //end of if(item->ac_number==i+1)  
                item=item->next;
            }//end of while(item!=NULL)
            
            total_demand+=demand;
            total_cost+=cost;
            total_time+=(time-start_time);//add only the flying time
        }    //end of for(i=0;i<fleet_size;i++)
        
		//calculation of Utilization NEU_UT
		temp_fleet=head_aircraft_list;
		for(i=0;i<fleet_size;i++)
		{
			utilization[i]=utilization[i]/temp_fleet->life;
			total_utilization+=utilization[i];
			temp_fleet=temp_fleet->next_aircraft;
		}
		total_utilization=total_utilization/fleet_size;
		//NEU_UT
	
        //calculating QoS 
        temp_data=head_data;
        OD_served=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            {
                if(temp_data->initial_freq_req!=0)
                QoS_all[i*AIRPORT_NO+j]=(1.0*temp_data->freq_offered)/temp_data->initial_freq_req;
                else
                QoS_all[i*AIRPORT_NO+j]=1.0;
                if(temp_data->freq_offered!=0)
                OD_served++;
                
            }   
            else
            QoS_all[i*AIRPORT_NO+j]=0;
            
            temp_data=temp_data->next_data; 
        }    
        //if(system_initial_freq_req==0)
        //QoS=1.0;
        //else
        //QoS=((1.0 *system_freq_offered)/system_initial_freq_req)*((1.0*counter)/(AIRPORT_NO*(AIRPORT_NO-1)));
        sum_QoS=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            sum_QoS+=QoS_all[i*AIRPORT_NO+j];
        }    
        ave_QoS=sum_QoS/(AIRPORT_NO*(AIRPORT_NO-1));
        
        
        sigma_variance=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            if(i!=j)
            sigma_variance+=((QoS_all[i*AIRPORT_NO+j]-ave_QoS)*(QoS_all[i*AIRPORT_NO+j]-ave_QoS));
        }    
        
        variance_QoS=sigma_variance/(AIRPORT_NO*(AIRPORT_NO-1));
        //===============objective function=================//
        obj_value[0]=total_demand;
        obj_value[1]=total_cost;
        obj_value[2]=total_time;
        obj_value[3]=variance_QoS; 
		obj_value[4]=total_utilization;//NEU_UT
          

			/*for(i=0;i<fleet_size;i++)//NEU_UT
			printf("\n%.2f\n",utilization[i]);//NEU_UT
			printf("\nOBJV:%.2f\n",obj_value[4]);//NEU_UT*/

		free(utilization);//NEU_BH
		free(QoS_all);//NEU_VLA            
}    
