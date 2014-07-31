#include "get_airport_data.h"
#include "new_airport_data.h"
#include "check_landing_availability.h"
void Get_your_flight(struct Fleet *);

void Get_your_flight(struct Fleet *candidate)
{
    int desti_index;
    choices *prev_choice,*head_choice;
	//NEU
	struct choice_history *pTempChoice=NULL;
	//NEU
    /*
    //everytime when a new aircraft starts, forget all previous choices
    current_choice=head_choice;
    while(current_choice!=NULL)
    {
        prev_choice=current_choice;
        current_choice=current_choice->next;
        free(prev_choice);
    }  
     */ 
    head_choice=NULL;
    
    desti_index= Get_airport_data(candidate);//get_airport_data
    
    while(desti_index!=NOT_FOUND)
        {
           
            Check_landing_availability(candidate,desti_index);
            
            if(STATUS==YES)
            {
                break;               
            }
            else
            {
                //keeping a record of all the destinations which we have already tested
                prev_choice=(choices *)malloc(sizeof(choices));
                prev_choice->index=desti_index;
                prev_choice->next=NULL;
                if(head_choice==NULL)
                {
                    head_choice=prev_choice;
                   
                }    
                else
                {
                    prev_choice->next=head_choice;
                    head_choice=prev_choice;
                }    
               //head_choice points to the last choice
               desti_index= New_airport_data(candidate,head_choice);
            }    //end of else
                    
       }//end of while(desti_index!=NOT_FOUND)
        
    if(desti_index==NOT_FOUND)
        {
            if(user_choice==YES)
            {
            FILE *fprocess=fopen("Output/PROCESS.txt","a");
            if(fprocess==NULL)
               {
                
                printf("\n Cann't open the file Output/PROCESS in get_your_flight");
                getch();
                exit(1);
                }
            fprintf(fprocess,"\n------------ No destination found ------------\n");
            fclose(fprocess);
            }
        }    
	
	//free memory in head_choice
	while(head_choice!=NULL)
	{
		pTempChoice=head_choice;
		head_choice=head_choice->next;
		free(pTempChoice);
	}


}  

/*Algorithm----------------
possible destination =get next destination as per demand			---(1.1)
        do
			{
            IF(any possible destination)
            {
                check airport availability		----(1.2)
          		IF ( *Status = yes)
        		Break;
        		ELSE
                possible destination =check another airport as destination             ----(1.3)
        	}
        	}while (there is any possibility)
        	
        IF(*Status =NO)
        	{
        		possible destination = the maintenance station to go  -----------(1.4)
        		do
        		{
                		IF(any possible destination)
                        {
                                check airport availability		----(1.2)
                        		IF ( *Status = yes)
                        		Break;
                        		ELSE
                        		possible destination =check another maintenance airport as destination			-----(1.5)
                		}
        	     }while (there is any possibility)
        
        	}
        
  }
*/
  
