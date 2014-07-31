void Get_grey_category();

void Get_grey_category()
{
    int j;
    struct Grey_indices *select,*probable,*reject;
    struct Final_grey *final,*last_final;
    
    head_final_grey=NULL;
    select=head_select;
    probable=head_probable;
    reject=head_reject;
    
    //final category = cat. which has highest value of grey index
 
    for(j=0;j<AIRPORT_NO;j++)
    {
        final=(struct Final_grey *)malloc(sizeof(struct Final_grey));
        if(select->grey_index>=probable->grey_index && select->grey_index>=reject->grey_index)
        {
            final->grey_char='s';//select cat.
            final->grey_index=select->grey_index;
        }
        else if (probable->grey_index>=reject->grey_index)    
        {
            final->grey_char='p';//probable cat.
            final->grey_index=probable->grey_index;
        } 
        else
        {
            final->grey_char='r';// reject cat.
            final->grey_index=reject->grey_index;
        }  
           
        final->next_ap_index=NULL;
        if(head_final_grey==NULL)
        {
            head_final_grey=final;
            last_final=final;
        }
        
        else
        {
            last_final->next_ap_index=final;
            last_final=last_final->next_ap_index;
        }
        
        select=select->next_ap_grey;
        probable=probable->next_ap_grey;
        reject=reject->next_ap_grey;  
    }
    
    //printing the final grey detail
    if(user_choice==YES)
    {
        FILE *fprocess=fopen("Output/PROCESS.txt","a");
        if(fprocess==NULL)
           {
            
            printf("\n Cann't open the file Output/PROCESS in get_grey_category");
            getch();
            exit(1);
            }
        fprintf(fprocess,"\n\n Final Grey Category");
        final=head_final_grey;
        fprintf(fprocess,"\n");
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess," %c\t",final->grey_char);
                final=final->next_ap_index;
            }
         
        fprintf(fprocess,"\n\n Final Grey Index");       
        final=head_final_grey;
        fprintf(fprocess,"\n");
            for(j=0;j<AIRPORT_NO;j++)
            {
                fprintf(fprocess," %.2f\t",final->grey_index);
                final=final->next_ap_index;
            }
                 
        fclose(fprocess);
    }//end of if(user_choice==YES)    
}//end of function                
            
        
            
    
