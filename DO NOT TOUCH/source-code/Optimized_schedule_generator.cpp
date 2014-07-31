/* To understanf GADO code , use the GADO code only
 * This program is highly modified to fit as per our requirments -Ajeet Singh
 */

/* The author of this software is Khaled Rasheed.
 * Copyright (c) 1998,1999,2000,2001,2002 by Khaled Rasheed. All rights
 * reserved. Distribution in any possible way is strictly prohibited
 * without explicit permission from the author.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, THE AUTHOR DOES NOT MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */
/*******************************************************/
//files added by ajeet
/********************************************************/
#include "cargo_schedulling.h"
long int all_trials_counter; //global variable
int best_case;
double min_objective_value;
double best_x[total_grey_indices];//no. of variables = total_grey_indices (declared in include_files.h)
//NEU_UT old:double best_obj_value[4];
double best_obj_value[5];//NEU_UT

/********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#ifdef __alpha
#include <sys/time.h>
#include <sys/resource.h>
#else
#ifdef __sun
#include <sys/time.h>
#include <sys/resource.h>
extern int getrusage( int who, struct rusage *rusage);
#else
#include <time.h>
#endif
#endif

#define maxn 25
#define popsize 250
#define storesize 20000
#define goup ((double)evals/(double)maxevals)
#define godown (1-(double)evals/(double)maxevals)
#define sqr(x) ((x)*(x))

void reseed();
void displaypop();
extern double drand48(); 
extern void srand48(long int rseedy); 

typedef struct individual 
{
  double       r[maxn+1];
  double       fitness;
  double       mom;
  double       violation;
  double       norm;
  char         cause;
}
individual;

/* Changes carried out for Windows Version of the code */


////////////////////////


double drand48(void);


#define C 16807
#define A 2147483647.0
double yz;

#pragma CODE_SECTION(srand48,  "xcode");
#pragma CODE_SECTION(drand48,  "icode");

extern void srand48(long int seed) {
yz= (double) seed;
} 

double drand48() {
long int ki;
double uu;

ki=(C* yz)/A;
yz=C* yz-ki*A;
uu=yz/(A-1);
return uu;
}

///////////////////////////

static individual pop[popsize+1],store[storesize+1],
  newborn,best,fittest,center;
static char ga_start[300]="",ga_end[300]="";
static int n,maxpop=100,maxstore=1000,minevalpop=10,maxevals=10000,
  maxevals_increment=1000,nnofset=0,nnk=2, maxtry=1000000,evals=0,
  succevals=0,feasevals=0,ins=0,outs=0,redundants=0,    
  population=0,stored=0, addnewborn=0,
  candidate1, candidate2, trycount, reseedcount=0,
  maxreseeds=20,no_screen=0,in_reseed=0,no_penalty_decrease=1,
  easy_start=1,gready_finish=1,nonlinear_penalty=0;
static double verybig=1e100, commonvar1=0,commonvar2=0,
  minr[maxn+1],maxr[maxn+1],sclr[maxn+1], nndist,
  currentradius1,currentradius2,currentradius,initialradius=0,
  reseedfraction=0.25,mutation_factor=1,GC_factor=0.2,GC_scale=1,GC_shrink=1,
  penaltycoef=1,penaltymult=2,penaltypower=1, rejecttolerance=1,
  stoptolerance=0.001,initialcrowdingfactor=0.9,finalcrowdingfactor=0;
  /* =========================================== */
enum {mode_read, mode_write, mode_append};
FILE* myfopen(char* f_name, int mode)
{
    char* mode_str;
    FILE* fp;

    if(mode == mode_read)
      mode_str = "r";
    else if(mode == mode_write)
      mode_str = "w";
    else mode_str = "a";
    fp = fopen(f_name, mode_str);

    if (fp == NULL)
    {
      if(mode == mode_read)
	mode_str = "read";
      else if(mode == mode_write)
	mode_str = "write";
      else mode_str = "append";
    
      printf("Open of %s for %s failed\n", f_name,
		 mode_str);
		 getch();
      exit(1);
    }
    return(fp);
}

void display(individual b)
{
  int i;
  
  printf("--- %.10g  (%.10g %.10g) ",b.fitness,b.violation,b.mom);
  for(i=1;i<=n;i++) printf("%.20g ",b.r[i]*sclr[i]);
  printf("& %d %d %d %d %c %d %d %d\n",
	 evals,succevals,feasevals,trycount,b.cause,ins,outs,redundants);
  fflush(stdout);
}

double distance(individual x,individual y)
{  
  int i;
  double temp=0;
 
  for(i=1;i<=n;i++)
    temp+=sqr(x.r[i]-y.r[i]);
/*    temp+=sqr((x.r[i]-y.r[i])/(maxr[i]-minr[i])); */
  return sqrt(temp); 
}

int selectgood(int p)
{
  int chosen;
  double s,m;

  if((easy_start)&&(goup<0.25)) 
    chosen= 1+floor(drand48()*(p-1));   /* no pressure */
  else
    if((gready_finish)&&(goup>0.75))
      chosen=1+floor(drand48()*drand48()*(p-1));   /* high pressure */
  else                               /* medium pressure */
    {
      s=drand48()*p*(p+1)/2.0;
      m=(2*p+1-sqrt(sqr(2*p+1)-8*s))/2.0;
      if(m<=0) chosen=1; else chosen=ceil(m);
    }
  return chosen;
}

double radius(void)
{
  double temp1=0,temp2=0;
  int i,j,count=0;
  
  for(i=1;i<=n;i++) center.r[i]=0;
  for(j=1;j<population;j++)
    /*    if(pop[j].fitness<verybig)  */
      {
	count+=1;
	for(i=1;i<=n;i++) center.r[i]+=pop[j].r[i];
	temp2+=distance(pop[1],pop[j]);
      }
  if(count<=0) count++;
  for(i=1;i<=n;i++) center.r[i]/=count;
  for(j=1;j<population;j++)
    /*    if(pop[j].fitness<verybig)  */
      {
	temp1+=distance(center,pop[j]);
      }
  temp1/=count;
  temp2/=count;
  currentradius1=temp1;
  currentradius2=temp2;
 
  return (temp1+temp2)/2;
}

int is_evaluable(individual x)
{
  return (x.fitness<verybig);
}
int is_feasible(individual x)
{
  return ((x.fitness<verybig)&&(x.violation==0));  
}

double get_fitness(double x, double y) 
{ 
  return (x+penaltycoef*pow(y,penaltypower));
}

void recomputefitness()
{
  int i,j;
  individual temp;
  double proposed;
  
  if(fittest.fitness>=best.fitness) 
    {
      if(no_penalty_decrease) return;
      j=0;
      for(i=1;i<population;i++) if(!(is_feasible(pop[i]))) j=1;
      if((j)||(penaltycoef<=0)) 
	return; 
      else penaltycoef/=sqrt(sqrt(penaltymult));
    }
  else
    {  
      proposed=(best.mom-fittest.mom)/
	(pow(fittest.violation,penaltypower)-pow(best.violation,penaltypower));
      if(proposed<=penaltycoef) return;
      if(proposed>penaltymult*penaltycoef) 
	penaltycoef*=penaltymult;
      else penaltycoef=proposed;
    }
  
  for(i=1;i<=population;i++)
    {
      pop[i].fitness=get_fitness(pop[i].mom,pop[i].violation);
      temp=pop[i];
      j=i-1;
      while (((j > 0) && (pop[j].fitness > temp.fitness)))
	{
	  pop[j + 1] = pop[j];
	  j-=1;
	}
      if(j<i-1) pop[j+1]=temp;
    }
  best.fitness=get_fitness(best.mom,best.violation);
  fittest=pop[1];
  printf("*** Penaly Coef=%g\n",penaltycoef);
}

double       ftness(individual   x,
		    double (*func)(double []),
		    double (*penalty)(double []))
{  
  double temp1,temp2;
  int   will_display=1;

  temp1=func(x.r);
  if((temp1>=1e10)||(temp1<=-1e10)) temp1=verybig;
  commonvar1=temp1; 
  temp2=penalty(x.r);
  if(nonlinear_penalty)
    {
      if(temp2<1) temp2=sqrt(temp2); else temp2=sqr(temp2);
    }
  if(temp1==verybig) temp2=temp1;
  commonvar2=temp2; 
  temp2=get_fitness(temp1,commonvar2);
  evals+=1;
  if(temp2<verybig) succevals+=1;
  if((temp1==temp2)&&(temp1!=verybig)) feasevals+=1;
  if ((commonvar2<best.violation)
      ||((temp2<best.fitness)&&(commonvar2==best.violation)))
    {
      /* printf("## %.15g %.15g %.15g
	 %.15g\n",temp2,best.fitness,best.fitness-temp2,distance(x,best)); */
      printf("+++");
      if (temp2<fittest.fitness)
	{
	  printf("&&&");
	  will_display=0;
	}
      best = x;
      best.mom=commonvar1;
      best.violation=commonvar2;
      best.fitness=temp2;
      display(best);
    }
  if (temp2<fittest.fitness)
    {
      fittest = x;
      fittest.mom=commonvar1;
      fittest.violation=commonvar2;
      fittest.fitness=temp2;
      if(will_display) display(fittest);
    }
  if (((evals % (2*maxpop))==0)&&(evals>0))
    recomputefitness();
  
  if ((evals % 100)==0)
    {
#ifdef __alpha
      struct rusage r_usage;
#endif
#ifdef __sun
      struct rusage r_usage;
#endif
      printf("$$$ %d %.5g (%.5g %.5g) %.5g (%.5g %.5g)",evals,best.fitness,best.violation,best.mom,fittest.fitness,fittest.violation,fittest.mom);
#ifndef __alpha
#ifndef __sun
      printf(" CPU time = %f\n",(double)clock()/CLOCKS_PER_SEC);
#endif
#endif
#ifdef __alpha
      getrusage(RUSAGE_SELF,&r_usage);
      printf(" CPU:opt= %f  ",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
      getrusage(RUSAGE_CHILDREN,&r_usage);
      printf("sim = %f\n",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
#endif
#ifdef __sun
      getrusage(RUSAGE_SELF,&r_usage);
      printf(" CPU:opt= %f  ",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
      getrusage(RUSAGE_CHILDREN,&r_usage);
      printf("sim = %f\n",(double)r_usage.ru_utime.tv_sec+(double)r_usage.ru_utime.tv_usec/1e6);
#endif
      {
	currentradius=radius();
	printf("### RADII %.10g %.10g %.10g %.10g\n",
	       currentradius1,currentradius2,currentradius,
	       reseedfraction*initialradius*godown*godown);
	if((currentradius<reseedfraction*initialradius*godown*godown)
	   &&(reseedcount<maxreseeds))
	  reseed();
      }
      fflush(stdout);
      if ((evals % 1000)==0) displaypop();
    }
  return temp2;
}
  /* ============================================ */
int knearest_neighbor(individual x, int k)
{
  int i,j,buffer[storesize],buffersize=0;
  double d,tempdist[storesize],tempfit,tempfit2;
  
  for(i=stored;i>=stored-k+1;i--)
    {
      d=distance(x,store[i]);
      j=buffersize;
      while((j > 0) && (tempdist[j]>d))
      {
	buffer[j+1]=buffer[j];
	tempdist[j+1]=tempdist[j];
	j-=1;
      }
      buffer[j+1]=i;
      tempdist[j+1]=d;
      buffersize++;
    }

  for(i=stored-k;i>=1;i--)
    {
      if((store[i].norm<x.norm+tempdist[k])
	 &&(store[i].norm>x.norm-tempdist[k]))
	if((d=distance(x,store[i]))<tempdist[k])
	  {
	    j=k;
	    while((j > 0) && (tempdist[j]>d))
	      {
		buffer[j+1]=buffer[j];
		tempdist[j+1]=tempdist[j];
		j-=1;
	      }
	    buffer[j+1]=i;
	    tempdist[j+1]=d;
	  }
    }

  i=buffer[1];
  nndist=tempdist[1];
  tempfit=get_fitness(store[i].mom,store[i].violation);
  for(j=2;j<=buffersize;j++)
    {
      int z=buffer[j];
      tempfit2=get_fitness(store[z].mom,store[z].violation);
      if(tempfit2<tempfit)
      {
	i=z;
	tempfit=tempfit2;
      }
    }
  return i;
}

int screens(individual x)
{/* machine_learning_dicision function */
  int nn;
  double tempfit,rad=rejecttolerance*initialradius*godown*godown/population; 
  double threshold;
  
  if(no_screen) return 1;
  if(evals<maxevals/4) return 1; /* too early to rely on history */
  if(drand48()<0.01) return 1;   /* to prevent infinite looping */
  nn=knearest_neighbor(x,nnk);
  if(nndist<=rad) {redundants+=1;return 0;}  /* redundant point */  
  tempfit=get_fitness(store[nn].mom,store[nn].violation);
  if(nnofset>0) threshold=pop[population-nnofset].fitness;
  else threshold=godown*pop[population].fitness
	 +goup*pop[population-1].fitness;
  return 
    (tempfit<threshold); 
}

double ga_gradient(individual x,individual y)
{
  double d;
  
  if((d=distance(x,y))<=1e-20) return -1;
  else return fabs(y.fitness-x.fitness)/d;
}

void displaypop()
{  
  int i,j; 
  FILE *IF;
  
  if(!(strcmp(ga_end,"NONE"))) return;
  IF=myfopen(ga_end,mode_append);
  fprintf(IF,"%d\n",population);    
  for(i=population;i>=1;i--)
    {
      for(j=1;j<=n;j++) fprintf(IF,"%.20g ",pop[i].r[j]*sclr[j]);
      fprintf(IF,"%.20g %.20g\n",pop[i].mom,pop[i].violation);
    }
  fclose(IF); 
}

  /* ============================================ */
void         introduce(individual   x)
{
  int      i,replaced;
  double replaceddistance;
  
  if(!(in_reseed))
    if(stored<maxstore)
      {
	stored+=1;
	store[stored]=x;
      }
    else
      {
	replaced=1 + drand48()*(stored - 1);
	store[replaced]=x;
      }
  
  if(population>=maxpop)
    { int limit;
    
      if(!(is_evaluable(x))) return; /* unevaluable */
      limit=maxpop*(1-(godown*initialcrowdingfactor+goup*finalcrowdingfactor));
      replaced=maxpop;
      replaceddistance=distance(x,pop[replaced]);
      if(!((pop[replaced].fitness>=verybig)))
	for(i=maxpop-1;((i>limit)&&(pop[i].fitness>x.fitness));i--)
	  if(distance(x,pop[i])<replaceddistance)
	    {  
	      replaced=i;
	      replaceddistance=distance(x,pop[i]);
	    } 
      i=replaced;
      if((pop[i].violation<=best.violation)&&(pop[i].mom<=best.mom))
	{   /****** never replace the best most feasible point ****/
	  if(i>=population) i--;
	  else i=population;
	}
      while(i<maxpop)
	{
	  pop[i]=pop[i+1]; 
	  i++;
	}
      population-=1;
    }
  i = population;
  while (((i > 0) && (pop[i].fitness > x.fitness)))
      {
	pop[i + 1] = pop[i];
	i-=1;
      }
  pop[i + 1] = x;
  population+=1;
}

  /* ============================================ */

double       bounded(double       min,
                     double       max)
{
  
  return min + drand48() * (max - min);
  
}

  /* ============================================ */

double       variation(int index,
		       char         typec,
		       double       current,
                       double       min,
                       double       max)
{
  double       temp,amount,size,selector;

  if(typec=='P') size=0.15*mutation_factor;
  else if(typec=='R') size=0.10*mutation_factor;
  else size=0.05*mutation_factor;
  
  selector=drand48();
  if(selector<0.1)
    { /*********** gready mutation ***********/
      if(drand48()>0.5) /*** up mutation ****/
	amount=(max-current)*drand48()*drand48();
      else /*** down mutation ****/
	amount=-(current-min)*drand48()*drand48();
      temp=current+amount;      
    }
  else if(selector<0.4)
    {  /*********** Shrinking window mutation ***********/ 
      amount=0.5*size*godown*(max-min);
      temp = bounded(current-amount,current+amount);
      if (temp < min)
	temp = min;  
      else
	if (temp > max) 
	  temp = max; 
    }
  else
    { /*********** Non Uniform mutation ***********/
      if(drand48()>0.5) /*** up mutation ****/
	amount=(max-current)*size*godown*drand48();
      else /*** down mutation ****/
	amount=-(current-min)*size*godown*drand48();
      temp=current+amount;
    }
  return temp;
}

  /* ============================================ */

void generate(double (*func)(double []),double (*penalty)(double []))
{
  individual   indbuf;
  int i,starts=0,flag=0;
  FILE *IF;
  double dum;
  
  if(strcmp(ga_start,"NONE"))
    {   
      IF=myfopen(ga_start,mode_read);
      fscanf(IF,"%d",&starts);
      printf("Reading %d points from %s\n",starts,ga_start);
      if(0==starts) fclose(IF); else flag=1;
    }
  population = 0;
  do 
    {
      if(evals<starts)
	{
	  for(i=1;i<=n;i++)
	    {
	      fscanf(IF,"%lf",&indbuf.r[i]);
	      indbuf.r[i]/=sclr[i];
	    }
	  fscanf(IF,"%lf",&dum);
	  fscanf(IF,"%lf",&dum);
	}
      else
	{
	  for(i=1;i<=n;i++)
	    indbuf.r[i] = bounded(minr[i],maxr[i]); 
	}
      indbuf.cause='Z';
      indbuf.fitness = ftness(indbuf,func,penalty);
      indbuf.mom=commonvar1;
      indbuf.violation=commonvar2;
      indbuf.norm=0;
      for(i=1;i<=n;i++) indbuf.norm+=sqr(indbuf.r[i]);
      indbuf.norm=sqrt(indbuf.norm);
      introduce(indbuf);
      if(is_evaluable(indbuf))
	{ 
	  printf("%g %d %d %d\n",indbuf.fitness,population,evals,succevals);
	  fflush(stdout);
	}
    }
  while (!((population>=maxpop)&&(evals>=minevalpop)));
  printf("end of random explore phase\n");
  initialradius=currentradius=radius(); 
  printf("Initial radius=%.10g\n",initialradius);
  if(flag) fclose(IF);
  fflush(stdout);
}
  /* ============================================ */
void         reseed()
{
  int i;
  
  if((goup>0.95)&&(gready_finish)) return;
  in_reseed=1;
  for(i=1;i<=stored;i++) 
    store[i].fitness=get_fitness(store[i].mom,store[i].violation);

  reseedcount+=1;
  population = 2;
  pop[2]=best; /****** never lose the best most feasible point ***/
  
  for(i=stored;i>=1;i--)
    if(distance(store[i],pop[1])>2*reseedfraction*initialradius*godown*godown) 
      introduce(store[i]);
  currentradius=radius();  
  printf("end of reseed phase:Population=%d RADII %.10g %.10g %.10g %.10g\n",
	 population,currentradius1,currentradius2,currentradius,
	 reseedfraction*initialradius*godown*godown);
  fflush(stdout);
  in_reseed=0;
}
  /* ============================================ */
void   guidedcrossover(individual   x,
                       individual   y,
		       double (*func)(double []),
		       double (*penalty)(double []))
{
  double   ratio,size;
  int i;
  
  size=GC_scale*0.5*(GC_shrink+(1-GC_shrink)*godown);
  ratio = bounded(-size,0.2*size);
  newborn.cause='G';
  for(i=1;i<=n;i++)
    newborn.r[i] = ratio * x.r[i] + (1 - ratio) * y.r[i];
      
  for(i=1;i<=n;i++)
    {
      if (newborn.r[i]<minr[i]) newborn.r[i]=minr[i]; 
      if (newborn.r[i]>maxr[i]) newborn.r[i]=maxr[i];
    }
  newborn.norm=0;
  for(i=1;i<=n;i++) newborn.norm+=sqr(newborn.r[i]);
  newborn.norm=sqrt(newborn.norm);
/* &&& machine learning decision */
  addnewborn=0;

  if(screens(newborn))
    { 
      newborn.fitness = ftness(newborn,func,penalty);
      newborn.mom=commonvar1 ;
      newborn.violation=commonvar2 ;
      addnewborn=1;
    }
}
	
  /* ============================================ */
void   crossover(individual   x,
                       individual   y,
		       double (*func)(double []),
		       double (*penalty)(double []))
{ 
  int i,crosspoint;
  double selector=drand48();


  if(selector<0.25)    /*************** Line crossover *************/
    { double ratio;   
      newborn.cause='L';
      if(x.fitness<y.fitness) 
	ratio = bounded(-2,3+2*goup); 
      else ratio = bounded(-2-2*goup,3);
      
      for(i=1;i<=n;i++)
	{    
	  newborn.r[i] = ratio * x.r[i] + (1 - ratio) * y.r[i];
	}
    }
  else if(selector<0.5) /********** Point crossover  **********/
    {
      newborn.cause='P';    
      crosspoint=floor(bounded(2,n));
     
      for(i=1;i<crosspoint;i++)
	newborn.r[i] = x.r[i];
      newborn.r[crosspoint]=
	(drand48()<0.5)? x.r[crosspoint]:y.r[crosspoint];
      for(i=crosspoint+1;i<=n;i++)
	newborn.r[i] = y.r[i];
    }
  else if(selector<0.75) /********** double linear crossover  **********/
    { double ratio1,ratio2;
    
      newborn.cause='D';    
      crosspoint=floor(bounded(2,n));
      ratio1 = bounded(-2,3); 
      ratio2 = bounded(-2,3); 
      for(i=1;i<=crosspoint;i++)
	  newborn.r[i] = ratio1 * x.r[i] + (1 - ratio1) * y.r[i];	

      for(i=crosspoint+1;i<=n;i++)
	  newborn.r[i] = ratio2 * x.r[i] + (1 - ratio2) * y.r[i];
    }
  else  /********** random crossover  **********/
    {
      newborn.cause='R';    
      
      for(i=1;i<=n;i++)
	newborn.r[i]=(drand48()<0.5)? x.r[i]:y.r[i];
    }

  for(i=1;i<=n;i++)
    {    
      outs+=1;
      if (newborn.r[i]<minr[i])
	newborn.r[i]=minr[i];
      else
	if (newborn.r[i]>maxr[i])
	  newborn.r[i]=maxr[i];    
	else
	  {
	    outs-=1;
	    ins+=1;
	  }
    }
  /******************** mutation *****************/  
  for(i=1;i<=n;i++)
    if(drand48()<0.4*godown*godown+0.1)
      newborn.r[i]=variation(i,newborn.cause,newborn.r[i],minr[i],maxr[i]); 

  newborn.norm=0;
  for(i=1;i<=n;i++) newborn.norm+=sqr(newborn.r[i]);
  newborn.norm=sqrt(newborn.norm);
/* &&& machine learning decision */
  addnewborn=0;
  if(screens(newborn))
  {
    newborn.fitness = ftness(newborn,func,penalty);
    newborn.mom=commonvar1 ;
    newborn.violation=commonvar2 ;
    addnewborn=1;    
  }
}


  /* ============================================ */
double ga(int global_n, double *scales, double *x, double *bl, double
*bu, char *seed_file, char *ga_input, double (*func)(double []), double (*penalty)(double []))//,double *best_x_in_ga) 
//last variable in function call is newly added to store the value of design variables for best case
{
  int i;
  FILE *IF;
  long int rseed;
  
  n=global_n;    /*** important link ******/
  
  IF=myfopen(seed_file,mode_read);
  fscanf(IF,"%ld",&rseed);
  fclose(IF);

  IF=myfopen(ga_input,mode_read);
  fscanf(IF,"maxpop=%d\n",&maxpop);
  fscanf(IF,"maxstore=%d\n",&maxstore);
  fscanf(IF,"minevalpop=%d\n",&minevalpop);
  fscanf(IF,"maxevals=%d\n",&maxevals);
  fscanf(IF,"maxevals_increment=%d\n",&maxevals_increment);
  fscanf(IF,"penaltycoef=%lf\n",&penaltycoef);
  fscanf(IF,"penaltymult=%lf\n",&penaltymult);
  fscanf(IF,"penaltypower=%lf\n",&penaltypower);
  fscanf(IF,"stoptolerance=%lf\n",&stoptolerance);
  fscanf(IF,"rejecttolerance=%lf\n",&rejecttolerance);
  fscanf(IF,"maxreseeds=%d\n",&maxreseeds);
  fscanf(IF,"reseedfraction=%lf\n",&reseedfraction);
  fscanf(IF,"mutation_factor=%lf\n",&mutation_factor);
  fscanf(IF,"GC_factor=%lf\n",&GC_factor);
  fscanf(IF,"GC_scale=%lf\n",&GC_scale);
  fscanf(IF,"GC_shrink=%lf\n",&GC_shrink);
  fscanf(IF,"Nonlinear penalty=%d\n",&nonlinear_penalty);
  fscanf(IF,"No penalty decrease=%d\n",&no_penalty_decrease);
  fscanf(IF,"Easy start=%d\n",&easy_start);
  fscanf(IF,"Gready finish=%d\n",&gready_finish);
  fscanf(IF,"No Screen=%d\n",&no_screen);
  fscanf(IF,"K for Knearest neighbor=%d\n",&nnk);
  fscanf(IF,"screening offset=%d\n",&nnofset);
  fscanf(IF,"initial_crowding_factor=%lf\n",&initialcrowdingfactor);
  fscanf(IF,"final_crowding_factor=%lf\n",&finalcrowdingfactor);
  fscanf(IF,"Starting point file=%s\n",ga_start); 
  fscanf(IF,"Ending point file=%s",ga_end); 
  fclose(IF);

  printf("Random seed=%ld\n",rseed);
  printf("Population size=%d\n",maxpop);
  printf("Store size=%d\n",maxstore);
  printf("Mimimum # of pop evals=%d\n",minevalpop);
  printf("Maximum # of Evaluations=%d\n",maxevals);
  printf("Evaluations increment over maximum=%d\n",maxevals_increment);
  printf("penaltycoef=%g\n",penaltycoef);
  printf("penaltymult=%g\n",penaltymult);
  printf("penaltypower=%g\n",penaltypower);
  printf("stoptolerance=%g\n",stoptolerance);
  printf("rejecttolerance=%g\n",rejecttolerance);
  printf("maxreseeds=%d\n",maxreseeds);
  printf("reseedfraction=%g\n",reseedfraction);
  printf("mutation_factor=%g\n",mutation_factor);
  printf("GC_factor=%g\n",GC_factor);
  printf("GC_scale=%g\n",GC_scale);
  printf("GC_shrink=%g\n",GC_shrink);
  printf("Nonlinear penalty=%d\n",nonlinear_penalty);
  printf("No penalty decrease=%d\n",no_penalty_decrease);
  printf("Easy start=%d\n",easy_start);
  printf("Gready finish=%d\n",gready_finish);
  printf("No Screen=%d\n",no_screen);
  printf("K for Knearest neighbor=%d\n",nnk);
  printf("screening offset=%d\n",nnofset);
  printf("initial_crowding_factor=%g\n",initialcrowdingfactor);
  printf("final_crowding_factor=%g\n",finalcrowdingfactor);
  printf("Starting point file=%s\n",ga_start);
  printf("Ending point file=%s\n",ga_end);
  fflush(stdout);
  
  srand48(rseed);
  for(i=1;i<=n;i++) 
    {
      minr[i]=bl[i-1];
      maxr[i]=bu[i-1];
      sclr[i]=scales[i-1];
    }
  
  best.fitness=best.mom=best.violation=verybig;
  fittest.fitness=fittest.mom=fittest.violation=verybig;

  generate(func,penalty);
  
  for (trycount = 1;
       ((trycount<=maxtry)&&(evals<=maxevals)
	&&(currentradius>stoptolerance*initialradius)); trycount++)
    {
      int j;
      double maxgradient,tempgradient;
      
      if(drand48()<GC_factor*goup)
	{
          candidate1 = selectgood(population);
          candidate2 = (candidate1==1)? 2 : 1;
	  maxgradient=ga_gradient(pop[candidate1],pop[candidate2]);
	  if((goup<0.95)||(!(gready_finish)))
	    for(j=1;j<population;j++)
	      if((j!=candidate1)&&
		 ((tempgradient=ga_gradient(pop[candidate1],pop[j]))
		  >maxgradient))
		{
		  candidate2 = j;
		  maxgradient=tempgradient;
		}
	  if(pop[candidate1].fitness<=pop[candidate2].fitness)
	    guidedcrossover(pop[candidate2],pop[candidate1],
			    func,penalty); 
	  else
	    guidedcrossover(pop[candidate1],pop[candidate2],
			    func,penalty); 
        }      
      else
        {  
          candidate1 = selectgood(population);
	  do
	    {
	      candidate2 = selectgood(population);
	    }
	  while(candidate1==candidate2);
	  crossover(pop[candidate1],pop[candidate2],func,penalty);
	}
      
/* &&& machine learning decision */
      if(addnewborn!=1) 
	continue;    
      introduce(newborn);      
      if((evals>=maxevals)&&(currentradius>stoptolerance*initialradius))
      {
	maxevals+=maxevals_increment;
	rejecttolerance=0;
	/* no_screen=1; */
	reseedcount=maxreseeds;
      }
    }
  
  for(i=1;i<=n;i++) x[i]=best.r[i];
  //new added loop to store the values of design variables for best case
  //for(i=1;i<=n;i++) best_x_in_ga[i]=best.r[i];
  displaypop();
  return best.fitness;
}
/******************* main interface  **********************/
double objective_function(double x[1+maxn])
{
   //NEU_UT old: double obj_value[4];
   double obj_value[5];//NEU_UT
   double design_x[total_grey_indices];
   double sum_of_variables;
   double output_value;
   int i;
   FILE *fp;
     int j;
    
   //NEU 
   for(i=0;i<total_grey_indices;i++)
   design_x[i]=x[i+1];//since our variable in x[1,2,3]
   sum_of_variables=1;
   //alter Code Anfang
   /*sum_of_variables=0;
   for(i=0;i<total_grey_indices;i++)
   sum_of_variables+=design_x[i];*/
   //alter Code Ende
   double dSumPercent=0;
   for(i=0;i<total_grey_indices;i++)
   dSumPercent+=design_x[i];
   for(i=0;i<total_grey_indices;i++)
   design_x[i]=design_x[i]/dSumPercent;
   //NEU 
   
   if(sum_of_variables==1)//constraint in our program
   {
       //Cargo_scheduling(design_x,3,obj_value,NO);//NO means don't print anything
       Cargo_scheduling(design_x,total_grey_indices,obj_value,YES);//YES means  print schedule
       //-----------------description of the array returned from cargo_scheduling function--------------------------//
       //obj_value[0]= demand
       //obj_value[1]=cost
       //obj_value[2]=time
       //obj_value[3]=QoS_variance
       //objective value is minimized in GADO's GA code
       //to convert to maximization problem reverse your objective function
       //-----------------------------different objective function--------------------------------------//
       
        switch(objective_pointer)
        {
        case 1:
        output_value= 1/(obj_value[0]); // min- the reverse of demand i.e. max the demand
        break;
        case 2:
        output_value= obj_value[1]; // min- the total operating cost
        break;
        case 3:
        output_value= 1/(obj_value[2]); // min- the reverse of time i.e. max time i.e. max ac utilization
        break;
        case 4:
        output_value = obj_value[3]; // min. - the variance in QoS i.e. improving the QoS
        break;
        case 5:
        output_value = obj_value[1]/obj_value[0]; // min. - cost per demand
        break;
        case 6:
        output_value=obj_value[2]/obj_value[0];  // min. - time per demand
        break;
		//NEU_UT
		case 7:
		output_value=1/(obj_value[4]);  //min the reverse of Utilization
		break;//NEU_UT
        case 8:
        output_value= obj_value[0]; // min- the demand 
        break;
        case 9:
        output_value= 1/(obj_value[1]); // min- the reverse of cost i.e. max cost
        break;
        case 10:
        output_value= obj_value[2]; // min- the total time aircrafts operated
        break;
        case 11:
        output_value= 1/(obj_value[3]); // min- the reverse of variance in QoS i.e. max the variance i.e. min QoS
        break;
        case 12:
        output_value = obj_value[0]/obj_value[1]; // min. - demand per cost
        break;
        case 13:
        output_value=obj_value[0]/obj_value[2];  // min. - demand per time 
        break;
		//NEU_UT
		case 14:
        output_value=obj_value[4];  //min the Utilization
        break;
		//NEU_UT
        default:
        printf("\n Error : Problem in objective function option");
        getch();
        exit(0);
                                        
        }    
        //------------------------------------------------------------// 
           
              // output_value = obj_value[1]/obj_value[0]; // min. - cost per demand
       //because max demand =125 and min cost =48.6 in diff runs
       //  output_value = ((obj_value[1]/48.6)+(125/obj_value[0]));// min. - cost per demand i.e. max. demand per cost
       // output_value=obj_value[2]/obj_value[0];  // min. - time per demand
       //output_value = ((obj_value[2]/(max_time_slot*fleet_size))+(initial_total_demand/obj_value[0])); //min. - time per demand i.e. max. - demand per time
       // output_value = obj_value[3]; // min. - the variance in QoS i.e. improving the QoS
       //output_value= 1/(obj_value[0]); // min- the reverse of demand i.e. max the demand
       //output_value= obj_value[1]; // min- the total operating cost
       //output_value= obj_value[2]; // min- the total time aircrafts operated
       //output_value= 1/(obj_value[2]); // min- the reverse of total time aircrafts operated i.e. max aircraft utilization
       if((best_case==NO)||(output_value <= min_objective_value ))
       { 
           min_objective_value=output_value;
           for(i=0;i<total_grey_indices;i++)
           best_x[i]=design_x[i];

		   //NEU old:for(i=0;i<4;i++)
           for(i=0;i<5;i++) //NEU_UT
           best_obj_value[i]=obj_value[i];
           best_case=YES;

          //storing this schedule    
          FILE *fp1,*fp2;
          char ch;
           fp1=fopen(output_filename,"r");
           if(fp1==NULL)
           {
               //printf("\n Error in writing in the %s file ",all_cases_filename);
               printf("\n cann't open the "); puts(output_filename);printf(" file in main optimization program");
               getch();
               exit(1);
           }
           fp2=fopen(best_cases_filename,"w");
           if(fp2==NULL)
           {
               //printf("\n Error in writing in the %s file ",all_cases_filename);
               printf("\n cann't make the "); puts(best_cases_filename);printf(" file in main optimization program");
               getch();
               exit(1);
           }
           do
           {
               ch=getc(fp1);
               putc(ch,fp2);
           }while(ch!=EOF);
           fclose(fp1);
           fclose(fp2);    
       }    
           
           
  }    
   else
   output_value= verybig;
   
   //----------printing the record---------------//
   all_trials_counter++; //global variable , initialized in main program before calling the optimizer
   fp=fopen(all_cases_filename,"a");
   if(fp==NULL)
   {
       //printf("\n Error in writing in the %s file ",all_cases_filename);
       printf("\n cann't make the "); puts(all_cases_filename);printf(" file in main optimization program");
       getch();
       exit(1);
   }
   fprintf(fp," %ld )  ",all_trials_counter);
   
   
   //NEU
   for( j=0;j<total_grey_indices;j++)
          fprintf(fp,"%f + ",design_x[j]);
   //alter Code Anfang
	//	  for( j=1;j<=total_grey_indices;j++)
   //     fprintf(fp,"%f + ",x[j]);
   //alter Code Ende
   //NEU
   fprintf(fp," = %f",sum_of_variables);    
   fprintf(fp,", objective value => %f \n",output_value);
   
   fclose(fp);
   //-------------------------------------------//
   return(output_value);
   
 
  /* this should be replaced with the code that calls your simulator, or
analysis tool and reports the measure of merit corresponding to the
variable set x[] which is the optimization variables set */
}



double penalty_function(double x[1+maxn])
{
  double temp;
  
  return 0;
  
  /* This should be replaced by a function of the constraint violations, if
any occur. I propose multiplying a large number by the sum of the absolute
values of the constraint violations */  

}

//NEU void eingefügt!
void main()
{
    double bl[10],bu[10],it,x[10],scale[10];
      unsigned long trials=1000;
      int i,j,k,l;
      
    
    
    //Ajeet ---- My new variables-------//
    //double best_x[4];//no. of variables+1
    //double final_x[3];
    //double obj_value[4];
    char *ch,temp_ch[2];//NEU old: temp_ch[1] -> access violation
    int objective_flag;
    //long int best_try_counter;
    //---------------------------------------//
    //for running different initial vaules in GA code
    int seed_counter,max_seed_counter;
    // max_seed_counter=20;
    //long int new_seed[20]={1,2,3,4,5,10,20,30,40,50,100,200,500,1000,5000,10000,20000,50000,100000,999999};
    long int initial_seed_value,new_seed;
    int seed_generator,seed_gap;
    //---------------------------------------//
    time_t start_time,end_time;
    struct tm * timeinfo;
    double diff_time;
    FILE *fp;
        
    //----------user input before calling the optimizer----------//
    printf("\n -------Airline Network Optimization for cargo Airlines--------BETA version");
    printf("\n >>>>****for testing and evalution only **** not for commercial use****<<<<");
    printf("\n ---------------------------------------------------------------------------");
    
    //-----------getting objective function from user-----------------------//
    printf("\n ------Possible Objective Functions------ ");
    printf("\n 1. Max. Cargo");  
    printf("\n 2. Min. Cost");  
    printf("\n 3. Max. Time ( or max. aircraft utilization)");
    printf("\n 4. Max. Quality of Service");
    printf("\n 5. Max. Cargo/Rs.");
    printf("\n 6. Max. Cargo/min.");     
	printf("\n 7. Max. Fleet Utilization");//NEU_UT
    printf("\n -----Worst case analysis----------");
    printf("\n 8. Min. Cargo");
    printf("\n 9. Max. Cost");
    printf("\n 10. Min. Time"); 
    printf("\n 11. Min. Quality of Service");
    printf("\n 12. Min. Cargo/Rs.");
    printf("\n 13. Min. Cargo/min.");
	printf("\n 14. Min. Fleet Utilization");//NEU_UT
    printf("\n -----------------------------------");
    printf("\n Choose your Objective Function : ");
    do
    {
        objective_flag=NO;
        scanf("%d",&objective_pointer);
        fflush(stdin);
        switch(objective_pointer)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
			case 13://NEU_UT
			case 14://NEU_UT

                objective_flag=YES;
                break;
            default:
                objective_flag=NO;
                break;
        }
        if(objective_flag==NO)
        printf("\n Enter a valid Choice :");  
    }while(objective_flag!=YES);
    fflush(stdin);

    //----------------------------------------------------------------------//
    printf("\n Enter the Input File name (to read network detail) \n ( without "".txt"" Extension) max 40 character - : ");
    //scanf("%s",&input_filename);//global variable in my program
    gets(input_filename);
    
    printf("\n Enter the Output File name (to store the final airline schedules ) \n ( without "".txt"" Extension) max 40 character - : ");
    //scanf("%s",&filename);//global variable in my program
    gets(filename);
    printf("\n Enter the Folder name (to store all output files)\n max 40 character - : ");
    //scanf("%s",&foldername);
    gets(foldername);
    printf("\n--------------------------------------------------------------------");
    printf("\n Make sure that folder is already created \n (press any key to continue) - ");
    getch();
  

        
    time(&start_time);
    timeinfo = localtime ( &start_time );
    strcat(foldername,"/");
        
    strcpy(compare_filename,input_filename);
    strcat(input_filename,".txt");
    //adding folder name with the output file name
    strcpy(temp_filename,foldername);
    strcat(temp_filename,filename);
    strcpy(filename,temp_filename);
    //adding folder name with the compare file name
    strcpy(temp_filename,foldername);
    strcat(temp_filename,compare_filename);
    strcpy(compare_filename,temp_filename);
    strcat(compare_filename,"_compare_schedules.txt");
    fp=fopen(compare_filename,"w");
            if(fp==NULL)
                {
                    printf("\n cann't make the "); puts(compare_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
			fprintf(fp,"                       Indices                                      Network Parameters                                                     Started at : %s",asctime(timeinfo));//NEU
            fprintf(fp,"\n Seed                  Demand/Cost/Time/Priority                    Demand/Cost/Time/VarQoS/Utilization                                    Objective Function Values");//NEU
			fprintf(fp,"\n -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");//NEU
	fclose(fp);       
    //----reading the inital seed value and no. of times to run the code again
    fp=fopen("ga_run","r");
    if(fp==NULL)
    {
        printf("\n Cann't open the file ga_run");
        getch();
        exit(1);
    }    
    fscanf(fp,"max no. of runs (for optimization code)=%d\n",&max_seed_counter);
    fscanf(fp,"initial seed value (any random number)=%ld",&initial_seed_value); 
    //----------running for different initial values ---------------------//
    seed_generator=0;
    seed_gap=1;
    new_seed=initial_seed_value;
    for(seed_counter=0;seed_counter<max_seed_counter;seed_counter++)
    {
            trials=1000;
            //==================================//
            //optimization variavles limit
                  /*
                  bl[0]=0;
                  bu[0]=1;
                  scale[0]=1;
                  bl[1]=0;
                  bu[1]=1;
                  scale[1]=1;
                  bl[2]=0;
                  bu[2]=1;
                  scale[2]=1;
                  bl[3]=0;
                  bu[3]=1;
                  scale[3]=1;
                  bl[4]=0;
                  bu[4]=1;
                  scale[4]=1;
                  */
                  for(i=0;i<total_grey_indices;i++)//total_grey_indices (declared in include_files.h)
                  {
                      bl[i]=0;
                      bu[i]=1;
                      scale[i]=1;
                  }    
            //==================================//
            //-------- adding the seed value with the output file name----------//
            strcpy(output_filename,filename);
            strcat(output_filename,"_");
            //generating a new seed value
            if(seed_generator==0)
            {
                seed_generator=max_seed_counter/5;
                new_seed+=((seed_gap-1)*seed_generator*initial_seed_value);
                seed_gap+=(initial_seed_value/10);
                seed_gap++;
            }
            else
            {
                
                new_seed+=((1*seed_gap)+((seed_counter*5)/max_seed_counter));
            }    
            seed_generator--;    
            //writing the seed value in the file
            fp=fopen("ga_seed","w");
            if(fp==NULL)
                {
                    printf("\n Error in writing in ga_seed file in main optimization program");
                    getch();
                    exit(1);
                }
            //fprintf(fp,"%ld",new_seed[seed_counter]);
            fprintf(fp,"%ld",new_seed);
            fclose(fp);
            //reading back as an character
            fp=fopen("ga_seed","r");
            if(fp==NULL)
                {
                    printf("\n Error in reading from ga_seed file in main optimization program");
                    getch();
                    exit(1);
                }
            fscanf(fp,"%c",temp_ch);
			temp_ch[1]='\0';//NEU
            while(!feof(fp))
            {
				strcat(output_filename,temp_ch);
                fscanf(fp,"%c",&temp_ch);
            }    
            fclose(fp);
            strcpy(all_cases_filename,output_filename);
            strcpy(best_cases_filename,output_filename);
            strcat(all_cases_filename,"_all_trials.txt");
            //strcat(best_cases_filename,"_best_trials.txt");
            strcat(best_cases_filename,"_final_schedule.txt");
            strcpy(output_filename,foldername);
            strcat(output_filename,"/");
            strcat(output_filename,"temp_file.txt");
            
            //strcat(output_filename,"_final_schedule.txt");
            //strcat(output_filename,".txt");
            //----------deleting all the privious info, if all cases file is already existing-----------------//
            fp=fopen(all_cases_filename,"w");
            if(fp==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",all_cases_filename);
                    printf("\n cann't make the "); puts(all_cases_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
            fprintf(fp," trial_no )----------variables-----------sum of variables--------objective value------------------\n");
            fclose(fp);
            //--------------reinitializing all the GADO's global variables before calling the GA code again------------------//
             maxpop=100;maxstore=1000;minevalpop=10;maxevals=10000;
              maxevals_increment=1000;nnofset=0;nnk=2; maxtry=1000000;evals=0;
              succevals=0;feasevals=0;ins=0;outs=0;redundants=0;    
              population=0;stored=0; addnewborn=0;
              candidate1; candidate2; trycount; reseedcount=0;
              maxreseeds=20;no_screen=0;in_reseed=0;no_penalty_decrease=1;
              easy_start=1;gready_finish=1;nonlinear_penalty=0;
             verybig=1e100; commonvar1=0;commonvar2=0;
             initialradius=0;
              reseedfraction=0.25;mutation_factor=1;GC_factor=0.2;GC_scale=1;GC_shrink=1;
              penaltycoef=1;penaltymult=2;penaltypower=1; rejecttolerance=1;
              stoptolerance=0.001;initialcrowdingfactor=0.9;finalcrowdingfactor=0;
              
                
            //----------calling the optimizer--------------------------------//
            //x is design variable supplied by optimizer
            all_trials_counter=0; //global variable
            best_case=NO;//global variable, an indicator to store the best case
            it=ga(total_grey_indices, scale,x, bl, bu,"ga_seed","ga_input",objective_function,penalty_function);
            
              
            
            //----------printing all the best cases--------//
            /*
            fp=fopen(best_cases_filename,"w");
            if(fp==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",best_cases_filename);
                    printf("\n cann't make the "); puts(best_cases_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
           fprintf(fp," -----------------Variables--------------------Objective Function----\n");
           best_try_counter=0;
           for(i=0;i<population;i++)
           {
               
               fprintf(fp," %ld )",best_try_counter); 
               for( j=1;j<4;j++)
                fprintf(fp," %f ",pop[i].r[j]);
               fprintf(fp," Objective value => %f \n",objective_function(pop[i].r));
               best_try_counter++;
            }    
            fclose(fp);
            */
             //=======================================================//
            // print the Final best schedule for this seed
            //=======================================================//
             
             //for(i=0;i<3;i++)
             //final_x[i]=best_x[i+1];
            
           // Cargo_scheduling(final_x,3,obj_value,YES);//YES means  print 
            
            fp=fopen(compare_filename,"a");
            if(fp==NULL)
                {
                    //printf("\n cann't make the %s file in main optimization program",best_cases_filename);
                    printf("\n cann't open the "); puts(compare_filename);printf(" file in main optimization program");
                    getch();
                    exit(1);
                }
            fprintf(fp,"\n %-15ld => ",new_seed);
            for(i=0;i<total_grey_indices;i++)
            fprintf(fp,"%9.4f ",best_x[i]);
            fprintf(fp," => ");
			//NEU_UT alt: for(i=0;i<4;i++)
            for(i=0;i<5;i++) //NEU_UT
            fprintf(fp,"%12.4f ",best_obj_value[i]);
            fprintf(fp," => ");
            fprintf(fp,"%12.4f",it);//our objective function value
            time(&end_time);
            timeinfo = localtime ( &end_time );
            fprintf(fp," , Generated on: %s ",asctime(timeinfo));
        
            fclose(fp);
    }    //end of for(seed_counter=0;seed_counter<max_seed_counter;seed_counter++)
    
        time(&end_time);
        timeinfo = localtime ( &end_time );
        printf("\n\n Ends at : %s ",asctime(timeinfo));
        diff_time = difftime(end_time,start_time);
        printf("\n Time taken = %.2lf Seconds or %.2lf Minutes",diff_time,diff_time/60.0);
        printf("\n--------------------------------------------------------");   
        printf("\n\t\t !!! Completed !!!\n");
        printf("\n ===========================================================================");
        printf("\n This program is designed and developed at ");
        printf("\n Aerospace Engineering Department, IIT Bombay \n");    
        printf("\n For any suggestion/query, contact - ");
        printf("\n Prof. Rajkumar S. Pant, LTA Systems Lab, rkpant@aero.iitb.ac.in");
        printf("\n Ajeet Singh, M.Tech. Student(2005-2007), ajeet@aero.iitb.ac.in");
		printf("\n Florian Maul, Technical University of Munich, florian.maul@mytum.de");
        printf("\n ===========================================================================");
        printf("\n Press any key to exit---\n");

        getch();
}   //end of main function 

/*
int main()
{
  double bl[10],bu[10],it,x[10],scale[10];
  unsigned long trials=1000;
  int i,j,k,l;
  
  bl[0]=0;
  bu[0]=10-0.0001;
  scale[0]=1;
  bl[1]=0;
  bu[1]=40-0.0001;
  scale[1]=1;
  bl[2]=0;
  bu[2]=10-0.0001;
  scale[2]=1;
  bl[3]=0;
  bu[3]=40-0.0001;
  scale[3]=1;  

  it=ga(4, scale,x, bl,
  bu,"ga_seed","ga_input",objective_function,penalty_function);

  return 0; 
}


*/
