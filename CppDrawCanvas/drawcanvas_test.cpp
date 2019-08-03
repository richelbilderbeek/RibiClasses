#include "drawcanvas.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>

#include <boost/algorithm/string/split.hpp>
#include <boost/geometry.hpp>
#include <boost/math/constants/constants.hpp>

#include <QString>

#include "canvascolorsystems.h"
#include "canvascoordinatsystems.h"
#include "container.h"
#include "dotmatrixstring.h"
#include "fileio.h"
#include "geometry.h"
#include "ribi_regex.h"
#include "xml.h"



void ribi::DrawCanvasTest() noexcept
{
  //Drawing text
  {
    const int maxx = 90;
    const int maxy = 18;
    DrawCanvas canvas(maxx,maxy,CanvasColorSystem::invert);
    std::stringstream s_before;
    s_before << canvas;
    const std::string str_before {s_before.str() };
    assert(static_cast<int>(str_before.size()) - maxy == maxx * maxy); //-maxy, as newlines are added
    assert(std::count(str_before.begin(),str_before.end(),' ') == maxx * maxy); //Only spaces

    canvas.DrawText(1,1,"Hello world");

    std::stringstream s_after;
    s_after << canvas;
    const std::string str_after {s_after.str() };
    assert(std::count(str_after.begin(),str_after.end(),' ') != maxx * maxy); //Line trly drawn
  }
  //Is a line that starts and ends beyond the canvas drawn?
  {
    const int maxx = 3;
    const int maxy = 4;
    DrawCanvas canvas(maxx,maxy,CanvasColorSystem::invert);
    std::stringstream s_before;
    s_before << canvas;
    const std::string str_before {s_before.str() };
    assert(static_cast<int>(str_before.size()) - maxy == maxx * maxy); //-maxy, as newlines are added
    assert(std::count(str_before.begin(),str_before.end(),' ') == maxx * maxy); //Only spaces

    canvas.DrawLine(-maxx,-maxy,maxx*2.0,maxy*2.0);

    std::stringstream s_after;
    s_after << canvas;
    const std::string str_after {s_after.str() };
    assert(std::count(str_after.begin(),str_after.end(),' ') != maxx * maxy); //Line trly drawn
  }
  //Draw a polygon
  {
    /*

    6 +
      |
    5 +      -C
      |     - |
    4 +   --  |
      |  -    |
    3 + B     |
      | |    |
    2 + |    |
      | |   |
    1 + A---D
      |
    0 +-+-+-+-+-+-+

      0 1 2 3 4 5 6
    */
    const int maxx = 22;
    const int maxy = 22;
    DrawCanvas canvas(
      maxx,
      maxy,
      CanvasColorSystem::invert,
      CanvasCoordinatSystem::graph
    );
    const std::vector<Coordinat> points {
      {  4.0,  4.0}, //A
      {  4.0, 12.0}, //B
      { 16.0, 20.0}, //C
      { 12.0,  4.0}  //D
    };
    boost::geometry::model::polygon<Coordinat> polygon;
    boost::geometry::append(polygon,points);
    canvas.DrawPolygon(polygon);
    {
      std::stringstream s;
      s << canvas;
      assert(!s.str().empty());
    }
  }
  //Draw a smiley is all coordinat- and colorsystem combinations
  for (int i=0; i!=4; ++i)
  {
    const int maxx = 79;
    const int maxy = 23;
    DrawCanvas canvas(maxx,maxy);
    canvas.SetColorSystem(
      i % 2
      ? CanvasColorSystem::normal
      : CanvasColorSystem::invert);
    canvas.SetCoordinatSystem(
      i / 2
      ? CanvasCoordinatSystem::screen
      : CanvasCoordinatSystem::graph);

    //Determine and calculate dimensions and coordinats of smiley
    const double maxxD = static_cast<double>(maxx);
    const double maxyD = static_cast<double>(maxy);
    const double midX        = 0.50 * maxxD;
    const double midY        = 0.50 * maxyD;
    const double headRay     = 0.50 * maxyD;
    const double eyeLeftX    = 0.50 * maxxD - (0.35 * headRay) ;
    const double eyeLeftY    = 0.50 * maxyD - (0.25 * headRay) ;
    const double eyeRightX   = 0.50 * maxxD + (0.35 * headRay) ;
    const double eyeRightY   = 0.50 * maxyD - (0.25 * headRay) ;
    const double eyeRay      = 0.30 * headRay;
    const double mouthLeftX  = 0.50 * maxxD - (0.7 * headRay) ;
    const double mouthMidX   = 0.50 * maxxD;
    const double mouthRightX = 0.50 * maxxD + (0.7 * headRay) ;
    const double mouthLeftY  = 0.50 * maxyD + (0.2 * headRay) ;
    const double mouthMidY   = 0.50 * maxyD + (0.7 * headRay) ;
    const double mouthRightY = 0.50 * maxyD + (0.2 * headRay) ;
    //Draw the image on DrawCanvas
    canvas.DrawCircle(midX, midY, headRay);
    canvas.DrawCircle(eyeLeftX, eyeLeftY, eyeRay);
    canvas.DrawDot(eyeLeftX, eyeLeftY);
    canvas.DrawCircle(eyeRightX, eyeRightY, eyeRay);
    canvas.DrawDot(eyeRightX, eyeRightY);
    canvas.DrawLine(mouthLeftX, mouthLeftY, mouthMidX, mouthMidY);
    canvas.DrawLine(mouthMidX, mouthMidY, mouthRightX, mouthRightY);
    canvas.DrawLine(mouthRightX, mouthRightY, mouthLeftX, mouthLeftY);
    {
      std::stringstream s;
      s << canvas;
      assert(!s.str().empty());
    }
    canvas.Clear();
    {
      canvas.SetColorSystem(CanvasColorSystem::invert); //Background = Black
      std::stringstream s;
      s << canvas;
      const std::string t { s.str() };
      assert(std::count(t.begin(),t.end(),' ') == canvas.GetWidth() * canvas.GetHeight());

    }
  }
  //Saving and loading
  {
    const int maxx = 2;
    const int maxy = 3;
    DrawCanvas canvas(maxx,maxy,CanvasColorSystem::invert);
    canvas.DrawLine(-maxx,-maxy,maxx*2.0,maxy*2.0);

    const DrawCanvas old_canvas(
      canvas.GetGreynesses(),
      canvas.GetColorSystem(),
      canvas.GetCoordinatSystem()
    );
    assert(old_canvas == canvas);

    const std::string temp_filename { fileio::FileIo().GetTempFileName() };
    canvas.Save(temp_filename);
    canvas.Clear();

    assert(old_canvas != canvas);

    const DrawCanvas new_canvas(temp_filename);

    assert(old_canvas == new_canvas);

  }
}

