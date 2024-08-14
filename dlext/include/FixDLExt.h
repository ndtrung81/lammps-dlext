// SPDX-License-Identifier: MIT
// This file is part of `lammps-dlext`, see LICENSE.md

#ifndef DLEXT_SAMPLER_H_
#define DLEXT_SAMPLER_H_

#include "dlpack/dlpack.h"
#include "fix.h"
#include <functional>

namespace LAMMPS_NS
{
namespace dlext
{

// { // Aliases
const auto kOnHost = ExecutionSpace::Host;
const auto kOnDevice = ExecutionSpace::Device;

using TimeStep = bigint;  // bigint depends on how LAMMPS was built
using DLExtCallback = std::function<void(TimeStep)>;
using DLExtSetVirial = std::function<void(double*)>;

// } // Aliases

//!
//!  FixDLExt is essentially a LAMMPS fix that allows an external callback
//!  to access and or modify the atom information. The callback interface
//!  allows for complete flexibility on what code to execute during its call,
//!  so it's advised to use it with caution.
//!
//!  NOTE: A closely related example is the existing FixExternal in LAMMPS
//!    (docs.lammps.org/fix_external.html)
//!
class DEFAULT_VISIBILITY FixDLExt : public Fix {
public:
    //! Constructor
    FixDLExt(LAMMPS* lmp, int narg, char** arg);

    int setmask() override;
    void post_force(int) override;
    void set_callback(DLExtCallback& cb);
    void set_virial_callback(DLExtSetVirial& cb);
    void set_virial_global(int flag) { virial_global_flag = flag; }

    // Provide easy access to the atom pointers
    Atom* atom_ptr() const;
    AtomKokkos* atom_kokkos_ptr() const;

    //! Given an execution space, returns kDLCUDA if LAMMPS was built with KOKKOS and
    //! Cuda supoprt, and it's available at runtime. Otherwise, returns kDLCPU.
    DLDeviceType device_type(ExecutionSpace requested_space) const;

    //! The device id where this class instances are being executed
    int device_id() const;

    // Convenience methods for retriving the number of particles
    int local_particle_number() const;
    bigint global_particle_number() const;

    //! If KOKKOS is available, synchronize on the particle data on the requested space
    void synchronize(ExecutionSpace requested_space = kOnDevice);

private:
    DLExtCallback callback = [](TimeStep) { };
    DLExtSetVirial setVirial = [](double*) { };
    ExecutionSpace try_pick(ExecutionSpace requested_space) const;
};

inline bool has_kokkos_cuda_enabled(LAMMPS* lmp)
{
    bool has_cuda = strcmp(LMPDeviceType::name(), "Cuda") == 0;
    return has_cuda & (lmp->kokkos != nullptr);
}

void register_FixDLExt(LAMMPS* lmp);

}  // namespace dlext
}  // namespace LAMMPS_NS

#endif  // DLEXT_SAMPLER_H_
