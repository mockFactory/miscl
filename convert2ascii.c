#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nrutil.h"

int main(int argc, char **argv)
{
  FILE *fp2, *fp;
  int nhalo, nfof=0, npbh, i, j;
  float *xxh, *yxh, *zxh, *vxh, *vyh, *vzh, *hmass, *density, frac=1, *positions;
  float OMEGA_M, BOX_SIZE, RESOLUTION, vfac, MASS_PER_PARTICLE, REDSHIFT, RHO_CRIT=2.775e11;

  if(atoi(argv[1])==2) goto FOF_HALOS;

  fprintf(stderr,"Opening [%s]\n",argv[2]);
  fp2=fopen(argv[2],"r");
  // get that number of halos
  fread(&nhalo,sizeof(int),1,fp2);
  npbh = nhalo;
  fprintf(stderr,"Number of halos: %d %f\n",nhalo,sizeof(float)*nhalo*7.0/1024./1024.);

  // get total number of halos and allocate memory
  hmass = vector(1,nhalo);
  xxh = vector(1,nhalo);
  yxh = vector(1,nhalo);
  zxh = vector(1,nhalo);
  vxh = vector(1,nhalo);
  vyh = vector(1,nhalo);
  vzh = vector(1,nhalo);
  density = vector(1,nhalo);

  fprintf(stderr,"Reading in the PB-halos...\n");
  fread(&hmass[1+nfof],sizeof(float),npbh,fp2);
  fread(&xxh[1+nfof],sizeof(float),npbh,fp2);
  fread(&yxh[1+nfof],sizeof(float),npbh,fp2);
  fread(&zxh[1+nfof],sizeof(float),npbh,fp2);
  fread(&vxh[1+nfof],sizeof(float),npbh,fp2);
  fread(&vyh[1+nfof],sizeof(float),npbh,fp2);
  fread(&vzh[1+nfof],sizeof(float),npbh,fp2);
  fread(&density[1+nfof],sizeof(float),npbh,fp2);
  fclose(fp2);

  if(argc>3)
    frac = atof(argv[3]);

  for(i=1;i<=nhalo;++i)
    if(drand48()<frac)
      printf("%e %e %e %e %e %e %e %e\n",
	     hmass[i],xxh[i],yxh[i],zxh[i],vxh[i],vyh[i],vzh[i],density[i]);

  exit(0);

 FOF_HALOS:

  if(argc>3)
    frac = atof(argv[3]);


  // the fof halo file
  fprintf(stderr,"Opening [%s]\n",argv[2]);
  fp=fopen(argv[2],"r");
  // get number of fofhalos
  fread(&nfof,sizeof(int),1,fp);
  positions = vector(0,3*nfof-1);
  nhalo = nfof;
  hmass = vector(1,nhalo);
  xxh = vector(1,nhalo);
  yxh = vector(1,nhalo);
  zxh = vector(1,nhalo);
  vxh = vector(1,nhalo);
  vyh = vector(1,nhalo);
  vzh = vector(1,nhalo);


  BOX_SIZE = atof(argv[4]);
  OMEGA_M = atof(argv[5]);
  RESOLUTION = atof(argv[6]);
  REDSHIFT = atof(argv[6]);

  // read in FOF halos
  fprintf(stderr,"Reading in the FOF halos...\n");
  vfac = BOX_SIZE*sqrt(OMEGA_M*pow(1+REDSHIFT,3.0)+(1-OMEGA_M))*100/(1+REDSHIFT);
  MASS_PER_PARTICLE = OMEGA_M*RHO_CRIT*pow(RESOLUTION,3.0);
  fprintf(stderr,"FOF Mass per particle: %e\n",MASS_PER_PARTICLE);
  fread(&hmass[1],sizeof(float),nfof,fp);
  for(i=1;i<=nfof;++i)
    hmass[i] = hmass[i]*MASS_PER_PARTICLE;

  fread(positions,sizeof(float),3*nfof,fp);
  j = -1;
  for(i=1;i<=nfof;++i)
    {
      xxh[i] =positions[++j]*BOX_SIZE;
      yxh[i] =positions[++j]*BOX_SIZE;
      zxh[i] =positions[++j]*BOX_SIZE;
    }
  fread(positions,sizeof(float),3*nfof,fp);
  j = -1;
  for(i=1;i<=nfof;++i)
    {
      vxh[i] =positions[++j]*vfac;
      vyh[i] =positions[++j]*vfac;
      vzh[i] =positions[++j]*vfac;
    }
  free_vector(positions,0,3*nfof-1);

  for(i=1;i<=nfof;++i)
    if(drand48()<frac)
      printf("%e %e %e %e %e %e %e\n",
	     hmass[i],xxh[i],yxh[i],zxh[i],vxh[i],vyh[i],vzh[i]);
  
 
}
