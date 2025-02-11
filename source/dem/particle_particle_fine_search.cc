#include <dem/particle_particle_fine_search.h>

using namespace dealii;

template <int dim>
ParticleParticleFineSearch<dim>::ParticleParticleFineSearch()
{}

template <int dim>
void
ParticleParticleFineSearch<dim>::particle_particle_fine_search(
  const typename dem_data_containers::dem_data_structures<
    dim>::particle_particle_candidates &local_contact_pair_candidates,
  const typename dem_data_containers::dem_data_structures<
    dim>::particle_particle_candidates &ghost_contact_pair_candidates,
  typename dem_data_containers::dem_data_structures<
    dim>::adjacent_particle_pairs &local_adjacent_particles,
  typename dem_data_containers::dem_data_structures<
    dim>::adjacent_particle_pairs &ghost_adjacent_particles,
  typename dem_data_containers::dem_data_structures<
    dim>::particle_index_iterator_map &particle_container,
  const double                         neighborhood_threshold)
{
  // First iterating over local adjacent_particles
  for (auto &&adjacent_particles_list :
       local_adjacent_particles | boost::adaptors::map_values)

    {
      // Iterating over each map which contains the contact information
      // including particles I and II
      for (auto adjacent_particles_list_iterator =
             adjacent_particles_list.begin();
           adjacent_particles_list_iterator != adjacent_particles_list.end();)
        {
          // Getting contact information and particles I and II as local
          // variables
          auto adjacent_pair_information =
            adjacent_particles_list_iterator->second;
          auto particle_one = adjacent_pair_information.particle_one;
          auto particle_two = adjacent_pair_information.particle_two;

          // Finding the properties of the particles in contact
          Point<dim, double> particle_one_location =
            particle_one->get_location();
          Point<dim, double> particle_two_location =
            particle_two->get_location();

          // Finding distance
          const double square_distance =
            particle_one_location.distance_square(particle_two_location);
          if (square_distance > neighborhood_threshold)
            {
              adjacent_particles_list.erase(adjacent_particles_list_iterator++);
            }
          else
            {
              ++adjacent_particles_list_iterator;
            }
        }
    }

  // Now iterating over local_contact_pair_candidates (maps of pairs), which
  // is the output of broad search. If a pair is in vicinity (distance <
  // threshold), it is added to the local_adjacent_particles
  for (auto const &[particle_one_id, second_particle_container] :
       local_contact_pair_candidates)
    {
      if (!second_particle_container.empty())
        {
          auto               particle_one = particle_container[particle_one_id];
          Point<dim, double> particle_one_location =
            particle_one->get_location();

          for (const unsigned int &particle_two_id : second_particle_container)
            {
              auto particle_two = particle_container[particle_two_id];
              Point<dim, double> particle_two_location =
                particle_two->get_location();

              // Finding distance
              const double square_distance =
                particle_one_location.distance_square(particle_two_location);

              // If the particles distance is less than the threshold
              if (square_distance < neighborhood_threshold)
                {
                  // Getting the particle one contact list and particle two id
                  auto particle_one_contact_list =
                    &local_adjacent_particles[particle_one_id];

                  // Adding the elements to contact info
                  particle_particle_contact_info_struct<dim> contact_info;
                  contact_info.tangential_overlap = 0;
                  contact_info.particle_one       = particle_one;
                  contact_info.particle_two       = particle_two;

                  particle_one_contact_list->insert(
                    {particle_two_id, contact_info});
                }
            }
        }
    }

  // Second iterating over local-ghost adjacent_particles
  for (auto &&adjacent_particles_list :
       ghost_adjacent_particles | boost::adaptors::map_values)
    {
      // Iterating over each map which contains the contact information
      // including particles I and II
      for (auto adjacent_particles_list_iterator =
             adjacent_particles_list.begin();
           adjacent_particles_list_iterator != adjacent_particles_list.end();)
        {
          // Getting contact information and particles I and II as local
          // variables
          auto adjacent_pair_information =
            adjacent_particles_list_iterator->second;
          auto particle_one = adjacent_pair_information.particle_one;
          auto particle_two = adjacent_pair_information.particle_two;

          // Finding the properties of the particles in contact
          Point<dim, double> particle_one_location =
            particle_one->get_location();
          Point<dim, double> particle_two_location =
            particle_two->get_location();

          // Finding distance
          const double square_distance =
            particle_one_location.distance_square(particle_two_location);
          if (square_distance > neighborhood_threshold)
            {
              adjacent_particles_list.erase(adjacent_particles_list_iterator++);
            }
          else
            {
              ++adjacent_particles_list_iterator;
            }
        }
    }

  // Now iterating over ghost_contact_pair_candidates (map of pairs), which
  // is the output of broad search. If a pair is in vicinity (distance <
  // threshold), it is added to the ghost_adjacent_particles
  for (auto const &[particle_one_id, second_particle_container] :
       ghost_contact_pair_candidates)
    {
      if (!second_particle_container.empty())
        {
          auto               particle_one = particle_container[particle_one_id];
          Point<dim, double> particle_one_location =
            particle_one->get_location();

          for (const unsigned int &particle_two_id : second_particle_container)
            {
              auto particle_two = particle_container[particle_two_id];
              Point<dim, double> particle_two_location =
                particle_two->get_location();

              // Finding distance
              const double square_distance =
                particle_one_location.distance_square(particle_two_location);

              // If the particles distance is less than the threshold
              if (square_distance < neighborhood_threshold)
                {
                  // Getting the particle one contact list and particle two id
                  auto particle_one_contact_list =
                    &ghost_adjacent_particles[particle_one_id];

                  // Adding elements to contact info
                  particle_particle_contact_info_struct<dim> contact_info;
                  contact_info.tangential_overlap = 0;
                  contact_info.particle_one       = particle_one;
                  contact_info.particle_two       = particle_two;

                  particle_one_contact_list->insert(
                    {particle_two_id, contact_info});
                }
            }
        }
    }
}

template class ParticleParticleFineSearch<2>;
template class ParticleParticleFineSearch<3>;
