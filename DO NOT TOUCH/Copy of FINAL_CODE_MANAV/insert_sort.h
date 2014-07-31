
//struct Fleet * Insert_sort(struct Fleet *,struct Fleet *);
void Insert_sort(struct Fleet *);

//struct Fleet * Insert_sort(struct Fleet *head_aircraft_list,struct Fleet *item)
void Insert_sort(struct Fleet *item)
{
    struct Fleet *prev=NULL,*list;
    
    list =head_aircraft_list; // list is now pointing towars the start of the list
    if(head_aircraft_list==NULL) //means there is no item in the list
        head_aircraft_list=item;
     
    else
    {
      if(list->takeoff_time > item->takeoff_time)//insert at start
        {
              item->next_aircraft=list;
              head_aircraft_list=item;
        }    
    
      else
         {
               while(list->takeoff_time <= item->takeoff_time)
                     {
                               prev=list;
                               if(list->next_aircraft==NULL) // insert at  last node
                                {
                                         list=NULL;
                                         break;
                                }    
          
                               list=list->next_aircraft;
                       }    
      
            prev->next_aircraft=item;
            item->next_aircraft=list;
    
    
       }
      
 
     }   
     //return(head_aircraft_list);
    
}   //end of function 

