float Get_cost(int, int, int);
float Get_cost(int actype, int source, int destination)

{
	//input values: A/C-Type, Source, Destination
	//to be calculated: Blockhours, Possible Payload
	float fCost=0, fBH=0;
	int i,j;
	struct AC_data *temp_acdata=NULL;
	struct static_cost *temp_cost;
	struct Data *temp_data;
	struct aea_input *temp_aea;

	FILE *fp;
	char ch;
	/*double MSSP,BFE,MTOW,W_fuel,W_af;
	double engine_cost,No_Engines,BPR,OPR,N_c,T_sls,N_shaft,K;
	double Price_fuel,F_resval,F_int,R_ins;
	double Life,C_coc,C_cab,N_coc,N_cab,R_lab;*/
	float ac_capacity=0;
	double W_payload;

	if(source==destination)
	return 0;

	//calculate Cost if cCostWithAEA is "y"
	if(cCostWithAEA==121)
	{
		//get blocktime in minutes
		fBH = Get_route_BT(source,destination);
		double T_blk=fBH/60;

		//calculate Distance
		temp_data=head_data;
		//going up to right place
		for(i=1;i<source;i++)
		for(j=0;j<AIRPORT_NO;j++)
			temp_data=temp_data->next_data;
		for(i=1;i<destination;i++)
			temp_data=temp_data->next_data;
		double L_stage=temp_data->distance;	//stage distance in km

		//calculate Payload in metric tons
		ac_capacity=Get_capacity(actype,source,destination);
		if(temp_data->demand > ac_capacity)
		W_payload=ac_capacity;
        else
        W_payload=temp_data->demand;

		//read aea_input data
		temp_aea=head_aea_input;
		//goto actype
		for(i=1;i<actype;i++)
			temp_aea=temp_aea->next_ac;
		double MSSP=temp_aea->MSSP;
		double BFE=temp_aea->BFE;
		double MTOW=temp_aea->MTOW;
		double W_fuel=temp_aea->W_fuel;
		double W_af=temp_aea->W_af;
		double engine_cost=temp_aea->engine_cost;
		double No_Engines=temp_aea->No_Engines;
		double BPR=temp_aea->BPR;
		double OPR=temp_aea->OPR;
		double N_c=temp_aea->N_c;
		double T_sls=temp_aea->T_sls;
		double N_shaft=temp_aea->N_shaft;
		double Price_fuel=temp_aea->Price_fuel;
		double F_resval=temp_aea->F_resval;
		double F_int=temp_aea->F_int;
		double R_ins=temp_aea->R_ins;
		double Life=temp_aea->Life;
		double C_coc=temp_aea->C_coc;
		double C_cab=temp_aea->C_cab;
		double N_coc=temp_aea->N_coc;
		double N_cab=temp_aea->N_cab;
		double R_lab=temp_aea->R_lab;
		double K;

		//temp: calculate blockfuel with a linear approximation:
		struct AC_data *temp_ac_data;
		temp_ac_data=head_ac_data;
		//going to data of aircraft type
		while(temp_ac_data->ac_type!=actype)
		temp_ac_data=temp_ac_data->next_type;
		W_fuel=5+W_fuel*(temp_data->distance/temp_ac_data->fR3);//Min 5 tons blockfuel
		//end temp
		
	
		//double T_flight=16000;	//time of flight (oder vielleicht eher weglassen und direkt blockhours nehmen
		//--E_tdel->Eng_labour->C_maint->C_opr-----
		if(N_shaft==1)
		K=0.5;
		else if(N_shaft==2)
		K=0.57;
		else if (N_shaft==3)
		K=0.64;
		else
		exit(1);
		//--total_invest->C_depr->C_stand--
		double ADP=MSSP+BFE+0.06*(MSSP+BFE)+0.025*MSSP;
		double Airfr_spares=0.1*(ADP-engine_cost);
		double Eng_spares=0.3*engine_cost;
		//--U_ann->C_depr->C_stand---------
		//double T_blk=T_flight/3600+0.25; //unit: hours -> plug in T_blk directly
		double T_flight=T_blk-0.25; //reverse
		//--Eng_labour->C_maint->C_opr-----
		double E_tdel=0.21*R_lab*(1.27-0.2*pow(BPR,0.2))*(0.032*N_c+K)*pow((1+T_sls),0.4);
		double C_del=1.3*E_tdel;
		//--Eng_material->C_maint->C_opr---
		double E_tdem=2.56*pow((1+T_sls),0.8)*(1.27-0.2*pow(BPR,0.2))*((0.4*pow((OPR/20),1.3)+0.4)+(0.032*N_c+K));
		double C_dem=1.3*E_tdem;
		//--C_depr->C_stand----
		double total_invest=ADP+Airfr_spares+Eng_spares;
		double U_ann=T_blk*3750/(T_blk+0.5);
		//--C_crew->C_opr------
		double Coc_crew_cost=C_coc*N_coc;
		double Cab_crew_cost=C_cab*N_cab;
		//--C_maint->C_opr-----
		double Airfr_labour=(0.09*W_af+6.7-350/(W_af+75))*((0.8+0.68*(T_blk-0.25))/T_blk)*R_lab;
		double Airfr_material=((4.2+2.2*(T_blk-0.25))/T_blk)*(ADP-engine_cost);
		double Eng_labour=E_tdel+C_del;
		double Eng_material=E_tdem+C_dem;
		double E_doc=(No_Engines*(E_tdel+E_tdem)*(T_flight-0.25+1.3)/T_flight);
		//--C_stand------------
		double C_depr=total_invest*1000000*(1-F_resval)/(U_ann*Life);
		double C_intr=total_invest*1000000*F_int/U_ann;
		double C_ins=R_ins*ADP*1000000/U_ann;
		//--C_apt--------------
		double C_nav=0.5*L_stage*pow((MTOW/50),0.5)/T_blk;//stimmt
		double C_hand=100*W_payload/T_blk;
		double C_land=7.8*MTOW/T_blk;
		//--C_opr--------------
		double C_fuel=W_fuel*1000*Price_fuel/T_blk;//pricefuel is per kilo?!
		double C_crew=Coc_crew_cost+Cab_crew_cost;//stimmt
		double C_maint=Airfr_labour+Airfr_material+E_doc;
		//--DOC_total----------
		double C_stand=C_depr+C_intr+C_ins;//stimmt
		double C_apt=C_nav+C_hand+C_land;//stimmt
		double C_opr=C_fuel+C_crew+C_maint;		
		//--FINAL--------------
		double DOC_total=C_stand+C_opr+C_apt;
		//printf("\n\n%.5f",DOC_total);
		//employ AE method START

		fCost=DOC_total*T_blk*40/100000;//DOC*T_blk*(dollar to rupies)/(1lakh)
		return fCost; //Lakh Rupies for the flight

	}
	else if(cCostWithAEA==110) //means: take cost value from input file (ac-specific)
	{
		temp_acdata=head_ac_data;
		//go to the actype cost data
		while(temp_acdata->ac_type!=actype)
			temp_acdata=temp_acdata->next_type;
		temp_cost=temp_acdata->static_cost;
		//going up to right place
		for(i=1;i<source;i++)
		for(j=0;j<AIRPORT_NO;j++)
			temp_cost=temp_cost->next_cost;
		for(i=1;i<destination;i++)
			temp_cost=temp_cost->next_cost;
		//printf("\n\nACtype: %d Airport: %d Nach: %d Cost: %f",actype,source,destination,temp_cost->cost);
		return temp_cost->cost;
	}
	else
	{
		printf("Error in cCostWithAEA");
		getch();
		exit(1);
	}
}    //end of function