void Bubble_sort (float *,int);
void Bubble_sort( float *array, int size)
{
    int i,j;
    float t;
    //printf("\n\n In -");
    //for(i=0;i<size;i++)
   // printf("\n %.2f",array[i]);
    for(i=1;i<=size-1;i++)
    {
        for(j=1;j<=size-i;j++)
        {
            if(array[j-1]<=array[j])
            {
                //printf("\n cahnging %d and %d",j-1,j);
                t=array[j-1];
                array[j-1]=array[j];
                array[j]=t;
            }    
            //else
            //continue;
        }    
    }  
   /// printf("\n\n out -");
   // for(i=0;i<size;i++)
   // printf("\n %.2f",array[i]);  
}    
