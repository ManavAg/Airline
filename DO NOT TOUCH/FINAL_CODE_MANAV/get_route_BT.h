float Get_route_BT(int, int);

float Get_route_BT(int source, int desti)
{
    
    int i,j;
    struct Data *data;
    
    data=head_data;
    for(i=1;i<source;i++) // going to the required airport
    for(j=0;j<AIRPORT_NO;j++)
    data=data->next_data;
    for(j=1;j<desti;j++)//going to the required column in that line
        data=data->next_data;
    return(data->block_time);//now we are at right position-read it

}  //end of function  
