void Write_output();
void Write_output()
{
    int i,j;
    int OD_served;
    int freq,flag;
    int system_freq_offered,system_initial_freq_req;
    float demand,cost,route_time,objective_value,index[total_grey_indices],QoS,remaining_demand;
    //NEU_UT old:	float total_demand,total_cost,total_time,start_time;
	float total_demand,total_cost,total_time,start_time,total_utilization;//NEU_UT
	float prev_route_time;
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
    struct Grey *grey_priority;
    struct Data *temp_data;
    
    time_t current_time;
    struct tm * timeinfo;
    
                 time(&current_time);
                 timeinfo = localtime ( &current_time );
    
    FILE *fprocess=fopen(output_filename,"w");
                if(fprocess==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",output_filename);
                    printf("\n cann't make the "); puts(output_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
                fprintf(fprocess," -------Airline Network Optimization for cargo Airlines--------BETA version");
                fprintf(fprocess,"\n >>>>****for testing and evalution only **** not for commercial use****<<<<\n");
                fprintf(fprocess,"\n ===========================================================================");
                fprintf(fprocess,"\n This program is designed and developed at ");
                fprintf(fprocess,"\n Aerospace Engineering Department, IIT Bombay \n");    
                fprintf(fprocess,"\n For any query/suggestion, contact - ");
                fprintf(fprocess,"\n Prof. Rajkumar S. Pant , LTA systems lab, rkpant@aero.iitb.ac.in");
                fprintf(fprocess,"\n Ajeet Singh, MTech Student(2005-2007), ajeet@aero.iitb.ac.in");
				fprintf(fprocess,"\n Florian Maul, Technical University of Munich, florian.maul@mytum.de");
                fprintf(fprocess,"\n ===========================================================================\n");

                fprintf(fprocess,"\n The Schedule is generated on :\n %s ",asctime(timeinfo));//writing the start time
                fprintf(fprocess,"\n ===========================================================================\n");
                //fprintf(fp," Objective function value = %f\n",it);//writing the best objective value returned by GA
                //fprintf(fp,"\n ===========================================================================\n");
           /*
             fclose(fprocess);
         FILE *fprocess=fopen(output_filename,"a");
        if(fprocess==NULL)
        {
            //printf("\n cann't make the %s file in write_output",output_filename);
            printf("\n cann't make the "); puts(output_filename);printf(" file in write_output");
            getch();
            exit(1);
        }    
        */
        fprintf(fprocess,"\n\n ------------------------------------------");
        fprintf(fprocess,"\n System Detail");
        fprintf(fprocess,"\n ------------------------------------------");
        fprintf(fprocess,"\n Aircraft\tDemand\tCost\tTime");
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
            route_time=0;
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
                        route_time=item->landing;
                        prev_route_time=item->landing;
                    }    
                    else
                    {
                        route_time=prev_route_time;
                    }    
                    cost+=item->operating_cost;
                }    
                item=item->next;
            }
            fprintf(fprocess,"\n   %d\t\t%.2f\t%.2f\t%.2f",i+1,demand,cost,route_time-start_time);
            total_demand+=demand;
            total_cost+=cost;
            total_time+=(route_time-start_time);//add only the flying time
        } //end of for(i=0;i<fleet_size;i++)

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

        fprintf(fprocess,"\n ------------------------------------------");
        //fprintf(fprocess,"\n Sum\t\t%.2f\t%.2f\t%.2f\t%.2e\t%.2e\t%.2e",total_demand,total_cost,total_time,(total_cost/total_demand)/total_time,(total_cost/total_demand),total_demand/total_time); 
        fprintf(fprocess,"\n Sum\t\t%.2f\t%.2f\t%.2f",total_demand,total_cost,total_time); 
        
        grey_priority=head_grey;
        for(i=0;i<total_grey_indices;i++)
        {
            switch(i)
                {
                    case 0:
                        index[0]=grey_priority->priority;//priority of demand index 
                        break;
                    case 1:
                        index[1]=grey_priority->priority;//priority of cost index 
                        break;
                    case 2:
                        index[2]=grey_priority->priority;//priority of time index 
                        break;
                        
                  /*
                     case 3:
                        index[3]=grey_priority->priority;  //priority of network design index
                        break;
                        */
                    case 3:
                        index[3]=grey_priority->priority;//priority of route priority index
                        break;
                        
                    
                    default :
                        {
                            printf("\n Error in reading grey indices priorities in write_output\n Please check the input data");
                            getch();
                            exit(1);
                        }    
                    
                }//end of switch  
         
                grey_priority=grey_priority->next_grey;
         }
         
         
        //objective_value =(index[0]*total_demand)+(index[1]/total_cost)+(index[2]/total_time);
       // objective_value =(index[0]*total_demand)-(index[1]*total_cost)-(index[2]*total_time);
        fprintf(fprocess,"\n\n Priority of Demand Index  \t= %.4f",index[0]);
        fprintf(fprocess,"\n Priority of Cost Index  \t= %.4f",index[1]);
        fprintf(fprocess,"\n Priority of Time index  \t= %.4f",index[2]);
        //fprintf(fprocess,"\n Priority of Network Design index = %.2f",index[3]);
        fprintf(fprocess,"\n Priority of Route Prio. index \t= %.4f",index[3]);
        
        //fprintf(fprocess,"\n Priority of Route priority index = %.2f",index[3]);
        //fprintf(fprocess,"\n Priority of Quality of service Index = %.2f",index[4]);
        //fprintf(fprocess,"\n\n Objective function value = %.4f",objective_value);
        /*
        system_initial_freq_req=0;
        system_freq_offered=0;
        temp_data=head_data;
        counter=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            system_initial_freq_req +=temp_data->initial_freq_req;
            if(i!=j)
            system_freq_offered+=temp_data->freq_offered;
            if(temp_data->freq_offered!=0 && i!=j)
            counter++;
            temp_data=temp_data->next_data;
        } 
        
        if(system_initial_freq_req==0)
        QoS=1.0;
        else
        QoS=((1.0 *system_freq_offered)/system_initial_freq_req)*((1.0*counter)/(AIRPORT_NO*(AIRPORT_NO-1)));
       */
         /*
        fprintf(fprocess,"\n\n");
        fprintf(fprocess,"\t%.2e",(total_cost/total_demand)/total_time);
        fprintf(fprocess,"\t%.2e",total_cost/total_demand);
        fprintf(fprocess,"\t%.2e",total_demand/total_cost);
        fprintf(fprocess,"\t%.2e",total_demand/total_time);
        fprintf(fprocess,"\t%.2e",QoS);
        fprintf(fprocess,"\t%d",counter);
       */
       temp_data=head_data;
        /*
        counter=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
        {
            system_initial_freq_req +=temp_data->initial_freq_req;
            if(i!=j)
            system_freq_offered+=temp_data->freq_offered;
            if(temp_data->freq_offered!=0 && i!=j)
            counter++;
            temp_data=temp_data->next_data;
        } 
        */
       
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
       
       temp_data=head_data;
       remaining_demand=0;
        for(i=0;i<AIRPORT_NO;i++)
        for(j=0;j<AIRPORT_NO;j++)
            {
                remaining_demand+=temp_data->demand;
                temp_data=temp_data->next_data;
            }
            
        fprintf(fprocess,"\n\n Tons/Lakhs Rs \t= %.4f",total_demand/total_cost);
        fprintf(fprocess,"\n Tons/Hour \t= %.4f",total_demand*60/total_time);
        fprintf(fprocess,"\n Variance QOS \t= %.4f",variance_QoS);
		fprintf(fprocess,"\n\n Fleet Utilization [%%] \t= %.3f",total_utilization*100);//NEU_UT
        fprintf(fprocess,"\n No. of OD pair served \t= %d",OD_served);
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Network Schedule");
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Flight Ac     Type  Source Takeoff Destination Landing Payload	Empty	Cost");
        item=head_route;
        while(item!=NULL)
        {
            fprintf(fprocess,"\n %d \t%d \t%s \t%d \t%.2f\t%d\t%.2f\t%.2f\t%.2f\t%.2f ",item->flight_no,item->ac_number,item->ac_code,item->sr_index,item->takeoff,item->desti_index,item->landing,item->cargo_carried,item->residual_capacity,item->operating_cost);//NEU_MAC (+ac_type)
            item=item->next;
        }
        
        //checking if their any extra carried detail in the network
        flag=NO;
        item=head_route;
        while(item!=NULL)
        {
            if(item->extra_detail!=NULL)
            {
                extra_item=item->extra_detail;
                while(extra_item!=NULL)
                {
                    if(extra_item->extra_cargo!=0)
                    {
                        fprintf(fprocess,"\n\n Extra Carried demand detail -");
                        fprintf(fprocess,"\n [ Flight Ac\tExtra_Cargo for Destination ]");
                        flag=YES;
                        break;
                    } 
                    extra_item=extra_item->next_extra;
                }   //end of  while(extra_item!=NULL)
                if(flag==YES)
                break;
            }   //end of if(item->extra_detail!=NULL)
            item=item->next;
        }//end of while(item!=NULL)
        //printing the extra carried detail
        item=head_route;
        while(item!=NULL)
        {
            if(item->extra_detail!=NULL)
            {
                
                extra_item=item->extra_detail;
                while(extra_item!=NULL)
                {
                    if(extra_item->extra_cargo!=0)
                    fprintf(fprocess,"\n [   %d\t  %d\t%.2f\t\t%d\t ]",item->flight_no,item->ac_number,extra_item->extra_cargo,extra_item->desti_index);
                    extra_item=extra_item->next_extra;
                }    
            }   
            item=item->next;
        }   
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Aircraft Schedule"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        for(i=0;i<fleet_size;i++)
        {
			fprintf(fprocess,"\n\n\n For Aircraft %d)\n --------------",i+1);
            fprintf(fprocess,"\n Flight Type Source Takeoff    Destination Landing Payload\tEmpty\tCost");
			item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
					fprintf(fprocess,"\n %d\t%s\t%d\t%-7.2f\t%d\t   %-7.2f   %-7.2f\t%-7.2f\t%-7.2f ",item->flight_no,item->ac_code,item->sr_index,item->takeoff,item->desti_index,item->landing,item->cargo_carried,item->residual_capacity,item->operating_cost);
				item=item->next;
            }
			fprintf(fprocess,"\n Utilization [%%] = %.3f",utilization[i]*100);	//NEU_UT
            //checking if their any extra carried detail for the i+1th aircraft
            flag=NO;
            item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
                {
                    
                    if(item->extra_detail!=NULL)
                    {
                        extra_item=item->extra_detail;
                        while(extra_item!=NULL)
                        {
                            if(extra_item->extra_cargo!=0)
                            {
                                fprintf(fprocess,"\n\n Extra Carried demand detail -");
                                fprintf(fprocess,"\n [ Flight\tExtra_Cargo for \tDestination ]");
                                flag=YES;
                                break;
                            } 
                            extra_item=extra_item->next_extra;
                        }
                        if(flag==YES)
                        break;       
                    } //end of if(item->extra_detail!=NULL)           
                }    //end of if(item->ac_number==i+1)
                item=item->next;
            }  //end of while(item!=NULL)             
            //printing the extra carried detail
            item=head_route;
            while(item!=NULL)
            {
                if(item->ac_number==i+1)
                {
                    
                    if(item->extra_detail!=NULL)
                    {
                        
                        extra_item=item->extra_detail;
                        while(extra_item!=NULL)
                        {
                            if(extra_item->extra_cargo!=0)
                            fprintf(fprocess,"\n [   %d\t\t   %-7.2f\t\t   %d\t    ]",item->flight_no,extra_item->extra_cargo,extra_item->desti_index);
                            extra_item=extra_item->next_extra;
                        }    
                    }   
                }    //end of if(item->ac_number==i+1)
                item=item->next;
            }  //end of while(item!=NULL) 
            fprintf(fprocess,"\n ------------------------------");
            
        }  //end of for(i=0;i<fleet_size;i++)  
        
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Remaining Demand"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n\n");
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
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Remaining Demand = %.2f",remaining_demand);
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Frequency of operation between OD pairs"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n\n");
        
        temp_data=head_data;
        for(i=0;i<AIRPORT_NO;i++)
        {
            for(j=0;j<AIRPORT_NO;j++)
            {
                if(i==j)
                fprintf(fprocess," %d\t",0);
                else
                fprintf(fprocess," %d\t",temp_data->freq_offered);
                temp_data=temp_data->next_data;
            }    
            fprintf(fprocess,"\n");
        }    
        
        fprintf(fprocess,"\n\n\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n Quality of Service between OD pairs"); 
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        fprintf(fprocess,"\n\n");
        for(i=0;i<AIRPORT_NO;i++)
        {
            for(j=0;j<AIRPORT_NO;j++)
            {
                
                fprintf(fprocess," %.2f\t",QoS_all[i*AIRPORT_NO+j]);
                
            }    
            fprintf(fprocess,"\n");
        }    
        fprintf(fprocess,"\n --------------------------------------------------------------------------");
        /*
        fprintf(fprocess,"\n Sum of QoS = %f",sum_QoS);
        fprintf(fprocess,"\n Ave of QoS = %f",ave_QoS);
        fprintf(fprocess,"\n sigma of variance in QoS = %f",sigma_variance);
        fprintf(fprocess,"\n variance of QoS = %f",variance_QoS);
        */
        fclose(fprocess);

		free(utilization);//NEU_BH
		free(QoS_all);//NEU_VLA  
    
}//end of function    
