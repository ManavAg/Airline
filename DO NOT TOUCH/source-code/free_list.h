void Free_list(void);

void Free_list()
{

	//to be deleted with free (struct types for temp pointers):
	//Grey_indices: head_select, head_reject, head_probable (OK)
	//Airports: head_airport (OK)
	//Data: head_data (OK)
	//Fleet: head_fleet, head_aircraft_list (already free in sorted_fleet, OK)
	//AC_data: head_ac_data and sublist static_cost
	//AEA_input
	//Bases: head_base
	//Grey: head_grey
	//Final_grey: head_final_grey
	//Whitening_decision: BIG, SMALL
	//route_detail: head_route, last_route
	
	struct Fleet *pTempFleet=NULL;
	struct AC_data *pTempACdata=NULL;
	struct static_cost *pTempStaticCost1=NULL;
	struct static_cost *pTempStaticCost2=NULL;
	struct Grey_indices *pTempGreyIndices=NULL;
	struct Airports *pTempAirports=NULL;
	struct timing *pTempTiming1=NULL;
	struct timing *pTempTiming2=NULL;
	struct Data *pTempData=NULL;
	struct aea_input *pAEAinput;
	struct Bases *pTempBases=NULL;
	struct Grey *pTempGrey=NULL;
	struct Final_grey *pTempFinalGrey=NULL;
	struct route_detail *pTempRouteDetail=NULL;
	struct extra *pTempExtra1=NULL;
	struct extra *pTempExtra2=NULL;

	//free memory in head_select, head_reject, head_probable
	//[allocated in initialize_grey_index (each set=Airport No.]
	while(head_select!=NULL)
		{
			pTempGreyIndices=head_select;
			head_select=head_select->next_ap_grey;
			free(pTempGreyIndices);
		}
	while(head_reject!=NULL)
		{
			pTempGreyIndices=head_reject;
			head_reject=head_reject->next_ap_grey;
			free(pTempGreyIndices);
		}
	while(head_probable!=NULL)
		{
			pTempGreyIndices=head_probable;
			head_probable=head_probable->next_ap_grey;
			free(pTempGreyIndices);
		}
	pTempGreyIndices=NULL;

	//free memory in head_airport
	while(head_airport!=NULL)
		{
			//first delete sublist takeoff
			pTempTiming1=head_airport->takeoff_time;
			while(pTempTiming1!=NULL)
			{
				pTempTiming2=pTempTiming1;
				pTempTiming1=pTempTiming1->next_time;
				free(pTempTiming2);
			}
			//second delete sublist landing
			pTempTiming1=head_airport->landing_time;
			while(pTempTiming1!=NULL)
			{
				pTempTiming2=pTempTiming1;
				pTempTiming1=pTempTiming1->next_time;
				free(pTempTiming2);
			}
			pTempTiming2=NULL;
			//third delete mainlist
			pTempAirports=head_airport;
			head_airport=head_airport->next_airport;
			free(pTempAirports);
		}
	pTempAirports=NULL;

	//free memory in head_data
	//[allocated in read_input (airportno^^^2)]
	while(head_data!=NULL)
	{
		pTempData=head_data;
		head_data=head_data->next_data;
		free(pTempData);
	}
	pTempData=NULL;

	//free memory in head_aircraft_list 
	//[allocated in sorted fleet (Fleet Size) & everything_fine (sets = total flights-Fleet Size)]
	while(head_aircraft_list!=NULL)
		{
			pTempFleet=head_aircraft_list;
			head_aircraft_list=head_aircraft_list->next_aircraft;
			free(pTempFleet);
		}
	pTempFleet=NULL;

	//free memory in head_ac_data
	//[allocated in read_input (no. of ac_types)]
	while(head_ac_data!=NULL)
		{
			//first delete sublist static_cost
			pTempStaticCost1=head_ac_data->static_cost;
			while(pTempStaticCost1!=NULL)
			{
				pTempStaticCost2=pTempStaticCost1;
				pTempStaticCost1=pTempStaticCost1->next_cost;
				free(pTempStaticCost2);
			}
			pTempStaticCost2=NULL;	
			//then main list
			pTempACdata=head_ac_data;
			head_ac_data=head_ac_data->next_type;
			free(pTempACdata);
		}
	pTempACdata=NULL;

	//free memory in head_aea_input
	//[allocated in read_input (no of actypes)]
	while(head_aea_input!=NULL)
	{
		pAEAinput=head_aea_input;
		head_aea_input=head_aea_input->next_ac;
		free(pAEAinput);
	}
	pAEAinput=NULL;


	//free memory in head_base
	//[allocated in read_input (no of bases)]
	while(head_base!=NULL)
	{
		pTempBases=head_base;
		head_base=head_base->next_base;
		free(pTempBases);
	}
	pTempBases=NULL;

	//free memory in head_grey
	//[allocated in cargo_schedulling(no. of grey indices)]
	while(head_grey!=NULL)
	{
		pTempGrey=head_grey;
		head_grey=head_grey->next_grey;
		free(pTempGrey);
	}
	pTempGrey=NULL;

	//free memory in head_final_grey
	//[allocated in get_grey_category (no. aiports)] 
	while(head_final_grey!=NULL)
	{
		pTempFinalGrey=head_final_grey;
		head_final_grey=head_final_grey->next_ap_index;
		free(pTempFinalGrey);
	}
	pTempFinalGrey=NULL;

	//free memory in BIG, SMALL
	//[allocated in read_input (1)]
	free(BIG);
	free(SMALL);
	BIG=NULL;
	SMALL=NULL;

	//free memory in head_route (&last_route)
	//[allocated in store_route_info(no. routes)&store_updated_route(extras)]
		while(head_route!=NULL)
		{
			//first delete sublist extra
			pTempExtra1=head_route->extra_detail;
			while(pTempExtra1!=NULL)
			{
				pTempExtra2=pTempExtra1;
				pTempExtra1=pTempExtra1->next_extra;
				free(pTempExtra2);
			}
			pTempExtra2=NULL;
			//second delete mainlist
			pTempRouteDetail=head_route;
			head_route=head_route->next;
			free(pTempRouteDetail);
		}
	pTempRouteDetail=NULL;

} // end of function