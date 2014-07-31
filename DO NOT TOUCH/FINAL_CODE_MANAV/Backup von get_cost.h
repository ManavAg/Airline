float Get_cost(int, int, int);
float Get_cost(int actype, int source, int destination)

{
	//input values: A/C-Type, Source, Destination
	//to be calculated: Blockhours, Possible Payload
	float fCost=0, fBH=0;
	int i,j;
	struct AC_data *temp_acdata=NULL;
	struct static_cost *temp_cost;

	if(source==destination)
	return 0;

	//calculate BH if cCostWithAEA is "y"
	if(cCostWithAEA==121)
	{
		fBH = Get_route_BT(source,destination);

		//employ AE method START
		
		//Input Data at interface for DOC calc
		double L_stage=2000;	//stage distance in km
		double W_payload=15;	//payload in metric tons
		double T_flight=16000;	//time of flight (oder vielleicht eher weglassen und direkt blockhours nehmen
		//Input Data:
		double F_resval=0.1;   //Aircraft Residual value as a fraction of original ADP=0,1
		double Life=14;		//useful operational life of the aircraft	
		double F_int=0.053;		//factor on account of rate of interest = 0,053
		double R_ins=0.5;		//interest rate =0,5%
		double MSSP=100;		//manufacturers standard study price in million $
		double BFE=6;		//Buyer Furnished Equipment in million $
		double MTOW=100;		//max takeoff weight in metric tons
		double W_fuel=20;		//block fuel weight in metric tons
		double Price_fuel=0.21;	//price of fuel in $/kg
		double C_coc=246.5;		//cost of cockpit crew member $/h
		double C_cab=81;		//cost of cabin crew member $/h
		double N_coc=2;		//no. of cockpit crew members
		double N_cab=0;		//no. of cabin crew members
		double W_af=60;		//Airframe weight in metric tons i.e Manufcaturers Empty Wight +2% less weight of bare engines
		double R_lab=63;		//labor rate incl. burden = $63/h (1989 price)
		double engine_cost=10; //total cost of engines in million $
		double No_Engines=4;	//number of engines
		double BPR=5;		//bypass pressure ratio
		double OPR=20;		//overall enginepressure ratio
		double N_c=2;		//Number of compr. stages incl fan stages
		double T_sls=80	;	//Sea level static TO thrust in metric tons
		double N_shaft=2;	//no. of shafts
		double K;

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
		double T_blk=T_flight/3600+0.25;
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
		printf("\n\n%.5f",DOC_total);
		//employ AE method START

		fCost=DOC_total*40*fBH/600000;//DOC*(dollar to rupies)*BH/(10000*60min)
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