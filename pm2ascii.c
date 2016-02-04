int main(int argc, char **argv)
{
  int ichunk, nchunk, nchunk_reads, nread, ntotread=0;
  int ii, i, j;

  // Gadget stuff
  float *xp, *yp, *zp, *vxp, *vyp, *vzp, *mvect, mlim, *density;
  int np, npcheck, *iii;
  char filename[1000], fname[1000];
  float BOX_SIZE=2560, subfrac = 0.01;

  //TPM input
  struct tpmhdr1 {
    int npart;
    int nsph;
    int nstar;
    float aa;
    float softlen; 
  } tpmhdr;
  float *positions, vfac;

  fp = fopen(argv[1],"r");
  if(argc>2)
    BOX_SIZE = atof(argv[2]);
  if(argc>3)
    subfrac = atof(argv[3]);

  // get header
  fread(&idum,sizeof(int),1,fp);
  fread(&idum,sizeof(int),1,fp);
  fread(&tpmhdr,1,idum,fp);
  np = tpmhdr.npart;
  printf("particles in TPM file: %d %d\n",tpmhdr.npart,np);
  printf("aexp: %f\n",tpmhdr.aa);
  fflush(stdout);
  xp = vector(1,np);
  yp = vector(1,np);
  zp = vector(1,np);
  density = vector(1,np);
  printf("done allocation.\n");
  fflush(stdout);

  // read in the data in 10 discrete chunks
  nchunk = np/10;
  positions = malloc(((long)nchunk)*3*(long)sizeof(float));
  printf("done allocation of temp array. nchunk= %d %d\n",nchunk,np);
  fflush(stdout);

  nchunk_reads = np/nchunk + 1;
  if(np%nchunk==0)nchunk_reads--;

  ii = 1;
  for(ichunk=1;ichunk<=nchunk_reads;++ichunk)
    {
      nread = nchunk;
      if(ntotread + nread>np) nread = np-ntotread;
      fprintf(stderr,"%d %d %d %d %d\n",ichunk,ii,nread,ntotread,np);
      fread(positions,(long)sizeof(float),3*(long)nread,fp);
      
      j = -1;
      for(i=1;i<=nread;++i)
	{
	  xp[ii] =positions[++j]*BOX_SIZE;
	  yp[ii] =positions[++j]*BOX_SIZE;
	  zp[ii] =positions[++j]*BOX_SIZE;
	  ii++;
	}
      ntotread += nread;
    }
  fprintf(stderr,"Read %d positions\n",ntotread);


}
