#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nrutil.h"

int main(int argc, char **argv)
{
  FILE *fp2, *fp;
  int nhalo, nfof=0, npbh, i, j, nhalotot=0, ncur=0, nfiles;
  float *xxh, *yxh, *zxh, *vxh, *vyh, *vzh, *hmass, *density, frac=1, *positions;
  float OMEGA_M, BOX_SIZE, RESOLUTION, vfac, MASS_PER_PARTICLE, REDSHIFT, RHO_CRIT=2.775e11;
  char fname[1000];

  nfiles = atoi(argv[2]);

  for(j=0;j<nfiles;++j)
    {
      sprintf(fname,"%s.%02d.HaloFile",argv[1],j);
      fprintf(stderr,"Opening [%s]\n",fname);
      fp2=fopen(fname,"r");
      // get that number of halos
      fread(&nhalo,sizeof(int),1,fp2);
      fprintf(stderr,"%02d> %d\n",j,nhalo);
      nhalotot += nhalo;
      fclose(fp2);
    }

  fprintf(stderr,"Number of halos: %d %f\n",nhalotot,sizeof(float)*nhalotot*7.0/1024./1024.);

  // get total number of halos and allocate memory
  nhalo = nhalotot;
  hmass = vector(1,nhalo);
  xxh = vector(1,nhalo);
  yxh = vector(1,nhalo);
  zxh = vector(1,nhalo);
  vxh = vector(1,nhalo);
  vyh = vector(1,nhalo);
  vzh = vector(1,nhalo);
  density = vector(1,nhalo);

  for(j=0;j<nfiles;++j)
    {
      sprintf(fname,"%s.%02d.HaloFile",argv[1],j);
      fprintf(stderr,"Opening [%s]\n",fname);
      fp2=fopen(fname,"r");
      // get that number of halos
      fread(&npbh,sizeof(int),1,fp2);

      fprintf(stderr,"Reading in the PB-halos...\n");
      fread(&hmass[1+ncur],sizeof(float),npbh,fp2);
      fread(&xxh[1+ncur],sizeof(float),npbh,fp2);
      fread(&yxh[1+ncur],sizeof(float),npbh,fp2);
      fread(&zxh[1+ncur],sizeof(float),npbh,fp2);
      fread(&vxh[1+ncur],sizeof(float),npbh,fp2);
      fread(&vyh[1+ncur],sizeof(float),npbh,fp2);
      fread(&vzh[1+ncur],sizeof(float),npbh,fp2);
      fread(&density[1+ncur],sizeof(float),npbh,fp2);
      ncur += npbh;
      fclose(fp2);
    }

  nhalo = nhalotot;

  // now we need to remove pairs at the junctions
  // screw it. it's such a small fraction of pairs.

  sprintf(fname,"%s.HaloFile",argv[1]);
  fp2 = fopen(fname,"w");

  fwrite(&nhalotot,sizeof(int),1,fp2);
  fwrite(hmass,sizeof(float),nhalo,fp2);
  fwrite(xxh,sizeof(float),nhalo,fp2);
  fwrite(yxh,sizeof(float),nhalo,fp2);
  fwrite(zxh,sizeof(float),nhalo,fp2);
  fwrite(vxh,sizeof(float),nhalo,fp2);
  fwrite(vyh,sizeof(float),nhalo,fp2);
  fwrite(vzh,sizeof(float),nhalo,fp2);
  fwrite(density,sizeof(float),nhalo,fp2);
  fclose(fp2);

 
}

