void No_holding();

void No_holding()
{
	int iBlockTime=0;
	//connect pTempRoute with final route data head_route
	struct route_detail *pTempRoute=head_route;
	while(pTempRoute!=NULL)
		{
		iBlockTime=Get_route_BT(pTempRoute->sr_index,pTempRoute->desti_index);
		//erase holding if Block time in schedule != real Blocktime
		if (pTempRoute->landing-pTempRoute->takeoff!=iBlockTime)
			pTempRoute->takeoff=pTempRoute->landing-iBlockTime;
		pTempRoute=pTempRoute->next;
		}
}