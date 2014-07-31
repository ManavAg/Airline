function trial(a)
{ 
  alert(a);
}


function Median_calc(array, size)
{   
  var i;
    if((size%2)==0)//even number
    {
        i=size/2;
        return((array[i-1]+array[i])/2);
    }    
    else//odd number
    {
        i=(size+1)/2;
        return(array[i-1]);
    }    
}    
