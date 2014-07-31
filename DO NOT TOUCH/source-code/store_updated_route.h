void Store_updated_route(struct empty_detail *,int ,float );

void Store_updated_route(struct empty_detail *empty,int final_destination,float demand_req)
{
    int i,j;
    float demand=0,min_residual=0,total_carried,demand_carry=0;
    struct Data *temp_data;
    struct empty_detail *find_empty,*temp_empty;
    struct Fleet *fleet;
    struct extra *extra_carry,*last_carry;
    struct route_detail *item;
	//NEU
	struct empty_detail *pTempDeleter;
    FILE *fprocess;

    total_carried=0.0;
    if(user_choice==YES)
    {
        fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
            {
                printf("\n cann't open the Output/PROCESS file in store_updated_route");
                getch();
                exit(1);
            }
        fprintf(fprocess,"\n demand required = %.2f ",demand_req);
    }    
              do
              { 
                //read the demand between its source and our final destination
                temp_data=head_data;
                //going upto the source airport from where our aircraft takesoff empty
                for(i=1;i<empty->source;i++)
                for(j=0;j<AIRPORT_NO;j++)
                temp_data=temp_data->next_data;
                //going upto the final destination airport
                for(j=1;j<final_destination;j++)
                temp_data=temp_data->next_data;
                if(user_choice==YES)
                fprintf(fprocess,"\n demand between %d and %d = %.2f ",empty->source,final_destination,temp_data->demand);
                //if min demand is satisfied then finding next min demand in remaining network
                if(demand_req==0.0)
                {
                    find_empty=empty;
                    if(find_empty->next_empty_detail!=NULL)
                    {
                        min_residual=find_empty->residual_capacity;
                        while(find_empty->next_empty_detail!=NULL)
                        {
                            find_empty=find_empty->next_empty_detail;
                            if(min_residual>find_empty->residual_capacity)
                            min_residual=find_empty->residual_capacity;
                        }
                        demand_req=min_residual-total_carried;
                        if(user_choice==YES)
                        fprintf(fprocess,"\n new demand  req = %.2f",demand_req);
                    }
                }//end of if(demand_req==0)
                
                //finding how much extra to carry
                if( temp_data->demand >= demand_req)
                  {
                      demand_carry=demand_req;
                      
                  }
                  else
                  {
                      demand_carry=temp_data->demand;                      
                      
                  }
                  if(user_choice==YES)
                  fprintf(fprocess,"\n Extra demand carried = %.2f",demand_carry);
                  //updating the main demand data
                  temp_data->demand-=demand_carry;
                  
                  
                  //updating the cargo carried and residual capacity in main route detail
                  extra_carry=(struct extra *)malloc(sizeof(struct extra));
                  extra_carry->desti_index=final_destination;
                  extra_carry->extra_cargo=demand_carry;
                  extra_carry->next_extra=NULL;
                  
                  //where to add this info in main route detail 
                  item=head_route;
                  for(i=1;i<empty->flight;i++)
                      item=item->next;
                  //now item showin right place to add info    
                  
                  //adding it 
                  if( item->extra_detail==NULL)
                  {
                      item->extra_detail=extra_carry;
                      
                  }
                  else
                  {
                      last_carry=item->extra_detail;
                      while(last_carry->next_extra!=NULL)
                      last_carry=last_carry->next_extra;
                      last_carry->next_extra=extra_carry;
                      
                  }
                     
                  //updating the info of main route detail
                  temp_empty=empty;
                  while(temp_empty!=NULL)
                  {
                      item=head_route;
                      for(i=1;i<temp_empty->flight;i++)
                      item=item->next;
                      
                      if(user_choice==YES)
                      {
                          fprintf(fprocess,"\n before carrying extra demand-");
                          fprintf(fprocess,"\n cargo carried on the flight %d = %.2f",item->flight_no,item->cargo_carried);
                          fprintf(fprocess,"\n empty = %.2f",item->residual_capacity);        
                      } 
                      
                      item->cargo_carried+=demand_carry;
                      item->residual_capacity-=demand_carry;
                      temp_empty=temp_empty->next_empty_detail;
                      
                      if(user_choice==YES)
                      {
                          fprintf(fprocess,"\n After carrying extra demand");
                          fprintf(fprocess,"\n total cargo carried on the flight %d = %.2f",item->flight_no,item->cargo_carried);
                          fprintf(fprocess,"\n Remaining empty = %.2f",item->residual_capacity);    
                      }  
                      
                  }    
                  
                    
                  
                  
                  
                  //remaining demand required
                  demand_req-=demand_carry;
                  if(user_choice==YES)
                  fprintf(fprocess,"\n demand required now = %.2f",demand_req);
                  //total extra demand which we had already collected
                  total_carried+=demand_carry;
                  
                  //go on next detail to update
				  //NEU
				  pTempDeleter=empty;
				  //NEU
                  empty=empty->next_empty_detail;
				  //NEU
				  free(pTempDeleter);
                  //NEU
              }while(empty!=NULL);    
               if(user_choice==YES)
               {
                    fprintf(fprocess,"\n Updated Demand -\n");
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
                   fprintf(fprocess,"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
                   fclose(fprocess);  
               }    

	//NEU
	pTempDeleter=NULL;
	//NEU
}//end of function    
