/* ---------------------------------------------------------------------
 *
 * Copyright (C) 2019 - 2020 by the Lethe authors
 *
 * This file is part of the Lethe library
 *
 * The Lethe library is free software; you can use it, redistribute
 * it, and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * The full text of the license can be found in the file LICENSE at
 * the top level of the Lethe distribution.
 *
 * ---------------------------------------------------------------------

 *
 * Author: Shahab Golshan, Polytechnique Montreal, 2019
 */

#include <dem/data_containers.h>
#include <dem/particle_point_line_contact_info_struct.h>
#include <dem/update_ghost_particle_particle_contact_container.h>
#include <dem/update_local_particle_particle_contact_container.h>
#include <dem/update_particle_container.h>
#include <dem/update_particle_point_line_contact_container.h>
#include <dem/update_particle_wall_contact_container.h>

using namespace dealii;

#ifndef locate_local_particles_h
#  define locate_local_particles_h

/**
 * Updates the iterators to particles in local-local contact containers. This is
 * essential since sort_particles_into_subdomains_and_cells() and
 * exchange_ghost_particles() functions change the iterator to particles
 * everytime they are called.
 *
 * @param particle_handler
 * @param particle_container A container that contains the updated iterators to
 * all local and ghost particles
 * @param ghost_adjacent_particles Container that contains all the contact
 * information of adjacent local-ghost particles
 * @param local_adjacent_particles Container that contains all the contact
 * information of adjacent local-local particles
 * @param particle_wall_pairs_in_contact Container that contains all the contact
 * information of particle-wall contacts
 * @param particle_floating_wall_pairs_in_contact Container that contains all the contact
 * information of particle-floating wall contacts
 * @param particle_floating_mesh_pairs_in_contact Container that contains all the contact
 * information of particle-floating mesh contacts
 * @param particle_points_in_contact Container that contains all the contact
 * information of particle-point contacts
 * @param particle_lines_in_contact Container that contains all the contact
 * information of particle-line contacts
 *
 */

template <int dim>
void
locate_local_particles_in_cells(
  const Particles::ParticleHandler<dim> &particle_handler,
  typename dem_data_containers::dem_data_structures<
    dim>::particle_index_iterator_map &particle_container,
  typename dem_data_containers::dem_data_structures<
    dim>::adjacent_particle_pairs &ghost_adjacent_particles,
  typename dem_data_containers::dem_data_structures<
    dim>::adjacent_particle_pairs &local_adjacent_particles,
  typename dem_data_containers::dem_data_structures<
    dim>::particle_wall_in_contact &particle_wall_pairs_in_contact,
  typename dem_data_containers::dem_data_structures<
    dim>::particle_wall_in_contact &particle_floating_wall_pairs_in_contact,
  typename dem_data_containers::dem_data_structures<dim>::
    particle_floating_mesh_in_contact &particle_floating_mesh_pairs_in_contact,
  typename dem_data_containers::dem_data_structures<
    dim>::particle_point_line_contact_info &particle_points_in_contact,
  typename dem_data_containers::dem_data_structures<
    dim>::particle_point_line_contact_info &particle_lines_in_contact);

#endif /* locate_local_particles_h */
