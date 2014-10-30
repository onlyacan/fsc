#include "main.h"
#include "mesh.h"
#include "field.h"

/* globle values*/
mesh *m;
const real xlen = 0.3;
const real ylen = 0.3;
const id   nx=3, ny=3;


extern id    nlines;
extern elem *lines;



int main(int argc, char *agrv[])
{

  real vol;
  id i;
  pt p0, p1;
  int ic;
  vec S; 


  printf("fs....\n");
  //block_mesh(xlen, ylen, nx, ny, 1.0); 

  printf("input:%s\n", agrv[1]);
  mesh *m = read_gmsh(agrv[1]);
  mesh_print(stdout, m);
  /* real *fx = malloc(sizeof(real) * m->nf); */
  /* calc_cds_fx(m, fx); */


  /* for(i = 0; i < m-> nf; i++) */
  /*     fprintf(stdout, "fx[%lli] = %lf\n", i, fx[i]); */


  for(i = 0; i < m-> nc; i++)
      vol += m-> cs[i].vol;

  fprintf(stdout, "total volume:%lf\n", vol);


  // output the plot file 
  // 1. points
  FILE *fp;
  char fn_points[] = "points";
  char fn_lines[] =  "lines";
  char fn_normals[] = "normals";
  char fn_centers[] = "centers";
  char fn_boundaries[] = "boundaries";
  

  //------------------------------------------------------

  fp = fopen(fn_points, "w");
  for(i = 0; i < m-> np; i ++){
      fprintf(fp, "%12e  %12e\n", m->ps[i][0], m->ps[i][1]);
  }
  fclose(fp);


  //------------------------------------------------------

  fp = fopen(fn_centers, "w");
  for(ic = 0; ic < m-> nc; ic ++){
      fprintf(fp, "%12e  %12e\n", m->cs[ic].ct[0], m->cs[ic].ct[1]);
  }
  fclose(fp);


  //------------------------------------------------------

  fp = fopen(fn_lines, "w");
  
  for(ic = 0; ic < m-> nc; ic ++){
      for(i = 0; i < m-> cs[ic].nsf; i ++){
          p0 = m->cs[ic].sfs[i]->ps[0];
          p1 = m->cs[ic].sfs[i]->ps[1];
          fprintf(fp, "%12e  %12e  %12e  %12e\n", p0[0], p0[1], p1[0]-p0[0], p1[1]-p0[1]);

      }
      

  }


  fclose(fp);


  //------------------------------------------------------

  fp = fopen(fn_boundaries, "w");
  
  for(i = 0; i < m -> nbf; i++){
      p0 = m->ps[(lines+i)-> nds[0]-1];
      p1 = m->ps[(lines+i)-> nds[1]-1];
      fprintf(fp, "%12e  %12e  %12e  %12e\n", p0[0], p0[1], p1[0]-p0[0], p1[1]-p0[1]);
      
  }
      
  fclose(fp);


  //------------------------------------------------------

  fp = fopen(fn_normals, "w");
  
  for(ic = 0; ic < m-> nc; ic ++){
      for(i = 0; i < m-> cs[ic].nsf; i ++){
          p0 = m->cs[ic].sfs[i]->ps[0];
          p1 = m->cs[ic].sfs[i]->ps[1];
          S = m->cs[ic].sfs[i]->S;
          fprintf(fp, "%12e  %12e  %12e  %12e\n", 0.5 *(p0[0] + p1[0]), 0.5 *(p0[1]+ p1[1]), S[0], S[1]);

      }
      

  }

  fclose(fp);


  system("gnuplot plot_mesh.plt");
  return 0;
}




  

