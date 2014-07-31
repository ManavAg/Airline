
int Get_base_data(struct Fleet *);


int Get_base_data(struct Fleet *candidate)
{
    int i,j,k;
    int HIGHEST_PRIORITY_FLAG;//to check if we gor any base station with priority=1
    int base_aircraft;
    int best_index,selected_index;
    char best_cat;
    float best_grey_index,best_demand;
    struct Bases *base;
    struct Final_grey *grey;
    struct Fleet *temp_head;
    struct Data *data;
    float random_value;
    time_t current_time;
    struct tm * timeinfo;
    FILE *fprocess;
    //FILE *fp;
    //fp=fopen(output_filename,"a");
    //fprintf(fp,"\n searching for base for  %d aircraft ",candidate->ac_index);
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
        {
            printf("\n Cann't open the file Output/PROCESS in get_base_data");
            getch();
            exit(1);
        }    
    }        

 
    //starting the search
    base=head_base;
    best_index=-1;
    if(best_index==-1)  
    {
   
        
        HIGHEST_PRIORITY_FLAG=NO;
        
        for(k=0;k<total_bases;k++)
        {
                         
            //going upto the our candidate airport to get grey details of all airports
            grey=head_final_grey;
           
            //reading the grey detail of the base station on route from our candidate airport
            for(i=1;i<base->base_index;i++)
            grey=grey->next_ap_index;
            
            //going upto the airport where our candidate aircraft is available
            data=head_data;
            for(i=1;i<candidate->ap_index;i++)
            for(j=0;j<AIRPORT_NO;j++)
            data=data->next_data;
            
            //reading the detail of the base station on route from our candidate airport
            for(i=1;i<base->base_index;i++)
            data=data->next_data;
            
            if(data->priority!=-1)//don't look the base if priority=-1
            {
             if((data->priority==1 )||(HIGHEST_PRIORITY_FLAG==NO))
             { 
            
            //find out how many aircraft are present their already
            base_aircraft=0;
            temp_head=candidate;
                while(temp_head!=NULL)//look all the fleets
                {
                    if((temp_head->ap_index==base->base_index)&&(temp_head->ac_index != candidate->ac_index))//if aircraft is on our base station
                    {
                        if(user_choice==YES)
                            fprintf(fprocess,"\n Aircraft no. %d is available at base airport %d",temp_head->ac_index,base->base_index);
                        //fprintf(fp,"\n Aircraft no. %d is available at base airport %d",temp_head->ac_index,base->base_index);
                       
                        base_aircraft++;
                    }    
                    temp_head=temp_head->next_aircraft;
                }
             if(user_choice==YES)
             {
                 fprintf(fprocess,"\n Total %d aircraft are available on base airport %d",base_aircraft,base->base_index);
                 
             }    
             //fprintf(fp,"\n Total %d aircraft are available on base airport %d",base_aircraft,base->base_index);
                 
            
            //if base station can handle more aircraft then only its my possible base station
            //I cann't assume that aircraft which are already their will leave the base staion for me
          
              //if we are on the base station itself then no need to check anything we will not go to anywhere
          /*
          if(base->base_index==candidate->ap_index)
          {
              best_index=base->base_index;
              if(user_choice ==YES)
                        fprintf(fprocess,"\n We are on the base station itself. No need to check other options");
              break;
          }      
          else if(base->hanger_capacity- base_aircraft >0)
          */
          //fprintf(fp,"\n base capacity = %d",base->hanger_capacity);
          if(base->hanger_capacity- base_aircraft >0)
            {
                //printf("\n looking for base airport number = %d as an option to go",base->base_index);
                if(user_choice ==YES)
                           fprintf(fprocess,"\n looking for base airport number = %d as an option to go",base->base_index);
                
                //fprintf(fp,"\n looking for base airport number = %d as an option to go",base->base_index);
                
                if(data->priority==1 && HIGHEST_PRIORITY_FLAG==NO)//first highest priority route, 
                {
                    HIGHEST_PRIORITY_FLAG=YES;
                    best_index=-1; 
                    if(user_choice ==YES)
                           fprintf(fprocess,"\n Base Airport number %d has the highest priority (1) to go",i+1);                 
                    
                } 
                
                //fprintf(fp,"\n (%d/%d/%d)",best_index,base->base_index,candidate->ap_index);
                
                if(best_index==-1)//means this is the first node
                {
                          best_index=base->base_index;
                          best_cat=grey->grey_char;
                          best_grey_index=grey->grey_index;
                          best_demand=data->demand;
                         
                }    
                else
                {
                    //fprintf(fp,"[ %c/%c ]",best_cat,grey->grey_char);
                    //fprintf(fp,"[ %.2f/%.2f ]",best_grey_index,grey->grey_index);
                   if(best_cat=='s'&& grey->grey_char=='s')
                   {
                       //fprintf(fp,"s[ %.2f/%.2f ]",best_grey_index,grey->grey_index);
                       if(best_grey_index < grey->grey_index)
                       {
                          // printf("\n changing the best selection becouse we got better option in select category");
                           if(user_choice ==YES)
                           fprintf(fprocess,"\n changing the best selection becouse we got better option in select category");
                           best_index=base->base_index;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                           best_demand=data->demand;
                       } 
                       
                       else if((best_grey_index == grey->grey_index))
                           {
                               //NEU_MAC old:selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time);//desti1,desti2,source
                               selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type);//NEU_MAC
							   if(selected_index==base->base_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                 
                               }    
                               /*
                               if(base->base_index==candidate->ap_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                  }
                                  else
                                  {    
                                      //calculating random variable
                                      current_time=time(NULL);
                                        timeinfo=localtime(&current_time);
                                        srand(time(NULL)*rand()*1000);
                                       random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                      if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                      fprintf(fp,"\n random value = %.2f",random_value);
                                      if(random_value>0.5)
                                      {
                                          // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                           if(user_choice ==YES)
                                           fprintf(fprocess,"\n changing the best selection becouse we got better option in select category (randomly)");
                                           best_index=base->base_index;
                                           best_cat=grey->grey_char;
                                           best_grey_index=grey->grey_index;
                                            best_demand=data->demand;
                                        }
                                  }       
                                  */ 
                           }
                               
                   }    
                   else if(best_cat!='s'&& grey->grey_char=='s')
                   {
                       
                       //printf("\n changing the best selection becouse of category");
                       if(user_choice ==YES)
                            fprintf(fprocess,"\n changing the best selection becouse of category");
                       
                       best_index=base->base_index;
                       best_cat=grey->grey_char;
                       best_grey_index=grey->grey_index;
                       best_demand=data->demand;
                   }    
                   else if(best_cat=='p'&& grey->grey_char =='p')
                   {
                       if(best_grey_index < grey->grey_index)
                       {
                           //printf("\n changing the best selection becouse we got better option in probable category");
                           if(user_choice ==YES)
                                fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category");
                           best_index=base->base_index;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                           best_demand=data->demand;
                       }
                       
                       else if((best_grey_index == grey->grey_index))
                           {
                                selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type);//desti1,desti2,source NEU_MAC
                               if(selected_index==base->base_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                 
                               }
                               /*
                               if(base->base_index==candidate->ap_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                  }
                                  else
                                  {    
                                       //calculating random variable
                                      current_time=time(NULL);
                                        timeinfo=localtime(&current_time);
                                        srand(time(NULL)*rand()*1000);
                                       random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                      if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                      fprintf(fp,"\n random value = %.2f",random_value);
                                      if(random_value>0.5)
                                      {
                                          // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                           if(user_choice ==YES)
                                           fprintf(fprocess,"\n changing the best selection becouse we got better option in probable category (randomly)");
                                           best_index=base->base_index;
                                           best_cat=grey->grey_char;
                                           best_grey_index=grey->grey_index;
                                            best_demand=data->demand;
                                        } 
                                    }   
                                    */ 
                           } 
                             
                   } 
                   else if(best_cat=='r'&& grey->grey_char!='r')
                   {
                       //printf("\n changing the best selection becouse of category");
                       if(user_choice ==YES)
                            fprintf(fprocess,"\n changing the best selection becouse of category");
                       
                       best_index=base->base_index;
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
                           best_index=base->base_index;
                           best_cat=grey->grey_char;
                           best_grey_index=grey->grey_index;
                           best_demand=data->demand;
                       } 
                       
                       else if((best_grey_index == grey->grey_index))
                           {
                                selected_index=Select_if_same_index(best_index,base->base_index,candidate->ap_index,candidate->takeoff_time,candidate->ac_type);//desti1,desti2,source NEU_MAC
                               if(selected_index==base->base_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                 
                               }
                               /*
                              if(base->base_index==candidate->ap_index)
                               {
                                      best_index=base->base_index;
                                      best_cat=grey->grey_char;
                                      best_grey_index=grey->grey_index;
                                      best_demand=data->demand;
                                  }
                                  else
                                  {    
                                      //calculating random variable
                                      current_time=time(NULL);
                                        timeinfo=localtime(&current_time);
                                        srand(time(NULL)*rand()*1000);
                                      random_value=((timeinfo->tm_sec /60.0)+((rand()%100)/100.0))/2;
                                      if(user_choice ==YES)
                                           fprintf(fprocess,"\n random value = %.2f",random_value);
                                     fprintf(fp,"\n random value = %.2f",random_value);
                                     if(random_value>0.5)
                                      {
                                          // printf("\n changing the best selection because we got better option in select category(with more demand)");
                                           if(user_choice ==YES)
                                           fprintf(fprocess,"\n changing the best selection becouse we got better option in reject category (randomly)");
                                           best_index=base->base_index;
                                           best_cat=grey->grey_char;
                                           best_grey_index=grey->grey_index;
                                            best_demand=data->demand;
                                        } 
                                    } 
                                    */   
                           }   
                           
                   } 
                  }//end of else 
                
            }//end of if(base->hanger_capacity- base_aircraft >0)
            else
            {
                if(user_choice==YES)
                {
                    
                    fprintf(fprocess,"\n Base station %d is full upto its capacity",base->base_index);
                    
                }    
            }   
            }//end of if((data->priority==1 &&  HIGHEST_PRIORITY_FLAG==YES)||HIGHEST_PRIORITY_FLAG==NO)    
           else
             {
                 if(user_choice ==YES)
                              fprintf(fprocess,"\n The Base Airport %d don't have priority = 1",base->base_index);
                 
                 
             }    
             }//end of if(data->priority!=-1)
             else
             {
                 if(user_choice ==YES)
                              fprintf(fprocess,"\n The Base Airport %d has the priority = -1",base->base_index);
                 
             }
         base=base->next_base;            
        }//end of for(i=0;i<total_bases;i++)  
    }//end of  if(best_index==-1)          
// fclose(fp);
//returning the possible destination               
           if(best_index==-1)  
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n No Possible base station ");
                   fclose(fprocess); 
               }
               return(NOT_FOUND);
           }    
           else
           {
               if(user_choice==YES)
               {
                   fprintf(fprocess,"\n Possible base station = %d ",best_index);
                   fclose(fprocess); 
               }    
              return(best_index);
           } 
}//end of function    
