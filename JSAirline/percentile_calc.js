function Percentile_calc(array, per)
{
    var index, place;
    index = ceil((per*size)/100.0);
    place =(size-index)+1;//sience our array is arranged in reverse order, desending 
    return(array[place-1]);   
}    
