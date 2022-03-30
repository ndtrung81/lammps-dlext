# lammps-dlext

Provide access to LAMMPS simulation data on CPU or GPU through DLPack

## Installation

Install upstream LAMMPS
* Build LAMMPS with KOKKOS and PYTHON packages
* Install LAMMPS into some location
* Copy src/fmt into include the installation folder include/lammps/fmt
* Copy several key headers from src/KOKKOS (e.g. kokkos_type.h, atom_kokkos.h) into include/lammps/KOKKOS

Build LAMMPS dlext (this package):
* Set the same install path as the LAMMPS python module:
  mkdir build && cd build
  cmake ../ -DCMAKE_INSTALL_PATH=/home/ndtrung/miniconda3/envs/pysages/lib/python3.9/site-packages/lammps


