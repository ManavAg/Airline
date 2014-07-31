void Initialize_grey_index (void);
void Initialize_grey_index()
{
    int i;

    struct Grey_indices *select,*last_select,*probable,*last_probable,*reject,*last_reject;
   

    //Initializing the indices for select category
    for(i=0;i<AIRPORT_NO;i++)
    {
       select=(struct Grey_indices *)malloc(sizeof(struct Grey_indices));
       select->cost_index=0;
       select->demand_index=0;
       select->grey_index=0;
       select->route_priority_index=0;
       select->time_index=0;
       //select->network_design_index=0;
       select->next_ap_grey=NULL;
        
        if(head_select==NULL)
        {
            head_select=select;
            last_select=select;
        }
        
        else
        {
            last_select->next_ap_grey=select;
            last_select=last_select->next_ap_grey;
        } 
               
    }
    
    //Initializing the indices for probable category    
    for(i=0;i<AIRPORT_NO;i++)
    {
        probable=(struct Grey_indices *)malloc(sizeof(struct Grey_indices));
        probable->cost_index=0;
        probable->demand_index=0;
        probable->grey_index=0;
        probable->route_priority_index=0;
        //probable->network_design_index=0;
        probable->time_index=0;

        probable->next_ap_grey=NULL;
        if(head_probable==NULL)
        {
            head_probable=probable;
            last_probable=probable;
        }
        else
        {
            last_probable->next_ap_grey=probable;
            last_probable=last_probable->next_ap_grey;
        }        
    }
    
    //Initializing the indices for reject category
    for(i=0;i<AIRPORT_NO;i++)
    {
        reject=(struct Grey_indices *)malloc(sizeof(struct Grey_indices)); 
        reject->cost_index=0;
        reject->demand_index=0;
        reject->grey_index=0;
        reject->route_priority_index=0;
        reject->time_index=0;
        //reject->network_design_index=0;

        reject->next_ap_grey=NULL;
        if(head_reject==NULL)
        {
            head_reject=reject;
            last_reject=reject;
        }
        else
        {
            last_reject->next_ap_grey=reject;
            last_reject=last_reject->next_ap_grey;
        }        
    }
    
  
 
  

}    //end of function
