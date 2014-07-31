float Get_capacity(int, int, int);
float Get_capacity(int iAircraftType, int source, int destination)

{
	//input values: Blockhours, Aircraftype
	int i,j;
	float fCapacity=0;
	//float fBH;
	float fR1,fR2,fR3,fPL1,fPL2;
	struct AC_data *temp_ac_data;
	struct Data *temp_data;

	/*//calculate Blocktime
	fBH = Get_route_BT(source,destination);*/

	if(source==destination)
	return fCapacity=0;

	//calculate Distance
	temp_data=head_data;
	//going up to right place
	for(i=1;i<source;i++)
	for(j=0;j<AIRPORT_NO;j++)
		temp_data=temp_data->next_data;
	for(i=1;i<destination;i++)
		temp_data=temp_data->next_data;
	
	//going to data of aircraft type
	temp_ac_data=head_ac_data;
	while(temp_ac_data->ac_type!=iAircraftType)
		temp_ac_data=temp_ac_data->next_type;

	//calculation of Payload <- Distance
	if(temp_data->distance<=temp_ac_data->fR1)
		fCapacity=temp_ac_data->fPL1;
	else if((temp_data->distance>temp_ac_data->fR1) && (temp_data->distance<=temp_ac_data->fR2))
		fCapacity=temp_ac_data->fPL1-((temp_data->distance-temp_ac_data->fR1)*(temp_ac_data->fPL1-temp_ac_data->fPL2)/(temp_ac_data->fR2-temp_ac_data->fR1));
	else if((temp_data->distance>temp_ac_data->fR2) && (temp_data->distance<=temp_ac_data->fR3))
		fCapacity=temp_ac_data->fPL2-((temp_data->distance-temp_ac_data->fR2)*temp_ac_data->fPL2/(temp_ac_data->fR3-temp_ac_data->fR2));
	else
	{
		printf("Route Impossible >maximumRange for Type %d",iAircraftType);
		getch();
		exit(1);
	}

	return fCapacity;

}    //end of function
         
          

    
