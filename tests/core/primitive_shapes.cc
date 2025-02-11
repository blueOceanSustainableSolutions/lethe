/**
 * @brief Tests the shape representation.
 *
 */

// Lethe
#include <core/shape.h>

// Tests (with common definitions)
#include <../tests/tests.h>

void
test()
{
  deallog << "Beginning" << std::endl;

  // Solid parameters
  double                    radius    = 2;
  double                    thickness = 0.2;
  Tensor<1, 3>              half_lengths({1., 1., 3.});
  double                    tan_theta = 2.;
  double                    height    = 1.5;
  Point<3>                  position({0., 0., 0.});
  Tensor<1, 3>              orientation({0., 0., 0.});
  std::shared_ptr<Shape<3>> sphere =
    std::make_shared<Sphere<3>>(radius, position, orientation);
  std::shared_ptr<Shape<3>> rectangle =
    std::make_shared<Rectangle<3>>(half_lengths, position, orientation);
  std::shared_ptr<Shape<3>> ellipsoid =
    std::make_shared<Ellipsoid<3>>(half_lengths, position, orientation);
  std::shared_ptr<Shape<3>> torus =
    std::make_shared<Torus<3>>(radius, thickness, position, orientation);
  std::shared_ptr<Shape<3>> cone =
    std::make_shared<Cone<3>>(tan_theta, height, position, orientation);
  std::shared_ptr<Shape<3>> cut_sphere = std::make_shared<CutHollowSphere<3>>(
    radius, height, thickness, position, orientation);
  std::shared_ptr<Shape<3>> death_star = std::make_shared<DeathStar<3>>(
    radius, radius, thickness, position, orientation);


  deallog << "Testing value" << std::endl;
  // Testing value of all shape, to confirm proper implementation
  Point<3> p({1., 0.8, 0.75});
  deallog << " Sphere , SD = " << sphere->value(p) << std::endl;
  deallog << " Rectangle , SD = " << rectangle->value(p) << std::endl;
  deallog << " Ellipsoid , SD = " << ellipsoid->value(p) << std::endl;
  deallog << " Torus , SD = " << torus->value(p) << std::endl;
  deallog << " Cone , SD = " << cone->value(p) << std::endl;
  deallog << " Cut Hollow Sphere , SD = " << cut_sphere->value(p) << std::endl;
  deallog << " Death Star , SD = " << death_star->value(p) << std::endl;
  deallog << "OK" << std::endl;

  deallog << "Testing rotation and translation" << std::endl;
  // Only on one shape: rectangle
  Tensor<1, 3> rotation({1., 0.5, 0.});
  Point<3>     translation({0.2, 0., 0.3});
  rectangle->set_orientation(rotation);
  rectangle->set_position(translation);
  deallog << " New distance for rectangle , SD = " << rectangle->value(p)
          << std::endl;
  deallog << "OK" << std::endl;

  deallog << "Testing gradient" << std::endl;
  Tensor<1, 3> gradient_sphere    = sphere->gradient(p);
  Tensor<1, 3> gradient_ellipsoid = ellipsoid->gradient(p);
  deallog << " Gradient for sphere at p[0] = " << gradient_sphere[0]
          << std::endl;
  deallog << " Gradient for sphere at p[1] = " << gradient_sphere[1]
          << std::endl;
  deallog << " Gradient for sphere at p[2] = " << gradient_sphere[2]
          << std::endl;
  deallog << " Gradient for ellipsoid at p[0] = " << gradient_ellipsoid[0]
          << std::endl;
  deallog << " Gradient for ellipsoid at p[1] = " << gradient_ellipsoid[1]
          << std::endl;
  deallog << " Gradient for ellipsoid at p[2] = " << gradient_ellipsoid[2]
          << std::endl;
  deallog << "OK" << std::endl;

  deallog << "Testing copy" << std::endl;
  std::shared_ptr<Shape<3>> copy_cone = cone->static_copy();
  copy_cone->set_position(translation);
  deallog << " Position of original cone[0] = " << cone->get_position()[0]
          << std::endl;
  deallog << " Position of original cone[1] = " << cone->get_position()[1]
          << std::endl;
  deallog << " Position of original cone[2] = " << cone->get_position()[2]
          << std::endl;
  deallog << " Translation of copy cone" << std::endl;
  deallog << " Position of copy cone[0] = " << copy_cone->get_position()[0]
          << std::endl;
  deallog << " Position of copy cone[1] = " << copy_cone->get_position()[1]
          << std::endl;
  deallog << " Position of copy cone[2] = " << copy_cone->get_position()[2]
          << std::endl;
  deallog << "OK" << std::endl;
}

int
main()
{
  try
    {
      initlog();
      test();
    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    }
}
