void Get_grey_index();

void Get_grey_index()
{
    int j,k;
    struct Grey_indices *select,*probable,*reject;
    struct Grey *grey_priority;
    float index;

    //calculating the grey index in select category
    select=head_select;

    for(j=0;j<AIRPORT_NO;j++)
    {
        select->grey_index=0;
        grey_priority=head_grey;
        for(k=0;k<total_grey_indices;k++)
        {
            switch(k)
            {
                case 0:
                    index=select->demand_index; 
                    break;
                case 1:
                    index=select->cost_index;
                    break;
                case 2:
                    index=select->time_index;
                    break;
                /*
                case 3:
                    index=select->network_design_index;
                    break;
                    */
                case 3:
                    index=select->route_priority_index;
                    break;
                    
                default :
                    {
                        printf("\n Error in reading grey indices priorities in get_grey-index\n Please check the input data");
                        getch();
                        exit(1);
                    }    
                
            }//end of switch  
            //grey index is the weightage sum of all the indices      
            select->grey_index+=(grey_priority->priority*index);
            grey_priority=grey_priority->next_grey;
        }
        select=select->next_ap_grey;
    }      
    

    //calculating the grey index in probable category
    probable=head_probable;
    
    for(j=0;j<AIRPORT_NO;j++)
    {
        probable->grey_index=0;
        grey_priority=head_grey;
        for(k=0;k<total_grey_indices;k++)
        {
            switch(k)
            {
                case 0:
                    index=probable->demand_index;            
                    break;
                case 1:
                    index=probable->cost_index;
                    break;
                case 2:
                    index=probable->time_index;
                    break;
                /*
                case 3:
                    index=probable->network_design_index;
                    break;
                    */
                case 3:
                    index=probable->route_priority_index;
                    break;
                    
                default :
                    {
                        printf("\n Error in reading grey indices priorities in get_grey_index\n Please check the input data");
                        getch();
                        exit(1);
                    }    
                
            }//end of switch  
            //grey index is the weightage sum of all the indices      
            probable->grey_index+=(grey_priority->priority*index);
            grey_priority=grey_priority->next_grey;
        }
        probable=probable->next_ap_grey;
    } 
    
    //calculating the grey index in reject category
    reject=head_reject;
    
    for(j=0;j<AIRPORT_NO;j++)
    {
        reject->grey_index=0;
        grey_priority=head_grey;
        for(k=0;k<total_grey_indices;k++)
        {
            switch(k)
            {
                case 0:
                    index=reject->demand_index;            
                    break;
                case 1:
                    index=reject->cost_index;
                    break;
                case 2:
                    index=reject->time_index;
                    break;
               /*
                 case 3:
                     index=reject->network_design_index;
                     break;
                     */
                case 3:
                    index=reject->route_priority_index;
                    break;
                    
                default :
                    {
                        printf("\n Error in reading grey indices priorities in get_grey_index\n Please check the input data");
                        getch();
                        exit(1);
                    }    
                
            }//end of switch  
            //grey index is the weightage sum of all the indices      
            reject->grey_index+=(grey_priority->priority*index);
            grey_priority=grey_priority->next_grey;
        }
        reject=reject->next_ap_grey;
    }   
    
    
    //printing the grey index information
     if(user_choice==YES)
     {
         FILE *fprocess=fopen("Output/PROCESS.txt","a");
         if(fprocess==NULL)
         {
             printf("\n Cann't open the file Output/PROCESS in get_grey_index");
             getch();
             exit(1);
         }    
                  

         fprintf(fprocess,"\n Grey index in different categories : ( Select \\ Probable \\ Reject)");
         select=head_select;
         reject=head_reject;
         probable=head_probable;
             fprintf(fprocess,"\n");
             
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->grey_index);
                 select=select->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->grey_index);
                 probable=probable->next_ap_grey;
             }
             fprintf(fprocess," \t\t");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->grey_index);
                 reject=reject->next_ap_grey;
             }
                 
         fclose(fprocess);
         }//end of if(user_choice==YES)    
}    //end of function
/*
         fprintf(fprocess,"\n\n grey Index in Select Category");
         select=head_select;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",select->grey_index);
                 select=select->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n grey Index in Probable Category");
         probable=head_probable;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",probable->grey_index);
                 probable=probable->next_ap_grey;
             }             
         }
         
         fprintf(fprocess,"\n\n grey Index in Reject Category");
         reject=head_reject;
         for(i=0;i<AIRPORT_NO;i++)
         {
             fprintf(fprocess,"\n");
             for(j=0;j<AIRPORT_NO;j++)
             {
                 fprintf(fprocess," %.2f\t",reject->grey_index);
                 reject=reject->next_ap_grey;
             }             
         }   
         
         fclose(fprocess);     
     }//end of if(user_choice==YES)    
 

}//end of function    

*/
