#include <rpt/parameters_rpt.h>

#include <ctime>
#include <sstream>

void
Parameters::RPTParameters::declare_parameters(ParameterHandler &prm)
{
  prm.enter_subsection("rpt parameters");
  {
    prm.declare_entry("particle positions file",
                      "none",
                      Patterns::FileName(),
                      "Particle positions filename");

    prm.declare_entry("verbosity",
                      "quiet",
                      Patterns::Selection("quiet|verbose"),
                      "Enable to show results during computation "
                      "Choices are <quiet|verbose>.");

    prm.declare_entry(
      "export counts",
      "false",
      Patterns::Bool(),
      "Enable to export counts result in a .csv file <true|false>");

    prm.declare_entry("counts file",
                      "counts",
                      Patterns::FileName(),
                      "Exported count's results filename");

    prm.declare_entry("monte carlo iteration",
                      "1",
                      Patterns::Integer(),
                      "Number of Monte Carlo iteration");

    prm.declare_entry("random number seed",
                      "auto",
                      Patterns::Anything(),
                      "Seed of the random number generator <auto|number>");

    prm.declare_entry("reactor height",
                      "0.1",
                      Patterns::Double(),
                      "Height of the reactor or tank");

    prm.declare_entry("reactor radius",
                      "0.1",
                      Patterns::Double(),
                      "Radius of the reactor or tank");

    prm.declare_entry("peak-to-total ratio",
                      "1",
                      Patterns::Double(),
                      "Peak-to-total ratio");

    prm.declare_entry("sampling time",
                      "1",
                      Patterns::Double(),
                      "Sampling time of the counting");

    prm.declare_entry("gamma-rays emitted",
                      "1",
                      Patterns::Double(),
                      "Number of gamma-rays emitted by each disintegration");


    prm.declare_entry("attenuation coefficient detector",
                      "1",
                      Patterns::Double(),
                      "Total linear attenuation coefficient of the detector");
  }
  prm.leave_subsection();
}

void
Parameters::RPTParameters::parse_parameters(ParameterHandler &prm)
{
  prm.enter_subsection("rpt parameters");
  {
    particle_positions_file = prm.get("particle positions file");
    export_counts           = prm.get_bool("export counts");
    export_counts_file      = prm.get("counts file");
    n_monte_carlo_iteration = prm.get_integer("monte carlo iteration");
    reactor_height          = prm.get_double("reactor height");
    reactor_radius          = prm.get_double("reactor radius");
    peak_to_total_ratio     = prm.get_double("peak-to-total ratio");
    sampling_time           = prm.get_double("sampling time");
    gamma_rays_emitted      = prm.get_double("gamma-rays emitted");
    attenuation_coefficient_detector =
      prm.get_double("attenuation coefficient detector");

    const std::string op = prm.get("verbosity");
    if (op == "verbose")
      verbosity = Verbosity::verbose;
    if (op == "quiet")
      verbosity = Verbosity::quiet;

    seed = (prm.get("random number seed") == "auto") ?
             std::time(nullptr) :
             prm.get_integer("random number seed");
  }
  prm.leave_subsection();
}


void
Parameters::RPTTuningParameters::declare_parameters(ParameterHandler &prm)
{
  prm.enter_subsection("parameter tuning");
  {
    prm.declare_entry("tuning",
                      "false",
                      Patterns::Bool(),
                      "Enable parameter tuning <true|false>");

    prm.declare_entry("cost function type",
                      "larachi",
                      Patterns::Selection("larachi|L1|L2"),
                      "Cost function used for the parameter tuning"
                      "Choices are <larachi|L1|L2>.");

    prm.declare_entry("experimental data file",
                      "none",
                      Patterns::FileName(),
                      "Experimental counts data file name");
  }
  prm.leave_subsection();
}

void
Parameters::RPTTuningParameters::parse_parameters(ParameterHandler &prm)
{
  prm.enter_subsection("parameter tuning");
  {
    tuning            = prm.get_bool("tuning");
    experimental_file = prm.get("experimental data file");

    const std::string type = prm.get("cost function type");
    if (type == "larachi")
      cost_function_type = CostFunctionType::larachi;
    else if (type == "l1")
      cost_function_type = CostFunctionType::l1;
    else if (type == "l2")
      cost_function_type = CostFunctionType::l2;
    else
      throw std::logic_error(
        "Error, invalid cost function type. Choices are larachi, l2 or l1.");

    experimental_file = prm.get("experimental data file");
  }
  prm.leave_subsection();
}

void
Parameters::DetectorParameters::declare_parameters(ParameterHandler &prm)
{
  prm.enter_subsection("detector parameters");
  {
    prm.declare_entry("number", "1", Patterns::Integer(), "Number of detector");

    prm.declare_entry("radius",
                      "1",
                      Patterns::Double(),
                      "Radius of all detectors");

    prm.declare_entry("length",
                      "1",
                      Patterns::Double(),
                      "Length of all detectors");

    prm.declare_entry("detector positions file",
                      "none",
                      Patterns::FileName(),
                      "Detector positions file name");

    prm.declare_entry("dead time",
                      "1",
                      Patterns::List(Patterns::Double()),
                      "Dead time of the detector per accepted pulse");

    prm.declare_entry("activity",
                      "1",
                      Patterns::List(Patterns::Double()),
                      "Activity of the tracer");

    prm.declare_entry("attenuation coefficient reactor",
                      "1",
                      Patterns::List(Patterns::Double()),
                      "Total linear attenuation coefficient of the medium");
  }
  prm.leave_subsection();
}

// Dead time of the detector, activity of the tracer particle and attenuation
// coefficient of the reactor with respect to each detector can be defined in
// detector parameter subsection in the parameter file. Having more than one
// detector, these parameters can be defined as the Following format: (For
// example for dead time and having 3 detectors: Deadtime = 1,1,1)

void
Parameters::DetectorParameters::parse_parameters(ParameterHandler &prm)
{
  prm.enter_subsection("detector parameters");
  {
    radius                  = prm.get_double("radius");
    length                  = prm.get_double("length");
    detector_positions_file = prm.get("detector positions file");

    // Read dead time, activity and attenuation coefficient reactor as a single
    // string
    std::string dead_time_str = prm.get("dead time");
    std::string activity_str  = prm.get("activity");
    std::string attenuation_coefficient_reactor_str =
      prm.get("attenuation coefficient reactor");
    // Convert "dead time, activity and attenuation coefficient reactor" strings
    // to vector of strings
    std::vector<std::string> dead_time_str_list(
      Utilities::split_string_list(dead_time_str));
    std::vector<std::string> activity_str_list(
      Utilities::split_string_list(activity_str));
    std::vector<std::string> attenuation_coefficient_reactor_str_list(
      Utilities::split_string_list(attenuation_coefficient_reactor_str));

    // Convert "dead time, activity and attenuation coefficient reactor" string
    // vector to double vector
    dead_time = Utilities::string_to_double(dead_time_str_list);
    activity  = Utilities::string_to_double(activity_str_list);
    attenuation_coefficient_reactor =
      Utilities::string_to_double(attenuation_coefficient_reactor_str_list);
  }

  prm.leave_subsection();
}

void
Parameters::RPTReconstructionParameters::declare_parameters(
  ParameterHandler &prm)
{
  prm.enter_subsection("reconstruction");
  {
    prm.declare_entry("refinement",
                      "1",
                      Patterns::Integer(),
                      "Number of refinement for the reactor");

    prm.declare_entry(
      "coarse mesh level",
      "0",
      Patterns::Integer(),
      "Level of the coarse mesh where all the counts of the first vertices");

    prm.declare_entry(
      "reconstruction counts file",
      "reconstruction_counts",
      Patterns::FileName(),
      "Counts of every detectors of the unknown particle position filename");

    prm.declare_entry("export reconstruction positions file",
                      "reconstruction_positions",
                      Patterns::FileName(),
                      "Export positions found by reconstruction");

    prm.declare_entry(
      "analyse positions",
      "false",
      Patterns::Bool(),
      "Enable evaluation of results with known positions <true|false>");

    prm.declare_entry("known positions file",
                      "known_positions_file",
                      Patterns::FileName(),
                      "Known positions (artificial or from experimental");

    prm.declare_entry("verbose clock",
                      "false",
                      Patterns::Bool(),
                      "Allow to show total wallclock time elapsed since start");
  }
  prm.leave_subsection();
}

void
Parameters::RPTReconstructionParameters::parse_parameters(ParameterHandler &prm)
{
  prm.enter_subsection("reconstruction");
  {
    reactor_refinement         = prm.get_integer("refinement");
    coarse_mesh_level          = prm.get_integer("coarse mesh level");
    reconstruction_counts_file = prm.get("reconstruction counts file");
    reconstruction_positions_file =
      prm.get("export reconstruction positions file");
    analyse_positions    = prm.get_bool("analyse positions");
    known_positions_file = prm.get("known positions file");
    verbose_clock        = prm.get_bool("verbose clock");

    if (coarse_mesh_level > reactor_refinement)
      throw std::logic_error(
        "Error, the level of the coarse mesh can't be higher that the refinement value (max level).");
  }
  prm.leave_subsection();
}
