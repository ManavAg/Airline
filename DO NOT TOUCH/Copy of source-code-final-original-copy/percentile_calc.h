float Percentile_calc(float *,int,float);

float Percentile_calc(float *array ,int size,float per)
{
    float index;
    int place;
    index = ceil((per*size)/100.0);
    place =(size-(int)(index))+1;//sience our array is arranged in reverse order, desending 
    return(array[place-1]);   

}    
