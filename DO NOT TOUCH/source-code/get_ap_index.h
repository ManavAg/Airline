int Get_ap_index(char *);

//finding index of the source airport
int Get_ap_index(char *ap_name)
{   
    struct Airports *airport;
   
    int i,flag=NOT_FOUND;
    airport=head_airport;   
    //while(fscanf(fp,"%s %f",temp,&delay)!=EOF && flag!=FOUND)
    for(i=0;i<AIRPORT_NO && flag!=FOUND;i++)
    {
        if(strcmp(airport->name,ap_name)==0)// match
        flag=FOUND;
        else
        airport=airport->next_airport;
    }    
                
if(flag!=FOUND)
    return(NOT_FOUND);
else
    return(airport->ap_index);
        
}    

