#include "coord.h"
#include "neighbor_list.h"
#include "SimulationRegion.h"
#include <vector>

// normalize coords
template <typename FPTYPE>
void
normalize_coord_cpu(
    FPTYPE * coord,
    const int natom,
    const Region<FPTYPE> & region)
{
  for(int ii = 0; ii < natom; ++ii){
    FPTYPE ri[3];
    convert_to_inter_cpu(ri, region, coord+3*ii);
    for(int dd = 0; dd < 3; ++dd){
      while(ri[dd] >= 1.) ri[dd] -= 1.;
      while(ri[dd] <  0.) ri[dd] += 1.;
    }
    convert_to_phys_cpu(coord+3*ii, region, ri);
  }
}


template <typename FPTYPE>
int
copy_coord_cpu(
    FPTYPE * out_c,
    int * out_t,
    int * mapping,
    int * nall,
    const FPTYPE * in_c,
    const int * in_t,
    const int & nloc,
    const int & mem_nall,
    const float & rcut,
    const Region<FPTYPE> & region)
{
  std::vector<double> coord(nloc * 3);
  std::vector<int> atype(nloc);
  std::copy(in_c, in_c+nloc*3, coord.begin());
  std::copy(in_t, in_t+nloc, atype.begin());
  SimulationRegion<double> tmpr;
  double tmp_boxt[9];
  std::copy(region.boxt, region.boxt+9, tmp_boxt);
  tmpr.reinitBox(tmp_boxt);
  
  std::vector<double > out_coord;
  std::vector<int> out_atype, out_mapping, ncell, ngcell;
  copy_coord(out_coord, out_atype, out_mapping, ncell, ngcell, coord, atype, rcut, tmpr);
  
  *nall = out_atype.size();
  if(*nall > mem_nall){
    // size of the output arrays is not large enough
    return 1;
  }
  else{
    std::copy(out_coord.begin(), out_coord.end(), out_c);
    std::copy(out_atype.begin(), out_atype.end(), out_t);
    std::copy(out_mapping.begin(), out_mapping.end(), mapping);
  }
  return 0;
}


template
void
normalize_coord_cpu<double>(
    double * coord,
    const int natom,
    const Region<double> & region);

template
void
normalize_coord_cpu<float>(
    float * coord,
    const int natom,
    const Region<float> & region);

template
int
copy_coord_cpu<double>(
    double * out_c,
    int * out_t,
    int * mapping,
    int * nall,
    const double * in_c,
    const int * in_t,
    const int & nloc,
    const int & mem_nall,
    const float & rcut,
    const Region<double> & region);

template
int
copy_coord_cpu<float>(
    float * out_c,
    int * out_t,
    int * mapping,
    int * nall,
    const float * in_c,
    const int * in_t,
    const int & nloc,
    const int & mem_nall,
    const float & rcut,
    const Region<float> & region);


