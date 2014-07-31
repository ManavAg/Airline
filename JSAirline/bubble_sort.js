function Bubble_sort(array)
{
    var i,j,t;
    for(i=1;i<=array.length-1;i++)
    {
        for(j=1;j<=array.length-i;j++)
        {
            if(array[j-1]<=array[j])
            {
                //printf("\n cahnging %d and %d",j-1,j);
                t=array[j-1];
                array[j-1]=array[j];
                array[j]=t;
            }    
        }    
    }  
  return array;
}    
