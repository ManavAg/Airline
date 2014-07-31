#include "get_base_data.h"
#include "new_base_data.h"
void Go_to_base(struct Fleet *);          
     
void Go_to_base(struct Fleet *candidate)
     {    
          int base_index;
          choices *prev_choice,*head_base_choice=NULL;
		  //NEU
		  struct choice_history *pTempChoice=NULL;
		  //NEU
          
          SENDING_TO_BASE=YES;
          base_index= Get_base_data(candidate);
          
          while(base_index!=NOT_FOUND)
            {
                
                Check_landing_availability(candidate,base_index);
                
                if(STATUS==YES)
                {
                    break;
                }
                else
                {
                    
                     //keeping a record of all the destinations which we have already tested
                    prev_choice=(choices *)malloc(sizeof(choices));
                    prev_choice->index=base_index;
                    prev_choice->next=NULL;
                    if(head_base_choice==NULL)
                    {
                        head_base_choice=prev_choice;
                       // last=prev_choice;
                    }    
                    else
                    {
                        prev_choice->next=head_base_choice;
                        head_base_choice=prev_choice;
                    }    
                    
                    base_index= New_base_data(candidate,head_base_choice);
                } //end of else   
                       
            }//end of while(base_index!=NOT_FOUND)
            
           
           if(base_index==NOT_FOUND)
           {
                if(user_choice==YES)
                {
                    FILE *fprocess=fopen("Output/PROCESS.txt","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't open the file Output/PROCESS in go_to_base");
                        getch();
                        exit(1);
                        }
                    fprintf(fprocess,"\n------------ No base station is possible ------------\n");
                    fclose(fprocess);
                }
            }    
		//free memory in head_base_choice
		while(head_base_choice!=NULL)
		{
			pTempChoice=head_base_choice;
			head_base_choice=head_base_choice->next;
			free(pTempChoice);
		}         
              
        }//end of function    
        
        
        
        /*
        
        //Check_airport_availability(candidate,base_index);
                //reading average delay on the destination airport
                    
                    fp = fopen("Input/AIRPORT NAMES.txt","r");
                   if(fp==NULL)
                    {
                    #ifdef PROCESS                       
                    fprintf(fprocess,"\n Cann't find the file Input/AIRPORT NAMES in in go_to_base");
                    fclose(fprocess);
                    #endif
                    printf("\n Cann't find the file Input/AIRPORT NAMES in in go_to_base");
                    getch();
                    exit(1);
                    }  
                    for(i=1;i<=base_index;i++)  
                    fscanf(fp,"%s %f",temp,&average_delay);
                    fclose(fp);
                    
                    #ifdef PROCESS
                    fprintf(fprocess,"\n average delay on the base airport %d = %f",base_index,average_delay);
                    #endif
                    #ifdef PROCESS
                   fclose(fprocess);
                   #endif
                    
                    block_time=Get_route_BT(candidate->ap_index,base_index);//(source,destination)

                    #ifdef PROCESS
                    fprocess=fopen("Output/PROCESS.doc","a");
                    if(fprocess==NULL)
                       {
                        
                        printf("\n Cann't find the file Output/PROCESS in go_to_base");
                        getch();
                        exit(1);
                        }
                    fclose(fprocess);
                    #endif
                    
                    new_take_off = candidate->take_off +(block_time+ average_delay +TURN_AROUND_TIME);
                    time_diff =0;
                    Check_end_time(candidate,new_take_off,base_index,time_diff);
                    
                    
                    */
