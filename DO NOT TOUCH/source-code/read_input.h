#include "get_ap_index.h"
void Read_input();

void Read_input()
{
    FILE *fp;
    char ch;
    int i,j,k;//NEU_DOC
    int total_capacity=0;
    float cost,system_cost;
    struct timing *current_time,*last_time;
    struct Airports *airport,*last_airport;
    struct Data *data,*last_data;
    struct Fleet *fleet,*last_fleet;
    struct Bases *base,*last_base;
    struct Grey *grey,*last_grey;
	struct AC_data *ac_data=NULL,*last_ac_data=NULL;//NEU_MAC
	struct static_cost *head_static_cost, *static_cost, *last_static_cost;//NEU_DOC
	struct aea_input *aea_input, *last_aea_input;//NEU_DOC
    float sum;
    
    
    //opening the input file
 
    fp=fopen(input_filename,"r");
    if(fp==NULL)
    {
        printf("\n cann't open the %s file",input_filename);
        getch();
        exit(1);
    } 
    

    /*
    //opening the input file
    fp=fopen("Input.txt","r");
    if(fp==NULL)
    {
        printf("\n cann't open the \"Input\" file");
        getch();
        exit(1);
    }  
    */  
    //reading the initial comment lines
    for(i=0;i<6;i++)
    {
        do
        {
            ch=getc(fp);
        }while(ch!='\n');
    }  
    //reading total number of airports in the network
    //NEU die zeile wurde deaktiviert TEMPORÄR WG VARIABLE LENGTH ARRAY
	fscanf(fp,"%d",&AIRPORT_NO);//wieder rück
	//NEU
    
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }

    //Reading the Airport details
    for(i=0;i<AIRPORT_NO;i++)
    {
        
        airport=(struct Airports *)malloc(sizeof(struct Airports));
        fscanf(fp,"%d %s %f %d",&airport->ap_index,airport->name,&airport->delay,&airport->takeoff_slots);
        airport->takeoff_time=NULL;
		//NEU
		airport->next_airport=NULL;
		//NEU
        for(j=0;j<airport->takeoff_slots;j++)
        {
            current_time=(struct timing *)malloc(sizeof(struct timing));
            fscanf(fp,"%f-%f",&current_time->start,&current_time->end);
            current_time->next_time=NULL;
            if(airport->takeoff_time==NULL)
            {
                airport->takeoff_time=current_time;
                last_time=current_time;
            }
            else
            {
                last_time->next_time=current_time;
                last_time=current_time;
            }            
        }
        fscanf(fp,"%d",&airport->landing_slots);
        airport->landing_time=NULL;
        for(j=0;j<airport->landing_slots;j++)
        {
            current_time=(struct timing *)malloc(sizeof(struct timing));
            fscanf(fp,"%f-%f",&current_time->start,&current_time->end);
            current_time->next_time=NULL;
            if(airport->landing_time==NULL)
            {
                airport->landing_time=current_time;
                last_time=current_time;
            }
            else
            {
                last_time->next_time=current_time;
                last_time=current_time;
            }
        }
        fscanf(fp,"%f",&airport->BELF);
        
        if(head_airport==NULL)
        {
            head_airport=airport;
            last_airport=airport;
        }
        else
        {
            last_airport->next_airport=airport;
            last_airport=airport;
        }    
    
    } 
    /*
    //printing the airport details
    printf("\n Airport Detail -");
    airport=head_airport;
    for(j=0;j<AIRPORT_NO;j++)
    { 
       
       printf("\n %d %s %.2f %d ",airport->ap_index,airport->name,airport->delay,airport->takeoff_slots);
       last_time=airport->takeoff_time;
        for(i=0;i<airport->takeoff_slots;i++)
        {
            printf(" %.2f-%.2f ",last_time->start,last_time->end);
            last_time=last_time->next_time;
        }  
        printf(" %d ",airport->landing_slots);
        last_time=airport->landing_time;
        for(i=0;i<airport->landing_slots;i++)
        {
            printf(" %.2f-%.2f ",last_time->start,last_time->end);
            last_time=last_time->next_time;
        } 
        printf(" %.2f ",airport->BELF);
        airport=airport->next_airport;
    } 
    */
    //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the demand array
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        data=(struct Data *)malloc(sizeof(struct Data));
        fscanf(fp,"%f",&data->demand);
        data->next_data=NULL;
        if(head_data==NULL)
        {
            head_data=data;
            last_data=data;
        }
        else
        {
            last_data->next_data=data;
            last_data=last_data->next_data;
        }
    } 
    
     /*operating cost read deactivated NEU_DOC
	 //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }           
    //reading the  Operating cost array
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->TOC);
        data=data->next_data;
    }NEU_DOC deactivation end*/
   /* 
    //calculating total system operating cost
    data=head_data;
    system_cost=0;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        system_cost+=data->TOC;
        data=data->next_data;
    }    
    //calculating the cost ratio at each airport
    airport=head_airport;
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    {
        cost=0;            
        for(j=0;j<AIRPORT_NO;j++)
        {
            cost+=data->TOC;
            data=data->next_data;
        }
        airport->ap_cost_ratio=cost/system_cost;
        airport=airport->next_airport;
    }        
    
    
    
  */  
    
    
    //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }           
    //reading the  Block Time array
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->block_time);
        data=data->next_data;
    }     
    //reading the comment lines NEU_PLR
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//reading the  Distance array NEU_PLR
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->distance);
        data=data->next_data;
    }     
    /* reading of capacity deactivated NEU_PLR
	//reading the comment lines NEU_DOC
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the  Aircraft capacity Array
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->capacity);
        data=data->next_data;
    }deactivation end NEU_PLR*/
    
    //reading the comment lines
     for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }  
    //reading total aircraft available in fleet
    fscanf(fp,"%d",&fleet_size);      
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }  
    //reading the fleet detail
    for(i=0;i<fleet_size;i++)
    {
        fleet=(struct Fleet *)malloc(sizeof(struct Fleet));
        fscanf(fp,"%d %d %s %f %f %f",&fleet->ac_index,&fleet->ac_type,fleet->ap_name,&fleet->takeoff_time,&fleet->life,&fleet->BH_max); //NEU_MAC NEU_BH (ac_type,BH_max,BH_remain)
		fleet->BH_remain=fleet->BH_max; //NEU_BH
        fleet->ap_index=Get_ap_index(fleet->ap_name);
        if(fleet->ap_index==NOT_FOUND)
        {
            printf("\n Airport not found in the list, Please check the airport names");
            getch();
            exit(1);
        }    
        fleet->next_aircraft=NULL;
        if(head_fleet==NULL)
        {
            head_fleet=fleet;
            last_fleet=fleet;
        }
        else
        {
            last_fleet->next_aircraft=fleet;
            last_fleet=last_fleet->next_aircraft;
        }
    }  
    /*
    //printing the fleet detail
    printf("\n Fleet detail");
    fleet=head_fleet;
    for(i=0;i<fleet_size;i++)
    {
        printf("\n %d %s %d %.2f %.2f ",fleet->ac_index,fleet->ap_name,fleet->ap_index,fleet->takeoff_time,fleet->life);
        fleet=fleet->next_aircraft;
    }    
    */
	//reading the comment lines
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//reading no. of ac_types NEU_MAC
	fscanf(fp,"%d",&ACTYPE_NO);
	//reading the comment lines NEU_DOC
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	fscanf(fp,"%c",&cCostWithAEA);
	if (cCostWithAEA!=110 && cCostWithAEA!=121)
	{
		printf("\nCost Function Input must be y or n\nEND");
		getch();
        exit(0);
	}
	//reading the comment lines NEU_MAC
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//reading fleet type specification NEU_MAC NEU_PLR
	for(i=0;i<ACTYPE_NO;i++)
    {
		ac_data=(struct AC_data *)malloc(sizeof(struct AC_data));
        fscanf(fp,"%d %s %f %f %f %f %f",&ac_data->ac_type,&ac_data->ac_code,&ac_data->fR1,&ac_data->fR2,&ac_data->fR3,&ac_data->fPL1,&ac_data->fPL2); //NEU_MAC
        ac_data->next_type=NULL;
        if(head_ac_data==NULL)
        {
            head_ac_data=ac_data;
            last_ac_data=ac_data;
        }
        else
        {
            last_ac_data->next_type=ac_data;
            last_ac_data=last_ac_data->next_type;
        }
    }  
	//reading the comment lines NEU_DOC
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
	//Reading Static Operating Cost NEU_DOC
	ac_data=head_ac_data;
	for(i=0;i<ACTYPE_NO;i++)
	{

		head_static_cost=NULL;
		for(j=0;j<(AIRPORT_NO*AIRPORT_NO);j++)
		{
			static_cost=(struct static_cost *)malloc(sizeof(struct static_cost));
			fscanf(fp,"%f",&static_cost->cost); //NEU_MAC
			static_cost->next_cost=NULL;
			if(head_static_cost==NULL)
			{
				head_static_cost=static_cost;
				last_static_cost=static_cost;
			}
			else
			{
				last_static_cost->next_cost=static_cost;
				last_static_cost=last_static_cost->next_cost;
			}
		}
		ac_data->static_cost=head_static_cost;
		ac_data=ac_data->next_type;
		for(k=0;k<2;k++)
		{    
			do
			{
					ch=getc(fp);
			}while(ch!='\n');
		}
	}

	/*//reading the comment lines NEU_MAC
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }*/
    //reading the total number of bases in network 
	//NEU die zeile wurde deaktiviert
    fscanf(fp,"%d",&total_bases);//NEU_VLA bases wieder einlesbar, alles durch malloc ersetzt
	//NEU
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    } 
    //Reading the base station detail
    for(i=0;i<total_bases;i++)
    {
        base=(struct Bases *)malloc(sizeof(struct Bases));
        fscanf(fp,"%d %s %d",&base->base_index,base->base_name,&base->hanger_capacity);                  
        base->base_index=Get_ap_index(base->base_name);
        base->next_base=NULL;
        if(head_base==NULL)
        {
            head_base=base;
            last_base=base;
        }
        else
        {
            last_base->next_base=base;
            last_base=last_base->next_base;
        }
    }
    base=head_base;
    while(base!=NULL)
    {
        total_capacity+=base->hanger_capacity;
        base=base->next_base;
    }
    if(fleet_size>total_capacity)
    {
        printf("\n Error : Total base capacity is less then the fleet size");
        getch();
        exit(1);
    }        
    //reading the comment lines
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }            
    //reading the max. time slot for operation
    fscanf(fp,"%f",&max_time_slot);

    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }   
    //reading the route priorities   
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        fscanf(fp,"%f",&data->priority);
        data=data->next_data;
    }  
    //checking the data entries, main diagnol elements should be zero
    data=head_data;
    for(i=0;i<AIRPORT_NO;i++)
    for(j=0;j<AIRPORT_NO;j++)
    {
        if(i==j)
        {
            if(data->block_time!=0)
            {
                printf("\n\n ERROR ! - the diagonal elements in Block time input should be zero\n\n");
                getch();
                exit(1);
            }    
            if(data->demand!=0)
            {
                printf("\n\n ERROR ! - the diagonal elements in Demand input should be zero\n\n");
                getch();
                exit(1);
            }
            if(data->distance!=0) //NEU_PLR (was ->capacity)
            {
                printf("\n\n ERROR ! - the diagonal elements in Aircraft capacity input should be zero\n\n");
                getch();
                exit(1);
            }
            /*if(data->TOC!=0) deactivated NEU_PLR
            {
                printf("\n\n ERROR ! - the diagonal elements in Total operating cost input should be zero\n\n");
                getch();
                exit(1);
            }*/
            if(data->priority!=0)
            {
                printf("\n\n ERROR ! - the diagonal elements in Priority of Routes input should be 0 \n\n");
                getch();
                exit(1);
            }
        }    
        data=data->next_data;
    }    
        
  /*
    //reading the comment lines
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    } 
    //reading the total number of grey indices
    fscanf(fp,"%d",&total_grey_indices);
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    
    //reading the grey indices priorities
    for(i=0;i<total_grey_indices;i++)
    {
        grey=(struct Grey *)malloc(sizeof(struct Grey));
        fscanf(fp,"%f",&grey->priority);
        grey->next_grey=NULL;
        if(head_grey==NULL)
        {
            head_grey=grey;
            last_grey=grey;
        }
        else
        {
            last_grey->next_grey=grey;
            last_grey=last_grey->next_grey;
        }
    } 
    
   
    //checking - the sum of priorities should =1
    grey=head_grey;
    sum=0;
    for(i=0;i<total_grey_indices;i++)
    {
        sum+=grey->priority;
        grey=grey->next_grey;
    }
    if(sum!=1)
    {
        printf("\n\n Error -");
        printf("\n The sum of grey Indices priorites should = 1");
        printf("\n Please check the input file");
        printf("\n Press any key to exit-");
        getch();
        exit(1);
    }        
    */
    
    //reading the comment lines
    for(i=0;i<4;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //assiging the memory for reading the whitening function values or decision values
    BIG=(struct Whitening_decision *)malloc(sizeof(struct Whitening_decision));
    SMALL=(struct Whitening_decision *)malloc(sizeof(struct Whitening_decision));
   
     //reading the whitening values for demand index  
    fscanf(fp,"%f %f",&BIG->demand,&SMALL->demand);
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for cost index  
    fscanf(fp,"%f %f",&BIG->cost,&SMALL->cost);
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for time index  
    fscanf(fp,"%f %f",&BIG->time,&SMALL->time);
    /*
    //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for Demand satisfaction index 
     fscanf(fp,"%f %f",&BIG->network_design,&SMALL->network_design);
     */
     //reading the comment lines
    for(i=0;i<3;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the whitening values for route priority index  
    fscanf(fp,"%f %f",&BIG->route_priority,&SMALL->route_priority);
   

     
     //reading the comment lines
    for(i=0;i<2;i++)
    {    
        do
        {
                ch=getc(fp);
        }while(ch!='\n');
    }
    //reading the TURN AROUND TIME
    fscanf(fp,"%f",&TURN_AROUND_TIME);
    //closing the input file
    fclose(fp);

	//read AEA input file NEU_DOC
	if(cCostWithAEA==121)
	{
		fp=fopen("DataAEA.txt","r");
		if(fp==NULL)
		{
			printf("\n Can't find DataAEA.txt for AEA calculation");
			getch();
			exit(1);
		}
		//reading the initial lines
		for(i=0;i<4;i++)
		{
			do
			{
				ch=getc(fp);
			}while(ch!='\n');
		}  
		for(j=0;j<ACTYPE_NO;j++)
		{
			aea_input=(struct aea_input *)malloc(sizeof(struct aea_input));
			//read comment lines
			for(i=0;i<4;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}  
			//Read Airframe data
			fscanf(fp,"%lf %lf %lf %lf %lf",&aea_input->MSSP,&aea_input->BFE,&aea_input->MTOW,&aea_input->W_fuel,&aea_input->W_af);
			//next row
			for(i=0;i<3;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}
			//Read Engine Data
			fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf",&aea_input->engine_cost,&aea_input->No_Engines,&aea_input->BPR,&aea_input->OPR,&aea_input->N_c,&aea_input->T_sls,&aea_input->N_shaft);
			//next row
			for(i=0;i<3;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}
			//Read Finance Data
			fscanf(fp,"%lf %lf %lf %lf",&aea_input->Price_fuel,&aea_input->F_resval,&aea_input->F_int,&aea_input->R_ins);
			//next row
			for(i=0;i<3;i++)
			{
				do
				{
					ch=getc(fp);
				}while(ch!='\n');
			}
			//read Other Data
			fscanf(fp,"%lf %lf %lf %lf %lf %lf",&aea_input->Life,&aea_input->C_coc,&aea_input->C_cab,&aea_input->N_coc,&aea_input->N_cab,&aea_input->R_lab);
			aea_input->next_ac=NULL;
			if(head_aea_input==NULL)
			{
				head_aea_input=aea_input;
				last_aea_input=aea_input;
			}
			else
			{
				last_aea_input->next_ac=aea_input;
				last_aea_input=last_aea_input->next_ac;
			}
		}
		fclose(fp);
	}//end of if(cCostWithAEA==121)//NEU_DOC

}//end of function    
